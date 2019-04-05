/*=========================================================================

  Program:   ParaView
  Module:    vtkPVPanoramicProjectionView.cxx

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.visocyte.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkPVPanoramicProjectionView.h"

#include "vtkCullerCollection.h"
#include "vtkObjectFactory.h"
#include "vtkPVSynchronizedRenderer.h"
#include "vtkPanoramicProjectionPass.h"
#include "vtkRenderer.h"

//-----------------------------------------------------------------------------
vtkStandardNewMacro(vtkPVPanoramicProjectionView);

//----------------------------------------------------------------------------
void vtkPVPanoramicProjectionView::Initialize(unsigned int id)
{
  this->Superclass::Initialize(id);

  this->SynchronizedRenderers->SetImageProcessingPass(this->ProjectionPass);

  // remove cullers
  this->GetRenderer()->GetCullers()->RemoveAllItems();

  // depth peeling is not working currently with panoramic pass
  this->GetRenderer()->UseDepthPeelingOff();

  // FXAA is not supported when this pass is active
  this->UseFXAA = false;
}

//----------------------------------------------------------------------------
void vtkPVPanoramicProjectionView::SetProjectionType(int type)
{
  this->ProjectionPass->SetProjectionType(type);
}

//----------------------------------------------------------------------------
void vtkPVPanoramicProjectionView::SetCubeResolution(int resolution)
{
  this->ProjectionPass->SetCubeResolution(resolution);
}

//----------------------------------------------------------------------------
void vtkPVPanoramicProjectionView::SetAngle(double angle)
{
  this->ProjectionPass->SetAngle(angle);
}

//----------------------------------------------------------------------------
void vtkPVPanoramicProjectionView::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}
