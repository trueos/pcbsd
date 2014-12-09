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
#include <QFile>
#include <QSettings>
#include <QTextStream>
#include <QDebug>
#include <QInputDialog>
#include <QXmlStreamReader>

#include <pcbsd-utils.h>
#include <pcbsd-netif.h>
#include <unistd.h>

#include "wificonfigwidgetbase.h"
#include "ui_wificonfigwidgetbase.h"

QString saveSSID; //need this to pass to a couple slots rather than requiring a currently available wifi

void wificonfigwidgetbase::slotClose()
{
    exit(0);
}

void wificonfigwidgetbase::slotApply()
{
    QString tmp;
    QString ifConfigLine;
    if ( ! pushApply->isEnabled() ) {
	return;
    }

    // Get ifConfig Line
    if (! checkDHCP->isChecked() )
    {
	if ( lineIP->text() == "..." || lineNetmask->text() == "..." )
	{
	    QMessageBox::information( this, tr("Missing Fields"),
                    tr("You must enter an IP and Netmask to continue!\n") );
	    return;
	}
	
	if ( ! pcbsd::Utils::validateIPV4(lineIP->text()) ) {
	    QMessageBox::warning( this, tr("Warning"),
	    tr("IP Address is out of range! (") + lineIP->text() + tr(") Fields must be between 0-255.") );
	   return;
               }
	
	if ( ! pcbsd::Utils::validateIPV4(lineNetmask->text()) ) {
	    QMessageBox::warning( this, tr("Warning"),
	    tr("Netmask is out of range! (") + lineNetmask->text() + tr(") Fields must be between 0-255.") );
	   return;
               }
	
	if ( checkMAC->isChecked() )
	    ifConfigLine=lineIP->text() + " netmask " + lineNetmask->text();
	else
	    ifConfigLine=lineIP->text() + " netmask " + lineNetmask->text() + " ether " + lineMAC->text();

    } else {
	ifConfigLine="SYNCDHCP";
    } 

    // See if we need to enable a country code
    if ( groupCountryCode->isChecked() )
    {
       tmp = comboCountryCode->currentText().section("(", 1, 1);
       tmp = tmp.section(")", 0, 0);
       ifConfigLine+=" country " + tmp;
    }

    // Now create the wpa_supplicant file based on saved configuration
    updateWPASupp();

    // Only updating WPA supp?
    if (WPAONLY) 
       return;
    
    // If the user disabled the device, do so now
    if ( checkDisableWireless->isChecked() )
    {
	 pcbsd::Utils::setConfFileValue( "/etc/rc.conf", "ifconfig_" + DeviceName, "", -1);
	 runCommand("ifconfig " + DeviceName + " down");
         pushApply->setEnabled(false);
	 return;
    } 

    // Save the config
    pcbsd::Utils::setConfFileValue( "/etc/rc.conf", "ifconfig_lagg0", "", -1);
    pcbsd::Utils::setConfFileValue( "/etc/rc.conf", "ifconfig_" + DeviceName, \
	 "ifconfig_" + DeviceName + "=\"WPA " + ifConfigLine + "\"", -1);

    // Check if we need to enable LAGG
    if ( usingLagg )
      NetworkInterface::enableLagg(DeviceName);

    // Set perms on wpa_supplicant.conf
    system("chmod 600 /etc/wpa_supplicant.conf");

    // Restart the network
    buttonCancel->setEnabled(false);
    pcbsd::Utils::restartNetworking();

    buttonCancel->setEnabled(true);
    pushApply->setEnabled(false);
}


