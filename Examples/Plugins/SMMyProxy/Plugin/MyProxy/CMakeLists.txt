set(classes
  vtkSMMyElevationProxy
  vtkSMMyElevationProxy)

vtk_module_add_module(MyProxy
  CLASSES ${classes})

visocyte_add_server_manager_xmls(
  XMLS  MyProxy.xml)
