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
#include <iostream>
#include <QDir>
#include <QFile>
#include <QtNetwork/QHostAddress>
#include <QMenu>
#include <QSettings>
#include <QString>
#include <QTemporaryFile>
#include <QTextStream>
#include <pcbsd-netif.h>
#include <pcbsd-utils.h>
#include <unistd.h>
#include <sys/types.h>

#include "networkman.h"
#include "ui_networkman.h"
#include "../../../config.h"
#include "dnslist.h"

// Set our max number of wlan[0-9] devices we'll support
#define MAX_WLAN_DEVICES 10

using namespace std;

void NetworkMan::Init()
{
    InstallerMode = false;
    Changed = FALSE;
    
    colourWhite = QColor(255, 255, 255);
    colourRed = QColor(255, 78, 78);
    
    // Set tray icon disabled, since we may be running under root mode
    //checkSysTray->setEnabled(FALSE);
    //checkSysTray->setHidden(TRUE);
    
    
    // Let the list box know to add new devices
    firstUpdate = 0;
    
    // Make sure we are running as root
    if ( getuid() != 0 )
    {
	 QMessageBox::critical( this, tr("Need Root Access"),
	  tr("This program must be run as root!") );
	 exit(1);
    }

    // Get the username we're running under
    username = QString::fromLocal8Bit(getenv("LOGNAME"));
        
    // Set global slots
    connect(pushSave, SIGNAL(clicked()), this, SLOT(slotSave()) );
    connect(pushClose, SIGNAL(clicked()), this, SLOT(slotClose()) );

    // Set slots for the network devices tab
    connect(listNetDev, SIGNAL(itemSelectionChanged()), this, SLOT(DevSelectionChanged()) );
    connect(pushConfigure, SIGNAL(clicked()), this, SLOT(PropertiesSlot()) );
    connect(checkSysTray, SIGNAL(clicked()), this, SLOT(slotSysTray()) );
    listNetDev->setContextMenuPolicy( Qt::CustomContextMenu );
    connect(listNetDev, SIGNAL(itemDoubleClicked( QListWidgetItem *) ), this, SLOT(slotDoubleClick(QListWidgetItem *) ) );
    connect(listNetDev, SIGNAL(customContextMenuRequested( const QPoint &) ), this, SLOT(slotListRightClick(const QPoint &) ) );

    // Set slots for proxy stuff
    connect( checkProxyUser, SIGNAL( clicked() ), this, SLOT( slotProxyUserChecked() ) );

    // Set the slots for the advanced tab
    connect(lineDNS1, SIGNAL(textChanged(const QString &)), this, SLOT(slotCheckGlobalText()) );
    connect(lineDNS2, SIGNAL(textChanged(const QString &)), this, SLOT(slotCheckGlobalText()) );
    connect(lineIPv6DNS1, SIGNAL(textChanged(const QString &)), this, SLOT(slotCheckGlobalText()) );
    connect(lineIPv6DNS2, SIGNAL(textChanged(const QString &)), this, SLOT(slotCheckGlobalText()) );
    connect(lineGateway, SIGNAL(textChanged(const QString &)), this, SLOT(slotCheckGlobalText()) );
    connect(lineIPv6Gateway, SIGNAL(textChanged(const QString &)), this, SLOT(slotCheckGlobalText()) );
    connect(lineHostname, SIGNAL(textChanged(const QString &)), this, SLOT(slotCheckGlobalText()) );
    connect(lineSearchDomain, SIGNAL(textChanged(const QString &)), this, SLOT(slotCheckGlobalText()) );

    
    connect(lineProxyAddress, SIGNAL(textChanged(const QString &)), this, SLOT(slotCheckGlobalText()) );
    connect(lineProxyUser, SIGNAL(textChanged(const QString &)), this, SLOT(slotCheckGlobalText()) );
    connect(lineProxyPass, SIGNAL(textChanged(const QString &)), this, SLOT(slotCheckGlobalText()) );
    connect(checkProxyUser, SIGNAL(clicked()), this, SLOT(slotCheckGlobalText()) );
    connect(radioHTTPProxy, SIGNAL(clicked()), this, SLOT(slotCheckGlobalText()) );
    connect(radioSOCKSProxy, SIGNAL(clicked()), this, SLOT(slotCheckGlobalText()) );
    connect(spinProxyPort, SIGNAL(valueChanged(int)), this, SLOT(slotCheckGlobalText()) );

    connect(groupDNS, SIGNAL(clicked(bool)), this, SLOT(slotCheckGlobalText()) );
    connect(groupGateway, SIGNAL(clicked(bool)), this, SLOT(slotCheckGlobalText()) );
    connect(groupIPv6, SIGNAL(clicked(bool)), this, SLOT(slotCheckGlobalText()) );
    connect(groupIPv6, SIGNAL(clicked(bool)), this, SLOT(slotCheckGlobalText()) );
    connect(checkLagg, SIGNAL(clicked(bool)), this, SLOT(slotCheckGlobalText()) );
    connect(groupProxySettings, SIGNAL(clicked(bool)), this, SLOT(slotCheckGlobalText()) );

    connect(PublicDNS1, SIGNAL(clicked()), this, SLOT(slotPublicDNS1Clicked()) );
    connect(PublicDNS2, SIGNAL(clicked()), this, SLOT(slotPublicDNS2Clicked()) );
    connect(IPV6DNS1Button, SIGNAL(clicked()), this, SLOT(slotIPV6DNS1Clicked()) );
    connect(IPV6DNS2Button, SIGNAL(clicked()), this, SLOT(slotIPV6DNS2Clicked()) );

    // Get the FreeBSD Major version we are using
    checkFreeBSDVer();
    
    // Load any global settings
    loadGlobals();
    
     // Start the device detection process
    detectDev();
    
    // Mark selection as changed
    DevSelectionChanged();
}


