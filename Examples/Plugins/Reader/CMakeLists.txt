
# create a plugin that adds a reader to the Visocyte GUI
# it is added in the file dialog when doing opens/saves.

cmake_minimum_required(VERSION 3.3)

IF(NOT Visocyte_BINARY_DIR)
  FIND_PACKAGE(Visocyte REQUIRED)
  INCLUDE(${VISOCYTE_USE_FILE})
ENDIF()

# Set a consistent MACOSX_RPATH default across all CMake versions.
# When CMake 2.8.12 is required, change this default to 1.
# When CMake 3.0.0 is required, remove this block (see CMP0042).
if(NOT DEFINED CMAKE_MACOSX_RPATH)
  set(CMAKE_MACOSX_RPATH 0)
endif()

IF(VISOCYTE_BUILD_QT_GUI)
  # The pqReader.xml file contains xml defining readers with their file
  # extensions and descriptions.  The Server Manager XML file defines the
  # properties of the reader.  This example uses the already existing PNG
  # reader but gives it another identity in the Server Manager.  Adding your
  # own reader to Visocyte would mean the SourceProxy class points to your
  # class.
  ADD_VISOCYTE_PLUGIN(MyPNGReader "1.0"
    SERVER_MANAGER_XML readers.xml
    REQUIRED_ON_SERVER
    )
ENDIF()
