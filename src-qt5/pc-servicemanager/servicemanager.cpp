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
*****************************************************************************/
#include <QDebug>
#include <QTimer>
#include <QTreeWidgetItem>
#include <QTextStream>
#include <pcbsd-utils.h>

#include "ui_servicemanager.h"
#include "servicemanager.h"

void ServiceManager::ProgramInit(QString chroot, QString IP)
{
    // Set any warden stuff
    wDir = chroot;
    wIP = IP;

    listServices->setColumnHidden(0, true);
    listServices->setColumnHidden(1, true);
    
    pushStart->setEnabled(false);
    pushStop->setEnabled(false);
    pushRestart->setEnabled(false);
    pushEnableStartup->setEnabled(false);
    pushDisableStartup->setEnabled(false);
    
     // Connect the buttons
    connect( pushStart, SIGNAL( clicked() ), this, SLOT( startSlot() ) );
    connect( pushStop, SIGNAL( clicked() ), this, SLOT( stopSlot() ) );
    connect( pushRestart, SIGNAL( clicked() ), this, SLOT( restartSlot() ) );
    connect( pushEnableStartup, SIGNAL( clicked() ), this, SLOT( enableSlot() ) );
    connect( pushDisableStartup, SIGNAL( clicked() ), this, SLOT( disableSlot() ) );
    connect( push_close, SIGNAL(clicked()), this, SLOT(close()) );
    
    // Connect the list box
    connect( listServices, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)), this, SLOT( listSelectionChanged() ) );
     
    // Start loading the various service files
    populateList();
    
    // Start checking the status of these services
    currentCheckRunningItem = new QTreeWidgetItemIterator(listServices);
    QTimer::singleShot(1000, this, SLOT(checkRunning()));
    
    // Start checking if services are enabled
    checkEnabled();

    if ( ! wDir.isEmpty() )
      textTopLabel->setText(tr("Managing services for Warden Jail:") + " " +  wIP);
}

void ServiceManager::setButtonsAllEnabled(bool enabled)
{
  pushStart->setEnabled(enabled); 
  pushStop->setEnabled(enabled); 
  pushRestart->setEnabled(enabled); 
  pushEnableStartup->setEnabled(enabled); 
  pushDisableStartup->setEnabled(enabled); 

}

void ServiceManager::startSlot()
{
    QTreeWidgetItem *item = listServices->currentItem();
      if (!item)
	return;
	  
    workingTreeWidgetItem = item;	 
	  
    // Disable the buttons until we are done
    setButtonsAllEnabled(false);

    // Start the process
    QString prog;
    QStringList args;

    if ( wDir.isEmpty() ) {
      prog = item->text(0);
      args << "start";
    } else {
      prog = "warden";
      args << "chroot" << wIP << item->text(0) + " start";
    }


    // Show the progress GUI
    progressUI *servAction = new progressUI;
    servAction->startServ(prog, args);
    servAction->show();

    // Set item staus to pending
    item->setText(3, tr("Checking...") );

    listSelectionChanged();

}


void ServiceManager::stopSlot()
{
    QTreeWidgetItem *item = listServices->currentItem();
      if (!item)
	return;
	  
    workingTreeWidgetItem = item;	 
	  
    // Disable the buttons until we are done
    setButtonsAllEnabled(false);

    // Stop the process
    QString prog;
    QStringList args;

    if ( wDir.isEmpty() ) {
      prog = item->text(0);
      args << "stop";
    } else {
      prog = "warden";
      args << "chroot" << wIP << item->text(0) + " stop";
    }

    // Show the progress GUI
    progressUI *servAction = new progressUI;
    servAction->startServ(prog, args);
    servAction->show();

    // Set item staus to pending
    item->setText(3, tr("Checking...") );

    listSelectionChanged();

}


void ServiceManager::restartSlot()
{
    QTreeWidgetItem *item = listServices->currentItem();
      if (!item)
	return;
	  
    workingTreeWidgetItem = item;	 

    // Disable the buttons until we are done
    setButtonsAllEnabled(false);

    // Restart the process
    QString prog;
    QStringList args;

    if ( wDir.isEmpty() ) {
      prog = item->text(0);
      args << "start";
    } else {
      prog = "warden";
      args << "chroot" << wIP << item->text(0) + " restart";
    }


    // Show the progress GUI
    progressUI *servAction = new progressUI;
    servAction->startServ(prog, args);
    servAction->show();

    // Set item staus to pending
    item->setText(3, tr("Checking...") );

    listSelectionChanged();

}