void wificonfigwidgetbase::updateWPASupp()
{
    bool listEmpty = false;
    QString tmp;

    QFile fileout( "/etc/wpa_supplicant.conf" );
    if ( fileout.open( QIODevice::WriteOnly ) ) {
       QTextStream streamout( &fileout );


       // Loop through all the devices we have now
       for(int curItem=0; curItem < 145; curItem++)
       {
         if ( SSIDList[curItem].isEmpty() ){
	   if(curItem==0){ listEmpty=true; }
	   break;
	 }
	 
         // Fix to prevent kernel panic on net restart
	 if ( curItem == 0 )
           streamout << "ctrl_interface=/var/run/wpa_supplicant\n\n";

	 if ( WPAONLY ) {
           streamout << "ap_scan=0\n";
           streamout << "fast_reauth=0\n";
           streamout << "network={\n ssid=\"" + SSIDList[curItem] + "\"\n";
         } else {
           if ( BSSID[curItem] )
             streamout << "network={\n bssid=" + SSIDList[curItem] + "\n";
           else
             streamout << "network={\n ssid=\"" + SSIDList[curItem] + "\"\n";
	 }
        
         streamout << " priority=" << 145 - curItem << "\n";

	 if ( ! WPAONLY )
           streamout << " scan_ssid=1\n";

         if ( SSIDEncType[curItem] == NO_ENCRYPTION )
         {
            streamout << " key_mgmt=NONE\n";
         } else if (SSIDEncType[curItem] == WEP_ENCRYPTION ) {
           // Ensure we don't end up with invalid key index
	   if ( WEPIndex[curItem] !=0 && WEPIndex[curItem] !=1 && WEPIndex[curItem] !=2 && WEPIndex[curItem] !=3 )
	      WEPIndex[curItem] = 0;
          
           streamout << " key_mgmt=NONE\n";
           streamout << " wep_tx_keyidx=" + tmp.setNum(WEPIndex[curItem]) + "\n";
           // Check if we are using a plaintext WEP or not
           if ( WEPHex[curItem] )
             streamout << " wep_key" + tmp.setNum(WEPIndex[curItem]) + "=" + WEPKey[curItem] + "\n";
           else
             streamout << " wep_key" + tmp.setNum(WEPIndex[curItem]) + "=\"" + WEPKey[curItem] + "\"\n";

         } else if (SSIDEncType[curItem] == WPA_ENCRYPTION ) {

            streamout << " psk=\"" + WPAPersonalKey[curItem] + "\"\n";	

         } else if (SSIDEncType[curItem] == WPAE_ENCRYPTION ) {
	    QString keyType;
	    if ( WPAEKeyMgmt[curItem] == KEYWPAEAP )
	       keyType="WPA-EAP";
	    if ( WPAEKeyMgmt[curItem] == KEY8021X )
	       keyType="IEEE8021X";

            if ( WPAEType[curItem] == 1) {
              streamout << " proto=RSN\n key_mgmt=" << keyType << "\n eap=TLS\n";
	      streamout << " identity=\"" + WPAEIdent[curItem] + "\"\n";
	      if ( ! WPAECACert[curItem].isEmpty() )
	        streamout << " ca_cert=\"" + WPAECACert[curItem] + "\"\n";
	      if ( ! WPAEClientCert[curItem].isEmpty() )
	        streamout << " client_cert=\"" + WPAEClientCert[curItem] + "\"\n";
	      streamout << " private_key=\"" + WPAEPrivKeyFile[curItem] + "\"\n";
	      streamout << " private_key_password=\"" + WPAEPassword[curItem] + "\"\n";

	    } else if (WPAEType[curItem] == 2) {
              streamout << " proto=RSN\n key_mgmt=" << keyType << "\n eap=TTLS\n";
	      streamout << " identity=\"" + WPAEIdent[curItem] + "\"\n";
	      streamout << " password=\"" + WPAEPassword[curItem] + "\"\n";
	      if ( ! WPAECACert[curItem].isEmpty() )
	        streamout << " ca_cert=\"" + WPAECACert[curItem] + "\"\n";
	      streamout << " phase2=\"auth=MD5\"\n";
	  
	    } else if (WPAEType[curItem] == 3) {
              streamout << " proto=RSN\n key_mgmt=" << keyType << "\n eap=PEAP\n";
	      streamout << " identity=\"" + WPAEIdent[curItem] + "\"\n";
	      streamout << " password=\"" + WPAEPassword[curItem] + "\"\n";
	      if ( ! WPAECACert[curItem].isEmpty() )
	        streamout << " ca_cert=\"" + WPAECACert[curItem] + "\"\n";
	      streamout << " phase1=\"peaplabel=0\"\n";
	      streamout << " phase2=\"auth=MSCHAPV2\"\n";
            }
         }

         streamout << "}\n\n";
       } // End of for loop

       fileout.close();
    } 

    // If no networks, clear out WPA supplicant.conf and reset rc.conf
    if ( listEmpty  && ! WPAONLY ){
       fileout.remove();
       pcbsd::Utils::setConfFileValue( "/etc/rc.conf", "ifconfig_lagg0", "", -1);
       pcbsd::Utils::setConfFileValue( "/etc/rc.conf", "ifconfig_" + DeviceName, "ifconfig_" + DeviceName + "=\"SYNCDHCP\"");
    }  

}


void wificonfigwidgetbase::slotCheckDHCPBox()
{
   if ( checkDHCP->isChecked() ) {
	groupBoxIP->setEnabled(false);
   } else {
	groupBoxIP->setEnabled(true);
   }
   
    pushApply->setEnabled(true);
}


// Function which is called at the program init
void wificonfigwidgetbase::setDevice( QString Device )
{ 
    
  // Connect our add / remove buttons for wifi
  connect( pushDown, SIGNAL( clicked() ), this, SLOT( slotMoveDown() ) ); 
  connect( pushUp, SIGNAL( clicked() ), this, SLOT( slotMoveUp() ) ); 
  connect( pushAddWifi, SIGNAL( clicked() ), this, SLOT( slotAddNewProfile() ) ); 
  connect( pushEditWifi, SIGNAL( clicked() ), this, SLOT( slotEditProfile() ) ); 
  connect( pushRemoveWifi, SIGNAL( clicked() ), this, SLOT( slotRemoveProfile() ) ); 
  connect( pushRescan, SIGNAL( clicked() ), this, SLOT( slotRescan() ) );
  connect( pushAddHidden, SIGNAL( clicked() ), this, SLOT( slotAddHiddenProfile() ) );
  connect( checkDHCP, SIGNAL( clicked() ), this, SLOT( slotCheckDHCPBox() ) );
  connect( groupCountryCode, SIGNAL( clicked() ), this, SLOT( slotCheckGlobalText() ) );

  // Save the device name for later
  DeviceName = Device;

  // Center the dialog window
  QDesktopWidget *d = QApplication::desktop();
  move(d->width() / 3, d->height() / 4);
 
  // Connect the slot to refresh
  QTimer::singleShot(100,  this,  SLOT(slotFinishLoading() ) );
  
  // Check if we are using the lagg0 device
  QSettings settings("PCBSD");
  usingLagg = settings.value("/pc-netmanager/useLagg", false).toBool();
 
  //Load the available wifi access points
  slotRescan();

}

void wificonfigwidgetbase::loadCountryCodes()
{
  QFile file("/etc/regdomain.xml");
  if (!file.open(QFile::ReadOnly | QFile::Text))
  return;

  QXmlStreamReader reader;
  QString code, name;
  int comboNum = 0;

  reader.setDevice(&file);
  reader.readNext();
  while (!reader.atEnd()) {
      if (reader.isStartElement()) {
          if (reader.name() == "country") {
	     name="";
	     code = reader.attributes().value("id").toString();
             while (!reader.atEnd()) {
                 if (reader.isEndElement() && reader.name() == "country")
		    break;
                 if (reader.isStartElement() && reader.name() == "name") {
	   	    name = reader.readElementText();
	         }
                 reader.readNext();
             }
	     if ( !name.isEmpty() && !code.isEmpty() && code != "DEBUG" )
		comboCountryCode->addItem(name + " (" + code.toLower() + ")" );
   	     if ( Country == code.toLower() )
	        comboCountryCode->setCurrentIndex(comboNum);

             comboNum++;
	  }
      }
      reader.readNext();
  }

  // Now if we change index, enable Apply Button
  connect( comboCountryCode, SIGNAL( currentIndexChanged(int) ), this, SLOT( slotCheckGlobalText() ) );
}

