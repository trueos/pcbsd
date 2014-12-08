#include "wifiselectiondialog.h"
#include "ui_wifiselectiondialog.h"
#include <qtextstream.h>
#include <pcbsd-ui.h>
#include <QGroupBox>

void wifiselectiondialog::init(QString device)
{
   DeviceName = device;
    
   connect( pushScan, SIGNAL( clicked() ), this, SLOT(slotOpenWifiScan()) );
   connect( pushConnect, SIGNAL( clicked() ), this, SLOT(slotConnect()) );
   connect( pushCancel, SIGNAL( clicked() ), this, SLOT(slotCancel()) );
   connect( pushSecurity, SIGNAL( clicked() ), this, SLOT(slotCheckWhichSecurity()) );
}


void wifiselectiondialog::slotOpenWifiScan()
{
   dialogWifiscanssid = new wifiscanssid();
   dialogWifiscanssid->init(DeviceName);
   connect( dialogWifiscanssid, SIGNAL( saved(QString) ), this, SLOT( slotSSIDChanged(QString) ) );
   dialogWifiscanssid->exec();
}


void wifiselectiondialog::slotSSIDChanged( QString newSSID )
{
       lineSSID->setText(newSSID);
}

   
void wifiselectiondialog::slotCancel()
{
    close();
}

void wifiselectiondialog::slotConnect()
{
    QString selectedSSID;
    
    // Make sure a device was selected
    if (lineSSID->text().isEmpty())
    {
	QMessageBox::warning( this, tr("No SSID!"), tr("Error: You must select a wireless network to connect!\n") );
	return;
    }

    if ( (checkBSSID->isChecked() && lineSSID->text().length() != 17) || (checkBSSID->isChecked() && lineSSID->text().count(":") != 5) )
    {
	QMessageBox::warning( this, tr("Invalid BSSID!"), tr("Error: The specified BSSID appears invalid. It must be in the format xx:xx:xx:xx:xx:xx\n") );
	return;
    }

    // Check that we even have a WEP Key yet
    if (radioSecurityWEP->isChecked() && WEPKey.isEmpty() ) {
       QMessageBox::warning( this, tr("Warning"), tr("WEP is selected, but not configured!\nPlease configure your WEP key before saving!") );
       return;
    }

    // Check that we even have a WPA-Personal Key yet
    if (radioSecurityWPAPersonal->isChecked() && WPAPersonalKey.isEmpty() ) {
       QMessageBox::warning( this, tr("Warning"), tr("WPA-Personal is selected, but not configured!\nPlease configure your WPA key before saving!") );
       return;
    }

    // Check all the WPAEnt settings
    if (radioSecurityWPAEnt->isChecked() ) {

       // Check if the key is setup for WPA EAP-TLS
       if ( WPAEType == 1 && ( WPAEIdent.isEmpty() || WPAEPrivKeyFile.isEmpty() || WPAEPassword.isEmpty() ))
       {
          QMessageBox::warning( this, tr("Warning"), tr("WPA-Enterprise is selected, but not configured!\nPlease configure your WPA settings before saving!") );
          return;
       }

       // Check if the key is setup for WPA EAP-TTLS && EAP-PEAP
       if ( (WPAEType == 2 || WPAEType == 3) && ( WPAEIdent.isEmpty() || WPAEPassword.isEmpty() ) ) 
       {
          QMessageBox::warning( this, tr("Warning"), tr("WPA-Enterprise is selected, but not configured!\nPlease configure your WPA settings before saving!") );
          return;
       }

    } // End of WPAEnt setting check
    

    if( ! EditingSSID.isEmpty() )
    {
      // We are editing an existing SSID, issue delete command before adding new one
      emit signalDeleteSSID(EditingSSID);
    }
    
    selectedSSID = lineSSID->text();
    bool usingBSSID = checkBSSID->isChecked() ;
    
    // Emit the new network SSID the user selected
    if ( radioSecurityDisabled->isChecked() )
    {
        emit signalSavedOpen(selectedSSID, usingBSSID); 
    }

    if ( radioSecurityWEP->isChecked() )
    {
        emit signalSavedWEP(selectedSSID, usingBSSID, WEPKey, WEPIndex, WEPHex ); 
    }

    if ( radioSecurityWPAPersonal->isChecked() )
    {
        emit signalSavedWPA(selectedSSID, usingBSSID, WPAPersonalKey ); 
    }
    
    if ( radioSecurityWPAEnt->isChecked() )
    {
        emit signalSavedWPAE(selectedSSID, usingBSSID, WPAEType, WPAEIdent, WPAECACert, WPAEClientCert, WPAEPrivKeyFile, WPAEPassword, WPAEKeyMgmt ); 
    }

    close();
}

