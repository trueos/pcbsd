#include "LPBackend.h"

// ==============
//     Informational
// ==============
QStringList LPBackend::listPossibleDatasets(){
  QString cmd = "zpool list -H -o name";
  //Need output, so run this in a QProcess
  QProcess *proc = new QProcess;
  proc->setProcessChannelMode(QProcess::MergedChannels);
  proc->start(cmd);
  proc->waitForFinished();
  QStringList out = QString(proc->readAllStandardOutput()).split("\n");	
  delete proc;
  //Now process the output (one dataset per line - no headers)
  QStringList list;
  for(int i=0; i<out.length(); i++){
    QString ds = out[i].section("/",0,0).simplified();
    if(!ds.isEmpty()){ list << ds; }
  }
  list.removeDuplicates();
  return list;	
}

QStringList LPBackend::listDatasets(){
  QString cmd = "lpreserver listcron";
  //Need output, so run this in a QProcess
  QProcess *proc = new QProcess;
  proc->setProcessChannelMode(QProcess::MergedChannels);
  proc->start(cmd);
  proc->waitForFinished();
  QStringList out = QString(proc->readAllStandardOutput()).split("\n");	
  delete proc;
  //Now process the output
  QStringList list;
  for(int i=2; i<out.length(); i++){ //skip the first two lines (headers)
    QString ds = out[i].section(" - ",0,0).simplified();
    if(!ds.isEmpty()){ list << ds; }
  }
  return list;
}

QStringList LPBackend::listDatasetSubsets(QString dataset){
  QString cmd = "zfs list -H -t filesystem -o name,mountpoint,mounted";
  //Need output, so run this in a QProcess
  QProcess *proc = new QProcess;
  proc->setProcessChannelMode(QProcess::MergedChannels);
  proc->start(cmd);
  proc->waitForFinished();
  QStringList out = QString(proc->readAllStandardOutput()).split("\n");	
  delete proc;
  //Now process the output (one dataset per line - no headers)
  QStringList list;
  for(int i=0; i<out.length(); i++){
    if(out[i].startsWith(dataset+"/")){
      if(out[i].section("\t",2,2,QString::SectionSkipEmpty).simplified() == "yes"){
        QString ds = out[i].section("\t",1,1).simplified(); //save the mountpoint
        if(!ds.isEmpty()){ list << ds; }
      }
    }
  }
  list.removeDuplicates();	
  return list;
}

QStringList LPBackend::listSnapshots(QString dsmountpoint){
  //List all the snapshots available for the given dataset mountpoint
  QDir dir(dsmountpoint+"/.zfs/snapshot");
  QStringList list;
  if(dir.exists()){
    list = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name | QDir::Reversed);
  }
  return list;
}

QStringList LPBackend::listLPSnapshots(QString dataset){
  QString cmd = "lpreserver listsnap "+dataset;
  //Need output, so run this in a QProcess
  QProcess *proc = new QProcess;
  proc->setProcessChannelMode(QProcess::MergedChannels);
  proc->start(cmd);
  proc->waitForFinished();
  QStringList out = QString(proc->readAllStandardOutput()).split("\n");	
  delete proc;
  //Now process the output
  QStringList list;
  for(int i=out.length()-1; i>=0; i--){ //go in reverse order for proper time format (newest first)
    if(out[i].startsWith(dataset+"@")){
      QString snap = out[i].section("@",1,3).section(" ",0,0).simplified();;
      if(!snap.isEmpty()){ list << snap; }
    }
  }
  return list;	
}

QStringList LPBackend::listReplicationTargets(){
  QString cmd = "lpreserver replicate list";
  //Need output, so run this in a QProcess
  QProcess *proc = new QProcess;
  proc->setProcessChannelMode(QProcess::MergedChannels);
  proc->start(cmd);
  proc->waitForFinished();
  QStringList out = QString(proc->readAllStandardOutput()).split("\n");	
  delete proc;
  //Now process the output
  QStringList list;
  for(int i=0; i<out.length(); i++){
    if(out[i].contains("->")){
      QString ds = out[i].section("->",0,0).simplified();
      if(!ds.isEmpty()){ list << ds; }
    }
  }
  return list;		
}