void wificonfigwidgetbase::slotRescan()
{
    QString strength, ssid, security, FileLoad;
    QStringList ifconfout, ifline;    
    int foundItem = 0;

    // Clear the list box and disable the add button
    listNewWifi->clear();
    pushAddWifi->setEnabled(false);
            
    // Start the scan and get the output
    ifconfout = pcbsd::Utils::runShellCommand("ifconfig -v " + DeviceName + " up list scan");

    //display the info for each wifi access point
    for(int i=1; i<ifconfout.size(); i++){    //Skip the header line by starting at 1
      ifline = NetworkInterface::parseWifiScanLine(ifconfout[i],true); //get a single line
      //save the info for this wifi
      //qDebug() << "ifconfig run";
      ssid = ifline[0];
      strength = ifline[4];
      //determine the icon based on if there is security encryption
      security = ifline[6]; //NetworkInterface::getWifiSecurity(ssid,DeviceName);
      if(security.contains("None")){
	FileLoad = ":object-unlocked.png";
      }else{
	FileLoad = ":object-locked.png";
      }
      //qDebug() << "ssid check";
      //Make sure we do not display blank ssid entry points
      if( !ssid.isEmpty() ){

      // qDebug()<<"Checking for duplicate SSID's";
      bool duplicateSSID = false;
      int dupRow;
      for ( int z = 0 ; z < listNewWifi->count() ; z++){
        //qDebug() << listNewWifi->item(z)->text() << ssid;
	if ( listNewWifi->item(z)->text().contains(ssid+" (") ){
          dupRow = z;
          duplicateSSID = true;
          break;
	}
      }
      if( !duplicateSSID ){
        //Add the wifi access point to the list
        listNewWifi->addItem(new QListWidgetItem(QIcon(FileLoad), ssid + " (signal: " +strength + ")") );
        foundItem = 1; //set the flag for wifi signals found 
      }else{
        //Check if the new signal strength is greater, replace if it is
        QString oldStrength = listNewWifi->item(dupRow)->text();
        oldStrength = oldStrength.section("(signal:",1,1).section("%)",0,0).simplified();
        if( strength.section("%",0,0).toInt() > oldStrength.toInt() ){
          //New signal strength is greater - replace the string
          qDebug() << "Changing signal strength for "+ssid+" to reflect a stronger signal found";
          //qDebug() << strength << oldStrength;
          listNewWifi->item(dupRow)->setText( ssid+" (signal: "+strength+")" );
	}
      }
      } //End of check for empty SSID entry points 
    }
    
    if ( foundItem == 1 ){
      listNewWifi->setCurrentRow(-1);
      pushAddWifi->setEnabled(true);
    } else {
      pushAddWifi->setEnabled(false);
    }

}

// Slot which moves a SSID higher in the connection priority
void wificonfigwidgetbase::slotMoveUp()
{
    QString tmpString;
    int tmpInt;
    bool tmpBool;
    int curRow = 1;
    bool MovedItem = false;

    // Check if we have a selection to work with
    if ( listWifi->currentRow() != -1 )
    {
      // Locate the SSID we want to remove
      QString editSSID = listWifi->item(listWifi->currentRow())->text();
      for ( int i = 0; i < 145; i++)
      {
          if ( SSIDList[i] == editSSID && i != 0 )
          {
            // We found the ssid we need to move up
            tmpString = SSIDList[i-1];
            SSIDList[i-1] = SSIDList[i] ; 
            SSIDList[i] = tmpString; 
            
            tmpInt = SSIDEncType[i-1];
            SSIDEncType[i-1] = SSIDEncType[i];
            SSIDEncType[i] = tmpInt;

            tmpBool = BSSID[i-1];
            BSSID[i-1] = BSSID[i];
            BSSID[i] = tmpBool;

            tmpString = WEPKey[i-1];
            WEPKey[i-1] = WEPKey[i];
            WEPKey[i] = tmpString;

            tmpInt = WEPIndex[i-1];
            WEPIndex[i-1] = WEPIndex[i];
            WEPIndex[i] = tmpInt;

            tmpBool = WEPHex[i-1];
            WEPHex[i-1] = WEPHex[i];
            WEPHex[i] = tmpBool;

            tmpString = WPAPersonalKey[i-1];
            WPAPersonalKey[i-1] = WPAPersonalKey[i];
            WPAPersonalKey[i] = tmpString;

            tmpString = WPAEIdent[i-1];
            WPAEIdent[i-1] = WPAEIdent[i];
            WPAEIdent[i] = tmpString;

            tmpString = WPAECACert[i-1];
            WPAECACert[i-1] = WPAECACert[i];
            WPAECACert[i] = tmpString;

            tmpString = WPAEClientCert[i-1];
            WPAEClientCert[i-1] = WPAEClientCert[i];
            WPAEClientCert[i] = tmpString;

            tmpString = WPAEPrivKeyFile[i-1];
            WPAEPrivKeyFile[i-1] = WPAEPrivKeyFile[i];
            WPAEPrivKeyFile[i] = tmpString;

            tmpString = WPAEPassword[i-1];
            WPAEPassword[i-1] = WPAEPassword[i];
            WPAEPassword[i] = tmpString;

            tmpInt = WPAEType[i-1];
            WPAEType[i-1] = WPAEType[i];
            WPAEType[i] = tmpInt;

            tmpInt = WPAEKeyMgmt[i-1];
            WPAEKeyMgmt[i-1] = WPAEKeyMgmt[i];
            WPAEKeyMgmt[i] = tmpInt;

            curRow=i-1;
            MovedItem=true;
            break;
          }
      }

      // Refresh the SSID list and enable the apply button
      if ( MovedItem) {
        slotRefreshSSIDList();
        pushApply->setEnabled(true);
        listWifi->setCurrentRow(curRow);
      }
    }
}


