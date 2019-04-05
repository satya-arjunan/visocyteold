set(interfaces)
set(sources)
if (VISOCYTE_BUILD_QT_GUI)
  list(APPEND sources
    pqNonOrthogonalAutoStart.cxx
    pqNonOrthogonalAutoStart.h
    pqSetupModelTransformBehavior.cxx
    pqSetupModelTransformBehavior.h)
  visocyte_plugin_add_auto_start(
    CLASS_NAME pqNonOrthogonalAutoStart
    INTERFACES autostart_interface
    SOURCES autostart_sources)
  list(APPEND interfaces
    ${autostart_interface})
  list(APPEND sources
    ${autostart_sources})
endif ()

visocyte_add_plugin(NonOrthogonalSource
  VERSION       "1.0"
  UI_INTERFACES ${interfaces}
  SOURCES       ${sources}
  MODULES       NonOrthogonalSource::vtkNonOrthogonalSources
  MODULE_FILES  "${CMAKE_CURRENT_SOURCE_DIR}/Sources/vtk.module")

if (VISOCYTE_BUILD_QT_GUI)
  target_link_libraries(NonOrthogonalSource
    PRIVATE
      Visocyte::pqApplicationComponents)
endif ()

if (BUILD_TESTING)
  add_subdirectory(Testing)
endif ()
