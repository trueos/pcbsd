/***************************************************************************
 *   Copyright (C) 2011 - iXsystems				 	  *
 *   kris@pcbsd.org  *
 *   tim@pcbsd.org   *
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
#include <QDateTime>
#include <QDebug>
#include <QFileDialog>
#include <QList>
#include <QPixmap>
#include <QSettings>
#include <QTimer>
#include <QTextStream>
#include <QStatusBar>

/* Local includes */
#include "ui_softmanager-main.h"
#include "softmanager-main.h"
#include <pcbsd-utils.h>
#include "pbi.h"
#include "clickedWidget.h"
#include "dialogPreferences.h"
#include "../config.h"


void PBM::ProgramInit()
{ 
   qDebug("Application starting...");

   // Load preferences
   slotLoadPref();

   // Set our default status
   pbistatus = PBI_UPDATED;
   isWorkingOnPBI = false;
   autoUpdateSoftware = false;

   // Check if member of group operator
   checkGroupOperator();

   // Load the program preferences
   slotLoadSettings();

   // Configure the menu
   setupMenuBar();
    
   // Connect our buttons / slots
   connect(toolHomeButton, SIGNAL( clicked() ), this, SLOT(slotHomeButtonClicked() ) );
   connect(toolBackButton, SIGNAL( clicked() ), this, SLOT(slotBackButtonClicked() ) );
   connect(pushSearch, SIGNAL( clicked() ), this, SLOT(slotSearchClicked() ) );
   connect(lineSearch, SIGNAL( returnPressed() ), this, SLOT(slotSearchClicked() ) );
   connect(lineSearch, SIGNAL( textChanged(const QString &) ), this, SLOT(slotSearchAsTyped() ) );
   connect(action_Quit, SIGNAL( triggered() ), this, SLOT(slotQuitButtonClicked()) );
   connect(actionPreferences, SIGNAL( triggered() ), this, SLOT(slotPrefButtonClicked()) );
   connect(tabPBM, SIGNAL( currentChanged(int) ), this, SLOT(slotTabChanged(int)) );

   // Connect the SoftwareListBox with signals
   SoftwareListBox->setContextMenuPolicy( Qt::CustomContextMenu);
   connect( SoftwareListBox, SIGNAL(customContextMenuRequested (const QPoint & ) ), this, SLOT(slotPBIRightClicked() ) );
   connect( SoftwareListBox, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int) ), this, SLOT(slotViewPBIClicked() ) );

   // Connect our main UI Buttons
   connect(pushUpdateAll, SIGNAL( clicked() ), this, SLOT(slotUpdateAllClicked() ) );
   connect(RemoveButton, SIGNAL( clicked() ), this, SLOT(RemoveButtonSlot() ) );
   connect(ProgressCancelButton, SIGNAL( clicked() ), this, SLOT(RemoveButtonSlot() ) );
   //Setup status bar
   AllPBIsLabel = new QLabel(this);
   InstalledPBIsLabel = new QLabel(this);
   statusBar()->addWidget(AllPBIsLabel);
   statusBar()->addWidget(InstalledPBIsLabel);

   // Begin to display the software browser
   displaySoftwareLoading();
   QTimer::singleShot(500, this, SLOT(startSoftwareBrowser() ) );

   // Confirm if we are running as root
   //checkRoot();
    
   qDebug("Populating list...");

   // Populate the PBI List
   PopulateList();
   refreshAppInfo(dynamic_cast<PBI*>(SoftwareListBox->currentItem()));
   AppInfoStack->setVisible(false);

   // Setup the filewatcher and PopulateList
   fileWatcher = new QFileSystemWatcher();
   if ( wardenDir.isEmpty() )
     fileWatcher->addPath("/var/db/pbi/installed");
   else
     fileWatcher->addPath(wardenDir + "/var/db/pbi/installed");
   connect(fileWatcher, SIGNAL(directoryChanged(const QString&)), this, SLOT(PopulateList()));

   // Load any saved states for PBIs
   //loadStatePBI(); 
    
   // Get the username of the person running X
   RealUserName = getlogin();
   qDebug() << "RealUser:" << RealUserName;

  /**********************************************************************/

  // Initialize some of our various variables
  totalSeconds = 0;
  downloadSize = 0;
  totalSteps = 0;
  attemptedRedownload = 0;
  firstInstall = 0;
  firstDownload = 0;
  currentWorkingPBI = NULL;

  // Connect slots for PBI updates
  connect( pushUpdatePBIs, SIGNAL( clicked() ), this, SLOT( slotStartPBIUpgrades() ) );
  connect( SoftwareListBox, SIGNAL( itemSelectionChanged() ), this, SLOT( slotPBISelectionChanged() ) );
  SoftwareListBox->resizeColumnToContents(0);
  // Start our loop to process PBIs
  QTimer::singleShot(3000, this, SLOT(slotProcessPBIs() ) );


}

