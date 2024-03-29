set (__dependencies)
if (VISOCYTE_USE_MPI)
    #set (__dependencies vtkFiltersParallelMPI)
  if (VISOCYTE_USE_ICE_T)
      #list(APPEND __dependencies vtkicet)
  endif()

  # needed for mpich
  add_definitions("-DMPICH_IGNORE_CXX_SEEK")
endif()

if(VISOCYTE_ENABLE_PYTHON)
  #list(APPEND __dependencies vtkRenderingMatplotlib)
endif()

list(APPEND __dependencies vtkglew)

if(VISOCYTE_USE_OSPRAY)
  #list(APPEND __dependencies vtkRenderingOSPRay)
endif()

if (Module_vtkRenderingCore)
  list(APPEND __dependencies
    vtkChartsCore
    vtkCommonColor
    vtkCommonComputationalGeometry
    vtkFiltersExtraction
    vtkFiltersGeneric
    vtkFiltersHyperTree
    vtkFiltersParallel
    vtkFiltersParallelMPI
    vtkIOExport
    vtkIOExportOpenGL2
    vtkIOImage
    vtkIOXML
    vtkInteractionStyle
    vtkParallelMPI
    vtkRenderingAnnotation
    vtkRenderingOpenGL2
    vtkRenderingParallel
    vtkicet
    vtklz4)
endif ()

vtk_module(vtkPVVTKExtensionsRendering
  GROUPS
    Qt
    VisocyteRendering
  DEPENDS
    vtkFiltersExtraction
    vtkFiltersSources
    vtkPVVTKExtensionsCore
    vtkRenderingVolume
    vtkRenderingVolumeOpenGL2

    ${__dependencies}
  COMPILE_DEPENDS

  TEST_DEPENDS
    vtkInteractionStyle
    vtkIOAMR
    vtkIOXML
    vtkTestingRendering

  TEST_LABELS
    VISOCYTE
  KIT
    vtkPVExtensions
)
