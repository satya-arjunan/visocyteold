# Make sure we can start the DataProber server
add_test(NAME pvweb-StartTest
  COMMAND
    "$<TARGET_FILE:Visocyte::pvpython>"
    "${CMAKE_BINARY_DIR}/${VISOCYTE_PYTHON_SITE_PACKAGES_SUFFIX}/visocyte/web/test_server.py"
    --port 9739
    --timeout 1)
