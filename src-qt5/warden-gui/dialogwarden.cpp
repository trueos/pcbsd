/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
****************************************************************************/
#include "dialogwarden.h"
#include <QDate>
#include <QDebug>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QSettings>
#include <QTextStream>
#include <QTime>



#include "dialogworking.h"
#include "dialogconfig.h"
#include "dialogdisplayoutput.h"
#include "dialogimport.h"
#include "dialogTemplates.h"
#include "wizardNewJail.h"
#include "pcbsd-utils.h"

/* Update interval in ms  - Every 5 seconds */
#define  UPDATE_MSEC 1000 * 5

void dialogWarden::helpAbout()
{

}


void dialogWarden::programInit()
{  
   // Disable these until we check if the jail is running
   pushTerminal->setEnabled(false);

   // Remove for now, done by syscache / tray / appcafe
   pushUpdate->setHidden(true);

   // Setup our listview options
   listJails->setSelectionMode(QAbstractItemView::SingleSelection);
   listJails->setAllColumnsShowFocus(true);

   // Run this a single time, just to be sure we did any jail import / migration
   // from legacy jails
   system("warden list");

   // Hide the identifier column / host column
   listJails->setColumnHidden(0, true);
   listJails->setColumnWidth(1, 200);
   listJails->setColumnWidth(2, 200);
   listJails->setColumnWidth(3, 150);
   listJails->setColumnWidth(4, 40);

   groupJailTab->setTitle(tr("<- Please select a jail"));
   groupJailTab->setEnabled(false);
    
   // Read in our configuration data
   readConfig();

   // Start our monitor service to check if jails are running
   checkingStatus = false;
   checkingUpdates = false;
   currentStatusWorkingJail = "";
   currentUpdateWorkingJail = "";
   statusTimer = new QTimer(this);
   connect( statusTimer, SIGNAL(timeout()), this, SLOT( slotMonitorJailStatus() ) );
   slotMonitorJailStatus();
   
   // Start our monitor service for jail details
   checkingDetails = false;
   currentDetailsWorkingJail = "";
   detailsTimer = new QTimer(this);
   connect( detailsTimer, SIGNAL(timeout()), this, SLOT( slotMonitorJailDetails() ) );
   slotMonitorJailDetails();
   
   // Start by refreshing our jail list
   refreshJails();
    
   // Connect the rightclick slot 
   listJails->setContextMenuPolicy( Qt::CustomContextMenu);
   connect( listJails, SIGNAL(customContextMenuRequested (const QPoint & ) ), this, SLOT(slotJailRightClicked() ) );
   connect( listJails, SIGNAL(itemSelectionChanged() ), this, SLOT(slotCurrentJailChanged() ) );
   connect( pushAdd, SIGNAL(clicked()), this, SLOT(slotClickedNewJail() ) );
   connect( pushRemove, SIGNAL(clicked()), this, SLOT(slotDeleteJail() ) );
   connect( pushExportJail, SIGNAL(clicked()), this, SLOT(slotExportJail() ) );
   connect( pushServiceGUI, SIGNAL(clicked()), this, SLOT(slotServiceGUI() ) );
   connect( pushTerminal, SIGNAL(clicked()), this, SLOT(slotTerminal() ) );
   connect( pushUserAdmin, SIGNAL(clicked()), this, SLOT(slotUserAdmin() ) );
   connect( pushConfigure, SIGNAL(clicked()), this, SLOT(slotPushEditIP() ) );
   connect( pushStart, SIGNAL(clicked()), this, SLOT(slotStartJail() ) );

   // Snapshot support
   connect( pushCreateSnap, SIGNAL(clicked()), this, SLOT(slotCreateSnap() ) );
   connect( pushRestoreSnap, SIGNAL(clicked()), this, SLOT(slotRestoreSnap() ) );
   connect( pushRemoveSnap, SIGNAL(clicked()), this, SLOT(slotRemoveSnap() ) );
   //connect( sliderSnaps, SIGNAL(sliderMoved(int)), this, SLOT(slotSnapSliderChanged(int) ) );
   connect( sliderSnaps, SIGNAL(valueChanged(int)), this, SLOT(slotSnapSliderChanged(int) ) );

   // Snapshot cron support
   connect( comboSnapFrequency, SIGNAL(currentIndexChanged(int)), this, SLOT(slotCronSnapshotChanged() ) );
   connect( spinSnapDays, SIGNAL(valueChanged(int)), this, SLOT(slotCronSnapshotChanged() ) );
   connect( groupSnapSchedule, SIGNAL(clicked(bool)), this, SLOT(slotCronSnapshotChanged() ) );

   QTimer::singleShot(100, this, SLOT(slotCheckNic()) );

   // Disable the start button until status's come back
   pushStart->setEnabled(false);
}


void dialogWarden::slotCheckNic() 
{
   QString NIC = pcbsd::Utils::getValFromPCConf("/usr/local/etc/warden.conf", "NIC");
   if ( system("ifconfig " + NIC.toLatin1() + " >/dev/null 2>/dev/null") != 0 )
   {
      // Confirm with user
      int ret = QMessageBox::warning(this, tr("Warden"),
                                tr("No valid network interface specified! Select one now?"),
                                QMessageBox::Yes | QMessageBox::No,
                                QMessageBox::No);
      if ( ret == QMessageBox::No)
         return;

      slotOpenConfig();
   }
}

