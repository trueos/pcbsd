#include "softmanager-main.h"
#include <QTimer>
#include <QTextStream>
#include <QSettings>
#include <QToolTip>
#include <QDateTime>
#include "pbi.h"


/***********************************************************************************/
/* Code to perform the PBI updates / checks                                     */
/***********************************************************************************/

void PBM::slotStartPBIUpdateCheck()
{
  if ( pbistatus == PBI_CHECKING4UPDATES || pbistatus == PBI_UPDATING )
    return;

  // No updates yet, disable the button
  pushUpdateAll->setVisible(false);

  // Set our Iterator, and start checking our list for updates!
  currentCheckingPBI = new QTreeWidgetItemIterator(SoftwareListBox);
  pbistatus = PBI_CHECKING4UPDATES;
  slotPBICheckUpdate();

} 


// Start checking our list of PBIs which may be updatable
void PBM::slotPBICheckUpdate()
{
  // Need to loop through iterator here, and test each PBI for updates 
  // and if it is, check for update, then continue on to next currentCheckingPBI++ until we
  // are at the end of the list
  QTreeWidgetItemIterator it(*currentCheckingPBI);
  PBI *item = dynamic_cast<PBI*>(*it);
  if (*it) {
    if ( item->getWorkingStatus() != PBIINACTIVE || item->getProgIndexName().isEmpty() || item->doDelete() )
    {
       // This PBI is missing a update URL, is already busy or deleted, skip to next!
       (*currentCheckingPBI)++;
       QTimer::singleShot(100, this, SLOT(slotPBICheckUpdate() ) );
       return;
    }
  }  else {
    // All finished checking for updates! Lets reset status and return now
    pbistatus = PBI_UPDATED;
    return;
  }
  

  // If we've gotten this far, we have a new PBI to check for updates to!
  ///item->setText(1, tr("Checking for Updates..."));
  resfreshAppStateText(item, tr("Checking for Updates..."));
  qDebug() << "Starting update check for:" << item->getProgIndexName();

  checkPBIProc = new QProcess();
  checkPBIProc->setProcessChannelMode(QProcess::MergedChannels);
  connect(checkPBIProc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(slotPBIUpdateCheckDone()) );
  if ( wardenDir.isEmpty() )
    checkPBIProc->start(QString("pbi_update"), QStringList() << "-c" << item->getProgIndexName() );
  else
    checkPBIProc->start(QString("warden"), QStringList() << "chroot" << wardenIP << "pbi_update -c " + item->getProgIndexName() );

}

void PBM::slotReadPBIBuffer()
{
    QTreeWidgetItemIterator it(*currentCheckingPBI);
    PBI *item = dynamic_cast<PBI*>(*it);
    QString output, tmp;

    // Read in the output from the update service
    output = checkPBIProc->readAll();

    qDebug() << "Update Return:" << output;

    if ( output.indexOf("Available: ") != -1  && ! output.isEmpty())
    {
	  tmp = output.remove(0, output.indexOf("Available: "));
	  tmp = tmp.replace("Available: ", "");
          item->setDownloadVersion(tmp.simplified());
	
          // Great, we have a PBI with an available update! Notify the widget now          
          item->setUpdateAvail(true);
          refreshAppInfo(item);

	  // Enable the update all button
      pushUpdateAll->setVisible(true);
    } else {
      // Update is invalid for some reason, no update for you!      
      item->setUpdateAvail(false);
      refreshAppInfo(item);
    }

    // All done reading PBI Update check buffer data, continue on to next PBI in list
    (*currentCheckingPBI)++;
    QTimer::singleShot(100, this, SLOT(slotPBICheckUpdate() ) );
}


void PBM::slotPBIUpdateCheckDone()
{
  QTimer::singleShot(200, this, SLOT(slotReadPBIBuffer() ) );
}


// Slot called when the currently selected PBI changes
void PBM::slotPBISelectionChanged()
{
   refreshAppInfo(dynamic_cast<PBI*>(SoftwareListBox->currentItem()));
}


