if (BUILD_SHARED_LIBS)
  ExternalData_Expand_Arguments("VisocyteData" _
    "DATA{${CMAKE_CURRENT_SOURCE_DIR}/Data/blow.vtk}")

  visocyte_add_client_tests(
    LOAD_PLUGIN "TemporalParallelismScriptGenerator"
    TEST_DATA_TARGET VisocyteData
    TEST_SCRIPTS TemporalParallelism.xml)
endif ()
