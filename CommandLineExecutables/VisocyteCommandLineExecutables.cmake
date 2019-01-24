function (visocyte_add_executable name)
  add_executable("${name}" ${ARGN})
  add_executable("Visocyte::${name}" ALIAS "${name}")

  target_link_libraries("${name}"
    PRIVATE
      Visocyte::ServerManagerApplication)

  if (NOT BUILD_SHARED_LIBS)
    target_link_libraries("${name}"
      PRIVATE
        visocyte_plugins_static)
  endif ()

  if (PARAVIEW_ENABLE_PYTHON)
    target_link_libraries("${name}"
      PRIVATE
        Visocyte::pvpythonmodules
        Visocyte::PythonInitializer)
  endif ()

  if (visocyte_exe_job_link_pool)
    set_property(TARGET "${name}"
      PROPERTY
        JOB_POOL_LINK "${visocyte_exe_job_link_pool}")
  endif ()

  install(
    TARGETS     "${name}"
    DESTINATION bin
    COMPONENT   runtime
    EXPORT      Visocyte)
endfunction ()
