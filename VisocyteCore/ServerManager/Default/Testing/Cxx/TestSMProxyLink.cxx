/*=========================================================================

Program:   Visocyte
Module:    vtkSMProxyTest.cxx

Copyright (c) Kitware, Inc.
All rights reserved.
See Copyright.txt or http://www.visocyte.org/HTML/Copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "vtkSMProxyLinkTest.h"

#include "vtkInitializationHelper.h"
#include "vtkPVOptions.h"
#include "vtkProcessModule.h"

int TestSMProxyLink(int argc, char* argv[])
{
  vtkPVOptions* options = vtkPVOptions::New();
  vtkInitializationHelper::Initialize(argc, argv, vtkProcessModule::PROCESS_CLIENT, options);

  vtkSMProxyLinkTest test;
  int ret = QTest::qExec(&test, argc, argv);

  vtkInitializationHelper::Finalize();
  options->Delete();

  return ret;
}
