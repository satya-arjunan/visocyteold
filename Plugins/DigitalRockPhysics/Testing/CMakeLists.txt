if (VISOCYTE_BUILD_QT_GUI)
  set(_visocyte_add_tests_default_test_data_target DigitalRockPhysicsData)
  ExternalData_Expand_Arguments("${_visocyte_add_tests_default_test_data_target}" _
    "DATA{Data/rocksample.vti}")

  visocyte_add_client_tests(
    LOAD_PLUGIN "DigitalRockPhysics"
    BASELINE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Data/Baseline"
    TEST_SCRIPTS DigitalRockPhysicsAnalysisFilter.xml)
  visocyte_add_client_tests(
    LOAD_PLUGIN "DigitalRockPhysics"
    BASELINE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Data/Baseline"
    TEST_SCRIPTS DigitalRockPhysicsExplodeFilter.xml)

  if (VISOCYTE_USE_MPI)
    visocyte_add_client_server_tests(
      LOAD_PLUGIN "DigitalRockPhysics"
      BASELINE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Data/Baseline"
      TEST_SCRIPTS DigitalRockPhysicsAnalysisFilter.xml)
    visocyte_add_client_server_tests(
      LOAD_PLUGIN "DigitalRockPhysics"
      BASELINE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Data/Baseline"
      TEST_SCRIPTS DigitalRockPhysicsExplodeFilter.xml)
   endif ()

   ExternalData_add_target("${_visocyte_add_tests_default_test_data_target}")
endif ()