void dialogWarden::refreshJails()
{
   QString jType, jName, jZFS;
   QStringList jD;
   QStringList mountOut;
 
   update();
    
   listJails->clear();
   jailDetails.clear();
   
   // Check our JailDir and see what we have
   QDir d(JailDir);
   d.setFilter(QDir::Dirs | QDir::Hidden);
   d.setNameFilters(QStringList() << ".*.meta");

   // Start mount cmd
   QProcess m;
   m.start(QString("mount"), QStringList());
   while(m.state() == QProcess::Starting || m.state() == QProcess::Running) {
      m.waitForFinished(200);
      QCoreApplication::processEvents();
   }
   // Get output of mount now
   while (m.canReadLine())
     mountOut << m.readLine().simplified();

   for ( unsigned int i = 0; i < d.count(); i++ ) {
       
       // Traverse our directories, looking for jails
       if ( d[i] == "." || d[i] == ".." )
          continue;

       jD.clear();

       qDebug() << "Checking:" << d[i];

       jName = d[i].replace(".meta", "").remove(0, 1);
	   
       // Check if this jail is created on ZFS
       jZFS="NO";
       for (int z = 0; z < mountOut.size(); ++z)
	  if ( mountOut.at(z).indexOf("on " + JailDir + "/" + jName) != -1 && mountOut.at(z).indexOf("(zfs,") != -1 )
             jZFS="YES";

       // Check for the hostname of this jail
       QFile file( JailDir + "/" + d[i] + "/host" );
       if ( ! file.exists() )
          continue;
       if ( ! file.open( QIODevice::ReadOnly ) )
          continue;
       QTextStream stream( &file );
       QString host;
       while ( !stream.atEnd() )
          host = stream.readLine(); // line of text excluding '\n'
       file.close();
	    
       // Build list of IPs for this jail
       QString jIPs = "";
       QStringList ipFiles;
       ipFiles << "ipv4" << "ipv6";
       for (int f = 0; f < ipFiles.size(); ++f) {
         QFile fileip(JailDir + "/" + d[i] + "/" + ipFiles.at(f) );
         if ( fileip.exists() ) {
           if (fileip.open( QIODevice::ReadOnly ) ) {
              QTextStream stream2( &fileip );
              while ( !stream2.atEnd() )
                jIPs = jIPs + stream2.readLine().simplified() + " "; // line of text excluding '\n'
	   }
           fileip.close();
         }
       }
       qDebug() << jIPs;

       // Check the type of jail
       jType = "Traditional";
       QFile file3(JailDir + "/" + d[i] + "/jail-portjail" );
       if ( file3.exists() )
         jType = "Ports Jail";
       QFile filelin(JailDir + "/" + d[i] + "/jail-linux" );
       if ( filelin.exists() )
         jType = "Linux Jail";

       // Display the jail in the tree widget now
       QStringList cols;
       cols << jName << host;

       QTreeWidgetItem *curItem = new QTreeWidgetItem( listJails, cols );
       if ( ! listJails->currentItem() )
         listJails->setCurrentItem(curItem);

       // Save additional jail details into list
       jD << jName << "Pending" << jType << jIPs << "" << "" << "" << jZFS;
       jailDetails << jD;
   
   } // end of loop
   
   listJails->sortByColumn(1, Qt::AscendingOrder);
   update();

   slotCurrentJailChanged();

   // Now refresh jail details
   slotMonitorJailDetails();
   
}


void dialogWarden::readConfig()
{  
    
    ProgDir = "/usr/local/share/warden";
    TMPDIR = pcbsd::Utils::getValFromPCConf("/usr/local/etc/warden.conf", "WTMP");
    JailDir = pcbsd::Utils::getValFromPCConf("/usr/local/etc/warden.conf", "JDIR");
    
}


void dialogWarden::slotOpenConfig()
{
        // Launch our AddPartitionDialog to add a new device
        configDialog = new dialogConfig();
        connect(configDialog, SIGNAL(saved()),this, SLOT(readConfig()) );
        configDialog->programInit();
        configDialog->exec();

}


void dialogWarden::slotExit()
{
    exit(0);
}

void dialogWarden::slotCheckJailDetails()
{
    QString oldWorking = currentDetailsWorkingJail;
    int found = 0;
    // Mark that we are in the middle of checking the details
    checkingDetails = true;
    
    // Read in our iterator and start checking jail status
    QTreeWidgetItemIterator it( listJails );
    while (*it) {
	
	if ( currentDetailsWorkingJail.isEmpty() ) {
	    currentDetailsWorkingJail = (*it)->text(0);
	    break;
	}
	
	if ( found == 1) {
	  currentDetailsWorkingJail = (*it)->text(0);
	  break;
	}
	
	if ( currentDetailsWorkingJail == (*it)->text(0) )
	    found = 1;
	
        ++it;
    }
    
    
    // Are we ready to take a break?
    if ( currentDetailsWorkingJail == oldWorking ) {
	checkingDetails = false;
	currentDetailsWorkingJail="";
	return;
    }

    // Build list of IPs for this jail
    QString jIPs = "";
    QStringList ipFiles;
    ipFiles << "ipv4" << "ipv6";
    for (int f = 0; f < ipFiles.size(); ++f) {
       QFile fileip(JailDir + "/." + currentDetailsWorkingJail + ".meta/" + ipFiles.at(f) );
       if ( fileip.exists() ) {
         if (fileip.open( QIODevice::ReadOnly ) ) {
            QTextStream stream2( &fileip );
            while ( !stream2.atEnd() )
             jIPs = jIPs + stream2.readLine().simplified() + " "; // line of text excluding '\n'
         }
         fileip.close();
       }
    }
    qDebug() << "Found IPS:" << jIPs;

    // Save the new jail IPs
    for (int i=0; i < jailDetails.count(); ++i) {
      if ( jailDetails.at(i).at(0) != currentDetailsWorkingJail )
         continue;     
      QStringList tmpList = jailDetails.at(i);
      tmpList.replace(3, jIPs);
      jailDetails.replace(i, tmpList);
    }


    // Run the getUpdatesDir.sh script to rsync with master server
    detailsProc = new QProcess( this );
    QString program = ProgDir + "/scripts/backend/details.sh";
    QStringList args;
    args << currentDetailsWorkingJail;

    qDebug() << "Checking for details to: " << currentDetailsWorkingJail;
    
    // Connect the exited signal and start the process
    connect( detailsProc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(slotCheckDetailsReturn() ) );
    detailsProc->start(program, args);
}


void dialogWarden::slotMonitorJailDetails()
{
  if ( checkingDetails == true) {
       // Set our timer to check again
       detailsTimer->start( 1000 * 60 * 10 );
  } else {
      currentDetailsWorkingJail = "";
      slotCheckJailDetails();
      // Set our timer to check again
      detailsTimer->start( 1000 * 60 * 10 );
  }

  if ( checkingUpdates == true) {
       // Set our timer to check again
       detailsTimer->start( 1000 * 60 * 10 );
  } else {
      currentUpdateWorkingJail = "";
      slotCheckForUpdates();
      // Set our timer to check again
      detailsTimer->start( 1000 * 60 * 10 );
  }

}

