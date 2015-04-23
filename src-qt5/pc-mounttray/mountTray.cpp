
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

#include "DeviceWidget.h"

MountTray::~MountTray(){
}

void MountTray::programInit()
{
  DCheck = new DevCheck(); //initialize class for checking devices
  qDebug() << "pc-mounttray: starting up";
  MTINIT=true; //set the flag that the mount tray is initializing;
  //getInitialUsername(); //try to detect the non-root user who is running the program with root permissions
  //getFileManager();
    
  loadSavedSettings();
  
  trayIcon = new QSystemTrayIcon(this);
  trayIconMenu = new QMenu();
    trayIcon->setContextMenu(trayIconMenu);
	
  //Generate the system menu options (these don't change)
  sysMenu = new QMenu( tr("More Options") );
    sysMenu->setIcon( QIcon(":icons/config.png") );
    //Add the additional options
    //sysMenu->addAction( QIcon(":icons/folder.png"), tr("Open Media Directory"), this, SLOT(slotOpenMediaDir()) );
    sysMenu->addAction( QIcon(":icons/harddrive.png"), tr("View Disk Usage"),this,SLOT(slotOpenFSDialog()) );
    sysMenu->addAction( QIcon(":icons/refresh.png"),tr("Rescan Devices"), this, SLOT(slotRescan()) );
    //Add the setting dialog option seperately
    sysMenu->addSeparator();
    sysMenu->addAction( QIcon(":icons/dvd.png"), tr("Load ISO File"), this, SLOT(slotOpenISO()) );
    sysMenu->addAction( QIcon(":icons/config.png"), tr("Change Settings"), this, SLOT(slotOpenSettings()) );
    //Add the Close button seperately
    sysMenu->addSeparator();
    sysMenu->addAction( QIcon(":icons/application-exit.png"), tr("Close Tray"), this, SLOT(closeTray()) );
  
    menuline = trayIconMenu->addSeparator();
    trayIconMenu->addMenu(sysMenu);
    
  // Tie the left-click signal to open the context menu
  connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(slotTrayActivated()) );
  //Connect the message clicked slot
  connect(trayIcon,SIGNAL(messageClicked()),this,SLOT(slotPopupClicked()) );
  //Set the default Tray Icon (will change once tray menus are set)
  trayIcon->setIcon(QIcon(":icons/CDdevices-inactive.png"));
  trayIcon->show();

  //Do an initial scan of the devices with dmesg
  //qDebug() << "-Performing initial device scan";
  //scanInitialDevices();

  
  //Startup the devd watching process
  qDebug() << "-Starting up the DEVD watcher";
  devdTimer = new QTimer();
  devdTimer->setSingleShot(true);
  connect(devdTimer,SIGNAL(timeout()),this,SLOT(UpdateDeviceMenu()) ); //slotDevChanges()));
  startupDevdProc();
  
  //Start up the filesystem watcher
  diskWatcher = new FSWatcher();
  connect(diskWatcher,SIGNAL(FSWarning(QString,QString)),this,SLOT(slotDisplayWarning(QString,QString)));
  if(useDiskWatcher){ 
    qDebug() << "-Starting up the disk space alert system";
    diskWatcher->start(diskTimerMaxMS); 
  }
  
  //Update the tray menu and icons
  //updateMenu();
  UpdateDeviceMenu();
  
  qDebug() << "-Program now ready for use";
  QTimer::singleShot(15000, this, SLOT(slotDoneWithInit()) ); //give it 15 seconds before allowing single-instance or FS checks
}

void MountTray::slotTrayActivated() {
     UpdateDeviceMenu(true); //do the quick version
     //qDebug() << "Show Menu";
     trayIcon->contextMenu()->popup( QPoint( trayIcon->geometry().x()+(trayIcon->geometry().width()/2), trayIcon->geometry().y() + (trayIcon->geometry().height()/2)  ) );
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
  devdTimer->start(1500); //wait 1.5 seconds before checking for device changes
  return;
}

