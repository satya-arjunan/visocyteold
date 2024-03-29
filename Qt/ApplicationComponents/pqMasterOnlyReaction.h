/*=========================================================================

   Program: Visocyte
   Module:    pqMasterOnlyReaction.h

   Copyright (c) 2005,2006 Sandia Corporation, Kitware Inc.
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

========================================================================*/
#ifndef pqMasterOnlyReaction_h
#define pqMasterOnlyReaction_h

#include "pqApplicationComponentsModule.h"
#include "pqReaction.h"
#include <QAction>

/**
* @defgroup Reactions Visocyte Reactions
* Visocyte client relies of a collection of reactions that autonomous entities
* that use pqApplicationCore and other core components to get their work done which
* keeping track for their own enabled state without any external input. To
* use, simple create this reaction and assign it to a menu
* and add it to menus/toolbars whatever.
*/

/**
* @ingroup Reactions
* This is a superclass just to make it easier to collect all such reactions.
*/
class PQAPPLICATIONCOMPONENTS_EXPORT pqMasterOnlyReaction : public pqReaction
{
  Q_OBJECT
  typedef pqReaction Superclass;

public:
  pqMasterOnlyReaction(QAction* parentObject);
  pqMasterOnlyReaction(QAction* parentObject, Qt::ConnectionType type);

protected slots:
  void updateEnableState() override;

private:
  Q_DISABLE_COPY(pqMasterOnlyReaction)
};

#endif
