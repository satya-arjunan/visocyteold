# On windows, execute_process runs VISOCYTE_EXECUTABLE in background.
# We prepend "cmd /c" to force visocyte's window to be shown to ensure proper
# mouse interactions with the GUI.
if(WIN32)
  set(VISOCYTE_EXECUTABLE cmd /c ${VISOCYTE_EXECUTABLE})
endif()

# set `DASHBOARD_TEST_FROM_CTEST` environment variable to ensure
# the executables realize we're running tests.
set(ENV{DASHBOARD_TEST_FROM_CTEST} "1")

message(
  "${VISOCYTE_EXECUTABLE} -dr --test-directory=${TEMPORARY_DIR} --data-directory=${DATA_DIR} --test-script=${TEST_SCRIPT} --exit"
)

execute_process(
  COMMAND ${VISOCYTE_EXECUTABLE} -dr
          --test-directory=${TEMPORARY_DIR}
          --data-directory=${DATA_DIR}
          --test-script=${TEST_SCRIPT}
          --exit
  RESULT_VARIABLE rv
)
if (NOT rv EQUAL 0)
  message(FATAL_ERROR "ParaView return value was ${rv}")
endif()

message(
  "${PVPYTHON_EXECUTABLE} -dr ${TEST_VERIFIER} -T ${TEMPORARY_DIR} -N ${TEST_NAME}"
)

execute_process(
  COMMAND ${PVPYTHON_EXECUTABLE} -dr
  ${TEST_VERIFIER}
  -T ${TEMPORARY_DIR}
  -N ${TEST_NAME}
  RESULT_VARIABLE rv
)
if (NOT rv EQUAL 0)
  message(FATAL_ERROR "pvpython return value was ${rv}")
endif()
