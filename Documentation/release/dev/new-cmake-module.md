# New CMake Module System

VTK's module system has been updated and Visocyte now works with it. CMake
variables have changed names:

  - `Module_X` becomes `VTK_MODULE_ENABLE_M` where X is the "library name"
    (e.g., `vtkPVCommon` and `M` is the sanitized module name (e.g.,
    `Visocyte::Common`'s sanitized name is `Visocyte_Common`).
  - `VISOCYTE_BUILD_PLUGIN_P` becomes `VISOCYTE_PLUGIN_ENABLE_P`. Enabling a
    plugin now requests that its required modules are built.
  - Visocyte's flags forcefully disable some modules. For example, Visocyte
    can no longer be built with `VISOCYTE_USE_MPI` and still build some
    MPI-releated modules.

## Server Manager XML

Server Manager XML files may now be attached to modules using
`visocyte_server_manager_add_xmls`:

```cmake
visocyte_server_manager_add_xmls(
  XMLS mysm.xml)
```

for the current module or:

```cmake
visocyte_server_manager_add_xmls(
  MODULE Some::VTKModule
  XMLS mysm.xml)
```

to attach it to an external module.

Server Manager XML contents are then provided via the
`visocyte_server_manager_process` function:

```cmake
visocyte_server_manager_process(
  MODULES module1 module2
  TARGET  a_target_name
  XML_FILES list_of_xml_files_variable)
```

which provides a target which may be linked to in order to access the XML files
attached to the given modules. See `CMake/VisocyteServerManager.cmake` for
details.

## Plugins

Visocyte's plugin CMake API has also been improved. Documentation is in
`CMake/VisocytePlugin.cmake`. The biggest change is that for a plugin to
provide its own classes which are used from Server Manager XML files, the
classes must be part of a VTK module. Visocyte's plugins have been updated to
use this new pattern, so looking in `Plugins` for examples is recommended.

In shared builds, plugins are now built using `add_library(MODULE)` which
means that they may not be directly linked to. When building clients, there
are options to specify plugins to load on startup.

In static builds, the `visocyte_plugin_build`'s `TARGET` parameter creates a
target which may be linked to in order to initialize the built plugins. See
its documentation for details.

## Clients

The CMake API has been updated to be use CMake targets and stricter argument
parsing. See `CMake/VisocyteClient.cmake` for documentation.
