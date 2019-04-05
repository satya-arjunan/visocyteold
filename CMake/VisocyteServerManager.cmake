#[==[.md
# Server Manager XMLs

Visocyte uses XML files to describe filters available in its user interface.
Modules may add filters to the UI by providing XML files.

TODO: Document the ServerManager XML format.
#]==]

#[==[.md
## Adding XMLs to modules

Modules may have associated XML files. They can be added to the module target
using this function.

```
visocyte_add_server_manager_xmls(
  XMLS <xml>...)
```
#]==]
function (visocyte_add_server_manager_xmls)
  cmake_parse_arguments(_visocyte_add_sm
    ""
    "MODULE"
    "XMLS"
    ${ARGN})

  if (_visocyte_add_sm_UNPARSED_ARGUMENTS)
    message(FATAL_ERROR
      "Unparsed arguments for visocyte_add_server_manager_xmls: "
      "${_visocyte_add_sm_UNPARSED_ARGUMENTS}")
  endif ()

  if (NOT _visocyte_add_sm_XMLS)
    message(FATAL_ERROR
      "The `XMLS` argument is required.")
  endif ()

  if (NOT DEFINED _visocyte_add_sm_MODULE)
    set(_visocyte_add_sm_MODULE "${_vtk_build_module}")
  endif ()

  foreach (_visocyte_add_sm_xml IN LISTS _visocyte_add_sm_XMLS)
    if (NOT IS_ABSOLUTE "${_visocyte_add_sm_xml}")
      set(_visocyte_add_sm_xml "${CMAKE_CURRENT_SOURCE_DIR}/${_visocyte_add_sm_xml}")
    endif ()

    _vtk_module_set_module_property("${_visocyte_add_sm_MODULE}" APPEND
      PROPERTY  "visocyte_server_manager_xml"
      VALUE     "${_visocyte_add_sm_xml}")
  endforeach ()
endfunction ()

#[==[.md
## Building XML files

There are two functions offered to build server manager XML files. The first
uses modules:

```
visocyte_server_manager_process(
  MODULES <module>...
  TARGET <target>
  [FILES <file>...]
  [XML_FILES  <variable>])
```

The `MODULES` argument contains the modules to include in the server manager
target. The function gathers the XML files declared using
`visocyte_add_server_manager_xmls` and then calls
`visocyte_server_manager_process_files`. If additional, non-module related XML
files are required, they may be passed via `FILES`.

If `XML_FILES` is given, the list of process XML files are set on the given
variable.
#]==]
function (visocyte_server_manager_process)
  cmake_parse_arguments(_visocyte_sm_process
    ""
    "TARGET;XML_FILES"
    "MODULES;FILES"
    ${ARGN})

  if (_visocyte_sm_process_UNPARSED_ARGUMENTS)
    message(FATAL_ERROR
      "Unparsed arguments for visocyte_server_manager_process: "
      "${_visocyte_sm_process_UNPARSED_ARGUMENTS}")
  endif ()

  if (NOT _visocyte_sm_process_MODULES)
    message(FATAL_ERROR
      "The `MODULES` argument is required.")
  endif ()

  if (NOT DEFINED _visocyte_sm_process_TARGET)
    message(FATAL_ERROR
      "The `TARGET` argument is required.")
  endif ()

  set(_visocyte_sm_process_files)
  foreach (_visocyte_sm_process_module IN LISTS _visocyte_sm_process_MODULES)
    _vtk_module_get_module_property("${_visocyte_sm_process_module}"
      PROPERTY  "visocyte_server_manager_xml"
      VARIABLE  _visocyte_sm_process_module_files)
    list(APPEND _visocyte_sm_process_files
      ${_visocyte_sm_process_module_files})
  endforeach ()

  list(APPEND _visocyte_sm_process_files
    ${_visocyte_sm_process_FILES})

  visocyte_server_manager_process_files(
    TARGET  ${_visocyte_sm_process_TARGET}
    FILES   ${_visocyte_sm_process_files})

  if (DEFINED _visocyte_sm_process_XML_FILES)
    set("${_visocyte_sm_process_XML_FILES}"
      "${_visocyte_sm_process_files}"
      PARENT_SCOPE)
  endif ()
