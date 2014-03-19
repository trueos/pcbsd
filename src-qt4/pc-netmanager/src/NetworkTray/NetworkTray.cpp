
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

#include "NetworkTray.h"
#include <pcbsd-netif.h>
#include <pcbsd-utils.h>
#include "../../../config.h"


/* Update interval in ms */
#define  UPDATE_MSEC 15000

// Public Variables
QString DeviceName;
QString DeviceType;
QString DeviceParent;
QString DeviceIP;
QString DeviceIPv6;
QString DeviceMAC;
QString DeviceSSID;
QString DeviceSignalStrength = "";
int 	DeviceSavedStrength = 0;
QString DeviceWirelessSpeed = "";
QString DeviceStatus;
QString DeviceUpStatus;
QString DeviceIdent;
QString DeviceNetmask;
QString DeviceMedia;
QString username;

void NetworkTray::programInit(QString Device)
{
  // Load the icons we'll be using into memory
  loadIcons();

  QString tmp;
  QIcon Icon;
  DeviceName = Device;
  DeviceType = getTypeForIdent(DeviceName);

  QString cmd = IFCONFIG + " lagg0 2>/dev/null | grep " + DeviceName;
  QString checkLagg = getLineFromCommandOutput(cmd.toLatin1());
  if ( ! checkLagg.isEmpty() )
    usingLagg = true;
  else
    usingLagg = false;
  

  // Get the username of the person we are running as
  username = QString::fromLocal8Bit(getenv("LOGNAME"));
  
  // Confirm this is a legit device
  confirmDevice(DeviceName); 

  // Update the ifconfig line we will be parsing
  slotUpdateIfStatus();

  // Get the MAC for this device
  DeviceMAC=getMacForIdent(DeviceName);

  if ( DeviceName.indexOf("wlan") != -1 )
  {
     DeviceParent = getWifiParent(DeviceName);

     // Get HW Identify line
     DeviceIdent = getNameForIdent(DeviceParent);

     // If the name comes up bogus for some reason
     if ( DeviceIdent.isEmpty() ) 
       DeviceIdent = tr("Unknown Wireless Device");
  } else {
      DeviceIdent = getNameForIdent( DeviceName );
  }

  trayIcon = new QSystemTrayIcon(this);
  
  // Connect our double-click slot and message clicked slot
  connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(slotTrayActivated(QSystemTrayIcon::ActivationReason)));
  connect(trayIcon, SIGNAL( messageClicked() ), this,SLOT( openConfigDlg() ));  
  trayIcon->setIcon(iconWifiDisconnect);
  trayIcon->show();
  
  //Display a message about the wireless status
  QTimer::singleShot(5000,this,SLOT(slotCheckWifiAvailability() ));

  // Create the tooltip popup now
  displayTooltip();

  // Start the monitor service on a refresh loop
  monitorStatus();

}


// Function which locates the parent device of a wlan child device
QString NetworkTray::getWifiParent(QString dev)
{
   dev.remove(0, dev.size() -1 );
   QString DevNum = dev;
   return pcbsd::Utils::sysctl("net.wlan." + DevNum + ".%parent");
}

void NetworkTray::confirmDevice( QString device )
{
   QString command = IFCONFIG + " -l | grep " + device;
   QString line = getLineFromCommandOutput(command);
   if ( line.isEmpty() )
   {
      // Opps, this device doesn't seem to exist!
      exit(1);
   }
}

QString NetworkTray::getLineFromCommandOutput( QString command )
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


QString NetworkTray::getNameForIdent( QString ident )
{
  NetworkInterface ifr(ident);
  return ifr.desc();
}


