file(MAKE_DIRECTORY ${VISOCYTE_GENERATED_DOCUMENTATION_OUTPUT_DIRECTORY})

#---------------------------------------------------------------------------------
# add a txt file to indicate which source version the documentation was
# generated for.
set(CMAKE_CONFIGURABLE_FILE_CONTENT "${VISOCYTE_VERSION_FULL}")
configure_file("${CMAKE_ROOT}/Modules/CMakeConfigurableFile.in"
               "${VISOCYTE_GENERATED_DOCUMENTATION_OUTPUT_DIRECTORY}/version.txt")
unset(CMAKE_CONFIGURABLE_FILE_CONTENT)

#---------------------------------------------------------------------------------
# add a custom command to package the documentations into a tarball that's
# easy to move around.
add_custom_target(VisocyteDoc-TGZ
  COMMAND
    ${CMAKE_COMMAND} -E tar cfz
    ${CMAKE_CURRENT_BINARY_DIR}/VisocyteGeneratedDocs.tar.gz
    ${VISOCYTE_GENERATED_DOCUMENTATION_OUTPUT_DIRECTORY}
  WORKING_DIRECTORY ${VISOCYTE_GENERATED_DOCUMENTATION_OUTPUT_DIRECTORY}/..
  COMMENT "Generate VisocyteGeneratedDocs.tar.gz")

add_dependencies(VisocyteDoc-TGZ VisocyteDoxygenDoc)
if (VISOCYTE_ENABLE_PYTHON)
  add_dependencies(VisocyteDoc-TGZ VisocytePythonDoc)
endif ()