// function to check if we have available updates selected
bool PBM::checkPBISelectedUpdates()
{
   bool found = false;
   QTreeWidgetItemIterator it(SoftwareListBox);
   while (*it) {
      PBI *item = dynamic_cast<PBI*>(*it);
      if ( item->getUpdateAvail() && (*it)->isSelected() )
      {
        // The user wants to update this item! Set the status and tmpfile now
        found = true;
        markPBIForUpdate(item);
      }
     ++it;
   }
   return found;
}

// Marks a PBI is ready for update
void PBM::markPBIForUpdate(PBI *item)
{
   // Mark the status as PBIUpgrading   
   item->setWorkingStatus(PBIUPDATING);
   resfreshAppStateText(item, tr("Pending Upgrade"));
   reportAppProgress(item, 0, 0, tr("Pending Upgrade"));

}

// The user has requested to start upgrading the PBIs, lets do it!
void PBM::slotStartPBIUpgrades()
{

  // Lets start by checking if we have any PBIs selected to update and populate the todo list
  if ( ! checkPBISelectedUpdates() ) 
     QMessageBox::critical( 0, tr("Update Manager"), tr("No software selected to update!"), QMessageBox::Ok );
}


// Function which purges and frees up deleted items, called by slotProcessPBIs when its free
void PBM::purgeAllDeletedPBI()
{
   //qDebug() << "Running purgeAllDeletedPBI";
   QTreeWidgetItemIterator it(SoftwareListBox);
   while (*it) {
     PBI *item = dynamic_cast<PBI*>(*it);
     if (item->doDelete() && item->getName() != "PBIDELETED" ) {
       qDebug() << "Deleting: " << item->getName();
       SoftwareListBox->removeItemWidget(item, 0);
       item->setName("PBIDELETED");
       //delete item;
     }
     ++it;
   }
}


// Process our PBI list, see what is next to do and do it
void PBM::slotProcessPBIs()
{
  // If we are already working on a PBI, lets come back later
  if ( isWorkingOnPBI ) {
     QTimer::singleShot(300, this, SLOT(slotProcessPBIs() ) );
     return;
  }

  if (!currentWorkingPBI) {

    // Init our new Iterator
    currentWorkingPBI = new QTreeWidgetItemIterator(SoftwareListBox);
  }

  // Nothing is currently being worked on, lets see whats next to work on
  QTreeWidgetItemIterator it(*currentWorkingPBI);
  if (*it) {
    PBI *item = dynamic_cast<PBI*>(*it);
    if ( item->getWorkingStatus() != PBIINACTIVE && item->doDelete() == false)
    {
       isWorkingOnPBI = true;

       // We've found a new PBI to work on, lets start it up
       reportAppProgress(item, 0, 0, tr("Starting Download"),"");

       item->setDownloadAttempts(0);

       // Start the download now
       slotDownloadPBIUpdate();
    } else {
      // Update our working PBI
      (*currentWorkingPBI)++;
    }
  } else {
    // Purge any deleted items before starting a new Iterator
    purgeAllDeletedPBI();
    currentWorkingPBI = new QTreeWidgetItemIterator(SoftwareListBox);
  }
  
  // Re-run this function to keep tabs on our children!
  QTimer::singleShot(300, this, SLOT(slotProcessPBIs() ) );
}


