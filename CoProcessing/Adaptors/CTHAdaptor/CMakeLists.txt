cmake_minimum_required(VERSION 3.3)
project(CTHAdaptor)

find_package(Visocyte REQUIRED COMPONENTS vtkPVPythonCatalyst)
include("${VISOCYTE_USE_FILE}")
set(CTHAdaptor_SRCS
  CTHAdaptor.cxx
  vtkCTHSource.cxx
  vtkCTHDataArray.cxx
)

add_library(CTHAdaptor ${CTHAdaptor_SRCS})
target_link_libraries(
  CTHAdaptor
  vtkPVPythonCatalyst)
