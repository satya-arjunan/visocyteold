if (VISOCYTE_BUILD_QT_GUI)
  set(test_xmls
    IndeXRepresentation.xml
  )
  set(IndeXRepresentation_DISABLE_CS TRUE)
  set(IndeXRepresentation_DISABLE_CRS TRUE)

  # The embedded free IndeX license doesn't support parallel processing.
  # Disable the test on MPI builds.
  if (NOT VISOCYTE_USE_MPI)
    list(APPEND test_xmls
      IndeXRepresentationServer.xml)
    set(IndeXRepresentationServer_DISABLE_C TRUE)
    # This won't work until image delivery for the render server is
    # implemented.
    set(IndeXRepresentationServer_DISABLE_CRS TRUE)
  endif ()

  visocyte_add_client_tests(
    BASELINE_DIR ${VISOCYTE_TEST_BASELINE_DIR}
    TEST_DATA_TARGET VisocyteData
    TEST_SCRIPTS ${test_xmls}
  )
endif ()
