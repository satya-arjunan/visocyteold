set(module_tests
  AnalyzeReaderWriterPlugin.xml
  NiftiReaderWriterPlugin.xml)

if (VISOCYTE_BUILD_QT_GUI AND BUILD_SHARED_LIBS)
  set(_visocyte_add_tests_default_test_data_target VisocyteData)
  ExternalData_Expand_Arguments(VisocyteData _
    "DATA{${CMAKE_CURRENT_SOURCE_DIR}/Data/ANALYZE/,REGEX:.*}"
    "DATA{${CMAKE_CURRENT_SOURCE_DIR}/Data/minimal.nii}")

  visocyte_add_client_tests(
    LOAD_PLUGIN   "AnalyzeNIfTIReaderWriter"
    BASELINE_DIR  ${VISOCYTE_TEST_BASELINE_DIR}
    TEST_DATA_TARGET VisocyteData
    TEST_SCRIPTS  ${module_tests})
endif ()
