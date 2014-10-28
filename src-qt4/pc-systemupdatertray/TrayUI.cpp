#include "TrayUI.h"

//PUBLIC
TrayUI::TrayUI() : QSystemTrayIcon(){
  qDebug() << "Starting Up System Updater Tray...";
  //Set all the initial flags ( <0 means do initial checks if appropriate)
  PKGSTATUS=-1;
  SYSSTATUS=-1;
  WARDENSTATUS=-1;
  noInternet = false; //assume internet is available until we get a failure
  wasworking = false;
  //Load the tray settings file
  settings = new QSettings("PCBSD");
    settings->sync(); //make sure to load it right away
  //Setup the checktimer
  chktime = new QTimer(this);
	chktime->setInterval(1000 * 60 * 60 * 24); //every 24 hours
	connect(chktime, SIGNAL(timeout()), this, SLOT(checkForUpdates()) );
  //Generate the Menu
  menu = new QMenu(0);
  this->setContextMenu(menu);
  connect(menu, SIGNAL(triggered(QAction*)), this, SLOT(slotItemClicked(QAction*)) );
  //Now fill the menu with items
    // - System Update Manager
  QAction* act = new QAction( QIcon(":/images/sysupdater.png"), tr("Start the Update Manager"), this);
	act->setWhatsThis("sys"); //system updater code
	menu->addAction(act);
    // - Separator
  menu->addSeparator();
    // - AppCafe
  act = new QAction( QIcon(":/images/appcafe.png"), tr("Start the AppCafe"), this);
	act->setWhatsThis("pkg"); // PKG code
	menu->addAction(act);
    // - Warden
  act = new QAction( QIcon(":/images/warden.png"), tr("Start the Warden"), this);
	act->setWhatsThis("warden"); //warden code
	menu->addAction(act);
    // - Separator
  menu->addSeparator();
    // - Check for Updates
  act = new QAction( QIcon(":/images/view-refresh.png"), tr("Check For Updates"), this);
	act->setWhatsThis("update"); //update check code
	menu->addAction(act);
    // - Separator
  menu->addSeparator();
    // - Run At Startup Checkbox
  runAtStartup = new QCheckBox(tr("Run At Startup"), 0);
    runAtStartup->setChecked( settings->value("/PC-BSD/SystemUpdater/runAtStartup",true).toBool() );
    connect(runAtStartup, SIGNAL(clicked()), this, SLOT(slotRunAtStartupClicked()) );
  rasAct = new QWidgetAction(this);
    rasAct->setDefaultWidget(runAtStartup);
    menu->addAction(rasAct);
    // - Display Notifications Checkbox
  showNotifications = new QCheckBox(tr("Display Notifications"), 0);
    showNotifications->setChecked( settings->value("/PC-BSD/SystemUpdater/displayPopup",true).toBool() );
    connect(showNotifications, SIGNAL(clicked()), this, SLOT(slotShowMessagesClicked()) );
  snAct = new QWidgetAction(this);
    snAct->setDefaultWidget(showNotifications);
    menu->addAction(snAct);   

  makeScheduleMenu();
  menu->addAction(tr("Automatic updates check"))->setMenu(schedule_menu);

    // - Separator
  menu->addSeparator();
    // - Warden
  act = new QAction( tr("Quit"), this);
	act->setWhatsThis("quit"); //system updater code
	menu->addAction(act);

  //Now Update the tray visuals
  updateTrayIcon();
  updateToolTip();
  //Start up the system flag watcher and connect the signals/slots
  watcher = new SystemFlagWatcher(this);
	connect(watcher,SIGNAL(FlagChanged(SystemFlags::SYSFLAG, SystemFlags::SYSMESSAGE)),this,SLOT(watcherMessage(SystemFlags::SYSFLAG, SystemFlags::SYSMESSAGE)) );
  //watcher->checkForRecent(10); //Check for flags in the last 10 minutes
  
  //Now connect the tray clicked signal
  connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(slotTrayClicked(QSystemTrayIcon::ActivationReason)) );
  connect(this, SIGNAL(messageClicked()), this, SLOT(launchApp()) );
  
  //Startup the initial checks in 1 minute
  QTimer::singleShot(60000, this, SLOT(startupChecks()));

  //Periodically check timer start
  checkTimer=new QTimer(this);
  checkTimer->setInterval(60000); // 1min
  connect(checkTimer, SIGNAL(timeout()), this, SLOT(slotCheckTimer()));
  checkTimer->start();

}

TrayUI::~TrayUI(){
}

