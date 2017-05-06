/***************************************************************************
 *   Copyright (C) 2006 - 2009 TrueOS Software   *
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
#include "ui_progress.h"
#include "progress.h"


void progressUI::startServ(QString cmd, QStringList args)
{
  qDebug() << "Starting restore widget...";
  setWindowModality( Qt::WindowModal );
   
  connect(pushDone,SIGNAL(clicked()),this,SLOT(slotDone()));
  viewRestoreOutput->setMaximumBlockCount(10000);

  servProc = new QProcess(this);
  connect(servProc,SIGNAL(finished( int, QProcess::ExitStatus)),this,SLOT(slotProcFinished( int, QProcess::ExitStatus)));
  connect(servProc,SIGNAL(readyRead()),this,SLOT(slotReadProc()));
  servProc->setProcessChannelMode(QProcess::MergedChannels);
  servProc->start(cmd, args);
}

void progressUI::slotReadProc()
{
   QString tmp;
   while ( servProc->canReadLine() )
   {
     tmp = servProc->readLine();
     viewRestoreOutput->insertPlainText(tmp);
     viewRestoreOutput->moveCursor(QTextCursor::End);
   }
}

void progressUI::slotProcFinished( int exit, QProcess::ExitStatus status)
{
  if ( status != QProcess::NormalExit || exit != 0)
  {
     QMessageBox::critical ( this, tr("Failed"), tr("The specified command failed."), QMessageBox::Ok);
     labelRestoreStatus->setText(tr("Failed"));
  } else {
     labelRestoreStatus->setText(tr("Finished"));
  }

  pushDone->setText(tr("Close"));
}

void progressUI::slotDone()
{
  if ( servProc->state() == QProcess::Running )
  {
    servProc->kill();
  }

  close();
}
