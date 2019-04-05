cmake_minimum_required(VERSION 3.8)
project(Clone1)

find_package(Visocyte REQUIRED)

if (NOT VISOCYTE_BUILD_QT_GUI)
  message(STATUS
    "${CMAKE_PROJECT_NAME} example requires VISOCYTE_BUILD_QT_GUI to be "
    "enabled. Please rebuild Visocyte (or point to a different build of "
    "Visocyte) with VISOCYTE_BUILD_QT_GUI set to TRUE")
  return ()
endif()

include(GNUInstallDirs)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")

set(CMAKE_AUTOMOC 1)
set(CMAKE_AUTOUIC 1)

set(sources
  myMainWindow.cxx
  myMainWindow.h)
set(ui_files
  myMainWindow.ui)
set(xml_files
  VisocyteFilters.xml
  #VisocyteReaders.xml
  VisocyteSources.xml)

visocyte_client_add(
  NAME visocyte_revised
  VERSION "1.1.1"
  SOURCES ${sources} ${ui_files}
  APPLICATION_XMLS ${xml_files}

  MAIN_WINDOW_CLASS myMainWindow

  ORGANIZATION "Kitware, Inc."
  TITLE "Visocyte (ReVisEd)")
