set(classes
  vtkShearedWaveletSource)

vtk_module_add_module(NonOrthogonalSource::vtkNonOrthogonalSources
  CLASSES ${classes})

visocyte_add_server_manager_xmls(
  XMLS  vtkNonOrthogonalSource.xml)
