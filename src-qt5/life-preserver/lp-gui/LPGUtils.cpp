#include "LPGUtils.h"

LPDataset LPGUtils::loadPoolData(QString zpool){
  //Load the current information for the given zpool
  qDebug() << "[DEBUG] New Dataset: " << zpool;
  LPDataset DSC;
  //List all the mountpoints in this dataset
  qDebug() << "[DEBUG] list snapshots";
  QStringList subsets = LPBackend::listDatasetSubsets(zpool);
  QStringList lpsnapcomments;
  QStringList lpsnaps = LPBackend::listLPSnapshots(zpool, lpsnapcomments);
  //Fill the snapshots/comments hash
  for(int i=0; i<lpsnaps.length() && i<lpsnapcomments.length(); i++){
    DSC.snapComment.insert(lpsnaps[i], lpsnapcomments[i]);
  }
  //populate the list of snapshots available for each mountpoint
  for(int i=0; i<subsets.length(); i++){
    //qDebug() << "Subset:" << subsets[i];
    QStringList snaps = LPBackend::listSnapshots(subsets[i]);
    //qDebug() << " - Snapshots:" << snaps;
    if(snaps.isEmpty()){
      //invalid subset - remove it from the list
      subsets.removeAt(i);
      i--;
    }else{
      QStringList subsnaps;
      //only list the valid snapshots that life preserver created
      for(int s=0; s<lpsnaps.length(); s++){
	int index = snaps.indexOf(lpsnaps[s]);
        if(index > -1){ subsnaps << lpsnaps[s]; snaps.removeAt(index); }
      }
      /*//Now list all the other available snapshots (no certain ordering)
      if(!snaps.isEmpty()){try 
	subsnaps << "--"; //so we know that this is a divider between the sections
	subsnaps << snaps;
      }*/
      DSC.subsetHash.insert(subsets[i],subsnaps); //add it to the internal container hash
    }
  }
  //Parse "zpool status <pool>"
  qDebug() << "[DEBUG] get zpool status";
  QStringList zstat = LPBackend::getCmdOutput("zpool status "+zpool);
  qDebug() << zstat.join("\n");
  //qDebug() << "zpool status "+zpool+":\n" << zstat.join("\n");
  bool atheader=false;
  QStringList disks, diskstates, running, errors, finished;
  for(int i=0; i<zstat.length(); i++){
    if(zstat[i].isEmpty()){ continue; }
    else if(zstat[i].startsWith("  scan:")){
      QString line = zstat[i].section(":",1,50).replace("\t"," ").simplified();
      if(line.contains("scrub repaired ")){
	QString timestamp = line.section(" ",9,13,QString::SectionSkipEmpty);
	QString numerrors = line.section(" ",6,6,QString::SectionSkipEmpty);
        finished << QString(QObject::tr("Scrub Finished: %1 (%2 errors)")).arg(timestamp, numerrors);
      }else if(line.contains("scrub cancel")){
	 QString timestamp = line.section(" ",3,7,QString::SectionSkipEmpty);     
	  finished << QString(QObject::tr("Scrub Cancelled: %1")).arg(timestamp);
      }else if(line.contains("scrub")){
	QString timestamp = line.section(" ",4,8,QString::SectionSkipEmpty);
        running << QString(QObject::tr("Scrub Started: %1")).arg(timestamp);      
      }else if(line.contains("resilvered")){
	QString timestamp = line.section(" ",8,12,QString::SectionSkipEmpty);
	QString numerrors = line.section(" ",5,5,QString::SectionSkipEmpty);
        finished << QString(QObject::tr("Resilver Finished: %1 (%2 errors)")).arg(timestamp, numerrors);
      }
    }else if(zstat[i].contains("NAME") && zstat[i].contains("STATE") && zstat[i].contains("READ") ){
      //qDebug() <<"Found header"; 
      atheader=true;
    }else if(zstat[i].startsWith("errors:")){ 
      atheader=false; 
    }else if(atheader){
      QString line = zstat[i].replace("\t"," ").simplified();
      QString dev = line.section(" ",0,0,QString::SectionSkipEmpty);
      QString state = line.section(" ",1,1,QString::SectionSkipEmpty);
      //qDebug() << "Found device:" << dev << state;
      if(dev == zpool){
	DSC.poolStatus = state;
      }else if(line.contains("(resilvering)")){
	disks << dev; diskstates << state; //record this disk and state
	running << QString(QObject::tr("%1: Currently Resilvering")).arg(dev);
      }else{
	disks << dev; diskstates << state; //record this disk and state
	if(state != "ONLINE"){
	  errors << QString(QObject::tr("%1: %2")).arg(dev, state);
	}
      }
    }
  }
  qDebug() << "[DEBUG] Get latest snapshot/replication info";
  //Now get the latest Snapshot/Replication information
  QStringList lpstat = LPBackend::listCurrentStatus();
  //qDebug() << " - " << lpstat;
  for(int i=0; i<lpstat.length(); i++){
    if(lpstat[i].section(":::",0,0) == zpool){
      QString lastSnap = lpstat[i].section(":::",1,1);
      QString lastRep = lpstat[i].section(":::",2,2);
      QString reptarget = lpstat[i].section(":::",3,3);
      if(lastSnap=="-"){ DSC.latestSnapshot = QObject::tr("No Snapshots Available"); }
      else{ DSC.latestSnapshot = lastSnap; }
      if(lastRep!="-"){
        finished << QString(QObject::tr("Latest Replication: %1")).arg(lastRep);
      }else if(LPBackend::listReplicationTargets().contains(zpool) ){
	errors << QObject::tr("No Successful Replication");
      }
      if(reptarget=="-"){ DSC.repHost.clear(); }
      else{ DSC.repHost << reptarget; }
    }
  }
  qDebug() << "[DEBUG] save info to the structure and finish";
  //Now save the info to the dataset
  DSC.harddisks = disks;
  DSC.harddiskStatus = diskstates;
  DSC.errorStatus = errors.join("\n");
  DSC.runningStatus = running.join("\n");
  DSC.finishedStatus = finished.join("\n");
  //Return the dataset
  return DSC;
}

