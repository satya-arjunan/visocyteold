set(classes
  vtkPVPythonAlgorithmPlugin
  vtkSIPythonSourceProxy)

vtk_module_add_module(Visocyte::PythonAlgorithm
  CLASSES ${classes})
