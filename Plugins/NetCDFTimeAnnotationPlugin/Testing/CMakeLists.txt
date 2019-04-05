if (VISOCYTE_BUILD_QT_GUI AND BUILD_SHARED_LIBS)
  ExternalData_Expand_Arguments("ParaViewData" _
    "DATA{Data/tos_O1_2001-2002.nc}")

  visocyte_add_client_tests(
    LOAD_PLUGIN "NetCDFTimeAnnotationPlugin"
    BASELINE_DIR ${VISOCYTE_TEST_BASELINE_DIR}
    TEST_DATA_TARGET ParaViewData
    TEST_SCRIPTS NetCDFTimeAnnotationFilter.xml)

  visocyte_add_client_server_tests(
    LOAD_PLUGIN "NetCDFTimeAnnotationPlugin"
    BASELINE_DIR ${VISOCYTE_TEST_BASELINE_DIR}
    TEST_DATA_TARGET ParaViewData
    TEST_SCRIPTS NetCDFTimeAnnotationFilter.xml)
endif ()
