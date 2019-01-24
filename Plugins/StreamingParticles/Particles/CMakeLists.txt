set(classes
  vtkPVRandomPointsStreamingSource
  vtkStreamingParticlesPriorityQueue
  vtkStreamingParticlesRepresentation)

vtk_module_add_module(StreamingParticles::vtkStreamingParticles
  CLASSES ${classes})

visocyte_add_server_manager_xmls(
  XMLS  StreamingParticles.xml)