// Slot which moves a SSID lower in the connection priority
void wificonfigwidgetbase::slotMoveDown()
{
    QString tmpString;
    int tmpInt;
    bool tmpBool;
    int curRow = 1;
    bool MovedItem = false;

    // Check if we have a selection to work with
    if ( listWifi->currentRow() != -1 )
    {
      // Locate the SSID we want to remove
      QString editSSID = listWifi->item(listWifi->currentRow())->text();
      for ( int i = 0; i < 145; i++)
      {
          if ( SSIDList[i] == editSSID && ! SSIDList[i+1].isEmpty())
          {
            // We found the ssid we need to move down
            tmpString = SSIDList[i+1];
            SSIDList[i+1] = SSIDList[i] ; 
            SSIDList[i] = tmpString; 
            
            tmpInt = SSIDEncType[i+1];
            SSIDEncType[i+1] = SSIDEncType[i];
            SSIDEncType[i] = tmpInt;

            tmpBool = BSSID[i+1];
            BSSID[i+1] = BSSID[i];
            BSSID[i] = tmpBool;

            tmpString = WEPKey[i+1];
            WEPKey[i+1] = WEPKey[i];
            WEPKey[i] = tmpString;

            tmpInt = WEPIndex[i+1];
            WEPIndex[i+1] = WEPIndex[i];
            WEPIndex[i] = tmpInt;

            tmpBool = WEPHex[i+1];
            WEPHex[i+1] = WEPHex[i];
            WEPHex[i] = tmpBool;

            tmpString = WPAPersonalKey[i+1];
            WPAPersonalKey[i+1] = WPAPersonalKey[i];
            WPAPersonalKey[i] = tmpString;

            tmpString = WPAEIdent[i+1];
            WPAEIdent[i+1] = WPAEIdent[i];
            WPAEIdent[i] = tmpString;

            tmpString = WPAECACert[i+1];
            WPAECACert[i+1] = WPAECACert[i];
            WPAECACert[i] = tmpString;

            tmpString = WPAEClientCert[i+1];
            WPAEClientCert[i+1] = WPAEClientCert[i];
            WPAEClientCert[i] = tmpString;

            tmpString = WPAEPrivKeyFile[i+1];
            WPAEPrivKeyFile[i+1] = WPAEPrivKeyFile[i];
            WPAEPrivKeyFile[i] = tmpString;

            tmpString = WPAEPassword[i+1];
            WPAEPassword[i+1] = WPAEPassword[i];
            WPAEPassword[i] = tmpString;

            tmpInt = WPAEType[i+1];
            WPAEType[i+1] = WPAEType[i];
            WPAEType[i] = tmpInt;

            tmpInt = WPAEKeyMgmt[i+1];
            WPAEKeyMgmt[i+1] = WPAEKeyMgmt[i];
            WPAEKeyMgmt[i] = tmpInt;

            curRow=i+1;
	    MovedItem=true;
            break;
          }
      }

      // Refresh the SSID list and enable the apply button
      if ( MovedItem) {
        slotRefreshSSIDList();
        pushApply->setEnabled(true);
        listWifi->setCurrentRow(curRow);
       }

    }
}

void wificonfigwidgetbase::slotEditProfile()
{
    bool foundSSID=false;
    int curItem=0;

    // Check if we have a selection to work with
    if ( listWifi->currentRow() != -1 )
    {
      // Locate the SSID we want to remove
      QString editSSID = listWifi->item(listWifi->currentRow())->text();
      for ( int i = 0; i < 145; i++)
      {
          if ( SSIDList[i] == editSSID )
          {
            // We found the ssid we need to edit
            foundSSID=true; 
            curItem=i;
            break;
          }
      }

      // Lets go ahead and open the edit dialog on this SSID
      if (foundSSID)
      {
          // Set our internal flag that this is an edit on an existing device
          wifiselect = new wifiselectiondialog();
          wifiselect->init(DeviceName);
          wifiselect->setWPAOnly(WPAONLY);

          // Check the type of SSID this is, and issue appropriate edit 
          if ( SSIDEncType[curItem] == NO_ENCRYPTION) {
             wifiselect->initEdit(SSIDList[curItem], BSSID[curItem]);
          }
          if ( SSIDEncType[curItem] == WEP_ENCRYPTION) {
             wifiselect->initEdit(SSIDList[curItem], BSSID[curItem], WEPKey[curItem], WEPIndex[curItem], WEPHex[curItem]);
          }
          if ( SSIDEncType[curItem] == WPA_ENCRYPTION) {
             wifiselect->initEdit(SSIDList[curItem], BSSID[curItem], WPAPersonalKey[curItem]);
          }
          if ( SSIDEncType[curItem] == WPAE_ENCRYPTION) {
             wifiselect->initEdit(SSIDList[curItem], BSSID[curItem], WPAEType[curItem], WPAEIdent[curItem], WPAECACert[curItem], WPAEClientCert[curItem], WPAEPrivKeyFile[curItem], WPAEPassword[curItem], WPAEKeyMgmt[curItem]);
          }


          // Connect our delete signal, which runs before we add a new SSID
          connect( wifiselect, SIGNAL( signalDeleteSSID(QString) ), this, SLOT( slotRemoveProfileSSID(QString) ) ); 

          // Connect our save signals
          connect( wifiselect, SIGNAL( signalSavedOpen(QString, bool) ), this, SLOT( slotAddNewProfileOpen(QString, bool) ) );
          connect( wifiselect, SIGNAL( signalSavedWEP( QString, bool, QString, int, bool ) ), this, SLOT( slotAddNewProfileWEP( QString, bool, QString, int, bool) ) );
   connect( wifiselect, SIGNAL( signalSavedWPA(QString, bool, QString) ), this, SLOT( slotAddNewProfileWPA(QString, bool, QString) ) );
   	  connect( wifiselect, SIGNAL( signalSavedWPAE(QString, bool, int, QString, QString, QString, QString, QString, int) ), this, SLOT ( slotAddNewProfileWPAE(QString, bool, int, QString, QString, QString, QString, QString, int) ) );

          wifiselect->exec();
      }


    }
}


void wificonfigwidgetbase::slotRemoveProfile()
{
    // Check if we have a selection to work with
    if ( listWifi->currentRow() != -1 )
    {
      // Locate the SSID we want to remove
      QString removeSSID = listWifi->item(listWifi->currentRow())->text();
      slotRemoveProfileSSID(removeSSID);

      // Refresh the SSID list and enable the apply button
      slotRefreshSSIDList();
      pushApply->setEnabled(true);

    }
  
}

