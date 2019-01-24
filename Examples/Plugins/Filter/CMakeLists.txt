cmake_minimum_required(VERSION 3.3)

if (NOT Visocyte_BINARY_DIR)
  find_package(Visocyte REQUIRED)
  include(${VISOCYTE_USE_FILE})
endif()

# Set a consistent MACOSX_RPATH default across all CMake versions.
# When CMake 2.8.12 is required, change this default to 1.
# When CMake 3.0.0 is required, remove this block (see CMP0042).
if(NOT DEFINED CMAKE_MACOSX_RPATH)
  set(CMAKE_MACOSX_RPATH 0)
endif()

include(VisocytePlugins)

# create a visocyte plugin containing server manager xml and the server
# manager classes to build
# this plugin can be loaded on the server side

ADD_VISOCYTE_PLUGIN(SMMyElevation "1.0"
  SERVER_MANAGER_XML MyElevationFilter.xml
  SERVER_MANAGER_SOURCES vtkMyElevationFilter.cxx)

