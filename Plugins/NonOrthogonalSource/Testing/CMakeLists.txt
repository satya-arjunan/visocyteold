if (VISOCYTE_BUILD_QT_GUI AND BUILD_SHARED_LIBS)
  set (xmls
    NonOrthogonalSliceView.xml
    NonOrthographicOrthographicView.xml
  )

  set (NonOrthogonalSliceView_FORCE_SERIAL TRUE)

  visocyte_add_client_tests(
    LOAD_PLUGIN "NonOrthogonalSource"
    BASELINE_DIR ${VISOCYTE_TEST_BASELINE_DIR}
    TEST_DATA_TARGET ParaViewData
    TEST_SCRIPTS ${xmls}
  )
  visocyte_add_client_server_tests(
    LOAD_PLUGIN "NonOrthogonalSource"
    BASELINE_DIR ${VISOCYTE_TEST_BASELINE_DIR}
    TEST_DATA_TARGET ParaViewData
    TEST_SCRIPTS ${xmls}
  )
  visocyte_add_client_server_render_tests(
    LOAD_PLUGIN "NonOrthogonalSource"
    BASELINE_DIR ${VISOCYTE_TEST_BASELINE_DIR}
    TEST_DATA_TARGET ParaViewData
    TEST_SCRIPTS ${xmls}
  )
endif ()
