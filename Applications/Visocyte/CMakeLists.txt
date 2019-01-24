cmake_dependent_option(VISOCYTE_ENABLE_EMBEDDED_DOCUMENTATION
  "Build the Visocyte help menu documentation for Visocyte and its plugins" ON
  "VISOCYTE_BUILD_QT_GUI" ON)
mark_as_advanced(VISOCYTE_ENABLE_EMBEDDED_DOCUMENTATION)

set(xmls
  "${CMAKE_CURRENT_SOURCE_DIR}/VisocyteFilters.xml"
  "${CMAKE_CURRENT_SOURCE_DIR}/VisocyteSources.xml")

if (VISOCYTE_ENABLE_EMBEDDED_DOCUMENTATION)
  #------------------------------------------------------------------------------
  # Build Online-Help (aka Embedded Help) for the Visocyte application.
  # This is done after the above piece of code that sets the application_gui_xmls
  # variable. Documentation/CMakeLists.txt depends on it.
  add_subdirectory(Documentation)
endif ()

if (DEFINED VISOCYTE_VERSION_NICKNAME)
  set(visocyte_version "${VISOCYTE_VERSION_NICKNAME}")
else ()
  set(visocyte_version "${VISOCYTE_VERSION_FULL}")
endif ()
set(visocyte_title "")

set(CMAKE_AUTOMOC 1)
set(CMAKE_AUTOUIC 1)

set(sources
  VisocyteMainWindow.cxx
  VisocyteMainWindow.h
  VisocyteMainWindow.ui)

set(visocyte_forward_executable OFF)
if (UNIX AND NOT APPLE)
  set(visocyte_forward_executable ON)
endif ()

find_package(Qt5 REQUIRED QUIET COMPONENTS Core Widgets)

visocyte_client_add(
  NAME              visocyte
  NAMESPACE         "Visocyte"
  EXPORT            "Visocyte"
  APPLICATION_NAME  "Visocyte"
  TITLE             "Visocyte ${visocyte_version}"
  ORGANIZATION      "Visocyte"
  VERSION           "${VISOCYTE_VERSION_FULL}"
  MAIN_WINDOW_CLASS "VisocyteMainWindow"
  BUNDLE_ICON       "${CMAKE_CURRENT_SOURCE_DIR}/pvIcon.icns"
  APPLICATION_ICON  "${CMAKE_CURRENT_SOURCE_DIR}/pvIcon.ico"
  SOURCES           ${sources}
  PLUGINS_TARGET    visocyte_plugins
  APPLICATION_XMLS  ${xmls}
  FORWARD_EXECUTABLE "${visocyte_forward_executable}")

find_package(Qt5 REQUIRED QUIET COMPONENTS Widgets)

target_link_libraries(visocyte
  PRIVATE
    Visocyte::ServerManagerDefault
    Qt5::Core
    Qt5::Widgets)

if (VISOCYTE_ENABLE_EMBEDDED_DOCUMENTATION)
  target_compile_definitions(visocyte
    PRIVATE
      VISOCYTE_ENABLE_EMBEDDED_DOCUMENTATION)
  target_link_libraries(visocyte
    PRIVATE
      vtkVisocyteDocumentation)
endif ()

if (VISOCYTE_ENABLE_PYTHON)
  target_link_libraries(visocyte
    PRIVATE
      Visocyte::pqPython
      Visocyte::PythonInitializer)
endif ()

if (BUILD_TESTING)
  add_subdirectory(Testing)
endif ()

if (UNIX AND NOT APPLE)
  configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/org.visocyte.Visocyte.desktop.in"
    "${CMAKE_CURRENT_BINARY_DIR}/org.visocyte.Visocyte.desktop"
    @ONLY)
  install(
    FILES       "${CMAKE_CURRENT_BINARY_DIR}/org.visocyte.Visocyte.desktop"
    DESTINATION share/applications
    COMPONENT   runtime)
  foreach (iconsize IN ITEMS 22x22 32x32 96x96)
    install(
      FILES       "pvIcon-${iconsize}.png"
      DESTINATION "share/icons/hicolor/${iconsize}/apps"
      RENAME      visocyte.png
      COMPONENT   runtime)
  endforeach ()
  install(
    FILES       org.visocyte.Visocyte.appdata.xml
    DESTINATION share/appdata
    COMPONENT   runtime)
endif ()
