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
#include "../config.h"

#include <QProcess>
#include <QStringList>
#include <QDate>
#include <pcbsd-utils.h>

#include <iostream>

typedef struct _SComponent
{
    QString Name;
    QString Prefix;
}SComponent;

const SComponent DEComponents[] ={
    {"KDE", "kde-runtime-"},
    {"Gnome", "gnome2-lite-"},
    {"LXDE", "lxde-meta-"},
    {"XFCE", "xfce-"}
};

const SComponent DEComponentsNG[]={
    {"KDE applicarions", "kde-baseapps"},
    {"KDE plasma", "kde-workspace"},
    {"Gnome", "gnome3"},
    {"Cinnamon", "cinnamon"},
    {"Mate", "mate-desktop"},
    {"LXDE", "lxde-meta"},
    {"XFCE", "xfce"},
    {"Enlightenment", "enlightenment"},
    {"Awesome", "awesome"},
    {"FVWM", "fvwm2"},
    {"Lumina", "lumina"},
    {"Fluxbox", "fluxbox"},
    {"FVWM", "fvwm"},
    {"I3", "i3"},
    {"IceWM", "icewm"},
    {"Openbox", "openbox"},
    {"RatPoison", "ratpoison"},
    {"SpectrWM", "spectrwm"},
    {"WindowLab", "windowlab"},
    {"WindowMaker", "windowmaker"}
    
    
};

const int DEComponents_size = sizeof(DEComponents) / sizeof(SComponent);
const int DEComponentsNG_size = sizeof(DEComponentsNG) / sizeof(SComponent);

const char* const PCBSD_UTILS_PACKAGE = "pcbsd-utils";
const char* const PCBSD_UTILS_QT_PACKAGE = "pcbsd-utils-qt5";

////////////////////////////////////////////////////////////////////////////
MainWindow::MainWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainWindow)
{
    pkginfo = 0;
    ui->setupUi(this);

    QString Arch;
    QString Ver;

    //uname
    QStringList unameParsed = QString(pcbsd::Utils::runShellCommand("uname -imnr").at(0)).split(" ", QString::SkipEmptyParts);
    if (unameParsed.size()== 4)
    {
        ui->HostnameLabel->setText(unameParsed.at(0));
        ui->FreeBSDVerLabel->setText(pcbsd::Utils::runShellCommand("freebsd-version").join(""));
        Arch = unameParsed.at(2);
        ui->ArchLabel->setText(unameParsed.at(2));
        ui->IdentLabel->setText(unameParsed.at(3));
    }

    // pcbsd version
    ui->VersionLabel->setText(PCBSDVERSION);

    // Utils version
    ui->UtilsLabel->setText(pcbsd::Utils::runShellCommand("pkg query '%v' pcbsd-utils-qt5").at(0).section("'", 1, 1));

    QString PkgSet="PRODUCTION";
    PkgSet = pcbsd::Utils::getValFromPCBSDConf("PACKAGE_SET");
    if (PkgSet.toUpper().trimmed() == "ENTERPRISE")
    {
        PkgSet="ENTERPRISE";
        PkgSet+=QString(" ") + tr("(LTS)");
    }
    if (PkgSet.toUpper().trimmed() == "EDGE")
    {
        PkgSet="EDGE";
        PkgSet+=QString(" ") + tr("(unstable)");
    }
    if (PkgSet.toUpper().trimmed() == "CUSTOM")
    {
        PkgSet="CUSTOM";
        QString PkgUrl;
        PkgUrl = pcbsd::Utils::getValFromPCBSDConf("PACKAGE_URL");
        ui->PkgSetLabel->setToolTip(PkgUrl);
    }
    ui->PkgSetLabel->setText(PkgSet);


    // CPU
    ui->CPUInfoLabel->setText(pcbsd::Utils::sysctl("hw.model"));

    // memory
    ui->MemoryLabel->setText(pcbsd::Utils::bytesToHumanReadable(pcbsd::Utils::sysctlAsInt("hw.physmem")));
    //this->setFixedSize(this->geometry().width(),this->geometry().height());
}

////////////////////////////////////////////////////////////////////////////
MainWindow::~MainWindow()
{
    delete ui;
}

////////////////////////////////////////////////////////////////////////////
bool MainWindow::checkNG()
{
    return QFile::exists("/usr/local/sbin/pkg2ng");
}

