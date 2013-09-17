#include "LPWatcher.h"

/* ------ HASH NUMBERING NOTE -----
  Each set of 10 is a different type of status
    "message" status: 10-19
    "replication" status: 20-29
    "critical" status: 30-39
    "mirror" status: 40-49
    "resilver" status: 50-59
    "scrub" status: 60-69
  Within each set:
    *0 = ID Code (for internal identification as necessary)
    *1 = zpool (example: tank1)
    *2 = summary (shortened version of the message - tooltips)
    *3 = message (full message)
    *4 = timestamp (full date/time timestamp in readable format)
    *5 = short timestamp (just time in readable format)

  Valid Internal ID's:
    SNAPCREATED -> new snapshot created
    STARTED    	-> Task started
    RUNNING  	-> Task running (I.E. status update)
    FINISHED  	-> Task finished
    ERROR       	-> Task failed
    
*/

LPWatcher::LPWatcher() : QObject(){
  //Initialize the path variables
  FILE_LOG = "/var/log/lpreserver/lpreserver.log";
  FILE_ERROR="/var/log/lpreserver/error.log";
  FILE_REPLICATION=""; //this is set automatically based on the log file outputs
  sysCheckTime = 300000; // 5 minutes
  //initialize the watcher and timer
  watcher = new QFileSystemWatcher(this);
    connect(watcher, SIGNAL(fileChanged(QString)),this,SLOT(fileChanged(QString)) );
  timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(checkPoolStatus()) );
  //initialize the log file reader
  logfile = new QFile(FILE_LOG, this);
  LFSTREAM = new QTextStream(logfile);
  //initialize the replication file reader
  repfile = new QFile(this);
}

LPWatcher::~LPWatcher(){
  //clean up the internal objects
  delete watcher;
  delete timer;
  delete logfile;
  delete LFSTREAM;
}

// -----------------------------------
//    PUBLIC FUNCTIONS
// -----------------------------------
void LPWatcher::start(){
  if(!logfile->exists()){
    QString dir = FILE_LOG;
	  dir.chop( dir.section("/",-1).count()+1 );
    if(!QFile::exists(dir)){ system( QString("mkdir -p "+dir).toUtf8() ); }
    system( QString("touch "+FILE_LOG).toUtf8() );
  }
  //Read the current state of the log file
  logfile->open(QIODevice::ReadOnly | QIODevice::Text);
  readLogFile(true); //do this quietly the first time through
  //Now start up the log file watcher
  watcher->addPath(FILE_LOG);
  //Now check for any current errors in the LPbackend
  checkPoolStatus();
  //And start up the error file watcher
  if(!timer->isActive()){ timer->start(sysCheckTime); }
}

void LPWatcher::stop(){
  watcher->removePaths(watcher->files());
  logfile->close();
  timer->stop();
}

QStringList LPWatcher::getMessages(QString type, QStringList msgList){
  QStringList output;
  type = type.toLower();
  //Valid types - "critical"/"running"/"message"
  //Valid messages - "dataset","message","summary","id", "timestamp", "time"
  unsigned int base;
  if(type=="message"){base=10;}
  else if(type=="replicate"){base=20;}
  else if(type=="critical"){base=30;}
  else if(type=="mirror"){base=40;}
  else if(type=="resilver"){base=50;}
  else if(type=="scrub"){base=60;}
  else{ return output; } //invalid input type
  //Now fill the output array based upon requested outputs
  for(int i=0; i<msgList.length(); i++){
    msgList[i] = msgList[i].toLower();
    if(msgList[i]=="id" && LOGS.contains(base)){ output << LOGS[base]; }
    else if(msgList[i]=="device" && LOGS.contains(base+1)){ output << LOGS[base+1]; }
    else if(msgList[i]=="summary" && LOGS.contains(base+2)){ output << LOGS[base+2]; }
    else if(msgList[i]=="message" && LOGS.contains(base+3)){ output << LOGS[base+3]; }
    else if(msgList[i]=="timestamp" && LOGS.contains(base+4)){ output << LOGS[base+4]; }
    else if(msgList[i]=="time" && LOGS.contains(base+5)){ output << LOGS[base+5]; }
    else{ output << ""; }
  }
  //Return the output list
  return output;
}