QStringList LPBackend::listCurrentStatus(){
  QString cmd = "lpreserver status";
  //Need output, so run this in a QProcess
  QProcess *proc = new QProcess;
  proc->setProcessChannelMode(QProcess::MergedChannels);
  proc->start(cmd);
  proc->waitForFinished();
  QStringList out = QString(proc->readAllStandardOutput()).split("\n");	
  delete proc;
  QStringList list;
  //Now process the output	
  for(int i=2; i<out.length(); i++){ //first 2 lines are headers
    //Format: <dataset>:::<lastsnapshot | NONE>:::<lastreplication | NONE>
    QString ds  = out[i].section(" - ",0,0).simplified();
    QString snap = out[i].section(" - ",1,1).simplified();
    QString rep = out[i].section(" - ",2,2).simplified();
    if(snap == "NONE"){ snap = "-"; }
    if(rep == "NONE"){ rep = "-"; }
    list << ds +":::"+ snap+":::"+rep;
  }
  return list;
}

// ==================
//    Dataset Management
// ==================
bool LPBackend::setupDataset(QString dataset, int time, int numToKeep){
  //Configure inputs
  QString freq;
  if(time == -30){ freq = "30min"; }
  else if(time == -10){ freq = "10min"; }
  else if(time == -5){ freq = "5min"; }
  else if(time >= 0 && time < 24){ freq = "daily@"+QString::number(time); }
  else{ freq = "hourly"; }
  
  //Create the command
  QString cmd = "lpreserver cronsnap "+dataset+" start "+freq+" "+QString::number(numToKeep);
  int ret = system(cmd.toUtf8());
  return (ret == 0);
}

bool LPBackend::removeDataset(QString dataset){
  QString cmd = "lpreserver cronsnap "+dataset+" stop";
  int ret = system(cmd.toUtf8());	
  return (ret == 0);
}

bool LPBackend::datasetInfo(QString dataset, int& time, int& numToKeep){
  QString cmd = "lpreserver listcron";
  //Need output, so run this in a QProcess
  QProcess *proc = new QProcess;
  proc->setProcessChannelMode(QProcess::MergedChannels);
  proc->start(cmd);
  proc->waitForFinished();
  QStringList out = QString(proc->readAllStandardOutput()).split("\n");	
  delete proc;
  //Now process the output
  bool ok = false;
  for(int i=0; i<out.length(); i++){
    if(out[i].section(" - ",0,0).simplified() == dataset){
      //Get time schedule (in integer format)
      QString sch = out[i].section(" - ",1,1).simplified();
      if(sch.startsWith("daily@")){ time = sch.section("@",1,1).simplified().toInt(); }
      else if(sch=="5min"){time = -5;}
      else if(sch=="10min"){time = -10;}
      else if(sch=="30min"){time = -30;}
      else{ time = -60; } //hourly
      //Get total snapshots
      numToKeep = out[i].section("- total:",1,1).simplified().toInt();
      ok=true;
      break;
    }
  }
  //qDebug() << "lpreserver cronsnap:\n" << out << QString::number(time) << QString::number(numToKeep);
  return ok;
}

// ==================
//    Snapshop Management
// ==================
bool LPBackend::newSnapshot(QString dataset){
  QString cmd = "lpreserver mksnap "+dataset;
  int ret = system(cmd.toUtf8());
  return (ret == 0);
}

bool LPBackend::removeSnapshot(QString dataset, QString snapshot){
  QString cmd = "lpreserver rmsnap "+dataset +" "+snapshot;
  int ret = system(cmd.toUtf8());	
  return (ret == 0);
}