// Start grabbing the filesize for this PBI and lets get them downloaded
void PBM::slotDownloadPBIUpdate()
{

  QTreeWidgetItemIterator it(*currentWorkingPBI);
  PBI *item = dynamic_cast<PBI*>(*it);

  // Make sure this item hasn't been deleted
  if ( item->doDelete() ) {
     isWorkingOnPBI = false;
     return;
  }

  // Reset this PBIs status, as not failing the download
  item->setDownloadFailed(false);

  // Check if we need to start the next download
  if ( item->getDownloadAttempts() == 3)
  {
     // Reset the status to inactive for this PBI
     item->setWorkingStatus(PBIINACTIVE);
     isWorkingOnPBI = false;
     ///item->setStatusText(tr("Download failed! Please try again later."));
     item->setDownloadFailed(true);
     refreshAppInfo(item);
     return;
  }

  if ( wardenDir.isEmpty() )
    fileWatcher->removePath("/var/db/pbi/installed");
  else
    fileWatcher->removePath( wardenDir + "/var/db/pbi/installed");

  // Update the status if the attempted download worked
  if ( item->getDownloadAttempts() >= 1)
  {
      resfreshAppStateText(item, tr("Download failed... Attempting to re-download..."));
      reportAppProgress(item, 0, 0, tr("Download starting..."),tr("Download failed... Attempting to re-download..."));
  }
  else
  {
      item->setStatusText(tr("Download starting..."));
      reportAppProgress(item, 0, 0, tr("Download starting..."),"");
  }

  // Start the upgrade process
  upgradePBIProc = new QProcess( this );

  // Set the process environment flag for parsing download status
  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  env.insert("PBI_FETCH_PARSING", "YES");
  upgradePBIProc->setProcessEnvironment(env);

  // Setup the read connections
  upgradePBIProc->setProcessChannelMode(QProcess::MergedChannels);
  connect( upgradePBIProc, SIGNAL(readyRead()), this, SLOT(slotParsePBIOutput() ) );
  connect( upgradePBIProc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(slotFinishedPBIInstall() ) );
  afterDownload=false;

  // Doing a new download
  if ( item->getWorkingStatus() == PBINEWDOWNLOAD ) {
     if ( wardenDir.isEmpty() ) {
	if (item->getInstallBy() == "root" ) 
    	  upgradePBIProc->start(QString("/usr/local/bin/pc-su"), QStringList() << "pbi_add" << "-f" << "--repo" << item->getDownloadRepo() << "--rArch" << item->getDownloadArch() << "--rVer" << item->getDownloadVersion() << "-r" << item->getName());
	else
          upgradePBIProc->start(QString("pbi_add"), QStringList() << "-f" << "-v" << "--repo" << item->getDownloadRepo() << "--rArch" << item->getDownloadArch() << "--rVer" << item->getDownloadVersion() << "-r" << item->getName());
     } else {
       // Using warden jail
       upgradePBIProc->start(QString("warden"), QStringList() << "chroot" << wardenIP << "pbi_add -f --repo " + item->getDownloadRepo() + " --rArch " + item->getDownloadArch() + " --rVer " + item->getDownloadVersion() + " -r " + item->getName());
       qDebug()  << QString("warden") << "chroot" << wardenIP << "pbi_add -f --repo " + item->getDownloadRepo() + " --rArch " + item->getDownloadArch() + " --rVer " + item->getDownloadVersion() + " -r " + item->getName();
     }

  // Doing an upgrade
  } else {
     if ( wardenDir.isEmpty() ) {
	if (item->getInstallBy() == "root" ) 
    	  upgradePBIProc->start(QString("/usr/local/bin/pc-su"), QStringList() << "pbi_update" << item->getProgIndexName());
	else
    	  upgradePBIProc->start(QString("pbi_update"), QStringList() << item->getProgIndexName());
     } else {
       // Doing warden update
       upgradePBIProc->start(QString("warden"), QStringList() << "chroot" << wardenIP << "pbi_update " + item->getProgIndexName());
     }
  }

  // Set the flag that we are working on this PBI
  item->setIsWorking(true);
}

// Slot which reads output of PBI management command
void PBM::slotParsePBIOutput()
{
  QString line;

  QTreeWidgetItemIterator it(*currentWorkingPBI);
  PBI *item = dynamic_cast<PBI*>(*it);

  // Read the output of the upgrade process
  while (upgradePBIProc->canReadLine() ) {
    line = upgradePBIProc->readLine().simplified();    

    if (!afterDownload)
    {
        if (!parsePBIDownload(item, line))
        {
           afterDownload = true;
           if (item->getWorkingStatus() == PBIUPDATING)
           {
               resfreshAppStateText(item, tr("Updating"));
               reportAppProgress(item, 0,0,tr("Updating..."),"");
           }
        }
    } else {      
        ///item->setStatusText(tr("Installing..."));         
        if (item->getWorkingStatus() != PBIUPDATING)
            parsePBIInstall(item, line);
    }
  }
}

