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
/*
typedef struct _SComponentsGroup
{
    QString           groupName;
    const SComponent* pComponent;
    int               componentsCount;
}SComponentsGroup;
*/
const SComponent DEComponents[]={
    {"KDE applications", "kde-baseapps"},
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

const SComponent LibComponents[]={
    {"Qt 4.x", "qt4-corelib"},
    {"Qt 5.x", "qt5-core"},
    {"Gtk 2.x", "gtk2"},
    {"Gtk 3.x", "gtk3"},
    {"EFL", "efl"},
};

const SComponent OfficeComponents[]={
    {"Open Office", "openoffice-4"},
    {"Open Office (dev)", "openoffice-devel"},
    {"Libre Office", "libreoffice"},
    {"Calligra", "calligra"},
};

const SComponent VideoComponents[]={
    {"Xorg", "xorg"},
    {"NVidia", "nvidia-driver"},
    {"NVidia 173x driver", "nvidia-driver-173"},
    {"NVidia 304x driver", "nvidia-driver-304"},
    {"NVidia 340x driver", "nvidia-driver-340"},
    {"Intel driver", "xf86-video-intel"},
    {"AMD/ATI driver", "xf86-video-ati"},
    {"VirtualBox driver", "virtualbox-ose-additions"},
    {"VMWare driver", "xf86-video-vmware"},
};

const SComponent ScriptComponents[]={
    {"Perl", "perl5"},
    {"Python 2.x", "python2"},
    {"Python 3.x", "python3"},
};

const SComponent EmulatorsComponents[]=
{
    {"Wine", "wine"},
    {"Wine i386", "i386-wine"},
    {"Wine i386 (dev)", "i386-wine-devel"},
};

const SComponent PCBSDComponents[]=
{
    {"CLI utils", "pcbsd-utils"},
    {"UI utils", "pcbsd-utils-qt5"},
};


////////////////////////////////////////////////////////////////////////////
MainWindow::MainWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainWindow)
{
    pkginfo = 0;
    ui->setupUi(this);

    QString Arch;

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
bool MainWindow::checkNGPackage(QString name, QString &outVer)
{
    outVer="";
    QString cmd = QString("pkg query %v ") + name;
    QStringList out = pcbsd::Utils::runShellCommand(cmd);
    if (!out.size())
        return false;    
    outVer = out[0];
    return ( outVer.length() > 0 );
}

////////////////////////////////////////////////////////////////////////////
void MainWindow::addComponentItem(QString name, QString version, QTreeWidgetItem *parrent)
{
    QTreeWidgetItem* item = new QTreeWidgetItem(parrent);
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    item->setText(0, name);
    item->setText(1, version);

    parrent->addChild(item);
}

////////////////////////////////////////////////////////////////////////////
void MainWindow::addComponents(QString groupName, const SComponent* components, int componentsSize)
{
    QTreeWidgetItem* item = new QTreeWidgetItem(ui->ComponentsList);
    item->setText(0, groupName);
    QFont font = item->font(0);
    font.setBold(true);
    item->setFont(0, font);
    for(int i=0; i<componentsSize; i++){
        QString ver;
        if (checkNGPackage(components[i].Prefix, ver))
        {
            addComponentItem(components[i].Name, ver, item);
        }

    }
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
void MainWindow::on_BackButton_clicked()
{
    ui->MainStack->setCurrentIndex(0);
}

////////////////////////////////////////////////////////////////////////////
void MainWindow::slotPackagesFinished()
{
    ui->WaitTextLabel->setVisible(false);
}

#define FILL_GROUP(group_name, group_array)\
    addComponents(tr(group_name),&group_array[0], sizeof(group_array)/sizeof(SComponent));

////////////////////////////////////////////////////////////////////////////
void MainWindow::on_pushButton_clicked()
{
    ui->ComponentsList->clear();

    ui->WaitTextLabel->setVisible(false);

    FILL_GROUP("PCBSD components",PCBSDComponents);
    FILL_GROUP("Video stack", VideoComponents);
    FILL_GROUP("Toolkits", LibComponents);
    FILL_GROUP("Desktops",DEComponents);
    FILL_GROUP("Office",OfficeComponents);
    FILL_GROUP("Scripting languages",ScriptComponents);

    ui->MainStack->setCurrentIndex(1);

    int h_width = ui->ComponentsList->header()->geometry().width() * 0.65;
    ui->ComponentsList->header()->resizeSection(0, h_width);

}