void TrayUI::makeScheduleMenu()
{
    schedule_menu = new QMenu(menu);
    int interval= settings->value("/PC-BSD/SystemUpdater/checkIntervalMin",360).toInt();

    disableAutoCheck = new QRadioButton(tr("Disable"), schedule_menu);
    disableAutoCheck->setChecked(interval <= 0);
    dacAct = new QWidgetAction(this);
    dacAct->setDefaultWidget(disableAutoCheck);
    connect(disableAutoCheck, SIGNAL(clicked()), this, SLOT(slotParsePeriodControls()) );
    schedule_menu->addAction(dacAct);

    schedule_menu->addSeparator();

    checkEveryHour = new QRadioButton(tr("Every hour"), schedule_menu);
    checkEveryHour->setChecked((interval>0)&&(interval<=60));
    connect(checkEveryHour, SIGNAL(clicked()), this, SLOT(slotParsePeriodControls()) );
    dhAct = new QWidgetAction(this);
    dhAct->setDefaultWidget(checkEveryHour);
    schedule_menu->addAction(dhAct);

    checkEvery6hrs = new QRadioButton(tr("Every 6 hours"), schedule_menu);
    checkEvery6hrs->setChecked((interval>60)&&(interval<=360));
    connect(checkEvery6hrs, SIGNAL(clicked()), this, SLOT(slotParsePeriodControls()) );
    d6hAct = new QWidgetAction(this);
    d6hAct->setDefaultWidget(checkEvery6hrs);
    schedule_menu->addAction(d6hAct);

    checkEveryDay = new QRadioButton(tr("Every day"), schedule_menu);
    checkEveryDay->setChecked(interval>360);
    connect(checkEveryDay, SIGNAL(clicked()), this, SLOT(slotParsePeriodControls()) );
    ddAct = new QWidgetAction(this);
    ddAct->setDefaultWidget(checkEveryDay);
    schedule_menu->addAction(ddAct);

    currentCheckInterval = interval;
    nextCheckTime = QTime::currentTime().addSecs(interval*60);
}

// ===============
//  PRIVATE FUNCTIONS
// ===============
void TrayUI::updateTrayIcon(){
  bool isworking = false;
  QString msg;
  if(SYSSTATUS==3 || PKGSTATUS==3 || WARDENSTATUS==3){
    this->setIcon( QIcon(":/images/updating.png") );
    isworking = true;
  }else if( SYSSTATUS==1 || PKGSTATUS==1 || WARDENSTATUS==1){
    this->setIcon( QIcon(":/images/working.png") );
    isworking = true;
  }else if( rebootNeeded() ){
    this->setIcon( QIcon(":/images/restart.png") );
    msg = tr("System Reboot Required");
  }else if( noInternet ){
    this->setIcon( QIcon(":/images/connecterror.png") );
  }else if(SYSSTATUS==2){
    this->setIcon( QIcon(":/images/sysupdates.png") );
    msg = tr("System Updates Available");
  }else if(PKGSTATUS==2){
    this->setIcon( QIcon(":/images/pkgupdates.png") );
    msg = tr("Package Updates Available");
  }else if(WARDENSTATUS==2){
    this->setIcon( QIcon(":/images/sysupdates.png") );
    msg = tr("Jail Updates Available");
  }else{
    //everything up to date
    this->setIcon( QIcon(":/images/updated.png") );
  }
  //Show a popup Notification if done working
  if(!isworking && wasworking && showNotifications->isChecked() && !msg.isEmpty()){
    this->showMessage(tr("PC-BSD System Message"),msg,QSystemTrayIcon::Information, 2000); //2 sec notification
  }
  wasworking = isworking; //save this for later
}

void TrayUI::updateToolTip(){
  QString msg = tr("PC-BSD Update Manager")+"\n";
  //Now generate the tooltip
  if(rebootNeeded()){
    msg.append( "\n"+tr("System Reboot Required"));
  }else if(noInternet){
    msg.append("\n"+tr("Error checking for updates")+"\n"+tr("Please make sure you have a working internet connection") );
  }else if(SYSSTATUS<=0 && PKGSTATUS<=0 && WARDENSTATUS<=0){
    msg.append("\n"+tr("Your system is fully updated") );
  }else{
    if(SYSSTATUS==2){ msg.append("\n"+tr("System Updates Available")); }
    else if(SYSSTATUS==1){ msg.append("\n"+tr("Checking for system updates...") ); }
    else if(SYSSTATUS==3){ msg.append("\n"+tr("System Updating...") ); }
    if(PKGSTATUS==2){ msg.append("\n"+tr("Package Updates Available") ); }
    else if(PKGSTATUS==1){ msg.append("\n"+tr("Checking for package updates...") ); }
    else if(PKGSTATUS==3){ msg.append("\n"+tr("Packages Updating...") ); }
    if(WARDENSTATUS==2){ msg.append("\n"+tr("Jail Updates Available") ); }
    else if(WARDENSTATUS==1){ msg.append("\n"+tr("Checking for jail updates...") ); }
    else if(WARDENSTATUS==3){ msg.append("\n"+tr("Jails Updating...") ); }
  }
  
  this->setToolTip(msg);
}