void dialogWarden::slotCheckForUpdates()
{
    qDebug() << "Starting Update Check...";

    QString oldWorking = currentUpdateWorkingJail;
    checkingUpdates = true;

    int found = 0;

    // Read in our iterator and start checking jail status
    QTreeWidgetItemIterator it( listJails );
    while (*it) {
        if ( currentUpdateWorkingJail.isEmpty() ) {
            currentUpdateWorkingJail = (*it)->text(0);
            break;
        }
        if ( found == 1)
        {
          currentUpdateWorkingJail = (*it)->text(0);
          break;
        }
        if ( currentUpdateWorkingJail == (*it)->text(0) )
            found = 1;
        ++it;
    }
   
    
    // Are we ready to take a break?
    if ( currentUpdateWorkingJail == oldWorking ) {
        checkingUpdates = false;
        currentUpdateWorkingJail="";
        qDebug() << "Finished Update Check...";
        return;
    }

    updateProc = new QProcess( this );
    QString program = "warden";
    QStringList args;
    args << "checkup" << currentUpdateWorkingJail;
    
    // Connect the exited signal and start the process
    connect( updateProc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(slotCheckUpdateReturn() ) );
    updateProc->start(program, args);

}

void dialogWarden::slotCheckUpdateReturn()
{
    bool haveUp = false;
    QString line;

    while (updateProc->canReadLine()) {
        line = updateProc->readLine().simplified();
        qDebug() << "Update output..." << line;
        if ( line.indexOf("The following package updates") == 0 ) {
         haveUp = true;
         break;
        }
        if ( line.indexOf("The following files will be updated") == 0 ) {
         haveUp = true;
         break;
        }
    }

    // Read in our iterator and update jail update status
    QTreeWidgetItemIterator it( listJails );
    while (*it) {
      if ( currentUpdateWorkingJail == (*it)->text(0) )
        break;
      ++it;
    }

    if ( ! (*it) )
       return;


    qDebug() << (*it)->text(0) << haveUp;

    if ( haveUp ) {
      (*it)->setText(4, tr("Updates available!"));
      (*it)->setIcon(4, QIcon(":updateavail.png"));
    } else {
      (*it)->setText(4, "");
      (*it)->setIcon(4, QIcon());
    }

    slotCheckForUpdates();
}

void dialogWarden::slotCheckDetailsReturn()
{
    QString line, size, ports, connections;

    // Read the output and parse
    while ( detailsProc->canReadLine() )
    {
	line = detailsProc->readLine().simplified();
        if ( line.indexOf("Disk Usage: ") == 0 ) {
          line.replace("Disk Usage: ", "");
          size = line;
          continue;
        }
        if ( line.indexOf("Active Ports: ") == 0 ) {
          line.replace("Active Ports: ", "");
          ports = line;
          continue;
        }
        if ( line.indexOf("Current Connections: ") == 0 ) {
          line.replace("Current Connections: ", "");
          connections = line;
          continue;
        }
    }

    // Now save details into our array
    for (int i=0; i < jailDetails.count(); ++i) {
      if ( jailDetails.at(i).at(0) != currentDetailsWorkingJail )
         continue;	
      QStringList tmpList = jailDetails.at(i);

      if ( jailDetails.at(i).at(1) == "Pending" )
         tmpList.replace(1, "Complete");

      tmpList.replace(4, size);
      tmpList.replace(5, ports);
      tmpList.replace(6, connections);
      jailDetails.replace(i, tmpList);
    }
    
    // See if we can update the GUI details
    refreshJailDetailsView();

    // Check the next jail, if there are any more left. 
    slotCheckJailDetails();
}

void dialogWarden::slotCheckJailStatus()
{
    QString oldWorking = currentStatusWorkingJail;
    int found = 0;
    // Mark that we are in the middle of checking the status
    checkingStatus = true;
    
    // Read in our iterator and start checking jail status
    QTreeWidgetItemIterator it( listJails );
    while (*it) {
	
	if ( currentStatusWorkingJail.isEmpty() ) {
	    currentStatusWorkingJail = (*it)->text(0);
	    break;
	}
	
	if ( found == 1)
	{
	  currentStatusWorkingJail = (*it)->text(0);
	  break;
	}
	
	if ( currentStatusWorkingJail == (*it)->text(0) )
	{
	    found = 1;
	}
	
	
        ++it;
    }
    
    
    // Are we ready to take a break?
    if ( currentStatusWorkingJail == oldWorking ) {
	checkingStatus = false;
	currentStatusWorkingJail="";
	return;
    }

    // Run the getUpdatesDir.sh script to rsync with master server
    statusProc = new QProcess( this );
    QString program = ProgDir + "/scripts/backend/checkstatus.sh";
    QStringList args;
    args << currentStatusWorkingJail;
    
    // Connect the exited signal and start the process
    connect( statusProc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(slotCheckStatusReturn() ) );
    statusProc->start(program, args);
}


void dialogWarden::slotMonitorJailStatus()
{
    if ( checkingStatus == true) {
       // Set our timer to check again
       statusTimer->start( UPDATE_MSEC );
  } else {
      currentStatusWorkingJail = "";
      slotCheckJailStatus();
      // Set our timer to check again
      statusTimer->start( UPDATE_MSEC );
  }

}


void dialogWarden::slotCheckStatusReturn()
{

    // Check the exit status
    int exitStatus = statusProc->exitCode();
    bool running;
    
        // Read in our iterator and locate the jail, changing its status
    QTreeWidgetItemIterator it( listJails );
    while (*it) {
	if ( currentStatusWorkingJail == (*it)->text(0) )
	{
	    if ( exitStatus == 0) {
                running = true;
		(*it)->setIcon(1, QIcon(":running.png"));
	        (*it)->setText(2, "Running");
	    } else {
                running = false;
		(*it)->setIcon(1, QIcon(":stopped.png"));
	        (*it)->setText(2, "Not Running");
	    }

	    // See if we can update push buttons
            if ( listJails->currentItem()) {
              if ( listJails->currentItem()->text(0) == currentStatusWorkingJail ) {
		if ( running ) {
		  pushTerminal->setEnabled(true);
		  pushStart->setEnabled(true);
		  pushStart->setText(tr("&Stop Jail"));
		  pushStart->setIcon(QIcon(":stopjail.png"));
		  pushStart->setIconSize(QSize(16,16));
		  pushStart->setToolTip(tr("Stop the selected jail"));
		} else {
		  pushTerminal->setEnabled(false);
		  pushStart->setText(tr("&Start Jail"));
		  pushStart->setEnabled(true);
		  pushStart->setIcon(QIcon(":running.png"));
		  pushStart->setIconSize(QSize(16,16));
		  pushStart->setToolTip(tr("Start the selected jail"));
		}
              }
	    }
	    break;
	}
        ++it;
    }
    
    // Check the next jail, if there are any more left. 
    slotCheckJailStatus();
}




