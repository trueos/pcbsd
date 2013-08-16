#include "LPTray.h"

//PUBLIC
LPTray::LPTray() : QSystemTrayIcon(){
  //Start up the log file watcher
  QString logfile = "/var/log/lpreserver/lpreserver.log";
  watcher = new QFileSystemWatcher();
	if(!QFile::exists(logfile)){ 
	  if(!QFile::exists("/var/log/lpreserver")){ system( "mkdir /var/log/lpreserver"); }
	  system( QString("touch "+logfile).toUtf8() ); 
	}
	watcher->addPath(logfile);
  logFile = new QFile(logfile);
	logFile->open(QIODevice::ReadOnly | QIODevice::Text); //open it now, for faster reading
  LFStream = new QTextStream(logFile);
  	connect(watcher, SIGNAL(fileChanged(QString)),this,SLOT(slotNewLogMessage(QString)) ); //now connect the signal/slot
  //Setup the context menu
  menu = new QMenu;
	menu->addAction(new QAction(QIcon(":/images/application-exit.png"),tr("Close Life Preserver Tray"),this) );
	connect(menu, SIGNAL(triggered(QAction*)), this, SLOT(slotClose()) );
  this->setContextMenu(menu);
  //Setup the animated icon timer
  timer = new QTimer();
	timer->setInterval(100);
	connect(timer, SIGNAL(timeout()), this, SLOT(displayWorkingIcon()) );
  //Setup initial icon for the tray
  this->setIcon( QIcon(":/images/tray-icon-idle.png") );
  //Create the configuration GUI
  GUI = new mainUI();
  //connect other signals/slots
  connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(slotTrayClicked(QSystemTrayIcon::ActivationReason)) );
  //Make sure we check the latest line in the logfile
  QTimer::singleShot(1000, this,SLOT(firstCheck()));
}

LPTray::~LPTray(){
  if(statFile != 0){ statFile->close(); }
  logFile->close();
  delete statFile;
  delete logFile;
  delete watcher;
  delete menu;
  delete timer;
}

// ===============
//  PRIVATE FUNCTIONS
// ===============
void LPTray::parseLogMessage(QString log){
  //Divide up the log into it's sections
  QString timestamp = log.section(":",0,2).simplified();
  QString time = timestamp.section(" ",3,3).simplified();
  QString message = log.section(":",3,3).toLower().simplified();
  QString dev = log.section(":",4,4).simplified(); //dataset/snapshot/nothing
  //Now decide what to do/show because of the log message
  qDebug() << "New Log Message:" << log;
  if(message.contains("creating snapshot")){
    dev = message.section(" ",-1).simplified();
    this->showMessage( time, QString(tr("Creating snapshot for %1")).arg(dev), QSystemTrayIcon::Information, 5000);
    //Just set the standard idle icon
    this->setIcon( QIcon(":/images/tray-icon-idle.png") );   
//  }else if(message.contains("pruning snapshot")){
  }else if(message.contains("starting replication")){
    startWorkingIcon();
    //Setup the file watcher for this new log file
    sFile = dev;
    if(!sFile.isEmpty()){
      if(!QFile::exists(sFile)){ system( QString("touch "+sFile).toUtf8() ); }
      statFile = new QFile(sFile);
        statFile->open(QIODevice::ReadOnly | QIODevice::Text);
        SFStream = new QTextStream(statFile);
      watcher->addPath(sFile); //will update/set tooltips
    }
  }else if(message.contains("finished replication")){
    stopWorkingIcon();
    //Stop the file wather from watching the status file and clean up
    if(!sFile.isEmpty()){
      watcher->removePath(sFile);
	  statFile->close();
	  sFile.clear();
    }
    //Clean up and show messages
    repTotK.clear();
    this->setToolTip("");
    dev = message.section(" ",-1).simplified();
    this->showMessage( time, QString(tr("Finished replication for %1")).arg(dev), QSystemTrayIcon::Information, 5000);
  }else if( message.contains("FAILED replication") ){
    stopWorkingIcon();
    //Stop the file wather from watching the status file and clean up
    if(!sFile.isEmpty()){
      watcher->removePath(sFile);
	  statFile->close();
	  sFile.clear();
    }
    //Clean up and show messages
    repTotK.clear();
    dev = message.section(" ",-1).simplified();
    QString file = log.section("LOGFILE:",1,1).simplified();
    QString tt = QString(tr("%1: Replication Failed on %2")).arg(time,dev) +"\n"+ QString(tr("Logfile available at: %1")).arg(file);
    this->setToolTip(tt);   
    this->showMessage( time, QString(tr("Replication Error for %1")).arg(dev), QSystemTrayIcon::Information, 5000);
    this->setIcon(QIcon(":/images/tray-icon-failed.png"));
  }else{
    //Just set the standard idle icon
    //this->setIcon( QIcon(":/images/tray-icon-idle.png") );    
  }
  if(GUI->isVisible()){
    GUI->updateDisplay();
  }
}

