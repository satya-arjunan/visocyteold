function (visocyte_add_test_python)
  set(_vtk_testing_python_exe
    "$<TARGET_FILE:ParaView::pvpython>"
    -dr
    ${visocyte_python_args})
  vtk_add_test_python(${ARGN})
endfunction ()

function (visocyte_add_test_python_mpi)
  set(_vtk_testing_python_exe
    "$<TARGET_FILE:ParaView::pvpython>"
    -dr
    ${visocyte_python_args})
  vtk_add_test_python_mpi(${ARGN})
endfunction ()

function (visocyte_add_test_pvbatch)
  set(_vtk_testing_python_exe
    "$<TARGET_FILE:ParaView::pvbatch>"
    -dr
    ${visocyte_pvbatch_args})
  set(vtk_test_prefix "Batch-${vtk_test_prefix}")
  vtk_add_test_python(${ARGN})
endfunction ()

function (visocyte_add_test_pvbatch_mpi)
  set(_vtk_testing_python_exe
    "$<TARGET_FILE:ParaView::pvbatch>"
    -dr
    ${visocyte_pvbatch_args})
  set(vtk_test_prefix "Batch-${vtk_test_prefix}")
  vtk_add_test_python_mpi(${ARGN})
endfunction ()

function(visocyte_add_test_driven)
  if (NOT (TARGET pvserver AND TARGET pvpython))
    return()
  endif ()
  set(_vtk_testing_python_exe "$<TARGET_FILE:ParaView::smTestDriver>")
  list(APPEND VTK_PYTHON_ARGS
    --server $<TARGET_FILE:ParaView::pvserver>
    --client $<TARGET_FILE:ParaView::pvpython> -dr)
  vtk_add_test_python(${ARGN})
endfunction ()