bool TrayUI::rebootNeeded(){
  return QFile::exists("/tmp/.fbsdup-reboot");
}

void TrayUI::startPKGCheck(){
  if(PKGSTATUS==1){ return; } //already checking for updates
  qDebug() << " -Starting Package Check...";
  PKGSTATUS=1; //working
  updateTrayIcon();
  updateToolTip();
  QString info = pcbsd::Utils::runShellCommand("syscache \"pkg #system hasupdates\"").join("");
  if(info.isEmpty() || info.contains("ERROR") ){ PKGSTATUS=0; noInternet=true; }
  else if(info.toLower().simplified()=="true"){ PKGSTATUS=2; noInternet=false; }
  else{ PKGSTATUS=0; noInternet=false; } //no updates available
}

void TrayUI::startSYSCheck(){
  if(rebootNeeded()){ return; } //do not start another check if a reboot is required first
  if(SYSSTATUS==1){ return; } //already checking for updates
  qDebug() << " -Starting System Check...";
  //QString cmd = "sudo pc-updatemanager check";
  SYSSTATUS=1; //working
  //QProcess::startDetached(cmd);
  updateTrayIcon();
  updateToolTip();  
  QString info = pcbsd::Utils::runShellCommand("syscache hasupdates").join("");
  if(info.isEmpty() || info.contains("ERROR") ){ SYSSTATUS=0; }
  else if(info.toLower().simplified()=="true"){ SYSSTATUS=2; }
  else{ SYSSTATUS=0; } //no updates available
}

void TrayUI::startWardenCheck(){
  //WARDENSTATUS=0;
  //return; //Warden check command not currently working - just keep it invisible
  //-------
  if(rebootNeeded()){ return; } //do not start another check if a reboot is required first
  if(WARDENSTATUS==1){ return; } //already checking for updates
  qDebug() << " -Starting Warden Check...";
  WARDENSTATUS=1; //working
  updateTrayIcon();
  updateToolTip();
  
  QStringList info = pcbsd::Utils::runShellCommand("syscache \"jail list\"").join("").split(", ");
  for(int i=0; i<info.length(); i++){
    if(info[i].isEmpty()){ continue; }
    else if( info[i].contains("[ERROR]") ){ WARDENSTATUS=0; break;} //unknown jails - assume none so success
    //Check for updates in this jail
    info[i] = pcbsd::Utils::runShellCommand("syscache \"pkg "+info[i]+" hasupdates\"").join("");
    if(info[i].toLower().simplified()=="true"){ WARDENSTATUS=2; break; } //updates available: stop checking others
    //keep checking if it gets here (no updates for this one)
  }
  if(WARDENSTATUS==1){ WARDENSTATUS=0; } //Nothing found - success
}

void TrayUI::setCheckInterval(int sec)
{    
    if (!sec)
    {
        if (checkTimer->isActive())
        {
            checkTimer->stop();
        }
    }
    else
    {
        nextCheckTime = QTime::currentTime().addSecs(sec);
        if (!checkTimer->isActive())
        {
            checkTimer->start();
        }
    }

    if (sec!=currentCheckInterval)
        settings->setValue("/PC-BSD/SystemUpdater/checkIntervalMin", sec);
    currentCheckInterval = sec;
}

// ===============
//     PRIVATE SLOTS
// ===============
void TrayUI::checkForUpdates(){
  //Simplification function to start all checks
    startSYSCheck();
    startPKGCheck();
    startWardenCheck();
    updateTrayIcon();
    updateToolTip();
    QTimer::singleShot(60000, watcher, SLOT(checkFlags()) ); //make sure to manually check 1 minute from now
}

void TrayUI::startupChecks(){
  //Slot to perform startup checks as necessary
  // - This should make sure we don't re-check systems that were checked recently
  if(SYSSTATUS<0){ startSYSCheck(); }
  if(PKGSTATUS<0){ startPKGCheck(); }
  if(WARDENSTATUS<0){ startWardenCheck(); }
  updateTrayIcon();
  updateToolTip();  
  QTimer::singleShot(60000, watcher, SLOT(checkFlags()) ); //make sure to manually check 1 minute from now
}

