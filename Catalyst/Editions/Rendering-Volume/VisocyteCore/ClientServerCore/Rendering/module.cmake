set (__dependencies)
set (__private_dependencies)
if (VISOCYTE_USE_PISTON)
  list (APPEND __dependencies vtkAcceleratorsPiston)
endif()
if(VISOCYTE_USE_VTKM)
  list(APPEND __private_dependencies
    vtkAcceleratorsVTKm
    vtkm
  )
endif()

vtk_module(vtkPVClientServerCoreRendering
  GROUPS
    VisocyteRendering
  DEPENDS
    #vtkDomainsChemistry
    #vtkFiltersAMR
    vtkPVClientServerCoreCore
    vtkPVVTKExtensionsDefault
    vtkPVVTKExtensionsRendering
    #vtkWebGLExporter
    vtkRenderingLabel
    #vtkRenderingVolumeAMR
    #vtkRenderingVolumeOpenGL
    vtkViewsContext2D
    vtkViewsCore
    ${__dependencies}
  PRIVATE_DEPENDS
    vtksys
    vtkzlib
    ${__private_dependencies}
  TEST_LABELS
    VISOCYTE
  KIT
    vtkPVClientServer
)
