/*=========================================================================

   Program: Visocyte
   Module:    pqSetMainWindowTitleReaction.h

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
#ifndef pqSetMainWindowTitleReaction_h
#define pqSetMainWindowTitleReaction_h

#include "pqReaction.h"

/**
* @ingroup Reactions
* pqSetMainWindowTitleReaction is the reaction that popups the Set Window Title
* Dialog allowing the user edit the main window title.
*/
class PQAPPLICATIONCOMPONENTS_EXPORT pqSetMainWindowTitleReaction : public pqReaction
{
  Q_OBJECT
  typedef pqReaction Superclass;

public:
  /**
  * Constructor. Parent cannot be NULL.
  */
  pqSetMainWindowTitleReaction(QAction* parent);
  ~pqSetMainWindowTitleReaction() override;

  /**
  * Show the main window title editor dialog.
  */
  void showSetMainWindowTitleDialog();

protected:
  /**
  * Called when the action is triggered.
  */
  void onTriggered() override { pqSetMainWindowTitleReaction::showSetMainWindowTitleDialog(); }

private:
  Q_DISABLE_COPY(pqSetMainWindowTitleReaction)
};

#endif