void dialogWarden::slotJailRightClicked()
{
    QString Status, Updates;

    if ( listJails->currentItem()) {
          popupip = listJails->currentItem()->text(0);
          Status = listJails->currentItem()->text(2);
          Updates = listJails->currentItem()->text(3);
	  popup = new QMenu();
	  if ( Status == "Running" )
	      popup->addAction( tr("Stop this Jail") , this, SLOT(slotStopJail() )  );
	  else
	      popup->addAction( tr("Start this Jail") , this, SLOT(slotStartJail() )  );
	  popup->addSeparator();
	  if ( ! Updates.isEmpty() )
	      popup->addAction( tr("Start update manager") , this, SLOT(slotUpdate() )  );
	  popup->addAction( tr("Toggle Autostart") , this, SLOT(slotToggleAutostartClicked() )  );
	  popup->addAction( tr("Export jail to .wdn file") , this, SLOT(slotExportJail() )  );
	  popup->addAction( tr("Clone this Jail") , this, SLOT(slotMakeClone() )  );
	  popup->addSeparator();
	  popup->addAction( tr("Delete Jail") , this, SLOT(slotDeleteJail() )  );
	  popup->exec( QCursor::pos() );


    }

    
}


void dialogWarden::slotStopJail()
{
        if ( !listJails->currentItem())
          return;

        QString tmp;
        popupip = listJails->currentItem()->text(0);
	
        // Launch our working dialog to prevent further action until we are finished
        workingDialog = new dialogWorking();
        workingDialog->setModal(true);
        workingDialog->programInit();
        tmp = tr("Stopping Jail");
        workingDialog->setDialogTitle(tmp);
        tmp = tr("Stopping Jail:") + " " + popupip;
        workingDialog->setDialogText(tmp);
        workingDialog->show();
	
	
      // Now start the script to stop this jail
      stopJailProc = new QProcess( this );
      QString program = "warden";
      QStringList args;
      args << "stop" <<popupip;
      
      // Connect the exited signal and start the process
      connect( stopJailProc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(slotFinishedWorking() ) );
      stopJailProc->start(program, args);
      pushStart->setEnabled(false);
	
}


void dialogWarden::slotStartJail()
{
    if ( ! listJails->currentItem() )
      return;

    popupip = listJails->currentItem()->text(0);
    

    // If jail is running, lets stop it
    if ( listJails->currentItem()->text(2) == "Running" ) {
      slotStopJail();
       return;
    }

    QString tmp;
	
    // Launch our working dialog to prevent further action until we are finished
    workingDialog = new dialogWorking();
    workingDialog->setModal(true);
    workingDialog->programInit();
    tmp = tr("Starting Jail");
    workingDialog->setDialogTitle(tmp);
    tmp = tr("Starting Jail:") + " " + popupip;
    workingDialog->setDialogText(tmp);
    workingDialog->show();
	
	
    // Now start the script to stop this jail
    startJailProc = new QProcess( this );
    QString program =  "warden";
    QStringList args;
    args << "start" << popupip;
      
    // Connect the exited signal and start the process
    connect( startJailProc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(slotFinishedWorking() ) );
    startJailProc->start(program, args);
    pushStart->setEnabled(false);
}



void dialogWarden::slotExportJail()
{
    QString exportFile, tmp;

    if ( ! listJails->currentItem() )
      return;

    popupip = listJails->currentItem()->text(0);
    
    exportFile = QFileDialog::getExistingDirectory(
                    this,
		    tr("Choose a directory to save the finished .wdn file:"),
                    "/root",
                    QFileDialog::ShowDirsOnly );
    
    if ( exportFile.isEmpty() )
	return;
    
    // Lauch the command output dialog
    dialogOutput = new dialogDisplayOutput();
    dialogOutput->setModal(true);
    dialogOutput->programInit(false);
    dialogOutput->setDialogCaption(tr("Exporting Jail:") + " " + popupip);
    dialogOutput->setDialogText("");
    tmp = tr("Exporting Jail:") + " " + popupip + "\n";
    dialogOutput->appendDialogText(tmp);
    dialogOutput->show();
    
    // Now start the script to stop this jail
    exportJailProc = new QProcess( this );
    QString program = "warden";
    QStringList args;
    args << "export" << popupip << "--dir=" + exportFile;
      
    // Connect the exited signal and start the process 
    connect( exportJailProc, SIGNAL(readyReadStandardOutput()), this, SLOT(slotReadExportOutput() ) );
    connect( exportJailProc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(slotFinishedExport() ) );
    exportJailProc->start(program, args);
   
}

void dialogWarden::slotFinishedExport()
{
  dialogOutput->setCloseHide( false );
}

void dialogWarden::slotListJailPkgs()
{
    if ( ! listJails->currentItem() )
      return;

    popupip = listJails->currentItem()->text(0);
    
    dialogOutput = new dialogDisplayOutput();
    dialogOutput->setModal(true);
    dialogOutput->programInit(true);
    dialogOutput->setDialogCaption("Jail Packages: " + popupip);
    dialogOutput->setDialogText("");
    dialogOutput->show();
    
    // Now start the script to stop this jail
    listPackagesProc = new QProcess( this );
    QString program = ProgDir + "/scripts/backend/listpkgs.sh";
    QStringList args;
    args << popupip;

    listPackagesProc->setProcessChannelMode(QProcess::MergedChannels);
    listPackagesProc->setReadChannel(QProcess::StandardOutput);

    // Connect the exited signal and start the process 
    connect( listPackagesProc, SIGNAL(readyReadStandardOutput ()), this, SLOT(slotReadPkgsOutput() ) );

    listPackagesProc->start(program, args);
}


