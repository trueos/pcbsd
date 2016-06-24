/***************************************************************************
 *   Copyright (C) 2007, 2012 by Tim McCormick   *
 *   tim@pcbsd.org   *
 *                                                                         *
 *   Permission is hereby granted, free of charge, to any person obtaining *
 *   a copy of this software and associated documentation files (the       *
 *   "Software"), to deal in the Software without restriction, including   *
 *   without limitation the rights to use, copy, modify, merge, publish,   *
 *   distribute, sublicense, and/or sell copies of the Software, and to    *
 *   permit persons to whom the Software is furnished to do so, subject to *
 *   the following conditions:                                             *
 *                                                                         *
 *   The above copyright notice and this permission notice shall be        *
 *   included in all copies or substantial portions of the Software.       *
 *                                                                         *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       *
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    *
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. *
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR     *
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, *
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR *
 *   OTHER DEALINGS IN THE SOFTWARE.                                       *
 ***************************************************************************/

#include <qlayout.h>
#include <qtranslator.h>
#include <qtextcodec.h>
#include <unistd.h>
#include <sys/types.h>

#include "pcbsdusermanager.h"

void PCBSDUserManager::setupUMDialogs()
{
    layout = new QGridLayout(widgetUserContainer);
    changesMade = false;
    //Init GUI
    if (getuid() == 0) //Check for root
    {
       // Setup our buttons
       pushSave->setEnabled(false);
       connect(pushClose, SIGNAL(clicked()), this, SLOT(slotClose()));
       connect(pushSave, SIGNAL(clicked()), this, SLOT(slotSave()));

       //Setup all the classes
       back = new UserManagerBackend(wDir);
       advancedGui = new mainDlgCode();
       advancedGui->programInit(back, wDir);
       simpleGui = new SimpleDlgCode();
       simpleGui->programInit(back, wDir);
       //Now load the appropriate GUI
       settings = new QSettings("TrueOS", "UserManager", this);
       if(settings->value("basicview",true).toBool()){
	 switchToSimple();
       }else{
	 switchToAdvanced();
       }
       //Connect Signals/Slots
       connect(advancedGui, SIGNAL(changed( bool )), this, SLOT(configChanged()));
       connect(simpleGui, SIGNAL(changed()), this, SLOT(configChanged()));
       connect(simpleGui, SIGNAL(advancedView()), this, SLOT(switchToAdvanced()));
       connect(advancedGui, SIGNAL(simpleView()), this, SLOT(switchToSimple()));
    }
    else
    {
       qDebug("Error, needs to be run as root");
       exit(1);
    }
};


PCBSDUserManager::~PCBSDUserManager()
{
}

void PCBSDUserManager::switchToAdvanced()
{
    simpleGui->setParent(0);
    layout->addWidget(advancedGui);
    advancedGui->setParent(widgetUserContainer);
    advancedGui->show();
    settings->setValue("basicview",false);
}

void PCBSDUserManager::switchToSimple()
{
    advancedGui->setParent(0);
    simpleGui->setParent(widgetUserContainer);
    layout->addWidget(simpleGui);
    simpleGui->show();
    settings->setValue("basicview", true);
}

void PCBSDUserManager::configChanged()
{
  pushSave->setEnabled(true);
  changesMade = true;
}

void PCBSDUserManager::slotSave()
{
  if (back->commit())
  {
    pushSave->setEnabled(false);
    changesMade = false;
  }
}

void PCBSDUserManager::slotClose()
{
  if(changesMade)
  {
      int answer = QMessageBox::question(this,
                                         tr("Apply pending changes?"),
                                         tr("There are pending changes which have not been applied. Would you like to apply them before quitting?") ,
                                         QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
                                         QMessageBox::Cancel);
      if(answer == QMessageBox::Yes) back->commit();
      else if(answer == QMessageBox::Cancel) return;
  }
  close();
}

/**
  * Override QDialog's closeEvent() which is triggered when the window 'X' is hit and use our close slot
  */
void PCBSDUserManager::closeEvent(QCloseEvent*)
{
    slotClose();
}

void PCBSDUserManager::setWardenMode(QString d, QString ip)
{
  wDir = d;
  wIP = ip;
  setWindowTitle(tr("Editing Users for Jail:") + " " + wIP );
}
