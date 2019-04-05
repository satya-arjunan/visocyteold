if (VISOCYTE_BUILD_QT_GUI AND BUILD_SHARED_LIBS)

  ExternalData_Expand_Arguments("VisocyteData" _
    "DATA{Data/explicitStructuredGrid.vtu}"
    "DATA{Data/Baseline/ExplicitStructuredGridReal.png}"
    "DATA{Data/Baseline/ExplicitStructuredGridSmooth.png}"
  )

  visocyte_add_client_tests(
    LOAD_PLUGIN "ExplicitStructuredGrid"
    BASELINE_DIR ${VISOCYTE_TEST_BASELINE_DIR}
    TEST_DATA_TARGET VisocyteData
    TEST_SCRIPTS ${CMAKE_CURRENT_SOURCE_DIR}/ExplicitStructuredGridSmooth.xml
  )
  visocyte_add_client_server_tests(
    LOAD_PLUGIN "ExplicitStructuredGrid"
    BASELINE_DIR ${VISOCYTE_TEST_BASELINE_DIR}
    TEST_DATA_TARGET VisocyteData
    TEST_SCRIPTS ${CMAKE_CURRENT_SOURCE_DIR}/ExplicitStructuredGridSmooth.xml
  )
  visocyte_add_client_server_render_tests(
    LOAD_PLUGIN "ExplicitStructuredGrid"
    BASELINE_DIR ${VISOCYTE_TEST_BASELINE_DIR}
    TEST_DATA_TARGET VisocyteData
    TEST_SCRIPTS ${CMAKE_CURRENT_SOURCE_DIR}/ExplicitStructuredGridSmooth.xml
  )

  visocyte_add_client_tests(
    LOAD_PLUGIN "ExplicitStructuredGrid"
    BASELINE_DIR ${VISOCYTE_TEST_BASELINE_DIR}
    TEST_DATA_TARGET VisocyteData
    TEST_SCRIPTS ${CMAKE_CURRENT_SOURCE_DIR}/ExplicitStructuredGridReal.xml
  )
  visocyte_add_client_server_tests(
    LOAD_PLUGIN "ExplicitStructuredGrid"
    BASELINE_DIR ${VISOCYTE_TEST_BASELINE_DIR}
    TEST_DATA_TARGET VisocyteData
    TEST_SCRIPTS ${CMAKE_CURRENT_SOURCE_DIR}/ExplicitStructuredGridReal.xml
  )
  visocyte_add_client_server_render_tests(
    LOAD_PLUGIN "ExplicitStructuredGrid"
    BASELINE_DIR ${VISOCYTE_TEST_BASELINE_DIR}
    TEST_DATA_TARGET VisocyteData
    TEST_SCRIPTS ${CMAKE_CURRENT_SOURCE_DIR}/ExplicitStructuredGridReal.xml
  )

endif()
