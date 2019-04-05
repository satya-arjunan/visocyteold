# This cmake script tests python state file saving and loading. It
# first launches visocyte, runs an XML test, and saves the resulting
# state file. It then launches pvpython with a small test driver
# that loads the state file and then compares the resulting image to the
# baseline.

# On windows, execute_process runs VISOCYTE_EXECUTABLE in background.
# We prepend "cmd /c" to force visocyte's window to be shown to ensure proper
# mouse interactions with the GUI.
if(WIN32)
  set(VISOCYTE_EXECUTABLE cmd /c ${VISOCYTE_EXECUTABLE})
endif()

# set `DASHBOARD_TEST_FROM_CTEST` environment variable to ensure
# the executables realize we're running tests.
set(ENV{DASHBOARD_TEST_FROM_CTEST} "1")

# run visocyte to setup and save the python state file
execute_process(
  COMMAND ${VISOCYTE_EXECUTABLE} -dr
          --test-directory=${TEMPORARY_DIR}
          --test-script=${TEST_SCRIPT}
          --exit
  RESULT_VARIABLE rv)
if(NOT rv EQUAL 0)
  message(FATAL_ERROR "ParaView return value was ${rv}")
endif()

# run pvpython to load the state file and verify the result
execute_process(
  COMMAND ${PVPYTHON_EXECUTABLE} -dr
  ${TEST_DRIVER}
  ${TEMPORARY_DIR}/${PYTHON_STATE_TEST_NAME}-StateFile.py
  -T ${TEMPORARY_DIR}
  -V ${VISOCYTE_TEST_OUTPUT_BASELINE_DIR}/${PYTHON_STATE_TEST_NAME}.png
  RESULT_VARIABLE rv)
if(NOT rv EQUAL 0)
  message(FATAL_ERROR "PVPython return value was ${rv}")
endif()
