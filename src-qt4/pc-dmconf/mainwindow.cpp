/**************************************************************************
*   Copyright (C) 2012 by Yuri Momotyuk                                   *
*   yurkis@gmail.com                                                      *
*   Copyright (C) 2013 by Kris Moore                                      *
*   kris@pcbsd.org                                                        *
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
*   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*
*   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR     *
*   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, *
*   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR *
*   OTHER DEALINGS IN THE SOFTWARE.                                       *
***************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pcbsd-utils.h"

#include <QInputDialog>
#include <QMessageBox>
#include <QTextStream>

#define DM_CONFIG_FILE QString("/usr/local/etc/pcdm.conf")

///////////////////////////////////////////////////////////////////////////////
inline bool toBoolean(QString Str, bool Default=false)
{
    if (Str.trimmed().toLower() == "true")
        return true;
    if (Str.trimmed().toLower() == "false")
        return false;
    return Default;
}

///////////////////////////////////////////////////////////////////////////////
inline QString fromBoolean(bool flag)
{
    return (flag)?QString("true"):QString("false");
}

///////////////////////////////////////////////////////////////////////////////
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initUI();
}

///////////////////////////////////////////////////////////////////////////////
MainWindow::~MainWindow()
{
    delete ui;
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::initUI()
{
    //Make sure the conf file exists
    if(!QFile::exists(DM_CONFIG_FILE)){
      qDebug() << "Copying over the default configuration file:" << DM_CONFIG_FILE;
      if( !QFile::copy(DM_CONFIG_FILE+".dist", DM_CONFIG_FILE) ){
	QMessageBox::warning(this,tr("Missing Config File"), QString(tr("The PCDM configuration file could not be found: %1")).arg(DM_CONFIG_FILE)+"\n\n"+tr("This application will now close"));
	exit(1);
      }
    }
    getUsers();
    ui->UsersList->clear();
    QString autoLogUser = pcbsd::Utils::getValFromSHFile(DM_CONFIG_FILE, "AUTO_LOGIN_USER");
    for (int  i=0; i<mvUsers.size(); i++)
    {
        ui->UsersList->addItem(mvUsers[i]);
	if ( autoLogUser == mvUsers[i] )
          ui->UsersList->setCurrentIndex(i);
    }

    QString autoLog = pcbsd::Utils::getValFromSHFile(DM_CONFIG_FILE, "ENABLE_AUTO_LOGIN");
    if ( autoLog == "TRUE" ) {
      ui->AutoLoginEnabledCB->setChecked(true);
      ui->UsersList->setEnabled(true);
    } else {
      ui->AutoLoginEnabledCB->setChecked(false);
      ui->UsersList->setEnabled(false);
    }

    ui->EnableVNC->setChecked(false);
    QString vnc = pcbsd::Utils::getValFromSHFile(DM_CONFIG_FILE, "ALLOW_REMOTE_LOGIN");
    if ( vnc == "TRUE" )
      ui->EnableVNC->setChecked(true);

    ui->checkShowPW->setChecked(false);
    QString showpw = pcbsd::Utils::getValFromSHFile(DM_CONFIG_FILE, "ENABLE_VIEW_PASSWORD_BUTTON");
    if ( showpw == "TRUE" )
      ui->checkShowPW->setChecked(true);

    ui->SaveButton->setEnabled(false);
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_UsersList_currentIndexChanged(int item) {
    Q_UNUSED(item);
    ui->SaveButton->setEnabled(true);
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::getUsers()
{
    /*
        here is modifyed code from pc-usermanager
    */
    mvUsers.clear();
    QFile userFile("/etc/passwd");
    if ( userFile.open(QIODevice::ReadOnly) ) {
        QTextStream stream(&userFile);
        stream.setCodec("UTF-8");
        QString line;

        while ( !stream.atEnd() ) {
            line = stream.readLine();

            if ((line.trimmed().indexOf("#") != 0) && (! line.isEmpty())) { //Make sure it isn't a comment or blank
                QString username = line.section(":",0,0);
                QString homedir = line.section(":",5,5);

		// Ignore PEFS encrypted users
		if ( QFile::exists(homedir + "/.pefs.db") )
		  continue;

                int uid = line.section(":",2,2).toInt();
                if ((uid>1000)&&(uid<65534))
                    mvUsers.push_back(username);
            }//if gout user
        }//for all lines
    }//if can open file
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_SaveButton_clicked()
{
    bool ok;
    system("touch " + DM_CONFIG_FILE.toLatin1());
    if ( ui->AutoLoginEnabledCB->isChecked() ) {
       // First ask for password
       QString pw = QInputDialog::getText(this, tr("Password Request"),
                        tr("Please enter the login password for this user"), 
                        QLineEdit::Password, "", &ok);
       if ( ! ok )
          return;
      
       pcbsd::Utils::setConfFileValue(DM_CONFIG_FILE, "ENABLE_AUTO_LOGIN", "ENABLE_AUTO_LOGIN=TRUE", -1);
       pcbsd::Utils::setConfFileValue(DM_CONFIG_FILE, "AUTO_LOGIN_USER", "AUTO_LOGIN_USER=" + ui->UsersList->currentText(), -1);
       pcbsd::Utils::setConfFileValue(DM_CONFIG_FILE, "AUTO_LOGIN_PASSWORD", "AUTO_LOGIN_PASSWORD=" + pw, -1);
    } else {
       pcbsd::Utils::setConfFileValue(DM_CONFIG_FILE, "ENABLE_AUTO_LOGIN", "ENABLE_AUTO_LOGIN=FALSE", -1);
       pcbsd::Utils::setConfFileValue(DM_CONFIG_FILE, "AUTO_LOGIN_USER", "", -1);
       pcbsd::Utils::setConfFileValue(DM_CONFIG_FILE, "AUTO_LOGIN_PASSWORD", "", -1);
    }

    if ( ui->EnableVNC->isChecked() ) {
       // First ask for password
       QString pw = QInputDialog::getText(this, tr("Password Request"),
                        tr("Please enter the remote login password"), 
                        QLineEdit::Password, "", &ok);
       if ( ! ok )
          return;

       QFile file("/usr/local/etc/vncpass");
       if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
          return;

       QTextStream out(&file);
       out << pw;
       file.close();
       system("chmod 600 /usr/local/etc/vncpass");

       pcbsd::Utils::setConfFileValue(DM_CONFIG_FILE, "ALLOW_REMOTE_LOGIN", "ALLOW_REMOTE_LOGIN=TRUE", -1);
    } else {
       pcbsd::Utils::setConfFileValue(DM_CONFIG_FILE, "ALLOW_REMOTE_LOGIN", "ALLOW_REMOTE_LOGIN=FALSE", -1);
       system("rm /usr/local/etc/vncpass 2>/dev/null");
    }

    if ( ui->checkShowPW->isChecked() )
       pcbsd::Utils::setConfFileValue(DM_CONFIG_FILE, "ENABLE_VIEW_PASSWORD_BUTTON", "ENABLE_VIEW_PASSWORD_BUTTON=TRUE", -1);
    else
       pcbsd::Utils::setConfFileValue(DM_CONFIG_FILE, "ENABLE_VIEW_PASSWORD_BUTTON", "ENABLE_VIEW_PASSWORD_BUTTON=FALSE", -1);

    // Lastly make sure we set perms
    system("chmod 600 " + DM_CONFIG_FILE.toLatin1());
    ui->SaveButton->setEnabled(false);
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_AutoLoginEnabledCB_clicked(bool checked)
{
    ui->UsersList->setEnabled(checked);
    ui->SaveButton->setEnabled(true);
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_EnableVNC_clicked(bool checked)
{
  Q_UNUSED(checked);
  ui->SaveButton->setEnabled(true);
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_checkShowPW_clicked(bool checked)
{
  Q_UNUSED(checked);
  ui->SaveButton->setEnabled(true);
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::slotSingleInstance()
{
   this->hide();
   this->showNormal();
   this->activateWindow();
   this->raise();
}
