r"""
The VisocyteWeb iPython module is used as a helper to create custom
iPython notebook profile.

The following sample show how the helper class can be used inside
an iPython profile.

# Global python import
import exceptions, logging, random, sys, threading, time, os

# Update python path to have Visocyte libs
pv_path = '/.../Visocyte/build'
sys.path.append('%s/lib' % pv_path)
sys.path.append('%s/lib/site-packages' % pv_path)

# iPython import
from IPython.display import HTML
from IPython.parallel import Client
import visocyte
from visocyte.web import ipython as pv_ipython
from vtk import *

iPythonClient = None
visocyteHelper = pv_ipython.VisocyteIPython()
webArguments = pv_ipython.WebArguments('/.../path-to-web-directory')

def _start_visocyte():
    visocyteHelper.Initialize()
    visocyteHelper.SetWebProtocol(pv_ipython.IPythonProtocol, webArguments)
    return visocyteHelper.Start()


def _stop_visocyte():
    visocyteHelper.Finalize()


def _pv_activate_dataset():
    pv_ipython.IPythonProtocol.ActivateDataSet('iPython-demo')


def _push_new_timestep():
    # processing code generating new vtkDataSet
    # newDataset = ...
    pv_ipython.IPythonProtocol.RegisterDataSet('iPython-demo', newDataset)


def StartVisocyte(height=600, path='/apps/Visualizer/'):
    global iPythonClient, visocyteHelper
    if not iPythonClient:
        iPythonClient = Client(profile='pvw')
    urls = iPythonClient[:].apply_sync(lambda:_start_visocyte())
    url = ""
    for i in urls:
        if len(i) > 0:
            url = i
    return  HTML("<iframe src='%s%s' width='100%%' height='%i'></iframe>"%(url, path, height))


def StopVisocyte():
    global iPythonClient, visocyteHelper
    iPythonClient[:].apply_sync(lambda:_stop_visocyte())


def ActivateDataSet():
    iPythonClient[:].apply_sync(lambda:_pv_activate_dataset())


def ComputeNextTimeStep():
    global iPythonClient
    if not iPythonClient:
        iPythonClient = Client(profile='pvw')
    iPythonClient[:].apply_sync(lambda:_push_new_timestep())

"""

import exceptions, traceback, logging, random, sys, threading, time, os, visocyte

from mpi4py import MPI
from vtkmodules.web import server
from visocyte.vtk import *
from vtkmodules.vtkCommonCore import *
from vtkmodules.vtkCommonDataModel import *
from vtkmodules.vtkCommonExecutionModel import *
from vtkmodules.vtkFiltersSources import *
from vtkmodules.vtkParallelCore import *
from vtkmodules.vtkVisocyteWebCore import *
from vtkmodules.vtkPVClientServerCoreCore import *
from vtkmodules.vtkPVServerManagerApplication import *
from vtkmodules.vtkPVServerManagerCore import *
from vtkmodules.vtkPVVTKExtensionsCore import *
from vtkmodules.vtkWebCore import *
from visocyte.web import wamp as pv_wamp

#------------------------------------------------------------------------------
# Global variables
#------------------------------------------------------------------------------
logger = logging.getLogger()
logger.setLevel(logging.ERROR)

#------------------------------------------------------------------------------
# Global internal methods
#------------------------------------------------------------------------------
def _get_hostname():
    import socket
    if socket.gethostname().find('.')>=0:
        return socket.gethostname()
    else:
        return socket.gethostbyaddr(socket.gethostname())[0]

