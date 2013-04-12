/***************************************************************************
 *   Copyright (C) 2006 - 2009 PC-BSD Software   *
 *   kris@pcbsd.com  *
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
#include "ui_lifePreserverWizard.h"
#include "lifePreserverWizard.h"
#include "preserver.h"
#include "externals.h"

/* KDE */


void lifePreserverWizard::widgetInit()
{ 
   qDebug() << "Starting wizard...";
   setWindowModality ( Qt::ApplicationModal );

   // Connect our slots for complete status checking
   connect(this,SIGNAL(currentIdChanged(int)),this,SLOT(slotCheckComplete()));
   connect(radioDisable,SIGNAL(clicked()),this,SLOT(slotCheckComplete()));
   connect(radioDaily,SIGNAL(clicked()),this,SLOT(slotCheckComplete()));
   connect(radioWeekly,SIGNAL(clicked()),this,SLOT(slotCheckComplete()));
   connect(lineHostName,SIGNAL(textChanged(const QString)),this,SLOT(slotCheckComplete()));
   connect(lineUserName,SIGNAL(textChanged(const QString)),this,SLOT(slotCheckComplete()));
}

// Logic checks to see if we are ready to move onto next page
bool lifePreserverWizard::validatePage()
{


  switch (currentId()) {
     case Page_Intro:
         button(QWizard::NextButton)->setEnabled(true);
         return true;
     case Page_RemoteDev:
         if ( lineHostName->text().isEmpty() ) {
           button(QWizard::NextButton)->setEnabled(false);
           return true;
         } 
         if ( lineUserName->text().isEmpty() ) {
           button(QWizard::NextButton)->setEnabled(false);
           return true;
         }
         // if we get this far, all the fields are filled in
         button(QWizard::NextButton)->setEnabled(true);
         return true;
     case Page_Finished:
         button(QWizard::FinishButton)->setEnabled(true);
         return true;
     default:
         return true;
  }

  return true;
}

void lifePreserverWizard::slotCheckComplete()
{
   // Validate this page
   validatePage();
}

void lifePreserverWizard::accept()
{
  if ( ! doesPreserverExist() )
  {
    setEnabled(false);
    QString tmpPort;
    tmpPort.setNum(spinPort->value());
    QString program = "xterm";
    QStringList arguments;
    arguments << "-e" << LIFEPRESERVERPATH + "/scripts/setup-ssh-keys.sh " \
    + lineUserName->text() + " " + lineHostName->text() + " " + tmpPort;

    qDebug() << program << arguments;
    QProcess *sshAuthProc = new QProcess(this);
    connect(sshAuthProc,SIGNAL(finished( int, QProcess::ExitStatus)),this,SLOT(slotSSHProcFinished( int, QProcess::ExitStatus)));
    sshAuthProc->start(program, arguments);
  } else {
    QMessageBox::critical ( this, tr("Duplicate Preserver"), tr("A preserver for this user + host already exists! Please select a different username / host when creating a new lifePreserver."), QMessageBox::Ok);
  }

}

void lifePreserverWizard::slotSSHProcFinished ( int exitCode, QProcess::ExitStatus exitStatus )
{
  if ( exitCode == 0 && exitStatus != QProcess::CrashExit && !QFile::exists("/tmp/.failedPreserver-" + lineUserName->text()))
  {
    // Save the new Preserver Job
    savePreserver(); 
    emit added();
    close();
  } else {
    if ( QFile::exists("/tmp/.failedPreserver-" + lineUserName->text()))
    {
       QFile::remove("/tmp/.failedPreserver-" + lineUserName->text());
    }
    QMessageBox::critical ( this, tr("Failed setting up SSH keys"), tr("Failed setting up the SSH keys for this server. Please click Back to check your settings and try again."), QMessageBox::Ok);
    setEnabled(true);
  }
}


// Function which saves the Preserver into the registry
void lifePreserverWizard::savePreserver()
{
   QString tmp, tmp2;

   QString connectString = lineUserName->text() + "@" + lineHostName->text();
   PRESERVER myPreserver(connectString);
   myPreserver.setHost(connectString);
   myPreserver.setPort(spinPort->value());
   myPreserver.setType("rsync");
   myPreserver.setNumBackups(7);
   myPreserver.setFailedCleanup(true);

   // Set the default remote directory
   myPreserver.setRemoteDir("life-preserver");

   if ( radioDaily->isChecked() )
     myPreserver.setCron("daily");

   if ( radioWeekly->isChecked() )
     myPreserver.setCron("weekly");

}

bool lifePreserverWizard::doesPreserverExist()
{  
   QString connectString = lineUserName->text() + "@" + lineHostName->text();

   // Create our new Preserver Directory
   QDir findPreserverDir;
   findPreserverDir.setPath(LIFEPRESERVERSDIR + "/" + connectString);
   if ( findPreserverDir.exists() )
   {
     return true;
   }

   return false;
}

int lifePreserverWizard::nextId() const
{
   switch (currentId()) {
     case Page_Intro:
         return Page_RemoteDev;
     case Page_RemoteDev:
         return Page_Schedule;
     case Page_Schedule:
         return Page_Finished;
     case Page_Finished:
         return -1;
     default:
         return -1;
     }
}
