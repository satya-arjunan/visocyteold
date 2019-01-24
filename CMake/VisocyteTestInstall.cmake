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

#
# This script tests the Visocyte install tree by building the examples
#
# The script expects the following input variables:
# PARAVIEW_BINARY_DIR : Build path for Visocyte (To run 'make install' here)
# PARAVIEW_INSTALL_DIR : Install path for Visocyte Examples are built against
#                        (This should be CMAKE_INSTALL_PREFIX set for Visocyte)
# PARAVIEW_SOURCE_DIR : Visocyte source dir (For source location of Examples)
# PARAVIEW_TEST_DIR : Temporary directory for location of Examples build tree
# PARAVIEW_VERSION : Visocyte version string used when creating the installtree

message (STATUS "Building Examples against Visocyte install tree")
set (Visocyte_DIR
  $ENV{DESTDIR}${PARAVIEW_INSTALL_DIR}/lib/cmake/visocyte-${PARAVIEW_VERSION})

# Build target "INSTALL" for visocyte
if (MSVC)
  set (install_tgt "INSTALL")
  set (opt "/maxcpucount")
else ()
  set (install_tgt "install")
  set (opt "-j5")
endif ()
execute_process (COMMAND ${CMAKE_COMMAND}
  --build ${PARAVIEW_BINARY_DIR}
  --target ${install_tgt}
  -- ${opt}
  WORKING_DIRECTORY ${PARAVIEW_BINARY_DIR}
  RESULT_VARIABLE irv)
if (NOT irv EQUAL 0)
  message(FATAL_ERROR "Could not build target 'install' for Visocyte")
endif ()

set (INSTALL_TEST_BUILD_DIR ${PARAVIEW_TEST_DIR}/Examples-bld)
if (NOT EXISTS ${INSTALL_TEST_BUILD_DIR})
  execute_process(COMMAND ${CMAKE_COMMAND} -E
    make_directory ${INSTALL_TEST_BUILD_DIR})
endif ()
execute_process (
  COMMAND ${CMAKE_COMMAND}
  -DVisocyte_DIR:PATH=${Visocyte_DIR}
  ${PARAVIEW_SOURCE_DIR}/Examples
  WORKING_DIRECTORY ${INSTALL_TEST_BUILD_DIR}
  RESULT_VARIABLE crv)
if (NOT crv EQUAL 0)
  message(FATAL_ERROR "Configuration failed with return code ${crv}")
endif ()
execute_process (
  COMMAND ${CMAKE_COMMAND} --build ${INSTALL_TEST_BUILD_DIR} --clean-first
  WORKING_DIRECTORY ${INSTALL_TEST_BUILD_DIR}
  RESULT_VARIABLE rv)
if (NOT rv EQUAL 0)
  message(FATAL_ERROR "Build failed with return code ${rv}")
endif ()
