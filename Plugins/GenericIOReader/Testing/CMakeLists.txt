if (VISOCYTE_BUILD_QT_GUI AND BUILD_SHARED_LIBS)
  set(_visocyte_add_tests_default_test_data_target VisocyteData)
  ExternalData_Expand_Arguments(VisocyteData _
    "DATA{${CMAKE_CURRENT_SOURCE_DIR}/Data/Gmsh/,REGEX:viz_*}"
    "DATA{${CMAKE_CURRENT_SOURCE_DIR}/Data/Gmsh/naca0012/,REGEX:.*}")

  visocyte_add_client_tests(
    LOAD_PLUGIN "GenericIOReader"
    BASELINE_DIR ${VISOCYTE_TEST_BASELINE_DIR}
    TEST_DATA_TARGET VisocyteData
    TEST_SCRIPTS ${CMAKE_CURRENT_SOURCE_DIR}/GenericIOTest.xml)
endif ()
