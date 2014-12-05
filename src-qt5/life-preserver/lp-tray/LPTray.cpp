#include "LPTray.h"

//PUBLIC
LPTray::LPTray() : QSystemTrayIcon(){
  qDebug() << "Starting up Life-preserver Tray...";
  //Start up the log file watcher and connect the signals/slots
  watcher = new LPWatcher();
	connect(watcher,SIGNAL(MessageAvailable(QString)),this,SLOT(watcherMessage(QString)) );
	connect(watcher,SIGNAL(StatusUpdated()),this,SLOT(watcherMessage()) );
  //Load the tray settings file
  settings = new QSettings(QSettings::UserScope,"PCBSD", "life-preserver-tray");
  //Create the notification option widgets
  nShowAll = new QRadioButton(tr("Show all"));
	naAll = new QWidgetAction(this);
	naAll->setDefaultWidget(nShowAll);
  nShowError = new QRadioButton(tr("Warnings Only"));
	naErr = new QWidgetAction(this);
	naErr->setDefaultWidget(nShowError);
  nShowNone = new QRadioButton(tr("None"));
	naNone = new QWidgetAction(this);
	naNone->setDefaultWidget(nShowNone);
  //Create notification menu
  notificationMenu = new QMenu(tr("Popup Settings"));
	notificationMenu->setIcon( QIcon(":/images/configure.png") );
	notificationMenu->addAction(naAll);
	notificationMenu->addAction(naErr);
	notificationMenu->addAction(naNone);
  //Activate the proper notification setting widget
  QString popset = settings->value("popup-policy", "").toString();
  qDebug() << "Current Popup Policy:" << popset;
  if(popset=="all"){ nShowAll->setChecked(true); popupPolicy = 2; }
  else if(popset=="errors"){ nShowError->setChecked(true); popupPolicy = 1; }
  else if(popset=="none"){ nShowNone->setChecked(true); popupPolicy = 0; }
  else{ 
    nShowError->setChecked(true);
    settings->setValue("popup-policy","errors"); //save the proper setting
    popupPolicy = 1;
  }
  //Now connect the popup settings signals/slots
  connect(nShowAll, SIGNAL(clicked()), this, SLOT(changePopupPolicy()) );
  connect(nShowError, SIGNAL(clicked()), this, SLOT(changePopupPolicy()) );
  connect(nShowNone, SIGNAL(clicked()), this, SLOT(changePopupPolicy()) );
  //Setup the context menu
  menu = new QMenu;
	menu->addAction(QIcon(":/images/tray-icon-idle.png"),tr("Open Life Preserver"),this,SLOT(startGUI()) );
	menu->addSeparator();
	menu->addAction(QIcon(":/images/backup-failed.png"),tr("View Messages"),this,SLOT(startMessageDialog()) );
        menu->addMenu(notificationMenu);
	menu->addAction(QIcon(":/images/refresh.png"),tr("Refresh Tray"),this,SLOT(refreshStatus()) );
	menu->addSeparator();
	menu->addAction(QIcon(":/images/application-exit.png"),tr("Close Tray"),this,SLOT(slotClose()) );
  this->setContextMenu(menu);
  //Setup initial icon for the tray
  this->setIcon( QIcon(":/images/tray-icon-idle.png") );
  //Create the messages GUI
  msgdlg = new LPMessages();
  //connect other signals/slots
  connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(slotTrayClicked(QSystemTrayIcon::ActivationReason)) );
  //Start up the watcher
  watcher->start();
  updateTrayIcon();
  updateToolTip();
}

LPTray::~LPTray(){
  watcher->stop();
  delete watcher;
  delete msgdlg;
}

// ===============
//  PRIVATE FUNCTIONS
// ===============
void LPTray::updateTrayIcon(){
  if( watcher->hasError() ){
    //Errors - show that attention is required
    this->setIcon( QIcon(":/images/tray-icon-error.png") );
  }else if( watcher->isRunning() ){
    //Show the working icon
    this->setIcon( QIcon(":/images/tray-icon-working.png") );
  }else{
    //Show the idle icon
    this->setIcon( QIcon(":/images/tray-icon-idle.png") );
  }
  
}

void LPTray::updateToolTip(){
  QStringList info = watcher->getAllCurrentMessages();
  this->setToolTip( info.join("\n") );	
}

