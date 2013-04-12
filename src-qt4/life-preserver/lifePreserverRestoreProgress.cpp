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

/* Local includes */
#include "ui_lifePreserverRestoreProgress.h"
#include "lifePreserverRestoreProgress.h"
#include "preserver.h"
#include "externals.h"

/* KDE */


void lifeRestoreProgress::widgetInit(QString preserver, QString targetBackup, QString restoreFileListString)
{ 
   qDebug() << "Starting restore widget...";
   setWindowModality( Qt::WindowModal );

   restorePreserver = new PRESERVER(preserver);
   workingBackup = targetBackup;
   restoreList = restoreFileListString;
   
   connect(pushDone,SIGNAL(clicked()),this,SLOT(slotDone()));

   relKeepPath = false;

   viewRestoreOutput->setMaximumBlockCount(10000);
}

void lifeRestoreProgress::setRelativeRestore(QString relativeDirString, bool keepPath)
{
  relativeDir = relativeDirString;
  relKeepPath = keepPath; 
}

void lifeRestoreProgress::startRestore()
{
  QString prog = LIFEPRESERVERPATH + "/scripts/restore-rsync.sh";
  QStringList args;

  args << restorePreserver->getPreserverString() << workingBackup.simplified() << restoreList.simplified();

  // check if we need to enable relative restore
  if ( relativeDir.isEmpty() )
  {
    args << "/" << "ON";
  } else {
    if ( relKeepPath )
      args << relativeDir << "ON";
    else
      args << relativeDir << "OFF";
  }

  restoreProc = new QProcess(this);
  connect(restoreProc,SIGNAL(finished( int, QProcess::ExitStatus)),this,SLOT(slotProcFinished( int, QProcess::ExitStatus)));
  connect(restoreProc,SIGNAL(readyRead()),this,SLOT(slotReadProc()));
  restoreProc->setProcessChannelMode(QProcess::MergedChannels);
  restoreProc->start(prog, args);

}

void lifeRestoreProgress::slotReadProc()
{
   QString tmp;
   while ( restoreProc->canReadLine() )
   {
     tmp = restoreProc->readLine();
     viewRestoreOutput->insertPlainText(tmp);
     viewRestoreOutput->moveCursor(QTextCursor::End);
   }
}

void lifeRestoreProgress::slotProcFinished( int exit, QProcess::ExitStatus status)
{
  if ( status != QProcess::NormalExit || exit != 0)
  {
     QMessageBox::critical ( this, tr("Restore Failed"), tr("The restore failed! Please check your settings and try again!"), QMessageBox::Ok);
     labelRestoreStatus->setText(tr("Restore Failed"));
  } else {
     QMessageBox::information( this, tr("Restore Finished"), tr("The restore finished successfully!"), QMessageBox::Ok);
     labelRestoreStatus->setText(tr("Restore Finished"));
  }


  pushDone->setText(tr("Close"));
}

void lifeRestoreProgress::slotDone()
{
  if ( restoreProc->state() == QProcess::Running )
  {
    restoreProc->kill();
  }

  close();
}