bool PBM::parsePBIDownload(PBI *item, QString line)
{
    bool ok, ok2;
    long cur, tot;
    QString speed;
    QString tmp, tmp2;

    line = line.trimmed();
    if (line.indexOf("FETCHDONE") == 0 )
    {
        return false;
    }
    if ( line.indexOf("SIZE: ") == 0 ) {
        line.replace("SIZE: ", "");
        line.replace("DOWNLOADED: ", "");
        line.replace("SPEED: ", "");
        line.section(" ", 0, 0).toInt(&ok);
        line.section(" ", 1, 1).toInt(&ok2);
        //line.section(" ", 2, 2).toInt(&ok3);

        if ( ok && ok2 ) {
            tot = line.section(" ", 0, 0).toInt(&ok);
            cur = line.section(" ", 1, 1).toInt(&ok2);
            QString percent = QString::number( (float)(cur * 100)/tot );
            speed = line.section(" ", 2, 3);

            // Get the MB downloaded / total
            tot = tot / 1024;
            cur = cur / 1024;

            ////item->setStatusText
            QString ProgressString =QString(tr("Downloading: %1MB of %2MB (%3 %) at %4")).arg(cur).arg(tot).arg(percent.section(".", 0, 0)).arg(speed);
            QString ProgressString2 =QString(tr("%1MB of %2MB at %3")).arg(cur).arg(tot).arg(speed);
            //resfreshAppStateText(item, tr("Downloading:") + " (" + tmp.setNum(cur) + "/" + tmp2.setNum(tot) + "MB) " + percent.section(".", 0, 0) + "%" + tr(" at ") + speed);
            resfreshAppStateText(item, ProgressString);
            reportAppProgress(item, cur, tot, tr("Downloading..."),ProgressString2);
            return true;
        }
    }

    return true;
}

bool PBM::parsePBIInstall(PBI *item, QString line)
{
    static int nFiles = 0;
    static int curr = 0;
    line = line.trimmed();

    if (line.indexOf("TOTALFILES:") == 0 )
    {
        resfreshAppStateText(item, tr("Installing"));
        line.replace("TOTALFILES:" , "");
        nFiles = line.toInt();
        curr = 0;
        return true;
    }
    if (line.indexOf("x ./") == 0 )
    {
        line.replace("x ./", "");
        curr++;
        reportAppProgress(item, curr, nFiles, tr("Installing..."),line);
        return true;
    }
    if (line.indexOf("Extraction Finished!") == 0 )
    {
        nFiles = 0;
        curr = 0;
        reportAppProgress(item, 0, 0, tr("Finalizing installation..."), "");
        return false;
    }
    return true;
}


// Slot which checks a PBIs installation status and finishes up with this PBI
void PBM::slotFinishedPBIInstall()
{
  QTreeWidgetItemIterator it(*currentWorkingPBI);
  PBI *item = dynamic_cast<PBI*>(*it);
  item->setIsWorking(false);

  if ( upgradePBIProc->exitCode() != 0 ) {
     QMessageBox::critical( 0, tr("Online Update"), tr("An error occurred while installing") + " " + \
     item->getName() + " " + tr("Please check your network connection or mirror settings. "), QMessageBox::Ok );
  } else {

     // If this is a new install, also add the desktop icon
     if ( item->getWorkingStatus() == PBINEWDOWNLOAD && ! item->doDelete() && doDeskIcons)
	installDesktopIcon(item);

     // Update the status on the previous download to finished
     ////item->setStatusText("");
     resfreshAppStateText(item, "");
     refreshAppInfo(item);
     item->setNewDownload(false);

     // Update our trigger
     QFile pbiTrig( PBITRIGGER );
     if ( pbiTrig.open( QIODevice::WriteOnly ) ) {
       QTextStream streamTrig( &pbiTrig );
        streamTrig << QDateTime::currentDateTime().toString("hhmmss");
	pbiTrig.close();
     }
  }
  
  item->setWorkingStatus(PBIINACTIVE);
  refreshAppInfo(item);

  // Purge any deleted items
  purgeAllDeletedPBI();

  // Repopulate the PBI List
  PopulateList();

  // Savestate in case we close
  //saveStatePBI();

  // Restart the PBI file watcher
  if ( wardenDir.isEmpty() )
    fileWatcher->addPath("/var/db/pbi/installed");
  else
    fileWatcher->addPath( wardenDir + "/var/db/pbi/installed");

  // Mark this PBI as finished
  isWorkingOnPBI = false;
}


