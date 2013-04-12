/***************************************************************************
 *   Copyright (C) 2006 - 2009 PC-BSD Software   *
 *   kris@pcbsd.org  *
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
/* QT4 */
#include <QPixmap>
#include <QTextStream>
#include <QSettings>
#include <QDateTime>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QSettings>

/* Local includes */
#include "ui_lifePreserverSettings.h"
#include "lifePreserverSettings.h"
#include "preserver.h"
#include "externals.h"

/* KDE */


void pSettings::widgetInit(QString preserver)
{ 
   qDebug() << "Starting settings...";
   setWindowModality( Qt::WindowModal );

   editPreserver = new PRESERVER(preserver);
   
   // Connect our slots for complete status checking
   connect(pushOK,SIGNAL(clicked()),this,SLOT(slotOK()));
   connect(pushCancel,SIGNAL(clicked()),this,SLOT(slotCancel()));
   connect(pushExcludeList,SIGNAL(clicked()),this,SLOT(slotModifyExcludes()));
   connect(pushIncludeList,SIGNAL(clicked()),this,SLOT(slotModifyIncludes()));
   connect(radioDisable,SIGNAL(clicked()),this,SLOT(slotRadioChanged()));
   connect(radioDaily,SIGNAL(clicked()),this,SLOT(slotRadioChanged()));
   connect(radioWeekly,SIGNAL(clicked()),this,SLOT(slotRadioChanged()));

   // Set our values for this preserver
   spinNumBackups->setValue(editPreserver->getNumBackups());
   checkCleanupFailed->setChecked(editPreserver->getFailedCleanup());
   lineRemoteDir->setText(editPreserver->getRemoteDir());
   QString cron = editPreserver->getCronSettings();
   if ( cron == "disabled" )
     radioDisable->setChecked(true);
   else if ( cron == "daily" )
       radioDaily->setChecked(true);  
   else if ( cron == "weekly" )
       radioWeekly->setChecked(true);  
  
   // Set our radio enable / disables properly
   slotRadioChanged(); 

}

// Open the exclude dialog for this preserver
void pSettings::slotModifyExcludes()
{
  pUserExclude *editExclude = new pUserExclude();
  editExclude->widgetInit(editPreserver->getHost(), true);
  editExclude->show();
}

// Open the include dialog for this preserver
void pSettings::slotModifyIncludes()
{
  pUserExclude *editExclude = new pUserExclude();
  editExclude->widgetInit(editPreserver->getHost(), false);
  editExclude->show();
}

// Save the config to disk
void pSettings::savePreserver()
{
   QString tmp, tmp2;

   editPreserver->setNumBackups(spinNumBackups->value());
   editPreserver->setFailedCleanup(checkCleanupFailed->isChecked());
   editPreserver->setRemoteDir(lineRemoteDir->text());

   // Check if we've disabled cron
   if ( radioDisable->isChecked() )
      editPreserver->setCron("disabled");

   // Enable daily backups
   if ( radioDaily->isChecked() )
     editPreserver->setCron("daily");

   // Enable weekly backups
   if ( radioWeekly->isChecked() )
     editPreserver->setCron("weekly");
}

void pSettings::slotOK()
{
  savePreserver();
  close();
}

void pSettings::slotCancel()
{
  close();
}

void pSettings::slotRadioChanged()
{
   // Check if we need to enable / disable any stuff relating to automated backup selection
}
