find_package(Qt5 REQUIRED COMPONENTS Widgets Network)

set(interfaces)
set(sources
  pvOpenVRDockPanel.cxx
  pvOpenVRDockPanel.h
  vtkPVOpenVRHelper.cxx
  vtkPVOpenVRHelper.h)

visocyte_plugin_add_dock_window(
  CLASS_NAME pvOpenVRDockPanel
  DOCK_AREA Left
  INTERFACES dock_interfaces
  SOURCES dock_sources)
list(APPEND interfaces
  ${dock_interfaces})
list(APPEND sources
  ${dock_sources})

visocyte_add_plugin(OpenVR
  VERSION "1.1"
  UI_FILES pvOpenVRDockPanel.ui
  INTERFACES ${interfaces}
  SOURCES ${sources})

target_link_libraries(OpenVR
  PRIVATE
    Visocyte::ClientServerCoreRendering
    Visocyte::Common
    Visocyte::ServerManagerCore
    Visocyte::ServerManagerRendering
    Visocyte::VTKExtensionsRendering
    VTK::CommonCore
    VTK::CommonDataModel
    VTK::IOCore
    VTK::IOImage
    VTK::IOXML
    VTK::IOXMLParser
    VTK::InteractionWidgets
    VTK::RenderingCore
    VTK::RenderingOpenGL2
    VTK::RenderingOpenVR
    VTK::ViewsCore)