void PBM::slotPBIRightClicked()
{
   QTreeWidgetItemIterator it(SoftwareListBox);
   while (*it) {
      PBI *item = dynamic_cast<PBI*>(*it);
      if ( (*it)->isSelected() )
      {
         popup = new QMenu();
	 popup->setTitle(item->getName());

	 // If this is in an associated repo show the view details option
 	 if ( item->getRepoID() != "-1" )
           popup->addAction( tr("View details"), this, SLOT(slotViewPBIClicked()));

         if ( ! item->isNewDownload() ) {
           popup->addSeparator();
           if(item->hasDesktopEntries()){
             popup->addAction( tr("Install Desktop Icons"), this, SLOT(slotAddDesktopIcons()));
	   }
           if(item->hasMenuEntries()){
             popup->addAction( tr("Install Menu Icons"), this, SLOT(slotAddMenuIcons()));
             popup->addAction( tr("Install Menu Icons (All Users)"), this, SLOT(slotAddMenuIconsRoot()));
           }
           popup->addAction( tr("Install PATH links"), this, SLOT(slotAddPATHLink()));
           popup->addAction( tr("Install PATH links (All Users)"), this, SLOT(slotAddPATHLinkRoot()));
           if(item->hasDesktopEntries() || item->hasMenuEntries() ){ popup->addSeparator(); }
           popup->addAction( tr("Uninstall"), this, SLOT(RemoveButtonSlot()));
           popup->exec( QCursor::pos() );
         } else {
           // If this is a new download, offer cancel option
           if ( item->isNewDownload() ) {
             popup->addAction( tr("Cancel Download"), this, SLOT(RemoveButtonSlot()));
             popup->exec( QCursor::pos() );
           }
           // If the item failed to download, let us re-start it here
           if ( item->getDownloadFailed() ) {
             popup->addAction( tr("Retry Download"), this, SLOT(slotRetryPBIDownload()));
             popup->exec( QCursor::pos() );
           }
         }

      }
     it++;
   }
}

// Slot to mark a PBI as ready to re-download
void PBM::slotRetryPBIDownload()
{
   QTreeWidgetItemIterator it(SoftwareListBox);
   while (*it) {
      PBI *item = dynamic_cast<PBI*>(*it);
      if ( (*it)->isSelected() )
      {
        item->setNewDownload(true);
        item->setWorkingStatus(PBINEWDOWNLOAD);
        resfreshAppStateText(item, tr("Pending Download..."));
        //item->setStatusText(tr("Pending Download..."));
        return;
      }
      it++;
   }
}

