if (PARAVIEW_BUILD_QT_GUI AND BUILD_SHARED_LIBS)
  visocyte_add_client_tests(
    LOAD_PLUGIN "PanoramicProjectionView"
    BASELINE_DIR ${PARAVIEW_TEST_BASELINE_DIR}
    TEST_DATA_TARGET VisocyteData
    TEST_SCRIPTS PanoramicProjectionEquirectangular.xml PanoramicProjectionAzimuthal.xml)
  visocyte_add_client_server_tests(
    LOAD_PLUGIN "PanoramicProjectionView"
    BASELINE_DIR ${PARAVIEW_TEST_BASELINE_DIR}
    TEST_DATA_TARGET VisocyteData
    TEST_SCRIPTS PanoramicProjectionEquirectangular.xml PanoramicProjectionAzimuthal.xml)
endif ()