void NetworkMan::checkFreeBSDVer()
{
   QString command = "uname -r";
   QString unameoutput = getLineFromCommandOutput(command);
   unameoutput = unameoutput.remove(1, unameoutput.length());
   bool ok;
   FreeBSDMajor = unameoutput.toInt(&ok);
   if ( ! ok  ) {
     FreeBSDMajor = -1;
   }

}

void NetworkMan::detectDev()
{
    QString tmp;
    bool foundDev;

    listNetDev->clear();
    textGlobalError->setText(tr("Detecting Devices...") );
    pushConfigure->setEnabled(FALSE);
    
    QStringList ifs = NetworkInterface::getInterfaces();
    uint i = 0;
    for ( QStringList::Iterator it = ifs.begin(); it != ifs.end(); ++it )
    {
       foundDev = false;

       QString dev = *it;
       if (dev.indexOf("lo") != 0 
	   && dev.indexOf("fwe") == -1
	   && dev.indexOf("ipfw") == -1
	   && dev.indexOf("lagg") == -1
	   && dev.indexOf("plip") == -1
	   && dev.indexOf("pfsync") == -1
	   && dev.indexOf("pflog") == -1
	   && dev.indexOf("usbus") == -1
	   && dev.indexOf("vboxnet") == -1
	   && dev.indexOf("tun") == -1)
       {
           if ( FreeBSDMajor >= 8 )
           {
             tmp = getTypeForIdent(dev);
             if ( tmp == "Wireless" && dev.indexOf("wlan") != 0 ) {
               // Found a wireless device, confirm that it has corrisponding wlan device
               if ( ! checkWifiWlans("wlans_" + dev) ) {
                  // This device does NOT have a wlan[0-9] child, lets prompt the user to create it now

                  // Place a message box prompt here
                  QMessageBox msgBox;
 	          msgBox.setText("A new wireless device (" + dev + ") has been detected.");
 		  msgBox.setInformativeText("Do you want to enable this device now?");
 		  msgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
 		  msgBox.setDefaultButton(QMessageBox::Yes);
 		  int ret = msgBox.exec();
                  if ( ret == QMessageBox::Yes )
                  {
                    // Get the next available wlan[0-9] device
                    tmp = getNextAvailWlan();

                    QTemporaryFile tmpfile;
                    if ( tmpfile.open() ) {
                    QTextStream streamout( &tmpfile );
                      streamout << "#!/bin/sh\n";
                      streamout << IFCONFIG + tmp + " create wlandev " + dev + "\n";
                      streamout << "echo 'wlans_" + dev + "=\"" + tmp + "\"' >>/etc/rc.conf\n";
                      tmpfile.close();
                    }

                    // Create our script to setup the child wlan device
                    system("sh " + tmpfile.fileName().toLatin1());
      
                    // re-start the device detection process
                    QTimer::singleShot(100,  this,  SLOT(detectDev()));
                  }
               } 

             } else {


               // Using a regular ethernet device or a wlan[0-9] device
	       //qDebug("Found device: " + dev);
	       Devs[i] = dev;

	       // Determine if its a wireless or wired device
	       DevsType[i] = tmp;


               // If we've found a wifi device, with name of wlan[0-9]
               // Then set the Parent name to pull info direct from device
               if ( tmp == "Wireless" ) {
                 DevsParent[i] = getWifiParent(dev);

	         // Get HW Identify line
	         DevsName[i] = getNameForIdent(DevsParent[i]);

                 //QMessageBox::warning( this, "Testing", DevsName[i]);
                 // If the name comes up bogus for some reason
                 if( DevsName[i].isEmpty() ) {
                   DevsName[i] = tr("Unknown Wireless Device");
                 }
               } else {
	         // Get HW Identify line
	         DevsName[i] = getNameForIdent(Devs[i]);
               }

               foundDev = true;   
                
             }
           } else {
	     //qDebug("Found device: " + dev);
	     Devs[i] = dev;
	     // Determine if its a wireless or wired device
	     DevsType[i] = getTypeForIdent(Devs[i]);
	     // Get HW Idtentify line
	     DevsName[i] = getNameForIdent(Devs[i]);
             foundDev = true;   
	   
           }

         if(foundDev) {
           pushConfigure->setEnabled(TRUE);
      	   i++;
         }
       }
   }

    // Mark the last device as unused
    Devs[i] = "";

    textGlobalError->setText("");
    refreshDevices();
    DevSelectionChanged();

}