// Overloaded function which lets us set editing variables using WPA-Ent
void wifiselectiondialog::initEdit(QString selectedSSID, bool usingBSSID, int editWPAEType, QString editWPAEIdent, QString editWPAECACert, QString editWPAEClientCert, QString editWPAEPrivKeyFile, QString editWPAEPassword, int keyMgmt)
{
   pushConnect->setText(tr("Save"));
   EditingSSID=selectedSSID;
   SSID=selectedSSID;
   lineSSID->setText(selectedSSID);
   checkBSSID->setChecked(usingBSSID);
   radioSecurityWPAEnt->setChecked(true);
   WPAEType=editWPAEType; 
   WPAEIdent=editWPAEIdent; 
   WPAECACert=editWPAECACert; 
   WPAEClientCert=editWPAEClientCert; 
   WPAEPrivKeyFile=editWPAEPrivKeyFile; 
   WPAEPassword=editWPAEPassword; 
   WPAEKeyMgmt = keyMgmt;
}

// Overloaded function which lets us set editing variables using WPA-Personal
void wifiselectiondialog::initEdit(QString selectedSSID, bool usingBSSID, QString editWPAPersonalKey)
{
   pushConnect->setText(tr("Save"));
   EditingSSID=selectedSSID;
   SSID=selectedSSID;
   lineSSID->setText(selectedSSID);
   checkBSSID->setChecked(usingBSSID);
   radioSecurityWPAPersonal->setChecked(true);
   WPAPersonalKey=editWPAPersonalKey;
}

// Overloaded function which lets us set editing variables using WEP
void wifiselectiondialog::initEdit(QString selectedSSID, bool usingBSSID, QString editWEPKey, int editWEPIndex, bool editWEPHex)
{
   pushConnect->setText(tr("Save"));
   EditingSSID=selectedSSID;
   SSID=selectedSSID;
   lineSSID->setText(selectedSSID);
   checkBSSID->setChecked(usingBSSID);
   radioSecurityWEP->setChecked(true);
   WEPKey=editWEPKey;
   WEPIndex=editWEPIndex;
   WEPHex=editWEPHex;
}

// Overloaded function which lets us set what we are editing with no encryption
void wifiselectiondialog::initEdit(QString selectedSSID, bool usingBSSID)
{
   pushConnect->setText(tr("Save"));
   EditingSSID=selectedSSID;
   SSID=selectedSSID;
   lineSSID->setText(selectedSSID);
   checkBSSID->setChecked(usingBSSID);
   radioSecurityDisabled->setChecked(true);
}

void wifiselectiondialog::slotConfigWEP()
{
   dialogWEP = new wepConfig();
   
   if ( ! WEPKey.isEmpty() )
   {
       dialogWEP->setKey(WEPKey, WEPIndex, WEPHex);
   }
   
   connect( dialogWEP, SIGNAL( saved(QString, int, bool) ), this, SLOT( slotWEPChanged(QString, int, bool) ) ); 
   dialogWEP->exec();
}


void wifiselectiondialog::slotWEPChanged( QString newKey, int newIndex, bool hexkey )
{
    WEPKey = newKey;
    WEPIndex = newIndex;
    WEPHex = hexkey;
    // Check the status of the radio buttons
    slotCheckSecurityRadio();
    
}


