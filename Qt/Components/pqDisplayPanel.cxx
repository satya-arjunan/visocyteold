/*=========================================================================

   Program: Visocyte
   Module:    pqDisplayPanel.cxx

   Copyright (c) 2005-2008 Sandia Corporation, Kitware Inc.
   All rights reserved.

   Visocyte is a free software; you can redistribute it and/or modify it
   under the terms of the Visocyte license version 1.2.

   See License_v1.2.txt for the full Visocyte license.
   A copy of this license can be obtained by contacting
   Kitware Inc.
   28 Corporate Drive
   Clifton Park, NY 12065
   USA

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/

#include "pqDisplayPanel.h"
#include "pqDataRepresentation.h"
#include "pqPipelineSource.h"
#include "vtkSMProperty.h"
#include "vtkSMProxy.h"

pqDisplayPanel::pqDisplayPanel(pqRepresentation* display, QWidget* p)
  : QWidget(p)
  , Representation(display)
{
  pqDataRepresentation* dataRepr = qobject_cast<pqDataRepresentation*>(display);
  if (dataRepr)
  {
    pqPipelineSource* input = dataRepr->getInput();
    QObject::connect(input, SIGNAL(dataUpdated(pqPipelineSource*)), this, SLOT(dataUpdated()));
    this->dataUpdated();
  }
}

pqDisplayPanel::~pqDisplayPanel()
{
}

pqRepresentation* pqDisplayPanel::getRepresentation()
{
  return this->Representation;
}

void pqDisplayPanel::reloadGUI()
{
}

void pqDisplayPanel::dataUpdated()
{
}

void pqDisplayPanel::updateAllViews()
{
  if (this->Representation)
  {
    this->Representation->renderViewEventually();
  }
}