// Function which locates the parent device of a wlan child device
QString NetworkMan::getWifiParent(QString dev)
{  
   dev.remove(0, dev.size() -1 );
   QString DevNum = dev;
   return pcbsd::Utils::sysctl("net.wlan." + DevNum + ".%parent");
}

QString NetworkMan::getNextAvailWlan()
{
   QString tmp;
   QString line;
   bool avail;

   QFile file( "/etc/rc.conf" );
   if ( file.open( QIODevice::ReadOnly ) ) {
     QTextStream stream( &file );

     for ( int i = 0; i< MAX_WLAN_DEVICES; i++)
     {
        // Jump back to beginning of file
        stream.seek(0);
        avail = true;

        while ( !stream.atEnd() ) {
          line = stream.readLine(); // line of text excluding '\n'

          // If the KEY is found in the line, continue processing
          if ( line.indexOf("wlans_", 0) != -1 && line.indexOf("=") != -1 )
          {
             // Found a wlan_ line, check the wlan device to see if its available
             if ( line.indexOf("wlan" + tmp.setNum(i) ) != -1)
               avail = false;            
          }
        }

        if (avail) {
          line = "wlan" + tmp.setNum(i);
          return line;
        }
      }


     file.close();
   } 

   tmp = "";
   return tmp;

}

QString NetworkMan::getLineFromCommandOutput( QString command )
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


QString NetworkMan::getNameForIdent( QString ident )
{
    NetworkInterface ifr(ident);
    return ifr.desc();
}


QString NetworkMan::getIpForIdent( QString ident )
{
   QString cmd = IFCONFIG + " lagg0 2>/dev/null | grep " + ident;
   QString checkLagg = getLineFromCommandOutput(cmd.toLatin1());
   if ( ! checkLagg.isEmpty() )
    ident = "lagg0";

   NetworkInterface ifr(ident);
   return ifr.ipAsString();
}


QString NetworkMan::getMacForIdent( QString ident )
{
   NetworkInterface ifr(ident);
   return ifr.macAsString();
}


QString NetworkMan::getStatusForIdent( QString ident )
{
   NetworkInterface ifr(ident);
   return ifr.mediaStatusAsString();
}

void NetworkMan::DevSelectionChanged()
{
   int sel = listNetDev->currentRow();
    
   if ( sel != -1 ) {
    
   pushConfigure->setEnabled(TRUE);

	
   // Check to see if the network tray icon is enabled or not
   QString filename = PREFIX + "/share/pcbsd/xstartup/tray-" + Devs[sel] + ".sh";


   // First run a check to if we need to enable or disable the checkbox
   if ( QFile::exists( filename ) )
   {
     checkSysTray->setChecked(TRUE);
   } else {
     checkSysTray->setChecked(FALSE);
   }
    
    
   DevsIP[sel] = getIpForIdent(Devs[sel]);
   DevsStatus[sel] = getStatusForIdent(Devs[sel]);
   DevsNetmask[sel] = getNetmaskForIdent(Devs[sel]);
    
   textStatusLabel1->setText(tr("Address:"));	
   textStatus1->setText(tr("IP: ") + DevsIP[sel] + " / " + tr("Netmask: ") + DevsNetmask[sel]);

   if ( getTypeForIdent(Devs[sel]) == "Wireless" )
   {
     checkSysTray->setVisible(FALSE);
     textStatusLabel2->setText(tr("SSID:"));
     QString SSID = ""; 
     QString tmp;
       
     QProcess *getIfProc = new QProcess();
     getIfProc->start(IFCONFIG, QStringList() << Devs[sel]);
     if (getIfProc->waitForFinished(2000)) {
       tmp = getIfProc->readAllStandardOutput().simplified();
     }

     getIfProc->kill();
     delete getIfProc;

     if (tmp != "" && tmp.indexOf("ssid ") != -1){
    	SSID = tmp.remove(0, tmp.indexOf("ssid ") + 5);
    	SSID.truncate(tmp.indexOf("channel") - 1 );
     }

     textStatus2->setText(SSID);
   } else {
     if ( ! InstallerMode )
       checkSysTray->setVisible(TRUE);
     textStatusLabel2->setText(tr("MAC Address:"));
     textStatus2->setText(DevsMAC[sel]);
   }
   
   textStatusLabel3->setText(tr("Status:"));
   textStatus3->setText(DevsStatus[sel]);
    
  }
    
}