function (_visocyte_add_tests function)
  cmake_parse_arguments(_visocyte_add_tests
    "FORCE_SERIAL;FORCE_LOCK"
    "LOAD_PLUGIN;PLUGIN_PATH;CLIENT;TEST_DIRECTORY;TEST_DATA_TARGET;PREFIX;SUFFIX;_ENABLE_SUFFIX;_DISABLE_SUFFIX;BASELINE_DIR;DATA_DIRECTORY"
    "_COMMAND_PATTERN;TEST_SCRIPTS;ENVIRONMENT;ARGS;CLIENT_ARGS"
    ${ARGN})

  if (_visocyte_add_tests_UNPARSED_ARGUMENTS)
    message(FATAL_ERROR
      "Unparsed arguments for ${function}: "
      "${_visocyte_add_tests_UNPARSED_ARGUMENTS}")
  endif ()

  if (NOT DEFINED _visocyte_add_tests__COMMAND_PATTERN)
    message(FATAL_ERROR
      "The `_COMMAND_PATTERN` argument is required.")
  endif ()

  if (NOT DEFINED _visocyte_add_tests_TEST_DATA_TARGET)
    if (DEFINED _visocyte_add_tests_default_test_data_target)
      set(_visocyte_add_tests_TEST_DATA_TARGET
        "${_visocyte_add_tests_default_test_data_target}")
    else ()
      message(FATAL_ERROR
        "The `TEST_DATA_TARGET` argument is required.")
    endif ()
  endif ()

  if (NOT DEFINED _visocyte_add_tests_CLIENT)
    set(_visocyte_add_tests_CLIENT
      "$<TARGET_FILE:ParaView::visocyte>")
  endif ()

  if (NOT DEFINED _visocyte_add_tests_PREFIX)
    set(_visocyte_add_tests_PREFIX "pv")
  endif ()

  if (NOT DEFINED _visocyte_add_tests_TEST_DIRECTORY)
    set(_visocyte_add_tests_TEST_DIRECTORY
      "${CMAKE_BINARY_DIR}/Testing/Temporary")
  endif ()

  if (NOT DEFINED _visocyte_add_tests_DATA_DIRECTORY AND DEFINED _visocyte_add_tests_default_data_directory)
    set(_visocyte_add_tests_DATA_DIRECTORY
      "${_visocyte_add_tests_default_data_directory}")
  endif ()

  set(_visocyte_add_tests_args
    ${_visocyte_add_tests_ARGS})

  if (DEFINED _visocyte_add_tests_PLUGIN_PATH)
    list(APPEND _visocyte_add_tests_args
      "--test-plugin-path=${_visocyte_add_tests_PLUGIN_PATH}")
  endif ()

  if (DEFINED _visocyte_add_tests_LOAD_PLUGIN)
    list(APPEND _visocyte_add_tests_args
      "--test-plugin=${_visocyte_add_tests_LOAD_PLUGIN}")
  endif ()

  string(REPLACE "__visocyte_args__" "${_visocyte_add_tests_args}"
    _visocyte_add_tests__COMMAND_PATTERN
    "${_visocyte_add_tests__COMMAND_PATTERN}")
  string(REPLACE "__visocyte_client__" "${_visocyte_add_tests_CLIENT}"
    _visocyte_add_tests__COMMAND_PATTERN
    "${_visocyte_add_tests__COMMAND_PATTERN}")

  foreach (_visocyte_add_tests_script IN LISTS _visocyte_add_tests_TEST_SCRIPTS)
    if (NOT IS_ABSOLUTE "${_visocyte_add_tests_script}")
      set(_visocyte_add_tests_script
        "${CMAKE_CURRENT_SOURCE_DIR}/${_visocyte_add_tests_script}")
    endif ()
    get_filename_component(_visocyte_add_tests_name "${_visocyte_add_tests_script}" NAME_WE)
    set(_visocyte_add_tests_name_base "${_visocyte_add_tests_name}")
    string(APPEND _visocyte_add_tests_name "${_visocyte_add_tests_SUFFIX}")

    if (DEFINED _visocyte_add_tests__DISABLE_SUFFIX AND ${_visocyte_add_tests_name}${_visocyte_add_tests__DISABLE_SUFFIX})
      continue ()
    endif ()

    if (DEFINED _visocyte_add_tests__ENABLE_SUFFIX AND NOT ${_visocyte_add_tests_name}${_visocyte_add_tests__ENABLE_SUFFIX})
      continue ()
    endif ()

    # Build arguments to pass to the clients.
    set(_visocyte_add_tests_client_args
      "--test-directory=${_visocyte_add_tests_TEST_DIRECTORY}"
      ${_visocyte_add_tests_CLIENT_ARGS})
    if (DEFINED _visocyte_add_tests_BASELINE_DIR)
      if (DEFINED "${_visocyte_add_tests_name}_BASELINE")
        list(APPEND _visocyte_add_tests_client_args
          "--test-baseline=DATA{${_visocyte_add_tests_BASELINE_DIR}/${${_visocyte_add_tests_name_base}_BASELINE}}")
      else ()
        list(APPEND _visocyte_add_tests_client_args
          "--test-baseline=DATA{${_visocyte_add_tests_BASELINE_DIR}/${_visocyte_add_tests_name_base}.png}")
      endif ()
      ExternalData_Expand_Arguments("${_visocyte_add_tests_TEST_DATA_TARGET}" _
        "DATA{${_visocyte_add_tests_BASELINE_DIR}/,REGEX:${_visocyte_add_tests_name_base}(-.*)?(_[0-9]+)?.png}")
    endif ()
    if (DEFINED "${_visocyte_add_tests_name}_BASELINE")
      list(APPEND _visocyte_add_tests_client_args
        "--test-threshold=${${_visocyte_add_tests_name}_THRESHOLD}")
    endif ()
    if (DEFINED _visocyte_add_tests_DATA_DIRECTORY)
      list(APPEND _visocyte_add_tests_client_args
        "--data-directory=${_visocyte_add_tests_DATA_DIRECTORY}")
    endif ()

    string(REPLACE "__visocyte_script__" "--test-script=${_visocyte_add_tests_script}"
      _visocyte_add_tests_script_args
      "${_visocyte_add_tests__COMMAND_PATTERN}")
    string(REPLACE "__visocyte_client_args__" "${_visocyte_add_tests_client_args}"
      _visocyte_add_tests_script_args
      "${_visocyte_add_tests_script_args}")
    string(REPLACE "__visocyte_test_name__" "${_visocyte_add_tests_name_base}"
      _visocyte_add_tests_script_args
      "${_visocyte_add_tests_script_args}")

    ExternalData_add_test("${_visocyte_add_tests_TEST_DATA_TARGET}"
      NAME    "${_visocyte_add_tests_PREFIX}.${_visocyte_add_tests_name}"
      COMMAND ParaView::smTestDriver
              --enable-bt
              ${_visocyte_add_tests_script_args})
    set_property(TEST "${_visocyte_add_tests_PREFIX}.${_visocyte_add_tests_name}"
      PROPERTY
        LABELS ParaView)
    set_property(TEST "${_visocyte_add_tests_PREFIX}.${_visocyte_add_tests_name}"
      PROPERTY
        ENVIRONMENT "${_visocyte_add_tests_ENVIRONMENT}")
    if (${_visocyte_add_tests_name}_FORCE_SERIAL OR _visocyte_add_tests_FORCE_SERIAL)
      set_property(TEST "${_visocyte_add_tests_PREFIX}.${_visocyte_add_tests_name}"
        PROPERTY
          RUN_SERIAL ON)
    else ()
      # if the XML test contains VISOCYTE_TEST_ROOT we assume that we may be writing
      # to that file and reading it back in so we add a resource lock on the XML
      # file so that the pv.X, pvcx.X and pvcrs.X tests don't run simultaneously.
      # we only need to do this if the test isn't forced to be serial already.
      if (NOT ${_visocyte_add_tests_name}_FORCE_LOCK)
        file(STRINGS "${_visocyte_add_tests_script}" _visocyte_add_tests_visocyte_test_root REGEX VISOCYTE_TEST_ROOT)
      endif ()
      if (${_visocyte_add_tests_name}_FORCE_LOCK OR _visocyte_add_tests_visocyte_test_root)
        set_property(TEST "${_visocyte_add_tests_PREFIX}.${_visocyte_add_tests_name}"
          PROPERTY
            RESOURCE_LOCK "${_visocyte_add_tests_script}")
      endif ()
    endif ()
  endforeach ()
