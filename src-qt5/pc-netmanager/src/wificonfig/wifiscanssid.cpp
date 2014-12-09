#include "wifiscanssid.h"
#include "ui_wifiscanssid.h"
#include <qtextstream.h>
#include <qtimer.h>
#include <pcbsd-utils.h>
#include <pcbsd-netif.h>
#include <QDebug>


void wifiscanssid::init(QString device)
{
   DeviceName = device;
   pushSelect->setEnabled(false);
    
   QTimer::singleShot(500,  this,  SLOT(slotRescan()));

   connect( pushRescan, SIGNAL( clicked() ), this, SLOT(slotRescan()) );
   connect( pushSelect, SIGNAL( clicked() ), this, SLOT(slotConnect()) );
   connect( pushCancel, SIGNAL( clicked() ), this, SLOT(slotCancel()) );
}
   
void wifiscanssid::scanWifi()
{
    QString strength, ssid, tmp, FileLoad;
    QStringList ifconfout, ifline;    
    int newStrength;
    int foundItem = 0;

    // Clear the list box
    listWifi->clear();
    textTop->setText(tr("Scanning for wireless networks...") );
            
    // Start the scan and get the output
    ifconfout = pcbsd::Utils::runShellCommand("ifconfig -v " + DeviceName + " up list scan");

    //display the info for each wifi access point
    for(int i=1; i<ifconfout.size(); i++){    //Skip the header line by starting at 1
      ifline = NetworkInterface::parseWifiScanLine(ifconfout[i],true); //get a single line
      //save the info for this wifi
      ssid = ifline[0];
      strength = ifline[4];
      //determine the icon based on the signal strength
      tmp = strength.section("%",0,0,QString::SectionSkipEmpty); //remove the % from the end
      newStrength = tmp.toInt(); //integer strength value 
      bool duplicateSSID = false;
      for ( int z = 0 ; z < listWifi->count() ; z++){
        qDebug() << listWifi->item(z)->text() << ssid;
	if ( listWifi->item(z)->text().contains(ssid+" (") ){
          duplicateSSID = true;
          break;
	}
      }

      if( !duplicateSSID ){
        if ( newStrength < 25 ){	
	  FileLoad= PROGPATH + "/pics/tray_wifi.png";
        } else if ( newStrength < 50 ) {
	  FileLoad= PROGPATH + "/pics/tray_wifi30.png";
        }  else if ( newStrength < 75 ) {
	  FileLoad= PROGPATH + "/pics/tray_wifi60.png";
        } else {
  	  FileLoad= PROGPATH + "/pics/tray_wifi85.png";
        }
        QImage *Icon = new QImage(FileLoad);
        QPixmap PixmapIcon;
        PixmapIcon.fromImage(Icon->scaled(22,22));
        //Add the wifi access point to the list
        listWifi->addItem(new QListWidgetItem(PixmapIcon, ssid + " (signal strength: " +strength + ")") );
        foundItem = 1; //set the flag for wifi signals found 
      }else{
        //if the new signal is stronger, change the current label to reflect it

      } 
    }
    
    if ( foundItem == 1 ){
      textTop->setText(tr("Select a wireless network to connect.") );
      listWifi->setCurrentRow(0);
      pushSelect->setEnabled(true);
    } else {
      textTop->setText(tr("No wireless networks found!") );
      pushSelect->setEnabled(false);
    }

}


void wifiscanssid::slotCancel()
{
    close();
}


void wifiscanssid::slotConnect()
{
    QString selectedSSID;
    
    // Make sure a device was selected
    int sel = listWifi->currentRow();
    if ( sel == -1 )
    {
	QMessageBox::warning( this, tr("No selection"),
			    tr("Error: You must select a network to connect!\n") );
	return;
    }
    
    QListWidgetItem *selected = listWifi->currentItem(); 
    selectedSSID = selected->text();
    selectedSSID.truncate( selectedSSID.indexOf("(") - 1 );
    
    // Emit the new network SSID the user selected
    emit saved(selectedSSID);
    close();
}

void wifiscanssid::slotRescan()
{
    pushSelect->setEnabled(false);
    scanWifi();
}