void dialogWarden::slotDeleteJail()
{
    QString tmp;

    if ( ! listJails->currentItem() )
      return;

    popupip = listJails->currentItem()->text(0);

    // Confirm with user
    int ret = QMessageBox::warning(this, tr("Warden"),
                                tr("Are you sure you want to delete the jail") + ": " + popupip,
                                QMessageBox::Yes | QMessageBox::No,
                                QMessageBox::No);
    if ( ret == QMessageBox::No)
      return;
    
    // Launch our working dialog to prevent further action until we are finished
    workingDialog = new dialogWorking();
    workingDialog->setModal(true);
    workingDialog->programInit();
    tmp = tr("Deleting Jail");
    workingDialog->setDialogTitle(tmp);
    tmp = tr("Deleting Jail:") + " " + popupip;
    workingDialog->setDialogText(tmp);
    workingDialog->show();
  
    // Now start the script to stop this jail
    deleteJailProc = new QProcess( this );
    QString program = ProgDir + "/scripts/backend/deletejail.sh";
    QStringList args;
    args << popupip;
    
    // Connect the exited signal and start the process 
    connect( deleteJailProc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(refreshJails() ) );
    connect( deleteJailProc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(slotFinishedWorking() ) );
    deleteJailProc->start(program, args);
}




void dialogWarden::slotFinishedWorking()
{
        // Finished whatever we were working on, hide the dialog now. 
        workingDialog->hide();
	slotCurrentJailChanged();
}


void dialogWarden::runCommand( QString command )
{
        FILE *file = popen(command.toLatin1(),"r");
        fclose(file);
}


void dialogWarden::slotReadExportOutput()
{
    while ( exportJailProc->canReadLine() )
	dialogOutput->appendDialogText(exportJailProc->readLine());
}


void dialogWarden::slotFinishedOutput()
{
    // Hide the output dialog, not needed anymore
    dialogOutput->hide();
}


void dialogWarden::slotReadPkgsOutput()
{
    
    while ( listPackagesProc->canReadLine() )
    {
	dialogOutput->appendDialogText(listPackagesProc->readLine());
    }
}


void dialogWarden::slotClickedNewJail()
{
      QStringList uH, uI;

       // Launch our working dialog to prevent further action until we are finished
      newJailWizard = new wizardNewJail();
      newJailWizard->setModal(true);
      newJailWizard->programInit();

      // Build list of used jail IP / Hosts
      QTreeWidgetItemIterator it( listJails );
      int i = 0;
      while (*it) {
         uH << (*it)->text(0);
         uI << jailDetails.at(i).at(3);
         ++it;
         i++;
      }
      newJailWizard->setHostIPUsed(uH, uI);
      
      connect( newJailWizard, SIGNAL(create(const QString &, const QString &, const QString &, bool, bool, const QString &, bool, bool, bool, bool, const QString &, const QString &)), this, SLOT(slotCreateNewJail(const QString &, const QString &, const QString &, bool, bool, const QString &, bool, bool, bool, bool, const QString &, const QString &) ) );
      newJailWizard->show();
}


void dialogWarden::slotCreateNewJail( const QString &IP, const QString &IP6, const QString &host, bool tradjail, bool PCUtils, const QString &rootpw, bool src, bool ports, bool autostart, bool linuxJail, const QString &linuxScript, const QString &tplate )
{
    if ( tradjail || linuxJail )
      newRootPW=rootpw;
    else
      newRootPW="";
    
    newHost=host;

    /***********************************************************************************************************************
        Now create the jail here
    *************************************************************************************************************************/
      dialogOutput = new dialogDisplayOutput();
      dialogOutput->setModal(false);
      dialogOutput->programInit(false);
      dialogOutput->setDialogCaption("Creating Jail: " + IP);
      dialogOutput->setDialogText("");
      dialogOutput->show();
    
    
      // Now start the script to stop this jail
      createJailProc = new QProcess( this );
      QString program = "warden";
      QStringList args;
      args << "create" << host;

      QString newIP = IP;
     
      if ( IP.indexOf("/") == -1 )
         newIP = IP + "/24";

      // Set our optional flags
      if ( ! newIP.isEmpty() ) {
        args << "--ipv4=" + newIP;
      }

      if ( ! IP6.isEmpty() ) {
        args << "--ipv6=" + IP6;
      }

      if ( ! linuxJail && ! tplate.isEmpty() )
        args << "--template" << tplate.section(" ", 0, 0);

      if ( tradjail && ! PCUtils )
        args << "--vanilla";

      if ( ! tradjail && ! linuxJail )
        args << "--portjail";

      if ( src && ! linuxJail)
	args << "--src";

      if ( ports && ! linuxJail )
	args <<  "--ports";

      if ( linuxJail )
	args <<  "--linuxjail" << linuxScript ;

      if ( autostart )
	args << "--startauto";

      // Connect the exited signal and start the process 
      createJailProc->setProcessChannelMode(QProcess::MergedChannels);
      createJailProc->setReadChannel(QProcess::StandardOutput);
      connect( createJailProc, SIGNAL(readyReadStandardOutput ()), this, SLOT(slotReadCreateOutput() ) );
      connect( createJailProc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(slotFinishedJailCreate() ) );
      createJailProc->start(program, args);
    
}


void dialogWarden::slotReadCreateOutput()
{
    while ( createJailProc->canReadLine() )
    {
	dialogOutput->appendDialogText(createJailProc->readLine());
    }
}


void dialogWarden::slotFinishedJailCreate()
{
    // If this doesn't need a root pw, or doesn't have one
    if ( newRootPW.isEmpty() ) {
      slotFinishedUserAdd();
      return;
    }

    // Jail is done creating, now set rootpw
    userAddProc = new QProcess( this );
    QString program = ProgDir + "/scripts/backend/setupusers.sh";
    QStringList args;
    args << newHost << newRootPW;
      
    // Connect the exited signal and start the process 
    userAddProc->setProcessChannelMode(QProcess::MergedChannels);
    userAddProc->setReadChannel(QProcess::StandardOutput);
    connect( userAddProc, SIGNAL(readyReadStandardOutput()), this, SLOT(slotReadUserAddOutput() ) );
    connect( userAddProc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(slotFinishedUserAdd() ) );
    userAddProc->start(program, args);

}


void dialogWarden::slotReadUserAddOutput()
{
    while ( userAddProc->canReadLine() )
    {
	dialogOutput->appendDialogText(userAddProc->readLine());
    }
}


void dialogWarden::slotFinishedUserAdd()
{
    // All finished with creating a jail and adding a username to it
    
    // Unhide the close button
    dialogOutput->setCloseHide(false);

    // Refresh our jail list
    refreshJails();
}