void PBM::checkGroupOperator()
{
   QString loginName = getlogin();
   QStringList gNames;
   if ( loginName == "root" )
     return;

   QString tmp;
   QFile iFile("/etc/group");
   if ( ! iFile.open(QIODevice::ReadOnly | QIODevice::Text))
     return;
                
   while ( !iFile.atEnd() ) {
     tmp = iFile.readLine().simplified();
     if ( tmp.indexOf("operator") == 0 ) {
	gNames = tmp.section(":", 3, 3).split(",");
	break;
     }
   }
   iFile.close();
        
   for ( int i = 0; i < gNames.size(); ++i )
      if ( gNames.at(i).indexOf(loginName) == 0 )
	    return;

   QMessageBox::information( this, tr("Error!"), tr("Installing applications has been disabled for this user. Please add the user to the operator group or restart as root.") );
   close();  
}

void PBM::slotPrefButtonClicked()
{
  dialogPref = new dialogPreferences();
  dialogPref->programInit();
  dialogPref->setModal(true);
  connect( dialogPref, SIGNAL( saved() ), this, SLOT( slotLoadPref() ) );
  dialogPref->show(); 
}

void PBM::slotLoadPref()
{
  QSettings settings("PCBSD", "AppCafe");
  doMenuIcons = settings.value("menuIcons", true).toBool();
  doDeskIcons = settings.value("deskIcons", true).toBool();
}

void PBM::slotQuitButtonClicked()
{
  // Check if we are working right now
  if ( checkOkToClose() )
    close();
}

bool PBM::checkOkToClose()
{
  // If we are already working on a PBI, lets confirm the user wants to exit
  if ( isWorkingOnPBI ) {
     QString message = tr("You currently have PBIs downloading! Exit anyway?");
     if ( (QMessageBox::question(this, tr("Confirm"), message, QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) ) == QMessageBox::No)
     {
        return false;
     } else {
        // Yep, ready to cancel, lets stop the download / update
	if ( upgradePBIProc->state() == QProcess::Starting || upgradePBIProc->state() == QProcess::Running ) {
          upgradePBIProc->terminate();
	  upgradePBIProc->waitForFinished(5000);
          upgradePBIProc->kill();
	  upgradePBIProc->waitForFinished(5000);
	}
        isWorkingOnPBI = true;
        return true;
     }
  }

  return true;
}

// Adding a new Repo
void PBM::slotAddButtonClicked()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Select Repository"), "/home/" + RealUserName, "Repository File (*.rpo)");
  if ( ! fileName.isEmpty() && QFile::exists(fileName))
  {
	setEnabled(false);
	QApplication::processEvents();

	// Add the repo
        QString cmd;
        if ( wardenDir.isEmpty() )
	  cmd = "pc-su pbi_addrepo " + fileName;
        else
	  cmd = getPBICmd(QString("pbi_addrepo " + fileName));
	system(cmd.toLatin1());
	
	// Refresh the browser
	setEnabled(true);
	displaySoftwareLoading();
   	QTimer::singleShot(300, this, SLOT(startSoftwareBrowser() ) );
  }

}

void PBM::slotConfigButtonClicked()
{
  Repo tmpRepo = *(pbiRepos);

  // Create the repo config dialog
  softConfig = new softwareConfigWidget();
  softConfig->loadRepos(tmpRepo, curRepo);
  softConfig->programInit(wardenDir, wardenIP);
  softConfig->setModal(true);
  connect( softConfig, SIGNAL( saved() ), this, SLOT( slotRepoSaved() ) );
  softConfig->show(); 
}

// Reload the repo information
void PBM::slotRepoSaved()
{
	displaySoftwareLoading();
   	QTimer::singleShot(500, this, SLOT(startSoftwareBrowser() ) );
}