bool NetworkMan::checkWifiWlans( QString Key )
{
QFile file( "/etc/rc.conf" );
    if ( file.open( QIODevice::ReadOnly ) ) {
        QTextStream stream( &file );
        QString line;
        while ( !stream.atEnd() ) {
            line = stream.readLine(); // line of text excluding '\n'

          // If the KEY is found in the line, continue processing
          if ( line.indexOf(Key, 0) != -1)
          {
            return TRUE;
          }
        }
        file.close();
    }

    return FALSE;
}

bool NetworkMan::checkValue( QString File, QString Key, QString Value )
{
QFile file( File );
    if ( file.open( QIODevice::ReadOnly ) ) {
        QTextStream stream( &file );
        QString line;
        while ( !stream.atEnd() ) {
            line = stream.readLine(); // line of text excluding '\n'
            
               // If the KEY is found in the line, continue processing 
	if ( line.indexOf(Key, 0) != -1)
	{
	    line.remove(line.indexOf(Key, 0), Key.length());
	    if ( line.indexOf(Value, 0) != -1)
	    {
		return TRUE;
	    }
	    
	}
	    
	    
	    
        }
        file.close();
    }
    
    return FALSE;
}

void NetworkMan::PropertiesSlot()
{
    QString program;
    QStringList arguments;
    
    workingDev = listNetDev->currentRow();
    
    if ( workingDev != -1) {
    
    if ( DevsType[workingDev] == "Wireless" )
    {
	// If we are running as root
	if ( getuid() != 0 )
	{
          program = "pc-su";
	  arguments.clear();
	  arguments << "-d" << "--noignorebutton" << "pc-wificonfig" << Devs[workingDev];
	 } else {
          program = "pc-wificonfig";
	  arguments.clear();
	  arguments << Devs[workingDev];
	 }

	 ConfigDevice = new QProcess( this );
         ConfigDevice->start(program, arguments); 

    } else {
   
	// If we are running as root
	if ( getuid() != 0 )
	{
          program = "pc-su";
	  arguments.clear();
	  arguments << "-d" << "--noignorebutton" << "pc-ethernetconfig" << Devs[workingDev];
	 } else {
          program = "pc-ethernetconfig";
	  arguments.clear();
	  arguments << Devs[workingDev];
	 }

	 ConfigDevice = new QProcess( this );
         ConfigDevice->start(program, arguments); 

   }
    
  }
     
}





void NetworkMan::runCommand( QString command )
{
	FILE *file = popen(command.toLatin1(),"r"); 
	pclose(file);
}


void NetworkMan::loadGlobals()
{    
    QString tmp;
    lineHostname->setText(pcbsd::Utils::getConfFileValue("/etc/rc.conf", "hostname=", 1));

    tmp = pcbsd::Utils::getConfFileValue("/etc/rc.conf", "defaultrouter=", 1);
    if ( tmp.isEmpty() )
    {
       groupGateway->setChecked(false);
       // Check the default route
       QString checkRoute = getLineFromCommandOutput("route get default | grep gateway").simplified();
       if ( checkRoute.indexOf("gateway:") != -1 )
         lineGateway->setText(checkRoute.section(" ", 1, 1));
    } else {
        groupGateway->setChecked(true);
        lineGateway->setText(pcbsd::Utils::getConfFileValue("/etc/rc.conf", "defaultrouter=", 1) );
    }

    /* Use simple regular expressions to distinguish IPv4 and IPv6 addresses. */
    lineDNS1->setText(pcbsd::Utils::getConfFileValue("/etc/resolv.conf", "nameserver ", "\\b(?:\\d{1,3}\\.){3}\\d{1,3}\\b", 1) );
    lineDNS2->setText(pcbsd::Utils::getConfFileValue("/etc/resolv.conf", "nameserver ", "\\b(?:\\d{1,3}\\.){3}\\d{1,3}\\b", 2) );
    // Check if we are using custom DNS
    tmp = pcbsd::Utils::getConfFileValue("/etc/dhclient.conf", "supersede domain-name-servers", 1);
    if ( tmp.isEmpty() )
       groupDNS->setChecked(false);
    else
       groupDNS->setChecked(true);


    lineIPv6DNS1->setText(pcbsd::Utils::getConfFileValue("/etc/resolv.conf", "nameserver ", "\\b.*:.*:.*\\b", 1) );
    lineIPv6DNS2->setText(pcbsd::Utils::getConfFileValue("/etc/resolv.conf", "nameserver ", "\\b.*:.*:.*\\b", 2) );
    lineSearchDomain->setText(pcbsd::Utils::getConfFileValue("/etc/resolv.conf", "search ", 1) );

    // Check for IPv6 settings
    lineIPv6Gateway->setText(pcbsd::Utils::getConfFileValue("/etc/rc.conf", "ipv6_defaultrouter=", 1) );
    tmp = pcbsd::Utils::getConfFileValue("/etc/rc.conf", "ipv6_activate_all_interfaces=", 1) ;
    if ( tmp == "YES" )
        groupIPv6->setChecked(TRUE);
    else
        groupIPv6->setChecked(FALSE);


    // Check if we want to use the lagg interface
    QSettings settings("PCBSD");
    checkLagg->setChecked(settings.value("/pc-netmanager/useLagg", false).toBool());
    
    // Load Proxy Stuff
    loadProxyConfig();
	
}


