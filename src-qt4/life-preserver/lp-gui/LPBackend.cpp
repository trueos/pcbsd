#include "LPBackend.h"

// ==============
//     Informational
// ==============
QStringList LPBackend::listPossibleDatasets(){
  QString cmd = "zpool list -H -o name";
  QStringList out = LPBackend::getCmdOutput(cmd);
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
  QStringList out = LPBackend::getCmdOutput(cmd);
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
  QStringList out = LPBackend::getCmdOutput(cmd);
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
    list = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Time | QDir::Reversed);
  }
  return list;
}

QStringList LPBackend::listLPSnapshots(QString dataset){
  QString cmd = "lpreserver listsnap "+dataset;
  QStringList out = LPBackend::getCmdOutput(cmd);
  //Now process the output
  QStringList list;
  for(int i=0; i<out.length(); i++){ //oldest ->newest
    if(out[i].startsWith(dataset+"@")){
      QString snap = out[i].section("@",1,3).section(" ",0,0).simplified();;
      if(!snap.isEmpty()){ list << snap; }
    }
  }
   
  return list;	
}

QStringList LPBackend::listReplicationTargets(){
  QString cmd = "lpreserver replicate list";
  QStringList out = LPBackend::getCmdOutput(cmd);
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
  QStringList out = LPBackend::getCmdOutput(cmd);
  //Now process the output	
  QStringList list;
  for(int i=2; i<out.length(); i++){ //first 2 lines are headers
    //Format: <dataset>:::<lastsnapshot | NONE>:::<lastreplication | NONE>
    if(out[i].isEmpty()){ continue; }
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
  int ret = LPBackend::runCmd(cmd);
   
  return (ret == 0);
}

bool LPBackend::removeDataset(QString dataset){
  QString cmd = "lpreserver cronsnap "+dataset+" stop";
  int ret = LPBackend::runCmd(cmd);
   
  return (ret == 0);
}

bool LPBackend::datasetInfo(QString dataset, int& time, int& numToKeep){
  QString cmd = "lpreserver listcron";
  QStringList out = LPBackend::getCmdOutput(cmd);
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
void LPBackend::newSnapshot(QString dataset, QString snapshotname){
  //This needs to run externally - since the snapshot is simply added to the queue, and the replication
  //   afterwards may take a long time.
  QString cmd = "lpreserver mksnap --replicate "+dataset+" "+snapshotname;
  QProcess::startDetached(cmd);
   
  return;
}

bool LPBackend::removeSnapshot(QString dataset, QString snapshot){
  QString cmd = "lpreserver rmsnap "+dataset +" "+snapshot;
  int ret = LPBackend::runCmd(cmd);
   
  return (ret == 0);
}

bool LPBackend::revertSnapshot(QString dataset, QString snapshot){
  QString cmd = "lpreserver revertsnap "+dataset +" "+snapshot;
  int ret  = LPBackend::runCmd(cmd);
   
  return (ret == 0);
}

// ==================
//    Replication Management
// ==================
bool LPBackend::setupReplication(QString dataset, QString remotehost, QString user, int port, QString remotedataset, int time){
  QString stime = "sync"; //synchronize on snapshot creation (default)
  if(time >= 0 && time < 24){ stime = QString::number(time); } //daily at a particular hour (24 hour notation)
  
  
  QString cmd = "lpreserver replicate add "+remotehost+" "+user+" "+ QString::number(port)+" "+dataset+" "+remotedataset+" "+stime;
  int ret = LPBackend::runCmd(cmd);
  
  return (ret == 0);
}

bool LPBackend::removeReplication(QString dataset){
  QString cmd = "lpreserver replicate remove "+dataset;
  int ret = LPBackend::runCmd(cmd);	
   
  return (ret == 0);
}

bool LPBackend::replicationInfo(QString dataset, QString& remotehost, QString& user, int& port, QString& remotedataset, int& time){
  QString cmd = "lpreserver replicate list";
  QStringList out = LPBackend::getCmdOutput(cmd);
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

// ======================
//          SSH Key Management
// ======================
bool LPBackend::setupSSHKey(QString remoteHost, QString remoteUser, int remotePort){
  QString LPPATH = "/usr/local/share/lifePreserver";
  QString cmd = "xterm -e \""+LPPATH+"/scripts/setup-ssh-keys.sh "+remoteUser+" "+remoteHost+" "+QString::number(remotePort)+"\"";
  int ret = LPBackend::runCmd(cmd);
  return (ret == 0);
}

QStringList LPBackend::findValidUSBDevices(){
  //Return format: "<mountpoint> (<device node>")
  QString cmd = "mount";
  QStringList out = LPBackend::getCmdOutput(cmd);
  //Now process the output
  QStringList list;
  for(int i=0; i<out.length(); i++){
      if(out[i].startsWith("/dev/da") && out[i].contains("(msdosfs,")){
      QString mountpoint = out[i].section(" on ",1,1).section("(",0,0).simplified();
      QString devnode = out[i].section(" on ",0,0).section("/",-1).simplified();
      list << mountpoint +" ("+devnode+")";
    }
  }
  return list;
}

bool LPBackend::copySSHKey(QString mountPath, QString localHost){
  QString publicKey = "/root/.ssh/id_rsa";
  //copy the file onto the designated USB stick
  if(!mountPath.endsWith("/")){ mountPath.append("/"); }
  QDir lDir=mountPath + "lpreserver";
  if ( ! lDir.exists() )
     lDir.mkdir(lDir.path());

  mountPath.append("lpreserver/"+localHost+"-id_rsa");

  bool ok = QFile::copy(publicKey, mountPath);
  return ok;
}

// ======================
//        USB Device Management
// ======================
QStringList LPBackend::listDevices(){
  //Scan the system for all valid da* and ada* devices (USB/SCSI, SATA)
  //Return format: "<device node> (<device information>)"
  QDir devDir("/dev");
  QStringList devs = devDir.entryList(QStringList() << "da*"<<"ada*", QDir::System | QDir::NoSymLinks, QDir::Name);
  QStringList camOut = LPBackend::getCmdOutput("camcontrol devlist");
  QStringList output, flist;	
  for(int i=0; i<devs.length(); i++){
    flist = camOut.filter("("+devs[i]+",");
    //still need to add an additional device filter to weed out devices currently in use.
    if(!flist.isEmpty()){ output << devs[i] + " ("+flist[0].section(">",0,0).remove("<").simplified()+")"; }
  }
  return output;
}

bool LPBackend::isMounted(QString device){
  qDebug() << "Device mount check not implemented yet:" << device;
  return false;
}

bool LPBackend::unmountDevice(QString device){
  qDebug() << "Device unmounting not implemented yet:" << device;
  return false;
}

// ======================
//        ZPOOL Disk Management
// ======================
bool LPBackend::attachDisk(QString pool, QString disk){
  if( !disk.startsWith("/dev/") ){ disk.prepend("/dev/"); } //make sure it is the full disk path
  if( !QFile::exists(disk) ){ return false; } //make sure the disk exists
  QString cmd = "lpreserver zpool attach "+pool+" "+disk;
  //Run the command
  int ret = LPBackend::runCmd(cmd);
  return (ret ==0);
}

bool LPBackend::detachDisk(QString pool, QString disk){
  QString cmd = "lpreserver zpool detach "+pool+" "+disk;
  //Run the command
  int ret = LPBackend::runCmd(cmd);
  return (ret ==0);	
}

bool LPBackend::setDiskOnline(QString pool, QString disk){
  QString cmd = "lpreserver zpool online "+pool+" "+disk;
  //Run the command
  int ret = LPBackend::runCmd(cmd);
  return (ret ==0);	
}

bool LPBackend::setDiskOffline(QString pool, QString disk){
  QString cmd = "lpreserver zpool offline "+pool+" "+disk;
  //Run the command
  int ret = LPBackend::runCmd(cmd);
  return (ret ==0);	
}

// =========================
//             PRIVATE FUNCTIONS
// =========================
QStringList LPBackend::getCmdOutput(QString cmd){
  QProcess *proc = new QProcess;
  proc->setProcessChannelMode(QProcess::MergedChannels);
  proc->start(cmd);
  while(!proc->waitForFinished(300)){
    QCoreApplication::processEvents();
  }
  QStringList out = QString(proc->readAllStandardOutput()).split("\n");	
  delete proc;	
  return out;
}

int LPBackend::runCmd(QString cmd){
  QProcess *proc = new QProcess;
  proc->setProcessChannelMode(QProcess::MergedChannels);
  proc->start(cmd);
  while(!proc->waitForFinished(300)){
    QCoreApplication::processEvents();
  }
  int ret = proc->exitCode();
  delete proc;	
  return ret;
}