void MountTray::closeTray(){
  qDebug() << "pc-mounttray: closing down";
  //Kill the devd watching process
  qDebug() << " -Shutting down DEVD watcher";
  devdProc->disconnectFromServer();
  //Close down the application
  exit(0);
}

void MountTray::slotRescan(){
  //Display a notification
  qDebug() << "Re-scanning devices:";
  slotDisplayPopup(tr("Please Wait"),tr("Rescanning devices attached to the system"));
  //Rescan the device list for new devices
  UpdateDeviceMenu(false, true); //flag this as a refresh (don't show notifications)
  //Run the disk check if appropriate
  if(useDiskWatcher){ diskWatcher->checkFS(); }
  //Re-open the menu
  slotTrayActivated();
}

void MountTray::slotOpenFSDialog(){
  //Open up the Filesystem disk space monitoring dialog
  diskDisplay = new FSDialog();
  diskDisplay->show();
}

void MountTray::slotOpenSettings(){
  //Stop the refresh timer on the watcher
  diskWatcher->stop();
  //Open up the settings window and apply changes as necessary
  SettingsDialog *sdlg = new SettingsDialog();
  sdlg->useDiskWatcher = useDiskWatcher;
  sdlg->useDiskAutoTimer = useDiskTimerDevd;
  sdlg->diskRefreshMS = diskTimerMaxMS;
  sdlg->showDialog();
  //Now parse the output and save if necessary
  if(sdlg->SettingsChanged){
    useDiskWatcher = sdlg->useDiskWatcher;
    useDiskTimerDevd = sdlg->useDiskAutoTimer;
    diskTimerMaxMS = sdlg->diskRefreshMS;
    qDebug() << "INFO: Saving updated settings to file";
    saveCurrentSettings(); //update the saved settings
  }
  //Now restart the disk watcher if enabled
  if(useDiskWatcher){ diskWatcher->start(diskTimerMaxMS); }
}

void MountTray::slotOpenISO(){
  //prompt for the user to select a file
  QString file = QFileDialog::getOpenFileName( this, tr("Select ISO File"), QDir::homePath(), tr("ISO Files (*.iso)") );
  if(file.isEmpty()){ return; } //cancelled
  
  QString cmd = "pc-sysconfig \"load-iso "+file+"\"";
  QProcess::startDetached(cmd);
}

void MountTray::slotSingleInstance()
{
  trayIcon->show();
  //Also pop-up the mount tray settings dialog
  if(MTINIT){ return; }
  slotOpenSettings();
}

void MountTray::slotDoneWithInit(){
  MTINIT = false;
}

void MountTray::slotDisplayPopup(QString title, QString msg, QString device){
  popupSave = device; //so we know what to do when it is clicked
  //Display a popup bubble with the given message for 2 seconds
  trayIcon->contextMenu()->hide(); //close the menu list
  trayIcon->showMessage(title, msg , QSystemTrayIcon::NoIcon,1000 );
}

void MountTray::slotDisplayWarning(QString title, QString msg){
  popupSave="FSCHECK";
  //Display a popup bubble with the given message for 5 seconds
  trayIcon->contextMenu()->hide(); //close the menu list
  trayIcon->showMessage(title, msg , QSystemTrayIcon::Warning,5000 );
}

void MountTray::slotPopupClicked(){
  //Check the saved variable for what to do with this popup
  if(popupSave == "FSCHECK"){
    //Open up the filesystem disk space UI
    slotOpenFSDialog();
  }else if(!popupSave.isEmpty()){
    //Check if it is a currently valid device
    /*if(!popupSave.startsWith(DEVICEDIR)){ popupSave.prepend(DEVICEDIR); }
    for(int i=0; i<deviceList.length(); i++){
      if( deviceList[i]->device == popupSave){
        //See if the device is mounted
	if(deviceList[i]->isMounted()){
	  //Open up the mountpoint directory
	  openMediaDir(deviceList[i]->mountpoint);
	}else{
	  //Mount the device
	  deviceList[i]->mountItem();
	}
        break;
      }
    }*/
  }

}

