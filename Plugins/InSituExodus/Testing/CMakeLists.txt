if (VISOCYTE_BUILD_QT_GUI)
  set(test_xmls
    InSituExodusPluginTest.xml
  )

  visocyte_add_client_tests(
    BASELINE_DIR ${VISOCYTE_TEST_BASELINE_DIR}
    TEST_DATA_TARGET VisocyteData
    TEST_SCRIPTS ${test_xmls}
    LOAD_PLUGIN "InSituExodus"
  )

endif ()
