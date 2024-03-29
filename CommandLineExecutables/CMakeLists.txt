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

# TODO: Restore Cray ATP support via a new module.
# TODO: Support static plugins.
# TODO: Link pvserver and friends to Visocyte::PythonInitializer for static builds?

include("${CMAKE_CURRENT_SOURCE_DIR}/VisocyteCommandLineExecutables.cmake")

foreach (exe IN ITEMS pvserver pvdataserver pvrenderserver)
  visocyte_add_executable("${exe}" "${exe}.cxx")
endforeach ()

if (VISOCYTE_ENABLE_PYTHON)
  foreach (exe IN ITEMS pvbatch pvpython)
    visocyte_add_executable("${exe}" "${exe}.cxx")
    target_link_libraries("${exe}"
      PRIVATE
        VTK::PythonInterpreter)
  endforeach ()
endif ()

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/visocyte-config"
  "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/visocyte-config"
  COPYONLY)
install(
  PROGRAMS    "${CMAKE_CURRENT_SOURCE_DIR}/visocyte-config"
  DESTINATION "${CMAKE_INSTALL_BINDIR}"
  COMPONENT   development)