void wificonfigwidgetbase::slotAddHiddenProfile(){
  //Prompt for the desired SSID
  bool ok;
  QString ssidc = QInputDialog::getText(this,tr("Network Name"),tr("Please enter the name of the network you wish to add"), QLineEdit::Normal,"",&ok);
  //check if a name was given
  if( ok && !ssidc.isEmpty()){
    slotAddNewProfileSSID(ssidc.simplified());
  }
}

void wificonfigwidgetbase::slotAddNewProfile()
{
   //Get the selected SSID
   if( (listNewWifi->currentRow())!=-1 ){
    QString line = listNewWifi->item(listNewWifi->currentRow())->text();
    QString ssidc = line.section(" (",0,0,QString::SectionSkipEmpty);
    slotAddNewProfileSSID(ssidc);
   }else{
     QMessageBox::warning(this,"No Wifi Point Selected","Please select a wireless network to add");
   }    
}

void wificonfigwidgetbase::slotAddNewProfileSSID(QString ssidc){ 
  /*  ssidc - SSID of the network to add
  */

    //Get the Security Type
    QString sec = NetworkInterface::getWifiSecurity(ssidc,DeviceName);

    //Save the SSID for the future save slots
    saveSSID = ssidc;

    //Open the proper security dialog and link it to the save slots
    if(sec.contains("None")){
      slotAddNewProfileOpen(ssidc,false); //call the function to save the variables (no security settings)
    }else if(sec.contains("WEP")){
     dialogWEP = new wepConfig();
     connect( dialogWEP, SIGNAL( saved(QString, int, bool) ), this, SLOT( slotWEPSave(QString, int, bool) ) );
     dialogWEP->exec();
    }else if(sec.contains("WPA")){ //both WPA and WPA2 encryption
     dialogWPA = new dialogWPAPersonal();
     connect( dialogWPA, SIGNAL( saved(QString) ), this, SLOT( slotWPAPSave(QString) ) );
     dialogWPA->exec();
    }else{
      //Put error handling here
      slotAddNewProfileOpen(ssidc,false); //call the function to save the variables (no security settings)
      if ( ! WPAONLY )
        QMessageBox::warning(this,"Unknown Wifi Security","The security for this wifi access point could not be automatically determined.\n Please edit the profile to manually set the proper security encryption.");
    }
}

void wificonfigwidgetbase::slotWEPSave(QString newkey, int newindex, bool hexkey)
{
  //get the ssid
  //QString line = listNewWifi->item(listNewWifi->currentRow())->text();
  //QString ssidc = line.section(" (",0,0,QString::SectionSkipEmpty);
  QString ssidc = saveSSID;
  saveSSID.clear();
  //Add the profile
  slotAddNewProfileWEP(ssidc,false,newkey,newindex,hexkey);
}

void wificonfigwidgetbase::slotWPAPSave(QString newkey)
{
  //get the ssid
  //QString line = listNewWifi->item(listNewWifi->currentRow())->text();
  //QString ssidc = line.section(" (",0,0,QString::SectionSkipEmpty);
  QString ssidc = saveSSID;
  saveSSID.clear();
  //Add the profile
  slotAddNewProfileWPA(ssidc,false,newkey);
}


void wificonfigwidgetbase::runCommand( QString Command )
{
        FILE *file = popen(Command.toLatin1(),"r"); 
	pclose(file);
}

void wificonfigwidgetbase::slotRemoveProfileSSID(QString RemoveSSID)
{
   bool foundSSID=false;

   // Loop through, remove selected SSID, and move any entries after it up
   for (int remItem = 0; remItem < 145; remItem++)
   {

     if ( RemoveSSID == SSIDList[remItem] ) {
       foundSSID=true;
     }

     if(foundSSID) 
     {
       SSIDList[remItem] = SSIDList[remItem+1] ; 
       SSIDEncType[remItem] = SSIDEncType[remItem+1];
       BSSID[remItem] = BSSID[remItem+1];
       WEPKey[remItem] = WEPKey[remItem+1];
       WEPIndex[remItem] = WEPIndex[remItem+1];
       WEPHex[remItem] = WEPHex[remItem+1];
       WPAPersonalKey[remItem] = WPAPersonalKey[remItem+1];
       WPAEIdent[remItem] = WPAEIdent[remItem+1];
       WPAECACert[remItem] = WPAECACert[remItem+1];
       WPAEClientCert[remItem] = WPAEClientCert[remItem+1];
       WPAEPrivKeyFile[remItem] = WPAEPrivKeyFile[remItem+1];
       WPAEPassword[remItem] = WPAEPassword[remItem+1];
       WPAEType[remItem] = WPAEType[remItem+1];
       WPAEKeyMgmt[remItem] = WPAEKeyMgmt[remItem+1];
     }

     if ( SSIDList[remItem+1].isEmpty() )
     {
       break;
     }

   }

   
}

void wificonfigwidgetbase::slotAddNewProfileOpen(QString SSID, bool isBSSID)
{
   for (int dupItem = 0; dupItem < 145; dupItem++)
   {
      if ( SSID == SSIDList[dupItem] ) {
          QMessageBox::warning( this, tr("Error"), tr("You already have a wireless network with this SSID! Please remove it first.\n") );
          return;
      }
   }


   int curItem=0;

   while( ! SSIDList[curItem].isEmpty() && curItem < 145 )
   {
     curItem++;
   }

    // Find which current item we are working on

    SSIDList[curItem] = SSID;
    BSSID[curItem] = isBSSID;
    
    // Refresh the SSID listbox and enable the apply button
    slotRefreshSSIDList();
    slotCheckGlobalText();
}

