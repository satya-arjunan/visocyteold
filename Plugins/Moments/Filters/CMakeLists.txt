set(classes
  vtkMomentGlyphs
  vtkMomentVectors)

vtk_module_add_module(Moments::MomentFilters
  CLASSES ${classes})

visocyte_add_server_manager_xmls(
  XMLS  Moments.xml)