void ServiceManager::enableSlot()
{
    QTreeWidgetItem *item = listServices->currentItem();
    if (!item)
      return;
	  
    workingTreeWidgetItem = item;	 
    QStringList rcList;
    QString tag = item->text(1);

    // Disable the buttons until we are done
    setButtonsAllEnabled(false);

    pcbsd::Utils::setConfFileValue( wDir + "/etc/rc.conf", tag, tag + "=\"YES\"", -1);

    // Set the service as enabled
    item->setText(4, tr("Enabled"));
    listSelectionChanged();
}


void ServiceManager::disableSlot()
{
    QTreeWidgetItem *item = listServices->currentItem();
      if (!item)
        return;
	  
    workingTreeWidgetItem = item;	 
	  
    // Disable the buttons until we are done
    setButtonsAllEnabled(false);
    QStringList rcList;
    QString tag = item->text(1);

    // Disable the buttons until we are done
    setButtonsAllEnabled(false);

    pcbsd::Utils::setConfFileValue( wDir + "/etc/rc.conf", tag, tag + "=\"NO\"", -1);

    // Set the service as enabled
    item->setText(4, tr("Disabled"));
    listSelectionChanged();
}


void ServiceManager::populateList()
{
	QString tmp;
	bool valid;
	QString ServiceDir, ServiceTag, ServiceName;
    
	QStringList sDirs;
	sDirs << wDir + "/etc/rc.d" << wDir + "/usr/local/etc/rc.d";

	for ( int z = 0; z < sDirs.size(); ++z) {

          QDir d( sDirs.at(z) );
	
          d.setFilter( QDir::Files );
          d.setSorting( QDir::Name );

          if ( d.count() == 0 )
            return;
	
          for ( unsigned int i = 0; i < d.count(); i++ )
          {
	      ServiceDir=""; ServiceTag=""; ServiceName="";

	      QFile file( sDirs.at(z) + "/" + d[i] );
	      if ( file.open( QIODevice::ReadOnly ) ) {
		  valid=false;
		  ServiceDir=d[i];
		  QTextStream stream( &file );
		  stream.setCodec("UTF-8");
		  QString line;
		  while ( !stream.atEnd() ) {
		      line = stream.readLine(); // line of text excluding '\n'
		     
		      if ( line.indexOf("name=") == 0)
		      {
	  		valid=true;
			tmp = line.replace("name=", "");
			ServiceName = tmp.replace('"', "");
		      }
		      if ( line.indexOf("rcvar=") == 0)
		      {
			if ( tmp.isEmpty() )
			  continue;

			tmp = line.replace("rcvar=", "");
			tmp = tmp.replace('"', "");
			tmp = tmp.replace("'", "");
			tmp = tmp.replace("`", "");
			tmp = tmp.replace("$(set_rcvar)", "");
			tmp = tmp.replace("$set_rcvar", "");
			tmp = tmp.replace("set_rcvar", "");
			tmp = tmp.replace("${name}", "");
			tmp = tmp.replace("_enable", "");
			tmp = tmp.replace(" ", "");

			if (tmp.isEmpty())
		  	  ServiceTag = ServiceName + "_enable";
			else
		  	  ServiceTag = tmp;

			if ( ServiceTag.indexOf("_enable") == -1 )
				ServiceTag=ServiceTag + "_enable";
 			break;
	  	      }
	  	  }
		  file.close();

	  	  if ( !valid || ServiceTag.isEmpty() )
	  	    continue;

                  QStringList cols;
		  QString cDir = sDirs.at(z);
		  if ( ! wDir.isEmpty() )
		    cDir.replace(wDir, "");
                  cols << cDir + "/" + ServiceDir << ServiceTag << ServiceName << tr("Unknown") << tr("Unknown");
		  (void) new QTreeWidgetItem(listServices, cols );
		  qDebug() << "Added Service:" << cDir << ServiceDir << ServiceName << ServiceTag;
	      }
          }
 	}   

	listServices->sortItems(2, Qt::AscendingOrder);
}