QString LPGUtils::generateReversionFileName(QString fileName, QString destDir){
  fileName = fileName.section("/",-1); //Make sure we only have the filename (no paths)
  if( !destDir.endsWith("/") ){ destDir.append("/"); }
  //Quick check if that file already exists in the destination directory
  if( !QFile::exists(destDir+fileName) ){ return fileName; }
  //Change the filename to prevent overwriting an existing file
  QString ext = fileName.section(".",-1); //Get the extension for the filename
  if( fileName != ext && fileName != ("."+ext) ){
    //Extension found - need careful adjustment of filename
    QString newFileName = fileName;
    newFileName.replace("."+ext, "-reversion."+ext);
    int i=2;
    while( QFile::exists(destDir+newFileName) ){
      //Also need to append a reversion number
      newFileName = fileName; 
      newFileName.replace("."+ext, "-reversion"+QString::number(i)+"."+ext);
      i++; //for the next time around
    }
    fileName = newFileName;
  }else{
    //no extension - just append the reversion
    fileName.append("-reversion");
    if(QFile::exists(destDir+fileName) ){
      //Also need to append a reversion number
      int i=2;
      while( QFile::exists(destDir+fileName+QString::number(i)) ){ i++; }
      fileName.append( QString::number(i) );
    }
  }
  return fileName;
}

bool LPGUtils::revertFile(QString oldPath, QString newPath){
  qDebug() << "Reverting file:" << oldPath << " -> " << newPath;
  bool ok = QFile::copy(oldPath,newPath);
  //return the path to the new file if the copy was successful
  if(ok){ 
    //reset the permissions on the reverted file to match the original
    QFile::setPermissions(newPath, QFile::permissions(oldPath));
    QFileInfo FI(oldPath);
      system( QString("chown "+FI.owner()+":"+FI.group()+" "+newPath).toUtf8() );
  }else{
    qDebug() << " - Error: Could not copy file";
  }
  return ok;
}

QStringList LPGUtils::revertDir(QString oldPath, QString newPath){
  //Note: this is a recursive function and can take quite a while to perform lots of file copies

  //Load the directories and create it if necessary
  QDir oDir(oldPath);
  QDir nDir(newPath);
  bool ok=true;
  if( !nDir.exists() ){
    //Create the new Directory
    qDebug() << "Re-Create parent directory structure:" << newPath;
    nDir.cdUp();
    ok = nDir.mkpath(newPath.section("/",-1)); //also create all parent directories if necessary
    if(ok){ 
      qDebug() << " - Reset permissions on the main parent dir to match snapshot";
      nDir.cd(newPath.section("/",-1)); 
      QFile::setPermissions(newPath, QFile::permissions(oldPath)); //make sure the new dir has the old permissions
      QFileInfo FI(oldPath);
      system( QString("chown "+FI.owner()+":"+FI.group()+" "+newPath).toUtf8() );
    }
  }
  //Get a list of any files that error
  QStringList errors;
  if(!ok){
    errors << newPath;
    return errors;
  }
  //Get a list of all the files in the old dir and copy them over
  QStringList fList = oDir.entryList(QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot, QDir::Name);
  for(int i=0; i<fList.length(); i++){
    if( !revertFile(oldPath+"/"+fList[i], newPath+"/"+fList[i]) ){
       errors << newPath+"/"+fList[i];
    }
  }
  //Now list all the directories in the old dir and recursively copy them over
  fList = oDir.entryList(QDir::Dirs | QDir::Hidden | QDir::NoDotAndDotDot, QDir::Name);
  for(int i=0; i<fList.length(); i++){
    QStringList errs = revertDir(oldPath+"/"+fList[i], newPath+"/"+fList[i]);
    if( !errs.isEmpty() ){ errors << errs; }
  }
  return errors;
}

