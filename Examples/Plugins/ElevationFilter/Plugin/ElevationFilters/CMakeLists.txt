set(classes
  vtkMyElevationFilter)

vtk_module_add_module(ElevationFilters
  CLASSES ${classes})

visocyte_add_server_manager_xmls(
  XMLS  MyElevationFilter.xml)