QString NetworkMan::getNetmaskForIdent( QString ident )
{
   QString cmd = IFCONFIG + " lagg0 2>/dev/null | grep " + ident;
   QString checkLagg = getLineFromCommandOutput(cmd.toLatin1());
   if ( ! checkLagg.isEmpty() )
    ident = "lagg0";

   NetworkInterface ifr(ident);
   return ifr.netmaskAsString();
}


void NetworkMan::restartNetwork()
{
   pcbsd::Utils::restartNetworking();
}


QString NetworkMan::getTypeForIdent( QString ident )
{
   NetworkInterface ifr(ident);
   if (ifr.isWireless()) return "Wireless";
   return "Ethernet";
}


void NetworkMan::refreshDevices()
{
    
    int i = 0;
    listNetDev->clear(); 
    while (! Devs[i].isEmpty() )
    {
	
                DevsIP[i] = getIpForIdent(Devs[i]);
		DevsStatus[i] = getStatusForIdent(Devs[i]);
		DevsUp[i] = getUpStatusForIdent(Devs[i]);
		DevsNetmask[i] = getNetmaskForIdent(Devs[i]);
		
		if ( firstUpdate == 0) {
		  DevsMAC[i] = getMacForIdent(Devs[i]);
		  listNetDev->setCurrentRow(0);
	        }

		if ( DevsType[i] == "Wireless")
		{
		    
		    QPixmap PixmapIcon( PREFIX + "/share/pcbsd/pc-netmanager/pics/network_wifi.png");
		    listNetDev->addItem(new QListWidgetItem(PixmapIcon.scaled(32,32), Devs[i] + " (" +DevsName[i] + ")") );
		    
		 } else {
		    // If we are looking at a regular Ethernet device  
		    QPixmap PixmapIcon( PREFIX + "/share/pcbsd/pc-netmanager/pics/network_local.png");
		    listNetDev->addItem(new QListWidgetItem(PixmapIcon.scaled(32,32), Devs[i] + " (" +DevsName[i] + ")") );
		}
	
	i++;
    }
			   
}


void NetworkMan::slotSysTray()
{
    int  sel = listNetDev->currentRow();
    if ( sel == -1 )
       return;	
    
    QString Type;
    QString Command;
    
    // Start the tray applet
    if (checkSysTray->isChecked() )
    { 
	// Save the new file
        QFile fileout( PREFIX + "/share/pcbsd/xstartup/tray-" + Devs[sel] + ".sh");
        if ( fileout.open( QIODevice::WriteOnly ) ) {
          QTextStream streamout( &fileout );
          streamout << "sudo pc-nettray " + Devs[sel] + " &";
          fileout.close();
	    
	  runCommand("chmod 755 \"" + PREFIX + "/share/pcbsd/xstartup/tray-" + Devs[sel] + ".sh\"");
	  runCommand("sync");

          // Now start the tray icon
          QString sCmd = "su " + username + " -c \"sudo pc-nettray " + Devs[sel] + "\" &";
	  system(sCmd.toLatin1());
		    
        }
	
    } else {
	// Kill the tray applet
	runCommand("rm -f \"" + PREFIX + "/share/pcbsd/xstartup/tray-" + Devs[sel] + ".sh\"");
    }

}


void NetworkMan::slotDoubleClick( QListWidgetItem *item __unused )
{
    // Start the configuration dialog
    PropertiesSlot();
}

void NetworkMan::slotTimerRefresh()
{
    QTimer::singleShot( 10000, this, SLOT(refreshDevices()) );
}


void NetworkMan::slotListRightClick( const QPoint &pos __unused )
{
    int currentItem = listNetDev->currentRow();
    if ( currentItem != -1 ) {
	  popup = new QMenu();
	  popup->addAction( tr("Configure ") + Devs[currentItem], this, SLOT(PropertiesSlot()));
	  popup->addAction( tr("Device Information"), this, SLOT(slotShowInfoConfig()));
	  // If we are running as root
	  if ( getuid() == 0 ){
	    popup->addSeparator();
	    if ( DevsUp[currentItem] == "DOWN" ) {
	      popup->addAction( tr("Enable device"), this, SLOT(slotEnableDevice()));
	    } else {
	      popup->addAction( tr("Disable device"), this, SLOT(slotDisableDevice()));
	    }
	    if(!Devs[currentItem].startsWith("wlan") && (DevsType[currentItem]== "Wireless") ){
	      popup->addAction( tr("Setup Access Point"), this, SLOT(slotSetupAP()) );
	    }
	    popup->addSeparator();
	    popup->addAction( tr("Restart the Network"), this, SLOT(restartNetwork()));
	  }
	  popup->exec( QCursor::pos() );
    }

}


