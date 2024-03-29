#=========================================================================
#
#   Program: Visocyte
#   Module:  CMakeLists.txt
#
#   Copyright (c) 2005,2006 Sandia Corporation, Kitware Inc.
#   All rights reserved.
#
#   Visocyte is a free software; you can redistribute it and/or modify it
#   under the terms of the Visocyte license version 1.2.
#
#   See License_v1.2.txt for the full Visocyte license.
#   A copy of this license can be obtained by contacting
#   Kitware Inc.
#   28 Corporate Drive
#   Clifton Park, NY 12065
#   USA
#
#THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
#LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
#A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR
#CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
#EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
#PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
#LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
#NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
#SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
#========================================================================
cmake_minimum_required(VERSION 3.8)
project(MultiServerClient)

find_package(Visocyte REQUIRED)

if (NOT VISOCYTE_BUILD_QT_GUI)
  message(STATUS
    "${CMAKE_PROJECT_NAME} example requires VISOCYTE_BUILD_QT_GUI to be "
    "enabled. Please rebuild Visocyte (or point to a different build of "
    "Visocyte) with VISOCYTE_BUILD_QT_GUI set to TRUE")
  return ()
endif()

include(GNUInstallDirs)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")

set(CMAKE_AUTOMOC 1)
set(CMAKE_AUTOUIC 1)

set(sources
  MultiServerClientMainWindow.cxx
  MultiServerClientMainWindow.h)
set(ui_files
  MultiServerClientMainWindow.ui)
set(xml_files
  Sources.xml)

visocyte_client_add(
  NAME multiserverclient
  VERSION "1.0.0"
  SOURCES ${sources} ${ui_files}
  APPLICATION_XMLS ${xml_files}

  MAIN_WINDOW_CLASS MultiServerClientMainWindow

  ORGANIZATION "Kitware, Inc."
  TITLE "Multi-Server Client (Visocyte-based Example Application)")
