/*=========================================================================

Program:   Visocyte
Module:    pvbatch.cxx

Copyright (c) Kitware, Inc.
All rights reserved.
See Copyright.txt or http://www.visocyte.org/HTML/Copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "pvpython.h" // include this 1st.

#include "vtkOutputWindow.h"
#include "vtkPVConfig.h" // Required to get build options for visocyte
#include "vtkProcessModule.h"

int main(int argc, char* argv[])
{
  // Setup the output window to be vtkOutputWindow, rather than platform
  // specific one. This avoids creating vtkWin32OutputWindow on Windows, for
  // example, which puts all Python errors in a window rather than the terminal
  // as one would expect.
  auto opwindow = vtkOutputWindow::New();
  vtkOutputWindow::SetInstance(opwindow);
  opwindow->Delete();

  return VisocytePython::Run(vtkProcessModule::PROCESS_BATCH, argc, argv);
}