void TrayUI::launchApp(QString app){
  //Check for auto-launch
  if(app.isEmpty()){
    if(SYSSTATUS==2){ app = "sys"; }
    else if(PKGSTATUS==2){ app = "pkg"; }
    else if(WARDENSTATUS==2){ app = "warden"; }
    else{ app = "pkg"; }
  }
  //Now Launch the proper application
  QString cmd;
  if(app=="sys"){
    cmd = "pc-su pc-updategui";
  }else if(app=="pkg"){
    cmd = "pc-softwaremanager";
  }else if(app=="warden"){
    cmd = "pc-su warden gui";
  }else{ 
    return; //invalid app specified
  }
  qDebug() << "Startup External Application:" << cmd;
  QProcess::startDetached(cmd);
}

void TrayUI::watcherMessage(SystemFlags::SYSFLAG flag, SystemFlags::SYSMESSAGE msg){
  //reset the noInternet flag (prevent false positives, since something obviously just changed)
  bool oldstat = noInternet;
  if(flag != SystemFlags::NetRestart){ noInternet = false; }
  switch(flag){
	case SystemFlags::NetRestart:
	  if(msg==SystemFlags::Error){ noInternet = true; }
	  else{ noInternet = false; }
	  if(!noInternet && oldstat){ checkForUpdates(); } //only re-check if no internet previously available
	  break;
	case SystemFlags::PkgUpdate:
	  if(msg==SystemFlags::Working){ PKGSTATUS=1; }
	  else if(msg==SystemFlags::Updating){ PKGSTATUS=3; }
	  else{ startPKGCheck(); } //check it
	  break;
	case SystemFlags::SysUpdate:
	  if(msg==SystemFlags::Working){ SYSSTATUS=1; }
	  else if(msg==SystemFlags::Updating){ SYSSTATUS=3; }
	  else{ startSYSCheck(); } //unknown - check it
	  break;	
	case SystemFlags::WardenUpdate:
	  if(msg==SystemFlags::Working){ WARDENSTATUS=1; }
	  else if(msg==SystemFlags::Updating){ WARDENSTATUS=3; }
	  else{ startWardenCheck(); } //check it
	  break;	
  }
  qDebug() << "System Status Change:" << SYSSTATUS << PKGSTATUS << WARDENSTATUS << noInternet;
  //Update the tray icon
  updateTrayIcon();
  //Update the tooltip
  updateToolTip();
}

void TrayUI::slotItemClicked(QAction* act){
  QString code = act->whatsThis();
  if(code=="quit"){
    //Close the tray
    slotClose();
  }else if(code=="update"){
    //Check for updates
    checkForUpdates();
  }else if(code.isEmpty()){
    return;
  }else{
    //Launch one of the external applications
    launchApp(code);
  }
  
}

void TrayUI::slotTrayClicked(QSystemTrayIcon::ActivationReason reason){
  if(reason == QSystemTrayIcon::Context){
    this->contextMenu()->popup(QCursor::pos());
  }else{
    launchApp();
  }
}

void TrayUI::slotRunAtStartupClicked(){
  settings->setValue("/PC-BSD/SystemUpdater/runAtStartup",runAtStartup->isChecked());
  settings->sync(); //make sure to save to file right away
  //Now be sure to also save this to the PC-BSD system registry so it is acted upon properly
  QString cmd = "pbreg set /PC-BSD/SystemUpdater/runAtStartup false";
  if(runAtStartup->isChecked()){ cmd.replace(" false", " true"); }
  QProcess::startDetached(cmd);
}

void TrayUI::slotShowMessagesClicked(){
  settings->setValue("/PC-BSD/SystemUpdater/displayPopup",showNotifications->isChecked());	
  settings->sync(); //make sure to save to file right away
}

void TrayUI::slotClose(){
  qDebug() << "pc-systemupdatertray: Closing down...";
  QApplication::exit(0);
}

void TrayUI::slotSingleInstance(){
  this->show();
    //do nothing else at the moment
}

void TrayUI::slotCheckTimer()
{    
    int secsTo = QTime::currentTime().secsTo(nextCheckTime);
    //qDebug()<<secsTo;
    if (secsTo <= 0)
    {
        //It is time to check updates
        if ((PKGSTATUS == 3) || (PKGSTATUS==1)
          ||(WARDENSTATUS == 3) || (WARDENSTATUS == 1)
          ||(SYSSTATUS == 3) || (SYSSTATUS == 1))
        {
            //if busy now check after a minute
            nextCheckTime.addSecs(60);
            return;
        }
        qDebug()<<"Checking updates by schedule";
        checkForUpdates();
        nextCheckTime = QTime::currentTime().addSecs(currentCheckInterval * 60);
    }
}

void TrayUI::slotParsePeriodControls()
{
    int interval=0;
    if (checkEveryHour->isChecked())
        interval= 60;
    else if (checkEvery6hrs->isChecked())
        interval= 360;
    else if (checkEveryDay->isChecked())
        interval = 60 * 24;
    setCheckInterval(interval);
}
