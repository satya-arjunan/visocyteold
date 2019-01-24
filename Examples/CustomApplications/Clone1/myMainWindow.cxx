/*=========================================================================

   Program: Visocyte
   Module:    myMainWindow.cxx

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
#include "myMainWindow.h"
#include "ui_myMainWindow.h"

#ifdef PARAVIEW_USE_QTHELP
#include "pqHelpReaction.h"
#endif
#include "pqVisocyteBehaviors.h"
#include "pqVisocyteMenuBuilders.h"

class myMainWindow::pqInternals : public Ui::pqClientMainWindow
{
};

//-----------------------------------------------------------------------------
myMainWindow::myMainWindow()
{
  this->Internals = new pqInternals();
  this->Internals->setupUi(this);

  // Setup default GUI layout.

  // Set up the dock window corners to give the vertical docks more room.
  this->setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
  this->setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);

  this->Internals->animationViewDock->hide();
  this->Internals->statisticsDock->hide();
  this->Internals->comparativePanelDock->hide();
  this->tabifyDockWidget(this->Internals->animationViewDock, this->Internals->statisticsDock);

  // Enable help from the properties panel.
  QObject::connect(this->Internals->proxyTabWidget,
    SIGNAL(helpRequested(const QString&, const QString&)), this,
    SLOT(showHelpForProxy(const QString&, const QString&)));

  // Populate application menus with actions.
  pqVisocyteMenuBuilders::buildFileMenu(*this->Internals->menu_File);
  pqVisocyteMenuBuilders::buildEditMenu(*this->Internals->menu_Edit);

  // Populate sources menu.
  pqVisocyteMenuBuilders::buildSourcesMenu(*this->Internals->menuSources, this);

  // Populate filters menu.
  pqVisocyteMenuBuilders::buildFiltersMenu(*this->Internals->menuFilters, this);

  // Populate Tools menu.
  pqVisocyteMenuBuilders::buildToolsMenu(*this->Internals->menuTools);

  // setup the context menu for the pipeline browser.
  pqVisocyteMenuBuilders::buildPipelineBrowserContextMenu(
    *this->Internals->pipelineBrowser->contextMenu());

  pqVisocyteMenuBuilders::buildToolbars(*this);

  // Setup the View menu. This must be setup after all toolbars and dockwidgets
  // have been created.
  pqVisocyteMenuBuilders::buildViewMenu(*this->Internals->menu_View, *this);

  // Setup the menu to show macros.
  pqVisocyteMenuBuilders::buildMacrosMenu(*this->Internals->menu_Macros);

  // Setup the help menu.
  pqVisocyteMenuBuilders::buildHelpMenu(*this->Internals->menu_Help);

  // Final step, define application behaviors. Since we want all Visocyte
  // behaviors, we use this convenience method.
  new pqVisocyteBehaviors(this, this);
}

//-----------------------------------------------------------------------------
myMainWindow::~myMainWindow()
{
  delete this->Internals;
}

//-----------------------------------------------------------------------------
void myMainWindow::showHelpForProxy(const QString& groupname, const QString& proxyname)
{
#ifdef PARAVIEW_USE_QTHELP
  pqHelpReaction::showProxyHelp(groupname, proxyname);
#endif
}
