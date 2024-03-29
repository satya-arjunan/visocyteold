#==========================================================================
#
#     Program: Visocyte
#
#     Copyright (c) 2005-2008 Sandia Corporation, Kitware Inc.
#     All rights reserved.
#
#     Visocyte is a free software; you can redistribute it and/or modify it
#     under the terms of the Visocyte license version 1.2.
#
#     See License_v1.2.txt for the full Visocyte license.
#     A copy of this license can be obtained by contacting
#     Kitware Inc.
#     28 Corporate Drive
#     Clifton Park, NY 12065
#     USA
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#  ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
#  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
#  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR
#  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
#  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
#  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
#  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
#  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
#  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
#==========================================================================
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(Visocyte)

foreach (policy IN ITEMS
    CMP0083 # CMake 3.14
    )
  if (POLICY ${policy})
    cmake_policy(SET ${policy} NEW)
  endif ()
endforeach ()

# Disallow in-source build
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
  message(FATAL_ERROR
    "Visocyte requires an out-of-source build. Please create a separate "
    "binary directory and run CMake there.")
endif ()

#------------------------------------------------------------------------------
# Set a default build type if none was specified
get_property(generator_is_multi_config GLOBAL
  PROPERTY GENERATOR_IS_MULTI_CONFIG)
if (NOT CMAKE_BUILD_TYPE AND NOT generator_is_multi_config)
  message(STATUS "Setting build type to 'Debug' as none was specified.")
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE
    PROPERTY
      STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif ()

#------------------------------------------------------------------------------
# Choose static or shared libraries.
option(VISOCYTE_BUILD_SHARED_LIBS "Build Visocyte with shared libraries" ON)
set(BUILD_SHARED_LIBS "${VISOCYTE_BUILD_SHARED_LIBS}")
set(CMAKE_CXX_VISIBILITY_PRESET "hidden")
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

#------------------------------------------------------------------------------
# Protect against a stampede of static links at the same time.
if (NOT BUILD_SHARED_LIBS)
  set(pv_exe_job_link_pool static_exe_link)
  set_property(GLOBAL APPEND PROPERTY
    JOB_POOLS static_exe_link=1)
endif ()

