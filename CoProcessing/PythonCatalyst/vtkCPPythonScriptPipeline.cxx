/*=========================================================================

  Program:   Visocyte
  Module:    vtkCPPythonScriptPipeline.cxx

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.visocyte.org/HTML/Copyright.html for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notice for more information.

  =========================================================================*/
#include "vtkCPPythonScriptPipeline.h"

#include "vtkCPDataDescription.h"
#include "vtkMultiProcessController.h"
#include "vtkObjectFactory.h"
#include "vtkPythonInterpreter.h"

#include <sstream>
#include <string>
#include <vtksys/SystemTools.hxx>

vtkStandardNewMacro(vtkCPPythonScriptPipeline);
//----------------------------------------------------------------------------
vtkCPPythonScriptPipeline::vtkCPPythonScriptPipeline()
{
  this->PythonScriptName = nullptr;
}

//----------------------------------------------------------------------------
vtkCPPythonScriptPipeline::~vtkCPPythonScriptPipeline()
{
  this->SetPythonScriptName(nullptr);
}

//----------------------------------------------------------------------------
int vtkCPPythonScriptPipeline::Initialize(const char* fileName)
{
  // only process 0 checks if the file exists and broadcasts that information
  // to the other processes
  int fileExists = 0;
  vtkMultiProcessController* controller = vtkMultiProcessController::GetGlobalController();
  if (controller->GetLocalProcessId() == 0)
  {
    fileExists = vtksys::SystemTools::FileExists(fileName, true);
  }
  controller->Broadcast(&fileExists, 1, 0);
  if (fileExists == 0)
  {
    vtkErrorMacro("Could not find file " << fileName);
    return 0;
  }

  // for now do not check on filename extension:
  // vtksys::SystemTools::GetFilenameLastExtension(FileName) == ".py" == 0)

  std::string fileNamePath = vtksys::SystemTools::GetFilenamePath(fileName);
  std::string fileNameName = vtksys::SystemTools::GetFilenameWithoutExtension(
    vtksys::SystemTools::GetFilenameName(fileName));
  // need to save the script name as it is used as the name of the module
  this->SetPythonScriptName(fileNameName.c_str());

  // only process 0 reads the actual script and then broadcasts it out
  char* scriptText = nullptr;
  // we need to add the script path to PYTHONPATH
  char* scriptPath = nullptr;

  int rank = controller->GetLocalProcessId();
  int scriptSizes[2] = { 0, 0 };
  if (rank == 0)
  {
    std::string line;
    std::ifstream myfile(fileName);
    std::string desiredString;
    if (myfile.is_open())
    {
      while (getline(myfile, line))
      {
        this->FixEOL(line);
        desiredString.append(line).append("\n");
      }
      myfile.close();
    }

    if (fileNamePath.empty())
    {
      fileNamePath = ".";
    }
    scriptSizes[0] = static_cast<int>(fileNamePath.size() + 1);
    scriptPath = new char[scriptSizes[0]];
    memcpy(scriptPath, fileNamePath.c_str(), sizeof(char) * scriptSizes[0]);

    scriptSizes[1] = static_cast<int>(desiredString.size() + 1);
    scriptText = new char[scriptSizes[1]];
    memcpy(scriptText, desiredString.c_str(), sizeof(char) * scriptSizes[1]);
  }

  controller->Broadcast(scriptSizes, 2, 0);

  if (rank != 0)
  {
    scriptPath = new char[scriptSizes[0]];
    scriptText = new char[scriptSizes[1]];
  }

  controller->Broadcast(scriptPath, scriptSizes[0], 0);
  controller->Broadcast(scriptText, scriptSizes[1], 0);

  vtkPythonInterpreter::PrependPythonPath(scriptPath);

  // The code below creates a module from the scriptText string.
  // This requires the manual creation of a module object like this:
  //
  // import types
  // _foo = types.ModuleType('foo')
  // _foo.__file__ = 'foo.pyc'
  // import sys
  // sys.module['foo'] = _foo
  // _source= scriptText
  // _code = compile(_source, 'foo.py', 'exec')
  // exec _code in _foo.__dict__
  // del _source
  // del _code
  // import foo
  std::ostringstream loadPythonModules;
  loadPythonModules << "import types" << std::endl;
  loadPythonModules << "_" << fileNameName << " = types.ModuleType('" << fileNameName << "')"
                    << std::endl;
  loadPythonModules << "_" << fileNameName << ".__file__ = '" << fileNameName << ".pyc'"
                    << std::endl;

  loadPythonModules << "import sys" << std::endl;
  loadPythonModules << "sys.modules['" << fileNameName << "'] = _" << fileNameName << std::endl;

  loadPythonModules << "_source = \"\"\"" << std::endl;
  loadPythonModules << scriptText;
  loadPythonModules << "\"\"\"" << std::endl;

  loadPythonModules << "_code = compile(_source, \"" << fileNameName << ".py\", \"exec\")"
                    << std::endl;
  loadPythonModules << "exec(_code, _" << fileNameName << ".__dict__)" << std::endl;
  loadPythonModules << "del _source" << std::endl;
  loadPythonModules << "del _code" << std::endl;
  loadPythonModules << "import " << fileNameName << std::endl;

  delete[] scriptPath;
  delete[] scriptText;

  vtkPythonInterpreter::RunSimpleString(loadPythonModules.str().c_str());
  return 1;
}

//----------------------------------------------------------------------------
int vtkCPPythonScriptPipeline::RequestDataDescription(vtkCPDataDescription* dataDescription)
{
  if (!dataDescription)
  {
    vtkWarningMacro("dataDescription is NULL.");
    return 0;
  }

  // check the script to see if it should be run...
  vtkStdString dataDescriptionString = this->GetPythonAddress(dataDescription);

  std::ostringstream pythonInput;
  pythonInput << "dataDescription = vtkPVCatalyst.vtkCPDataDescription('" << dataDescriptionString
              << "')\n"
              << this->PythonScriptName << ".RequestDataDescription(dataDescription)\n";

  vtkPythonInterpreter::RunSimpleString(pythonInput.str().c_str());

  return dataDescription->GetIfAnyGridNecessary() ? 1 : 0;
}

//----------------------------------------------------------------------------
int vtkCPPythonScriptPipeline::CoProcess(vtkCPDataDescription* dataDescription)
{
  if (!dataDescription)
  {
    vtkWarningMacro("DataDescription is NULL.");
    return 0;
  }

  vtkStdString dataDescriptionString = this->GetPythonAddress(dataDescription);

  std::ostringstream pythonInput;
  pythonInput << "dataDescription = vtkPVCatalyst.vtkCPDataDescription('" << dataDescriptionString
              << "')\n"
              << this->PythonScriptName << ".DoCoProcessing(dataDescription)\n";

  vtkPythonInterpreter::RunSimpleString(pythonInput.str().c_str());

  return 1;
}

//----------------------------------------------------------------------------
int vtkCPPythonScriptPipeline::Finalize()
{
  std::ostringstream pythonInput;
  pythonInput << "if hasattr(" << this->PythonScriptName << ", 'Finalize'):\n"
              << "  " << this->PythonScriptName << ".Finalize()\n";

  vtkPythonInterpreter::RunSimpleString(pythonInput.str().c_str());

  return 1;
}

//----------------------------------------------------------------------------
void vtkCPPythonScriptPipeline::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << "PythonScriptName: " << this->PythonScriptName << "\n";
}