QString LPGUtils::packageHomeDir(QString username, QString packageName){
  //Check that the user directory exists
  //qDebug() << "Start package dir:" << username << packageName;
  if(!QFile::exists("/usr/home/"+username)){ return ""; }
  //Check that the package has the right extension
  if(!packageName.endsWith(".home.tar.gz")){ packageName.append(".home.tar.gz"); }
  //Generate any additional files to be contained in the package
  
  
  //Generate the command
  QString cmd = "tar";
  QStringList args;
  args << "-czf" << "/usr/home/"+packageName << "-C" << "/usr/home" << username;
  //Create the exclude list and skip these files
  QStringList excludes;
    excludes << "*flashplayer*" << "*/PBI-*.desktop"; //Don't include the flash plugin/PBI entries
  for(int i=0; i<excludes.length(); i++){
    args.prepend(excludes[i]); args.prepend("--exclude");
  }
  //Run the command
  qDebug() << "Package command:" << cmd << args;
  LPBackend::runCmd(cmd, args); //need to use this version due to the complex command

  //Check that the package was created
  QString packagePath;
  if(QFile::exists("/usr/home/"+packageName)){ packagePath = "/usr/home/"+packageName; }
  //Now return the path to the package file
  return packagePath;
}

bool LPGUtils::checkPackageUserPath(QString packagePath, QString *user){
  user->clear();
  //Determine if the file exists
  if( !QFile::exists(packagePath) ){ return false; }
  //Check the username of the home dir in the package
  QStringList ret = LPBackend::getCmdOutput("tar -tvf "+packagePath+" -q \"*/Desktop\"");
  if(ret.isEmpty()){ return false; }
  QString username = ret[0].section(" ",2,2,QString::SectionSkipEmpty).simplified();
  QString dirname = ret[0].section(" ",8,8,QString::SectionSkipEmpty).section("/",0,0).simplified();
  user->append(username); //additional output
  //Now check for the user on the local system
  //This is just a simple check that the user directory exists, and the user/directory are the same within the package
  return (username == dirname && QFile::exists("/usr/home/"+dirname) );	
}

bool LPGUtils::extractHomeDirPackage(QString packagePath){
 //Determine if the file exists
  if( !QFile::exists(packagePath) ){ return false; }
  //Now extract the archive in the home directory
  QString cmd = "tar -xpf "+packagePath+" -C /usr/home";
  qDebug() << "Extract command:" << cmd;
  int ret = LPBackend::runCmd(cmd);
  return (ret == 0);
}

QStringList LPGUtils::listAvailableHardDisks(){
  QDir dev("/dev");
  QStringList filters;
	filters << "ada*" << "da*";
  QStringList devs = dev.entryList(filters, QDir::Files | QDir::System | QDir::NoDotAndDotDot, QDir::Name);
  //Filter out all the partitions (only keep full devices)
  for(int i=0; i<devs.length(); i++){
    devs[i] = devs[i].section("s",0,0,QString::SectionSkipEmpty);
  }
  devs.removeDuplicates();
  return devs;
}

QStringList LPGUtils::scanNetworkSSH(){
  //Output format: <name>:::<address>::::<port>
  QStringList out;
  QStringList netout = LPBackend::getCmdOutput("avahi-browse -art");
  for(int i=0; i<netout.length(); i++){
    if(netout[i].startsWith("=") && netout[i].contains("local")){
      QString name, address, port;
      for(int j=0; j<3; j++){ //need the next 3 lines
        i++; //Move to the next line
	QString var = netout[i].section("=",0,0).replace("\t"," ").simplified();
	QString val = netout[i].section("[",1,1).section("]",0,0).simplified();
	if(var == "hostname"){ name = val.section(".local",0,0).simplified(); }
	else if(var == "address"){ address = val; }
	else if(var == "port"){ port = val; }
      }
      //Check that it is an SSH connection that is open (port 22)
      if(port == "22"){ 
	 out << name+":::"+address+":::"+port;
      }
    }
  }
  return out;
}