void wificonfigwidgetbase::slotAddNewProfileWEP( QString SSID, bool isBSSID, QString newKey, int newIndex, bool hexkey )
{
   for (int dupItem = 0; dupItem < 145; dupItem++)
   {
      if ( SSID == SSIDList[dupItem] ) {
        QMessageBox::warning( this, tr("Error"), tr("You already have a wireless network with this SSID! Please remove it first.\n") );
        return;
      }
   }


   int curItem=0;

   while( ! SSIDList[curItem].isEmpty() && curItem < 145 )
   {
     curItem++;
   }

    // Find which current item we are working on
  
    SSIDList[curItem] = SSID;
    SSIDEncType[curItem] = WEP_ENCRYPTION;
    BSSID[curItem] = isBSSID;
    WEPKey[curItem] = newKey;
    WEPIndex[curItem] = newIndex;
    WEPIndex[curItem]--;
    WEPHex[curItem] = hexkey;

    // Refresh the SSID listbox and enable the apply button
    slotRefreshSSIDList();
    slotCheckGlobalText();

}


void wificonfigwidgetbase::slotAddNewProfileWPA( QString SSID, bool isBSSID, QString newKey )
{
   for (int dupItem = 0; dupItem < 145; dupItem++)
   {
      if ( SSID == SSIDList[dupItem] ) {
        QMessageBox::warning( this, tr("Error"), tr("You already have a wireless network with this SSID! Please remove it first.\n") );
        return;
      }
   }

   int curItem=0;

   while( ! SSIDList[curItem].isEmpty() && curItem < 145 )
   {
     curItem++;
   }


   SSIDList[curItem] = SSID;
   SSIDEncType[curItem] = WPA_ENCRYPTION;
   BSSID[curItem] = isBSSID;
   WPAPersonalKey[curItem] = newKey;
   
   // Refresh the SSID listbox and enable the apply button
   slotRefreshSSIDList();
   slotCheckGlobalText();
}


void wificonfigwidgetbase::slotAddNewProfileWPAE( QString SSID, bool isBSSID, int type, QString EAPIdent, QString CACert, QString ClientCert, QString PrivKeyFile, QString PrivKeyPass, int keyMgmt )
{

   for (int dupItem = 0; dupItem < 145; dupItem++)
   {
      if ( SSID == SSIDList[dupItem] ) {
        QMessageBox::warning( this, tr("Error"), tr("You already have a wireless network with this SSID! Please remove it first.\n") );
        return;
      }
   }

   int curItem=0;

   while( ! SSIDList[curItem].isEmpty() && curItem < 145 )
   {
     curItem++;
   }


   SSIDList[curItem] = SSID;
   SSIDEncType[curItem] = WPAE_ENCRYPTION;
   BSSID[curItem] = isBSSID;
   WPAEType[curItem] = type;
   WPAEIdent[curItem] = EAPIdent;
   WPAECACert[curItem] = CACert;
   WPAEClientCert[curItem] = ClientCert;
   WPAEPrivKeyFile[curItem] = PrivKeyFile;
   WPAEPassword[curItem] = PrivKeyPass;
   WPAEKeyMgmt[curItem] = keyMgmt;

   // Refresh the SSID listbox and enable the apply button
   slotRefreshSSIDList();
   slotCheckGlobalText();

}


void wificonfigwidgetbase::slotCheckDisabled()
{
	if ( checkDisableWireless->isChecked() ) 
        {
	    tabMainWidget->setEnabled(false);
	} else {
	    tabMainWidget->setEnabled(true);
	}
	
	 pushApply->setEnabled(true); 
}


void wificonfigwidgetbase::slotOK()
{
    if ( pushApply->isEnabled() )
    {
        slotApply();
	sleep(1);
    }
        close();
}


void wificonfigwidgetbase::slotMACClicked()
{
   if ( checkMAC->isChecked() ) {
	groupMAC->setEnabled(false);
   } else {
	groupMAC->setEnabled(true);
   }
    pushApply->setEnabled(true);
}


QString wificonfigwidgetbase::getLineFromCommandOutput( QString command )
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


QString wificonfigwidgetbase::getNetmaskForIdent( QString ident )
{
   NetworkInterface ifr(ident);
   return ifr.netmaskAsString();
}


QString wificonfigwidgetbase::getOutErrorsForIdent( QString ident )
{
   NetworkInterface ifr(ident);
   return QString::number(ifr.errorsRx());
}


QString wificonfigwidgetbase::getOutPacketsForIdent( QString ident )
{
   NetworkInterface ifr(ident);
   return QString::number(ifr.packetsRx());
}


QString wificonfigwidgetbase::getInErrorsForIdent( QString ident )
{
   NetworkInterface ifr(ident);
   return QString::number(ifr.errorsTx());
}


QString wificonfigwidgetbase::getInPacketsForIdent( QString ident )
{
   NetworkInterface ifr(ident);
   return QString::number(ifr.packetsTx());
}


QString wificonfigwidgetbase::getStatusForIdent( QString ident )
{
   NetworkInterface ifr(ident);
   return ifr.mediaStatusAsString();
}


QString wificonfigwidgetbase::getMacForIdent( QString ident )
{
   NetworkInterface ifr(ident);
   return ifr.macAsString();
}


QString wificonfigwidgetbase::getIpForIdent( QString ident )
{
   NetworkInterface ifr(ident);
   return ifr.ipAsString();
}


QString wificonfigwidgetbase::getNameForIdent( QString ident )
{
   NetworkInterface ifr(ident);
   return ifr.desc();
}

