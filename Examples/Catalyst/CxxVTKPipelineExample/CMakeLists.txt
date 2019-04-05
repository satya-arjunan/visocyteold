# Note: this actually only requires Visocyte::Catalyst.
if (USE_CATALYST)
  add_library(CxxVTKPipelineExampleAdaptor
    FEAdaptor.cxx
    FEAdaptor.h
    vtkCPVTKPipeline.cxx
    vtkCPVTKPipeline.h)
  target_link_libraries(CxxVTKPipelineExampleAdaptor
    PRIVATE
      Visocyte::Catalyst
      Visocyte::ClientServerCoreCore
      Visocyte::VTKExtensionsDefault
      Visocyte::ServerManagerCore)
endif ()

add_executable(CxxVTKPipelineExample
  FEDataStructures.cxx
  FEDataStructures.h
  FEDriver.cxx)
if (TARGET CxxVTKPipelineExampleAdaptor)
  target_link_libraries(CxxVTKPipelineExample
    PRIVATE
      CxxVTKPipelineExampleAdaptor
      VTK::mpi)
else ()
  target_link_libraries(CxxVTKPipelineExample
    PRIVATE
      MPI::MPI_C)
endif ()

if (BUILD_TESTING)
  add_test(NAME CxxVTKPipelineExampleTest COMMAND CxxVTKPipelineExample 10 output)
endif()