QString NetworkTray::getIpForIdent()
{
  QString inputLine = ifconfigOutput;
  QString ip= "";
  QString tDev;
  if ( usingLagg ) {
    tDev = "lagg0";
    QString cmd = IFCONFIG + " " + tDev + " | grep " + DeviceName;
    QString checkLagg = getLineFromCommandOutput(cmd.toLatin1());
    if ( ! checkLagg.isEmpty() ) {
      cmd = IFCONFIG + " " + tDev + " | grep 'inet ' ";
      inputLine = getLineFromCommandOutput(cmd.toLatin1());
    }

    if (inputLine != "" && inputLine.indexOf("inet ") != -1){
      ip = inputLine.remove(0, inputLine.indexOf("inet ") + 5);
      ip.truncate(inputLine.indexOf(" "));
    }
    return ip;
  }

  if (inputLine != "" && inputLine.indexOf("inet ") != -1){
    ip = inputLine.remove(0, inputLine.indexOf("inet ") + 5);
    ip.truncate(inputLine.indexOf(" "));
  }
	
  return ip;
}


QString NetworkTray::getMacForIdent( QString ident )
{
  NetworkInterface ifr(ident);
  return ifr.macAsString();
}

QString NetworkTray::getSSIDForIdent()
{
  QString inputLine = ifconfigOutput;
  QString SSID = "";
	
  if (inputLine != "" && inputLine.indexOf("ssid ") != -1){
    SSID = inputLine.remove(0, inputLine.indexOf("ssid ") + 5);
    SSID.truncate(inputLine.indexOf("channel") - 1 );
  }
	
  return SSID;
}

QString NetworkTray::getNetmaskForIdent()
{
  QString inputLine = ifconfigOutput;
  QString netmask= "";
	
  if (inputLine != "" && inputLine.indexOf("netmask ") != -1){
    netmask = inputLine.remove(0, inputLine.indexOf("netmask ") + 8);
    netmask.truncate(netmask.indexOf(" "));
  }
	
  return netmask;
}

QString NetworkTray::getSignalStrengthForIdent( QString ident )
{
  // Get the signal strength of this device
  QString command = IFCONFIG + " " + ident + " list scan | grep " + DeviceSSID;
  QString line = getLineFromCommandOutput(command);
  QString tmp, sig, noise;
  bool ok, ok2;
  int isig, inoise, percent;
	
  tmp = line.simplified();

  // Lets find the signal strength / noise variables now
  tmp.remove(0, tmp.indexOf(":"));
  tmp.remove(0, tmp.indexOf(" "));

  // Get the noise
  noise = tmp.simplified();
  noise.remove(0, noise.lastIndexOf(":") + 1);
  noise.remove(noise.indexOf(" "), noise.size());
  noise = noise.simplified();
  if ( noise.indexOf("-") == 0)
	noise.remove(0, 1);

  // Get the signal
  sig = tmp.simplified();
  sig.remove(sig.indexOf(":"), sig.size());
  sig.remove(0, sig.lastIndexOf(" "));
  sig = sig.simplified();
  if ( sig.indexOf("-") == 0)
	sig.remove(0, 1);

  //qDebug() << "Signal:" << sig << " Noise:" << noise;

  // Now figure out the percentage
  isig = sig.toInt(&ok);
  inoise = noise.toInt(&ok2);
  if ( ok && ok2 ) {
     percent = (inoise - isig) * 4;
     // Sanity check
     if ( percent > 100 )
	percent = 100;
     if ( percent < 0 )
	percent = 0;
     tmp.setNum(percent);
  } else {
     tmp = "";
  }

  return tmp;	
}

QString NetworkTray::getWirelessSpeedForIdent( QString ident )
{
  QString command = IFCONFIG + " " + ident + " list scan | grep " + DeviceSSID;
  QString line = getLineFromCommandOutput(command);
  QString tmp;

  // Get the signal strength of this device
  tmp = line;
  tmp = tmp.simplified();

  // First remove the SSID which may have spaces in it
  tmp.remove(0, tmp.indexOf(":"));

  // Now find the Speed section
  tmp = tmp.section(" ", 2, 2);
  return tmp;
}

void NetworkTray::slotTrayActivated(QSystemTrayIcon::ActivationReason reason) {
   if(reason == QSystemTrayIcon::Trigger) {
      //openNetManager();
      openConfigDlg();
   }
}


