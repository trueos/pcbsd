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
#include "ui_lifePreserverRestore.h"
#include "lifePreserverRestore.h"
#include "ui_lifePreserverRestoreProgress.h"
#include "lifePreserverRestoreProgress.h"
#include "preserver.h"
#include "externals.h"

/* KDE */


void lifeRestore::widgetInit(QString preserver, QString targetBackup)
{ 
   qDebug() << "Starting restore widget...";
   setWindowModality( Qt::WindowModal );

   restorePreserver = new PRESERVER(preserver);
   workingBackup = targetBackup;
   
   connect(pushRestore,SIGNAL(clicked()),this,SLOT(slotRestore()));
   connect(pushCancel,SIGNAL(clicked()),this,SLOT(slotCancel()));
   connect(checkRelativeRestore,SIGNAL(clicked()),this,SLOT(slotRelativeChanged()));
   connect(pushSelDir,SIGNAL(clicked()),this,SLOT(slotSelRelDir()));

   // Connect our slots to check if we are ready to do a restore
   connect(lineRestoreList,SIGNAL(textChanged ( const QString & )),this,SLOT(slotCheckReady() ) );
   connect(lineRelRestore,SIGNAL(textChanged ( const QString & )),this,SLOT(slotCheckReady()));
   connect(checkRelativeRestore,SIGNAL(clicked()),this,SLOT(slotCheckReady()));

   pushRestore->setEnabled(false);

   // Set our radio enable / disables properly
   slotRelativeChanged(); 

}

void lifeRestore::slotCheckReady()
{
  bool ready;
  ready = true;

  if ( lineRestoreList->text().isEmpty() )
  {
    ready = false;
  } 

  if ( checkRelativeRestore->isChecked() && lineRelRestore->text().isEmpty())
  {
    ready = false; 
  }

  pushRestore->setEnabled(ready);
}

void lifeRestore::slotRestore()
{
  // Start our restore
  lifeRestoreProgress *lprp = new lifeRestoreProgress();
  lprp->widgetInit(restorePreserver->getPreserverString(), workingBackup, lineRestoreList->text());
  if ( checkRelativeRestore->isChecked() )
    lprp->setRelativeRestore(lineRelRestore->text(), checkPreservePath->isChecked());
  lprp->startRestore();
  lprp->show();

  close();
}

void lifeRestore::slotCancel()
{
  close();
}

void lifeRestore::slotSelRelDir()
{
  QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 "/home",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);

  if ( ! dir.isEmpty() )
  {
    lineRelRestore->setText(dir);
  } 

}

void lifeRestore::slotStartRestore()
{
}

void lifeRestore::slotRelativeChanged()
{
   // Check if we need to enable / disable any stuff relating to relative restores
   if ( checkRelativeRestore->isChecked() )
   {
       groupRelative->setEnabled(true);
   } else {
       groupRelative->setEnabled(false);
   }
}