void wificonfigwidgetbase::loadInfo()
{
  QString tmp;

  // Get all the device information for the info tab
  if ( textInfoName->text().isEmpty() ) {
    if ( DeviceName.indexOf("wlan") != -1 )
    {
      tmp = NetworkInterface::getWifiParent(DeviceName);

      // Get HW Identify line
      tmp = getNameForIdent(tmp);

      // If the name comes up bogus for some reason
      if( tmp.isEmpty() ) {
         tmp = tr("Unknown Wireless Device");
      }

      tmp.truncate(20);
      textInfoName->setText(tmp);

    } else {
      textInfoName->setText(getNameForIdent(DeviceName));
    }
  }


  if ( usingLagg ) {
    textIP->setText(getIpForIdent("lagg0") );
    textNetmask->setText(getNetmaskForIdent( "lagg0" ) );
    textGateway->setText(getGatewayForIdent( "lagg0" ) );
  } else {
    textIP->setText(getIpForIdent(DeviceName) );
    textNetmask->setText(getNetmaskForIdent( DeviceName ) );
    textGateway->setText(getGatewayForIdent( DeviceName ) );
  }

  textMac->setText(getMacForIdent( DeviceName ) );
  textStatus->setText(getStatusForIdent( DeviceName ) );
  textIPv6->setText(getIPv6ForIdent( DeviceName) );
  tmp = getMediaForIdent(DeviceName);
  tmp.truncate(20);
  textMedia->setText(tmp);
  
  // Get the packet status for this device
  textPacketsIn->setText(getInPacketsForIdent( DeviceName) );
  textPacketsInErrors->setText( getInErrorsForIdent( DeviceName) );
  textPacketsOut->setText(getOutPacketsForIdent( DeviceName) );
  textPacketsOutErrors->setText(getOutErrorsForIdent( DeviceName) );

  // Connect the slot to refresh
  QTimer::singleShot(3000,  this,  SLOT(loadInfo() ) );
}


void wificonfigwidgetbase::showInfo()
{
     tabMainWidget->setCurrentWidget( tabMainWidget->widget(2) );
}


void wificonfigwidgetbase::slotFinishLoading()
{
   QString tmp, tmp2;
   bool ok;

   // Get the ifconfig string
   if ( usingLagg )
     tmp = pcbsd::Utils::getConfFileValue( "/etc/rc.conf", "ifconfig_lagg0=", 1 );

   if ( tmp.isEmpty() )
     tmp = pcbsd::Utils::getConfFileValue( "/etc/rc.conf", "ifconfig_" + DeviceName + "=", 1 );

   if ( tmp.isEmpty() || tmp.indexOf("OFF") != -1 )  {
      checkDisableWireless->setChecked(true);
      tabMainWidget->setEnabled(false);
   } else if ( tmp.indexOf("DHCP") != -1 ) {
      checkDHCP->setChecked(true);
      lineNetmask->setText("255.255.255.0");
      slotCheckDHCPBox();
      
            // Look for the mac address change
	if ( tmp.indexOf("ether") != -1 ) {
		tmp2 = tmp;
		tmp2 = tmp2.remove(0, tmp.indexOf("ether") + 6 );
		//if ( tmp.indexOf(" ") != -1 ) {
		//  tmp2.truncate(tmp.indexOf(" ") );
		//}
		lineMAC->setText(tmp2);
		checkMAC->setChecked(false);
		groupMAC->setEnabled(true);
	} 


   } else {
     // Look for an IP configuration
     tmp2 = tmp;
     tmp2 = tmp2.simplified();

     if ( tmp.indexOf("laggport") != -1 )
       tmp2.remove(0, tmp2.lastIndexOf("laggport") + 9);

     if ( tmp.indexOf("country") != -1 )
       tmp2.remove(0, tmp2.lastIndexOf("country") + 10);

     if ( tmp.indexOf("WPA") == 0)
       tmp2.remove(0, tmp2.indexOf(" ") + 1);

     if ( ! tmp2.isEmpty()) {
        // Get the IP address first
        lineIP->setText(tmp2.section(" ", 0, 0));

          // Look for the netmask
          if ( tmp.indexOf("netmask") != -1 ) {
             tmp2 = tmp;
             tmp2 = tmp2.remove(0, tmp.indexOf("netmask") + 8 );
             if ( tmp.indexOf("ether") != -1 ) {
                tmp2.truncate(tmp.indexOf("ether") );
             }
             tmp2 = tmp2.simplified();

             lineNetmask->setText(tmp2);

          } else {
             // No netmask specified, default it instead
             lineNetmask->setText("255.255.255.0");
          }
      }
   }
   
   // Look for a country code
   tmp = pcbsd::Utils::getConfFileValue( "/etc/rc.conf", "ifconfig_" + DeviceName + "=", 1 );
   if ( tmp.indexOf("country ") != -1 ) {
       tmp.remove(0, tmp.lastIndexOf("country") + 8);
       Country = tmp.section(" ", 0,0);
       groupCountryCode->setChecked(true);
   }

   // Look for the wpa_supplicant.conf file
   QFile file( "/etc/wpa_supplicant.conf" );
   if ( file.open( QIODevice::ReadOnly ) ) {
      int curItem=0;
      QTextStream stream( &file );
      QString line;
      while ( !stream.atEnd() ) {

            line = stream.readLine(); // line of text excluding '\n'

	    // Check if we are at the end of a network profile block
            if ( line.indexOf("}") == 0)
	    {
		curItem++;
	    }

 	    if ( line.indexOf(" ssid=") != -1 )
 	    {
		tmp2 = line.remove(0, line.indexOf("\"") +1 );
		tmp2.truncate(tmp2.indexOf("\"") );
		SSIDList[curItem] = tmp2;
		BSSID[curItem] = false;
		SSIDEncType[curItem]=NO_ENCRYPTION;
   	    }

 	    if ( line.indexOf(" bssid=") != -1 )
 	    {
		tmp2 = line.remove(0, line.indexOf("=") +1 );
		SSIDList[curItem] = tmp2;
		BSSID[curItem] = true;
		SSIDEncType[curItem]=NO_ENCRYPTION;
   	    }
	
	
 	    // Check if we are using a WEP key for this network
	    if ( line.indexOf("wep_key") != -1 )
	    {		       
                if (line.indexOf("\"") != -1 )
                {
		   tmp2 = line.remove(0, line.indexOf("\"") + 1 );
		   tmp2.truncate(tmp2.indexOf("\""));
                   WEPHex[curItem]=false;
                } else {
		   tmp2 = line.remove(0, line.indexOf("=") + 1 );
                   WEPHex[curItem]=true;
                }
		// Save the keycode
		WEPKey[curItem] = tmp2;
		SSIDEncType[curItem]=WEP_ENCRYPTION;
 	    }

           // Check for the WEP tx key id
	   if ( line.indexOf("wep_tx_keyidx") != -1 )
	   {		       
		tmp2 = line.remove(0, line.indexOf("wep_tx_keyidx") +1 );
		// Save the keycode
		tmp2.truncate(tmp2.indexOf("=") );
		ok = tmp2.toInt(&ok);
		if ( ok ) {
		    WEPIndex[curItem] = tmp2.toInt(&ok);
		    WEPIndex[curItem]++;
		}
	   }
		  
           // Check if we are using WPA-Personal
	   if ( line.indexOf("psk=") != -1 )
	   {		       
		tmp2 = line.remove(0, line.indexOf("\"") + 1 );
		tmp2.truncate( tmp2.indexOf("\"") );
			 
			
    	        // Save the keycode
		WPAPersonalKey[curItem] = tmp2;
		SSIDEncType[curItem]=WPA_ENCRYPTION;
	   }
		    
	   // Check if we are using WPA-Enterprise
           if ( line.indexOf("eap=") != -1 )
	   {		       
		tmp2 = line.remove(0, line.indexOf("=") + 1 );

		//Check to see whic type of WPA-Ent we are using
		if ( tmp2 == "TLS") {
		    WPAEType[curItem] = 1;
		} else if (tmp2 == "TTLS" ) {
		    WPAEType[curItem] = 2;
		} else if (tmp2 == "PEAP" ) {
		    WPAEType[curItem] = 3;
		}
			
		SSIDEncType[curItem]=WPAE_ENCRYPTION;
	   }
		    
           // Check for a identity= line
           if ( line.indexOf("identity=") != -1 )
	   {		       
		tmp2 = line.remove(0, line.indexOf("\"") + 1 );
		tmp2.truncate( tmp2.indexOf("\"") );

		// Save the keycode
		WPAEIdent[curItem] = tmp2;
	   }
		    
	   // Check for a ca_cert= line
           if ( line.indexOf("ca_cert=") != -1 )
	   {		       
		tmp2 = line.remove(0, line.indexOf("\"") + 1 );
		tmp2.truncate( tmp2.indexOf("\"") );

		// Save the keycode
		WPAECACert[curItem] = tmp2;
	   }
	
	   // Check for a client_cert= line
	   if ( line.indexOf("client_cert=") != -1 )
	   {		       
		tmp2 = line.remove(0, line.indexOf("\"") + 1 );
		tmp2.truncate( tmp2.indexOf("\"") );

		// Save the keycode
		WPAEClientCert[curItem] = tmp2;
	   }
		    
          // Check for a private_key= line
           if ( line.indexOf("private_key=") != -1 )
	   {		       
		tmp2 = line.remove(0, line.indexOf("\"") + 1 );
		tmp2.truncate( tmp2.indexOf("\"") );

		// Save the keycode
		WPAEPrivKeyFile[curItem] = tmp2;
	   }
	    
          // Check for a private_key_passwd= line
          if ( line.indexOf("private_key_passwd=") != -1 )
          {		       
		tmp2 = line.remove(0, line.indexOf("\"") + 1 );
		tmp2.truncate( tmp2.indexOf("\"") );

		// Save the keycode
		WPAEPassword[curItem] = tmp2;
          }

          // Figure out the key mgmt
          if ( line.indexOf("key_mgmt=WPA-EAP") != -1 )
		WPAEKeyMgmt[curItem] = KEYWPAEAP;
          if ( line.indexOf("key_mgmt=IEEE8021X") != -1 )
		WPAEKeyMgmt[curItem] = KEY8021X;
	
         // Check for a private_key_passwd= line
         if ( line.indexOf("password=") != -1 )
         {		       
		tmp2 = line.remove(0, line.indexOf("\"") + 1 );
		tmp2.truncate( tmp2.indexOf("\"") );

		// Save the keycode
		WPAEPassword[curItem] = tmp2;
         }

      }

   }


   // Refresh the ssid profile list
   slotRefreshSSIDList();  

   // Load the list of country codes we can set
   loadCountryCodes();

   
   // Start loading the info tab
   loadInfo();

   pushApply->setEnabled(false);
}