void  NetworkTray::openConfigDlg() {

  if ( DeviceType == "Ethernet" )
  {
    QString program = "sudo";
    QStringList arguments;
    arguments << "pc-ethernetconfig" << DeviceName;
    QProcess *runCommandProc = new QProcess(this);
    runCommandProc->start(program, arguments);
  } else {
    QString program = "sudo";
    QStringList arguments;
    arguments << "pc-wificonfig" << DeviceName;
    QProcess *runCommandProc = new QProcess(this);
    runCommandProc->start(program, arguments);
  }

}

void  NetworkTray::displayTooltip() {

  QString  tooltipStr;
  tooltipStr = QString(tr("Device Name:") + " /dev/" + DeviceName);
  tooltipStr += "<br>" + DeviceIdent +"<hr>";
  if ( DeviceType == "Ethernet" )
  {
     if ( DeviceStatus == "active" || DeviceStatus == "" ) {
       tooltipStr += "<br>" + tr("IP:") + " " + DeviceIP;
       tooltipStr += "<br>" + tr("IPv6:") + " " + DeviceIPv6;
       tooltipStr += "<br>" + tr("Mac/Ether:") + " " + DeviceMAC;
       tooltipStr += "<br>" + tr("Media:") + " " + DeviceMedia;
       tooltipStr += "<br>" + tr("Status:") + " " + DeviceUpStatus;
     } else {
       tooltipStr += "<br>" + tr("Mac/Ether:") + " " + DeviceMAC;
       tooltipStr += "<br>" + tr("Media:") + " " + DeviceMedia;
       tooltipStr += "<br>" + tr("Status:") + " " + DeviceStatus;
       tooltipStr += "<br>" + tr("No connection detected.<br> Check your cable connection and try again!");
     }

   } else {
     // If this is a wireless device, give different output
     if ( DeviceStatus == "associated" ) {
	tooltipStr += "<br>" + tr("IP:") + " " + DeviceIP;
	tooltipStr += "<br>" + tr("IPv6:") + " " + DeviceIPv6;
      	tooltipStr += "<br>" + tr("SSID:") + " " + DeviceSSID;
      	tooltipStr += "<br>" + tr("Connection Strength:") + " " + DeviceSignalStrength + "%";
      	tooltipStr += "<br>" + tr("Connection Speed:") + " " + DeviceWirelessSpeed;
	tooltipStr += "<br>" + tr("Mac/Ether:") + " " + DeviceMAC;
	tooltipStr += "<br>" + tr("Media:") + " " + DeviceMedia;
   	tooltipStr += "<br>" + tr("Status:") + " " + DeviceStatus;
      } else {
	tooltipStr += "<br>" + tr("Mac/Ether:") + " " + DeviceMAC;
	tooltipStr += "<br>" + tr("Media:") + " " + DeviceMedia;
   	tooltipStr += "<br>" + tr("Status:") + " " + DeviceStatus;
	tooltipStr += "<br>" + tr("No wireless connection detected.<br> Double-click to start the wireless configuration wizard.");
      }
   }

  // Update the tooltop
  trayIcon->setToolTip(tooltipStr);
   
}

// Load all the icons we'll be using
void NetworkTray::loadIcons() {
  iconWiredConnect.addFile(PROGPATH + "/pics/tray_local.png");
  iconWiredDisconnect.addFile( PROGPATH + "/pics/tray_local_dis.png");
  iconWifiConnect.addFile( PROGPATH + "/pics/tray_wifi.png");
  iconWifiDisconnect.addFile( PROGPATH + "/pics/tray_wifi_dis.png");
  iconWifiConnect30.addFile( PROGPATH + "/pics/tray_wifi30.png");
  iconWifiConnect60.addFile( PROGPATH + "/pics/tray_wifi60.png");
  iconWifiConnect85.addFile( PROGPATH + "/pics/tray_wifi85.png");
  iconLocked.addFile( PROGPATH + "/pics/object-locked.png");
  iconUnlocked.addFile( PROGPATH + "/pics/object-unlocked.png");
}

void NetworkTray::slotQuit() {
   exit(0);
}

void NetworkTray::slotTriggerFileChanged() {
}

