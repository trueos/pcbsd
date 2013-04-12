/***************************************************************************
 *   Copyright (C) 2007 by Kris Moore   *
 *   kris@pcbsd.com   *
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

#include <iostream>
#include <QCheckBox>
#include <QDesktopWidget>
#include <QFile>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QTabWidget>
#include <QTextStream>
#include <QTimer>
#include <QTranslator>

#include <pcbsd-netif.h>
#include <pcbsd-utils.h>

#include "ethernetconfig.h"
#include "ui_ethernetconfig.h"


using namespace std;

QString DeviceName;



void ethernetconfig::slot_close()
{
   exit(0);
}

// Function to check if we have wlan devices on this system
bool ethernetconfig::isWifiEnabled() {
	QString command = "ifconfig -l";
	QString inputLine = getLineFromCommandOutput(command);
	if ( inputLine.indexOf("wlan") != -1 )
		return true;
	else
		return false;
}

void ethernetconfig::slot_apply()
{

   QString ifConfigLine;

    if ( ! buttonApply->isEnabled() )
	return;


    // If the user disabled the device, do so now
    if ( checkDisableNetwork->isChecked() )
    {
      pcbsd::Utils::setConfFileValue( "/etc/rc.conf", "ifconfig_" + DeviceName + "=", "", -1);
      runCommand("ifconfig " + DeviceName + " down");
      buttonApply->setEnabled(FALSE);
      return;
    }

    
    if ( groupStatic->isChecked() )
    {
	if ( lineIP->text() == "..." || lineNetmask->text() == "..." ) {
	    QMessageBox::information( this, tr("Missing Fields"),
                    tr("You must enter an IP and Netmask to continue!\n") );
	    return;
	}
	
	if ( ! pcbsd::Utils::validateIPV4(lineIP->text() ) ) {
	    QMessageBox::warning( this, tr("Warning"),
	    tr("IP Address is out of range! (") + lineIP->text() + tr(") Fields must be between 0-255.") );
	   return;
               }
	
	if ( ! pcbsd::Utils::validateIPV4(lineNetmask->text() ) ) {
	    QMessageBox::warning( this, tr("Warning"),
	    tr("Netmask is out of range! (") + lineNetmask->text() + tr(") Fields must be between 0-255.") );
	   return;
               }

	if ( groupWPAE->isChecked() )
	  ifConfigLine="WPA ";
	
	if ( checkMAC->isChecked() ) 
	    ifConfigLine+=lineIP->text() + " netmask " + lineNetmask->text();
	else
	    ifConfigLine+=lineIP->text() \
                        +" netmask " + lineNetmask->text() \
                        + " ether " + lineMAC->text();
	
    } else {
	// Using WPAE config?
	if ( groupWPAE->isChecked() )
	  ifConfigLine="WPA SYNCDHCP";
	else
	  ifConfigLine="DHCP";
    }

    if ( checkIPv6Enable->isChecked() ) {
       // Get rid of the ifconfig line for the ipv6 interface
       // Remove old deprecated syntax as well:
       pcbsd::Utils::setConfFileValue( "/etc/rc.conf", "ipv6_ifconfig_" + DeviceName + "=", "", -1);
       pcbsd::Utils::setConfFileValue( "/etc/rc.conf", "ifconfig_" + DeviceName + "_ipv6=", "", -1);
       // Set accept_rtadv
       pcbsd::Utils::setConfFileValue( "/etc/rc.conf", "ifconfig_" + DeviceName + "_ipv6=", "ifconfig_" + DeviceName + "_ipv6=\"inet6 accept_rtadv\"", -1);
    } else {
       // Set static address and disbale accept_rtadv.  This means someone will also have to set
       // the default gateway which currently is configured form the advanced tab.  This should
       // move here or all should move to a dedicated IPv6 tab like "General" is IPv4ish.
       if (lineIPv6Address->text() != "") {
	  QString tmp = lineIPv6Address->text();
          if (!tmp.contains(QRegExp("^inet6 "))) {
	     tmp.prepend("inet6 ");
          }
          pcbsd::Utils::setConfFileValue( "/etc/rc.conf", "ifconfig_" + DeviceName + "_ipv6=", "ifconfig_" + DeviceName + "_ipv6=\"" + tmp + " -accept_rtadv\"", -1);
       }
    }

   pcbsd::Utils::setConfFileValue( "/etc/rc.conf", "ifconfig_" + DeviceName + "=", "ifconfig_" + DeviceName + "=\"" + ifConfigLine + "\"", -1); 

   // See if we need to enable lagg on this device
   if ( isWifiEnabled() && useLagg )
      NetworkInterface::enableLagg(QString("wlan0"));
    
   pcbsd::Utils::restartNetworking();

   // Done, now set the apply button to off
   buttonApply->setEnabled(FALSE);
}

void ethernetconfig::slotIPCheckbox()
{
   // Done, now set the apply button to on
   buttonApply->setEnabled(TRUE);
}

void ethernetconfig::programInit()
{
   // See if we need to enable / disable the IP setup
   slotIPCheckbox();
   // Done, now set the apply button to on
   buttonApply->setEnabled(FALSE);

   // Connect our buttons / slots
   connect(buttonClose, SIGNAL(clicked()), this, SLOT(slot_close()) );
   connect(buttonApply, SIGNAL(clicked()), this, SLOT(slot_apply()) );
   connect(checkDisableNetwork, SIGNAL(clicked()), this, SLOT(slotDisableCheck()) );
   connect(groupStatic, SIGNAL(clicked()), this, SLOT(slotIPCheckbox()) );
   connect(groupWPAE, SIGNAL(clicked()), this, SLOT(slotIPCheckbox()) );
   connect(buttonOK, SIGNAL(clicked()), this, SLOT(slotOK()) );
   connect(checkMAC, SIGNAL(clicked()), this, SLOT(slotMacClicked()) );
   connect(lineIP, SIGNAL(textChanged(const QString &)), this, SLOT(slotCheckGlobalText() ) );
   connect(lineNetmask, SIGNAL(textChanged(const QString &)), this, SLOT(slotCheckGlobalText() ) );
   connect(checkIPv6Enable, SIGNAL(clicked()), this, SLOT(slotIPv6clicked() ) );
   connect(lineIPv6Address, SIGNAL(textChanged(const QString &)), this, SLOT(slotCheckGlobalText() ) );
   connect(lineMAC, SIGNAL(textChanged(const QString &)), this, SLOT(slotCheckGlobalText() ) );
   connect(pushWPAE, SIGNAL(clicked()), this, SLOT(slotConfigWPA()) );
}

void ethernetconfig::setDevice(QString Device)
{
   
   buttonApply->setEnabled(FALSE);

   DeviceName = Device;


   this->setWindowTitle(DeviceName + " " + tr("configuration") );

   // Connect the slot to load the values
   QTimer::singleShot(100,  this,  SLOT(slotFinishLoad()));

   
  

}

void ethernetconfig::runCommand( QString command )
{
	FILE *file = popen(command.toLatin1(),"r"); 
	pclose(file);
}


void ethernetconfig::slotDisableCheck()
{
  if ( checkDisableNetwork->isChecked() )
  {
	tabMainWidget->setEnabled(FALSE);
  } else {
	tabMainWidget->setEnabled(TRUE);
  }
   // Done, now set the apply button to on
   buttonApply->setEnabled(TRUE);
}

void ethernetconfig::slotOK()
{
  slot_apply();
  exit(0);
}

void ethernetconfig::slotIPv6clicked()
{
	if ( checkIPv6Enable->isChecked() ) {
		groupIPv6->setEnabled(FALSE);
	} else {
		groupIPv6->setEnabled(TRUE);
	}

   // Done, now set the apply button to on
   buttonApply->setEnabled(TRUE);
}

void ethernetconfig::slotMacClicked()
{
   if ( checkMAC->isChecked() ) {
	groupMAC->setEnabled(FALSE);
   } else {
	groupMAC->setEnabled(TRUE);
   }
   // Done, now set the apply button to on
   buttonApply->setEnabled(TRUE);
}

void ethernetconfig::loadInfo()
{
  QString tmp;

  // Get all the device information for the info tab
  if ( textInfoName->text().isEmpty() ) {
  tmp = getNameForIdent(DeviceName);
  tmp.truncate(25);
  textInfoName->setText(tmp);
  }

  QString fDev = DeviceName ;
  if ( useLagg )
    fDev = "lagg0" ;

  textIP->setText(getIpForIdent(fDev) );
  textNetmask->setText(getNetmaskForIdent(fDev) );
  textMac->setText(getMacForIdent( DeviceName ) );
  textStatus->setText(getStatusForIdent( DeviceName ) );
  textIPv6->setText(getIPv6ForIdent( DeviceName) );
  textGateway->setText(getGatewayForIdent(fDev) );
  tmp = getMediaForIdent(DeviceName);
  tmp.truncate(20);
  textMedia->setText(tmp);

  // Get the packet status for this device
  textPacketsIn->setText(getInPacketsForIdent( DeviceName) );
  textPacketsInErrors->setText( getInErrorsForIdent( DeviceName) );
  textPacketsOut->setText(getOutPacketsForIdent( DeviceName) );
  textPacketsOutErrors->setText(getOutErrorsForIdent( DeviceName) );

  // Connect the slot to refresh
  QTimer::singleShot(3000,  this,  SLOT(loadInfo()));
}

QString ethernetconfig::getNameForIdent( QString ident )
{
    NetworkInterface ifr(ident);
    return ifr.desc();
}

QString ethernetconfig::getIpForIdent( QString ident )
{
   NetworkInterface ifr(ident);
   return ifr.ipAsString();
}

QString ethernetconfig::getMacForIdent( QString ident )
{
   NetworkInterface ifr(ident);
   return ifr.macAsString();
}

QString ethernetconfig::getStatusForIdent( QString ident )
{
   NetworkInterface ifr(ident);
   return ifr.mediaStatusAsString();
}

QString ethernetconfig::getInPacketsForIdent(QString ident)
{
   NetworkInterface ifr(ident);
   return QString::number(ifr.packetsRx());
}

QString ethernetconfig::getInErrorsForIdent(QString ident)
{
   NetworkInterface ifr(ident);
   return QString::number(ifr.errorsRx());
}

QString ethernetconfig::getOutPacketsForIdent(QString ident)
{
   NetworkInterface ifr(ident);
   return QString::number(ifr.packetsTx());
}

QString ethernetconfig::getOutErrorsForIdent(QString ident)
{
   NetworkInterface ifr(ident);
   return QString::number(ifr.errorsTx());
}

QString ethernetconfig::getNetmaskForIdent( QString ident )
{
   NetworkInterface ifr(ident);
   return ifr.netmaskAsString();
}

QString ethernetconfig::getLineFromCommandOutput( QString command )
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

void ethernetconfig::showInfo()
{
	tabMainWidget->setCurrentWidget( tabMainWidget->widget(2) );
}

void ethernetconfig::slotFinishLoad()
{
   QString tmp, tmp2;

   QSettings settings("PCBSD");
   useLagg = settings.value("/pc-netmanager/useLagg", false).toBool();

   // Start loading the device information
   if ( useLagg )
      tmp = pcbsd::Utils::getConfFileValue( "/etc/rc.conf", "ifconfig_lagg0=", 1 );

   if ( tmp.isEmpty() )
      tmp = pcbsd::Utils::getConfFileValue( "/etc/rc.conf", "ifconfig_" + DeviceName + "=", 1 );

   // Using WPAE config?
   if ( tmp.indexOf("WPA") != -1 )
      groupWPAE->setChecked(true);

   if ( tmp.isEmpty() ) {
      checkDisableNetwork->setChecked(TRUE);
      tabMainWidget->setEnabled(FALSE);
   } else if ( tmp.indexOf("DHCP") != -1 ) {

      groupStatic->setChecked(FALSE);
      lineNetmask->setText("255.255.255.0");

      // Look for the mac address change
	if ( tmp.indexOf("ether") != -1 ) {
		tmp2 = tmp;
		tmp2 = tmp2.remove(0, tmp.indexOf("ether") + 6 );
		//if ( tmp.indexOf(" ") != -1 ) {
		//  tmp2.truncate(tmp.indexOf(" ") );
		//}
		lineMAC->setText(tmp2);
		checkMAC->setChecked(FALSE);
		groupMAC->setEnabled(TRUE);
	} 

   } else {
        // Look for an IP configuration
        groupStatic->setChecked(TRUE);
        tmp2 = tmp;

	// Using the lagg port
	if ( useLagg ) {
        	tmp2.remove(0, tmp2.lastIndexOf("laggport") + 9);
        	tmp2 = tmp2.simplified(); 
        	tmp2.remove(0, tmp2.indexOf(" "));
	}

       	tmp2 = tmp2.simplified(); 

	// Remove any WPA line
	tmp2.replace("WPA ", "");

       	tmp2.truncate(tmp2.indexOf(" "));

      	if ( ! tmp2.isEmpty())
	{
	   lineIP->setText(tmp2);

	   // Look for the netmask
	   if ( tmp.indexOf("netmask") != -1 ) {
			tmp2 = tmp;
			tmp2 = tmp2.simplified();
			tmp2 = tmp2.remove(0, tmp.indexOf("netmask") + 8 );
  			if ( tmp.indexOf("ether") != -1 ) {
			tmp2.truncate(tmp.indexOf("ether") );
			}
			lineNetmask->setText(tmp2);
		} else {
		   lineNetmask->setText("255.255.255.0");
		}

		// Look for the mac address change
		if ( tmp.indexOf("ether") != -1 ) {
			tmp2 = tmp;
			tmp2 = tmp2.remove(0, tmp.indexOf("ether") + 6 );
                        tmp2 = tmp2.simplified();
			lineMAC->setText(tmp2);
			checkMAC->setChecked(FALSE);
			groupMAC->setEnabled(TRUE);
		} 

	}


   }


  // Check for the IPv6 Configuration
  // Start loading the device information
   tmp = pcbsd::Utils::getConfFileValue( "/etc/rc.conf", "ifconfig_" + DeviceName + "_ipv6=", 1 );
   if ( tmp.isEmpty() ) {
	// Check for backward compat.
	tmp = pcbsd::Utils::getConfFileValue( "/etc/rc.conf", "ipv6_ifconfig_" + DeviceName + "=", 1 );
   }
   if ( ! tmp.isEmpty() ) {
	// remove the SLAAC default.
	tmp.remove(QRegExp("inet6 "));
        tmp.remove(QRegExp("accept_rtadv"));
        tmp.remove(QRegExp("^\\s*"));
        tmp.remove(QRegExp("\\s*$"));
   }
   if ( tmp.isEmpty() ) {
        checkIPv6Enable->setChecked(TRUE);
   } else {
	checkIPv6Enable->setChecked(FALSE);
	lineIPv6Address->setText(tmp);
   }

  slotIPv6clicked();

  // Starts grabbing information for the info tab
  loadInfo();


  buttonApply->setEnabled(FALSE);
}

QString ethernetconfig::getIPv6ForIdent(QString ident)
{
	QString command = "nice ifconfig " +ident + " | grep \"inet6 \"";
	QString inputLine = getLineFromCommandOutput(command);
	QString ip= "";
	
	if (inputLine != "" && inputLine.indexOf("inet6 ") != -1){
		
		ip = inputLine.remove(0, inputLine.indexOf("inet6 ") + 5);
		ip.truncate(ip.indexOf("%"));
		
		
	}
	
	return ip;
}

QString ethernetconfig::getGatewayForIdent(QString ident)
{
        QString command = "nice netstat -n -r | grep \"default\" | grep " + ident;
	QString inputLine = getLineFromCommandOutput(command);
	QString packets = "";
	
	if (inputLine != "" ){
		inputLine = inputLine.simplified();
		packets = inputLine.section(' ', 1, 1 );
	}
	
	return packets;
}

QString ethernetconfig::getMediaForIdent( QString ident )
{
    	QString command = "nice ifconfig " +ident + " | grep \"media:\"";
	QString inputLine = getLineFromCommandOutput(command);
	QString status = "";
	
	if (inputLine != "" && inputLine.indexOf("media:") != -1){
		status = inputLine.remove(0, inputLine.indexOf("media:") + 7);
		status.truncate(inputLine.indexOf("\n") );
                if ( status.indexOf("(") != -1)
                {
                   status.remove(0, status.indexOf("(") + 1);
                }
                if ( status.indexOf(")") != -1)
                {
                   status.truncate(status.indexOf(")"));
                }

	}
	
	return status;
}

void ethernetconfig::slotCheckGlobalText()
{
  // Done, now set the apply button to off
  buttonApply->setEnabled(TRUE);
}

void ethernetconfig::slotConfigWPA()
{
   // Bring up the WPA-Enterprise config dialog
   QProcess::execute("/usr/local/bin/pc-wificonfig", QStringList() << "wpaonly" << DeviceName);
}
