
/* Qt */
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QImage>
#include <QMenu>
#include <QTranslator>

#include "mountTray.h"
#include <pcbsd-hardware.h>
#include <pcbsd-utils.h>

MountTray::~MountTray(){
}

void MountTray::programInit()
{
  DCheck = new DevCheck(); //initialize class for checking devices
  qDebug() << "pc-mounttray: starting up";
  getInitialUsername(); //try to detect the non-root user who is running the program with root permissions
  getDefaultFileManager(); //try to detect the default file-manager for opening the mount directory
  
  trayIcon = new QSystemTrayIcon(this);
  trayIconMenu = new QMenu();
  //Generate the system menu options (these don't change)
  sysMenu = new QMenu( tr("More Options") );
    sysMenu->setIcon( QIcon(":icons/config.png") );
    //Add the additional options
    sysMenu->addAction( QIcon(":icons/folder.png"), tr("Open Media Directory"), this, SLOT(slotOpenMediaDir()) );
    sysMenu->addAction( QIcon(":icons/harddrive.png"), tr("View Disk Usage"),this,SLOT(slotOpenFSDialog()) );
    sysMenu->addAction( QIcon(":icons/refresh.png"),tr("Rescan Devices"), this, SLOT(slotRescan()) );
    //Add the setting dialog option seperately
    sysMenu->addSeparator();
    sysMenu->addAction( QIcon(":icons/config.png"), tr("Settings"), this, SLOT(slotOpenSettings()) );
    //Add the Close button seperately
    sysMenu->addSeparator();
    sysMenu->addAction( QIcon(":icons/application-exit.png"), tr("Close Tray"), this, SLOT(closeTray()) );
  
  // Tie the left-click signal to open the context menu
  connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(slotTrayActivated(QSystemTrayIcon::ActivationReason)) );
  
  //Set the default Tray Icon (will change once tray menus are set)
  trayIcon->setIcon(QIcon(":icons/CDdevices-inactive.png"));
  trayIcon->show();

  //Startup the devd watching process
  qDebug() << "-Starting up the DEVD watcher";
  devdTimer = new QTimer();
  devdTimer->setSingleShot(TRUE);
  connect(devdTimer,SIGNAL(timeout()),this,SLOT(slotDevChanges()));
  startupDevdProc();
  
  //Do an initial scan of the devices with dmesg
  qDebug() << "-Performing initial device scan";
  scanInitialDevices();
  
  //Start up the filesystem watcher
  qDebug() << "-Starting up the disk space alert system";
  diskWatcher = new FSWatcher();
  connect(diskWatcher,SIGNAL(FSWarning(QString,QString)),this,SLOT(slotDisplayWarning(QString,QString)));
  diskWatcher->start(3600000); // check every 1 hour in milliseconds
  
  //Update the tray menu and icons
  updateMenu();

  qDebug() << "-Program now ready for use";
}

void MountTray::updateMenu(){
  //Clear the menu
  trayIconMenu->clear();
  trayIconMenu->disconnect();
  numAvail = 0;
  numMount = 0;
  //Iterate through all the devices and add them as necessary
  for(int i=0; i<deviceList.length(); i++){
    if(deviceList[i]->isConnected()) { 
      trayIconMenu->addAction(deviceList[i]);
      numAvail++;
      if(deviceList[i]->isMounted() ){ numMount++; }
      deviceList[i]->updateItem(); //refresh the item now as well
    }else{
      //Remove any devices that are not connected
      deviceList.removeAt(i);
      i--; //roll back one to catch the new index values for the list
    }
  }
  //Separate the extra options at the end
  trayIconMenu->addSeparator();
  trayIconMenu->addMenu(sysMenu);
  //Apply the menu to the Tray
  trayIcon->setContextMenu(trayIconMenu);

  //Update the main icon based upon whether devices have been found
  if(numAvail==0){
    trayIcon->setIcon( QIcon(":icons/CDdevices-inactive.png") );
  }else{
    if(numMount==0){
      trayIcon->setIcon( QIcon(":icons/CDdevices.png") );
    }else{
      trayIcon->setIcon( QIcon(":icons/CDdevices.png") );
    }
  }
}

void MountTray::scanInitialDevices(){
  slotDevChanges(FALSE);
  return;
}

int MountTray::findDeviceInList(QString newDev){
  for(int i=0; i<deviceList.length(); i++){
    if( deviceList[i]->device == newDev ){ return i; }
  }
  return -1;
}

