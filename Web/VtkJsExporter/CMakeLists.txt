# The exporter will behave as any other Visocyte exporter (VRML, X3D, POV...)
# but will be able to generate both a vtkjs file along an HTML file that contains
# both the data and Visocyte Glance.

set(classes
  vtkPVWebExporter)

vtk_module_add_module(Visocyte::PVWebExporter
  CLASSES ${classes})
