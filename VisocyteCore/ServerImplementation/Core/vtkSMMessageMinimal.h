/*=========================================================================

  Program:   Visocyte
  Module:    $RCSfile$

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.visocyte.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// Include this header in your header files where you are referring to
// vtkSMMessage or vtkSMMessageCollection. It simply forward declares these
// without including any protobuf headers. In your cxx files, you need to
// include vtkSMMessage.h to see the full definition of the protobuf message
// classes as well as other operators.

#ifndef vtkSMMessageMinimal_h
#define vtkSMMessageMinimal_h

#include "vtkPVServerImplementationCoreModule.h"
#include "vtkSystemIncludes.h"

namespace visocyte_protobuf
{
class Message;
class MessageCollection;
}

typedef visocyte_protobuf::Message vtkSMMessage;
typedef visocyte_protobuf::MessageCollection vtkSMMessageCollection;

#endif

// VTK-HeaderTest-Exclude: vtkSMMessageMinimal.h
