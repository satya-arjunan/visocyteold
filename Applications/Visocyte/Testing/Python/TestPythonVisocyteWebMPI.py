#/usr/bin/env python

# Global python import
import exceptions, traceback, logging, random, sys, threading, time, os

# Update python path to have Visocyte libs
build_path='/Volumes/SebKitSSD/Kitware/code/Visocyte/build-ninja'
sys.path.append('%s/lib'%build_path)
sys.path.append('%s/lib/site-packages'%build_path)

# Visocyte import
from vtk.web import server
from visocyte.vtk import *
from visocyte.web import wamp as pv_wamp
from visocyte.web import ipython as pv_ipython

from vtk.vtkCommonCore import *
from vtk.vtkCommonDataModel import *
from vtk.vtkCommonExecutionModel import *
from vtk.vtkFiltersSources import *
from vtk.vtkParallelCore import *
from vtk.vtkVisocyteWebCore import *
from vtk.vtkPVClientServerCoreCore import *
from vtk.vtkPVServerManagerApplication import *
from vtk.vtkPVServerManagerCore import *
from vtkPVVTKExtensionsCore import *

#------------------------------------------------------------------------------
# InLine protocol
#------------------------------------------------------------------------------

class TestProtocol(pv_wamp.PVServerProtocol):
    dataDir        = None
    authKey        = "vtkweb-secret"
    fileToLoad     = None
    groupRegex     = "[0-9]+\\."
    excludeRegex   = "^\\.|~$|^\\$"

    @staticmethod
    def updateArguments(options):
        TestProtocol.dataDir      = options.dataDir
        TestProtocol.authKey      = options.authKey
        TestProtocol.fileToLoad   = options.fileToLoad
        TestProtocol.authKey      = options.authKey
        TestProtocol.groupRegex   = options.groupRegex
        TestProtocol.excludeRegex = options.excludeRegex

    def initialize(self):
        from visocyte import simple
        from visocyte.web import protocols as pv_protocols

        # Bring used components
        self.registerVtkWebProtocol(pv_protocols.VisocyteWebFileListing(TestProtocol.dataDir, "Home", TestProtocol.excludeRegex, TestProtocol.groupRegex))
        self.registerVtkWebProtocol(pv_protocols.VisocyteWebPipelineManager(TestProtocol.dataDir, TestProtocol.fileToLoad))
        self.registerVtkWebProtocol(pv_protocols.VisocyteWebMouseHandler())
        self.registerVtkWebProtocol(pv_protocols.VisocyteWebViewPort())
        self.registerVtkWebProtocol(pv_protocols.VisocyteWebViewPortImageDelivery())
        self.registerVtkWebProtocol(pv_protocols.VisocyteWebViewPortGeometryDelivery())
        self.registerVtkWebProtocol(pv_protocols.VisocyteWebTimeHandler())
        self.registerVtkWebProtocol(pv_protocols.VisocyteWebRemoteConnection())
        self.registerVtkWebProtocol(pv_protocols.VisocyteWebFileManager(TestProtocol.dataDir))

        # Update authentication key to use
        self.updateSecret(TestProtocol.authKey)


#------------------------------------------------------------------------------
# Start server
#------------------------------------------------------------------------------

visocyteHelper = pv_ipython.VisocyteIPython()
webArguments   = pv_ipython.WebArguments('%s/www' % build_path)

def start():
    visocyteHelper.Initialize(os.path.join(os.getcwd(), 'Testing', 'Temporary', 'mpi-python'))
    visocyteHelper.SetWebProtocol(TestProtocol, webArguments)
    return visocyteHelper.Start()

def start_thread():
    thread = threading.Thread(target=start)
    print ("Starting thread")
    thread.start()
    for i in range(20):
        print ("Working... %ds" % (i*5))
        time.sleep(5)
    thread.join()
    print ("Done")

#------------------------------------------------------------------------------
# Main
#------------------------------------------------------------------------------
if __name__ == "__main__":
    start_thread()
