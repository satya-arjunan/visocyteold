cmake_minimum_required(VERSION 3.3)
include(CTest)
enable_testing()

find_package(ParaView REQUIRED)
include("${VISOCYTE_USE_FILE}")
include_directories(${VISOCYTE_INCLUDE_DIRS})

file(GLOB dirs */)

foreach (dir ${dirs})
  if (IS_DIRECTORY "${dir}")
    add_subdirectory("${dir}")
  endif ()
endforeach ()