void MountTray::addDevice(QString dev, QString label, QString type, QString filesys){
  if(!dev.startsWith(DEVICEDIR)){ dev.prepend(DEVICEDIR); }
  
  //Check if the device is already in the list
  int tot=0;
  for(int i=0; i<deviceList.length(); i++){
    if( deviceList[i]->device == dev ){ return; } //already exists, do nothing
    if( deviceList[i]->getDeviceName().startsWith(label) ){ tot++; }
  }
  //See if the label is unique as well, otherwise add a number to the end to make it unique
  if(tot > 0 && !label.isEmpty()){ label.append("-"+QString::number(tot)); }
 
  qDebug() << "Valid Device Connection:" << dev << type << label << filesys;
  //Create the menu item (will automount if necessary)
  MenuItem *tmp = new MenuItem(this, dev, label, type, filesys);
  //connect the signals/slots
  connect(tmp, SIGNAL(itemMounted(QString)), this, SLOT(openMediaDir(QString)) );
  connect(tmp, SIGNAL(newMessage(QString,QString)), this, SLOT(slotDisplayPopup(QString,QString)) );
  connect(tmp, SIGNAL(itemRemoved(QString)), this, SLOT(removeDevice(QString)) );
  deviceList << tmp;
  //Update the menu
  updateMenu();
}

void MountTray::removeDevice(QString dev){
  if(!dev.startsWith(DEVICEDIR)){ dev.prepend(DEVICEDIR); }
  
  //Find the device in the list
  int index = findDeviceInList(dev);
  if( index == -1 ){ return; } //does not exist, do nothing
  //Remove the menu entry from the list
  deviceList[index]->cleanup(); //make sure it is unmounted with mountpoint removed
  deviceList.removeAt(index);
  //Update the menu
  updateMenu();
  qDebug() << "Valid Device Removal:" <<  dev;
}

void MountTray::slotTrayActivated(QSystemTrayIcon::ActivationReason reason) {
   if(reason == QSystemTrayIcon::Trigger) {
     trayIcon->contextMenu()->popup(QCursor::pos());
   }
}


void MountTray::startupDevdProc(){
  devdProc = new QLocalSocket(this);
  devdProc->connectToServer("/var/run/devd.pipe",QIODevice::ReadOnly | QIODevice::Text);
  if( devdProc->waitForConnected(3000) ){ //Max wait of 3 sec
    connect(devdProc, SIGNAL(readyRead()), this, SLOT(newDevdMessage()) );
  }else{
    qDebug() << " - Could not startup the devd watching process";
  }
}

void MountTray::newDevdMessage(){	
  devdTimer->start(1000); //wait 1.5 seconds before checking for device changes
  return;
}

void MountTray::slotDevChanges(bool showPopup){
  //This function actually checks the system device list for changes
  //  and updates the available devices appropriately
  
  if(DEBUG_MODE){ qDebug() << "Checking for Device Changes:"; }
  //oldsysdev is the old device list for the system
  QStringList osd = oldsysdev;
  QStringList nsd = DCheck->devChildren("");
  nsd.sort();
  oldsysdev = nsd; //save the new list as the old list for later
  //Now determine new/missing devices
  nsd.sort();
  QStringList rmList;
  for(int i=0; i<osd.length(); i++){
    int ni = nsd.indexOf(osd[i]); //new index
    if( ni == -1){ //device removed
      rmList << osd[i];
      osd.removeAt(i);
      i--;
    }else{ //both lists have device
      osd.removeAt(i); i--;
      nsd.removeAt(ni);
    }
  }
  //all that is left in nsd is the new additions

  //Remove any devices that have been disconnected
  if(DEBUG_MODE){ qDebug() << " -Device Removals:" << rmList; }
  for(int i=0; i<rmList.length(); i++){
    removeDevice(rmList[i]);
  }
  //Add any devices that have been connected
  if(DEBUG_MODE){ qDebug() << " -Device Additions:" << nsd; } 
  for(int i=0; i<nsd.length(); i++){
    //Check if it is a good device
    QString dlabel, dtype, dfs, dsize; //additional output info
    bool good = DCheck->devInfo(nsd[i],&dtype,&dlabel,&dfs,&dsize);
    if(good){
      //Now create a new entry for this device
      addDevice(nsd[i],dlabel,dtype,dfs);  
      //Show a message bubble
      if(showPopup){
        QString title = tr("New Device");
        QString message = QString( tr("%1 can now be accessed")).arg(dlabel);
        slotDisplayPopup(title, message);
      }
    }
  }
  //==== CD/DVD Devices ====
  //Always check cd/dvd devices because the device node will never be added/removed
  QStringList diskList;
  diskList << DCheck->devChildren("cd");
  diskList << DCheck->devChildren("acd");
  if(DEBUG_MODE){ qDebug() << " -Checking CD/DVD devices:" << diskList; }
  for(int i=0; i<diskList.length(); i++){
    if( rmList.contains(diskList[i]) || nsd.contains(diskList[i]) ){
      continue; //do not double check a device that was just checked;	    
    }
    //Check if it is a good device
    QString dlabel, dtype, dfs, dsize; //additional output info
    bool good = DCheck->devInfo(diskList[i],&dtype,&dlabel,&dfs,&dsize);
    if(good){
      //Now create a new entry for this device
      addDevice(diskList[i],dlabel,dtype,dfs);  
      //Show a message bubble
      if(showPopup){
        QString title = tr("New Device");
        QString message = QString( tr("%1 can now be accessed")).arg(dlabel);
        slotDisplayPopup(title, message);
      }
    }else{ //not good device
      removeDevice(diskList[i]);	    
    }
  } //end loop over cd/dvd devices
}