////////////////////////////////////////////////////////////////////////////
bool MainWindow::checkNGPackage(QString name, QString &outVer)
{
    outVer="";
    QStringList out = pcbsd::Utils::runShellCommand(QString("pkg info -f ") + name);
    if (!out.size())
        return false;
    for (int i=0; i<out.size(); i++)
    {
        if (out[i].contains("Version"))
        {
            outVer = out[i];
            outVer.replace("Version        : ","");            
            break;
        }
    }
    return ( outVer.length() > 0 );
}

////////////////////////////////////////////////////////////////////////////
void MainWindow::addDEItem(QString name, QString version)
{
    QTreeWidgetItem* item = new QTreeWidgetItem(ui->DEList);
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    QFont font = item->font(0);
    font.setBold(true);
    item->setFont(0, font);
    item->setText(0, name);
    item->setText(1, version);

    ui->DEList->addTopLevelItem(item);
}

////////////////////////////////////////////////////////////////////////////
void MainWindow::slotSingleInstance()
{
    this->hide();
    this->showNormal();
    this->activateWindow();
    this->raise();
}

////////////////////////////////////////////////////////////////////////////
void MainWindow::on_pushButton_2_clicked()
{
    //X.org version
    QStringList xorgver = pcbsd::Utils::runShellCommand("Xorg -version");
    for(int i=0; i<xorgver.size(); i++)
    {
        QString str = xorgver.at(i);
        std::cout<<str.toStdString();
        if ( str.indexOf("X.Org X Server ") == 0)
        {
            ui->XorgLabel->setText(str.replace("X.Org X Server ",""));
            break;
        }
    }

    if (checkNG())
    {
        QString ver;
        if (checkNGPackage(PCBSD_UTILS_PACKAGE, ver))
        {
            ui->pcbsd_utils->setText(QString("r") + ver);
        }
        if (checkNGPackage(PCBSD_UTILS_QT_PACKAGE, ver))
        {
            ui->pcbsd_utils_qt->setText(QString("r") + ver);
        }
    }
    ui->MainStack->setCurrentIndex(2);
}

////////////////////////////////////////////////////////////////////////////
void MainWindow::on_BackButton_clicked()
{
    ui->MainStack->setCurrentIndex(0);
}

////////////////////////////////////////////////////////////////////////////
void MainWindow::slotReadPkg()
{
    QString line;
    while(pkginfo->canReadLine() )
    {
        line = pkginfo->readLine().simplified();
        //std::cout<<line.toStdString()<<"\n";
        for(int i=0 ; i<DEComponents_size; i++)
        {
            if (0 == line.indexOf(DEComponents[i].Prefix, Qt::CaseInsensitive))
            {
                line = line.replace(DEComponents[i].Prefix, "");
                line= line.left(line.indexOf(" "));
                addDEItem(DEComponents[i].Name, line);
            }//if got DE
        }//for all DEs
    }// while can read line
}

////////////////////////////////////////////////////////////////////////////
void MainWindow::slotPackagesFinished()
{
    ui->WaitTextLabel->setVisible(false);
}

////////////////////////////////////////////////////////////////////////////
void MainWindow::on_pushButton_clicked()
{
    ui->DEList->clear();

    if (!checkNG())
    {
        pkginfo = new QProcess(this);
        pkginfo->setProcessChannelMode(QProcess::MergedChannels);
        connect( pkginfo, SIGNAL(readyRead()), this, SLOT(slotReadPkg() ) );
        connect( pkginfo, SIGNAL(finished ( int, QProcess::ExitStatus)), this, SLOT(slotPackagesFinished()));
        pkginfo->start("pkg_info");
        ui->WaitTextLabel->setVisible(true);
    }
    else
    {
        ui->WaitTextLabel->setVisible(false);
        for(int i=0 ; i<DEComponentsNG_size; i++)
        {
            QString ver;
            if (checkNGPackage(DEComponentsNG[i].Prefix, ver))
            {
                addDEItem(DEComponentsNG[i].Name, ver);
            }
        }//for all DEs
    }//if pkgng installed

    ui->MainStack->setCurrentIndex(1);
}

void MainWindow::on_BackButton_2_clicked()
{
    ui->MainStack->setCurrentIndex(0);
}