void LPTray::parseStatusMessage(QString stat){
  qDebug() << "New Status Message:" << stat;
  //Divide up the status message into sections
  stat.replace("\t"," ");
  QString dataset = stat.section(" ",2,2,QString::SectionSkipEmpty).section("/",0,0).simplified();
  QString cSize = stat.section(" ",1,1,QString::SectionSkipEmpty);
  //Now Setup the tooltip
  if(cSize != lastSize){ //don't update the tooltip if the same size info
    QString percent;
    if(!repTotK.isEmpty()){
      //calculate the percentage
      double tot = displayToDoubleK(repTotK);
      double c = displayToDoubleK(cSize);
      if( tot!=-1 & c!=-1){
        double p = (c*100)/tot;
	p = int(p*10)/10.0; //round to 1 decimel places
	percent = QString::number(p) + "%";
      }
    }
    if(repTotK.isEmpty()){ repTotK = "??"; }
    //Format the tooltip String
    QString status = cSize+"/"+repTotK;
    if(!percent.isEmpty()){ status.append(" ("+percent+")"); }
    QString txt = QString(tr("Replicating %1: %2")).arg(dataset, status);
    this->setToolTip(txt);
    lastSize = cSize; //save the current size for later
  }
}

void LPTray::startWorkingIcon(){
  this->setIcon( QIcon(":/images/tray-icon-active7.png"));
  //wNum = 1; //start on the first image
  //timer->start();
}

void LPTray::stopWorkingIcon(){
  //timer->stop();
  this->setIcon( QIcon(":/images/tray-icon-idle.png") );      
}

double LPTray::displayToDoubleK(QString displayNumber){
  QStringList labels; 
    labels << "K" << "M" << "G" << "T" << "P" << "E";
  QString clab = displayNumber.right(1); //last character is the size label
	displayNumber.chop(1); //remove the label from the number
  double num = displayNumber.toDouble();
  //Now format the number properly
  bool ok = false;
  for(int i=0; i<labels.length(); i++){
    if(labels[i] == clab){ ok = true; break; }
    else{ num = num*1024; } //get ready for the next size
  }
  if(!ok){ num = -1; } //could not determine the size
  return num;
}

// ===============
//     PRIVATE SLOTS
// ===============
void LPTray::firstCheck(){
  slotNewLogMessage("/var/log/lpreserver/lpreserver.log");
}

void LPTray::slotNewLogMessage(QString file){
  //qDebug() << "New Log Message in file:" << file;
  if(file == "/var/log/lpreserver/lpreserver.log"){
    //Backend Status Update
    //get the last line from the log file
    QString log;
    while( !LFStream->atEnd() ){ log = LFStream->readLine(); }
    //Now parse the log line and do stuff with it
    parseLogMessage(log);
  }else{
    //Replication status update
      //get the last line from the file
      QString stat;
      while( !SFStream->atEnd() ){ 
        QString line = SFStream->readLine(); 
	if(line.contains("total estimated size")){ repTotK = line.section(" ",-1).simplified(); } //save the total size to replicate
	else if(line.startsWith("send from ")){}
	else if(line.startsWith("TIME ")){}
	else{ stat = line; } //only save the relevant status line
      }
      //parse the status line
      if(!stat.isEmpty()){ parseStatusMessage(stat); }
  }
}

void LPTray::slotTrayClicked(QSystemTrayIcon::ActivationReason reason){
  if(reason == QSystemTrayIcon::Trigger){ 
    if(GUI->isVisible()){ GUI->hide(); }
    else{ startGUI(); }
  }else if( reason == QSystemTrayIcon::Context){
    this->contextMenu()->popup(QCursor::pos());
  }
}

void LPTray::slotClose(){
  exit(0);
}

void LPTray::slotSingleInstance(){
  this->show();
}

void LPTray::startGUI(){
  //Start up the GUI
    GUI->setupUI();
    GUI->raise();
    GUI->show();
}

void LPTray::displayWorkingIcon(){
  QString ico = ":/images/tray-icon-active"+QString::number(wNum)+".png";
  this->setIcon(QIcon(ico));
  if(wNum == 16){ wNum = 1; } //go back to the beginning of the loop
  else{ wNum++; }
}
