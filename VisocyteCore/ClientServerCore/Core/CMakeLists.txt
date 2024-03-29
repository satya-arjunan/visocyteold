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
set(classes
  vtkCompleteArrays
  vtkEnvironmentAnnotationFilter
  vtkExtractsDeliveryHelper
  vtkMPIMToNSocketConnection
  vtkMPIMToNSocketConnectionPortInformation
  vtkNetworkAccessManager
  vtkPConvertSelection
  vtkPVAlgorithmPortsInformation
  vtkPVArrayInformation
  vtkPVClassNameInformation
  vtkPVCompositeDataInformation
  vtkPVCompositeDataInformationIterator
  vtkPVDataInformation
  vtkPVDataInformationHelper
  vtkPVDataSetAttributesInformation
  vtkPVDataSizeInformation
  vtkPVDisableStackTraceSignalHandler
  vtkPVEnableStackTraceSignalHandler
  vtkPVExtractSelection
  vtkPVFileInformationHelper
  vtkPVGenericAttributeInformation
  vtkPVInformation
  vtkPVMemoryUseInformation
  vtkPVMultiClientsInformation
  vtkPVOptions
  vtkPVOptionsXMLParser
  vtkPVPlugin
  vtkPVPluginLoader
  vtkPVPluginTracker
  vtkPVPluginsInformation
  vtkPVProgressHandler
  vtkPVPythonInformation
  vtkPVPythonModule
  vtkPVPythonOptions
  vtkPVPythonPluginInterface
  vtkPVSILInformation
  vtkPVServerInformation
  vtkPVServerManagerPluginInterface
  vtkPVServerOptions
  vtkPVSession
  vtkPVSystemConfigInformation
  vtkPVSystemInformation
  vtkPVTemporalDataInformation
  vtkPVTimerInformation
  vtkProcessModule
  vtkProcessModuleAutoMPI
  vtkSession
  vtkSessionIterator
  vtkTCPNetworkAccessManager)

if (TARGET VTK::PythonInterpreter)
  # TODO: Split these into a separate module so that there isn't a public
  # optional dependency.
  list(APPEND classes
    vtkPythonExtractSelection
    vtkPythonSelector)
endif ()

cmake_dependent_option(VISOCYTE_USE_MPI_SSEND
  "Use MPI synchronous-send commands for communication" OFF
  "VISOCYTE_USE_MPI" OFF)
cmake_dependent_option(VISOCYTE_INITIALIZE_MPI_ON_CLIENT
  "Initialize MPI on client-processes by default. Can be overridden using command line arguments" ON
  "VISOCYTE_USE_MPI" OFF)
mark_as_advanced(
  VISOCYTE_INITIALIZE_MPI_ON_CLIENT
  VISOCYTE_USE_MPI_SSEND)

set(vtkProcessModule_defines)
if (VISOCYTE_USE_MPI_SSEND)
  list(APPEND vtkProcessModule_defines
    VISOCYTE_USE_MPI_SSEND)
endif ()
if (VISOCYTE_INITIALIZE_MPI_ON_CLIENT)
  list(APPEND vtkProcessModule_defines
    VISOCYTE_INITIALIZE_MPI_ON_CLIENT)
endif ()
set_source_files_properties(vtkProcessModule.cxx
  PROPERTIES
    COMPILE_DEFINITIONS "${vtkProcessModule_defines}")

# TODO: Expose CMake options for these?
set(vtkPVOptions_defines)
if (VISOCYTE_ALWAYS_SECURE_CONNECTION)
  list(APPEND vtkPVOptions_defines
    VISOCYTE_ALWAYS_SECURE_CONNECTION)
endif ()
if (VISOCYTE_ENABLE_FPE)
  list(APPEND vtkPVOptions_defines
    VISOCYTE_ENABLE_FPE)
endif ()
if (VISOCYTE_WITH_SUPERBUILD_MESA)
  list(APPEND vtkPVOptions_defines
    VISOCYTE_WITH_SUPERBUILD_MESA)

  if (VISOCYTE_WITH_SUPERBUILD_MESA_SWR)
    list(APPEND vtkPVOptions_defines
      VISOCYTE_WITH_SUPERBUILD_MESA_SWR)
  endif ()
endif ()
if (VTK_OPENGL_HAS_EGL)
  list(APPEND vtkPVOptions_defines
    VTK_OPENGL_HAS_EGL)
endif ()
set_source_files_properties(vtkPVOptions.cxx
  PROPERTIES
    COMPILE_DEFINITIONS "${vtkPVOptions_defines}")

set(vtkProcessModuleAutoMPI_defines)
if (TARGET VTK::ParallelMPI)
  list(APPEND vtkProcessModuleAutoMPI_defines
    "VISOCYTE_MPIEXEC_EXECUTABLE=\"${MPIEXEC_EXECUTABLE}\""
    "VISOCYTE_MPI_NUMPROC_FLAG=\"${MPIEXEC_NUMPROC_FLAG}\"")
  if (MPI_PREFLAGS)
    list(APPEND vtkProcessModuleAutoMPI_defines
      "VISOCYTE_MPI_PREFLAGS=\"${MPIEXEC_PREFLAGS}\"")
  endif ()
  if (MPI_POSTFLAGS)
    list(APPEND vtkProcessModuleAutoMPI_defines
      "VISOCYTE_MPI_POSTFLAGS=\"${MPIEXEC_POSTFLAGS}\"")
  endif ()
endif ()
set_source_files_properties(vtkProcessModuleAutoMPI.cxx
  PROPERTIES
    COMPILE_DEFINITIONS "${vtkProcessModuleAutoMPI_defines}")

set(VISOCYTE_PLUGIN_LOADER_PATHS ""
  CACHE STRING "Extra paths to search for plugins")
mark_as_advanced(VISOCYTE_PLUGIN_LOADER_PATHS)
if (VISOCYTE_PLUGIN_LOADER_PATHS)
  set_source_files_properties(vtkPVPluginLoader.cxx
    PROPERTIES
      COMPILE_DEFINITIONS VISOCYTE_PLUGIN_LOADER_PATHS=\"${VISOCYTE_PLUGIN_LOADER_PATHS}\")
endif ()

vtk_module_add_module(Visocyte::ClientServerCoreCore
  CLASSES ${classes})