void dialogWarden::slotImportJailClicked()
{
      // The user wants to import a jail, lets prompt for the .wdn file to use
      importFile="";
    
      importFile = QFileDialog::getOpenFileName(
                    this,
                    tr("Select warden jail package"),
                    "/home",
                    "Warden Jail Packs (*.wdn)");
      
      if ( importFile.isEmpty() )
	  return;

      // Bring up dialog to ask about import settings
      importDialog = new dialogImport();
      importDialog->programInit();
      connect( importDialog, SIGNAL(import(const QString &, const QString &, const QString &)), this, SLOT(slotImportConfigDone(const QString &, const QString &, const QString &) ) );
      importDialog->show();

}

void dialogWarden::slotImportConfigDone(const QString &JailName, const QString &IP, const QString &Host)
{

      // Display the import dialog output
      dialogOutput = new dialogDisplayOutput();
      dialogOutput->setModal(false);
      dialogOutput->programInit(false);
      dialogOutput->setDialogCaption("Importing Jail");
      dialogOutput->setDialogText("");
      dialogOutput->show();

      importJailProc = new QProcess( this );
      QString program = "warden";
      QStringList args;
      args << "import" << JailName << importFile;
      if ( ! IP.isEmpty() )
        args << "--ipv4=" + IP + "/24";
      if ( ! Host.isEmpty() )
        args << "--host=" + Host;

      qDebug() << args;
      
      // Connect the exited signal and start the process 
      importJailProc->setProcessChannelMode(QProcess::MergedChannels);
      importJailProc->setReadChannel(QProcess::StandardOutput);
      connect( importJailProc, SIGNAL(readyReadStandardOutput ()), this, SLOT(slotReadImportOutput() ) );
      connect( importJailProc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(slotFinishedJailImport()));
      importJailProc->start(program, args);

}


void dialogWarden::slotReadImportOutput()
{
    while ( importJailProc->canReadLine() )
    {
	dialogOutput->appendDialogText(importJailProc->readLine());
    }
}


void dialogWarden::slotFinishedJailImport()
{
    // All finished with importing a jail
    
    // Unhide the close button
    dialogOutput->setCloseHide(false);
    
       // Refresh our jail list
    refreshJails();
}

void dialogWarden::slotToggleAutostartClicked()
{
    // Change auto-start status of the jail
    //runCommand("warden auto " + popupip);
    
    toggleAutoProc = new QProcess( this );
    QString program = "warden";
    QStringList args;
    args << "auto" << popupip;

      
    // Connect the exited signal and start the process 
    connect( toggleAutoProc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(refreshJails() ) );
    toggleAutoProc->start(program, args);
    
}

void dialogWarden::refreshJailDetailsView()
{
   if ( ! listJails->currentItem() )
     return;

   QString tmp;

   // Load the details for this jail
   for (int i=0; i < jailDetails.count(); ++i) {
     if ( jailDetails.at(i).at(0) != listJails->currentItem()->text(0) )
        continue;

     // Depending upon the type of jail, we may need to hide stuff
     if ( jailDetails.at(i).at(2) == "Linux Jail" ) {
	pushServiceGUI->setHidden(true);
	pushUserAdmin->setHidden(true);
	tabJail->setTabEnabled(3, false);
     } else {
	pushServiceGUI->setHidden(false);
	pushUserAdmin->setHidden(false);
	tabJail->setTabEnabled(3, true);
     }

     // Load the details now
     labelType->setText(jailDetails.at(i).at(2));
     labelIPs->setText(jailDetails.at(i).at(3));

     // Not all details available, stop here
     if ( jailDetails.at(i).at(1) == "Pending" ) {
        labelSize->setText(tr("Loading..."));
        labelPorts->setText(tr("Loading..."));
        labelConnections->setText(tr("Loading..."));
        break;
     }

     labelSize->setText(jailDetails.at(i).at(4));
     tmp =jailDetails.at(i).at(5);
     tmp.replace(",", " " );
     labelPorts->setText(tmp);
     labelConnections->setText(jailDetails.at(i).at(6));

     // Is this jail ZFS?
     if ( jailDetails.at(i).at(7) == "YES" ) {
        groupSnaps->setEnabled(false); 
        labelSnap->setText(tr("Loading snapshots..."));
        QTimer::singleShot(50, this, SLOT(slotLoadSnapshots()) );
     } else {
	groupSnaps->setEnabled(false); 
	groupSnapSchedule->setEnabled(false); 
	labelSnap->setText(tr("For snapshot support you must be using ZFS"));
     }

     // Done loading all details
     break;
   }

}

void dialogWarden::slotLoadSnapshots()
{
   if ( ! listJails->currentItem() )
      return;

   bool ok, ok2;

   groupSnaps->setEnabled(false); 
   groupSnapSchedule->setEnabled(false); 
   labelSnap->setText(tr("Loading snapshots..."));
   disconnect(spinSnapDays, SIGNAL(valueChanged(int)), 0, 0);
   disconnect(comboSnapFrequency, SIGNAL(currentIndexChanged(int)), 0, 0);
   // Load the snapshot schedule
   QFile file(JailDir + "/." + listJails->currentItem()->text(0) + ".meta/cron" );
   if ( file.exists() && file.open( QIODevice::ReadOnly ) ) {
      QTextStream streamfreq( &file );
      if ( streamfreq.readLine().simplified() == "hourly" )
         comboSnapFrequency->setCurrentIndex(1);
      else
         comboSnapFrequency->setCurrentIndex(0);
      file.close();

     // Now see how many are set to keep
     QFile file2(JailDir + "/." + listJails->currentItem()->text(0) + ".meta/cron-keep" );
     if ( file2.exists() && file2.open( QIODevice::ReadOnly ) ) {
        QTextStream streamkeep( &file2 );
	int keep = streamkeep.readLine().toInt(&ok);
        if ( ok )
            spinSnapDays->setValue(keep);
     }
     groupSnapSchedule->setChecked(true); 
   } else {
     comboSnapFrequency->setCurrentIndex(0);
     spinSnapDays->setValue(10);
     groupSnapSchedule->setChecked(false); 
   }

   groupSnapSchedule->setEnabled(true); 
   connect( comboSnapFrequency, SIGNAL(currentIndexChanged(int)), this, SLOT(slotCronSnapshotChanged() ) );
   connect( spinSnapDays, SIGNAL(valueChanged(int)), this, SLOT(slotCronSnapshotChanged() ) );
   // Done loading snapshot schedule

   QString IP = listJails->currentItem()->text(0);

   // Grab the ZFS snapshot list
   QProcess m;
   m.start(QString("warden"), QStringList() << "snap" << "list" << IP );
   while(m.state() == QProcess::Starting || m.state() == QProcess::Running) {
      m.waitForFinished(200);
      QCoreApplication::processEvents();
   }

   // Check if the user changed jails before we finished loading snaps
   if ( ! listJails->currentItem() )
      return;
   if ( IP != listJails->currentItem()->text(0))
     return;

   snapshotList.clear();
   snapshotComments.clear();
   QString tmp, line, comment;

   qDebug() << "Getting ZFS snapshots for " + IP;

   // Get output of ZFS snapshots
   while (m.canReadLine()) {
      line = m.readLine().simplified();
      tmp = line;
      tmp = tmp.section(" ", 0, 0);
      tmp.section("-", 0, 0).toInt(&ok);
      tmp.section("-", 1, 1).toInt(&ok2);
      comment = line;
      comment = comment.section(" ", 1, -1);
      if ( comment == "-" )
         comment = tr("No label");
      if (ok && ok2) {
         snapshotList << tmp;
         snapshotComments << comment;
      }
   }

   qDebug() << "Available Snapshots:" << snapshotList;

   // If this jail has no snapshots, show that
   if ( snapshotList.count() <= 0 ) {
     labelSnap->setText(tr("No snapshots available. You may create one below."));
     groupSnaps->setEnabled(true); 
     sliderSnaps->setEnabled(false);
     pushRestoreSnap->setEnabled(false);
     pushRemoveSnap->setEnabled(false);
     return;
   }

   // List snapshots
   sliderSnaps->setRange(0, snapshotList.count() - 1 );
   sliderSnaps->setValue(snapshotList.count() -1 );
   tmp="";
 
   labelSnap->setText(getSnapDateReadable(snapshotList.at(snapshotList.count()-1)) + " ("+ snapshotComments.at(snapshotList.count()-1) + ") " + tmp);
   sliderSnaps->setEnabled(true);
   pushRestoreSnap->setEnabled(true);
   pushRemoveSnap->setEnabled(true);
   groupSnaps->setEnabled(true); 

}