void NetworkMan::slotDisableDevice()
{
        int currentItem = listNetDev->currentRow();
    if ( currentItem != -1 ) {
	runCommand(IFCONFIG + " " + Devs[currentItem] + " down");
    }

}

void NetworkMan::slotSetupAP(){
  int currentItem = listNetDev->currentRow();
  if(currentItem != -1){
    APSetupDialog dlg(Devs[currentItem], this);
    dlg.exec();
    if(!dlg.cancelled){
      //Re-load the info
    }
  }	  
}

void NetworkMan::slotEnableDevice()
{
        int currentItem = listNetDev->currentRow();
    if ( currentItem != -1 ) {
	runCommand(IFCONFIG + " " + Devs[currentItem] + " up");		
    }	
}


void NetworkMan::slotShowInfoConfig()
{
   QString program;
   QStringList arguments;

   workingDev = listNetDev->currentRow();
    
   if ( workingDev != -1) {
    
    if ( DevsType[workingDev] == "Wireless" )
    {

	// If we are running as root
	if ( getuid() != 0 )
	{
          program = "pc-su";
	  arguments.clear();
	  arguments << "-d" << "--noignorebutton" << "pc-wificonfig" << "info" << Devs[workingDev];
	 } else {
          program = "pc-wificonfig";
	  arguments.clear();
	  arguments << "info" << Devs[workingDev];
	 }

	 ConfigDevice = new QProcess( this );
         ConfigDevice->start(program, arguments); 

    } else {
   
	// If we are running as root
	if ( getuid() != 0 )
	{
          program = "pc-su";
	  arguments.clear();
	  arguments << "-d" << "--noignorebutton" << "pc-ethernetconfig" << "info" << Devs[workingDev];
	 } else {
          program = "pc-ethernetconfig";
	  arguments.clear();
	  arguments << "info" << Devs[workingDev];
	 }

	 ConfigDevice = new QProcess( this );
         ConfigDevice->start(program, arguments); 
   }
    
  }
}


void NetworkMan::setNotRoot()
{
    // Disable functionality since we are not running as root
    checkSysTray->setEnabled(FALSE);
    checkSysTray->setHidden(TRUE);
    lineDNS1->setEnabled(FALSE);
    lineDNS2->setEnabled(FALSE);
    lineIPv6DNS1->setEnabled(FALSE);
    lineIPv6DNS2->setEnabled(FALSE);
    lineHostname->setEnabled(FALSE);
    lineGateway->setEnabled(FALSE);
    lineIPv6Gateway->setEnabled(FALSE);
}


QString NetworkMan::getUpStatusForIdent( QString ident )
{
   NetworkInterface ifr(ident);
   if (ifr.isUp()) return "UP";
   return "DOWN";
}

void NetworkMan::slotCheckGlobalText()
{

   pushSave->setEnabled(false);
   QRegExp hostnameRegExp("^(([a-z0-9][a-z0-9-].*[a-z0-9])|([a-z0-9]+))$");
   textGlobalError->setText("");

    if ( groupIPv6->isChecked() ) {
	QHostAddress addr;
        if ( ! lineIPv6DNS1->text().isEmpty() ) {
            if ( addr.setAddress(lineIPv6DNS1->text()) == false || addr.protocol() != QAbstractSocket::IPv6Protocol ) {
         	textGlobalError->setText(tr("Invalid IPv6 DNS specified"));
	        return;
            }
        }
        if ( ! lineIPv6DNS2->text().isEmpty() ) {
            if ( addr.setAddress(lineIPv6DNS2->text()) == false || addr.protocol() != QAbstractSocket::IPv6Protocol ) {
         	textGlobalError->setText(tr("Invalid IPv6 DNS specified"));
	        return;
            }
        }
        if ( ! lineIPv6Gateway->text().isEmpty() ) {
            if ( addr.setAddress(lineIPv6Gateway->text()) == false || addr.protocol() != QAbstractSocket::IPv6Protocol ) {
         	textGlobalError->setText(tr("Invalid IPv6 Gateway specified"));
	        return;
            }
        }
    }
    
     if(groupDNS->isChecked()){
       if( lineDNS1->text() == "..." && lineDNS2->text() == "..." ){ return; }
       if ( lineDNS1->text() != "..."  && ! pcbsd::Utils::validateIPV4(lineDNS1->text() ) ) {
         textGlobalError->setText(tr("Invalid DNS specified"));
	 return;
       }
       if ( lineDNS2->text() != "..."  && ! pcbsd::Utils::validateIPV4(lineDNS2->text() ) ) {
         textGlobalError->setText(tr("Invalid DNS specified"));
	 return;
       }
     }
    
    
     if ( lineGateway->text() != "..."  && ! pcbsd::Utils::validateIPV4(lineGateway->text())  && groupGateway->isChecked() ) {
         textGlobalError->setText(tr("Invalid Gateway specified"));
	 return;
     }
     
     /**
     Hostnames must:
     - Contain only a-z, 0-9, and hyphens (-)
     - Not have a hyphen as the first or last character
     **/
     lineHostname->setText(lineHostname->text().toLower());
     if (lineHostname->text().isEmpty())
     {
       textGlobalError->setText(tr("Please enter a hostname"));
       return;
     }
     else if (hostnameRegExp.indexIn(lineHostname->text()) == -1)
     {
        textGlobalError->setText(tr("Hostname may only contain letters and numbers"));
	return;
     }

     pushSave->setEnabled(true);

}