QStringList LPWatcher::getAllCurrentMessages(){
  //Useful for quickly displaying all latest messages in a tooltip or summary
  QStringList output;
  if(LOGS.contains(12) && LOGS.contains(14)){ output << LOGS[14]+" -- "+LOGS[12]; }
  if(LOGS.contains(22) && LOGS.contains(24)){ output << LOGS[24]+" -- "+LOGS[22]; }
  if(LOGS.contains(32) && LOGS.contains(34)){ output << LOGS[34]+" -- "+LOGS[32]; }
  if(LOGS.contains(42) && LOGS.contains(44)){ output << LOGS[44]+" -- "+LOGS[42]; }
  if(LOGS.contains(52) && LOGS.contains(54)){ output << LOGS[54]+" -- "+LOGS[52]; }
  if(LOGS.contains(62) && LOGS.contains(64)){ output << LOGS[64]+" -- "+LOGS[62]; }
  return output;
}

bool LPWatcher::isRunning(){
  if(LOGS.value(20) == "STARTED" || LOGS.value(20) == "RUNNING"){ return true; }
  else if(LOGS.value(40) == "STARTED" || LOGS.value(40) == "RUNNING"){ return true; }
  else if(LOGS.value(50) == "STARTED" || LOGS.value(50) == "RUNNING"){ return true; }
  else if(LOGS.value(60) == "STARTED" || LOGS.value(60) == "RUNNING"){ return true; }
  else{ return false; }
}

bool LPWatcher::hasError(){
  return (LOGS.value(20)=="ERROR" || LOGS.contains(30) || LOGS.value(40)=="ERROR" || LOGS.value(50)=="ERROR" || LOGS.value(60)=="ERROR");
}

// -------------------------------------
//    PRIVATE FUNCTIONS
// -------------------------------------
void LPWatcher::readLogFile(bool quiet){
  QTextStream in(logfile);
  while(!LFSTREAM->atEnd()){
    QString log = LFSTREAM->readLine();
    //Divide up the log into it's sections
    QString timestamp = log.section(":",0,2).simplified();
    QString time = timestamp.section(" ",3,3).simplified();
    QString message = log.section(":",3,3).toLower().simplified();
    QString dev = log.section(":",4,4).simplified(); //dataset/snapshot/nothing
    //Now decide what to do/show because of the log message
    //qDebug() << "New Log Message:" << log;
    if(message.contains("creating snapshot")){
      dev = message.section(" ",-1).simplified();
      //Setup the status of the message
      LOGS.insert(10,"SNAPCREATED");
      LOGS.insert(11,dev); //dataset
      LOGS.insert(12, QString(tr("New snapshot of %1")).arg(dev) ); //summary
      LOGS.insert(13, QString(tr("Creating snapshot for %1")).arg(dev) );
      LOGS.insert(14, timestamp); //full timestamp
      LOGS.insert(15, time); // time only
      if(!quiet){ emit MessageAvailable("message"); }
    }else if(message.contains("starting replication")){
      //Setup the file watcher for this new log file
      FILE_REPLICATION = dev;
      startRepFileWatcher();
      //Set the appropriate status variables
      LOGS.insert(20,"REPSTARTED");
      // 21 - dataset set on update ping
      LOGS.insert(22, tr("Replication Started") ); //summary
      // 23 - Full message set on update ping
      LOGS.insert(24, timestamp); //full timestamp
      LOGS.insert(25, time); // time only
      if(!quiet){ emit MessageAvailable("replication"); }
    }else if(message.contains("finished replication")){
      stopRepFileWatcher();
      dev = message.section(" ",-1).simplified();
      //Now set the status of the process
      LOGS.insert(20,"REPFINISHED");
      LOGS.insert(21,dev); //dataset
      LOGS.insert(22, tr("Finished Replication") ); //summary
      LOGS.insert(23, QString(tr("Finished replication for %1")).arg(dev) );
      LOGS.insert(24, timestamp); //full timestamp
      LOGS.insert(25, time); // time only      
      if(!quiet){ emit MessageAvailable("replication"); }
    }else if( message.contains("FAILED replication") ){
      stopRepFileWatcher();
      //Now set the status of the process
      dev = message.section(" ",-1).simplified();
      QString file = log.section("LOGFILE:",1,1).simplified();
      QString tt = QString(tr("Replication Failed for %1")).arg(dev) +"\n"+ QString(tr("Logfile available at: %1")).arg(file);
      LOGS.insert(20,"REPERROR");
      LOGS.insert(21,dev); //dataset
      LOGS.insert(22, tr("Replication Failed") ); //summary
      LOGS.insert(23, tt );
      LOGS.insert(24, timestamp); //full timestamp
      LOGS.insert(25, time); // time only      
      if(!quiet){ emit MessageAvailable("replication"); }
    }
	  
  }
}

