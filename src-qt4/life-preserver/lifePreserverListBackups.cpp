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
#include <QMessageBox>
#include <QDebug>
#include <QTimer>

/* Local includes */
#include "ui_lifePreserverListBackups.h"
#include "lifePreserverListBackups.h"
#include "ui_lifePreserverRestore.h"
#include "lifePreserverRestore.h"
#include "preserver.h"
#include "externals.h"

/* KDE */


void lifeListBackups::widgetInit(QString preserver)
{ 
  qDebug() << "Starting listing...";
  setWindowModality( Qt::WindowModal );

  listPreserver = new PRESERVER(preserver);
   
  // Connect our slots for complete status checking
  connect(pushSelectBackup,SIGNAL(clicked()),this,SLOT(slotSelected()));
  connect(pushCancel,SIGNAL(clicked()),this,SLOT(slotCancel()));

  groupBackups->setEnabled(false);
  pushSelectBackup->setEnabled(false);

  // Start monitoring for changes
  QTimer::singleShot(500, this, SLOT(slotStartFetch() ) );
}

void lifeListBackups::slotStartFetch()
{ 
  QString backups = listPreserver->listServerBackups();
  if ( backups == "NONE" )
  {
    QMessageBox::critical ( this, tr("No Backups Found"), tr("No backups could be found on this server!"), QMessageBox::Ok);
  } else if ( backups == "FAILED" ) {
    QMessageBox::critical ( this, tr("Failed connecting to server"), tr("Failed connecting to the server! Check connection and try again!"), QMessageBox::Ok);
  } else {
    // We got here, list the backups and enable the buttons
    QStringList backupList = backups.split(":");
    for (int i = 0; i < backupList.size(); ++i)
          listBackups->addItem(backupList.at(i));
    listBackups->setCurrentRow(0);

    groupBackups->setEnabled(true);
    pushSelectBackup->setEnabled(true);
    labelStatus->setText(tr("The following backups are available:"));
  }
  
}

void lifeListBackups::slotSelected()
{ 
  if(! listBackups->currentItem()->text().isEmpty())
  {
     QString selBackup = listBackups->currentItem()->text();
     lifeRestore *lpr = new lifeRestore();
     lpr->widgetInit(listPreserver->getPreserverString(), selBackup);
     lpr->show();
     close();
  }

}

void lifeListBackups::slotCancel()
{ 
  close();
}
