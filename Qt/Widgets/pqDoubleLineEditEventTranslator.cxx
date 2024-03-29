/*=========================================================================

   Program: Visocyte
   Module:  pqDoubleLineEditEventTranslator.h

   Copyright (c) 2005-2018 Sandia Corporation, Kitware Inc.
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

// Qt Includes.
#include <QComboBox>
#include <QDebug>
#include <QEvent>

// Visocyte Includes.
#include "pqDoubleLineEdit.h"
#include "pqDoubleLineEditEventPlayer.h"
#include "pqDoubleLineEditEventTranslator.h"

// ----------------------------------------------------------------------------
pqDoubleLineEditEventTranslator::pqDoubleLineEditEventTranslator(QObject* parent)
  : pqWidgetEventTranslator(parent)
{
  this->CurrentObject = 0;
}

//-----------------------------------------------------------------------------
pqDoubleLineEditEventTranslator::~pqDoubleLineEditEventTranslator()
{
}

// ----------------------------------------------------------------------------
bool pqDoubleLineEditEventTranslator::translateEvent(QObject* object, QEvent* event, bool& error)
{
  Q_UNUSED(error);
  pqDoubleLineEdit* doubleLineEdit = NULL;
  for (QObject* test = object; doubleLineEdit == NULL && test != NULL; test = test->parent())
  {
    doubleLineEdit = qobject_cast<pqDoubleLineEdit*>(test);
  }

  if (!doubleLineEdit)
  {
    return false;
  }

  if (event->type() == QEvent::Enter && object == doubleLineEdit)
  {
    if (this->CurrentObject != object)
    {
      if (this->CurrentObject)
      {
        disconnect(this->CurrentObject, 0, this, 0);
      }
      this->CurrentObject = object;

      connect(doubleLineEdit, SIGNAL(destroyed(QObject*)), this, SLOT(onDestroyed()));
      connect(doubleLineEdit, SIGNAL(fullPrecisionTextChanged(const QString&)), this,
        SLOT(onFullPrecisionTextChangedAndEditingFinished(const QString&)));
    }
  }

  return true;
}

// ----------------------------------------------------------------------------
void pqDoubleLineEditEventTranslator::onDestroyed()
{
  this->CurrentObject = 0;
}

// ----------------------------------------------------------------------------
void pqDoubleLineEditEventTranslator::onFullPrecisionTextChangedAndEditingFinished(
  const QString& text)
{
  emit this->recordEvent(this->CurrentObject, pqDoubleLineEditEventPlayer::EVENT_NAME(), text);
}
