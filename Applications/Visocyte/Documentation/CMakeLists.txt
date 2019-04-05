configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/index.html"
  "${CMAKE_CURRENT_BINARY_DIR}/index.html"
  COPYONLY)

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/about.html.in"
  "${CMAKE_CURRENT_BINARY_DIR}/about.html"
  @ONLY)

visocyte_client_documentation(
  TARGET  visocyte_doc
  XMLS    ${xmls}
          ${visocyte_server_manager_files})

set(table_of_contents
  "<toc>
  <section title=\"ParaView User Manual\" ref=\"index.html\">
    <section title=\"Sources\" ref=\"Sources.html\"/>
    <section title=\"Filters\" ref=\"Filters.html\"/>
    <section title=\"Readers\" ref=\"Readers.html\"/>
    <section title=\"Writers\" ref=\"Writers.html\"/>
  </section>
</toc>")

set(visocyte_documentation_qrc
  "${CMAKE_CURRENT_BINARY_DIR}/visocyte_documentation.qrc")
visocyte_client_generate_help(
  NAME        visocyte
  OUTPUT_PATH visocyte_qch
  TARGET      visocyte_qhp
  OUTPUT_DIR  "${CMAKE_CURRENT_BINARY_DIR}"
  SOURCE_DIR  "${CMAKE_CURRENT_SOURCE_DIR}"
  PATTERNS    "*.html"
              "*.css"
              "*.png"
              "*.jpg"
              "images/*.jpg"
  DEPENDS     visocyte_doc
  TABLE_OF_CONTENTS "${table_of_contents}"
  RESOURCE_FILE   "${visocyte_documentation_qrc}"
  RESOURCE_PREFIX "/ParaView/Documentation")

set(CMAKE_AUTORCC 1)

find_package(Qt5 REQUIRED QUIET COMPONENTS Core)

add_library(vtkParaViewDocumentation STATIC
  ParaViewDocumentationInitializer.cxx
  ParaViewDocumentationInitializer.h
  "${visocyte_documentation_qrc}")
target_include_directories(vtkParaViewDocumentation
  PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>")
target_link_libraries(vtkParaViewDocumentation
  PRIVATE
    Qt5::Core)

install(
  FILES       "${visocyte_qch}"
  DESTINATION "${CMAKE_INSTALL_DOCDIR}"
  COMPONENT   doc)