void NetworkTray::monitorStatus(bool noloop) {
  // Start checking to see if the device has changed, and if it has inform the user
  QString tmp;  
  QIcon Icon;
  QString NotifyText = "";

  // Check the media status of this device
  DeviceMedia = getMediaForIdent();

  // Check for IPv6 Changes
  DeviceIPv6 = getIPv6ForIdent();

  // Get the device up status
  DeviceUpStatus = getUpStatusForIdent();


  if ( DeviceType == "Ethernet" )
  {
    if ( (DeviceStatus == "active" || DeviceStatus == "")  && tmp == "UP")
      Icon = iconWiredConnect;
    else
      Icon = iconWiredDisconnect;
  } else {
    if ( DeviceStatus == "associated" && tmp == "UP" )
      Icon = iconWifiConnect;
    else
      Icon = iconWifiDisconnect;
  }


  // First check the status of the device
  DeviceStatus = getStatusForIdent();
  if ( DeviceType == "Ethernet" )
  {
    if ( (DeviceStatus == "active" || DeviceStatus == "")  && DeviceUpStatus == "UP")
      Icon = iconWiredConnect;
    else
      Icon = iconWiredDisconnect;

  } else {
    if ( DeviceStatus == "associated" && DeviceUpStatus == "UP" )
      Icon = iconWifiConnect;
    else
      Icon = iconWifiDisconnect;
  }

  // Now check the IP Address for changes
  DeviceIP = getIpForIdent();
  DeviceNetmask = getNetmaskForIdent();

  if ( DeviceType == "Wireless" && DeviceStatus == "associated" )
  {
    // Now check the SSID Address for changes
    DeviceSSID = getSSIDForIdent();

    if (! DeviceSSID.isEmpty() )
    {
       int newStrength = 0;
       int iconStrength = 0;
       QString FileLoad;
       bool ok;
       // Get the strength of the signal
       tmp = getSignalStrengthForIdent( DeviceName );
       DeviceSignalStrength = tmp;
       // Figure out if we need to change the strength icon
       tmp.toInt(&ok);
       if ( ok ) {
         newStrength = tmp.toInt(&ok);
	 if ( newStrength < 5 )
         {	
	   iconStrength = 0;
           Icon = iconWifiConnect;
         } else if ( newStrength < 50 ) {
	   iconStrength = 30;
           Icon = iconWifiConnect30;
	 } else if ( newStrength < 75 ) {
	   iconStrength = 60;
           Icon = iconWifiConnect60;
         } else {
	   iconStrength = 85;
           Icon = iconWifiConnect85;
	 }
       } else {
	  DeviceSignalStrength = tr("Unknown");
       }
       // Get the connection speed being used
       DeviceWirelessSpeed = getWirelessSpeedForIdent( DeviceName );
     }

  }

  // Set the tray icon now
  trayIcon->setIcon(Icon);


  displayTooltip();
  updateWifiNetworks();
  
  if(!noloop){
    QTimer::singleShot(UPDATE_MSEC,  this,  SLOT(monitorStatus()));
  }

}

// If the user wants to restart the network, do so
void NetworkTray::slotRestartNetwork() {
  //trayIcon->showMessage( tr("Please Wait"),tr("Restarting Network"),QSystemTrayIcon::NoIcon,5000);  
  pcbsd::Utils::restartNetworking();
}

void NetworkTray::openNetManager() {
  QString arguments;
  arguments = "pc-netmanager";
  pcbsd::Utils::runShellCommand(arguments);
}

void  NetworkTray::openDeviceInfo() {

  QString program = "sudo";
  QStringList arguments;
  if ( DeviceType == "Ethernet" )
  {
    arguments << "pc-ethernetconfig" << "info" << DeviceName;
  } else {
    arguments << "pc-wificonfig" << "info" << DeviceName;
  }

  QProcess *runCommandProc = new QProcess(this);
  runCommandProc->start(program, arguments);
}

QString NetworkTray::getTypeForIdent( QString ident )
{
   NetworkInterface ifr(ident);
   if (ifr.isWireless()) return "Wireless";
   return "Ethernet";
}

