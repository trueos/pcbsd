
/* Qt */
#include <QDebug>
#include <QFile>
#include <QImage>
#include <QMenu>
#include <QProcess>
#include <QToolTip>
#include <QTextStream>
#include <QTimer>
#include <QTranslator>
#include <iostream>

#include "btTray.h"
#include <pcbsd-hardware.h>
#include <pcbsd-utils.h>



/* Update interval in ms */
#define  UPDATE_MSEC 10000

//bool isBTDevice=TRUE;
//bool isFirstRun=TRUE;

void BluetoothTray::programInit()
{
  isBTDevice = true;
  isFirstRun = true;
  starting = true;
  trayIcon = new QSystemTrayIcon(this);
  trayIconMenu = new QMenu;
    trayIcon->setContextMenu(trayIconMenu);  
	
  // Connect our double-click slot and message clicked slot
  connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(slotTrayActivated(QSystemTrayIcon::ActivationReason)));
  connect(trayIcon, SIGNAL( messageClicked() ), this,SLOT( startBluetoothManager() ));  
  trayIcon->setIcon(QIcon(":inactiveBluetooth.png"));
  trayIcon->show();
  
  //Start the service on a refresh loop
  QTimer::singleShot(100,this,SLOT(refreshTray() ));
  qDebug() << "pc-bluetoothtray: starting up";
}

bool BluetoothTray::checkForBtDevices(){
  bool oldStatus = isBTDevice;
  //Run a couple quick checks to see if a bluetooth device is plugged in
  QStringList result = pcbsd::Utils::listShellCommandSearch("ngctl list","ubt0");
  if( result.isEmpty()){ isBTDevice=FALSE; } //no device has ever been plugged in since compupter turned on
  //else if( result[result.length()-1].contains("disconnected") ){ isBTDevice=FALSE; } //last entry says the device has been removed
  else{ isBTDevice=TRUE; }
  //Determine if the status has changed
  bool status = (oldStatus != isBTDevice);    
  //Determine if the device is a new detection (but not the first time the computer is turned on)
  if(status && isBTDevice && !isFirstRun){
    //Show the popup message about a new device being discovered is not the first run after startup
    trayIcon->showMessage( tr("New Bluetooth Device Detected"),tr("Click here to configure Bluetooth device configurations"),QSystemTrayIcon::NoIcon,5000);
  }

  if(isFirstRun){
    status = TRUE;
    isFirstRun=FALSE; 
  }
  return status; //TRUE if device status changed from previous

}

void BluetoothTray::refreshTray(){
  //---DO NOT CALL THIS FUNCTION MANUALLY: AUTOMATICALLY REFRESHES---
  //only redo the Tray if the device status has changed -- allows for lower memory footprint on refresh
  if( checkForBtDevices() ){ 
    trayIconMenu->clear();
    if(isBTDevice){ //device currently connected
      trayIcon->setIcon(QIcon(":activeBluetooth.png")); //set proper tray icon
      //Setup the menu options
      trayIconMenu->addAction( tr("Start Bluetooth Manager"), this, SLOT(startBluetoothManager()));
      trayIconMenu->addAction( tr("Restart Bluetooth Services"), this, SLOT(restartBluetooth()));
      trayIconMenu->addSeparator();
      trayIconMenu->addAction( tr("Close Bluetooth Tray"), this, SLOT(closeTray()));
    }else{  //no device connected
      trayIcon->setIcon(QIcon(":inactiveBluetooth.png")); //set proper tray icon
      //Setup the menu options
      trayIconMenu->addAction( tr("Start Bluetooth Manager"), this, SLOT(startBluetoothManager()));
      trayIconMenu->addSeparator();
      trayIconMenu->addAction( tr("Close Bluetooth Tray"), this, SLOT(closeTray()));
    }
  }
  QTimer::singleShot(UPDATE_MSEC,this,SLOT(refreshTray() ));
  if(starting){ starting = false; } //this has already been run
}

void BluetoothTray::slotTrayActivated(QSystemTrayIcon::ActivationReason reason) {
   if(starting){ return; } //not ready yet
   if(reason == QSystemTrayIcon::DoubleClick) {
       startBluetoothManager();
   }else{
     //show the context menu
     trayIcon->contextMenu()->popup(QCursor::pos());
   }
}

void BluetoothTray::startBluetoothManager(){
  qDebug() << "pc-bluetoothtray: Starting Bluetooth Manager";
  //Start bluetooth manager completely seperate from the tray app
  //QString cmd = "pc-bluetoothmanager &"; //might need to be "sudo ..."
  QProcess::startDetached("pc-bluetoothmanager");
}

void BluetoothTray::restartBluetooth(){
  qDebug() << "pc-bluetoothtray: Restarting Bluetooth Services";
  trayIcon->showMessage( tr("Please Wait"),tr("Restarting Bluetooth Services"),QSystemTrayIcon::NoIcon,5000);  
  //Restart/reconnect the bluetooth services/devices
  Hardware::restartBT();
}

void BluetoothTray::closeTray(){
  qDebug() << "pc-bluetoothtray: closing down";
  //Close down the application
  exit(0);
}

void BluetoothTray::slotSingleInstance()
{
  trayIcon->show();
}
