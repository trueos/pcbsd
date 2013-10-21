#include "LPTray.h"

//PUBLIC
LPTray::LPTray() : QSystemTrayIcon(){
  //Start up the log file watcher and connect the signals/slots
  watcher = new LPWatcher();
	connect(watcher,SIGNAL(MessageAvailable(QString)),this,SLOT(watcherMessage(QString)) );
	connect(watcher,SIGNAL(StatusUpdated()),this,SLOT(watcherMessage()) );
  //Setup the context menu
  menu = new QMenu;
	menu->addAction(QIcon(":/images/tray-icon-idle.png"),tr("Start Application UI"),this,SLOT(startGUI()) );
	menu->addSeparator();
	menu->addAction(QIcon(":/images/backup-failed.png"),tr("View Messages"),this,SLOT(startMessageDialog()) );
	menu->addAction(QIcon(":/images/refresh.png"),tr("Refresh Tray"),this,SLOT(refreshStatus()) );
	menu->addSeparator();
	menu->addAction(QIcon(":/images/application-exit.png"),tr("Close Life Preserver Tray"),this,SLOT(slotClose()) );
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
  if(type=="message"){
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
    if(info[0] == "STARTED"){
      this->showMessage( info[1], info[2], QSystemTrayIcon::Information, 5000);
    }else if(info[0] == "FINISHED"){
      this->showMessage( info[1], info[2], QSystemTrayIcon::Information, 5000);
    }else if(info[0] == "ERROR"){
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
      if(info[0]=="ERROR"){ 
	this->showMessage(info[1], info[2], QSystemTrayIcon::Warning, 5000);
      }else{ 
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
  }
}

void LPTray::startGUI(){
  //Start up the GUI
  system("pc-su life-preserver");
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
    QStringList devs=info[2].split(":::"); QStringList msgs=info[3].split(":::");
    for(int j=0; j<devs.length(); j++){
      if(info[0] == "ERROR"){
        errs << info[1] + " (" + devs[j] + "): " + msgs[j];
      }else if(!info[0].isEmpty()){
        msgs << info[1] + " (" + devs[j] + "): " + msgs[j];
      }
    }
    if(!info[4].isEmpty()){
      //allow for multiple log files
      logs << info[4].split(":::"); // In format:  "example logfile </var/logs/example.log>"
    }
  }
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