bool LPBackend::revertSnapshot(QString dataset, QString snapshot){
  QString cmd = "lpreserver revertsnap "+dataset +" "+snapshot;
  int ret  = system(cmd.toUtf8());
  return (ret == 0);
}

QString LPBackend::revertSnapshotFile(QString dsmountpoint, QString snapshot, QString filepath){
  //Copy the given file from the snapshot back into the main dataset
  // -- filepath: full path to the file in the snapshot directory
  
  //Check that the file path is complete and the file exists
  if(!QFile::exists(filepath)){
    //invalid file given
    return "";
  }
  //Generate the new file path
  QString newfilepath = filepath.replace(dsmountpoint+"/.zfs/snapshot/"+snapshot, dsmountpoint);	
  if( QFile::exists(newfilepath) ){
    //get the file extension
    QString filename = newfilepath.section("/",-1);
    QString ext = filename.section(".",-1);
    if( !ext.isEmpty() && !filename.startsWith("."+ext) && ext!=filename){
      newfilepath.chop(ext.length()+1);
      newfilepath.append("-reversion."+ext);
      int i=1;
      //append a number to the end if a reversion file already exists
      while(QFile::exists(newfilepath)){
        newfilepath.chop(ext.length()+1);
        newfilepath.append(QString::number(i)+"."+ext);
      i  ++;
      }
    }else{
      //File without an extension - just append a number
      newfilepath.append("-reversion");
      int i=1; 
      QString npath = newfilepath;
      while(QFile::exists(npath)){
        npath = newfilepath.append(QString::number(i));
	i++;
      }
      newfilepath = npath;
    }
  }
  //perform the copy
  bool ok = QFile::copy(filepath,newfilepath);
  //return the path to the new file if the copy was successful
  if(ok){ 
    //reset the permissions on the reverted file to match the original
    QFile::setPermissions(newfilepath, QFile::permissions(filepath));
    return newfilepath;
  }else{ return ""; }
}

// ==================
//    Replication Management
// ==================
bool LPBackend::setupReplication(QString dataset, QString remotehost, QString user, int port, QString remotedataset, int time){
  QString stime = "sync"; //synchronize on snapshot creation (default)
  if(time >= 0 || time < 24){ stime = QString::number(time); } //daily at a particular hour (24 hour notation)
  
  QString cmd = "lpreserver replicate add "+remotehost+" "+user+" "+ QString::number(port)+" "+dataset+" "+remotedataset+" "+stime;
  int ret = system(cmd.toUtf8());
  return (ret == 0);
}

bool LPBackend::removeReplication(QString dataset){
  QString cmd = "lpreserver replicate remove "+dataset;
  int ret = system(cmd.toUtf8());	
  return (ret == 0);
}

bool LPBackend::replicationInfo(QString dataset, QString& remotehost, QString& user, int& port, QString& remotedataset, int& time){
  QString cmd = "lpreserver replicate list";
  //Need output, so run this in a QProcess
  QProcess *proc = new QProcess;
  proc->setProcessChannelMode(QProcess::MergedChannels);
  proc->start(cmd);
  proc->waitForFinished();
  QStringList out = QString(proc->readAllStandardOutput()).split("\n");	
  delete proc;
  //Now process the output
  bool ok = false;
  for(int i=0; i<out.length(); i++){
    if(out[i].contains("->") && out[i].startsWith(dataset)){
      QString data = out[i].section("->",1,1);
      user = data.section("@",0,0);
      remotehost = data.section("@",1,1).section("[",0,0);
      port = data.section("[",1,1).section("]",0,0).toInt();
      remotedataset = data.section(":",1,1).section(" Time",0,0);
      QString synchro = data.section("Time:",1,1).simplified();
	if(synchro == "sync"){ time = -1; }
	else{ time = synchro.toInt(); }
      ok = true;
      break;
    }
  }	  
  return ok;
}
