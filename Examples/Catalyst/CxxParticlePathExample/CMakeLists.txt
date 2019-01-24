cmake_minimum_required(VERSION 3.3)
project(CatalystCxxParticlePathExample)

set(USE_CATALYST ON CACHE BOOL "Link the simulator with Catalyst")
if(USE_CATALYST)
  # we need 5.0 for the proper in situ particle path filter
  find_package(Visocyte 5.0 REQUIRED COMPONENTS vtkPVPythonCatalyst)
  include("${VISOCYTE_USE_FILE}")
  set(Adaptor_SRCS
    FEAdaptor.cxx
    )
  add_library(CxxParticlePathExampleAdaptor ${Adaptor_SRCS})
  target_link_libraries(CxxParticlePathExampleAdaptor vtkPVPythonCatalyst vtkParallelMPI)
  add_definitions("-DUSE_CATALYST")
  if(NOT VISOCYTE_USE_MPI)
    message(SEND_ERROR "Visocyte must be built with MPI enabled")
  endif()
else()
  find_package(MPI REQUIRED)
  include_directories(${MPI_C_INCLUDE_PATH})
endif()

add_executable(CxxParticlePathExample FEDriver.cxx FEDataStructures.cxx)
if(USE_CATALYST)
  target_link_libraries(CxxParticlePathExample LINK_PRIVATE CxxParticlePathExampleAdaptor)
  include(vtkModuleMacros)
  include(vtkMPI)
  vtk_mpi_link(CxxParticlePathExample)
else()
  target_link_libraries(CxxParticlePathExample LINK_PRIVATE ${MPI_LIBRARIES})
endif()

option(BUILD_TESTING "Build Testing" OFF)
# Setup testing.
if (BUILD_TESTING)
  include(CTest)

  if(VISOCYTE_TEST_OUTPUT_DIR)
    set(TEST_OUTPUT_DIR ${VISOCYTE_TEST_OUTPUT_DIR})
  else(VISOCYTE_TEST_OUTPUT_DIR)
    set(TEST_OUTPUT_DIR "${CMAKE_BINARY_DIR}/Testing/Temporary")
  endif(VISOCYTE_TEST_OUTPUT_DIR)

  add_test(NAME CxxParticlePathExampleTest
    COMMAND ${CMAKE_COMMAND}
    -DCOPROCESSING_TEST_DRIVER:FILEPATH=$<TARGET_FILE:CxxParticlePathExample>
    -DCOPROCESSING_TEST_DIR:PATH=${TEST_OUTPUT_DIR}
    -DCOPROCESSING_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}
    -DPVBATCH:FILEPATH=$<TARGET_FILE:pvbatch>
    -DMPIEXEC:FILEPATH=${MPIEXEC}
    -DMPIEXEC_NUMPROC_FLAG:STRING=${MPIEXEC_NUMPROC_FLAG}
    -DMPIEXEC_NUMPROCS=4
    -DMPIEXEC_PREFLAGS:STRING=${MPIEXEC_PREFLAGS}
    -DVTK_MPI_POSTFLAGS:STRING=${VTK_MPI_POSTFLAGS}
    -P ${CMAKE_CURRENT_SOURCE_DIR}/ParticlePathTesting.cmake)
  set_tests_properties(CxxParticlePathExampleTest PROPERTIES LABELS "VISOCYTE;CATALYST")

endif (BUILD_TESTING)