// Slot which reads the arrays and displays the list of WIFI profiles
void wificonfigwidgetbase::slotRefreshSSIDList()
{

  listWifi->clear();

  int curItem=0;

  while( ! SSIDList[curItem].isEmpty() )
  {
     if ( SSIDEncType[curItem] == NO_ENCRYPTION )
     {
       listWifi->addItem(new QListWidgetItem(QIcon(":object-unlocked.png"), SSIDList[curItem] ) );
     } else {

       listWifi->addItem(new QListWidgetItem(QIcon(":object-locked.png"), SSIDList[curItem] ) );
     }

     curItem++;

     // We only need 145 wireless networks
     if ( curItem > 145 )
     {
	break;
     }
  }

}


QString wificonfigwidgetbase::getIPv6ForIdent( QString ident )
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


QString wificonfigwidgetbase::getGatewayForIdent( QString ident )
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


QString wificonfigwidgetbase::getMediaForIdent( QString ident )
{
    	QString command = "nice ifconfig " +ident + " | grep media:";
	QString inputLine = getLineFromCommandOutput(command);
	QString status = "";
	
	if (inputLine != "" && inputLine.indexOf("media:") != -1){
   	  status = inputLine.remove(0, inputLine.indexOf("media:") + 7);
	}
	
	return status;
}


void wificonfigwidgetbase::slotCheckGlobalText()
{
    pushApply->setEnabled(true);
}


void wificonfigwidgetbase::setModeWPAOnly()
{
    WPAONLY = true;
    tabMainWidget->removeTab(2);
    tabMainWidget->removeTab(1);
    pushRescan->setHidden(true);
    labelAvailWifi->setHidden(true);
    checkDisableWireless->setHidden(true);
    listNewWifi->setHidden(true);
    pushAddWifi->setHidden(true);
    pushAddHidden->setText(tr("Add &network"));
    setWindowTitle("WPA Supplicant Configuration");
    tabMainWidget->setTabText(0, tr("WPA Configuration") );
}
