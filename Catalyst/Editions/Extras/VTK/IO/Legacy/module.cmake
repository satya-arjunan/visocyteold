vtk_module(vtkIOLegacy
  GROUPS
    StandAlone
  DEPENDS
    vtkCommonDataModel
    vtkCommonSystem
    vtkCommonMisc
    vtkIOCore
  PRIVATE_DEPENDS
    vtksys
  TEST_DEPENDS
    vtkFiltersAMR
    vtkInteractionStyle
    vtkRenderingOpenGL
    vtkTestingRendering
  KIT
    vtkIO
  )
