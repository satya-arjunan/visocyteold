set(_VisocyteClient_cmake_dir "${CMAKE_CURRENT_LIST_DIR}")
set(_VisocyteClient_script_file "${CMAKE_CURRENT_LIST_FILE}")

#[==[.md
## Building a client

TODO: Document

```
visocyte_client_add(
  NAME    <name>
  VERSION <version>
  SOURCES <source>...
  [APPLICATION_XMLS <xml>...]
  [QCH_FILE <file>]

  [MAIN_WINDOW_CLASS    <class>]
  [MAIN_WINDOW_INCLUDE  <include>]

  [PLUGINS_TARGET   <target>]
  [REQUIRED_PLUGINS <plugin>...]
  [OPTIONAL_PLUGINS <plugin>...]

  [APPLICATION_NAME <name>]
  [ORGANIZATION     <organization>]
  [TITLE            <title>]

  [DEFAULT_STYLE  <style>]

  [APPLICATION_ICON <icon>]
  [BUNDLE_ICON      <icon>]
  [SPLASH_IMAGE     <image>]

  [NAMESPACE            <namespace>]
  [EXPORT               <export>]
  [FORCE_UNIX_LAYOUT    <ON|OFF>]
  [BUNDLE_DESTINATION   <directory>]
  [RUNTIME_DESTINATION  <directory>]
  [LIBRARY_DESTINATION  <directory>]
  [FORWARD_EXECUTABLE   <ON|OFF>]
```

  * `NAME`: (Required) The name of the application. This is used as the target
    name as well.
  * `VERSION`: (Required) The version of the application.
  * `SOURCES`: (Required) Source files for the application.
  * `APPLICATION_XMLS`: Server manager XML files.
  * `QCH_FILE`: The `.qch` file containing documentation.
  * `MAIN_WINDOW_CLASS`: (Defaults to `QMainWindow`) The name of the main
    window class.
  * `MAIN_WINDOW_INCLUDE`: (Defaults to `QMainWindow` or
    `<MAIN_WINDOW_CLASS>.h` if it is specified) The include file for the main
    window.
  * `PLUGINS_TARGET`: The target for static plugins. The associated function
    will be called upon startup.
  * `REQUIRED_PLUGINS`: Plugins to load upon startup.
  * `OPTIONAL_PLUGINS`: Plugins to load upon startup if available.
  * `APPLICATION_NAME`: (Defaults to `<NAME>`) The displayed name of the
    application.
  * `ORGANIZATION`: (Defaults to `Anonymous`) The organization for the
    application. This is used for the macOS GUI identifier.
  * `TITLE`: The window title for the application.
  * `DEFAULT_STYLE`: (Defaults to `plastique`) The default Qt style for the
    application.
  * `APPLICATION_ICON`: The path to the icon for the Windows application.
  * `BUNDLE_ICON`: The path to the icon for the macOS bundle.
  * `SPLASH_IMAGE`: The image to display upon startup.
  * `NAMESPACE`: If provided, an alias target `<NAMESPACE>::<NAME>` will be
    created.
  * `EXPORT`: If provided, the target will be exported.
  * `FORCE_UNIX_LAYOUT`: (Defaults to `OFF`) Forces a Unix-style layout even on
    platforms for which they are not the norm for GUI applications (e.g.,
    macOS).
  * `BUNDLE_DESTINATION`: (Defaults to
    `<RUNTIME_DESTINATION>/<NAME>.app/Contents/MacOS`) Where to place the
    bundle executable.
  * `RUNTIME_DESTINATION`: (Defaults to `${CMAKE_INSTALL_BINDIR}`) Where to
    place the binary.
  * `LIBRARY_DESTINATION`: (Defaults to `${CMAKE_INSTALL_LIBDIR}`) Where to
    place libraries. Only used if `FORWARD_EXECUTABLE` is `ON`.
  * `FORWARD_EXECUTABLE`: (Deprecated) (Defaults to `OFF`) On non-macOS Unix
    platforms, create a binary to set up `LD_LIBRARY_PATH` for the real
    executable.
