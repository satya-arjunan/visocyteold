if (NOT (DEFINED visocyte_cmake_dir AND
         DEFINED visocyte_cmake_build_dir AND
         DEFINED visocyte_cmake_destination AND
         DEFINED visocyte_modules))
  message(FATAL_ERROR
    "VisocyteInstallCMakePackage is missing input variables.")
endif ()

configure_file(
  "${visocyte_cmake_dir}/visocyte-config.cmake.in"
  "${visocyte_cmake_build_dir}/visocyte-config.cmake"
  @ONLY)
include(CMakePackageConfigHelpers)
write_basic_package_version_file("${visocyte_cmake_build_dir}/visocyte-config-version.cmake"
  VERSION "${VISOCYTE_VERSION_FULL}"
  COMPATIBILITY AnyNewerVersion)

# For convenience, a package is written to the top of the build tree. At some
# point, this should probably be deprecated and warn when it is used.
file(GENERATE
  OUTPUT  "${CMAKE_BINARY_DIR}/visocyte-config.cmake"
  CONTENT "include(\"${visocyte_cmake_build_dir}/visocyte-config.cmake\")\n")
configure_file(
  "${visocyte_cmake_build_dir}/visocyte-config-version.cmake"
  "${CMAKE_BINARY_DIR}/visocyte-config-version.cmake"
  COPYONLY)

set(visocyte_cmake_module_files
  FindCGNS.cmake

  # Client API
  visocyte_client_initializer.cxx.in
  visocyte_client_initializer.h.in
  visocyte_launcher.c.in
  visocyte_client_main.cxx.in
  VisocyteClient.cmake
  visocyte_servermanager_convert_categoryindex.xsl
  visocyte_servermanager_convert_html.xsl
  visocyte_servermanager_convert_wiki.xsl.in
  visocyte_servermanager_convert_xml.xsl

  # Plugin API
  visocyte_plugin.cxx.in
  visocyte_plugin.h.in
  VisocytePlugin.cmake
  pqActionGroupImplementation.cxx.in
  pqActionGroupImplementation.h.in
  pqAutoStartImplementation.cxx.in
  pqAutoStartImplementation.h.in
  pqDockWindowImplementation.cxx.in
  pqDockWindowImplementation.h.in
  pqGraphLayoutStrategyImplementation.cxx.in
  pqGraphLayoutStrategyImplementation.h.in
  pqPropertyWidgetInterface.cxx.in
  pqPropertyWidgetInterface.h.in
  pqServerManagerModelImplementation.cxx.in
  pqServerManagerModelImplementation.h.in
  pqToolBarImplementation.cxx.in
  pqToolBarImplementation.h.in
  pqTreeLayoutStrategyImplementation.cxx.in
  pqTreeLayoutStrategyImplementation.h.in
  pqViewFrameActionGroupImplementation.cxx.in
  pqViewFrameActionGroupImplementation.h.in

  # ServerManager API
  VisocyteServerManager.cmake

  # Testing
  VisocyteTesting.cmake

  # Client Server
  vtkModuleWrapClientServer.cmake)

set(visocyte_cmake_files_to_install)
foreach (visocyte_cmake_module_file IN LISTS visocyte_cmake_module_files)
  configure_file(
    "${visocyte_cmake_dir}/${visocyte_cmake_module_file}"
    "${visocyte_cmake_build_dir}/${visocyte_cmake_module_file}"
    COPYONLY)
  list(APPEND visocyte_cmake_files_to_install
    "${visocyte_cmake_module_file}")
endforeach ()

foreach (visocyte_cmake_file IN LISTS visocyte_cmake_files_to_install)
  get_filename_component(subdir "${visocyte_cmake_file}" DIRECTORY)
  install(
    FILES       "${visocyte_cmake_dir}/${visocyte_cmake_file}"
    DESTINATION "${visocyte_cmake_destination}/${subdir}"
    COMPONENT   "development")
endforeach ()

install(
  FILES       "${visocyte_cmake_build_dir}/visocyte-config.cmake"
              "${visocyte_cmake_build_dir}/visocyte-config-version.cmake"
  DESTINATION "${visocyte_cmake_destination}"
  COMPONENT   "development")

vtk_module_export_find_packages(
  CMAKE_DESTINATION "${visocyte_cmake_destination}"
  FILE_NAME         "Visocyte-vtk-module-find-packages.cmake"
  MODULES           ${visocyte_modules})