// Function which schedules a new PBI for download 
void PBM::scheduleNewPBIDownload(QString name, int reponum, bool root)
{
	int pbiNum = -1;
        for( int i = 0; i < pbiAvail->count(); i++)
                if ( pbiAvail->getName(i) == name && pbiAvail->getRepo(i) == pbiRepos->getID(curRepo)) {
                        pbiNum = i;
                        break;
                }
        if ( pbiNum == -1 )
                return;

	// Remove old page from browser stack
 	slotBackButtonClicked();

	// Show download page
  	tabPBM->setCurrentIndex(1);

  	// Create a new PBI object and mark it as a new download
  	QPixmap defaultIcon(":/download.png");
  	PBI *pbi = 0;
  	pbi = new PBI(defaultIcon.scaled(40,40));
  	pbi->setNewDownload(true);
  	pbi->setName(name);
  	pbi->setVersion(pbiAvail->getVer(pbiNum));
  	pbi->setDownloadArch(pbiAvail->getArch(pbiNum));
  	pbi->setDownloadVersion(pbiAvail->getVer(pbiNum));
  	pbi->setDownloadRepo(pbiRepos->getID(reponum));
  	pbi->setProgIndexName(name.toLower() + "-" + pbiAvail->getVer(pbiNum).toLower() + "-" + pbiAvail->getArch(pbiNum).toLower());
  	pbi->setWorkingStatus(PBINEWDOWNLOAD);

  	pbi->setUpdateAvail(false);

	if ( root )
		pbi->setInstallBy("root");

  	// Add to the list box
  	SoftwareListBox->addTopLevelItem(pbi); 
    resfreshAppStateText(pbi, tr("Pending Download..."));
    reportAppProgress(pbi, 0, 0, tr("Pending Download..."), "");
  	SoftwareListBox->resizeColumnToContents(0);

  	// Savestate in case we close
  	//saveStatePBI();
}

// Save our PBI states
void PBM::saveStatePBI()
{  
  QString tmp;

  // Save the current PBI states in the settings manager
  QSettings settings("PCBSD");

  // Write our PBI array
  settings.beginWriteArray("/PC-BSD/SoftwareManager/workingNewPBIs");
  int i = 0;
  QTreeWidgetItemIterator it(SoftwareListBox);
  while (*it) {
    PBI *item = dynamic_cast<PBI*>(*it);
    // Right now, only save the entry if its a new download and currently pending
    if ( ! item->doDelete() && item->getWorkingStatus() == PBINEWDOWNLOAD && item->isNewDownload() ) {
      settings.setArrayIndex(i);
      settings.setValue("IconPath", item->getIconPath());
      settings.setValue("Version", item->getVersion());
      settings.setValue("Name", item->getName()); 
      i++;
    }

    ++it;
  }
  settings.endArray();

 
}

// Load our PBI states to see if we have anything pending
void PBM::loadStatePBI()
{
  // Read in any saved working PBIs
  QSettings settings("PCBSD");

  // Read our PBI array
  int size = settings.beginReadArray("/PC-BSD/SoftwareManager/workingNewPBIs");
  for (int i = 0; i < size; ++i) {
     settings.setArrayIndex(i);
     // Create a new PBI object and mark it as a new download
     PBI *pbi = 0;
     QPixmap defaultIcon("/Programs/.config/pbidownload.png");
     pbi = new PBI(defaultIcon.scaled(40,40));

     pbi->setNewDownload(true);
     pbi->setWorkingStatus(PBINEWDOWNLOAD);
     pbi->setUpdateAvail(false);

     pbi->setName(settings.value("Name").toString());
     pbi->setVersion(settings.value("Version").toString());
     pbi->setDownloadVersion(settings.value("Version").toString());

     // Add to the list box
     SoftwareListBox->addTopLevelItem(pbi); 
     SoftwareListBox->resizeColumnToContents(0);
     resfreshAppStateText(pbi, tr("Pending Download..."));
     ///pbi->setStatusText(tr("Pending Download..."));

  }
  settings.endArray();

}

void PBM::slotRestartAllFailedPBIDownloads()
{
	QTreeWidgetItemIterator it(SoftwareListBox);
  	while (*it) {
      		PBI *item = dynamic_cast<PBI*>(*it);
      		if ( item->getWorkingStatus() == PBIINACTIVE && \
			 item->getDownloadFailed() == true )
      		{
       			item->setNewDownload(true);
        		item->setWorkingStatus(PBINEWDOWNLOAD);
                reportAppProgress(item, 0, 0, tr("Pending Download..."));
                resfreshAppStateText(item, tr("Pending Download..."));
      		}
      		it++;
  	}
}

