set(classes
  vtkMooseXfemClip)

vtk_module_add_module(MooseXfemClip::vtkMooseXfemClip
  CLASSES ${classes})

visocyte_add_server_manager_xmls(
  XMLS  MooseXfemClip.xml)