void NetworkMan::slotClose()
{
   close();
}

void NetworkMan::slotSave()
{
    
   if ( !lineHostname->text().isEmpty() && (lineHostname->text().toLower() != pcbsd::Utils::getConfFileValue("/etc/rc.conf", "hostname=", 1).toLower() ) ) {
      pcbsd::Utils::setConfFileValue("/etc/rc.conf", "hostname=", "hostname=\"" + lineHostname->text() + "\"", -1);
      pcbsd::Utils::setConfFileValue("/etc/hosts", "::1", "::1\t\t\tlocalhost localhost.localdomain " + lineHostname->text() + ".localhost " + lineHostname->text(), -1);
      pcbsd::Utils::setConfFileValue("/etc/hosts", "127.0.0.1", "127.0.0.1\t\tlocalhost localhost.localdomain " + lineHostname->text() + ".localhost " + lineHostname->text(), -1);
      QMessageBox::information(this,tr("Computer Restart Required"), tr("You must restart your computer to finish changing your hostname") );
   }
    
    
   if ( lineGateway->text() == "..." || ! groupGateway->isChecked() ) {
     pcbsd::Utils::setConfFileValue("/etc/rc.conf", "defaultrouter=", "", -1);
   } else {
     pcbsd::Utils::setConfFileValue("/etc/rc.conf", "defaultrouter=", "defaultrouter=\"" + lineGateway->text() + "\"", -1);  
   }
   
   int DNSline = 1;
   
   /* Prefer IPv6 nameservers if IPv6 is enabled. */
    if ( groupIPv6->isChecked() ) {
        if ( ! lineIPv6DNS1->text().isEmpty() ) {
            pcbsd::Utils::setConfFileValue("/etc/resolv.conf", "nameserver", "nameserver " + lineIPv6DNS1->text(), DNSline);  
            DNSline++;
        }
        if ( ! lineIPv6DNS2->text().isEmpty() ) {
            pcbsd::Utils::setConfFileValue("/etc/resolv.conf", "nameserver", "nameserver " + lineIPv6DNS2->text(), DNSline);  
            DNSline++;
        }
    }
   
   if ( lineDNS1->text() == "..."  || ! groupDNS->isChecked() ) {
     pcbsd::Utils::setConfFileValue("/etc/resolv.conf", "nameserver", "", DNSline);
   } else {
     pcbsd::Utils::setConfFileValue("/etc/resolv.conf", "nameserver", "nameserver " + lineDNS1->text(), DNSline);  
     DNSline++;
   }   
   if ( lineDNS2->text() == "..." || ! groupDNS->isChecked() ) {
     pcbsd::Utils::setConfFileValue("/etc/resolv.conf", "nameserver", "", DNSline);
   } else {
     pcbsd::Utils::setConfFileValue("/etc/resolv.conf", "nameserver", "nameserver " + lineDNS2->text(), DNSline);  
   }   
   
   // If we have custom DNS, make sure it survives a dhclient run
   if ( lineDNS1->text() != "..." && lineDNS2->text() != "..." && groupDNS->isChecked() )
     pcbsd::Utils::setConfFileValue("/etc/dhclient.conf", "supersede domain-name-servers", "supersede domain-name-servers " + lineDNS1->text() + ", " + lineDNS2->text() +";");  
   else if ( lineDNS1->text() != "..." )
     pcbsd::Utils::setConfFileValue("/etc/dhclient.conf", "supersede domain-name-servers", "supersede domain-name-servers " + lineDNS1->text() +";");  
   else if ( lineDNS2->text() != "..." )
     pcbsd::Utils::setConfFileValue("/etc/dhclient.conf", "supersede domain-name-servers", "supersede domain-name-servers " + lineDNS2->text() +";");  
   else
     pcbsd::Utils::setConfFileValue("/etc/dhclient.conf", "supersede domain-name-servers", "");  
    

   if ( lineSearchDomain->text().isEmpty() || ! groupDNS->isChecked() ) {
     pcbsd::Utils::setConfFileValue("/etc/resolv.conf", "search", "", 1);
   } else {
     pcbsd::Utils::setConfFileValue("/etc/resolv.conf", "search", "search " + lineSearchDomain->text(), 1);
   }

   // Save the IPv6 stuff
    if ( groupIPv6->isChecked() ) {
	pcbsd::Utils::setConfFileValue("/etc/rc.conf", "ipv6_activate_all_interfaces=", "ipv6_activate_all_interfaces=\"YES\"", -1);
        if ( ! lineIPv6Gateway->text().isEmpty() ) {
	    pcbsd::Utils::setConfFileValue("/etc/rc.conf", "ipv6_defaultrouter=", "ipv6_defaultrouter=\"" + lineIPv6Gateway->text() + "\"", -1);
        } else {
	    pcbsd::Utils::setConfFileValue("/etc/rc.conf", "ipv6_defaultrouter=", "", -1);
        }
    } else {
	pcbsd::Utils::setConfFileValue("/etc/rc.conf", "ipv6_activate_all_interfaces=", "", -1);
	pcbsd::Utils::setConfFileValue("/etc/rc.conf", "ipv6_defaultrouter=", "", -1);
    }

    // Save the lagg preference
    QSettings settings("PCBSD");
    settings.setValue("/pc-netmanager/useLagg", checkLagg->isChecked());

    // Make sure we enable lagg for this system
    if ( checkLagg->isChecked() )
       NetworkInterface::enableLagg(QString("wlan0"));

    // Save the proxy config
    saveProxyConfig();
   
    restartNetwork();

}

