/*=========================================================================

   Program: Visocyte
   Module:  pqLineEditEventPlayer.h

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
#ifndef pqLineEditEventPlayer_h
#define pqLineEditEventPlayer_h

#include "pqAbstractStringEventPlayer.h"
#include "pqWidgetsModule.h"

/**
* pqLineEditEventPlayer extends pqAbstractStringEventPlayer to ensure that
* pqLineEdit fires textChangedAndEditingFinished() signals in
* playback when "set_string" is handled.
*/
class PQWIDGETS_EXPORT pqLineEditEventPlayer : public pqAbstractStringEventPlayer
{
  Q_OBJECT
  typedef pqAbstractStringEventPlayer Superclass;

public:
  pqLineEditEventPlayer(QObject* parent = 0);
  ~pqLineEditEventPlayer() override;

  using Superclass::playEvent;
  bool playEvent(
    QObject* Object, const QString& Command, const QString& Arguments, bool& Error) override;

private:
  Q_DISABLE_COPY(pqLineEditEventPlayer)
};

#endif