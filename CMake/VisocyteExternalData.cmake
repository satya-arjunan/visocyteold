include(ExternalData)

if(NOT VISOCYTE_DATA_STORE)
  # Select a default from the following.
  set(VISOCYTE_DATA_STORE_DEFAULT "")
  if(EXISTS "${Visocyte_SOURCE_DIR}/.ExternalData/config/store")
    # Configuration left by developer setup script.
    file(STRINGS "${Visocyte_SOURCE_DIR}/.ExternalData/config/store"
      VISOCYTE_DATA_STORE_DEFAULT LIMIT_COUNT 1 LIMIT_INPUT 1024)
  elseif(IS_DIRECTORY "${CMAKE_SOURCE_DIR}/../VisocyteExternalData")
    # Adjacent directory created by user.
    get_filename_component(VISOCYTE_DATA_STORE_DEFAULT
      "${CMAKE_SOURCE_DIR}/../VisocyteExternalData" ABSOLUTE)
  elseif(IS_DIRECTORY "${CMAKE_SOURCE_DIR}/../ExternalData")
    # Generic adjacent directory created by user.
    get_filename_component(VISOCYTE_DATA_STORE_DEFAULT
      "${CMAKE_SOURCE_DIR}/../ExternalData" ABSOLUTE)
  elseif(DEFINED "ENV{VisocyteExternalData_OBJECT_STORES}")
    # The VisocyteExternalData environment variable.
    file(TO_CMAKE_PATH "$ENV{VisocyteExternalData_OBJECT_STORES}" VISOCYTE_DATA_STORE_DEFAULT)
  elseif(DEFINED "ENV{ExternalData_OBJECT_STORES}")
    # Generic ExternalData environment variable.
    file(TO_CMAKE_PATH "$ENV{ExternalData_OBJECT_STORES}" VISOCYTE_DATA_STORE_DEFAULT)
  endif()
endif()

# Provide users with an option to select a local object store,
# starting with the above-selected default.
set(VISOCYTE_DATA_STORE "${VISOCYTE_DATA_STORE_DEFAULT}" CACHE PATH
  "Local directory holding ExternalData objects in the layout %(algo)/%(hash).")
mark_as_advanced(VISOCYTE_DATA_STORE)

# Use a store in the build tree if none is otherwise configured.
if(NOT VISOCYTE_DATA_STORE)
  if(ExternalData_OBJECT_STORES)
    set(VISOCYTE_DATA_STORE "")
  else()
    set(VISOCYTE_DATA_STORE "${CMAKE_BINARY_DIR}/ExternalData/Objects")
    file(MAKE_DIRECTORY "${VISOCYTE_DATA_STORE}")
  endif()
endif()

# Tell ExternalData module about selected object stores.
list(APPEND ExternalData_OBJECT_STORES
  # Store selected by Visocyte-specific configuration above.
  ${VISOCYTE_DATA_STORE}

  # Local data store populated by the Visocyte pre-commit hook
  "${CMAKE_SOURCE_DIR}/.ExternalData"
  )

set(ExternalData_BINARY_ROOT ${CMAKE_BINARY_DIR}/ExternalData)

set(ExternalData_URL_TEMPLATES "" CACHE STRING
  "Additional URL templates for the ExternalData CMake script to look for testing data. E.g.
file:///var/bigharddrive/%(algo)/%(hash)")
mark_as_advanced(ExternalData_URL_TEMPLATES)
list(APPEND ExternalData_URL_TEMPLATES
  # Data published by Girder
  "https://data.kitware.com/api/v1/file/hashsum/%(algo)/%(hash)/download"

  # Data published by developers using git-gitlab-push
  "https://www.visocyte.org/files/ExternalData/%(algo)/%(hash)"
  )

# Tell ExternalData commands to transform raw files to content links.
# TODO: Condition this feature on presence of our pre-commit hook.
set(ExternalData_LINK_CONTENT SHA512)

# Match series of the form <base>.<ext>, <base>_<n>.<ext> such that <base> may
# end in a (test) number that is not part of any series numbering.
set(ExternalData_SERIES_PARSE "()(\\.[^./]*)$")
set(ExternalData_SERIES_MATCH "(_[0-9]+)?")

if(DEFINED ENV{DASHBOARD_TEST_FROM_CTEST})
  # Dashboard builds always need data.
  set(VISOCYTE_DATA_EXCLUDE_FROM_ALL OFF)
endif()

if(NOT DEFINED VISOCYTE_DATA_EXCLUDE_FROM_ALL)
  if(EXISTS "${Visocyte_SOURCE_DIR}/.ExternalData/config/exclude-from-all")
    # Configuration left by developer setup script.
    file(STRINGS "${Visocyte_SOURCE_DIR}/.ExternalData/config/exclude-from-all"
      VISOCYTE_DATA_EXCLUDE_FROM_ALL_DEFAULT LIMIT_COUNT 1 LIMIT_INPUT 1024)
  elseif(DEFINED "ENV{VISOCYTE_DATA_EXCLUDE_FROM_ALL}")
    set(VISOCYTE_DATA_EXCLUDE_FROM_ALL_DEFAULT
      "$ENV{VISOCYTE_DATA_EXCLUDE_FROM_ALL}")
  else()
    set(VISOCYTE_DATA_EXCLUDE_FROM_ALL_DEFAULT OFF)
  endif()
  set(VISOCYTE_DATA_EXCLUDE_FROM_ALL "${VISOCYTE_DATA_EXCLUDE_FROM_ALL_DEFAULT}"
    CACHE BOOL "Exclude test data download from default 'all' target."
    )
  mark_as_advanced(VISOCYTE_DATA_EXCLUDE_FROM_ALL)
endif()

# Tell VTK to act as we do but not to warn when we warn.
set(VTK_DATA_EXCLUDE_FROM_ALL ${VISOCYTE_DATA_EXCLUDE_FROM_ALL})
set(VTK_DATA_EXCLUDE_FROM_ALL_NO_WARNING 1)