void LPWatcher::readReplicationFile(bool quiet){
  QString stat;
  while( !RFSTREAM->atEnd() ){ 
    QString line = RFSTREAM->readLine(); 
    if(line.contains("total estimated size")){ repTotK = line.section(" ",-1).simplified(); } //save the total size to replicate
    else if(line.startsWith("send from ")){}
    else if(line.startsWith("TIME ")){}
    else{ stat = line; } //only save the relevant/latest status line
  }
  if(stat.isEmpty()){
    qDebug() << "New Status Message:" << stat;
    //Divide up the status message into sections
    stat.replace("\t"," ");
    QString dataset = stat.section(" ",2,2,QString::SectionSkipEmpty).section("/",0,0).simplified();
    QString cSize = stat.section(" ",1,1,QString::SectionSkipEmpty);
    //Now Setup the tooltip
    if(cSize != lastSize){ //don't update the info if the same size info
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
      //Format the info string
      QString status = cSize+"/"+repTotK;
      if(!percent.isEmpty()){ status.append(" ("+percent+")"); }
      QString txt = QString(tr("Replicating %1: %2")).arg(dataset, status);
      lastSize = cSize; //save the current size for later
      //Now set the current process status
      LOGS.insert(20,"RUNNING");
      LOGS.insert(21,dataset);
      LOGS.insert(23,txt);
      if(!quiet){ emit MessageAvailable("replication"); }
    }
  }
}

void LPWatcher::startRepFileWatcher(){
  if(FILE_REPLICATION.isEmpty()){ return; }
  if(!QFile::exists(FILE_REPLICATION)){ system( QString("touch "+FILE_REPLICATION).toUtf8() ); }
  repfile->setFileName(FILE_REPLICATION);
  repfile->open(QIODevice::ReadOnly | QIODevice::Text);
  RFSTREAM = new QTextStream(repfile);
  watcher->addPath(FILE_REPLICATION);
}

void LPWatcher::stopRepFileWatcher(){
  if(FILE_REPLICATION.isEmpty()){ return; }
  watcher->removePath(FILE_REPLICATION);
  repfile->close();
  delete RFSTREAM;
  FILE_REPLICATION.clear();
  repTotK.clear();
  lastSize.clear();
}