endfunction ()

function (visocyte_add_client_tests)
  _visocyte_add_tests("visocyte_add_client_tests"
    PREFIX "pv"
    _DISABLE_SUFFIX "_DISABLE_C"
    _COMMAND_PATTERN
      --client __visocyte_client__
        --enable-bt
        __visocyte_args__
        __visocyte_script__
        __visocyte_client_args__
        -dr
        --exit
    ${ARGN})
endfunction ()

function (visocyte_add_client_server_tests)
  _visocyte_add_tests("visocyte_add_client_server_tests"
    PREFIX "pvcs"
    _DISABLE_SUFFIX "_DISABLE_CS"
    _COMMAND_PATTERN
      --server "$<TARGET_FILE:ParaView::pvserver>"
        --enable-bt
        __visocyte_args__
      --client __visocyte_client__
        --enable-bt
        __visocyte_args__
        __visocyte_script__
        __visocyte_client_args__
        -dr
        --exit
    ${ARGN})
endfunction ()

function (visocyte_add_client_server_render_tests)
  _visocyte_add_tests("visocyte_add_client_server_render_tests"
    PREFIX "pvcrs"
    _DISABLE_SUFFIX "_DISABLE_CRS"
    _COMMAND_PATTERN
      --data-server "$<TARGET_FILE:ParaView::pvdataserver>"
        --enable-bt
        __visocyte_args__
      --render-server "$<TARGET_FILE:ParaView::pvrenderserver>"
        --enable-bt
        __visocyte_args__
      --client __visocyte_client__
        --enable-bt
        __visocyte_args__
        __visocyte_script__
        __visocyte_client_args__
        -dr
        --exit
    ${ARGN})
endfunction ()

function (visocyte_add_multi_client_tests)
  _visocyte_add_tests("visocyte_add_multi_client_tests"
    PREFIX "pvcs-multi-clients"
    _ENABLE_SUFFIX "_ENABLE_MULTI_CLIENT"
    _COMMAND_PATTERN
      --test-multi-clients
      --server "$<TARGET_FILE:ParaView::pvserver>"
        --enable-bt
        __visocyte_args__
      --client __visocyte_client__
        --enable-bt
        __visocyte_args__
        __visocyte_script__
        __visocyte_client_args__
        --test-master
        -dr
        --exit
      --client __visocyte_client__
        --enable-bt
        __visocyte_args__
        __visocyte_client_args__
        --test-slave
        -dr
        --exit
    ${ARGN})
endfunction ()

function (visocyte_add_multi_server_tests count)
  _visocyte_add_tests("visocyte_add_multi_server_tests"
    PREFIX "pvcs-multi-servers"
    SUFFIX "-${count}"
    _COMMAND_PATTERN
      --test-multi-servers "${count}"
      --server "$<TARGET_FILE:ParaView::pvserver>"
        --enable-bt
      --client __visocyte_client__
        --enable-bt
        __visocyte_args__
        __visocyte_script__
        __visocyte_client_args__
        -dr
        --exit
    ${ARGN})
endfunction ()

function (visocyte_add_tile_display_tests width height)
  if (NOT VISOCYTE_USE_MPI)
    return ()
  endif ()

  math(EXPR _visocyte_add_tile_display_cpu_count "${width} * ${height} - 1")

  _visocyte_add_tests("visocyte_add_tile_display_tests"
    FORCE_SERIAL
    PREFIX "pvcs-tile-display"
    SUFFIX "-${width}x${height}"
    ENVIRONMENT
      PV_ICET_WINDOW_BORDERS=1
    _COMMAND_PATTERN
      --test-tiled "${width}" "${height}"
      --server "$<TARGET_FILE:ParaView::pvserver>"
        --enable-bt
      --client __visocyte_client__
        --enable-bt
        __visocyte_args__
        __visocyte_script__
        __visocyte_client_args__
        "--tile-image-prefix=${CMAKE_BINARY_DIR}/Testing/Temporary/__visocyte_test_name__"
        -dr
        --exit
    ${ARGN})
endfunction ()
