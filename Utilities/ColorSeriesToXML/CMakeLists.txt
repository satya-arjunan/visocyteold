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
vtk_module_impl()
vtk_module_export("")

include(vtkForwardingExecutable)
if (NOT CMAKE_CROSSCOMPILING)
  add_executable(vtkColorSeriesToXML ColorSeriesToXML.cxx)
  target_link_libraries(vtkColorSeriesToXML
    vtkCommonCore
    vtkCommonExecutionModel
    vtkCommonColor
  )

  if (UNIX)
    # setup install rpath correctly so that vtkColorSeriesToXML can file needed libraries
    # at runtime.
    set (_temp_prefix "/example-prefix")
    set (_bin "${_temp_prefix}/${VTK_INSTALL_RUNTIME_DIR}")
    set (_so "${_temp_prefix}/${VTK_INSTALL_LIBRARY_DIR}")
    file (RELATIVE_PATH relative_path "${_bin}" "${_so}")
    set_target_properties(vtkColorSeriesToXML PROPERTIES
      INSTALL_RPATH "\$ORIGIN/${relative_path}")
  endif()

  vtk_compile_tools_target(vtkColorSeriesToXML)
endif()