endfunction ()

#[==[.md
The second way to process XML files directly.

``
visocyte_server_manager_process_files(
  FILES <file>...
  TARGET <target>)
```

The files passed to the `FILES` argument will be processed in to functions
which are then consumed by Visocyte applications.

The name of the target is given to the `TARGET` argument. By default, the
filename is `<TARGET>.h` and it contains a function named
`<TARGET>_initialize`. They may be changed using the `FILE_NAME` and
`FUNCTION_NAME` arguments. The target has an interface usage requirement that
will allow the generated header to be included.
#]==]
function (visocyte_server_manager_process_files)
  cmake_parse_arguments(_visocyte_sm_process_files
    ""
    "TARGET"
    "FILES"
    ${ARGN})

  if (_visocyte_sm_process_files_UNPARSED_ARGUMENTS)
    message(FATAL_ERROR
      "Unparsed arguments for visocyte_server_manager_process_files: "
      "${_visocyte_sm_process_files_UNPARSED_ARGUMENTS}")
  endif ()

  if (NOT DEFINED _visocyte_sm_process_files_TARGET)
    message(FATAL_ERROR
      "The `TARGET` argument is required.")
  endif ()

  set(_visocyte_sm_process_files_output_dir
    "${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/${_visocyte_sm_process_files_TARGET}")
  set(_visocyte_sm_process_files_output
    "${_visocyte_sm_process_files_output_dir}/${_visocyte_sm_process_files_TARGET}_data.h")
  add_custom_command(
    OUTPUT  "${_visocyte_sm_process_files_output}"
    DEPENDS ${_visocyte_sm_process_files_FILES}
            Visocyte::ProcessXML
    COMMAND Visocyte::ProcessXML
            "${_visocyte_sm_process_files_output}"
            "${_visocyte_sm_process_files_TARGET}"
            "Interface"
            "GetInterfaces"
            ${_visocyte_sm_process_files_FILES}
    COMMENT "Generating server manager headers for ${_visocyte_sm_process_files_TARGET}.")
  add_custom_target("${_visocyte_sm_process_files_TARGET}_xml_content"
    DEPENDS
      "${_visocyte_sm_process_files_output}")

  set(_visocyte_sm_process_files_init_content
    "#ifndef ${_visocyte_sm_process_files_TARGET}_h
#define ${_visocyte_sm_process_files_TARGET}_h

#include \"${_visocyte_sm_process_files_TARGET}_data.h\"
#include <string>
#include <vector>

void ${_visocyte_sm_process_files_TARGET}_initialize(std::vector<std::string>& xmls)
{\n  (void)xmls;\n")
  foreach (_visocyte_sm_process_files_file IN LISTS _visocyte_sm_process_files_FILES)
    get_filename_component(_visocyte_sm_process_files_name "${_visocyte_sm_process_files_file}" NAME_WE)
    string(APPEND _visocyte_sm_process_files_init_content
      "  {
    char *init_string = ${_visocyte_sm_process_files_TARGET}${_visocyte_sm_process_files_name}GetInterfaces();
    xmls.push_back(init_string);
    delete [] init_string;
  }\n")
  endforeach ()
  string(APPEND _visocyte_sm_process_files_init_content
    "}

#endif\n")

  file(GENERATE
    OUTPUT  "${_visocyte_sm_process_files_output_dir}/${_visocyte_sm_process_files_TARGET}.h"
    CONTENT "${_visocyte_sm_process_files_init_content}")

  add_library("${_visocyte_sm_process_files_TARGET}" INTERFACE)
  add_dependencies("${_visocyte_sm_process_files_TARGET}"
    "${_visocyte_sm_process_files_TARGET}_xml_content")
  target_include_directories("${_visocyte_sm_process_files_TARGET}"
    INTERFACE
      "$<BUILD_INTERFACE:${_visocyte_sm_process_files_output_dir}>")
  _vtk_module_apply_properties("${_visocyte_sm_process_files_TARGET}")
  _vtk_module_install("${_visocyte_sm_process_files_TARGET}")
endfunction ()
