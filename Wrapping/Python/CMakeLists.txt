set(python_files
  visocyte/annotation.py
  visocyte/_backwardscompatibilityhelper.py
  visocyte/benchmark/basic.py
  visocyte/benchmark/__init__.py
  visocyte/benchmark/logbase.py
  visocyte/benchmark/logparser.py
  visocyte/benchmark/manyspheres.py
  visocyte/benchmark/waveletcontour.py
  visocyte/benchmark/waveletvolume.py
  visocyte/calculator.py
  visocyte/collaboration.py
  visocyte/_colorMaps.py
  visocyte/coprocessing.py
  visocyte/cpexport.py
  visocyte/cpstate.py
  visocyte/demos/demo1.py
  visocyte/demos/show_grid_as_background.py
  visocyte/detail/__init__.py
  visocyte/detail/pythonalgorithm.py
  visocyte/extract_selection.py
  visocyte/__init__.py
  visocyte/lookuptable.py
  visocyte/numeric.py
  visocyte/numpy_support.py
  visocyte/pv-vtk-all.py
  visocyte/python_selector.py
  visocyte/python_view.py
  visocyte/servermanager.py
  visocyte/simple.py
  visocyte/smstate.py
  visocyte/smtesting.py
  visocyte/smtrace.py
  visocyte/spatiotemporalparallelism.py
  visocyte/util/__init__.py
  visocyte/util/vtkAlgorithm.py
  visocyte/variant.py
  visocyte/vtkConstants.py
  visocyte/vtk.py)

set(python_copied_modules)
foreach (python_file IN LISTS python_files)
  set(output_python_file
    "${CMAKE_BINARY_DIR}/${VISOCYTE_PYTHON_SITE_PACKAGES_SUFFIX}/${python_file}")
  add_custom_command(
    OUTPUT  "${output_python_file}"
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${python_file}"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/${python_file}"
            "${output_python_file}"
    COMMENT "Copying ${python_file} to the binary directory")
  get_filename_component(python_file_directory "${python_file}" DIRECTORY)
  install(
    FILES       "${python_file}"
    DESTINATION "${VISOCYTE_PYTHON_SITE_PACKAGES_SUFFIX}/${python_file_directory}"
    COMPONENT   "python")
  list(APPEND python_copied_modules
    "${output_python_file}")
endforeach ()
add_custom_target(visocyte_python_copy ALL
  DEPENDS
    ${python_copied_modules})
