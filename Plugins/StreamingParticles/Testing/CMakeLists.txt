# TODO: Update

if (FALSE AND VISOCYTE_BUILD_QT_GUI AND VISOCYTE_ENABLE_COSMOTOOLS)
  ExternalData_Expand_Arguments("VisocyteData" _
    "DATA{${CMAKE_CURRENT_SOURCE_DIR}/Data/adaptive-cosmo/,REGEX:.*}"
    "DATA{${CMAKE_CURRENT_SOURCE_DIR}/Data/multiresolution-streaming/,REGEX:.*}")

  visocyte_add_client_tests(
    COMMAND --client ${CLIENT_EXECUTABLE}
            --enable-bt
            -dr
            --enable-streaming
            --test-directory=${VISOCYTE_TEST_DIR}
            --test-plugin=StreamingParticles
    BASELINE_DIR ${VISOCYTE_TEST_BASELINE_DIR}
    TEST_SCRIPTS StreamingParticles.xml
    )
  visocyte_test_load_data_dirs(NewGenericIOStreaming
    multiresolution-streaming
    )
  visocyte_add_client_tests(
    COMMAND --client ${CLIENT_EXECUTABLE}
            --enable-bt
            -dr
            --mpi
            --enable-streaming
            --test-directory=${VISOCYTE_TEST_DIR}
            --test-plugin=StreamingParticles
    BASELINE_DIR ${VISOCYTE_TEST_BASELINE_DIR}
    TEST_SCRIPTS NewGenericIOStreaming.xml
    )
  visocyte_add_client_server_tests(
    COMMAND --server-np 2
            --server $<TARGET_FILE:pvserver>
            --enable-bt --enable-streaming
            --test-plugin=StreamingParticles
            --client ${CLIENT_EXECUTABLE}
            --enable-bt
            -dr
            --mpi
            --enable-streaming
            --test-directory=${VISOCYTE_TEST_DIR}
            --test-plugin=StreamingParticles
    BASELINE_DIR ${VISOCYTE_TEST_BASELINE_DIR}
    TEST_SCRIPTS NewGenericIOStreaming.xml
    )
  visocyte_add_client_server_render_tests(
    COMMAND --server-np 2
            --server $<TARGET_FILE:pvserver>
            --enable-bt --enable-streaming
            --test-plugin=StreamingParticles
            --client ${CLIENT_EXECUTABLE}
            --enable-bt
            -dr
            --mpi
            --enable-streaming
            --test-directory=${VISOCYTE_TEST_DIR}
            --test-plugin=StreamingParticles
    BASELINE_DIR ${VISOCYTE_TEST_BASELINE_DIR}
    TEST_SCRIPTS NewGenericIOStreaming.xml
    )
endif ()
