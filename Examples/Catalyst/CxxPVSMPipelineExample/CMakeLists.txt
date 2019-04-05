# Note: this actually only requires Visocyte::Catalyst.
if (USE_CATALYST)
  add_library(CxxPVSMPipelineExampleAdaptor
    FEAdaptor.cxx
    FEAdaptor.h
    vtkCPPVSMPipeline.cxx
    vtkCPPVSMPipeline.h)
  target_link_libraries(CxxPVSMPipelineExampleAdaptor
    PRIVATE
      Visocyte::Catalyst
      Visocyte::VTKExtensionsDefault
      Visocyte::ServerManagerCore)
endif ()

add_executable(CxxPVSMPipelineExample
  FEDataStructures.cxx
  FEDataStructures.h
  FEDriver.cxx)
if (TARGET CxxPVSMPipelineExampleAdaptor)
  target_link_libraries(CxxPVSMPipelineExample
    PRIVATE
      CxxPVSMPipelineExampleAdaptor
      VTK::mpi)
else ()
  target_link_libraries(CxxPVSMPipelineExample
    PRIVATE
      MPI::MPI_C)
endif ()

if (BUILD_TESTING)
  add_test(NAME CxxPVSMPipelineExampleTest COMMAND CxxPVSMPipelineExample 5 output)
endif()