#]==]
function (visocyte_client_add)
  cmake_parse_arguments(_visocyte_client
    ""
    "NAME;APPLICATION_NAME;ORGANIZATION;TITLE;SPLASH_IMAGE;BUNDLE_DESTINATION;BUNDLE_ICON;APPLICATION_ICON;MAIN_WINDOW_CLASS;MAIN_WINDOW_INCLUDE;VERSION;FORCE_UNIX_LAYOUT;PLUGINS_TARGET;DEFAULT_STYLE;FORWARD_EXECUTABLE;RUNTIME_DESTINATION;LIBRARY_DESTINATION;NAMESPACE;EXPORT"
    "REQUIRED_PLUGINS;OPTIONAL_PLUGINS;APPLICATION_XMLS;SOURCES;QCH_FILE"
    ${ARGN})

  if (_visocyte_client_UNPARSED_ARGUMENTS)
    message(FATAL_ERROR
      "Unparsed arguments for visocyte_client_add: "
      "${_visocyte_client_UNPARSED_ARGUMENTS}")
  endif ()

  # TODO: Installation.

  if (NOT DEFINED _visocyte_client_NAME)
    message(FATAL_ERROR
      "The `NAME` argument is required.")
  endif ()

  if (NOT DEFINED _visocyte_client_VERSION)
    message(FATAL_ERROR
      "The `VERSION` argument is required.")
  endif ()

  if (NOT DEFINED _visocyte_client_SOURCES)
    message(FATAL_ERROR
      "The `SOURCES` argument is required.")
  endif ()

  if (NOT DEFINED _visocyte_client_APPLICATION_NAME)
    set(_visocyte_client_APPLICATION_NAME
      "${_visocyte_client_NAME}")
  endif ()

  if (NOT DEFINED _visocyte_client_ORGANIZATION)
    set(_visocyte_client_ORGANIZATION
      "Anonymous")
  endif ()

  if (NOT DEFINED _visocyte_client_FORCE_UNIX_LAYOUT)
    set(_visocyte_client_FORCE_UNIX_LAYOUT
      OFF)
  endif ()

  if (NOT DEFINED _visocyte_client_DEFAULT_STYLE)
    set(_visocyte_client_DEFAULT_STYLE
      "plastique")
  endif ()

  if (NOT DEFINED _visocyte_client_RUNTIME_DESTINATION)
    set(_visocyte_client_RUNTIME_DESTINATION
      "${CMAKE_INSTALL_BINDIR}")
  endif ()

  if (NOT DEFINED _visocyte_client_LIBRARY_DESTINATION)
    set(_visocyte_client_LIBRARY_DESTINATION
      "${CMAKE_INSTALL_LIBDIR}")
  endif ()

  if (NOT DEFINED _visocyte_client_FORWARD_EXECUTABLE)
    #message(DEPRECATION
    #  "The `FORWARD_EXECUTABLE` argument is deprecated.")
    set(_visocyte_client_FORWARD_EXECUTABLE OFF)
  endif ()

  if (NOT DEFINED _visocyte_client_MAIN_WINDOW_CLASS)
    if (DEFINED _visocyte_client_MAIN_WINDOW_INCLUDE)
      message(FATAL_ERROR
        "The `MAIN_WINDOW_INCLUDE` argument cannot be specified without "
        "`MAIN_WINDOW_CLASS`.")
    endif ()

    set(_visocyte_client_MAIN_WINDOW_CLASS
      "QMainWindow")
    set(_visocyte_client_MAIN_WINDOW_INCLUDE
      "QMainWindow")
  endif ()

  if (NOT DEFINED _visocyte_client_MAIN_WINDOW_INCLUDE)
    set(_visocyte_client_MAIN_WINDOW_INCLUDE
      "${_visocyte_client_MAIN_WINDOW_CLASS}.h")
  endif ()

  set(_visocyte_client_extra_sources)
  set(_visocyte_client_bundle_args)

  set(_visocyte_client_executable_flags)
  if (WIN32)
    if (DEFINED _visocyte_client_APPLICATION_ICON)
      set(_visocyte_client_appicon_file
        "${CMAKE_CURRENT_BINARY_DIR}/${_visocyte_client_NAME}_appicon.rc")
      file(WRITE "${_visocyte_client_appicon_file}.tmp"
        "// Icon with the lowest ID value placed first to ensure that the application
// icon remains consistent on all systems.
IDI_ICON1 ICON \"${_visocyte_client_APPLICATION_ICON}\"\n")
      configure_file(
        "${_visocyte_client_appicon_file}.tmp"
        "${_visocyte_client_appicon_file}"
        COPYONLY)

      list(APPEND _visocyte_client_extra_sources
        "${_visocyte_client_appicon_file}")
    endif ()

    set(_visocyte_client_executable_flags
      WIN32)
  elseif (APPLE)
    # TODO: bundle icon
    # TODO: nib files

    if (NOT DEFINED _visocyte_client_BUNDLE_DESTINATION)
      set(_visocyte_client_BUNDLE_DESTINATION "${_visocyte_client_RUNTIME_DESTINATION}/${_visocyte_client_NAME}.app/Contents/MacOS")
    endif ()

    set(_visocyte_client_bundle_args
      BUNDLE DESTINATION "${_visocyte_client_BUNDLE_DESTINATION}")
    set(_visocyte_client_executable_flags
      MACOSX_BUNDLE)
  endif ()

  set(_visocyte_client_resource_files "")
  set(_visocyte_client_resource_init "")

  if (DEFINED _visocyte_client_SPLASH_IMAGE)
    set(_visocyte_client_splash_base_name
      "${_visocyte_client_NAME}_splash")
    set(_visocyte_client_splash_image_name
      "${_visocyte_client_splash_base_name}.img")
    set(_visocyte_client_splash_resource
      ":/${_visocyte_client_NAME}/${_visocyte_client_splash_image_name}")

    set(_visocyte_client_splash_resource_file
      "${CMAKE_CURRENT_BINARY_DIR}/${_visocyte_client_splash_base_name}.qrc")

    visocyte_client_qt_resource(
      OUTPUT  "${_visocyte_client_splash_resource_file}"
      PREFIX  "/${_visocyte_client_NAME}"
      ALIAS   "${_visocyte_client_splash_base_name}"
      FILE    "${_visocyte_client_SPLASH_IMAGE}")

    list(APPEND _visocyte_client_resource_files
      "${_visocyte_client_splash_resource_file}")
    string(APPEND _visocyte_client_resource_init
      "  Q_INIT_RESOURCE(${_visocyte_client_splash_base_name});\n")
    set(CMAKE_AUTORCC 1)
  endif ()

  if (DEFINED _visocyte_client_APPLICATION_XMLS)
    set(_visocyte_client_application_base_name
      "${_visocyte_client_NAME}_configuration")
    set(_visocyte_client_application_resource_file
      "${CMAKE_CURRENT_BINARY_DIR}/${_visocyte_client_application_base_name}.qrc")

    visocyte_client_qt_resources(
      OUTPUT  "${_visocyte_client_application_resource_file}"
      PREFIX  "/${_visocyte_client_NAME}/Configuration"
      FILES   "${_visocyte_client_APPLICATION_XMLS}")

    list(APPEND _visocyte_client_resource_files
      "${_visocyte_client_application_resource_file}")
    string(APPEND _visocyte_client_resource_init
      "  Q_INIT_RESOURCE(${_visocyte_client_application_base_name});\n")
    set(CMAKE_AUTORCC 1)
  endif ()

  if (DEFINED _visocyte_client_QCH_FILE)
    set(_visocyte_client_documentation_base_name
      "${_visocyte_client_NAME}_documentation")
    set(_visocyte_client_documentation_resource_file
      "${CMAKE_CURRENT_BINARY_DIR}/${_visocyte_client_documentation_base_name}.qrc")

    visocyte_client_qt_resources(
      OUTPUT  "${_visocyte_client_documentation_resource_file}"
      # This prefix is part of the API.
      PREFIX  "/${_visocyte_client_NAME}/Documentation"
      FILES   "${_visocyte_client_QCH_FILE}")
    set_property(SOURCE "${_visocyte_client_documentation_resource_file}"
      PROPERTY
        OBJECT_DEPENDS "${_visocyte_client_QCH_FILE}")

    list(APPEND _visocyte_client_resource_files
      "${_visocyte_client_documentation_resource_file}")
    string(APPEND _visocyte_client_resource_init
      "  Q_INIT_RESOURCE(${_visocyte_client_documentation_base_name});\n")
    set(CMAKE_AUTORCC 1)
  endif ()

  find_package(Qt5 REQUIRED QUIET COMPONENTS Widgets)

  set(_visocyte_client_source_files
    "${CMAKE_CURRENT_BINARY_DIR}/${_visocyte_client_NAME}_main.cxx"
    "${CMAKE_CURRENT_BINARY_DIR}/pq${_visocyte_client_NAME}Initializer.cxx"
    "${CMAKE_CURRENT_BINARY_DIR}/pq${_visocyte_client_NAME}Initializer.h")
  configure_file(
    "${_VisocyteClient_cmake_dir}/visocyte_client_main.cxx.in"
    "${CMAKE_CURRENT_BINARY_DIR}/${_visocyte_client_NAME}_main.cxx"
    @ONLY)
  configure_file(
    "${_VisocyteClient_cmake_dir}/visocyte_client_initializer.cxx.in"
    "${CMAKE_CURRENT_BINARY_DIR}/pq${_visocyte_client_NAME}Initializer.cxx"
    @ONLY)
  configure_file(
    "${_VisocyteClient_cmake_dir}/visocyte_client_initializer.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/pq${_visocyte_client_NAME}Initializer.h"
    @ONLY)

  if (_visocyte_client_resource_files)
    source_group("resources"
      FILES
        ${_visocyte_client_resource_files})
  endif ()
  add_executable("${_visocyte_client_NAME}" ${_visocyte_client_executable_flags}
    ${_visocyte_client_SOURCES}
    ${_visocyte_client_resource_files}
    ${_visocyte_client_source_files}
    ${_visocyte_client_extra_sources})
  if (DEFINED _visocyte_client_NAMESPACE)
    add_executable("${_visocyte_client_NAMESPACE}::${_visocyte_client_NAME}" ALIAS "${_visocyte_client_NAME}")
  endif ()
  target_include_directories("${_visocyte_client_NAME}"
    PRIVATE
      "${CMAKE_CURRENT_SOURCE_DIR}"
      "${CMAKE_CURRENT_BINARY_DIR}"
      # https://gitlab.kitware.com/cmake/cmake/issues/18049
      "$<TARGET_PROPERTY:VTK::vtksys,INTERFACE_INCLUDE_DIRECTORIES>")
  target_link_libraries("${_visocyte_client_NAME}"
    PRIVATE
      Qt5::Widgets
      Visocyte::pqApplicationComponents
      VTK::vtksys)

  if (DEFINED _visocyte_client_PLUGINS_TARGET)
    target_link_libraries("${_visocyte_client_NAME}"
      PRIVATE
        "${_visocyte_client_PLUGINS_TARGET}")
  endif ()

  # XXX(deprecated): Visocyte should not need to use this.
  set(_visocyte_client_destination "${_visocyte_client_RUNTIME_DESTINATION}")
  if (BUILD_SHARED_LIBS AND UNIX AND NOT APPLE AND _visocyte_client_FORWARD_EXECUTABLE)
    set(_visocyte_client_build_dir "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
    set(_visocyte_client_build_path "\"${_visocyte_client_build_dir}\"")
    foreach (_visocyte_client_build_path_dir IN LISTS )
      string(APPEND _visocyte_client_build_path
        ",\"${_visocyte_client_build_path_dir}\"")
    endforeach ()
    set(_visocyte_client_install_dir "../${_visocyte_client_LIBRARY_DESTINATION}")
    set(_visocyte_client_install_path "\"${_visocyte_client_install_dir}\"")
    foreach (_visocyte_client_install_path_dir IN LISTS )
      string(APPEND _visocyte_client_install_path
        ",\"${_visocyte_client_install_path_dir}\"")
    endforeach ()

    # TODO: Set variables for the file.
    configure_file(
      "${_VisocyteClient_cmake_dir}/visocyte_client_launcher.c.in"
      "${CMAKE_CURRENT_BINARY_DIR}/${_visocyte_client_NAME}_launcher.c"
      @ONLY)
    add_executable("${_visocyte_client_NAME}-launcher"
      "${CMAKE_CURRENT_BINARY_DIR}/${_visocyte_client_NAME}_launcher.c")
    # https://gitlab.kitware.com/cmake/cmake/issues/18049
    #target_link_libraries("${_visocyte_client_NAME}-launcher"
    #  PRIVATE
    #    VTK::vtksys)
    target_include_directories("${_visocyte_client_NAME}-launcher"
      PRIVATE
        "$<TARGET_PROPERTY:VTK::vtksys,INTERFACE_INCLUDE_DIRECTORIES>")
    set_target_properties("${_visocyte_client_NAME}-launcher"
      PROPERTIES
        OUTPUT_NAME               "${_visocyte_client_NAME}"
        RUNTIME_OUTPUT_DIRECTORY  "${CMAKE_CURRENT_BINARY_DIR}/launcher")
    add_dependencies("${_visocyte_client_NAME}-launcher"
      "${_visocyte_client_NAME}")

    install(
      TARGETS "${_visocyte_client_NAME}-launcher"
      RUNTIME DESTINATION "${_visocyte_client_RUNTIME_DESTINATION}")
    set(_visocyte_client_destination "${_visocyte_client_LIBRARY_DESTINATION}")
  endif ()

  set(_visocyte_client_export)
  if (DEFINED _visocyte_client_EXPORT)
    set(_visocyte_client_export
      EXPORT "${_visocyte_client_EXPORT}")
  endif ()

  install(
    TARGETS "${_visocyte_client_NAME}"
    ${_visocyte_client_export}
    ${_visocyte_client_bundle_args}
    RUNTIME DESTINATION "${_visocyte_client_destination}")

  if (APPLE)
    if (DEFINED _visocyte_client_BUNDLE_ICON)
      set_property(TARGET "${_visocyte_client_NAME}"
        PROPERTY
          MACOSX_BUNDLE_ICON_FILE "${_visocyte_client_bundle_icon}")
    endif ()
    string(TOLOWER "${_visocyte_client_ORGANIZATION}" _visocyte_client_organization)
    set_target_properties("${_visocyte_client_NAME}"
      PROPERTIES
        MACOSX_BUNDLE_BUNDLE_NAME           "${_visocyte_client_APPLICATION_NAME}"
        MACOSX_BUNDLE_GUI_IDENTIFIER        "org.${_visocyte_client_organization}.${_visocyte_client_APPLICATION_NAME}"
        MACOSX_BUNDLE_SHORT_VERSION_STRING  "${_visocyte_client_VERSION}")
  endif ()
endfunction ()

#[==[.md INTERNAL
## Quoting

Passing CMake lists down to the help generation and proxy documentation steps
requires escaping the `;` in them. These functions escape and unescape the
variable passed in. The new value is placed in the same variable in the calling
scope.
#]==]

function (_visocyte_client_escape_cmake_list variable)
  string(REPLACE "_" "_u" _escape_tmp "${${variable}}")
  string(REPLACE ";" "_s" _escape_tmp "${_escape_tmp}")
  set("${variable}"
    "${_escape_tmp}"
    PARENT_SCOPE)
endfunction ()

function (_visocyte_client_unescape_cmake_list variable)
  string(REPLACE "_s" ";" _escape_tmp "${${variable}}")
  string(REPLACE "_u" "_" _escape_tmp "${_escape_tmp}")
  set("${variable}"
    "${_escape_tmp}"
    PARENT_SCOPE)
endfunction ()

#[==[.md
## Documentation from XML files

Documentation can be generated from server manager XML files. The
`visocyte_client_documentation` generates Qt help, HTML, and Wiki documentation
from them.

```
visocyte_client_documentation(
  TARGET  <target>
  XMLS    <xml>...
  [OUTPUT_DIR <directory>])
```

  * `TARGET`: (Required) The name of the target to generate.
  * `XMLS`: (Required) The list of XML files to process.
  * `OUTPUT_DIR`: (Defaults to `${CMAKE_CURRENT_BINARY_DIR}`) Where to place
    generated documentation.
#]==]
function (visocyte_client_documentation)
  cmake_parse_arguments(_visocyte_client_doc
    ""
    "TARGET;OUTPUT_DIR"
    "XMLS"
    ${ARGN})

  if (_visocyte_client_doc_UNPARSED_ARGUMENTS)
    message(FATAL_ERROR
      "Unparsed arguments for visocyte_client_documentation: "
      "${_visocyte_client_doc_UNPARSED_ARGUMENTS}")
  endif ()

  if (NOT DEFINED _visocyte_client_doc_OUTPUT_DIR)
    set(_visocyte_client_doc_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}")
  endif ()

  if (NOT DEFINED _visocyte_client_doc_TARGET)
    message(FATAL_ERROR
      "The `TARGET` argument is required.")
  endif ()

  if (NOT DEFINED _visocyte_client_doc_XMLS)
    message(FATAL_ERROR
      "The `XMLS` argument is required.")
  endif ()

  find_program(qt_xmlpatterns_executable
    NAMES xmlpatterns-qt5 xmlpatterns
    HINTS "${Qt5_DIR}/../../../bin"
    DOC   "Path to xmlpatterns")
  mark_as_advanced(qt_xmlpatterns_executable)

  if (NOT qt_xmlpatterns_executable)
    message(FATAL_ERROR
      "Cannot find the xmlpatterns executable.")
  endif ()

  set(_visocyte_client_doc_xmls)
  foreach (_visocyte_client_doc_xml IN LISTS _visocyte_client_doc_XMLS)
    get_filename_component(_visocyte_client_doc_xml "${_visocyte_client_doc_xml}" ABSOLUTE)
    list(APPEND _visocyte_client_doc_xmls
      "${_visocyte_client_doc_xml}")
  endforeach ()

  # Escaping in order to pass as an argument.
  set(_visocyte_client_doc_xmls_list "${_visocyte_client_doc_xmls}")
  _visocyte_client_escape_cmake_list(_visocyte_client_doc_xmls)

  add_custom_command(
    OUTPUT  "${_visocyte_client_doc_OUTPUT_DIR}/${_visocyte_client_doc_TARGET}.xslt"
            ${_visocyte_client_doc_outputs}
    COMMAND "${CMAKE_COMMAND}"
            "-Dxmlpatterns=${qt_xmlpatterns_executable}"
            "-Doutput_dir=${_visocyte_client_doc_OUTPUT_DIR}"
            "-Doutput_file=${_visocyte_client_doc_OUTPUT_DIR}/${_visocyte_client_doc_TARGET}.xslt"
            "-Dxmls=${_visocyte_client_doc_xmls}"
            -D_visocyte_generate_proxy_documentation_run=ON
            -P "${_VisocyteClient_script_file}"
    DEPENDS ${_visocyte_client_doc_xmls_list}
            "${_VisocyteClient_script_file}"
            "${_VisocyteClient_cmake_dir}/visocyte_servermanager_convert_xml.xsl"
            "${_VisocyteClient_cmake_dir}/visocyte_servermanager_convert_categoryindex.xsl"
            "${_VisocyteClient_cmake_dir}/visocyte_servermanager_convert_html.xsl"
            "${_VisocyteClient_cmake_dir}/visocyte_servermanager_convert_wiki.xsl.in"
    WORKING_DIRECTORY "${_visocyte_client_doc_OUTPUT_DIR}"
    COMMENT "Generating documentation for ${_visocyte_client_doc_TARGET}")
  add_custom_target("${_visocyte_client_doc_TARGET}"
    DEPENDS
      "${_visocyte_client_doc_OUTPUT_DIR}/${_visocyte_client_doc_TARGET}.xslt"
      ${_visocyte_client_doc_outputs})
endfunction ()

# Generate proxy documentation.
if (_visocyte_generate_proxy_documentation_run AND CMAKE_SCRIPT_MODE_FILE)
  _visocyte_client_unescape_cmake_list(xmls)

  set(_visocyte_gpd_to_xml "${CMAKE_CURRENT_LIST_DIR}/visocyte_servermanager_convert_xml.xsl")
  set(_visocyte_gpd_to_catindex "${CMAKE_CURRENT_LIST_DIR}/visocyte_servermanager_convert_categoryindex.xsl")
  set(_visocyte_gpd_to_html "${CMAKE_CURRENT_LIST_DIR}/visocyte_servermanager_convert_html.xsl")
  set(_visocyte_gpd_to_wiki "${CMAKE_CURRENT_LIST_DIR}/visocyte_servermanager_convert_wiki.xsl.in")

  set(_visocyte_gpd_xslt "<xml>\n")
  file(MAKE_DIRECTORY "${output_dir}")
  foreach (_visocyte_gpd_xml IN LISTS xmls)
    execute_process(
      COMMAND "${xmlpatterns}"
              "${_visocyte_gpd_to_xml}"
              "${_visocyte_gpd_xml}"
      OUTPUT_VARIABLE _visocyte_gpd_output
      RESULT_VARIABLE _visocyte_gpd_result)
    if (_visocyte_gpd_result)
      message(FATAL_ERROR
        "Failed to convert servermanager XML")
    endif ()

    string(APPEND _visocyte_gpd_xslt
      "${_visocyte_gpd_output}")
  endforeach ()
  string(APPEND _visocyte_gpd_xslt
    "</xml>\n")

  file(WRITE "${output_file}.xslt"
    "${_visocyte_gpd_xslt}")
  execute_process(
    COMMAND "${xmlpatterns}"
            -output "${output_file}"
            "${_visocyte_gpd_to_catindex}"
            "${output_file}.xslt"
    RESULT_VARIABLE _visocyte_gpd_result)
  if (_visocyte_gpd_result)
    message(FATAL_ERROR
      "Failed to generate category index")
  endif ()

  # Generate HTML files.
  execute_process(
    COMMAND "${xmlpatterns}"
            "${_visocyte_gpd_to_html}"
            "${output_file}"
    OUTPUT_VARIABLE _visocyte_gpd_output
    RESULT_VARIABLE _visocyte_gpd_result
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  if (_visocyte_gpd_result)
    message(FATAL_ERROR
      "Failed to generate HTML output")
  endif ()

  # Escape semicolons.
  _visocyte_client_escape_cmake_list(_visocyte_gpd_output)
  # Convert into a list of HTML documents.
  string(REPLACE "</html>\n<html>" "</html>\n;<html>"  _visocyte_gpd_output "${_visocyte_gpd_output}")

  foreach (_visocyte_gpd_html_doc IN LISTS _visocyte_gpd_output)
    string(REGEX MATCH "<meta name=\"filename\" contents=\"([^\"]*)\"" _ "${_visocyte_gpd_html_doc}")
    set(_visocyte_gpd_filename "${CMAKE_MATCH_1}")
    if (NOT _visocyte_gpd_filename)
      message(FATAL_ERROR
        "No filename for an HTML output?")
    endif ()

    _visocyte_client_unescape_cmake_list(_visocyte_gpd_html_doc)

    # Replace reStructured Text markup.
    string(REGEX REPLACE "\\*\\*([^*]+)\\*\\*" "<b>\\1</b>" _visocyte_gpd_html_doc "${_visocyte_gpd_html_doc}")
    string(REGEX REPLACE "\\*([^*]+)\\*" "<em>\\1</em>" _visocyte_gpd_html_doc "${_visocyte_gpd_html_doc}")
    string(REGEX REPLACE "\n\n- " "\n<ul><li>" _visocyte_gpd_html_doc "${_visocyte_gpd_html_doc}")
    string(REGEX REPLACE "\n-" "\n<li>" _visocyte_gpd_html_doc "${_visocyte_gpd_html_doc}")
    string(REGEX REPLACE "<li>(.*)\n\n([^-])" "<li>\\1</ul>\n\\2" _visocyte_gpd_html_doc "${_visocyte_gpd_html_doc}")
    string(REGEX REPLACE "\n\n" "\n<p>\n" _visocyte_gpd_html_doc "${_visocyte_gpd_html_doc}")
    file(WRITE "${output_dir}/${_visocyte_gpd_filename}"
      "${_visocyte_gpd_html_doc}\n")
  endforeach ()

  # Generate Wiki files.
  string(REGEX MATCHALL "proxy_group=\"[^\"]*\"" _visocyte_gpd_groups "${_visocyte_gpd_xslt}")
  string(REGEX REPLACE "proxy_group=\"([^\"]*)\"" "\\1" _visocyte_gpd_groups "${_visocyte_gpd_groups}")
  list(APPEND _visocyte_gpd_groups readers)
  if (_visocyte_gpd_groups)
    list(REMOVE_DUPLICATES _visocyte_gpd_groups)
  endif ()

  foreach (_visocyte_gpd_group IN LISTS _visocyte_gpd_groups)
    if (_visocyte_gpd_group STREQUAL "readers")
      set(_visocyte_gpd_query "contains(lower-case($proxy_name),'reader')")
      set(_visocyte_gpd_group_real "sources")
    else ()
      set(_visocyte_gpd_query "not(contains(lower-case($proxy_name),'reader'))")
      set(_visocyte_gpd_group_real "${_visocyte_gpd_group}")
    endif ()

    set(_visocyte_gpd_wiki_xsl
      "${output_dir}/${_visocyte_gpd_group}.xsl")
    configure_file(
      "${_visocyte_gpd_to_wiki}"
      "${_visocyte_gpd_wiki_xsl}"
      @ONLY)
    execute_process(
      COMMAND "${xmlpatterns}"
              "${_visocyte_gpd_wiki_xsl}"
              "${output_file}"
      OUTPUT_VARIABLE _visocyte_gpd_output
      RESULT_VARIABLE _visocyte_gpd_result)
    if (_visocyte_gpd_result)
      message(FATAL_ERROR
        "Failed to generate Wiki output for ${_visocyte_gpd_group}")
    endif ()
    string(REGEX REPLACE " +" " " _visocyte_gpd_output "${_visocyte_gpd_output}")
    string(REPLACE "\n " "\n" _visocyte_gpd_output "${_visocyte_gpd_output}")
    file(WRITE "${output_dir}/${_visocyte_gpd_group}.wiki"
      "${_visocyte_gpd_output}")
  endforeach ()
endif ()

#[==[.md
## Generating help documentation

TODO: Document

```
visocyte_client_generate_help(
  NAME    <name>
  [TARGET <target>]

  [OUTPUT_DIR <directory>]
  [SOURCE_DIR <directory>]
  [PATTERNS   <pattern>...]
  [DEPENDS    <depend>...]

  [NAMESPACE  <namespace>]
  [FOLDER     <folder>]

  [TABLE_OF_CONTENTS      <toc>]
  [TABLE_OF_CONTENTS_FILE <tocfile>]

  [RESOURCE_FILE    <qrcfile>]
  [RESOURCE_PREFIX  <prefix>]
```

  * `NAME`: (Required) The basename of the generated `.qch` file.
  * `TARGET`: (Defaults to `<NAME>`) The name of the generated target.
  * `OUTPUT_DIR`: (Defaults to `${CMAKE_CURRENT_BINARY_DIR}`) Where to place
    generated files.
  * `SOURCE_DIR`: Where to copy input files from.
  * `PATTERNS`: (Defaults to `*.*`) If `SOURCE_DIR` is specified, files
    matching these globs will be copied to `OUTPUT_DIR`.
  * `DEPENDS`: A list of dependencies which are required before the help can be
    generated. Note that file paths which are generated via
    `add_custom_command` must be in the same directory as the
    `visocyte_client_generate_help` on non-Ninja generators.
  * `NAMESPACE`: (Defaults to `<NAME>.org`) The namespace for the generated
    help.
  * `FOLDER`: (Defaults to `<NAME>`) The folder for the generated help.
  * `TABLE_OF_CONTENTS` and `TABLE_OF_CONTENTS_FILE`: At most one may be
    provided. This is used as the `<toc>` element in the generated help. If not
    provided at all, a table of contents will be generated.
  * `RESOURCE_FILE`: If provided, a Qt resource file providing the contents of
    the generated help will be generated at this path. It will be available as
    `<RESOURCE_PREFIX>/<NAME>`.
  * `RESOURCE_PREFIX`: The prefix to use for the generated help's Qt resource.
#]==]
function (visocyte_client_generate_help)
  cmake_parse_arguments(_visocyte_client_help
    ""
    "NAME;TARGET;OUTPUT_DIR;SOURCE_DIR;NAMESPACE;FOLDER;TABLE_OF_CONTENTS;TABLE_OF_CONTENTS_FILE;RESOURCE_FILE;RESOURCE_PREFIX"
    "PATTERNS;DEPENDS"
    ${ARGN})

  if (_visocyte_client_help_UNPARSED_ARGUMENTS)
    message(FATAL_ERROR
      "Unparsed arguments for visocyte_client_generate_help: "
      "${_visocyte_client_help_UNPARSED_ARGUMENTS}")
  endif ()

  if (NOT DEFINED _visocyte_client_help_NAME)
    message(FATAL_ERROR
      "The `NAME` argument is required.")
  endif ()

  if (NOT DEFINED _visocyte_client_help_TARGET)
    set(_visocyte_client_help_TARGET
      "${_visocyte_client_help_NAME}")
  endif ()

  if (NOT DEFINED _visocyte_client_help_OUTPUT_DIR)
    set(_visocyte_client_help_OUTPUT_DIR
      "${CMAKE_CURRENT_BINARY_DIR}")
  endif ()

  if (NOT DEFINED _visocyte_client_help_NAMESPACE)
    set(_visocyte_client_help_NAMESPACE
      "${_visocyte_client_help_NAME}.org")
  endif ()

  if (NOT DEFINED _visocyte_client_help_FOLDER)
    set(_visocyte_client_help_FOLDER
      "${_visocyte_client_help_NAME}")
  endif ()

  if (DEFINED _visocyte_client_help_TABLE_OF_CONTENTS_FILE)
    file(READ "${_visocyte_client_help_TABLE_OF_CONTENTS_FILE}"
      _visocyte_client_help_toc)
  elseif (DEFINED _visocyte_client_help_TABLE_OF_CONTENTS)
    set(_visocyte_client_help_toc
      "${_visocyte_client_help_TABLE_OF_CONTENTS}")
  else ()
    set(_visocyte_client_help_toc)
  endif ()
  string(REPLACE "\n" " " _visocyte_client_help_toc "${_visocyte_client_help_toc}")

  if (NOT DEFINED _visocyte_client_help_PATTERNS)
    set(_visocyte_client_help_PATTERNS
      "*.*")
  endif ()

  find_package(Qt5 QUIET REQUIRED COMPONENTS Help)

  set(_visocyte_client_help_copy_sources)
  set(_visocyte_client_help_copied_sources)
  if (DEFINED _visocyte_client_help_SOURCE_DIR)
    set(_visocyte_client_help_copy_sources
      COMMAND "${CMAKE_COMMAND}" -E copy_directory
              "${_visocyte_client_help_SOURCE_DIR}"
              "${_visocyte_client_help_OUTPUT_DIR}")

    file(GLOB _visocyte_client_help_copied_sources
      ${_visocyte_client_help_PATTERNS})
  endif ()

  set(_visocyte_client_help_patterns "${_visocyte_client_help_PATTERNS}")
  _visocyte_client_escape_cmake_list(_visocyte_client_help_patterns)

  set(_visocyte_client_help_qhp
    "${_visocyte_client_help_OUTPUT_DIR}/${_visocyte_client_help_NAME}.qhp")
  set(_visocyte_client_help_output
    "${_visocyte_client_help_OUTPUT_DIR}/${_visocyte_client_help_NAME}.qch")
  add_custom_command(
    OUTPUT  "${_visocyte_client_help_output}"
    DEPENDS "${_VisocyteClient_script_file}"
            ${_visocyte_client_help_copied_sources}
            ${_visocyte_client_help_DEPENDS}
    ${_visocyte_client_help_copy_sources}
    COMMAND "${CMAKE_COMMAND}"
            "-Doutput_dir=${_visocyte_client_help_OUTPUT_DIR}"
            "-Doutput_file=${_visocyte_client_help_qhp}"
            "-Dnamespace=${_visocyte_client_help_NAMESPACE}"
            "-Dfolder=${_visocyte_client_help_FOLDER}"
            "-Dname=${_visocyte_client_help_NAME}"
            "-Dtoc=${_visocyte_client_help_toc}"
            "-Dpatterns=${_visocyte_client_help_patterns}"
            -D_visocyte_generate_help_run=ON
            -P "${_VisocyteClient_script_file}"
    VERBATIM
    COMMAND Qt5::qhelpgenerator
            "${_visocyte_client_help_qhp}"
            -s
            -o "${_visocyte_client_help_output}"
    COMMENT "Compiling Qt help for ${_visocyte_client_help_NAME}"
    WORKING_DIRECTORY "${_visocyte_client_help_OUTPUT_DIR}")
  add_custom_target("${_visocyte_client_help_TARGET}"
    DEPENDS
      "${_visocyte_client_help_output}")

  if (DEFINED _visocyte_client_help_RESOURCE_FILE)
    if (NOT DEFINED _visocyte_client_help_RESOURCE_PREFIX)
      message(FATAL_ERROR
        "The `RESOURCE_PREFIX` argument is required if `RESOURCE_FILE` is given.")
    endif ()

    visocyte_client_qt_resource(
      OUTPUT  "${_visocyte_client_help_RESOURCE_FILE}"
      PREFIX  "${_visocyte_client_help_RESOURCE_PREFIX}"
      FILE    "${_visocyte_client_help_output}")
    set_property(SOURCE "${_visocyte_client_help_RESOURCE_FILE}"
      PROPERTY
        OBJECT_DEPENDS "${_visocyte_client_help_output}")
  endif ()
endfunction ()

# Handle the generation of the help file.
if (_visocyte_generate_help_run AND CMAKE_SCRIPT_MODE_FILE)
  _visocyte_client_unescape_cmake_list(patterns)

  set(_visocyte_help_patterns)
  foreach (_visocyte_help_pattern IN LISTS patterns)
    if (IS_ABSOLUTE "${_visocyte_help_pattern}")
      list(APPEND _visocyte_help_patterns
        "${_visocyte_help_pattern}")
    else ()
      list(APPEND _visocyte_help_patterns
        "${output_dir}/${_visocyte_help_pattern}")
    endif ()
  endforeach ()

  file(GLOB _visocyte_help_files
    RELATIVE "${output_dir}"
    ${_visocyte_help_patterns})

  if (NOT toc)
    if (NOT _visocyte_help_files)
      message(FATAL_ERROR
        "No matching files given without a table of contents")
    endif ()
    set(_visocyte_help_subsections "")
    list(GET _visocyte_help_files 0
      _visocyte_help_index)
    set(_visocyte_help_subsections "")
    foreach (_visocyte_help_file IN LISTS _visocyte_help_files)
      if (NOT _visocyte_help_file MATCHES "\\.html$")
        continue ()
      endif ()
      get_filename_component(_visocyte_help_name "${_visocyte_help_file}" NAME_WE)
      set(_visocyte_help_title "${_visocyte_help_name}")
      file(READ "${_visocyte_help_file}" _visocyte_help_contents)
      string(REGEX MATCH "<title>([^<]*)</title>" _ "${_visocyte_help_contents}")
      if (CMAKE_MATCH_1)
        set(_visocyte_help_title "${CMAKE_MATCH_1}")
      endif ()
      string(APPEND _visocyte_help_subsections
        "    <section title=\"${_visocyte_help_title}\" ref=\"${_visocyte_help_file}\" />\n")

      string(TOLOWER "${_visocyte_help_name}" _visocyte_help_name_lower)
      if (_visocyte_help_name_lower STREQUAL "index")
        set(_visocyte_help_index
          "${_visocyte_help_file}")
      endif ()
    endforeach ()
    set(toc
      "<toc>\n  <section title=\"${name}\" ref=\"${_visocyte_help_index}\">\n${_visocyte_help_subsections}  </section>\n</toc>")
  endif ()

  set(_visocyte_help_file_entries "")
  foreach (_visocyte_help_file IN LISTS _visocyte_help_files)
    string(APPEND _visocyte_help_file_entries
      "      <file>${_visocyte_help_file}</file>\n")
  endforeach ()

  file(WRITE "${output_file}"
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>
<QtHelpProject version=\"1.0\">
  <namespace>${namespace}</namespace>
  <virtualFolder>${folder}</virtualFolder>
  <filterSection>
    ${toc}
    <keywords>
      <!-- TODO: how to handle keywords? -->
    </keywords>
    <files>
${_visocyte_help_file_entries}
    </files>
  </filterSection>
</QtHelpProject>\n")
endif ()

#[==[.md
## Qt resources

Compiling Qt resources into a client can be a little tedious. To help with
this, some functions are provided to make it easier to embed content into the
client.
#]==]

#[==[.md
### Single file

```
visocyte_client_qt_resource(
  OUTPUT  <file>
  PREFIX  <prefix>
  FILE    <file>
  [ALIAS  <alias>])
```

Outputs a Qt resource to the file given to the `OUTPUT` argument. Its resource
name is `<PREFIX>/<ALIAS>`. The contents are copied from the contents of the
file specified by the `FILE` argument. If not given the name of the file is
used as the `ALIAS`.
#]==]
function (visocyte_client_qt_resource)
  cmake_parse_arguments(_visocyte_client_resource
    ""
    "OUTPUT;PREFIX;ALIAS;FILE"
    ""
    ${ARGN})

  if (_visocyte_client_resource_UNPARSED_ARGUMENTS)
    message(FATAL_ERROR
      "Unparsed arguments for visocyte_client_qt_resource: "
      "${_visocyte_client_resource_UNPARSED_ARGUMENTS}")
  endif ()

  if (NOT DEFINED _visocyte_client_resource_OUTPUT)
    message(FATAL_ERROR
      "The `OUTPUT` argument is required.")
  endif ()

  if (NOT DEFINED _visocyte_client_resource_PREFIX)
    message(FATAL_ERROR
      "The `PREFIX` argument is required.")
  endif ()

  if (NOT DEFINED _visocyte_client_resource_FILE)
    message(FATAL_ERROR
      "The `FILE` argument is required.")
  endif ()

  if (NOT DEFINED _visocyte_client_resource_ALIAS)
    get_filename_component(_visocyte_client_resource_ALIAS
      "${_visocyte_client_resource_FILE}"
      NAME)
  endif ()

  get_filename_component(_visocyte_client_resource_file_path
    "${_visocyte_client_resource_FILE}"
    ABSOLUTE)
  get_filename_component(_visocyte_client_resource_file_path
    "${_visocyte_client_resource_file_path}"
    REALPATH)
  file(TO_NATIVE_PATH
    "${_visocyte_client_resource_file_path}"
    _visocyte_client_resource_file_path)

  # We cannot use file(GENERATE) because automoc doesn't like when generated
  # sources are in the source list.
  file(WRITE "${_visocyte_client_resource_OUTPUT}.tmp"
    "<RCC>
  <qresource prefix=\"/${_visocyte_client_resource_PREFIX}\">
    <file alias=\"${_visocyte_client_resource_ALIAS}\">${_visocyte_client_resource_file_path}</file>
  </qresource>
</RCC>\n")
  configure_file(
    "${_visocyte_client_resource_OUTPUT}.tmp"
    "${_visocyte_client_resource_OUTPUT}"
    COPYONLY)
endfunction ()

#[==[.md
### Many files

```
visocyte_client_qt_resources(
  OUTPUT  <file>
  PREFIX  <prefix>
  FILES   <file>...)
```

Outputs a Qt resource to the file given to the `OUTPUT` argument. Its resource
name is `<PREFIX>/<filename>` for each of the files in the given list. If
aliases other than the filenames are required, the
`visocyte_client_qt_resource` function should be used instead.
#]==]
function (visocyte_client_qt_resources)
  cmake_parse_arguments(_visocyte_client_resources
    ""
    "OUTPUT;PREFIX"
    "FILES"
    ${ARGN})

  if (_visocyte_client_resources_UNPARSED_ARGUMENTS)
    message(FATAL_ERROR
      "Unparsed arguments for visocyte_client_qt_resources: "
      "${_visocyte_client_resources_UNPARSED_ARGUMENTS}")
  endif ()

  if (NOT DEFINED _visocyte_client_resources_OUTPUT)
    message(FATAL_ERROR
      "The `OUTPUT` argument is required.")
  endif ()

  if (NOT DEFINED _visocyte_client_resources_PREFIX)
    message(FATAL_ERROR
      "The `PREFIX` argument is required.")
  endif ()

  if (NOT DEFINED _visocyte_client_resources_FILES)
    message(FATAL_ERROR
      "The `FILES` argument is required.")
  endif ()

  set(_visocyte_client_resources_contents)

  string(APPEND _visocyte_client_resources_contents
    "<RCC>\n  <qresource prefix=\"${_visocyte_client_resources_PREFIX}\">\n")
  foreach (_visocyte_client_resources_file IN LISTS _visocyte_client_resources_FILES)
    get_filename_component(_visocyte_client_resources_alias
      "${_visocyte_client_resources_file}"
      NAME)
    get_filename_component(_visocyte_client_resources_file_path
      "${_visocyte_client_resources_file}"
      ABSOLUTE)
    get_filename_component(_visocyte_client_resources_file_path
      "${_visocyte_client_resources_file_path}"
      REALPATH)
    file(TO_NATIVE_PATH
      "${_visocyte_client_resources_file_path}"
      _visocyte_client_resources_file_path)
    string(APPEND _visocyte_client_resources_contents
      "    <file alias=\"${_visocyte_client_resources_alias}\">${_visocyte_client_resources_file_path}</file>\n")
  endforeach ()
  string(APPEND _visocyte_client_resources_contents
    "  </qresource>\n</RCC>\n")

  # We cannot use file(GENERATE) because automoc doesn't like when generated
  # sources are in the source list.
  file(WRITE "${_visocyte_client_resources_OUTPUT}.tmp"
    "${_visocyte_client_resources_contents}")
  configure_file(
    "${_visocyte_client_resources_OUTPUT}.tmp"
    "${_visocyte_client_resources_OUTPUT}"
    COPYONLY)
endfunction ()
