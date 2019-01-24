cmake_minimum_required(VERSION 3.3)
project(Clone1)

FIND_PACKAGE(Visocyte REQUIRED)
INCLUDE(${VISOCYTE_USE_FILE})
if (NOT VISOCYTE_BUILD_QT_GUI)
  message(FATAL_ERROR
    "Clone1 example requires VISOCYTE_BUILD_QT_GUI to be enabled. "
    "Please rebuild Visocyte (or point to a different build of Visocyte) "
    "with VISOCYTE_BUILD_QT_GUI set to TRUE")
endif()

# Set a consistent MACOSX_RPATH default across all CMake versions.
# When CMake 2.8.12 is required, change this default to 1.
# When CMake 3.0.0 is required, remove this block (see CMP0042).
if(NOT DEFINED CMAKE_MACOSX_RPATH)
  set(CMAKE_MACOSX_RPATH 0)
endif()

# include rules for creating custom applications.
include(VisocyteBranding)
pv_find_package_qt(qt_targets
  QT5_COMPONENTS Widgets)

#------------------------------------------------------------------------------
# Add extra library containing custom code for the client.
pv_qt_wrap_cpp(MOC_BUILT_SOURCES myMainWindow.h)
pv_qt_wrap_ui(UI_BUILT_SOURCES myMainWindow.ui)

SOURCE_GROUP("Generated" FILES
  ${MOC_BUILT_SOURCES}
  ${UI_BUILT_SOURCES}
)

SET(Visocyte_SOURCE_FILES
  myMainWindow.cxx
  myMainWindow.h
  ${MOC_BUILT_SOURCES}
  ${UI_BUILT_SOURCES})

INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR})

build_visocyte_client(visocyte_revised
  TITLE "Visocyte (ReVisEd)"
  ORGANIZATION  "Kitware Inc."
  VERSION_MAJOR 1 
  VERSION_MINOR 1
  VERSION_PATCH 1
#  SPLASH_IMAGE "${CMAKE_CURRENT_SOURCE_DIR}/RSplash.png"
  PVMAIN_WINDOW myMainWindow
  PVMAIN_WINDOW_INCLUDE myMainWindow.h
  GUI_CONFIGURATION_XMLS
    ${CMAKE_CURRENT_SOURCE_DIR}/VisocyteSources.xml
    ${CMAKE_CURRENT_SOURCE_DIR}/VisocyteFilters.xml
  SOURCES ${Visocyte_SOURCE_FILES}
)
target_link_libraries(visocyte_revised LINK_PRIVATE ${qt_targets})
