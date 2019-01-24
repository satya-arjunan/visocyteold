if (PARAVIEW_BUILD_QT_GUI AND BUILD_SHARED_LIBS)
  visocyte_add_client_tests(
    LOAD_PLUGIN "StreamLinesRepresentation"
    BASELINE_DIR ${PARAVIEW_TEST_BASELINE_DIR}
    TEST_DATA_TARGET VisocyteData
    TEST_SCRIPTS StreamLinesRepresentationColor.xml)
  visocyte_add_client_tests(
    LOAD_PLUGIN "StreamLinesRepresentation"
    BASELINE_DIR ${PARAVIEW_TEST_BASELINE_DIR}
    TEST_DATA_TARGET VisocyteData
    TEST_SCRIPTS StreamLinesRepresentationThick.xml)
  visocyte_add_client_tests(
    LOAD_PLUGIN "StreamLinesRepresentation"
    BASELINE_DIR ${PARAVIEW_TEST_BASELINE_DIR}
    TEST_DATA_TARGET VisocyteData
    TEST_SCRIPTS StreamLinesRepresentationTransform.xml)
endif ()
