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
from vtk.vtkPVVTKExtensionsCore import *
from vtk import *

#------------------------------------------------------------------------------
# Start server
#------------------------------------------------------------------------------

visocyteHelper = pv_ipython.VisocyteIPython()
webArguments   = pv_ipython.WebArguments('%s/www' % build_path)
sphere = None

def start():
    visocyteHelper.Initialize(os.path.join(os.getcwd(), 'Testing', 'Temporary', 'mpi-python'))
    pv_ipython.IPythonProtocol.updateArguments(webArguments)
    visocyteHelper.SetWebProtocol(pv_ipython.IPythonProtocol, webArguments)
    return visocyteHelper.Start()

def start_thread():
    # Register some data at startup
    global sphere
    position = [random.random() * 2, random.random() * 2, random.random() * 2]
    sphere = vtkSphereSource()
    sphere.SetCenter(position)
    sphere.Update()
    pv_ipython.IPythonProtocol.RegisterDataSet('iPython-demo', sphere.GetOutput())

    # Start root+satelites
    thread = threading.Thread(target=start)
    print ("Starting thread")
    thread.start()
    for i in range(20):
        print ("Working... %ds" % (i*5))
        position = [random.random() * 2, random.random() * 2, random.random() * 2]
        print (position)
        sphere.SetCenter(position)
        sphere.Update()
        pv_ipython.IPythonProtocol.RegisterDataSet('iPython-demo', sphere.GetOutput())
        time.sleep(5)
        pv_ipython.IPythonProtocol.ActivateDataSet('iPython-demo')
    thread.join()
    print ("Done")

#------------------------------------------------------------------------------
# Main
#------------------------------------------------------------------------------
if __name__ == "__main__":
    start_thread()