QString NetworkTray::getStatusForIdent()
{
  QString inputLine = ifconfigOutput;
  QString status = "";
	
  if (inputLine != "" && inputLine.indexOf("associated") != -1)
    status = "associated";
  else if (inputLine != "" && inputLine.indexOf("active") != -1)
    status = "active";
  else
    status = "DOWN";  
	
  return status;
    
}

QString NetworkTray::getUpStatusForIdent()
{
  QString inputLine = ifconfigOutput;
  QString status = "";
	
  if (inputLine != "" && inputLine.indexOf("<UP") != -1)
    status = "UP";
  else
    status = "DOWN";  
	
  return status;
    
}

QString NetworkTray::getMediaForIdent()
{
  QString inputLine = ifconfigOutput;
  QString status = "";

  if (inputLine != "" && inputLine.indexOf("media:") != -1){
    status = inputLine.remove(0, inputLine.indexOf("media:") + 7);
    status.truncate(inputLine.indexOf("status:") - 1 );
  }
	
  return status;
}

QString NetworkTray::getIPv6ForIdent()
{
  QString inputLine = ifconfigOutput;
  QString ip= "";
	
  if (inputLine != "" && inputLine.indexOf("inet6 ") != -1){
    ip = inputLine.remove(0, inputLine.indexOf("inet6 ") + 5);
    ip.truncate(ip.indexOf("%"));
  }
	
  return ip;
}

void NetworkTray::slotUpdateIfStatus()
{
   QProcess *getIfProc = new QProcess();
   getIfProc->start(IFCONFIG, QStringList() << DeviceName);
   while(getIfProc->state() == QProcess::Starting || getIfProc->state() == QProcess::Running) {
      getIfProc->waitForFinished(50);
      //QCoreApplication::processEvents();
   }

   ifconfigOutput.clear();
   ifconfigOutput = getIfProc->readAllStandardOutput().simplified();

   //qDebug() << IFCONFIG + " output: " << ifconfigOutput <<"\n";

   getIfProc->kill();
   delete getIfProc;

   QTimer::singleShot(UPDATE_MSEC, this,  SLOT(slotUpdateIfStatus()) );
}

void NetworkTray::slotCheckWifiAvailability(){
  if(DeviceType == "Wireless"){
    // Get the device up status
    DeviceStatus = getStatusForIdent();
    //Show a message if the wifi is down
    if(DeviceStatus == "DOWN"){
      trayIcon->showMessage( tr("No Wireless Network Connection"),tr("Click here to configure wireless connections"),QSystemTrayIcon::NoIcon,15000);
    }
  }
}