void wifiselectiondialog::slotCheckSecurityRadio()
{
    // Check to see which radio button is checked and take appropriate actions
    
    if ( radioSecurityDisabled->isChecked() )
    {
	pushSecurity->setEnabled(false);
    } else if ( radioSecurityWEP->isChecked() ) {
	pushSecurity->setEnabled(true);
	// Check if the key is setup
	if ( ! WEPKey.isEmpty() )
	{
	    radioSecurityWEP->setText( tr("WEP (Configured)") );
	} else {
	    radioSecurityWEP->setText( tr("WEP") );
	}
    } else if ( radioSecurityWPAPersonal->isChecked() ) {
	pushSecurity->setEnabled(true);
	// Check if the key is setup
	if ( ! WPAPersonalKey.isEmpty() )
	{
	    radioSecurityWPAPersonal->setText( tr("WPA Personal (Configured)") );
	} else {
	    radioSecurityWPAPersonal->setText( tr("WPA Personal") );
	}
	
    } else if ( radioSecurityWPAEnt->isChecked() ) {
	pushSecurity->setEnabled(true);
	
	// Check if the key is setup
	if ( ! WPAEIdent.isEmpty() && ! WPAEPassword.isEmpty() ) {
	    radioSecurityWPAEnt->setText(tr("WPA Enterprise (Configured)") );
	} else {
	    radioSecurityWPAEnt->setText(tr("WPA Enterprise") );
	}

    }

}


void wifiselectiondialog::slotCheckWhichSecurity()
{
        if ( radioSecurityDisabled->isChecked() )
    {
	    return;
    } else if ( radioSecurityWEP->isChecked() ) {
	// Run the WEP config dialog
	slotConfigWEP();
    } else if ( radioSecurityWPAPersonal->isChecked() ) {
	slotConfigWPAP();
    } else if ( radioSecurityWPAEnt->isChecked() ) {
	slotConfigWPAE();
    }

}


void wifiselectiondialog::slotConfigWPAP()
{
    // Bring up the WPA-Personal config dialog
    
   dialogWPAP = new dialogWPAPersonal();
   
   if ( ! WPAPersonalKey.isEmpty() )
   {
       dialogWPAP->setKey(WPAPersonalKey);
   }
   
   connect( dialogWPAP, SIGNAL( saved(QString) ), this, SLOT( slotWPAPChanged(QString) ) ); 
   dialogWPAP->exec();

}


void wifiselectiondialog::slotConfigWPAE()
{
   // Bring up the WPA-Enterprise config dialog
    
   libWPAE = new dialogWPAE();
   
   if ( ! WPAEIdent.isEmpty() )
   {
       libWPAE->setVariables(WPAEType, WPAEIdent, WPAECACert, WPAEClientCert, WPAEPrivKeyFile, WPAEPassword, WPAEKeyMgmt);
   }
   
   connect( libWPAE, SIGNAL( saved(int, QString, QString, QString, QString, QString, int) ), this, SLOT( slotWPAEChanged(int, QString, QString, QString, QString, QString, int) ) ); 
   libWPAE->exec();
}


void wifiselectiondialog::slotWPAPChanged( QString newKey )
{
    WPAPersonalKey = newKey;
    slotCheckSecurityRadio();
}


void wifiselectiondialog::slotWPAEChanged( int type, QString EAPIdent, QString CACert, QString ClientCert, QString PrivKeyFile, QString PrivKeyPass, int keyMgmt )
{
  WPAEType = type;
  WPAEIdent=EAPIdent;
  WPAECACert=CACert;
  WPAEClientCert=ClientCert;
  WPAEPrivKeyFile=PrivKeyFile;
  WPAEPassword=PrivKeyPass;
  WPAEKeyMgmt=keyMgmt;

  slotCheckSecurityRadio();

}

void wifiselectiondialog::setWPAOnly(bool set)
{
   if ( ! set )
      return;

   groupBox->setHidden(true);
   radioSecurityWEP->setHidden(true);
   radioSecurityWPAPersonal->setHidden(true);

}