#------------------------------------------------------------------------------
# Visocyte iPython helper class
#------------------------------------------------------------------------------
class VisocyteIPython(object):
    processModule     = None
    globalController  = None
    localController   = None
    webProtocol       = None
    webArguments      = None
    processId         = -1
    number_of_process = -1

    def Initialize(self, log_file_path = None, logging_level = logging.DEBUG):
        if not VisocyteIPython.processModule:
            vtkInitializationHelper.Initialize("ipython-notebook", 4) # 4 is type of process
            VisocyteIPython.processModule = vtkProcessModule.GetProcessModule()
            VisocyteIPython.globalController = VisocyteIPython.processModule.GetGlobalController()

            if MPI.COMM_WORLD.Get_size() > 1 and (VisocyteIPython.globalController is None or VisocyteIPython.globalController.IsA("vtkDummyController") == True):
                import vtkParallelMPIPython
                VisocyteIPython.globalController = vtkParallelMPIPython.vtkMPIController()
                VisocyteIPython.globalController.Initialize()
                VisocyteIPython.globalController.SetGlobalController(VisocyteIPython.globalController)

            VisocyteIPython.processId = VisocyteIPython.globalController.GetLocalProcessId()
            VisocyteIPython.number_of_process = VisocyteIPython.globalController.GetNumberOfProcesses()
            VisocyteIPython.localController = VisocyteIPython.globalController.PartitionController(VisocyteIPython.number_of_process, VisocyteIPython.processId)

            # must unregister if the reference count is greater than 1
            if VisocyteIPython.localController.GetReferenceCount() > 1:
                VisocyteIPython.localController.UnRegister(None)

            VisocyteIPython.globalController.SetGlobalController(VisocyteIPython.localController)

            if log_file_path:
                formatter = logging.Formatter('%(asctime)s - %(levelname)s - %(message)s')
                fh = logging.FileHandler('%s-%s.txt' % (log_file_path, str(VisocyteIPython.processId)))
                fh.setLevel(logging_level)
                fh.setFormatter(formatter)
                logger.addHandler(fh)
                logger.info("Process %i initialized for Visocyte" % os.getpid())
                logger.info("Sub-Controller: " + str(VisocyteIPython.localController.GetLocalProcessId()) + "/" + str(VisocyteIPython.localController.GetNumberOfProcesses()))
                logger.info("GlobalController: " + str(VisocyteIPython.processId) + "/" + str(VisocyteIPython.number_of_process))
        else:
            logger.info("Visocyte has already been initialized. No operation was performed.")

    def Finalize(self):
        if VisocyteIPython.processModule:
            vtkInitializationHelper.Finalize()
            VisocyteIPython.processModule = None

    def GetProcessId(self):
        return VisocyteIPython.processId

    def GetNumberOfProcesses(self):
        return VisocyteIPython.number_of_process

    def __repr__(self):
        return self.__str__()

    def __str__(self):
        return "Host: %s - Controller: %s - Rank: %d/%d" % (_get_hostname(), VisocyteIPython.localController.GetClassName(), VisocyteIPython.processId, VisocyteIPython.number_of_process)

    def SetWebProtocol(self, protocol, arguments):
        VisocyteIPython.webProtocol = protocol
        VisocyteIPython.webArguments = arguments
        if not hasattr(VisocyteIPython.webArguments, 'port'):
            VisocyteIPython.webArguments.port = 8080
        VisocyteIPython.webProtocol.rootNode = (self.GetProcessId() == 0)
        VisocyteIPython.webProtocol.updateArguments(VisocyteIPython.webArguments)

    @staticmethod
    def _start_satelite():
        logger.info('Visocyte Satelite %d - Started' % VisocyteIPython.processId)
        sid = vtkSMSession.ConnectToSelf();
        vtkWebUtilities.ProcessRMIs()
        VisocyteIPython.processModule.UnRegisterSession(sid);
        logger.info('Visocyte Satelite  %d - Ended' % VisocyteIPython.processId)

    @staticmethod
    def _start_web_server():
        server.start_webserver(options=VisocyteIPython.webArguments, protocol=VisocyteIPython.webProtocol)
        from visocyte import simple
        simple.Disconnect()
        VisocyteIPython.localController.TriggerBreakRMIs()

    @staticmethod
    def debug():
        for i in range(10):
            logger.info('In debug loop ' + str(i))

    def Start(self):
        thread = None
        if self.GetProcessId() == 0:
            thread = threading.Thread(target=VisocyteIPython._start_web_server)
            thread.start()
            time.sleep(10)
            logger.info("WebServer thread started")
            return "http://%s:%d" % (_get_hostname(), VisocyteIPython.webArguments.port)
        else:
            thread = threading.Thread(target=VisocyteIPython._start_satelite)
            thread.start()
            logger.info("Satelite thread started")
            return ""