void PBM::slotViewPBIClicked()
{
	QTreeWidgetItemIterator it(SoftwareListBox);
   	while (*it) {
      		PBI *pbi = dynamic_cast<PBI*>(*it);
      		if ((*it)->isSelected())
      		{
			displayPBI(pbi->getName(), pbi->getRepoID());
			tabPBM->setCurrentIndex(0);
			return;
		}
      		it++;
	}

}

void PBM::slotAddDesktopIcons()
{
	QTreeWidgetItemIterator it(SoftwareListBox);
   	while (*it) {
      		PBI *pbi = dynamic_cast<PBI*>(*it);
      		if ((*it)->isSelected())
      		{
			installDesktopIcon(pbi);
			return;
		}
      		it++;
	}

}

void PBM::slotAddPATHLink()
{
	QTreeWidgetItemIterator it(SoftwareListBox);
   	while (*it) {
      		PBI *pbi = dynamic_cast<PBI*>(*it);
      		if ((*it)->isSelected())
      		{
			installPATHLink(pbi, false);
			return;
		}
      		it++;
	}
}

void PBM::slotAddPATHLinkRoot()
{
	QTreeWidgetItemIterator it(SoftwareListBox);
   	while (*it) {
      		PBI *pbi = dynamic_cast<PBI*>(*it);
      		if ((*it)->isSelected())
      		{
			installPATHLink(pbi, true);
			return;
		}
      		it++;
	}
}

void PBM::slotAddMenuIcons()
{
	QTreeWidgetItemIterator it(SoftwareListBox);
   	while (*it) {
      		PBI *pbi = dynamic_cast<PBI*>(*it);
      		if ((*it)->isSelected())
      		{
			installMenuIcon(pbi, false);
			return;
		}
      		it++;
	}
}

void PBM::slotAddMenuIconsRoot()
{
	QTreeWidgetItemIterator it(SoftwareListBox);
   	while (*it) {
      		PBI *pbi = dynamic_cast<PBI*>(*it);
      		if ((*it)->isSelected())
      		{
			installMenuIcon(pbi, true);
			return;
		}
      		it++;
	}
}

// Installs PATH links for this PBI
void PBM::installPATHLink(PBI *pbi, bool asRoot)
{
  installPBIDesktopIcon = new QProcess();
  if ( wardenDir.isEmpty() ) {
    if ( asRoot ) {
      QString cmd = "pbi_icon add-pathlnk " + pbi->getProgIndexName();
      installPBIDesktopIcon->start(QString("pc-su"), QStringList() << cmd );
    } else {
      installPBIDesktopIcon->start(QString("pbi_icon"), QStringList() << "add-pathlnk " + pbi->getProgIndexName() );
    }
  } else
    installPBIDesktopIcon->start(QString("warden"), QStringList() << "chroot" << wardenIP << "pbi_icon add-pathlnk " + pbi->getProgIndexName() );
}


// Installs desktop icons for this PBI
void PBM::installMenuIcon(PBI *pbi, bool asRoot)
{
  installPBIDesktopIcon = new QProcess();
  if ( wardenDir.isEmpty() ) {
    if ( asRoot ) {
      QString cmd = "pbi_icon add-menu " + pbi->getProgIndexName();
      // + " ; pbi_icon add-mime " + pbi->getProgIndexName();
      installPBIDesktopIcon->start(QString("pc-su"), QStringList() << cmd );
    } else {
      installPBIDesktopIcon->start(QString("pbi_icon"), QStringList() << "add-menu " + pbi->getProgIndexName() + "; pbi_icon add-mime " + pbi->getProgIndexName() );
    }
  } else
    installPBIDesktopIcon->start(QString("warden"), QStringList() << "chroot" << wardenIP << "pbi_icon add-menu " + pbi->getProgIndexName() + "; pbi_icon add-mime " + pbi->getProgIndexName() );
}

// Installs desktop icons for this PBI
void PBM::installDesktopIcon(PBI *pbi)
{
  QString cmd = getPBICmd("pbi_icon add-desktop " + pbi->getProgIndexName());
  qDebug() << "Running:" << cmd;
  system(cmd.toLatin1());
}