void MountTray::loadSavedSettings(){
  //The saved settings file
  QString filename = QDir::homePath()+"/.mounttray.settings";
  //Set the defaults
  useDiskWatcher=true; useDiskTimerDevd=true;
  diskTimerMaxMS=3600000; //1 hour refresh timer
  //Now load the file
  QFile file(filename);
  if(file.exists()){
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){ 
      qDebug() << "-Could not open settings file: using defaults";
      return; 
    }
    QTextStream in(&file);
    while(!in.atEnd()){
      QString line = in.readLine();
      if(!line.startsWith("#")){ //skip comment lines
        QString var = line.section(")",0,0,QString::SectionSkipEmpty).simplified();
        QString val = line.section(")",1,30,QString::SectionSkipEmpty).simplified();
        if(var=="UseDiskSpaceMonitoring"){ 
          if(val.toLower() == "true"){ useDiskWatcher = true;}
          else{ useDiskWatcher = false; }
        }else if(var=="UseDiskSpaceDevdTiming"){
          if(val.toLower() == "true"){ useDiskTimerDevd = true;}
          else{ useDiskTimerDevd = false; }	
        }else if(var=="DiskSpaceTimingMaxMilliseconds"){
          diskTimerMaxMS = val.toInt();	
        }
      }
    }
    file.close();
  }else{
    qDebug() << "-Creating new settings file with defaults";
    saveCurrentSettings();
  }
}

void MountTray::saveCurrentSettings(){
  //The saved settings file
  QString filename = QDir::homePath()+"/.mounttray.settings";
  //Now write the current values to the file
  QFile file(filename);
  if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
    qDebug() << "ERROR: Could not open file to save settings:"<<filename;
    return;
  }
  QTextStream out(&file);
  out << "#pc-mounttray saved settings file\n";
  out << "# DO NOT EDIT: Use the settings dialog in the application instead!!\n";
  //Save the settings
  out << "UseDiskSpaceMonitoring)";
  if(useDiskWatcher){ out << "true\n";}
  else{ out << "false\n"; }
  out << "UseDiskSpaceDevdTiming)";
  if(useDiskTimerDevd){ out << "true\n";}
  else{ out << "false\n"; }
  out << "DiskSpaceTimingMaxMilliseconds)"+QString::number(diskTimerMaxMS)+"\n";
  //Now close the file
  file.close();
}

void MountTray::slotCloseMenu(){
  trayIcon->contextMenu()->hide();
}

/*void MountTray::slotOpenAVDisk(QString type){
  if(MTINIT){ return; } //don't open the launcher during program initialization
  //Get the list of all AudioVideo Applications on the sytem
  QList<XDGFile> apps = XDGUtils::allApplications();
  apps = XDGUtils::filterAppsByCategory("AudioVideo", apps);
  apps = XDGUtils::sortAppsByName(apps);
  //Now generate the QStringList of application names
  QStringList names;
  for(int i=0; i<apps.length(); i++){
    //Filter out "invalid" applications (mixers, etc..)
    QString rname = apps[i].RawName();
    if(rname.contains("mixer", Qt::CaseInsensitive) || rname.contains("control", Qt::CaseInsensitive) ){ 
	//skip it
	apps.removeAt(i);
	i--;
    }else{
      QString txt;
      if( apps[i].Comment().isEmpty() ){ txt = apps[i].Name(); }
      else{ txt = apps[i].Name() +" ("+apps[i].Comment()+")"; }
      //Make sure that SMPlayer is listed first and recommended
      if(apps[i].RawName().toLower()=="smplayer"){
	 txt = apps[i].Name()+ "  **"+tr("Recommended")+"**"; 
	 names.prepend(txt); //put at the top
	 apps.move(i,0); //move the file to the front as well
      }else{
	 names << txt;
      }
    }
  }
  //Prompt for the user to select an application
  bool ok = false;
  QString appname = QInputDialog::getItem(0, QString(tr("%1 Disk")).arg(type) , tr("Open With:"), names,0, false, &ok);
  if(!ok || appname.isEmpty()){ return; }
  int index = names.indexOf(appname);
  if(index == -1){ return; }
  //Now start the application
  qDebug() << "Open "+type.toLower()+" disk:";
  qDebug() << " -- With:"<<appname;
  QString cmd = apps[index].Exec();
  //Only auto-start the disk with SMPlayer - no guarantee this method works for other apps
  if(apps[index].RawName().toLower()=="smplayer"){
    if(type.toLower()=="audio"){
      cmd.append(" cdda://1"); //audio cd
    }else{ //video DVD
      cmd.append(" dvd://1"); //video dvd
    }
  }
  qDebug() << " -- Exec:" << cmd;
  QProcess::startDetached( cmd );
}*/
  