#------------------------------------------------------------------------------
# Visocyte iPython protocol
#------------------------------------------------------------------------------

class IPythonProtocol(pv_wamp.PVServerProtocol):
    rootNode       = False
    dataDir        = None
    authKey        = "vtkweb-secret"
    fileToLoad     = None
    producer       = None
    groupRegex     = "[0-9]+\\."
    excludeRegex   = "^\\.|~$|^\\$"

    @staticmethod
    def ActivateDataSet(key):
        if IPythonProtocol.rootNode and IPythonProtocol.producer:
            IPythonProtocol.producer.UpdateDataset = ''
            IPythonProtocol.producer.UpdateDataset = key

    @staticmethod
    def RegisterDataSet(key, dataset):
        vtkDistributedTrivialProducer.SetGlobalOutput(key, dataset)

    @staticmethod
    def updateArguments(options):
        IPythonProtocol.dataDir      = options.dataDir
        IPythonProtocol.authKey      = options.authKey
        IPythonProtocol.fileToLoad   = options.fileToLoad
        IPythonProtocol.authKey      = options.authKey
        IPythonProtocol.groupRegex   = options.groupRegex
        IPythonProtocol.excludeRegex = options.excludeRegex

    def initialize(self):
        from visocyte import simple
        from visocyte.web import protocols as pv_protocols

        # Make sure Visocyte is initialized
        if not simple.servermanager.ActiveConnection:
            simple.Connect()

        if not IPythonProtocol.producer:
            IPythonProtocol.producer = simple.DistributedTrivialProducer()
            IPythonProtocol.ActivateDataSet('iPython-demo')
            simple.Show(IPythonProtocol.producer)
            simple.Render()

        # Bring used components
        self.registerVtkWebProtocol(pv_protocols.VisocyteWebFileListing(IPythonProtocol.dataDir, "Home", IPythonProtocol.excludeRegex, IPythonProtocol.groupRegex))
        self.registerVtkWebProtocol(pv_protocols.VisocyteWebPipelineManager(IPythonProtocol.dataDir, IPythonProtocol.fileToLoad))
        self.registerVtkWebProtocol(pv_protocols.VisocyteWebMouseHandler())
        self.registerVtkWebProtocol(pv_protocols.VisocyteWebViewPort())
        self.registerVtkWebProtocol(pv_protocols.VisocyteWebViewPortImageDelivery())
        self.registerVtkWebProtocol(pv_protocols.VisocyteWebViewPortGeometryDelivery())
        self.registerVtkWebProtocol(pv_protocols.VisocyteWebTimeHandler())
        self.registerVtkWebProtocol(pv_protocols.VisocyteWebRemoteConnection())
        self.registerVtkWebProtocol(pv_protocols.VisocyteWebFileManager(IPythonProtocol.dataDir))

        # Update authentication key to use
        self.updateSecret(IPythonProtocol.authKey)

    def __str__(self):
        return "Root node: " + str(IPythonProtocol.rootNode)

#------------------------------------------------------------------------------
# Visocyte iPython default arguments
#------------------------------------------------------------------------------

class WebArguments(object):

    def __init__(self, webDir = None):
        self.content          = webDir
        self.port             = 8080
        self.host             = 'localhost'
        self.debug            = 0
        self.timeout          = 120
        self.nosignalhandlers = True
        self.authKey          = 'vtkweb-secret'
        self.uploadDir        = ""
        self.testScriptPath   = ""
        self.baselineImgDir   = ""
        self.useBrowser       = ""
        self.tmpDirectory     = ""
        self.testImgFile      = ""
        self.forceFlush       = False
        self.dataDir          = '.'
        self.groupRegex       = "[0-9]+\\."
        self.excludeRegex     = "^\\.|~$|^\\$"
        self.fileToLoad       = None


    def __str__(self):
        return "http://%s:%d/%s" % (self.host, self.port, self.content)