void MountTray::closeTray(){
  qDebug() << "pc-mounttray: closing down";
  //Kill the devd watching process
  qDebug() << " -Shutting down DEVD watcher";
  devdProc->disconnectFromServer();
  qDebug() << " -Unmounting managed devices and mount points";
  for(int i=0; i<deviceList.length(); i++){
    deviceList[i]->cleanup();
  }
  //Close down the application
  exit(0);
}

void MountTray::getInitialUsername(){
  //Get the original user who started the tray app
  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  //qDebug() << "System Environment:" << env.toStringList();
  QString username = env.value( "LOGNAME" );
  //qDebug() << "First attempt user detected:" << username;
  if(username=="root"){
    username = env.value( "SUDO_USER" ); //try this environment variable instead
    //qDebug() << "Second attempt user detected:" << username;
  }
  if(username=="root" || username.isEmpty() ){
   //attempt another method of determining the username
   QStringList uList = pcbsd::Utils::runShellCommand("who");
   if( uList.length() > 0 ){ 
     username = uList[0].section(" ",0,0,QString::SectionSkipEmpty);
     for(int i=0; i<uList.length(); i++){
       if( !uList[i].section(" ",0,0,QString::SectionSkipEmpty).contains(username) ){
         username = "root"; //too many users logged in; this method will not work
         break;
       }
     }
     //qDebug() << "Third attempt user detected:" << username;
   }
  }
  if(username=="root"){
    QMessageBox::warning(this,tr("User Detection Error"),tr("Unable to determine the non-root user who started the application \nCan not open the file manager with root permissions") );
    return;
  }
  USERNAME=username.simplified(); //set the global variable
  if(DEBUG_MODE){ qDebug() << "-User detected:" << USERNAME; }
}

void MountTray::getDefaultFileManager(){
  //Get the command to open the appropriate file manager
  QString fmcmd = pcbsd::Utils::runShellCommand("de-info -fileman").join(" ");
  //qDebug() << "de-info result:" << fmcmd;
  fmcmd = fmcmd.remove("%s").simplified();
  //qDebug() << "FM command found:" << fmcmd;
  if( fmcmd.isEmpty() || fmcmd.contains("File ") ){fmcmd= "openwith"; } //Default to the "openwith" command

  FILEMAN = fmcmd.simplified();
  if(DEBUG_MODE){ qDebug() << "-File manager detected:" << FILEMAN; }
}

void MountTray::slotOpenMediaDir(){
  openMediaDir(MOUNTDIR);
}

void MountTray::openMediaDir(QString dir){
  //Open the default file-manager to the directory listed
  if(dir.isEmpty()){ dir = MOUNTDIR; }
  if(!dir.endsWith("/")){ dir.append("/"); } //make sure the filemanager knows it is a directory
  //Make sure we can setup user permissions
  if(USERNAME=="root"){
    qDebug() << "Cannot open filemanager with root permissions";
    return;
  }
  //Open the default file manager to the given directory as that user
  qDebug() << "Opening the file manager with user permissions";
  QString cmd = "su -m "+USERNAME+" -c \'"+FILEMAN+" "+dir+"\' &";
  //qDebug() << cmd;
  system( cmd.toUtf8() ); 
}

void MountTray::slotRescan(){
  //Display a notification
  qDebug() << "Re-scanning devices:";
  slotDisplayPopup(tr("Please Wait"),tr("Rescanning devices attached to the system"));
  //Rescan the device list for new devices
  scanInitialDevices();
  //Check that all the existing devices still exist
  for(int i=0; i<deviceList.length(); i++){
    deviceList[i]->updateItem();
  }
}

void MountTray::slotOpenFSDialog(){
  //Open up the Filesystem disk space monitoring dialog
  diskDisplay = new FSDialog();
  diskDisplay->show();
}

void MountTray::slotOpenSettings(){
  //Open up the settings window and apply changes as necessary
  qDebug() << "Settings window not implemented yet";
}

void MountTray::slotSingleInstance()
{
  trayIcon->show();
}

void MountTray::slotDisplayPopup(QString title, QString msg){
  //Display a popup bubble with the given message for 3 seconds
  trayIcon->contextMenu()->hide(); //close the menu list
  disconnect(trayIcon, SIGNAL(messageClicked()),0,0); //make sure only one signal/slot connection
  connect(trayIcon,SIGNAL(messageClicked()),this,SLOT(slotOpenMediaDir()) );
  trayIcon->showMessage(title, msg , QSystemTrayIcon::NoIcon,3000 );
}

void MountTray::slotDisplayWarning(QString title, QString msg){
  //Display a popup bubble with the given message for 5 seconds
  trayIcon->contextMenu()->hide(); //close the menu list
  disconnect(trayIcon, SIGNAL(messageClicked()),0,0); //make sure only one signal/slot connection
  connect(trayIcon,SIGNAL(messageClicked()),this,SLOT(slotOpenFSDialog()) );
  trayIcon->showMessage(title, msg , QSystemTrayIcon::Warning,5000 );
}