# Set up our directory structure for output libraries and binaries
include(GNUInstallDirs)
if (NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}")
endif ()
if (NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")
endif ()
if (NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")
endif ()

set(visocyte_cmake_dir "${CMAKE_CURRENT_SOURCE_DIR}/CMake")
set(vtk_cmake_dir "${CMAKE_CURRENT_SOURCE_DIR}/VTK/CMake")
list(APPEND CMAKE_MODULE_PATH
  "${visocyte_cmake_dir}"
  "${vtk_cmake_dir}")
include(VisocyteServerManager)
include(VisocyteTesting)
include(VisocyteClient)
include(VisocytePlugin)
include(vtkModuleWrapClientServer)
include(CTest)
set_property(CACHE BUILD_TESTING
  PROPERTY
    TYPE INTERNAL)

include(vtkCompilerChecks)
include(vtkCompilerPlatformFlags)
include(vtkCompilerExtraFlags)
include(vtkInitializeBuildType)
include(vtkSupportMacros)
include(vtkDirectories)

#-----------------------------------------------------------------------------
# Does VTK require support for 64 bit file systems
include(CheckCXXSourceRuns)
file(READ "${vtk_cmake_dir}/vtkRequireLargeFilesSupport.cxx"
  VTK_REQUIRE_LARGE_FILE_SUPPORT_FILE)
check_cxx_source_runs("${VTK_REQUIRE_LARGE_FILE_SUPPORT_FILE}"
  CMAKE_REQUIRE_LARGE_FILE_SUPPORT "Support for 64 bit file systems")
set(VTK_REQUIRE_LARGE_FILE_SUPPORT ${CMAKE_REQUIRE_LARGE_FILE_SUPPORT})

#-----------------------------------------------------------------------------
# Provide compatibility options.
option(VTK_LEGACY_REMOVE "Remove all legacy code completely." OFF)
option(VTK_LEGACY_SILENT "Silence all legacy code messages." OFF)
mark_as_advanced(VTK_LEGACY_REMOVE VTK_LEGACY_SILENT)

# Setup external data. this will automatically download the test data and
# baseline files to the build tree and set VISOCYTE_DATA_ROOT.
include(VisocyteExternalData)

find_package(Git QUIET)
include(VisocyteDetermineVersion)

#------------------------------------------------------------------------------
# Setup Visocyte Environment
#------------------------------------------------------------------------------
# Determine Visocyte Source Version
# Hardcoded version variables are readin from a separate file. This makes it
# easier to have a script to update version numbers automatically.
file(STRINGS version.txt version_txt)
extract_version_components("${version_txt}" "VISOCYTE")
determine_version("${CMAKE_CURRENT_SOURCE_DIR}" "${GIT_EXECUTABLE}" "VISOCYTE")

# Setting this ensures that "make install" will leave rpaths to external
# libraries (not part of the build-tree e.g. Qt, ffmpeg, etc.) intact on
# "make install". This ensures that one can install a version of Visocyte on the
# build machine without any issues. If this not desired, simply specify
# CMAKE_INSTALL_RPATH_USE_LINK_PATH when configuring Visocyte and
# "make install" will strip all rpaths, which is default behavior.
if (NOT DEFINED CMAKE_INSTALL_RPATH_USE_LINK_PATH)
  set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
endif ()

include(CMakeDependentOption)
set(examples_default OFF)
if (DEFINED VISOCYTE_BUILD_EXAMPLES)
  message(DEPRECATION
    "VISOCYTE_BUILD_EXAMPLES has been renamed to VISOCYTE_ENABLE_EXAMPLES.")
  # Use the old variable as the default for the new variable if present.
  set(examples_default "${VISOCYTE_BUILD_EXAMPLES}")
  # Remove the old setting from the cache and local scope.
  unset(VISOCYTE_BUILD_EXAMPLES)
  unset(VISOCYTE_BUILD_EXAMPLES CACHE)
endif ()
option(VISOCYTE_ENABLE_EXAMPLES "Enable Visocyte examples" "${examples_default}")
option(VISOCYTE_BUILD_DEVELOPER_DOCUMENTATION "Build Visocyte C++/Python docs" OFF)
option(VISOCYTE_BUILD_QT_GUI "Enable Visocyte Qt-based client" ON)
option(VISOCYTE_USE_MPI "Enable MPI support for parallel computing" OFF)
option(VISOCYTE_USE_VTKM "Enable VTK-m accelerated algorithms" ON)
option(VISOCYTE_USE_OSPRAY "Build Visocyte with OSPRay Ray Traced rendering" OFF)
option(VISOCYTE_ENABLE_LOGGING "Enable logging support." ON)
mark_as_advanced(VISOCYTE_ENABLE_LOGGING)

set(visocyte_web_default ON)
if (VISOCYTE_ENABLE_PYTHON AND WIN32)
  include("${CMAKE_CURRENT_SOURCE_DIR}/VTK/CMake/FindPythonModules.cmake")
  find_python_module(win32api have_pywin32)
  set(visocyte_web_default "${have_pywin32}")
endif ()
# Add ability to freeze Python modules.
cmake_dependent_option(VISOCYTE_FREEZE_PYTHON
  "Freeze Python packages/modules into the application." OFF
  # XXX(python): Freezing is not yet supported.
  "VISOCYTE_ENABLE_PYTHON;NOT WIN32;NOT APPLE;FALSE" OFF)
mark_as_advanced(VISOCYTE_FREEZE_PYTHON)
cmake_dependent_option(VISOCYTE_ENABLE_WEB "Enable/Disable web support" "${visocyte_web_default}"
  "VISOCYTE_ENABLE_PYTHON" OFF)
mark_as_advanced(VISOCYTE_ENABLE_WEB)

# NvPipe requires an NVIDIA GPU.
option(VISOCYTE_ENABLE_NVPIPE "Build Visocyte with NvPipe remoting. Requires CUDA and an NVIDIA GPU" OFF)

option(VISOCYTE_ENABLE_GDAL "Enable GDAL support." OFF)
mark_as_advanced(VISOCYTE_ENABLE_GDAL)
option(VISOCYTE_ENABLE_LAS "Enable LAS support." OFF)
mark_as_advanced(VISOCYTE_ENABLE_LAS)
option(VISOCYTE_ENABLE_OPENTURNS "Enable OpenTurns support." OFF)
option(VISOCYTE_ENABLE_PDAL "Enable PDAL support." OFF)
mark_as_advanced(VISOCYTE_ENABLE_PDAL)
option(VISOCYTE_ENABLE_MOTIONFX "Enable MotionFX support." OFF)
mark_as_advanced(VISOCYTE_ENABLE_MOTIONFX)
option(VISOCYTE_ENABLE_VISITBRIDGE "Enable VisIt readers." OFF)
option(VISOCYTE_ENABLE_XDMF2 "Enable Xdmf2 support." ON)
mark_as_advanced(VISOCYTE_ENABLE_XDMF2)
option(VISOCYTE_ENABLE_XDMF3 "Enable Xdmf3 support." OFF)

set(VISOCYTE_BUILD_TESTING "OFF"
  CACHE STRING "Build module testing directories")
set_property(CACHE VISOCYTE_BUILD_TESTING
  PROPERTY
    STRINGS "ON;OFF;WANT")
set(BUILD_TESTING OFF)
if (VISOCYTE_BUILD_TESTING)
  set(BUILD_TESTING ON)
endif ()

if (UNIX)
  option(VISOCYTE_ENABLE_FFMPEG "Enable FFMPEG Support." OFF)
endif ()

cmake_dependent_option(VISOCYTE_USE_MPI_SSEND
  "Use MPI synchronous-send commands for communication" OFF
  "VISOCYTE_USE_MPI" OFF)
cmake_dependent_option(VISOCYTE_USE_ICE_T
  "Enable IceT (needed for parallel rendering)" ON
  "VISOCYTE_USE_MPI" OFF)
cmake_dependent_option(VISOCYTE_INITIALIZE_MPI_ON_CLIENT
  "Initialize MPI on client-processes by default. Can be overridden using command line arguments" ON
  "VISOCYTE_USE_MPI" OFF)
mark_as_advanced(
  VISOCYTE_USE_ICE_T
  VISOCYTE_USE_MPI_SSEND
  VISOCYTE_INITIALIZE_MPI_ON_CLIENT)

cmake_dependent_option(VISOCYTE_ENABLE_QT_SUPPORT
  "Build Visocyte with Qt support (without GUI)" OFF
  "NOT VISOCYTE_BUILD_QT_GUI" ON)

# Add an option to enable using Qt Webkit for widgets, as needed.
# Default is OFF. We don't want to depend on WebKit unless absolutely needed.
# FIXME: Move this to the module which cares.
cmake_dependent_option(VISOCYTE_USE_QTWEBENGINE
  "Use Qt WebKit components as needed." OFF
  "VISOCYTE_ENABLE_QT_SUPPORT" OFF)
mark_as_advanced(VISOCYTE_USE_QTWEBENGINE)

# Add an option to enable using Qt Help, as needed.
# Default is ON to enable integrated help/documentation.
cmake_dependent_option(VISOCYTE_USE_QTHELP
  "Use Qt Help infrastructure as needed." ON
  "VISOCYTE_ENABLE_QT_SUPPORT" OFF)
mark_as_advanced(VISOCYTE_USE_QTHELP)

# If VISOCYTE_BUILD_QT_GUI is OFF, provide an option to the user to turn other
# command line executables ON/OFF.
cmake_dependent_option(VISOCYTE_ENABLE_COMMANDLINE_TOOLS
  "Build Visocyte command-line tools" ON
  "NOT VISOCYTE_BUILD_QT_GUI" ON)

option(VISOCYTE_ENABLE_PYTHON "Enable/Disable Python scripting support" OFF)

# If building on Unix with MPI enabled, we will present another option to
# enable building of CosmoTools VTK extensions. This option is by default
# OFF and set to OFF if Visocyte is not built with MPI.
cmake_dependent_option(VISOCYTE_ENABLE_COSMOTOOLS
  "Build Visocyte with CosmoTools VTK Extensions" OFF
  "UNIX;VISOCYTE_USE_MPI" OFF)
mark_as_advanced(VISOCYTE_ENABLE_COSMOTOOLS)

option(VISOCYTE_ENABLE_CATALYST "Enable Catalyst CoProcessing modules" ON)
cmake_dependent_option(VISOCYTE_BUILD_CATALYST_ADAPTORS
  "Build Adaptors for various simulation codes" OFF
  # XXX: Adaptors need some work yet.
  "FALSE;VISOCYTE_ENABLE_CATALYST;NOT WIN32" OFF)
mark_as_advanced(VISOCYTE_BUILD_CATALYST_ADAPTORS)

# XXX(VTK): External VTK is not yet actually supported.
if (FALSE)
option(VISOCYTE_USE_EXTERNAL_VTK "Use an external VTK." OFF)
mark_as_advanced(VISOCYTE_USE_EXTERNAL_VTK)
else ()
set(VISOCYTE_USE_EXTERNAL_VTK OFF)
endif ()

# Kits bundle multiple modules together into a single library, this
# is used to dramatically reduce the number of generated libraries.
option(VISOCYTE_ENABLE_KITS "Build Visocyte using kits instead of modules." OFF)
mark_as_advanced(VISOCYTE_ENABLE_KITS)

option(VISOCYTE_USE_EXTERNAL "Use external copies of third party libraries by default" OFF)
mark_as_advanced(VISOCYTE_USE_EXTERNAL)

set(visocyte_requested_modules)
set(visocyte_rejected_modules)

if (VISOCYTE_ENABLE_COMMANDLINE_TOOLS)
  list(APPEND visocyte_requested_modules
    Visocyte::ServerManagerApplication)
endif ()

if (VISOCYTE_ENABLE_COMMANDLINE_TOOLS AND VISOCYTE_ENABLE_PYTHON)
  list(APPEND visocyte_requested_modules
    VTK::PythonInterpreter)
endif ()

if (VISOCYTE_ENABLE_PYTHON)
  set(VISOCYTE_PYTHON_VERSION "3"
    CACHE STRING "")
  set_property(CACHE VISOCYTE_PYTHON_VERSION
    PROPERTY
      STRINGS "2;3")
  if (NOT VISOCYTE_PYTHON_VERSION STREQUAL "2" AND
      NOT VISOCYTE_PYTHON_VERSION STREQUAL "3")
    message(FATAL_ERROR
      "`VISOCYTE_PYTHON_VERSION` must either be 2 or 3.")
  endif ()
  set(VTK_PYTHON_VERSION "${VISOCYTE_PYTHON_VERSION}")

  list(APPEND visocyte_requested_modules
    Visocyte::PythonAlgorithm
    Visocyte::PythonInitializer
    VTK::RenderingMatplotlib)
else ()
  list(APPEND visocyte_rejected_modules
    Visocyte::PythonAlgorithm
    Visocyte::PythonInitializer
    VTK::RenderingMatplotlib)
endif ()

if (VISOCYTE_ENABLE_PYTHON AND (NOT VISOCYTE_PYTHON_VERSION STREQUAL "3"))
  # cinema_python doesn't support Python 3 yet
  # visocyte/visocyte#17353
  list(APPEND visocyte_requested_modules
    Visocyte::CinemaPython
    Visocyte::CinemaReader)
else ()
  list(APPEND visocyte_rejected_modules
    Visocyte::CinemaPython
    Visocyte::CinemaReader)
endif()

if (VISOCYTE_ENABLE_PYTHON AND VISOCYTE_BUILD_TESTING)
  # testing needs Python interpreter i.e. `PYTHON_EXECUTABLE` without which
  # our `find_python_module` checks to determine which tests to add or exclude
  # will fail. We find it here.
  find_package(PythonInterp "${VISOCYTE_PYTHON_VERSION}" QUIET)
  mark_as_advanced(PYTHON_EXECUTABLE)
endif()

if (VISOCYTE_BUILD_QT_GUI)
  list(APPEND visocyte_requested_modules
    Visocyte::Core
    Visocyte::ServerManagerApplication
    Visocyte::ServerManagerCore
    Visocyte::ServerManagerDefault
    Visocyte::pqApplicationComponents
    Visocyte::pqComponents
    Visocyte::pqCore)

  if (VISOCYTE_ENABLE_PYTHON)
    list(APPEND visocyte_requested_modules
      Visocyte::pqPython)
  endif ()
else ()
  list(APPEND visocyte_rejected_modules
    Visocyte::pqApplicationComponents
    Visocyte::pqComponents
    Visocyte::pqCore
    Visocyte::pqWidgets
    Visocyte::qttesting
    VTK::GUISupportQt)
endif ()

if (VISOCYTE_USE_MPI)
  list(APPEND visocyte_requested_modules
    VTK::FiltersParallelFlowPaths
    VTK::FiltersParallelGeometry
    VTK::FiltersParallelMPI
    # Required because of Visocyte::VTKExtensionsDefault having an optional dependency.
    VTK::IOMPIImage
    VTK::IOParallelLSDyna
    VTK::IOParallelNetCDF
    VTK::ParallelMPI
    Visocyte::VTKExtensionsDefaultParallel
    Visocyte::icet)
else ()
  list(APPEND visocyte_rejected_modules
    VTK::mpi)
endif ()

if (VISOCYTE_USE_MPI AND VISOCYTE_ENABLE_PYTHON)
  list(APPEND visocyte_requested_modules
    VTK::ParallelMPI4Py)
else ()
  list(APPEND visocyte_rejected_modules
    VTK::ParallelMPI4Py)
endif()

if (VISOCYTE_USE_VTKM)
  list(APPEND visocyte_requested_modules
    VTK::AcceleratorsVTKm)
endif ()

if (VISOCYTE_USE_OSPRAY)
  list(APPEND visocyte_requested_modules
    VTK::RenderingOSPRay)
endif ()

if (VISOCYTE_ENABLE_NVPIPE)
  list(APPEND visocyte_requested_modules
    Visocyte::nvpipe)
else ()
  list(APPEND visocyte_rejected_modules
    Visocyte::nvpipe)
endif ()

if (VISOCYTE_ENABLE_GDAL)
  list(APPEND visocyte_requested_modules
    VTK::IOGDAL)
endif ()

if (VISOCYTE_ENABLE_LAS)
  list(APPEND visocyte_requested_modules
    VTK::IOLAS)
endif ()

if (VISOCYTE_ENABLE_OPENTURNS)
  list(APPEND visocyte_requested_modules
    VTK::FiltersOpenTurns)
endif ()

if (VISOCYTE_ENABLE_PDAL)
  list(APPEND visocyte_requested_modules
    VTK::IOPDAL)
endif ()

if (VISOCYTE_ENABLE_MOTIONFX)
  list(APPEND visocyte_requested_modules
    VTK::IOMotionFX)
endif ()

if (VISOCYTE_ENABLE_VISITBRIDGE)
  list(APPEND visocyte_requested_modules
    Visocyte::IOVisItBridge
    Visocyte::VisItLib)
else ()
  list(APPEND visocyte_rejected_modules
    Visocyte::IOVisItBridge
    Visocyte::VisItLib)
endif ()

if (VISOCYTE_ENABLE_XDMF2)
  list(APPEND visocyte_requested_modules
    VTK::IOXdmf2)
endif ()

if (VISOCYTE_ENABLE_XDMF3)
  list(APPEND visocyte_requested_modules
    VTK::IOXdmf3)
endif ()

if (VISOCYTE_ENABLE_FFMPEG)
  list(APPEND visocyte_requested_modules
    VTK::IOFFMPEG)
endif ()

if (VISOCYTE_USE_ICE_T)
  list(APPEND visocyte_requested_modules
    Visocyte::icet)
endif ()

if (VISOCYTE_ENABLE_COSMOTOOLS)
  list(APPEND visocyte_requested_modules
    Visocyte::VTKExtensionsCosmoTools)
else ()
  list(APPEND visocyte_rejected_modules
    Visocyte::VTKExtensionsCosmoTools)
endif ()

if (VISOCYTE_ENABLE_CATALYST)
  list(APPEND visocyte_requested_modules
    Visocyte::Catalyst)

  if (VISOCYTE_ENABLE_PYTHON)
    list(APPEND visocyte_requested_modules
      Visocyte::PythonCatalyst)
  endif ()

  if (BUILD_TESTING)
    list(APPEND visocyte_requested_modules
      Visocyte::CatalystTestDriver)
  endif ()

  # When Catalyst is enabled, Fortran is optionally needed. Hence we enable
  # Fortran at the top level itself. When individual module called
  # enable_language(...), it failed during first cmake configure but worked o
  # subsequent. enable_language(... OPTIONAL) overcomes that issue altogether.

  if (NOT WIN32)
    # Theoretically, CheckFortran should not be needed, but it
    # enable_language(OPTIONAL) fails with Ninja generator.
    include(CheckFortran)
    check_fortran_support()
    if (CMAKE_Fortran_COMPILER)
      enable_language(Fortran OPTIONAL)
    endif ()
  endif ()
endif ()

if (VISOCYTE_BUILD_TESTING)
  list(APPEND visocyte_requested_modules
    Visocyte::smTestDriver)
endif ()

list(APPEND visocyte_requested_modules
  Visocyte::ProcessXML
  Visocyte::VTKExtensionsCGNSReader
  Visocyte::VTKExtensionsH5PartReader
  Visocyte::VTKExtensionsPoints
  Visocyte::WrapClientServer
  VTK::FiltersFlowPaths
  VTK::FiltersVerdict
  VTK::IOAMR
  VTK::IOAsynchronous # needed for cinema, but no harm adding always.
  VTK::IOParallelLSDyna
  VTK::IOTecplotTable
  VTK::IOTRUCHAS
  VTK::IOVeraOut
  VTK::IOVPIC
  )

if (VISOCYTE_ENABLE_WEB)
  list(APPEND visocyte_requested_modules
    Visocyte::PVWebCore
    Visocyte::PVWebPython
    VTK::WebCore
    VTK::WebPython
  )
  if (VISOCYTE_PYTHON_VERSION STREQUAL "2")
    list(APPEND visocyte_requested_modules
      Visocyte::PVWebPython2
    )
  else ()
    list(APPEND visocyte_rejected_modules
      Visocyte::PVWebPython2
    )
  endif ()
else()
  list(APPEND visocyte_rejected_modules
    Visocyte::PVWebCore
    Visocyte::PVWebPython
    Visocyte::PVWebPython2
    VTK::WebCore
    VTK::WebPython
  )
endif ()

if (visocyte_requested_modules)
  list(REMOVE_DUPLICATES visocyte_requested_modules)
endif ()

option(VISOCYTE_BUILD_ALL_MODULES "Build all modules by default" OFF)
mark_as_advanced(VISOCYTE_BUILD_ALL_MODULES)

if (VISOCYTE_USE_EXTERNAL_VTK)
  find_package(VTK REQUIRED)
else ()
  list(APPEND CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/VTK/CMake")
  include(vtkCMakeBackports)
  include(vtkModule)
  include(vtkEncodeString)
  include(vtkObjectFactory)
  include(vtkExternalData)
  include(vtkVersion)
  set(VTK_VERSION
      "${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.${VTK_BUILD_VERSION}")

  if (VISOCYTE_ENABLE_PYTHON)
    include(vtkModuleWrapPython)
  endif ()
endif ()

set_property(GLOBAL
  PROPERTY
    "_vtk_module_autoinit_include" "\"vtkAutoInit.h\"")

set(visocyte_default_plugins
  AcceleratedAlgorithms
  AnalyzeNIfTIReaderWriter
  ArrowGlyph
  BagPlotViewsAndFilters
  CDIReader
  DigitalRockPhysics
  EULATestPlugin
  EmbossingRepresentations
  EyeDomeLighting
  ExplicitStructuredGrid
  GenericIOReader
  GeodesicMeasurement
  GMVReader
  LagrangianParticleTracker
  Moments
  MooseXfemClip
  NetCDFTimeAnnotationPlugin
  NonOrthogonalSource
  PacMan
  PanoramicProjectionView
  SLACTools
  SierraPlotTools
  StreamLinesRepresentation
  StreamingParticles
  SurfaceLIC
  ThickenLayeredCells
  VTKmFilters
  pvblot)
foreach (visocyte_default_plugin IN LISTS visocyte_default_plugins)
  set("_visocyte_plugin_default_${visocyte_default_plugin}" ON)
endforeach ()

set(visocyte_plugin_directories
  "${CMAKE_CURRENT_SOURCE_DIR}/Plugins")
visocyte_plugin_find_plugins(visocyte_plugin_files ${visocyte_plugin_directories})
visocyte_plugin_scan(
  PLUGIN_FILES      ${visocyte_plugin_files}
  PROVIDES_PLUGINS  visocyte_plugins
  REQUIRES_MODULES  visocyte_plugin_required_modules)

set(_vtk_module_group_default_VisocyteCore "WANT")
set(_vtk_module_group_default_VisocyteRendering "WANT")

set(visocyte_source_directories
  "${CMAKE_CURRENT_SOURCE_DIR}/CoProcessing"
  "${CMAKE_CURRENT_SOURCE_DIR}/VisocyteCore"
  "${CMAKE_CURRENT_SOURCE_DIR}/Qt"
  "${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty"
  "${CMAKE_CURRENT_SOURCE_DIR}/Utilities"
  "${CMAKE_CURRENT_SOURCE_DIR}/Web")
vtk_module_find_modules(visocyte_module_files ${visocyte_source_directories})
vtk_module_find_kits(visocyte_kit_files ${visocyte_source_directories})
vtk_module_scan(
  MODULE_FILES        ${visocyte_module_files}
  KIT_FILES           ${visocyte_kit_files}
  REQUEST_MODULES     ${visocyte_requested_modules}
                      ${visocyte_plugin_required_modules}
  REJECT_MODULES      ${visocyte_rejected_modules}
  PROVIDES_MODULES    visocyte_modules
  PROVIDES_KITS       visocyte_kits
  REQUIRES_MODULES    visocyte_required_modules
  UNRECOGNIZED_MODULES visocyte_unrecognized_modules
  WANT_BY_DEFAULT     "${VISOCYTE_BUILD_ALL_MODULES}"
  ENABLE_TESTS        "${VISOCYTE_BUILD_TESTING}")

set(visocyte_unrecognized_visocyte_modules)
foreach (visocyte_module IN LISTS visocyte_unrecognized_modules)
  if (visocyte_module MATCHES "Visocyte::")
    list(APPEND visocyte_unrecognized_visocyte_modules
      "${visocyte_module}")
  endif ()
endforeach ()
if (visocyte_unrecognized_visocyte_modules)
  message(FATAL_ERROR
    "The following modules were requested or required, but not found: "
    "${visocyte_unrecognized_visocyte_modules}.")
endif ()

set(visocyte_cmake_destination
  "${CMAKE_INSTALL_LIBDIR}/cmake/visocyte-${VISOCYTE_VERSION_MAJOR}.${VISOCYTE_VERSION_MINOR}")

if (VISOCYTE_USE_EXTERNAL_VTK)
  if (visocyte_required_modules OR visocyte_unrecognized_modules)
    message(FATAL_ERROR
      "The following modules were requested or required, but not found: "
      "${visocyte_required_modules};${visocyte_unrecognized_modules}.")
  endif ()

  if (VISOCYTE_ENABLE_PYTHON)
    find_package("Python${VISOCYTE_PYTHON_VERSION}" QUIET REQUIRED COMPONENTS Interpreter)
    set(PYTHON_VERSION_MAJOR "${Python${VISOCYTE_PYTHON_VERSION}_VERSION_MAJOR}")
    set(PYTHON_VERSION_MINOR "${Python${VISOCYTE_PYTHON_VERSION}_VERSION_MINOR}")
    vtk_module_python_default_destination(VISOCYTE_PYTHON_SITE_PACKAGES_SUFFIX)
  endif ()
else ()
  set(vtk_requested_modules)
  foreach (vtk_module IN LISTS visocyte_requested_modules visocyte_plugin_required_modules)
    if (vtk_module MATCHES "Visocyte::")
      continue ()
    endif ()
    list(APPEND vtk_requested_modules
      "${vtk_module}")
  endforeach ()
  set(vtk_rejected_modules)
  foreach (vtk_module IN LISTS visocyte_rejected_modules)
    if (vtk_module MATCHES "Visocyte::")
      continue ()
    endif ()
    list(APPEND vtk_rejected_modules
      "${vtk_module}")
  endforeach ()

  cmake_dependent_option(VISOCYTE_BUILD_VTK_TESTING "" OFF
    "VISOCYTE_BUILD_TESTING" OFF)
  set(vtk_enable_tests "OFF")
  if (VISOCYTE_BUILD_VTK_TESTING)
    set(vtk_enable_tests "${VISOCYTE_BUILD_TESTING}")
    # Needed for `rtImageTest.py` tests to work. See note in `vtkModuleTesting`
    # about this.
    set(VTK_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/VTK")
  endif ()

  # Forward Python option to VTK.
  set(VTK_WRAP_PYTHON "${VISOCYTE_ENABLE_PYTHON}")

  # Forward logging option to VTK.
  set(VTK_ENABLE_LOGGING "${VISOCYTE_ENABLE_LOGGING}")
  if (VISOCYTE_ENABLE_LOGGING)
    list(APPEND vtk_requested_modules VTK::loguru)
  endif()

  vtk_module_find_modules(vtk_module_files "${CMAKE_CURRENT_SOURCE_DIR}/VTK")
  vtk_module_find_kits(vtk_kit_files "${CMAKE_CURRENT_SOURCE_DIR}/VTK")
  vtk_module_scan(
    MODULE_FILES        ${vtk_module_files}
    KIT_FILES           ${vtk_kit_files}
    REQUEST_MODULES     ${vtk_requested_modules}
                        ${visocyte_required_modules}
    REJECT_MODULES      ${vtk_rejected_modules}
    PROVIDES_MODULES    vtk_modules
    PROVIDES_KITS       vtk_kits
    REQUIRES_MODULES    vtk_required_modules
    UNRECOGNIZED_MODULES vtk_unrecognized_modules
    WANT_BY_DEFAULT     OFF
    ENABLE_TESTS        "${vtk_enable_tests}"
    HIDE_MODULES_FROM_CACHE ON)

  if (vtk_required_modules OR vtk_unrecognized_modules)
    message(FATAL_ERROR
      "The following modules were requested or required, but not found: "
      "${vtk_required_modules};${vtk_unrecognized_modules}.")
  endif ()

  if (VISOCYTE_ENABLE_PYTHON)
    find_package("Python${VISOCYTE_PYTHON_VERSION}" QUIET REQUIRED COMPONENTS Interpreter)
    set(PYTHON_VERSION_MAJOR "${Python${VISOCYTE_PYTHON_VERSION}_VERSION_MAJOR}")
    set(PYTHON_VERSION_MINOR "${Python${VISOCYTE_PYTHON_VERSION}_VERSION_MINOR}")
    vtk_module_python_default_destination(VISOCYTE_PYTHON_SITE_PACKAGES_SUFFIX)
    set(VTK_PYTHON_SITE_PACKAGES_SUFFIX "${VISOCYTE_PYTHON_SITE_PACKAGES_SUFFIX}")
  endif ()

  set(VTK_BUILD_QT_DESIGNER_PLUGIN OFF)
  set(VTK_INSTALL_PYTHON_EXES OFF)

  cmake_dependent_option(VTK_NO_PYTHON_THREADS "Disable Python Threads support" ON
    "VISOCYTE_ENABLE_PYTHON" OFF)
  mark_as_advanced(VTK_NO_PYTHON_THREADS)
  cmake_dependent_option(VTK_PYTHON_FULL_THREADSAFE "Wrap all Python calls with the GIL" ON
    "NOT VTK_NO_PYTHON_THREADS" OFF)
  mark_as_advanced(VTK_PYTHON_FULL_THREADSAFE)
  cmake_dependent_option(VTK_USE_TK "Build VTK with Tk support" OFF
    "VISOCYTE_ENABLE_PYTHON" OFF)

  vtk_module_build(
    MODULES             ${vtk_modules}
    KITS                ${vtk_kits}
    PACKAGE             VTK
    INSTALL_EXPORT      VTK
    HEADERS_DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/visocyte-${VISOCYTE_VERSION_MAJOR}.${VISOCYTE_VERSION_MINOR}"
    CMAKE_DESTINATION   "${visocyte_cmake_destination}/vtk"
    HIERARCHY_DESTINATION "${CMAKE_INSTALL_LIBDIR}/vtk/hierarchy/${CMAKE_PROJECT_NAME}"
    LIBRARY_NAME_SUFFIX "pv${VISOCYTE_VERSION_MAJOR}.${VISOCYTE_VERSION_MINOR}"
    VERSION             "${VISOCYTE_VERSION}"
    SOVERSION           "1"
    TEST_DATA_TARGET    VisocyteData
    BUILD_WITH_KITS     "${VISOCYTE_ENABLE_KITS}"
    USE_EXTERNAL        "${VISOCYTE_USE_EXTERNAL}"
    TEST_INPUT_DATA_DIRECTORY   "${CMAKE_CURRENT_SOURCE_DIR}/VTK/Testing"
    TEST_OUTPUT_DATA_DIRECTORY  "${CMAKE_CURRENT_BINARY_DIR}/ExternalData/VTK/Testing")

  if (NOT VISOCYTE_USE_EXTERNAL_VTK)
    set(vtk_cmake_dir
      "${CMAKE_CURRENT_SOURCE_DIR}/VTK/CMake")
    set(vtk_cmake_destination
      "${visocyte_cmake_destination}/vtk")
    set(vtk_cmake_build_dir
      "${CMAKE_CURRENT_BINARY_DIR}/${vtk_cmake_destination}")
    include("${CMAKE_CURRENT_SOURCE_DIR}/VTK/CMake/vtkInstallCMakePackage.cmake")
  endif ()
endif ()

set(visocyte_test_data_directory_input
  "${CMAKE_CURRENT_SOURCE_DIR}/Testing")
set(visocyte_test_data_directory_output
  "${CMAKE_CURRENT_BINARY_DIR}/ExternalData")
set(_visocyte_add_tests_default_data_directory
  "${visocyte_test_data_directory_output}")
set(_visocyte_add_tests_default_test_data_target
  "VisocyteData")

vtk_module_build(
  MODULES             ${visocyte_modules}
  KITS                ${visocyte_kits}
  INSTALL_EXPORT      Visocyte
  HEADERS_DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/visocyte-${VISOCYTE_VERSION_MAJOR}.${VISOCYTE_VERSION_MINOR}"
  CMAKE_DESTINATION   "${visocyte_cmake_destination}"
  HIERARCHY_DESTINATION "${CMAKE_INSTALL_LIBDIR}/vtk/hierarchy/${CMAKE_PROJECT_NAME}"
  LIBRARY_NAME_SUFFIX "pv${VISOCYTE_VERSION_MAJOR}.${VISOCYTE_VERSION_MINOR}"
  VERSION             "${VISOCYTE_VERSION}"
  SOVERSION           "1"
  TEST_DATA_TARGET    VisocyteData
  BUILD_WITH_KITS     "${VISOCYTE_ENABLE_KITS}"
  USE_EXTERNAL        "${VISOCYTE_USE_EXTERNAL}"
  TEST_INPUT_DATA_DIRECTORY   "${visocyte_test_data_directory_input}"
  TEST_OUTPUT_DATA_DIRECTORY  "${visocyte_test_data_directory_output}")

include(vtkModuleJson)
vtk_module_json(
  MODULES "${vtk_modules}"
  OUTPUT "vtk-modules.json")
vtk_module_json(
  MODULES "${visocyte_modules}"
  OUTPUT "visocyte-modules.json")

visocyte_plugin_build(
  RUNTIME_DESTINATION   "${CMAKE_INSTALL_BINDIR}"
  LIBRARY_DESTINATION   "${CMAKE_INSTALL_LIBDIR}"
  LIBRARY_SUBDIRECTORY  "visocyte-${VISOCYTE_VERSION_MAJOR}.${VISOCYTE_VERSION_MINOR}/plugins"
  PLUGINS_FILE_NAME ".plugins"
  TARGET  visocyte_plugins
  PLUGINS ${visocyte_plugins})

vtk_module_wrap_client_server(
  MODULES   ${visocyte_modules}
            ${vtk_modules}
  TARGET    visocyte_client_server
  INSTALL_EXPORT Visocyte)

visocyte_server_manager_process(
  MODULES   ${visocyte_modules}
            ${vtk_modules}
  TARGET    visocyte_server_manager
  XML_FILES visocyte_server_manager_files)

if (VISOCYTE_ENABLE_PYTHON)
  if (VISOCYTE_USE_EXTERNAL_VTK)
    # TODO: Check if VTK is already wrapped in Python.
    # Error out?
  else ()
    vtk_module_wrap_python(
      MODULES         ${vtk_modules}
      INSTALL_EXPORT  VTK
      PYTHON_PACKAGE  "vtkmodules"
      MODULE_DESTINATION  "${VISOCYTE_PYTHON_SITE_PACKAGES_SUFFIX}"
      CMAKE_DESTINATION   "${visocyte_cmake_destination}/vtk"
      WRAPPED_MODULES vtk_python_wrapped_modules
      TARGET          VTK::vtkpythonmodules)

    add_subdirectory(VTK/Wrapping/Python)
  endif ()

  get_property(vtk_required_python_modules GLOBAL
    PROPERTY  vtk_required_python_modules)
  if (vtk_required_python_modules)
    list(REMOVE_DUPLICATES vtk_required_python_modules)
  endif ()
  string(REPLACE ";" "\n" vtk_required_python_modules "${vtk_required_python_modules}")
  file(WRITE "${CMAKE_BINARY_DIR}/requirements.txt"
    "${vtk_required_python_modules}\n")

  vtk_module_wrap_python(
    MODULES         ${visocyte_modules}
    INSTALL_EXPORT  Visocyte
    PYTHON_PACKAGE  "visocyte.modules"
    MODULE_DESTINATION  "${VISOCYTE_PYTHON_SITE_PACKAGES_SUFFIX}"
    CMAKE_DESTINATION   "${visocyte_cmake_destination}"
    WRAPPED_MODULES visocyte_python_wrapped_modules
    TARGET          Visocyte::pvpythonmodules)
endif ()

if (VISOCYTE_ENABLE_WEB)
  configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/Web/package.json.in"
    "${CMAKE_CURRENT_BINARY_DIR}/package.json"
    @ONLY)
endif ()

if (VISOCYTE_ENABLE_COMMANDLINE_TOOLS)
  add_subdirectory(CommandLineExecutables)
endif ()
add_subdirectory(Applications)

if (VISOCYTE_ENABLE_PYTHON)
  add_subdirectory(Wrapping/Python)
endif ()

if (NOT VISOCYTE_USE_EXTERNAL_VTK)
  # Configure the CTestCustom.cmake file for exclusions.
  configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/VTK/CMake/CTestCustom.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/VTK/CTestCustom.cmake"
    @ONLY)
endif ()

# Configure the CTestCustom.cmake file for exclusions.
configure_file(
  "${visocyte_cmake_dir}/CTestCustom.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/CTestCustom.cmake"
  @ONLY)

ExternalData_Add_Target(VisocyteData)

set(visocyte_cmake_build_dir
  "${CMAKE_CURRENT_BINARY_DIR}/${visocyte_cmake_destination}")
include("${CMAKE_CURRENT_SOURCE_DIR}/CMake/VisocyteInstallCMakePackage.cmake")

set(is_git FALSE)
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.git")
  set(is_git TRUE)
endif ()

if (NOT WIN32 AND is_git AND Git_FOUND AND NOT GIT_VERSION_STRING VERSION_LESS "2.18.0")
  set(source_all)
  if (VISOCYTE_SOURCE_TARBALL_TARGETS)
    set(source_all ALL)
  endif ()

  add_custom_target(visocyte-source ${source_all})
  foreach (format tgz txz zip)
    add_custom_target("visocyte-source-${format}" ${source_all}
      COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/Utilities/Maintenance/create_tarballs.bash"
              "--${format}"
      WORKING_DIRECTORY
              "${CMAKE_CURRENT_BINARY_DIR}"
      COMMENT "Creating source tarball in ${format} format")
    add_dependencies("visocyte-source-${format}"
      VisocyteData)
    add_dependencies(visocyte-source
      "visocyte-source-${format}")
  endforeach ()
  # This is a hack.  Git version 2.17 does NOT like running these in parallel and
  # reports that a lockfile is already locked.  In order for the build to succeed,
  # force these to run in serial.
  add_dependencies(visocyte-source-tgz visocyte-source-txz)
  add_dependencies(visocyte-source-txz visocyte-source-zip)
endif ()

# This is the directory under which all generated docs, such as Doxygen and
# Sphinx generated docs, are placed.
if (NOT VISOCYTE_GENERATED_DOCUMENTATION_OUTPUT_DIRECTORY)
  set(VISOCYTE_GENERATED_DOCUMENTATION_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/doc")
endif ()

#-----------------------------------------------------------------------------
# Build doxygen documentation.
if (VISOCYTE_BUILD_DEVELOPER_DOCUMENTATION)
  add_subdirectory(Utilities/Doxygen)
  add_subdirectory(Utilities/Sphinx)
  add_subdirectory(Documentation)
endif()

#------------------------------------------------------------------------------
# Test to check install tree
#------------------------------------------------------------------------------
if (BUILD_TESTING AND VISOCYTE_INSTALL_DEVELOPMENT_FILES)
  add_test( NAME pv.TestDevelopmentInstall
    COMMAND ${CMAKE_COMMAND}
    -DVISOCYTE_BINARY_DIR:PATH=${CMAKE_BINARY_DIR}
    -DVISOCYTE_INSTALL_DIR:PATH=${CMAKE_INSTALL_PREFIX}
    -DVISOCYTE_SOURCE_DIR:PATH=${CMAKE_SOURCE_DIR}
    -DVISOCYTE_TEST_DIR:PATH=${CMAKE_BINARY_DIR}/Testing/Temporary
    -DVISOCYTE_VERSION:STRING=${VISOCYTE_VERSION}
    -P ${CMAKE_CURRENT_SOURCE_DIR}/CMake/VisocyteTestInstall.cmake)
  set_tests_properties(pv.TestDevelopmentInstall PROPERTIES
    LABELS "visocyte"
    ENVIRONMENT "DESTDIR=${CMAKE_BINARY_DIR}/test-install")
endif()
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# Test to check that we can process all of the Catalyst editions
#------------------------------------------------------------------------------
if (BUILD_TESTING AND VISOCYTE_ENABLE_PYTHON AND UNIX AND NOT APPLE)
if (FALSE) # XXX(FIXME)
  # disabling till we Catalyst editions are fixed post modularization
  add_test( NAME GenerateCatalystEditions
    COMMAND ${CMAKE_COMMAND}
    -DPYTHON_EXECUTABLE:FILEPATH=${PYTHON_EXECUTABLE}
    -DVISOCYTE_SOURCE_DIR:PATH=${CMAKE_SOURCE_DIR}
    -DVISOCYTE_TEST_DIR:PATH=${CMAKE_BINARY_DIR}/Testing/Temporary
    -P ${CMAKE_SOURCE_DIR}/CMake/VisocyteTestCatalystCodeGeneration.cmake)
  set_tests_properties(GenerateCatalystEditions PROPERTIES
    LABELS "visocyte;catalyst")
endif()
endif()
#------------------------------------------------------------------------------

#-----------------------------------------------------------------------------
if (VISOCYTE_BUILD_CATALYST_ADAPTORS)
  # BuildAdaptors.cmake builds the adaptors as a separate project. We mark is
  # OPTIONAL for Catalyst packages that don't have the adaptors included.
  include(${CMAKE_CURRENT_SOURCE_DIR}/CoProcessing/Adaptors/BuildAdaptors.cmake
          OPTIONAL)
endif()

#-----------------------------------------------------------------------------
if (VISOCYTE_ENABLE_EXAMPLES)
  add_subdirectory(Examples)
endif ()
