if (VISOCYTE_BUILD_QT_GUI)
  ExternalData_Expand_Arguments("VisocyteData" _
    "DATA{Data/,REGEX:init_solution_propagation_out\\.e.*}")

  visocyte_add_client_tests(
    LOAD_PLUGIN "MooseXfemClip"
    BASELINE_DIR ${VISOCYTE_TEST_BASELINE_DIR}
    TEST_DATA_TARGET VisocyteData
    TEST_SCRIPTS TestMooseXfemClip.xml
  )
endif ()
