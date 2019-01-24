cmake_minimum_required(VERSION 3.3)
project(CatalystFortran90FullExample CXX Fortran)

find_package(Visocyte 4.1 REQUIRED COMPONENTS vtkPVPythonCatalyst)

include(${VISOCYTE_USE_FILE})
if(NOT VISOCYTE_USE_MPI)
  message(SEND_ERROR "Visocyte must be built with MPI enabled")
endif()
if(NOT MPI_Fortran_LIBRARIES)
  find_package(MPI)
endif()

add_executable(Fortran90FullExample FEDriver.f90 FEFortranAdaptor.f90 FECxxAdaptor.cxx)
target_link_libraries(Fortran90FullExample vtkPVPythonCatalyst ${MPI_Fortran_LIBRARIES})
set_target_properties(Fortran90FullExample PROPERTIES
  LINKER_LANGUAGE Fortran)

option(BUILD_TESTING "Build Testing" OFF)
# Setup testing.
if (BUILD_TESTING)
  file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/SampleScripts/coproc.py DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
  include(CTest)
  add_test(NAME Fortran90FullExampleTest COMMAND Fortran90FullExample)
  set_tests_properties(Fortran90FullExampleTest PROPERTIES LABELS "VISOCYTE;CATALYST")
endif()
