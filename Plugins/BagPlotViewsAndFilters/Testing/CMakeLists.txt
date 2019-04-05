if (VISOCYTE_BUILD_QT_GUI AND BUILD_SHARED_LIBS)

  ExternalData_Expand_Arguments("VisocyteData" _
    "DATA{Data/Baseline/BagPlots.png}"
    "DATA{Data/Baseline/BagPlotsSelection.png}"
    "DATA{Data/Baseline/BagPlotMatrixView.png}"
    "DATA{Data/Baseline/BagPlotMatrixView_A.png}")

  visocyte_add_client_tests(
    LOAD_PLUGIN "BagPlotViewsAndFilters"
    BASELINE_DIR ${VISOCYTE_TEST_BASELINE_DIR}
    TEST_DATA_TARGET VisocyteData
    TEST_SCRIPTS ${CMAKE_CURRENT_SOURCE_DIR}/BagPlots.xml)
  visocyte_add_client_tests(
    LOAD_PLUGIN "BagPlotViewsAndFilters"
    BASELINE_DIR ${VISOCYTE_TEST_BASELINE_DIR}
    TEST_DATA_TARGET VisocyteData
    TEST_SCRIPTS ${CMAKE_CURRENT_SOURCE_DIR}/BagPlotsSelection.xml)
  visocyte_add_client_tests(
    LOAD_PLUGIN "BagPlotViewsAndFilters"
    BASELINE_DIR ${VISOCYTE_TEST_BASELINE_DIR}
    TEST_DATA_TARGET VisocyteData
    TEST_SCRIPTS ${CMAKE_CURRENT_SOURCE_DIR}/FunctionalBagPlots.xml)
  visocyte_add_client_tests(
    LOAD_PLUGIN "BagPlotViewsAndFilters"
    BASELINE_DIR ${VISOCYTE_TEST_BASELINE_DIR}
    TEST_DATA_TARGET VisocyteData
    TEST_SCRIPTS ${CMAKE_CURRENT_SOURCE_DIR}/BagPlotMatrixView.xml)
endif()
