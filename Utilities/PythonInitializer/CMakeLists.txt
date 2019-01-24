set(sources
  vtkPVPythonInitializer.cxx)

vtk_module_add_module(Visocyte::PythonInitializer
  SOURCES ${sources})

# TODO: Frozen python support

vtk_module_link(Visocyte::PythonInitializer
  PRIVATE
    Visocyte::pvpythonmodules)
