set(classes
  vtkMySpecialPolyDataMapper
  vtkMySpecialRepresentation)

vtk_module_add_module(GeometryRepresentations
  CLASSES ${classes})

visocyte_add_server_manager_xmls(
  XMLS  Representation.xml)
