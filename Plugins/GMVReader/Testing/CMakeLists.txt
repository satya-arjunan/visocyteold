set(suffix "")
if (VISOCYTE_ENABLE_VISITBRIDGE AND VISIT_BUILD_READER_GMV)
  string(APPEND suffix "_WithVisitBridge")
endif ()

set(GMV_READER_TESTS
  GMVReaderBasicRead${suffix}.xml
  GMVReaderInvalidFiles${suffix}.xml
)

if (VISOCYTE_BUILD_QT_GUI)
  visocyte_add_client_tests(
    BASELINE_DIR ${VISOCYTE_TEST_BASELINE_DIR}
    TEST_SCRIPTS ${GMV_READER_TESTS}
    TEST_DATA_TARGET VisocyteData
    LOAD_PLUGIN "GMVReader"
  )
  set_property(TEST "pv.GMVReaderInvalidFiles${suffix}"
    PROPERTY
      PASS_REGULAR_EXPRESSION "client0 process exited with code 0")

  visocyte_add_client_server_tests(
    BASELINE_DIR ${VISOCYTE_TEST_BASELINE_DIR}
    TEST_SCRIPTS ${GMV_READER_TESTS}
    TEST_DATA_TARGET VisocyteData
    LOAD_PLUGIN "GMVReader"
  )
  set_property(TEST "pvcs.GMVReaderInvalidFiles${suffix}"
    PROPERTY
      PASS_REGULAR_EXPRESSION "server process exited with code 0")
endif ()
