set(classes
  vtkPMaterialClusterAnalysisFilter
  vtkPMaterialClusterExplodeFilter)

vtk_module_add_module(DigitalRockPhysics::DigitalRocksFilters
  CLASSES ${classes})

visocyte_add_server_manager_xmls(
    XMLS  DigitalRockPhysics.xml)