void MountTray::UpdateDeviceMenu(bool fast, bool refresh){
  QStringList avail, mounted;
  QStringList tmp = pcbsd::Utils::runShellCommand("pc-sysconfig list-remdev list-mounteddev");
  if(tmp.length()<2 || tmp.join("").contains("Client Connection Error:") ){ return; } //invalid return
  if(!tmp[0].contains("[NO INFO]")){ avail = tmp[0].split(", "); }
  if(!tmp[1].contains("[NO INFO]")){ mounted = tmp[1].split(", "); }
  //qDebug() << "Update Devices:" << avail << mounted;
  //Update the current menu items as necessary
  bool newitems = false;
  for(int i=0; i<DEVLIST.length(); i++){
    QString dev = DEVLIST[i]->node();
    //qDebug() << "Check device:" << dev;
    if(avail.contains(dev)){ 
      if(fast){ DEVLIST[i]->QuickUpdate(mounted.contains(dev)); }
      else{ DEVLIST[i]->UpdateDevice(mounted.contains(dev)); }
      avail.removeAll(dev); //remove this device from the list for the moment
    }else{
      //Invalid device, remove it from the list
      disconnect(DEVLIST[i]);
      trayIconMenu->removeAction(DEVLIST[i]->action());
      DEVLIST.removeAt(i);
      i--;
    }
  }
  //Now create widgets for any new devices
  for(int i=0; i<avail.length(); i++){
    newitems = true;
    DeviceWidget *item = new DeviceWidget(this, avail[i]);
    connect(item, SIGNAL(CloseMenu()), this, SLOT(slotCloseMenu()) );
    connect(item, SIGNAL(RefreshDeviceList()), this, SLOT(UpdateDeviceMenu()) );
    connect(item, SIGNAL(ShowMessage(QString, QString)), this, SLOT(slotDisplayPopup(QString, QString)) );
    DEVLIST << item;
    trayIconMenu->insertAction(menuline, item->action()); //put them above the line
    item->UpdateDevice( mounted.contains(avail[i]) ); //need the full update to start
  }
  //Now show a popup message about any new devices
  if(!avail.isEmpty() && !MTINIT && newitems && !refresh){
    slotDisplayPopup(tr("Devices Available"), tr("New Devices are available for use"));
  }
  
  if(useDiskWatcher && useDiskTimerDevd && !MTINIT){ diskWatcher->checkFS(); }
  
   //Update the main icon based upon whether devices have been found
  if(DEVLIST.length()==0){
    trayIcon->setIcon( QIcon(":icons/CDdevices-inactive.png") );
  }else{
    if(mounted.length()==0){
      trayIcon->setIcon( QIcon(":icons/CDdevices.png") );
    }else{
      trayIcon->setIcon( QIcon(":icons/CDdevices.png") );
    }
  } 
}