void PBM::RemoveButtonSlot()
{
   //Stop the scanner from interfering
   if ( wardenDir.isEmpty() )
     fileWatcher->removePath("/var/db/pbi/installed");
   else
     fileWatcher->removePath(wardenDir + "/var/db/pbi/installed");
   
   QTreeWidgetItemIterator it(SoftwareListBox);
   while (*it) {
      PBI *pbi = dynamic_cast<PBI*>(*it);
      if ((*it)->isSelected())
      {
        tabPBM->setEnabled(FALSE);

        QString removeapp;
        QString message, tmp;
        
        if ( pbi->isNewDownload() )
          message = tr("Cancel this download?");
        else 
          message = tr("Remove this application?");

        removeapp = pbi->getName() + " " + pbi->getVersion();

        //Cancel if the user doesn't want to go ahead with removal
        if (QMessageBox::question(this, tr("Confirm"), message + " " + removeapp, QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::No)
        {
          tabPBM->setEnabled(TRUE);
          return;
        }

	if ( pbi->isWorking() ) {
	  qDebug() << "Stopping PBI download...";
	  pbi->setDownloadFailed(true);
	  // Yep, ready to cancel, lets stop the download / update
          if ( upgradePBIProc->state() == QProcess::Starting || upgradePBIProc->state() == QProcess::Running ) {
            upgradePBIProc->terminate();
            upgradePBIProc->waitForFinished(5000);
            upgradePBIProc->kill();
            upgradePBIProc->waitForFinished(5000);
          }
	}

        // Remove the PBI
        RemovePBI(pbi);

        // Repopulate the list
        PopulateList();

	qDebug() << "Done after cancel/remove";
        //Restart scanner
        if ( wardenDir.isEmpty() )
          fileWatcher->addPath("/var/db/pbi/installed");
        else
          fileWatcher->addPath( wardenDir + "/var/db/pbi/installed");
   
        tabPBM->setEnabled(TRUE);
        return;
       }
      ++it;
   }
}

void PBM::RemovePBI(PBI *pbi)
{
    QString ProgDirName, procommand;

    // Mark this PBI as deleted
    pbi->setDelete(true);

    // Check if this is a real PBI, or just a download item we want to remove
    if ( ! pbi->isNewDownload() && pbi->getDownloadFailed() != true )
    {
      // This is a already installed PBI, lets remove it now
      qDebug() << "Removing PBI: " + pbi->getProgIndexName();

      staticDialog *busyDialog = new staticDialog();
      busyDialog->setModal(true);
      QLabel *busyText = new QLabel(tr("Please wait, removing:") + " " + pbi->getName() );
      QVBoxLayout *busyLayout = new QVBoxLayout();
      busyLayout->addWidget(busyText);
      busyDialog->setLayout(busyLayout);
      busyDialog->show();

      // Remove icons
      /********************/
      QProcess pbiicondel;

      if ( wardenDir.isEmpty() )
        pbiicondel.start(QString("pbi_icon"), QStringList() << "del-desktop" << pbi->getProgIndexName()); 
      else
        pbiicondel.start(QString("warden"), QStringList() << "chroot" << wardenIP << "pbi_icon del-desktop " + pbi->getProgIndexName()); 
      while ( pbiicondel.state() != QProcess::NotRunning ) {
         pbiicondel.waitForFinished(50);
         QCoreApplication::processEvents();
      }

      if ( wardenDir.isEmpty() )
        pbiicondel.start(QString("pbi_icon"), QStringList() << "del-menu" << pbi->getProgIndexName()); 
      else
        pbiicondel.start(QString("warden"), QStringList() << "chroot" << wardenIP << "pbi_icon del-menu " + pbi->getProgIndexName()); 
      while ( pbiicondel.state() != QProcess::NotRunning ) {
         pbiicondel.waitForFinished(50);
         QCoreApplication::processEvents();
      }

      if ( wardenDir.isEmpty() )
        pbiicondel.start(QString("pbi_icon"), QStringList() << "del-mime" << pbi->getProgIndexName()); 
      else
        pbiicondel.start(QString("warden"), QStringList() << "chroot" << wardenIP << "pbi_icon del-mime " + pbi->getProgIndexName()); 
      while ( pbiicondel.state() != QProcess::NotRunning ) {
         pbiicondel.waitForFinished(50);
         QCoreApplication::processEvents();
      }

      if ( wardenDir.isEmpty() )
        pbiicondel.start(QString("pbi_icon"), QStringList() << "del-pathlnk" << pbi->getProgIndexName()); 
      else
        pbiicondel.start(QString("warden"), QStringList() << "chroot" << wardenIP << "pbi_icon del-pathlnk " + pbi->getProgIndexName()); 
      while ( pbiicondel.state() != QProcess::NotRunning ) {
         pbiicondel.waitForFinished(50);
         QCoreApplication::processEvents();
      }
      /********************/
      // Done removing icons

      // Do the delete now
      QProcess pbicmd;
      if ( wardenDir.isEmpty() ) {
        if ( pbi->getInstallBy() == "root" || pbi->getInstallBy() != RealUserName)
          pbicmd.start(QString("pc-su"), QStringList() << "pbi_delete" <<  pbi->getProgIndexName() ); 
        else
          pbicmd.start(QString("pbi_delete"), QStringList() << pbi->getProgIndexName() ); 
      } else {
          // Using warden
          pbicmd.start(QString("warden"), QStringList() << "chroot" << wardenIP << "pbi_delete " + pbi->getProgIndexName() ); 
      }
      while ( pbicmd.state() != QProcess::NotRunning ) {
         pbicmd.waitForFinished(50);
         QCoreApplication::processEvents();
      }

      busyDialog->hide();
    }

    // Save our states again
    saveStatePBI();
}


// Function which returns a reference to a PBI based on the given name
PBI *PBM::getPBIFromName(QString name)
{
  QTreeWidgetItemIterator it(SoftwareListBox);
  while (*it) {
    PBI *item = dynamic_cast<PBI*>(*it);
    if ( item->getName().toUpper().remove(" ") == name.toUpper().remove(" ") )
       return item; 
    ++it;
  }
  
  PBI *tmp = 0;
  return tmp;
}

// Function which looks for PBI name matches with no spaces / case insensitve
bool PBM::checkPBILoadedFuzzy(QString name)
{
  QString tmp, tmp2;
  tmp = name.toUpper();
  tmp = tmp.replace(" ", "");

  QTreeWidgetItemIterator it(SoftwareListBox);
  while (*it) {
    PBI *item = dynamic_cast<PBI*>(*it);
    tmp2 = item->getName().toUpper();
    tmp2 = tmp2.replace(" ", "");
    if ( tmp == tmp2 )
       return true; 
    ++it;
  }
  return false;
}

// Function which checks if we already have a PBI with this name in the Tree
bool PBM::checkPBILoaded(QString name)
{
  QString tmp, tmp2;
  tmp = name.toUpper();
  tmp = tmp.replace(" ", "");

  QTreeWidgetItemIterator it(SoftwareListBox);
  while (*it) {
    PBI *item = dynamic_cast<PBI*>(*it);
    tmp2 = item->getName().toUpper();
    tmp2 = tmp2.replace(" ", "");
    if ( tmp == tmp2 && ! item->doDelete())
       return true; 
    ++it;
  }
  return false;
}

void PBM::markAllPBIDirty()
{
   QTreeWidgetItemIterator it(SoftwareListBox);
   while (*it) {
     PBI *item = dynamic_cast<PBI*>(*it);
     if ( ! item->isNewDownload() )
       item->setDirty(true);
     ++it;
   }
}

void PBM::removeAllDeletedPBI()
{
   QTreeWidgetItemIterator it(SoftwareListBox);
   while (*it) {
     PBI *item = dynamic_cast<PBI*>(*it);
     if (item->doDelete() ) {
       item->setHidden(true);
     }
     ++it;
   }
}

void PBM::removeAllPBIDirty()
{
   QTreeWidgetItemIterator it(SoftwareListBox);
   while (*it) {
     PBI *item = dynamic_cast<PBI*>(*it);
     if (item->isDirty() ) {
       item->setDelete(true);
       item->setHidden(true);
       SoftwareListBox->removeItemWidget((*it), 0);
       //delete it;
     }
     ++it;
   }
}


// Displays our items on the list
void PBM::PopulateList()
{
    PBI *pbi = 0;
    bool foundPBI = false;
    QPixmap defaultIcon(":/application.png");
    QString tmp, line, name, progIndexName, version, prefix, author, website, arch, icon, sig, repoID, autoUpdate, installBy;

    // Start by marking all PBIs as dirty
    markAllPBIDirty();

    // Remove any PBI's we've deleted
    removeAllDeletedPBI();

    loadPBIs = new QProcess();
    loadPBIs->setProcessChannelMode(QProcess::MergedChannels);
    if ( wardenDir.isEmpty() )
      loadPBIs->start(QString("pbi_info"), QStringList() << "-v");
    else
      loadPBIs->start(QString("warden"), QStringList() << "chroot" << wardenIP << "pbi_info -v");
    loadPBIs->waitForFinished(-1);
    repoID="-1";

    //qDebug("Reading PBI List");
    while ( loadPBIs->canReadLine() ) {
       	line = loadPBIs->readLine().simplified(); // line of text excluding '\n'
	
	if ( line.isEmpty() && !progIndexName.isEmpty() && !name.isEmpty() && !version.isEmpty() && !installBy.isEmpty() && !repoID.isEmpty() )
		foundPBI = true;

	if (line.indexOf("PBI Information for:" ) ==  0 )
          progIndexName = line.replace("PBI Information for: ", "");

	if (line.indexOf("Name: " ) ==  0 )
          name = line.replace("Name: ", "");

	if ( line.indexOf("Version: ") == 0)
	  version = line.replace("Version: ", "");

	if ( line.indexOf("InstalledBy: ") == 0)
	  installBy = line.replace("InstalledBy: ", "");

	if ( line.indexOf("Icon:") == 0)
	  if ( wardenDir.isEmpty() )
            icon = line.replace("Icon: ", "");
	  else
            icon = wardenDir + line.replace("Icon: ", "");

	if ( line.indexOf("Author:") == 0)
          author = line.replace("Author: ", "");

	if ( line.indexOf("Website:") == 0)
          website = line.replace("Website: ", "");

	if ( line.indexOf("Arch:") == 0)
          arch = line.replace("Arch: ", "");

	if ( line.indexOf("AutoUpdate:") == 0)
          autoUpdate = line.replace("AutoUpdate: ", "");

	if ( line.indexOf("Associated Repo:") == 0) {
          repoID = line.replace("Associated Repo: ", "");
          repoID.truncate(repoID.indexOf(" "));
	}

	if ( foundPBI ) {
	  foundPBI = false;
          // Check if this PBI is already loaded, and update its details if so
          if ( checkPBILoaded(name) )
          {
             // This PBI already exists, just update the existing entry
             PBI *item = getPBIFromName(name);
             if (item->getVersion() != version )
                item->setWorkingStatus(PBIINACTIVE);
             item->setVersion(version);
             item->setIconPath(icon);
             item->setAuthor(author);
             item->setWebsite(website);
             item->setRepoID(repoID);
             item->setInstallBy(installBy);
             item->setDirty(false);
             item->setDelete(false);
             item->setHidden(false);
	     if ( autoUpdate == "YES" ) 
		item->setAutoUpdateStatus(true);
	     else
		item->setAutoUpdateStatus(false);
             //qDebug() << "Updating PBI:" << name;
           } else {
             // New PBI! Add it to our list
             pbi = new PBI(defaultIcon.scaled(40,40));
             pbi->setProgIndexName(progIndexName);
             pbi->setName(name);
             pbi->setVersion(version);
             pbi->setIconPath(icon);
             pbi->setAuthor(author);
             pbi->setWebsite(website);
             pbi->setArch(arch);
             pbi->setRepoID(repoID);
             pbi->setInstallBy(installBy);
	     pbi->setDirty(false);
	     pbi->setDelete(false);
	     pbi->setHidden(false);
	     if ( autoUpdate == "YES" ) 
		pbi->setAutoUpdateStatus(true);
	     else
		pbi->setAutoUpdateStatus(false);
             SoftwareListBox->addTopLevelItem(pbi); 
             qDebug() << "Found PBI:" << name << version << repoID << icon;
           }

	   progIndexName="";
	   name="";
	   version="";
	   icon="";
	   author="";
	   website="";
	   arch="";
    	   repoID="-1";
	   autoUpdate="";
	   installBy="";
    	}
		
    }
	    
    // Remove any PBIs still marked as dirty, they no longer are installed
    removeAllPBIDirty();

    // PBIs are already properly sorted from pbi_info output
    //SoftwareListBox->sortItems(0, Qt::AscendingOrder);
    SoftwareListBox->resizeColumnToContents(0); 

    refreshAppInfo(dynamic_cast<PBI*>(SoftwareListBox->currentItem()));

    // Update the number of PBIs installed
    int installed = 0;
    for ( int i = 0; i < SoftwareListBox->topLevelItemCount(); ++i) {
       PBI *pbi = dynamic_cast<PBI*>(SoftwareListBox->topLevelItem(i));
       if ( pbi->isHidden() == false && ! pbi->isNewDownload() )
          installed++;
    }
    InstalledPBIsLabel->setText(QString(tr("Installed: ")) + QString::number(installed));

   // Check for PBI updates again, since our list has now changed
   QTimer::singleShot(200, this, SLOT(slotStartPBIUpdateCheck()));
}




void PBM::checkRoot()
{
  int uid = getuid();
 
  if (uid != 0)
  {
    QMessageBox::information( this, tr("Error!"), tr("This program must be run as root!") );
    close();  
  }

}


QString PBM::bytesToHumanReadable( float bytes )
{
    QStringList suffix;
    suffix << "B" << "KB" << "MB" << "GB" << "TB" << "PB";
    int i = 0;
    while((bytes >= 1000) && ( i < (suffix.size() - 1 )) )
    {
	bytes = bytes / 1024;
	i++;
    }
    QString result = QString::number(bytes, 'f', 0);
    result += suffix[i];
    return result;
}


int PBM::getDirSize( QString path )
{
    int totalSize = 0;
    QStringList paths;
    paths += path;
    
    for (int i=0; i<paths.size(); i++)
    {
	QFileInfo dirInfo(paths[i]);
	if (!dirInfo.exists()) break;
	totalSize += dirInfo.size();
	QDir dir(paths[i]);
	const QFileInfoList files = dir.entryInfoList();
	for (int i=0; i<files.size(); i++)
	{
	    QFileInfo fileInfo = files.at(i);
	    if (fileInfo.isDir() && (( ! fileInfo.isSymLink()) && (fileInfo.fileName() != "..") && (fileInfo.fileName() != "."))) { paths += fileInfo.absoluteFilePath(); }
	    else if (fileInfo.isFile()) { totalSize += fileInfo.size(); }
	}
    }
    return totalSize;
}


QString PBM::getLineFromCommandOutput( QString command )
{
	FILE *file = popen(command.toLatin1(),"r"); 
	
	char buffer[100];
	
	QString line = ""; 
	char firstChar;
	
	if ((firstChar = fgetc(file)) != -1){
		line += firstChar;
		line += fgets(buffer,100,file);
	}
	
	
	pclose(file);
	
	
	return line;
}


void PBM::slotLoadSettings() {
  // Load the user preferences for the System Updater
  QSettings settings("PCBSD");

  // Get the PC-BSD Version
  Version = getLineFromCommandOutput("uname -r");

  // Get the system type
  QString command = "uname -m";
  Arch = getLineFromCommandOutput(command);
  Arch = Arch.simplified();

  QLocale mylocale;
  Lang = mylocale.name();
  if ( Lang == "C" )
    Lang = "en";

  // Load a custom tmpdir
  customTmpDir= PATCHTMPDIR_DEFAULT;

}

// Check if the tab has changed, and we need to redraw anything
void PBM::slotTabChanged(int tabindex)
{
	// We are only interested in switching back to the browser right now
	if ( tabindex != 0 )
		return;

	// If the currently shown page is a PBI, then lets refresh it
	if ( curPageType == "PBI" )
		displayPBI(curPageName, curPageRepo);

}

// Setup the menu bar
void PBM::setupMenuBar()
{
	repomenu = menuBar()->addMenu(tr("&Repositories"));
}

void PBM::slotSingleInstance()
{
  this->hide();
  this->showNormal();
  this->activateWindow();
  this->raise();
}

void PBM::closeEvent(QCloseEvent *event)
{
  // Check if we are working right now
  if ( checkOkToClose() )
    close();
  else
    event->ignore();
}


void PBM::showInstalledTab()
{
  tabPBM->setCurrentIndex(1);
}

void PBM::setWardenMode(QString wdir, QString wip)
{
  QDir testDir(wdir);
  if ( ! testDir.exists() ) {
    QMessageBox::critical(this, tr("AppCafe"),
                                tr("Invalid warden directory:") + " " + wdir,
                                QMessageBox::Ok, QMessageBox::Ok);
    exit(1);
  }
  wardenDir = wdir;
  wardenIP = wip;
  setWindowTitle(tr("AppCafe - Warden") + " " + wardenIP );
}

QString PBM::getPBICmd(QString cmd)
{
  if ( wardenDir.isEmpty() )
    return cmd;

  return QString("warden chroot " + wardenIP + " \"" + cmd + "\""); 
}

void PBM::slotUpdateAllClicked()
{
   QTreeWidgetItemIterator it(SoftwareListBox);
   while (*it) {
     PBI *item = dynamic_cast<PBI*>(*it);
     bool isUpdate= item->getUpdateAvail() && ! item->isNewDownload();
     if (isUpdate)
        markPBIForUpdate(item);
     ++it;
   }

}