// ===============
//     PRIVATE SLOTS
// ===============
void LPTray::watcherMessage(QString type){
  qDebug() << "New Watcher Message:" << type;
  QStringList info;
  if(type=="message" &&  popupPolicy > 1){
    //Show the message pop-up
    info << "time" << "message";
    info = watcher->getMessages(type,info);
    if(!info.isEmpty()){
      this->showMessage( info[0], info[1], QSystemTrayIcon::Information, 5000);
    }
  }else if(type=="replication"){
    info << "id" << "time" << "message";
    info = watcher->getMessages(type,info);
    if(info.isEmpty()){ return; }
    if(info[0] == "STARTED" && popupPolicy > 1 ){
      this->showMessage( info[1], info[2], QSystemTrayIcon::Information, 5000);
    }else if(info[0] == "FINISHED" && popupPolicy > 1){
      this->showMessage( info[1], info[2], QSystemTrayIcon::Information, 5000);
    }else if(info[0] == "ERROR" && popupPolicy > 0){
      this->showMessage( info[1], info[2], QSystemTrayIcon::Warning, 10000);
    }
    
  }else if(type=="critical"){
    info << "time" << "message" << "device";
    info = watcher->getMessages(type,info);
    if(!info.isEmpty()){
      QString msg = QString(tr("Time: %1")).arg(info[0]);
      //message/device information can have more than one listed
      QStringList devs = info[2].split(":::");	    
      QStringList msgs = info[1].split(":::");
      for(int i=0; i<devs.length(); i++){
        msg.append("\n"+QString(tr("%1: %2")).arg(devs[i],msgs[i]) );
      }
      QString title = tr("System Alert");
      QMessageBox::warning(0, title ,msg);
    }
  }else if(type=="mirror"){
	  
  }else if(type=="scrub" || type =="resilver"){
    info << "id" << "time" << "message";
    info = watcher->getMessages(type, info);
    if(!info.isEmpty()){
      if(info[0]=="ERROR" && popupPolicy > 0 ){ 
	this->showMessage(info[1], info[2], QSystemTrayIcon::Warning, 5000);
      }else if(popupPolicy > 1){ 
	this->showMessage(info[1], info[2], QSystemTrayIcon::Information, 5000); 
      }
    }
  }
  //Update the tray icon
  updateTrayIcon();
  //Update the tooltip
  updateToolTip();
}

void LPTray::slotTrayClicked(QSystemTrayIcon::ActivationReason reason){
  if(reason == QSystemTrayIcon::Trigger || reason == QSystemTrayIcon::Context){
    this->contextMenu()->popup(QCursor::pos());
  }
}

void LPTray::slotClose(){
  exit(0);
}

void LPTray::slotSingleInstance(){
  this->show();
  if(msgdlg->isVisible()){ 
    msgdlg->raise(); 
    msgdlg->show();
  }else if(!watcher->initPhase()){
    startGUI();
  }
}

void LPTray::startGUI(){
  //Start up the GUI
  QProcess::startDetached("pc-su life-preserver");
}

void LPTray::startMessageDialog(){
  //Get all the available messages from the watcher
  QStringList msgs, errs, logs; //message variables to fill
  //Cycle through the watcher types and get any messages
  QStringList types; types << "message" << "replication" << "critical" << "mirror" << "resilver" << "scrub";
  QStringList infoL; infoL << "id" << "timestamp" << "device" << "message" << "files";
  for(int i=0; i<types.length(); i++){
    QStringList info = watcher->getMessages(types[i], infoL);
    if(info.isEmpty()){continue;}
    //Take into account multiple devices/messages per type
    QStringList devs=info[2].split(":::"); QStringList msg=info[3].split(":::");
    for(int j=0; j<devs.length(); j++){
      if(info[0] == "ERROR"){
        errs << info[1] + " (" + devs[j] + "): " + msg[j];
      }else if(!info[0].isEmpty()){
        msgs << info[1] + " (" + devs[j] + "): " + msg[j];
      }
    }
    if(!info[4].isEmpty()){
      //allow for multiple log files
      logs << info[4].split(":::"); // In format:  "example logfile </var/logs/example.log>"
    }
  }

  // Add some of the standard log files
  logs << "System Logfile </var/log/lpreserver/lpreserver.log>";
  logs << "Error Log </var/log/lpreserver/lpreserver_failed.log>";

  //Now start up the dialog
  msgdlg->setMessages( msgs.join("\n"), errs.join("\n") );
  msgdlg->setLogFiles(logs);
  msgdlg->raise();
  msgdlg->show();
}

void LPTray::refreshStatus(){
  //Manually trigger the watcher to update status notifications
  watcher->refresh();
}

void LPTray::changePopupPolicy(){
  QString policy;
  if(nShowAll->isChecked()){
    policy = "all";
    popupPolicy = 2;
  }else if(nShowError->isChecked()){
    policy = "errors";
    popupPolicy = 1;
  }else if(nShowNone->isChecked()){
    policy = "none";
    popupPolicy = 0;
  }else{
    policy = "errors";
    popupPolicy = 1;
  }
  qDebug() << "Change Popup Policy:" << policy;
  settings->setValue("popup-policy", policy);
}
