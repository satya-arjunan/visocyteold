# On windows, execute_process runs PARAVIEW_EXECUTABLE in background.
# We prepend "cmd /c" to force visocyte's window to be shown to ensure proper
# mouse interactions with the GUI.
if(WIN32)
  set(PARAVIEW_EXECUTABLE cmd /c ${PARAVIEW_EXECUTABLE})
endif()

message(
  "${PARAVIEW_EXECUTABLE} -dr --data-directory=${PARAVIEW_TEST_OUTPUT_DIR} --test-script=${TEST_SCRIPT} --exit"
)

execute_process(
  COMMAND ${PARAVIEW_EXECUTABLE} -dr
          --data-directory=${PARAVIEW_TEST_OUTPUT_DIR}
          --test-script=${TEST_SCRIPT}
          --exit
  RESULT_VARIABLE rv
)
if (NOT rv EQUAL 0)
  message(FATAL_ERROR "Visocyte return value was ${rv}")
endif()

message(
  "${PVPYTHON_EXECUTABLE} -dr ${TEST_VERIFIER} -T ${PARAVIEW_TEST_OUTPUT_DIR} -N ${TEST_NAME}"
)

execute_process(
  COMMAND ${PVPYTHON_EXECUTABLE} -dr
  ${TEST_VERIFIER}
  -T ${PARAVIEW_TEST_OUTPUT_DIR}
  -N ${TEST_NAME}
  RESULT_VARIABLE rv
)
if (NOT rv EQUAL 0)
  message(FATAL_ERROR "pvpython return value was ${rv}")
endif()
