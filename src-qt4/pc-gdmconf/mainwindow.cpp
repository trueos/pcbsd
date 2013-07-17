/**************************************************************************
*   Copyright (C) 2012 by Yuri Momotyuk                                   *
*   yurkis@gmail.com                                                      *
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

#include <QMessageBox>
#include <QTextStream>

const char* const GDM_CONFIG_FILE="/usr/local/etc/gdm/custom.conf";
const char* const DAEMON_GROUP= "daemon";
const char* const SECURITY_GROUP = "security";

const char* const TIMED_LOGIN_ENABLED = "TimedLoginEnable";
const char* const TIMED_LOGIN_ENABLED_DEF="false";

const char* const TIMED_LOGIN = "TimedLogin";

const char* const TIMED_LOGIN_DELAY = "TimedLoginDelay";
const char* const TIMED_LOGIN_DELAY_DEF = "30";

const char* const XDCMP_GROUP = "xdmcp";

const char* const ENABLE_XDCMP = "Enable";
const char* const ENABLE_XDCMP_DEF = "false";

const char* const DISPLAYS_PER_HOST = "DisplaysPerHost";
const char* const DISPLAYS_PER_HOST_DEF  = "1";

const char* const MAX_SESSIONS = "MaxSessions";
const char* const MAX_SESSIONS_DEF = "16";

const char* const XDCMP_PORT = "Port";
const char* const XDCMP_PORT_DEF = "177";

const char* const DISALLOW_TCP = "DisallowTCP";
const char* const DISALLOW_TCP_DEF = "true";

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
    //Open gdm config file
    if (!mConf.open(GDM_CONFIG_FILE))
    {
        QMessageBox msg;
        msg.setText(tr("Can not open GDM configuration file"));
        msg.exec();
        exit(1);
    }    

    getUsers();
    ui->UsersList->clear();
    for (int  i=0; i<mvUsers.size(); i++)
    {
        ui->UsersList->addItem(mvUsers[i]);
    }

    bool isAutoLoginEnabled = toBoolean(mConf.getKey(DAEMON_GROUP, TIMED_LOGIN_ENABLED, TIMED_LOGIN_ENABLED_DEF));
    ui->AutoLoginEnabledCB->setChecked(isAutoLoginEnabled);
    ui->UsersList->setEnabled(isAutoLoginEnabled);
    ui->AutoLoginDelay->setEnabled(isAutoLoginEnabled);

    int AutoLoginDelay = mConf.getKey(DAEMON_GROUP, TIMED_LOGIN_DELAY, TIMED_LOGIN_DELAY_DEF).toInt();
    ui->AutoLoginDelay->setValue(AutoLoginDelay);

    bool isEnableXDCMP = toBoolean(mConf.getKey(XDCMP_GROUP, ENABLE_XDCMP, ENABLE_XDCMP_DEF));
    ui->EnableXDCMP->setChecked(isEnableXDCMP);
    ui->XCDMPPort->setEnabled(isEnableXDCMP);
    ui->MaxConnections->setEnabled(isEnableXDCMP);
    ui->MaxSessions->setEnabled(isEnableXDCMP);

    int port = mConf.getKey(XDCMP_GROUP, XDCMP_PORT,  XDCMP_PORT_DEF).toInt();
    ui->XCDMPPort->setValue(port);

    int MaxConnections = mConf.getKey(XDCMP_GROUP, DISPLAYS_PER_HOST,  DISPLAYS_PER_HOST_DEF).toInt();
    ui->MaxConnections->setValue(MaxConnections);
    int MaxSessions = mConf.getKey(XDCMP_GROUP, MAX_SESSIONS,  MAX_SESSIONS_DEF).toInt();
    ui->MaxSessions->setValue(MaxSessions);

    ui->AllowTCP->setChecked(!toBoolean(mConf.getKey(SECURITY_GROUP, DISALLOW_TCP, DISALLOW_TCP_DEF)));

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
    mConf.setKey(DAEMON_GROUP, TIMED_LOGIN_ENABLED, fromBoolean(ui->AutoLoginEnabledCB->checkState()==Qt::Checked));
    mConf.setKey(DAEMON_GROUP, TIMED_LOGIN_DELAY, QString::number(ui->AutoLoginDelay->value()));
    mConf.setKey(DAEMON_GROUP, TIMED_LOGIN, ui->UsersList->currentText());

    mConf.setKey(XDCMP_GROUP, ENABLE_XDCMP, fromBoolean(ui->EnableXDCMP->checkState()==Qt::Checked));
    mConf.setKey(XDCMP_GROUP, XDCMP_PORT, QString::number(ui->XCDMPPort->value()));
    mConf.setKey(XDCMP_GROUP, DISPLAYS_PER_HOST, QString::number(ui->MaxConnections->value()));
    mConf.setKey(XDCMP_GROUP, MAX_SESSIONS, QString::number(ui->MaxSessions->value()));

    mConf.setKey(SECURITY_GROUP, DISALLOW_TCP, fromBoolean(ui->AllowTCP->checkState()!=Qt::Checked));

    mConf.save();
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_AutoLoginEnabledCB_clicked(bool checked)
{
    ui->UsersList->setEnabled(checked);
    ui->AutoLoginDelay->setEnabled(checked);
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_EnableXDCMP_clicked(bool checked)
{
    ui->XCDMPPort->setEnabled(checked);
    ui->MaxConnections->setEnabled(checked);
    ui->MaxSessions->setEnabled(checked);
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::slotSingleInstance()
{
   this->hide();
   this->showNormal();
   this->activateWindow();
   this->raise();
}