void NetworkTray::updateWifiNetworks(){
  // Change the right-click of the tray icon to show all available wireless networks
  
  //update the list of wifi networks available
  QString cmd = "ifconfig "+DeviceName+" list scan";
  QStringList wifinet = pcbsd::Utils::runShellCommand(cmd);
 
 //Redo the tray menu
  trayIconMenu = new QMenu(this);
  trayActionGroup = new QActionGroup(this);
  trayIconMenu->clear();
  //QAction *act = trayIconMenu->addAction( tr("Wifi Quick-Connect") );
  //act->setEnabled(false);
  trayIconMenu->addSeparator();
  QIcon ssidIcon;
 //add an entry for each wifi network detected
  for(int i=1; i<wifinet.length(); i++){ //skip the first line (labels)
    //Get the ssid and Security for this network
    QStringList wdata = NetworkInterface::parseWifiScanLine(wifinet[i],false);
    QString ssid = wdata[0];
    QString strength = wdata[4];
    //Add this network to the list
    QString entry = ssid+" ("+strength+")"; // SSID (%Strength)
    //Get the proper "locked" or "unlocked" icon for the network
    if(wdata[6].contains("None")){
      ssidIcon = iconUnlocked;
    }else{
      ssidIcon = iconLocked;
    }
    //Make sure there is an ssid (don't show blank entry points)
    if( !ssid.isEmpty() ){

    //Check for duplicate SSID entries
    bool duplicateSSID = FALSE;
    int dupRow;
    for ( int z = 0 ; z < trayIconMenu->actions().count() ; z++){
      //qDebug() << listWifi->item(z)->text() << ssid;
      if ( trayIconMenu->actions()[z]->text().contains(ssid+" (") ){
        duplicateSSID = TRUE;
        dupRow = z;
        break;
      }
    }
    if(!duplicateSSID){
      //Create the action
      QAction* tempAct = new QAction(entry, this); //set the label for the action on creation
      tempAct->setObjectName(wdata[0]); //set the action name as the SSID
      tempAct->setIcon(ssidIcon); //set the action icon
      //add the action to the action group
      trayActionGroup->addAction(tempAct); 
      //Add the action to the menu
      trayIconMenu->addAction(tempAct);
    }else{
      //Check if the new signal strength is greater, replace if it is
      QString oldStrength = trayIconMenu->actions()[dupRow]->text();
      oldStrength = oldStrength.section("(",1,1).section("%)",0,0).simplified();
      if( strength.section("%",0,0).toInt() > oldStrength.toInt() ){
        //New signal strength is greater - replace the string
        //qDebug() << "Changing signal strength for "+ssid+" to reflect a stronger signal found";
        //qDebug() << strength << oldStrength;
        trayIconMenu->actions()[dupRow]->setText( ssid+" ("+strength+")" );
      }
    }
    } //end of the empty ssid check
  }
  //Connect the actionGroup signal with slotQuickConnect
  QObject::connect(trayActionGroup, SIGNAL(triggered(QAction*)),this,SLOT(slotGetNetKey(QAction*)));
  //Add the configuration options to the bottom
  trayIconMenu->addSeparator();
  trayIconMenu->addAction( tr("Configure ") + DeviceName, this, SLOT(openConfigDlg()));
  trayIconMenu->addAction( tr("Start the Network Manager"), this, SLOT(openNetManager()));
  trayIconMenu->addAction( tr("Restart the Network"), this, SLOT(slotRestartNetwork()));
  trayIconMenu->addAction( tr("Close the Network Monitor"), this, SLOT(slotQuit()));
  
  //attach the new menu to the tray
  trayIcon->setContextMenu(trayIconMenu); 
 
}

void NetworkTray::slotGetNetKey(QAction* act){
  //Get the small SSID from the action 
  QString smSSID = act->objectName();
  //trim the small ssid to remove any dots at the end
  smSSID = smSSID.section(".",0,0,QString::SectionSkipEmpty);
  
  //get the full SSID string
  QString dat = pcbsd::Utils::runShellCommandSearch("ifconfig -v "+DeviceName+" list scan",smSSID);
  QStringList wdat = NetworkInterface::parseWifiScanLine(dat,true);
  QString SSID = wdat[0];
  
  //Get the Security Type
  QString sectype = wdat[6];
  
  if(sectype == "None"){
    //run the Quick-Connect slot without a key
    slotQuickConnect("",SSID, false);
    
  }else{
    //Open the dialog to prompt for the Network Security Key
    dialogNetKey = new netKey();
    //Insert the SSID into the dialog
    dialogNetKey->setSSID(SSID);
    dialogNetKey->showKeyType( sectype == "WEP" );
    //connect the signal from the dialog to the quick-connect slot
    connect(dialogNetKey,SIGNAL( saved(QString,QString,bool) ),this,SLOT( slotQuickConnect(QString,QString,bool) ) );
    //Activate the dialog
    dialogNetKey->exec();
  }
}

void NetworkTray::slotQuickConnect(QString key,QString SSID, bool hexkey){
  
  //Run the wifiQuickConnect function
  NetworkInterface::wifiQuickConnect(SSID,key,DeviceName, hexkey);
  
  //Inform the user that it is connecting (this is done by libpcbsd now)
  //QString msg = tr("Connecting to ") + SSID;
  //trayIcon->showMessage( tr("Please Wait"),msg,QSystemTrayIcon::NoIcon,10000);
}