void ServiceManager::checkRunning()
{
    QTreeWidgetItemIterator it(*currentCheckRunningItem);
    if (*it)
    {
	if ( wDir.isEmpty() ) {
	  // Start the detection script
          QString prog = (*it)->text(0);
          QStringList args;
          args << "status";
	  CheckServiceRunning= new QProcess( this );
	  connect( CheckServiceRunning, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(checkRunningFinishedSlot()) );
	  CheckServiceRunning->start(prog, args);
	} else {
	  // Start the detection script
          QString prog = "warden";
          QStringList args;
          args << "chroot" << wIP << (*it)->text(0) + " status";
	  CheckServiceRunning= new QProcess( this );
	  connect( CheckServiceRunning, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(checkRunningFinishedSlot()) );
	  CheckServiceRunning->start(prog, args);
        }
	return;
    }
    

    // At the end! Restart scanning in a few seconds
    currentCheckRunningItem = new QTreeWidgetItemIterator(listServices);
    QTimer::singleShot(5000, this, SLOT(checkRunning()));

}


void ServiceManager::checkEnabled()
{
    QStringList rcList;
    QStringList rcFiles;
    QString tmp, tmp2;

    rcFiles << wDir + "/etc/rc.conf.local" << wDir + "/etc/rc.conf" << wDir + "/etc/rc.conf.pcbsd" << wDir + "/etc/defaults/rc.conf";

    // Read in the rc conf files
    for ( int r = 0; r<rcFiles.size(); r++ ) {
      QFile file( rcFiles.at(r) );
      if ( file.open( QIODevice::ReadOnly ) ) {
        QTextStream stream( &file );
        stream.setCodec("UTF-8");
        QString line;
        while ( !stream.atEnd() )
             rcList << stream.readLine(); 
        file.close();
      }
    }



    currentCheckEnabledItem = new QTreeWidgetItemIterator(listServices);
    QTreeWidgetItemIterator it(*currentCheckEnabledItem);
    while(*it)
    {
	(*it)->setText(4, tr("Disabled") );

	for ( int i = 0; i< rcList.size(); i++) {
  	  tmp = rcList.at(i).simplified();
	  tmp2 = (*it)->text(1) + "=\"YES\"";
	  if ( tmp.indexOf(tmp2) == 0 ) {
		(*it)->setText(4, tr("Enabled") );
		break;
	  }
	  tmp2 = (*it)->text(1) + "=\"NO\"";
	  if ( tmp.indexOf(tmp2) == 0 ) {
		(*it)->setText(4, tr("Disabled") );
		break;
	  }
	}
       it++; 
    }
}


void ServiceManager::checkRunningFinishedSlot()
{
    QTreeWidgetItemIterator it(*currentCheckRunningItem);

    if ( CheckServiceRunning->exitCode() == 0 && CheckServiceRunning->exitStatus() == QProcess::NormalExit)
    {
	QString tmp = CheckServiceRunning->readAll();
	if ( tmp.indexOf("is running") != -1 )
  	  (*it)->setText(3, tr("Running") );
	else if ( tmp.indexOf("not running") != -1 )
  	  (*it)->setText(3, tr("Stopped") );
	else
  	  (*it)->setText(3, "" );
    } else {
	(*it)->setText(3, "" );
    }

    (*currentCheckRunningItem)++; 

    // Update button status if we are on currently selected item
    QTreeWidgetItem *i = listServices->currentItem();
    if (i) 
      if(listServices->currentItem()->text(0) == (*it)->text(0) )
	listSelectionChanged();

    checkRunning();

}

void ServiceManager::listSelectionChanged()
{
    	  
    pushStart->setEnabled(false);
    pushStop->setEnabled(false);
    pushRestart->setEnabled(false);
    pushEnableStartup->setEnabled(false);
    pushDisableStartup->setEnabled(false);
    
    QTreeWidgetItem *i = listServices->currentItem();
     if (!i)
       return;
	
     if ( listServices->currentItem()->text(3) == tr("Running") )
     {
        pushStart->setEnabled(false);
        pushStop->setEnabled(true);
        pushRestart->setEnabled(true);
     }
	
     if ( listServices->currentItem()->text(3) == tr("Stopped")
       || (listServices->currentItem()->text(3).isEmpty() 
       && listServices->currentItem()->text(4) == tr("Enabled") ) )
     {
        pushStart->setEnabled(true);
        pushStop->setEnabled(false);
        pushRestart->setEnabled(false);
     }

     if ( listServices->currentItem()->text(4) == tr("Enabled") )
     {
        pushEnableStartup->setEnabled(false);
        pushDisableStartup->setEnabled(true);
     }
	
     if ( listServices->currentItem()->text(4) == tr("Disabled") )
     {
        pushEnableStartup->setEnabled(true);
        pushDisableStartup->setEnabled(false);
     }
}