QString dialogWarden::getSnapDateReadable(QString time)
{
   QDate date;
   QTime timestr;
   if ( time.count('-') == 1 ) {
     // Old Style
     date = QDate::fromString(time.section("-", 0, 0), "yyyyMMdd");
     timestr = QTime::fromString(time.section("-", 1, 1), "hhmmss");
   } else {
     // New Style
     date = QDate::fromString(time.section("-", 0, 2), "yyyy-MM-dd");
     timestr = QTime::fromString(time.section("-", 3, 5), "hh-mm-ss");
   }
   
   return QString(date.toString("ddd MMMM d',' yyyy") + " @ " + timestr.toString("h:mm:ss ap"));
}

void dialogWarden::slotCurrentJailChanged()
{
   if ( ! listJails->currentItem() ) {

     groupJailTab->setEnabled(false);
     groupJailTab->setTitle(tr("No jail selected!"));
     labelSize->setText("");
     labelPorts->setText("");
     labelConnections->setText("");
     labelType->setText("");
     labelIPs->setText("");
     labelStartAtBoot->setText("");
     pushStart->setEnabled(false);
     return;
   }

   currentSnapshot = -1;
   //pushStart->setEnabled(false);

   if ( listJails->currentItem()->text(2) == "Running" ) {
     //pushStart->setEnabled(true);
     pushTerminal->setEnabled(true);
     pushStart->setText(tr("&Stop Jail"));
     pushStart->setIcon(QIcon(":stopjail.png"));
     pushStart->setIconSize(QSize(16,16));
     pushStart->setToolTip(tr("Stop the selected jail"));
   }
   if ( listJails->currentItem()->text(2) == "Not Running" ) {
     //pushStart->setEnabled(true);
     pushTerminal->setEnabled(false);
     pushStart->setText(tr("&Start Jail"));
     pushStart->setIcon(QIcon(":running.png"));
     pushStart->setIconSize(QSize(16,16));
     pushStart->setToolTip(tr("Start the selected jail"));
   }

   groupJailTab->setTitle(tr("Working on jail:") + " " + listJails->currentItem()->text(0));
   groupJailTab->setEnabled(true);

   QString AutoStart = "Disabled";
   QFile file2(JailDir + "/." + listJails->currentItem()->text(0) + ".meta/autostart" );
   if ( file2.exists() )
      AutoStart = "Enabled";
   labelStartAtBoot->setText(AutoStart);

   refreshJailDetailsView();

}

void dialogWarden::slotUserAdmin()
{
   if ( ! listJails->currentItem() )
     return;
   QString cmd = "pc-usermanager -warden /usr/jails/" + listJails->currentItem()->text(0) + " " + listJails->currentItem()->text(0) + " &"; 
   system(cmd.toLatin1());
   
}

void dialogWarden::slotServiceGUI()
{
   if ( ! listJails->currentItem() )
     return;
   QString cmd = "pc-servicemanager -warden /usr/jails/" + listJails->currentItem()->text(0) + " " + listJails->currentItem()->text(0) + " &"; 
   system(cmd.toLatin1());
}

void dialogWarden::slotTerminal()
{
   if ( ! listJails->currentItem() )
     return;

   pcbsd::Utils::runInTerminal(QString("warden chroot ") + listJails->currentItem()->text(0),
                        QString("Jail: ") + listJails->currentItem()->text(0));
}

void dialogWarden::slotPushEditIP()
{

   if ( ! listJails->currentItem() )
     return;

   dIP = new dialogEditIP();
   connect(dIP, SIGNAL(saved()),this, SLOT(slotMonitorJailDetails()) );
   dIP->programInit(listJails->currentItem()->text(0));
   dIP->exec();

}

void dialogWarden::slotPushPackage()
{
   
   if ( ! listJails->currentItem() )
     return;
   QString cmd = "appcafe -jail " + listJails->currentItem()->text(0) + " &"; 
   system(cmd.toLatin1());
}


void dialogWarden::slotUpdate()
{
   
   if ( ! listJails->currentItem() )
     return;
   QString cmd = "pc-updategui -warden /usr/jails/" + listJails->currentItem()->text(0) + " " + listJails->currentItem()->text(0) + " &"; 
   system(cmd.toLatin1());
}