double LPWatcher::displayToDoubleK(QString displayNumber){
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

// ------------------------------
//    PRIVATE SLOTS
// ------------------------------
void LPWatcher::fileChanged(QString file){
  if(file == FILE_LOG){ readLogFile(); }
  else  if(file == FILE_REPLICATION){ readReplicationFile(); }
}

void LPWatcher::checkPoolStatus(){
  //Now check zpool status for bad/running statuses
  QStringList zstat = LPBackend::getCmdOutput("zpool status");
    //parse the output
    QString pool, state, timestamp;
    QStringList cDev, cStat, cMsg, cSummary;
    //qDebug() << "-----zpool status------";
    bool newresilver = false; bool newscrub = false; bool newerror = false;
    for(int i=0; i<zstat.length(); i++){
      zstat[i] = zstat[i].simplified();
      if(zstat[i].isEmpty()){ continue; }
      //qDebug() << zstat[i];
      if(zstat[i].startsWith("pool:")){ pool = zstat[i].section(":",1,10).simplified(); }
      else if(zstat[i].startsWith("state:")){ state = zstat[i].section(":",1,10).simplified(); }
      else if(zstat[i].startsWith("scan:")){
	//check for scrubs/resilvering progress
	// ------ SCRUB ------
	if(zstat[i].contains("scrub")){
	  if(zstat[i].contains(" scrub repaired ")){
	    //Scrub Finished
	    zstat[i]  = zstat[i].replace("\t"," ").simplified();
	    timestamp = zstat[i].section(" ",10,14,QString::SectionSkipEmpty);
	    QString numFixed = zstat[i].section(" ",3,3,QString::SectionSkipEmpty);
	    QString numErr = zstat[i].section(" ",7,7,QString::SectionSkipEmpty);
	    QString timeRun = zstat[i].section(" ",5,5,QString::SectionSkipEmpty);
	    //Scrub finished previously
	    if(numFixed.toInt() > 0){ 
	      if(LOGS.value(60)!="ERROR"){ newscrub=true; }
	      LOGS.insert(60, "ERROR"); 
	      LOGS.insert(62, QString(tr("Scrub repaired %1 bad blocks")).arg(numFixed) );
	      LOGS.insert(63, QString(tr("Scrub repaired %1 blocks in %2 with %3 errors")).arg(numFixed, timeRun, numErr) );
	    }else{ 
	      if(LOGS.contains(60) && LOGS.value(60)!="FINISHED"){ newscrub=true; }
	      LOGS.insert(60,"FINISHED"); 
	      LOGS.insert(62, tr("Scrub completed") );
	      LOGS.insert(63, tr("Scrub completed without needing repairs") );
	    }
	    LOGS.insert(61,pool);
	    LOGS.insert(64, timestamp);
	    LOGS.insert(65, timestamp.section(" ",3,3) );
	    if(timer->interval() != sysCheckTime){ timer->start(sysCheckTime); }
	  }else{
	    //Scrub is running - parse the line
	    timestamp = zstat[i].section(" ",5,9,QString::SectionSkipEmpty);
	    i++; QString remain = zstat[i].section(" ",7,7,QString::SectionSkipEmpty);
	    i++; QString percent = zstat[i].section(" ",2,2,QString::SectionSkipEmpty);
	    if(LOGS.value(60) != "RUNNING"){newscrub=true;}
	    LOGS.insert(60,"RUNNING");
	    LOGS.insert(61,pool);
	    LOGS.insert(62, QString(tr("Scrubbing %1: %2 (%3 remaining)")).arg(pool, percent, remain) );
	    LOGS.insert(63, QString(tr("Scrubbing %1: %2 (%3 remaining)")).arg(pool, percent, remain) );
	    LOGS.insert(64, timestamp);
	    LOGS.insert(65, timestamp.section(" ",3,3) );
	    if(timer->interval() != 60000){ timer->start(60000); } //put the timer on a 1 minute refresh since it is running
	  }
	  if(LOGS.contains(50) ){
	    //Only resilvering OR scrub is shown at a time - so remove the resilver info
	    LOGS.remove(50);
	    LOGS.remove(51);
	    LOGS.remove(52);
	    LOGS.remove(53);
	    LOGS.remove(54);
	    LOGS.remove(55);
	  }
	// --------- RESILVERING -------
	}else if(zstat[i].contains("resilver in progress")){
	  //Resilvering is currently running
	  timestamp = zstat[i].section(" ",5,9,QString::SectionSkipEmpty);
	  //need info from the next two lines
	  i++; QString timeleft = zstat[i].section(" ",7,7,QString::SectionSkipEmpty);
	  i++; QString percent = zstat[i].section(" ", 2,2,QString::SectionSkipEmpty);
	  //Setup the running re-silvering progress
	  if(LOGS.value(50)!="RUNNING"){newresilver=true; }
	  LOGS.insert(50, "RUNNING");
	  // 51 - need to put the actual device in here (not available on this line)
	  LOGS.insert(52, QString(tr("Resilvering: %1 (%2 remaining)")).arg(percent, timeleft) );
	  if(newresilver){ LOGS.insert(53, QString(tr("Resilvering Started: %1 remaining ")).arg( timeleft) ); }
	  else{ LOGS.insert(53,QString(tr("Resilvering: %1 (%2 remaining)")).arg(percent, timeleft) ); }
	  LOGS.insert(54, timestamp);
	  LOGS.insert(55, timestamp.section(" ",3,3) );
	  if(LOGS.contains(60) ){
	    //Only resilvering OR scrub is shown at a time - so remove the scrub info
	    LOGS.remove(60);
	    LOGS.remove(61);
	    LOGS.remove(62);
	    LOGS.remove(63);
	    LOGS.remove(64);
	    LOGS.remove(65);
	  }
	  if(timer->interval() != 60000){ timer->start(60000); }//put the timer on a 1 minute refresh since it is running
	}else if(zstat[i].contains("resilvered")){
	  //Resilvering is finished
	  timestamp = zstat[i].section(" ",9,13,QString::SectionSkipEmpty);
	  QString timecomplete = zstat[i].section(" ",4,4,QString::SectionSkipEmpty);
	  QString errors = zstat[i].section(" ", 6,6,QString::SectionSkipEmpty);
	  //Setup the running re-silvering progress
	  if(LOGS.contains(50)){newresilver=true; } //don't display message for first run
	  if(errors.toInt() > 0){ 
	    LOGS.insert(50, "ERROR");
	    LOGS.insert(52, QString(tr("Resilver completed in &1 with %2 errors")).arg(timecomplete, errors) );
	    LOGS.insert(53, QString(tr("Resilver completed in &1 with %2 errors")).arg(timecomplete, errors) );
	  }else{
	    LOGS.insert(50, "FINISHED");
	    LOGS.insert(52, QString(tr("Resilver completed successfully in &1")).arg(timecomplete) );
	    LOGS.insert(53, QString(tr("Resilver completed successfully in &1")).arg(timecomplete) ); 
          }
	  // 51 - need to put the actual device in here (not available on this line)
	  LOGS.insert(54, timestamp);
	  LOGS.insert(55, timestamp.section(" ",3,3) );
	  if(LOGS.contains(60) ){
	    //Only resilvering OR scrub is shown at a time - so remove the scrub info
	    LOGS.remove(60);
	    LOGS.remove(61);
	    LOGS.remove(62);
	    LOGS.remove(63);
	    LOGS.remove(64);
	    LOGS.remove(65);
	  }
	  if(timer->interval() != sysCheckTime){ timer->start(sysCheckTime); }
	}
      }else if(zstat[i].startsWith("errors:")){
	if(zstat[i] != "errors: No known data errors"){
	  qDebug() << "New zpool status error line that needs parsing:" << zstat[i];
	}
      }else if( state != "ONLINE" || !LOGS.value(50).isEmpty() ){
        //Check for state/resilvering of all real devices
	QString msg, summary, status;
	QString device = zstat[i].section(" ",0,0,QString::SectionSkipEmpty);
	if(zstat[i].contains("NAME STATE READ")){continue;} //nothing on this header line
	else if(zstat[i].contains("(resilvering)")){ LOGS.insert(51, device ); continue;}
	else if(zstat[i].contains("ONLINE")){continue;} //do nothing for this device - it is good
	else if(zstat[i].contains("OFFLINE")){ continue; } //do nothing - this status must be set manually - it is not a "random" status
        else if(zstat[i].contains("DEGRADED")){ 
	  cStat << "DEGRADED";
	  cMsg << tr("The device is in a degraded state, and should be replaced soon.");
	  cSummary << QString(tr("%1 is degraded.")).arg(device);
	  cDev << device;
	}else if(zstat[i].contains("FAULTED")){ 
	  cStat << "FAULTED";
	  cMsg << tr("The device is faulty, and should be replaced.");
	  cSummary << QString(tr("%1 is faulty.")).arg(device);
	  cDev << device;
	}else if(zstat[i].contains("REMOVED")){ 
	  cStat << "REMOVED";
	  cMsg << tr("The device was removed, and should be either be re-attached or replaced.");
	  cSummary << QString(tr("%1 was removed.")).arg(device);
	  cDev << device;
	}else if(zstat[i].contains("UNAVAIL")){ 
	  cStat << "UNAVAILABLE";
	  cMsg << tr("The device is unavailable and should be re-added to the pool.");
	  cSummary << QString(tr("%1 is unavailable.")).arg(device);
	  cDev << device;
	}
      }
    } //end of loop over zpool status lines
    
  //Add the critical messages to the hash
  if(cStat.isEmpty()){
    if(LOGS.contains(30)){
      LOGS.remove(30);
      LOGS.remove(31);
      LOGS.remove(32);
      LOGS.remove(33);
      LOGS.remove(34);
      LOGS.remove(35);
    }
  }else{
    if(LOGS.value(30) != cStat.join(":::") ){ newerror = true; }
    LOGS.insert(30, cStat.join(":::") );
    LOGS.insert(31, cDev.join(":::") );
    LOGS.insert(32, cSummary.join(":::") );
    LOGS.insert(33, cMsg.join(":::") );
    LOGS.insert(34, timestamp);
    LOGS.insert(35, timestamp.section(" ",3,3) );
  }
    
  //Now emit the appropriate signal
  if(newerror){ emit MessageAvailable("critical"); }
  else if(newresilver){ emit MessageAvailable("resilver"); }
  else if(newscrub){ emit MessageAvailable("scrub"); }
  else{ emit StatusUpdated(); }
}
