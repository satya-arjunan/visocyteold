coProcessor = None

def initialize():
    global coProcessor
    import visocyte
    import vtkParallelCorePython
    import vtk
    from mpi4py import MPI
    import os, sys

    visocyte.options.batch = True
    visocyte.options.symmetric = True
    import vtkPVClientServerCoreCorePython as CorePython
    try:
        import vtkPVServerManagerApplicationPython as ApplicationPython
    except:
        visocyte.print_error("Error: Cannot import vtkPVServerManagerApplicationPython")

    if not CorePython.vtkProcessModule.GetProcessModule():
        pvoptions = None
        if visocyte.options.batch:
            pvoptions = CorePython.vtkPVOptions();
            pvoptions.SetProcessType(CorePython.vtkPVOptions.PVBATCH)
            if visocyte.options.symmetric:
                pvoptions.SetSymmetricMPIMode(True)
        ApplicationPython.vtkInitializationHelper.Initialize(sys.executable, CorePython.vtkProcessModule.PROCESS_BATCH, pvoptions)

    import visocyte.servermanager as pvsm
    # we need Visocyte 4.2 since Visocyte 4.1 doesn't properly wrap
    # vtkPVPythonCatalystPython
    if pvsm.vtkSMProxyManager.GetVersionMajor() < 4 or (pvsm.vtkSMProxyManager.GetVersionMajor() == 4 and pvsm.vtkSMProxyManager.GetVersionMinor() < 2):
        print 'Must use Visocyte v4.2 or greater'
        sys.exit(0)

    import numpy
    from visocyte.modules import vtkPVCatalyst as catalyst
    import vtkPVPythonCatalystPython as pythoncatalyst
    import visocyte.simple
    import visocyte.vtk as vtk
    from visocyte.vtk.util import numpy_support
    visocyte.options.batch = True
    visocyte.options.symmetric = True

    coProcessor = catalyst.vtkCPProcessor()
    pm = visocyte.servermanager.vtkProcessModule.GetProcessModule()
    from mpi4py import MPI

def finalize():
    global coProcessor
    coProcessor.Finalize()
    # if we are running through Python we need to finalize extra stuff
    # to avoid memory leak messages.
    import sys, ntpath
    if ntpath.basename(sys.executable) == 'python':
        import vtkPVServerManagerApplicationPython as ApplicationPython
        ApplicationPython.vtkInitializationHelper.Finalize()

def addscript(name):
    global coProcessor
    import vtkPVPythonCatalystPython as pythoncatalyst
    pipeline = pythoncatalyst.vtkCPPythonScriptPipeline()
    pipeline.Initialize(name)
    coProcessor.AddPipeline(pipeline)

def coprocess(time, timeStep, grid, attributes):
    global coProcessor
    import vtk
    from visocyte.modules import vtkPVCatalyst as catalyst
    import visocyte
    from visocyte.vtk.util import numpy_support
    dataDescription = catalyst.vtkCPDataDescription()
    dataDescription.SetTimeData(time, timeStep)
    dataDescription.AddInput("input")

    if coProcessor.RequestDataDescription(dataDescription):
        import fedatastructures
        imageData = vtk.vtkImageData()
        imageData.SetExtent(grid.XStartPoint, grid.XEndPoint, 0, grid.NumberOfYPoints-1, 0, grid.NumberOfZPoints-1)
        imageData.SetSpacing(grid.Spacing)

        velocity = numpy_support.numpy_to_vtk(attributes.Velocity)
        velocity.SetName("velocity")
        imageData.GetPointData().AddArray(velocity)

        pressure = numpy_support.numpy_to_vtk(attributes.Pressure)
        pressure.SetName("pressure")
        imageData.GetCellData().AddArray(pressure)
        dataDescription.GetInputDescriptionByName("input").SetGrid(imageData)
        dataDescription.GetInputDescriptionByName("input").SetWholeExtent(0, grid.NumberOfGlobalXPoints-1, 0, grid.NumberOfYPoints-1, 0, grid.NumberOfZPoints-1)
        coProcessor.CoProcess(dataDescription)