void NetworkMan::slotProxyUserChecked() {
  if ( checkProxyUser->isChecked() ) {
    lineProxyUser->setEnabled(true);
    lineProxyPass->setEnabled(true);
  } else {
    lineProxyUser->setEnabled(false);
    lineProxyPass->setEnabled(false);
  }
}

void NetworkMan::loadProxyConfig()
{
   QString tmp;
   bool ok;

   tmp = pcbsd::Utils::getProxyURL();
   if ( tmp.isEmpty() )
      groupProxySettings->setChecked(false);
   else
      groupProxySettings->setChecked(true); 

   lineProxyAddress->setText(tmp);

   tmp = pcbsd::Utils::getProxyUser();
   lineProxyUser->setText(tmp);
   if ( tmp.isEmpty() )
      checkProxyUser->setChecked(false);
   else
      checkProxyUser->setChecked(true);

   lineProxyPass->setText(pcbsd::Utils::getProxyPass());

   tmp = pcbsd::Utils::getProxyPort();
   tmp.toInt(&ok);
   if ( ok )
      spinProxyPort->setValue(tmp.toInt(&ok));
   

   tmp = pcbsd::Utils::getProxyType();
   if ( tmp == "digest" )
      radioSOCKSProxy->setChecked(true);
   else
      radioHTTPProxy->setChecked(true);

   slotProxyUserChecked();
}

void NetworkMan::saveProxyConfig()
{
   QString tmp;

   if ( ! groupProxySettings->isChecked() ) {
         pcbsd::Utils::setProxyAuth(QString());
         pcbsd::Utils::setProxyAddress(QString());
   } else {
   	pcbsd::Utils::setProxyAddress(lineProxyAddress->text() + ":" + tmp.setNum(spinProxyPort->value()));

	QString authLine;
	if ( ! checkProxyUser->isChecked() || lineProxyUser->text().isEmpty() ) {
     	  pcbsd::Utils::setProxyAuth(QString());
	} else {
   	  if ( radioHTTPProxy->isChecked() )
		authLine="basic:*:";
   	  else
		authLine="digest:*:";
	  authLine += lineProxyUser->text() + ":";
	  authLine += lineProxyPass->text();
     	  pcbsd::Utils::setProxyAuth(authLine);
	}
        QMessageBox::warning( this, tr("Proxy enabled"), tr("You may need to re-login for proxy settings to take effect."));
   }

}

void NetworkMan::slotPublicDNS1Clicked()
{
    DNSList* dlg=new DNSList(this, DNSList::eIPV4);
    dlg->exec();
    if(dlg->success){
        lineDNS1->setText(dlg->ipout);
        slotCheckGlobalText();
    }
}

void NetworkMan::slotPublicDNS2Clicked()
{
    DNSList* dlg=new DNSList(this, DNSList::eIPV4);
    dlg->exec();
    if(dlg->success){
        lineDNS2->setText(dlg->ipout);
        slotCheckGlobalText();
    }
}

void NetworkMan::slotIPV6DNS1Clicked()
{
    DNSList* dlg=new DNSList (this,DNSList::eIPV6);
    dlg->exec();
    if(dlg->success){
        lineIPv6DNS1->setText(dlg->ipout);
        slotCheckGlobalText();
    }
}

void NetworkMan::slotIPV6DNS2Clicked()
{
    DNSList* dlg=new DNSList (this,DNSList::eIPV6);
    dlg->exec();
    if(dlg->success){
        lineIPv6DNS2->setText(dlg->ipout);
        slotCheckGlobalText();
    }
}

void NetworkMan::slotSingleInstance()
{
  this->show();
  this->raise();
}


void NetworkMan::setInstallerMode()
{
  checkSysTray->setHidden(true);
  groupBoxMisc->setHidden(true);
  InstallerMode = true;
}
