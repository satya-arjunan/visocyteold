#/usr/bin/env python

# Global python import
import exceptions, logging, random, sys, threading, time, os

# Update python path to have Visocyte libs
build_path='/Volumes/SebKitSSD/Kitware/code/Visocyte/build-ninja'
sys.path.append('%s/lib'%build_path)
sys.path.append('%s/lib/site-packages'%build_path)

# iPython import
#from IPython.display import HTML
#from IPython.parallel import Client
import visocyte
from visocyte.web import ipython as pv_ipython
from vtk import *

iPythonClient = None
visocyteHelper = pv_ipython.VisocyteIPython()
webArguments = pv_ipython.WebArguments('/.../path-to-web-directory')

def _start_visocyte():
    visocyteHelper.Initialize()

    visocyteHelper.SetWebProtocol(IPythonProtocol, webArguments)
    return visocyteHelper.Start()


def _stop_visocyte():
    visocyteHelper.Finalize()


def _pv_activate_dataset():
    IPythonProtocol.ActivateDataSet('iPython-demo')


def _push_new_timestep():
    # processing code generating new vtkDataSet
    # newDataset = ...
    IPythonProtocol.RegisterDataSet('iPython-demo', newDataset)


def StartVisocyte(height=600, path='/apps/WebVisualizer/'):
    global iPythonClient, visocyteHelper
    if not iPythonClient:
        iPythonClient = Client()
    urls = iPythonClient[:].apply_sync(lambda:_start_visocyte())
    url = ""
    for i in urls:
        if len(i) > 0:
            url = i
    return  HTML("<iframe src='%s/%s' width='100%%' height='%i'></iframe>"%(url, path, height))


def StopVisocyte():
    global iPythonClient, visocyteHelper
    iPythonClient[:].apply_sync(lambda:_stop_visocyte())


def ActivateDataSet():
    iPythonClient[:].apply_sync(lambda:_pv_activate_dataset())


def ComputeNextTimeStep(ds):
    iPythonClient[:].apply_sync(lambda:_push_new_timestep())


print ("Start waiting")
time.sleep(10)
print ("Done")