void dialogWarden::slotMakeClone()
{
   if ( ! listJails->currentItem() )
      return;


   bool ok;
   QString newJail = QInputDialog::getText(this, tr("Enter hostname for cloned jail"),
                                          tr("Cloned jail hostname:"), QLineEdit::Normal,
                                          "", &ok, 0, Qt::ImhUrlCharactersOnly);
   if ( ! ok || newJail.isEmpty() )
      return;
   

   QString IP = listJails->currentItem()->text(0);

   // Now start the script to stop this jail
   createJailProc = new QProcess( this ); 
   QString program = "warden";
   QStringList args;
   args << "clone" << IP << newJail;

   // Connect the exited signal and start the process
   createJailProc->setProcessChannelMode(QProcess::MergedChannels);
   createJailProc->setReadChannel(QProcess::StandardOutput);
   connect( createJailProc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(refreshJails() ) );
   createJailProc->start(program, args);

}

void dialogWarden::slotCreateSnap()
{
   if ( ! listJails->currentItem() )
      return;

   currentSnapshot = -1;

   // Update status
   groupSnaps->setEnabled(false); 
   labelSnap->setText(tr("Creating new snapshot... Please wait..."));

   QString IP = listJails->currentItem()->text(0);

   // Ask user for comment to this snapshot
   bool ok;
   QString comment = QInputDialog::getText (this, tr("Snapshot comment"), tr("Snapshot comment"), QLineEdit::Normal, "GUI snapshot", &ok);
   if ( ! ok )
      comment = "GUI snapshot";

   // Now start the script to stop this jail
   createJailProc = new QProcess( this ); 
   QString program = "warden";
   QStringList args;
   args << "snap" << "make" << IP << comment;

   // Connect the exited signal and start the process
   createJailProc->setProcessChannelMode(QProcess::MergedChannels);
   createJailProc->setReadChannel(QProcess::StandardOutput);
   connect( createJailProc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(slotLoadSnapshots() ) );
   createJailProc->start(program, args);

}

void dialogWarden::slotRestoreSnap()
{
   // Update status
   currentSnapshot = -1;

   QString IP = listJails->currentItem()->text(0);

   int ret = QMessageBox::question(this, tr("Warden"),
                                tr("Are you sure you want to revert to the snapshot:") + "\n" + getSnapDateReadable(snapshotList.at(sliderSnaps->value() ) ) + "?" + "\n" + tr("Any newer snapshots will be lost, and the jail will be restarted!"),
                                QMessageBox::Yes | QMessageBox::No,
                                QMessageBox::No);
   if ( ret != QMessageBox::Yes )
      return;

   groupSnaps->setEnabled(false); 
   labelSnap->setText(tr("Reverting snapshot... Please wait..."));

   dialogOutput = new dialogDisplayOutput();
   dialogOutput->setModal(true);
   dialogOutput->programInit(false);
   dialogOutput->setDialogCaption(tr("Reverting snapshot"));
   dialogOutput->setDialogText(tr("Reverting to snapshot:") + " " + getSnapDateReadable(snapshotList.at(sliderSnaps->value() ) ));
   dialogOutput->show();
    

   // Revert the snapshot
   createJailProc = new QProcess( this ); 
   QString program = "warden";
   QStringList args;
   args << "snap" << "revert" << IP << snapshotList.at(sliderSnaps->value());

   // Connect the exited signal and start the process
   createJailProc->setProcessChannelMode(QProcess::MergedChannels);
   createJailProc->setReadChannel(QProcess::StandardOutput);
   connect( createJailProc, SIGNAL(readyReadStandardOutput ()), this, SLOT(slotReadCreateOutput() ) );
   connect( createJailProc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(slotShowDialogCloseButton() ) );
   connect( createJailProc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(slotLoadSnapshots() ) );
   createJailProc->start(program, args);


}

void dialogWarden::slotRemoveSnap()
{

   QString IP = listJails->currentItem()->text(0);

   currentSnapshot = -1;

   int ret = QMessageBox::question(this, tr("Warden"),
                                tr("Are you sure you want to remove the snapshot:") + "\n" + getSnapDateReadable(snapshotList.at(sliderSnaps->value() ) ) + "?",
                                QMessageBox::Yes | QMessageBox::No,
                                QMessageBox::No);
   if ( ret != QMessageBox::Yes )
      return;

   // Update status
   groupSnaps->setEnabled(false); 
   labelSnap->setText(tr("Removing snapshot... Please wait..."));

   // Remove the snapshot
   createJailProc = new QProcess( this ); 
   QString program = "warden";
   QStringList args;
   args << "snap" << "remove" << IP << snapshotList.at(sliderSnaps->value());

   // Connect the exited signal and start the process
   createJailProc->setProcessChannelMode(QProcess::MergedChannels);
   createJailProc->setReadChannel(QProcess::StandardOutput);
   connect( createJailProc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(slotLoadSnapshots() ) );
   createJailProc->start(program, args);

}


void dialogWarden::slotSnapSliderChanged(int newVal)
{  
   QString IP = listJails->currentItem()->text(0);
   QString tmp;
   pushRemoveSnap->setEnabled(true);
   labelSnap->setText(getSnapDateReadable(snapshotList.at(newVal)) + " (" + snapshotComments.at(newVal) + ") " + tmp);
}

void dialogWarden::slotShowDialogCloseButton()
{
    // Unhide the close button
    dialogOutput->setCloseHide(false);
}

void dialogWarden::slotCronSnapshotChanged()
{
   QString IP = listJails->currentItem()->text(0);

   if ( groupSnapSchedule->isChecked() ) {
     qDebug() << "Enabling cron snapshots";
     QProcess m;
     m.start(QString("warden"), QStringList() << "cronsnap" << IP << "start" << comboSnapFrequency->currentText() << QString::number(spinSnapDays->value()) );
     while(m.state() == QProcess::Starting || m.state() == QProcess::Running) {
        m.waitForFinished(200);
        QCoreApplication::processEvents();
     }
   } else {
     qDebug() << "Disabling cron snapshots";
     QProcess m;
     m.start(QString("warden"), QStringList() << "cronsnap" << IP << "stop" );
     while(m.state() == QProcess::Starting || m.state() == QProcess::Running) {
        m.waitForFinished(200);
        QCoreApplication::processEvents();
     }
   }

}

void dialogWarden::slotSingleInstance()
{
  this->hide();
  this->showNormal();
  this->activateWindow();
  this->raise();
}

void dialogWarden::slotTemplateOpen()
{
   tempDialog = new dialogTemplates();
   tempDialog->programInit();
   tempDialog->exec();
}
