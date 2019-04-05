option(VISOCYTE_CLIENT_RENDER_SERVER_TESTS
  "Turn on/off client tests that involve separate data and render servers."
  ON)
mark_as_advanced(VISOCYTE_CLIENT_RENDER_SERVER_TESTS)

# Set these to make the testing macros work properly.
set(_vtk_build_TEST_OUTPUT_DATA_DIRECTORY "${visocyte_test_data_directory_output}")
set(_vtk_build_TEST_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Testing/Temporary")
set(_vtk_build_TEST_DATA_TARGET "VisocyteData")
set(VISOCYTE_TEST_BASELINE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Data/Baseline")

add_subdirectory(XML)
if (VISOCYTE_ENABLE_PYTHON)
  add_subdirectory(Python)
endif ()
