/**************************************************************************
*   Copyright (C) 2011 by Yuri Momotyuk                                   *
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

#include <QDebug>
#include "mainwnd.h"
#include "ui_mainwnd.h"
#include "deinfo.h"
#include "../config.h"

#include <QFile>
#include <iostream>


#define ITEMS_PREFIX   PREFIX + "/share/pcbsd/pc-controlpanel/items/"
#define SOFTWARE_DIR   ITEMS_PREFIX + "software"
#define SYSTEM_DIR     ITEMS_PREFIX + "system"
#define HARDWARE_DIR   ITEMS_PREFIX + "hardware"
#define NETWORKING_DIR ITEMS_PREFIX + "network"
#define TOOLS_DIR      ITEMS_PREFIX + "tools"
#define DE_DIR         ITEMS_PREFIX + "de"

#define PBI_INSTALLED_DIRECTORY "/var/db/pbi/installed"

static QString DETEXT;

///////////////////////////////////////////////////////////////////////////////
MainWnd::MainWnd(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWnd)
{
    ui->setupUi(this);

    misWheelGroup= checkUserGroup("wheel");
    misOperatorGroup = checkUserGroup("operator");

    setupGroups();

    InstalledDEList.refresh();
    if (InstalledDEList.active())
    {
        mCurrentDE = *InstalledDEList.active();
        mvEnabledDE.push_back(mCurrentDE.mName);
    }    

    DETEXT = tr("Desktop environment");

    DEChoiseMenu = new QMenu("", this);
    setupDEChooser();
    fillGroups();

}

///////////////////////////////////////////////////////////////////////////////
MainWnd::~MainWnd()
{
    delete ui;
}

///////////////////////////////////////////////////////////////////////////////
void MainWnd::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

///////////////////////////////////////////////////////////////////////////////
void MainWnd::setupGroups()
{

    //qDebug() << "Running setup groups";
    setupGroup(&SoftwareList, ui->SoftwareGBox);
    setupGroup(&SystemList, ui->SystemGBox);
    setupGroup(&HardwareList, ui->HardwareGBox);
    setupGroup(&NetworkingList, ui->NetworkingGBox);
    setupGroup(&ToolsList, ui->ToolsGBox);
    setupGroup(&DEList, ui->DEGBox);
}

///////////////////////////////////////////////////////////////////////////////
void MainWnd::setupGroup(QGroupList** List, QWidget* Parrent)
{
	QGridLayout *layout = new QGridLayout(Parrent);
	(*List)=new QGroupList(Parrent);
	layout->addWidget(*List);
	Parrent->setLayout(layout); 
        //qDebug() << "SetupGroup";
        QObject::connect(*List, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(on_itemActivated(QListWidgetItem*)));
}

///////////////////////////////////////////////////////////////////////////////
void MainWnd::fillGroups()
{
        //qDebug() << "Running fillGroups";
        mRefreshMutex.lock();

        bool isVisible;

        ui->centralWidget->setEnabled(false);
        ui->statusBar->showMessage(tr("Reading items..."));
        QApplication::processEvents();

        bool isSu = misWheelGroup || misOperatorGroup;
        bool isSudo = misWheelGroup || misOperatorGroup;

        InstalledPBIs.refresh();        

        isVisible = (bool)SoftwareList->read(SOFTWARE_DIR, mvEnabledDE, isSudo, isSu);
	ui->SoftwareGBox->setVisible(isVisible);
	ui->SoftwareLine->setVisible(isVisible);

        isVisible = (bool)SystemList->read(SYSTEM_DIR, mvEnabledDE, isSudo, isSu);
	ui->SystemGBox->setVisible(isVisible);
	ui->SystemLine->setVisible(isVisible);

        isVisible = (bool)HardwareList->read(HARDWARE_DIR, mvEnabledDE, isSudo, isSu);
	ui->HardwareGBox->setVisible(isVisible);
	ui->HardwareLine->setVisible(isVisible);

        isVisible = (bool)NetworkingList->read(NETWORKING_DIR, mvEnabledDE, isSudo, isSu);
	ui->NetworkingGBox->setVisible(isVisible);
	ui->NetworkingLine->setVisible(isVisible);

        isVisible = (bool)ToolsList->read(TOOLS_DIR, mvEnabledDE, isSudo, isSu);
	ui->ToolsGBox->setVisible(isVisible);
	ui->ToolsLine->setVisible(isVisible);

        isVisible = (bool)DEList->read(DE_DIR, mvEnabledDE, isSudo, isSu);
        ui->DEGBox->setVisible(isVisible);
        ui->DELine->setVisible(isVisible);

        ui->statusBar->clearMessage();
        ui->centralWidget->setEnabled(true);

        if( NetworkingList->sizeHintForRow(0) > 0 ) {
          SystemList->setGridSize(QSize(NetworkingList->sizeHintForColumn(0), NetworkingList->sizeHintForRow(0)));
          SoftwareList->setGridSize(QSize(NetworkingList->sizeHintForColumn(0), NetworkingList->sizeHintForRow(0)));
          HardwareList->setGridSize(QSize(NetworkingList->sizeHintForColumn(0), NetworkingList->sizeHintForRow(0)));
          ToolsList->setGridSize(QSize(NetworkingList->sizeHintForColumn(0), NetworkingList->sizeHintForRow(0)));
          DEList->setGridSize(QSize(NetworkingList->sizeHintForColumn(0), NetworkingList->sizeHintForRow(0)));
          NetworkingList->setGridSize(QSize(NetworkingList->sizeHintForColumn(0), NetworkingList->sizeHintForRow(0)));
          SystemList->update();
          SoftwareList->update();
          HardwareList->update();
          ToolsList->update();
          DEList->update();
          NetworkingList->update();
          //qDebug() << NetworkingList->sizeHintForRow(0);
          //qDebug() << NetworkingList->sizeHintForColumn(0);
        } 

        mRefreshMutex.unlock();
}

///////////////////////////////////////////////////////////////////////////////
void MainWnd::on_itemActivated ( QListWidgetItem* item )
{
    if( item )
    {
        ((QCPItem*)item)->launch();
    }
}

///////////////////////////////////////////////////////////////////////////////
void MainWnd::on_lineEdit_textChanged(QString Text)
{
	bool isVisible;

    Text=Text.trimmed();

	isVisible = SoftwareList->setFilter(Text);
	ui->SoftwareGBox->setVisible(isVisible);
	ui->SoftwareLine->setVisible(isVisible);
		SoftwareList->setDisplayDEName(misDisplayDEName);

	isVisible = SystemList->setFilter(Text);
	ui->SystemGBox->setVisible(isVisible);
	ui->SystemLine->setVisible(isVisible);
        SystemList->setDisplayDEName(misDisplayDEName);

	isVisible = NetworkingList->setFilter(Text);
	ui->NetworkingGBox->setVisible(isVisible);
	ui->NetworkingLine->setVisible(isVisible);
        NetworkingList->setDisplayDEName(misDisplayDEName);

	isVisible = ToolsList->setFilter(Text);
	ui->ToolsGBox->setVisible(isVisible);
	ui->ToolsLine->setVisible(isVisible);
        ToolsList->setDisplayDEName(misDisplayDEName);

	isVisible = HardwareList->setFilter(Text);
	ui->HardwareGBox->setVisible(isVisible);
	ui->HardwareLine->setVisible(isVisible);
        HardwareList->setDisplayDEName(misDisplayDEName);

        isVisible = DEList->setFilter(Text);
        ui->DEGBox->setVisible(isVisible);
        ui->DELine->setVisible(isVisible);
        DEList->setDisplayDEName(misDisplayDEName);

}

///////////////////////////////////////////////////////////////////////////////
void MainWnd::on_toolButton_2_clicked()
{
	fillGroups();
	on_lineEdit_textChanged(ui->lineEdit->text());
}

///////////////////////////////////////////////////////////////////////////////
void MainWnd::setupDEChooser()
{
    DEChoiseMenu->addAction(ui->actionAll);
    DEChoiseMenu->addAction(ui->actionKDE);
    DEChoiseMenu->addAction(ui->actionGnome);
    DEChoiseMenu->addAction(ui->actionXFCE);
    DEChoiseMenu->addAction(ui->actionLXDE);
    DEChoiseMenu->addAction(ui->actionEnlightenment);
    DEChoiseMenu->addAction(ui->actionUnsupported);

    ui->DEChooserButton->setMenu(DEChoiseMenu);

    QString Current =  tr("(current)");
    QString Unsupported= tr("Unsupported DE");

#define SETUP_ACTION(action_name, DEName)\
    if (InstalledDEList.byName(DEName))\
    {\
        ui->action_name->setVisible(true);\
        if (InstalledDEList.byName(DEName)->misActive){\
            ui->action_name->setText(QString(DEName) + Current);\
            ui->DEChooserButton->setIcon(ui->action_name->icon());}\
        else\
            ui->action_name->setText(QString(DEName));\
        }else{ui->action_name->setVisible(false);}

SETUP_ACTION( actionKDE, "KDE" );
SETUP_ACTION( actionGnome, "Gnome" );
SETUP_ACTION( actionXFCE, "XFCE" );
SETUP_ACTION( actionLXDE, "LXDE" );
SETUP_ACTION( actionEnlightenment, "Enlightenment" );

#undef SETUP_ACTION

        if (!InstalledDEList.active())
	{
		ui->actionUnsupported->setText(Unsupported + Current);
		ui->DEChooserButton->setIcon(ui->actionUnsupported->icon());
		ui->actionUnsupported->setVisible(true);
	}
	else
		ui->actionUnsupported->setVisible(false);

        ui->DEChooserButton->setVisible(InstalledDEList.size()>1);

}

///////////////////////////////////////////////////////////////////////////////
void MainWnd::on_actionAll_triggered()
{
    ui->DEChooserButton->setIcon(ui->actionAll->icon());

    mvEnabledDE.clear();
    for (int i=0; i<InstalledDEList.size(); i++)
    {
        mvEnabledDE.push_back(InstalledDEList[i].mName);
    }

    ui->DEGBox->setTitle(DETEXT + " " + tr ("(All installed)"));

    misDisplayDEName = true;

    //refresh
    on_toolButton_2_clicked();
}

///////////////////////////////////////////////////////////////////////////////
void MainWnd::on_actionKDE_triggered()
{
    ui->DEChooserButton->setIcon(ui->actionKDE->icon());
    if (mCurrentDE.mName.trimmed().compare("KDE", Qt::CaseInsensitive))
        ui->DEGBox->setTitle(DETEXT + " " + tr ("(KDE)"));
    else
        ui->DEGBox->setTitle(DETEXT);

     mvEnabledDE.clear();
     mvEnabledDE.push_back("KDE");

    misDisplayDEName = false;

    //refresh
    on_toolButton_2_clicked();
}

///////////////////////////////////////////////////////////////////////////////
void MainWnd::on_actionLXDE_triggered()
{
    ui->DEChooserButton->setIcon(ui->actionLXDE->icon());
    if (mCurrentDE.mName.trimmed().compare("LXDE", Qt::CaseInsensitive))
        ui->DEGBox->setTitle(DETEXT + " " + tr ("(LXDE)"));
    else
        ui->DEGBox->setTitle(DETEXT);

    mvEnabledDE.clear();
    mvEnabledDE.push_back("LXDE");

    misDisplayDEName = false;

    //refresh
    on_toolButton_2_clicked();
}

///////////////////////////////////////////////////////////////////////////////
void MainWnd::on_actionGnome_triggered()
{
    ui->DEChooserButton->setIcon(ui->actionGnome->icon());
    if (mCurrentDE.mName.trimmed().compare("gnome", Qt::CaseInsensitive))
        ui->DEGBox->setTitle(DETEXT + " " + tr ("(Gnome)"));
    else
        ui->DEGBox->setTitle(DETEXT);

    mvEnabledDE.clear();
    mvEnabledDE.push_back("Gnome");

    misDisplayDEName = false;

    //refresh
    on_toolButton_2_clicked();
}

///////////////////////////////////////////////////////////////////////////////
void MainWnd::on_actionEnlightenment_triggered()
{
    ui->DEChooserButton->setIcon(ui->actionEnlightenment->icon());
    if (mCurrentDE.mName.trimmed().compare("Enlightenment", Qt::CaseInsensitive))
        ui->DEGBox->setTitle(DETEXT + " " + tr ("(Enlightenment)"));
    else
        ui->DEGBox->setTitle(DETEXT);

    mvEnabledDE.clear();
    mvEnabledDE.push_back("Enlightenment");
    mvEnabledDE.push_back("E16");

    misDisplayDEName = false;

    //refresh
    on_toolButton_2_clicked();
}

///////////////////////////////////////////////////////////////////////////////
void MainWnd::on_actionXFCE_triggered()
{
    ui->DEChooserButton->setIcon(ui->actionXFCE->icon());
    if (mCurrentDE.mName.trimmed().compare("XFCE", Qt::CaseInsensitive))
        ui->DEGBox->setTitle(DETEXT + " " + tr ("(XFCE)"));
    else
        ui->DEGBox->setTitle(DETEXT);

    mvEnabledDE.clear();
    mvEnabledDE.push_back("XFCE");

    misDisplayDEName = false;
    //refresh
    on_toolButton_2_clicked();
}

///////////////////////////////////////////////////////////////////////////////
void MainWnd::on_actionUnsupported_triggered()
{
	mvEnabledDE.clear();

	misDisplayDEName = false;
	//refresh
	on_toolButton_2_clicked();
}

///////////////////////////////////////////////////////////////////////////////
void MainWnd::slotSingleInstance()
{
   this->hide();
   this->showNormal();
   this->activateWindow();
   this->raise();
}

///////////////////////////////////////////////////////////////////////////////
void MainWnd::onDirectoryChanged(const QString& path)
{
    //Refresh if items or installed pbis dir was changed
    QObject::disconnect(&mDirWatcher,0,0,0);
    //qDebug() << "onDirectoryChanged";
    on_toolButton_2_clicked();
    QObject::connect(&mDirWatcher, SIGNAL(directoryChanged(const QString &)),
                     this, SLOT(onDirectoryChanged(const QString &)));
}

///////////////////////////////////////////////////////////////////////////////
bool MainWnd::checkUserGroup(QString groupName)
{
   QString loginName = getlogin();
   QStringList gNames;
   if ( loginName == "root" )
     return true;

   QString tmp;
   QFile iFile("/etc/group");
   if ( ! iFile.open(QIODevice::ReadOnly | QIODevice::Text))
     return true; //or FALSE?
                
   while ( !iFile.atEnd() ) {
     tmp = iFile.readLine().simplified();
     if ( tmp.indexOf(groupName) == 0 ) {
	gNames = tmp.section(":", 3, 3).split(",");
	break;
     }
   }
   iFile.close();
        
   for ( int i = 0; i < gNames.size(); ++i )
      if ( gNames.at(i).indexOf(loginName) == 0 )
            return true;

   return false;
}
