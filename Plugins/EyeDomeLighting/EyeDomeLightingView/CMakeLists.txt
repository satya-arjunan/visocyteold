set(classes
  vtkPVRenderViewWithEDL)

vtk_module_add_module(EyeDomeLighting::EyeDomeLightingView
  CLASSES ${classes})

visocyte_add_server_manager_xmls(
  XMLS  RenderPassViews.xml)
