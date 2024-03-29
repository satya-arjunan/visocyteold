cmake_minimum_required(VERSION 3.3)
project(SpreadSheet)

find_package(Visocyte REQUIRED)
include(${VISOCYTE_USE_FILE})
if (NOT VISOCYTE_BUILD_QT_GUI)
  message(FATAL_ERROR
    "SpreadSheet example requires VISOCYTE_BUILD_QT_GUI to be enabled. "
    "Please rebuild Visocyte (or point to a different build of Visocyte) "
    "with VISOCYTE_BUILD_QT_GUI set to TRUE")
endif()

# Set a consistent MACOSX_RPATH default across all CMake versions.
# When CMake 2.8.12 is required, change this default to 1.
# When CMake 3.0.0 is required, remove this block (see CMP0042).
if(NOT DEFINED CMAKE_MACOSX_RPATH)
  set(CMAKE_MACOSX_RPATH 0)
endif()

# Variables that need to be set for installation code to work correctly.
SET (BUILD_SHARED_LIBS ${VISOCYTE_BUILD_SHARED_LIBS}) # enable shared builds.

SET (SOURCE_FILES
  SpreadSheetMainWindow.cxx
  SpreadSheetMainWindow.h
)

pv_qt_wrap_cpp(MOC_SRCS SpreadSheetMainWindow.h)
pv_qt_wrap_ui(UI_BUILT_SOURCES SpreadSheetMainWindow.ui)

##########################################################################
## Add resources (icons etc.)
SET (UI_RESOURCES
  SpreadSheet.qrc
)

pv_qt_add_resources(RCS_BUILT_SOURCES ${UI_RESOURCES})

SOURCE_GROUP("Resources" FILES
  ${UI_RESOURCES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR})


#------------------------------------------------------------------------------
include(VisocyteBranding)

# Build the client
build_visocyte_client(spreadsheet
    TITLE "Raw Data Viewer (Visocyte-based Example Application)"
    ORGANIZATION  "Kitware Inc"
    VERSION_MAJOR 1
    VERSION_MINOR 0
    VERSION_PATCH 0
    PVMAIN_WINDOW SpreadSheetMainWindow
    PVMAIN_WINDOW_INCLUDE SpreadSheetMainWindow.h
    GUI_CONFIGURATION_XMLS
      ${CMAKE_CURRENT_SOURCE_DIR}/SupportedReader.xml
    SOURCES ${SOURCE_FILES} ${MOC_SRCS} ${UI_BUILT_SOURCES} ${RCS_BUILT_SOURCES}
)
