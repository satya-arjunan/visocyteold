set(classes
  vtkPVPanoramicProjectionView)

vtk_module_add_module(PanoramicProjectionView::vtkPanoramicProjectionViews
  CLASSES ${classes})

visocyte_add_server_manager_xmls(
  XMLS  PanoramicProjectionView.xml)
