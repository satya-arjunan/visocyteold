set(classes
  vtkGenIOReader)

vtk_module_add_module(GenericIOReader::vtkGenericIOReader
  CLASSES ${classes})

vtk_module_link(GenericIOReader::vtkGenericIOReader
  PUBLIC
    LANL_GenericIO)

visocyte_add_server_manager_xmls(
  XMLS vtkGenIOReader.xml)

add_subdirectory(LANL)
