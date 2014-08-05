#include "DB.h"

/* === Representation of Database ====
  HASH -> Info: [JailList, RepoList] 
	       Categories: [Jails/<jail name>/, Repos/<repoID>]
  
  Jail Info Category -> Info: [JID, jailIP, jailPath, repoID, hasUpdates, updateLog, pkgList, lastSyncTimeStamp (internal)]
				Categories: [pkg/<local pkg origin>/]
				
  Repo Info Category -> Info: [pkgList, lastSyncTimeStamp (internal)]
				  Categories: [pkg/<remote pkg origin>/]
				  
  Local Pkg Info: [name, origin, description, comment, version, maintainer, website, 
		license, size, arch, isOrphan, isLocked, message, timestamp, dependencies, reverse dependencies,
		categories, files, options, users, groups, annotations]
		
  Remote Pkg Info: [name, origin, version, maintainer, comment, description, website, arch,
		licence, size, message, dependencies, reverse dependencies, categories, options,
		annotations]


  EXAMPLE: 
  To fetch the name of a pkg on the repository for a jail:
  
  (Input string variables: myjail and mypkg)
  QString repoID = HASH->value("Jail/"+myjail+"/repoID","");
  QString name = HASH->value("Repo/"+repoID+"/"+mypkg+"/name","");
  
*/

#define LISTDELIMITER QString("::::")
#define LOCALSYSTEM QString("**LOCALSYSTEM**")

DB::DB(QObject *parent) : QObject(parent){
  HASH = new QHash<QString, QString>;
  SYNC = new Syncer(this, HASH);
	connect(SYNC, SIGNAL(finishedLocal()), this, SLOT(localSyncFinished()) );
	connect(SYNC, SIGNAL(finishedRemote()), this, SLOT(remoteSyncFinished()) );
	connect(SYNC, SIGNAL(finishedPBI()), this, SLOT(pbiSyncFinished()) );
	connect(SYNC, SIGNAL(finishedSystem()), this, SLOT(systemSyncFinished()) );
	connect(SYNC, SIGNAL(finishedJails()), this, SLOT(jailSyncFinished()) );
  chkTime = new QTimer(this);
	chkTime->setInterval(1000); // 1 second delay for sync on changes
	chkTime->setSingleShot(true);
	connect(chkTime, SIGNAL(timeout()), this, SLOT(kickoffSync()) );
  watcher = new QFileSystemWatcher(this);
    connect(watcher, SIGNAL(directoryChanged(QString)), this, SLOT(watcherChange()) );
    connect(watcher, SIGNAL(fileChanged(QString)), this, SLOT(watcherChange()) );
  //Setup the watcher to look for the pc-systemflag flags
  if(!QFile::exists("/tmp/.pcbsdflags")){ QProcess::startDetached("pc-systemflag CHECKDIR"); }
  locrun = remrun = pbirun = jrun = sysrun = false;
}

DB::~DB(){
  if(SYNC->isRunning()){ SYNC->quit(); }//make sure the sync gets stopped appropriately
  delete HASH;
}

// ===============
//    PUBLIC
// ===============
void DB::startSync(){
  // --reset watched directories
  if(!watcher->directories().isEmpty()){ watcher->removePaths( watcher->directories() ); }
  watcher->addPath("/var/db/pkg"); //local system pkg database should always be watched
  watcher->addPath("/tmp/.pcbsdflags"); //local PC-BSD system flags
  watcher->addPath("/var/db/pbi/index"); //local PBI index directory
  QTimer::singleShot(0,this, SLOT(kickoffSync()));
}

void DB::shutDown(){
   if(SYNC->isRunning()){ SYNC->quit(); } //make sure the sync gets stopped appropriately
  HASH->clear();
}

QString DB::fetchInfo(QStringList request){
  QString hashkey;
  //Determine the internal hash key for the particular request
  if(request.length()==1){
	  
  }else if(request.length()==2){
    if(request[0]=="jail"){
      if(request[1]=="list"){ hashkey = "JailList"; }
    }
    
  }else if(request.length()==3){
    if(request[0]=="jail"){
      hashkey = "Jails/"+request[1];
      if(request[2]=="id"){ hashkey.append("/JID"); }
      else if(request[2]=="ip"){ hashkey.append("/jailIP"); }
      else if(request[2]=="path"){ hashkey.append("/jailPath"); }
      else{ hashkey.clear(); }
    }else if(request[0]=="pkg"){
      if(request[1]=="#system"){ hashkey="Jails/"+LOCALSYSTEM+"/"; }
      else{ hashkey="Jails/"+request[1]+"/"; }
      if(request[2]=="installedlist"){ hashkey.append("pkgList"); }
      else if(request[2]=="hasupdates"){ hashkey.append("hasUpdates"); }
      else if(request[2]=="updatemessage"){ hashkey.append("updateLog"); }
      else if(request[2]=="remotelist"){ hashkey="Repos/"+HASH->value(hashkey+"repoID")+"/pkgList"; }
      else{ hashkey.clear(); }
    }else if(request[0]=="pbi"){
      if(request[1]=="list"){
        if(request[2]=="apps"){ hashkey="PBI/pbiList"; }
	else if(request[2]=="cats"){ hashkey = "PBI/catList"; }
	else if(request[2]=="new"){ hashkey = "PBI/newappList"; }
	else if(request[2]=="highlighted"){ hashkey = "PBI/highappList"; }
	else if(request[2]=="recommended"){ hashkey = "PBI/recappList"; }
      }		
    }
  }else if(request.length()==4){
    if(request[0]=="pbi"){
      if(request[1]=="app"){
        hashkey = "PBI/"+request[2]+"/"+request[3]; //pkg origin and variable
      }else if(request[1]=="cat"){
	hashkey = "PBI/cats/"+request[2]+"/"+request[3]; //pkg origin and variable
      }
    }
  }else if(request.length()==5){
    if(request[0]=="pkg"){
      if(request[1]=="#system"){ hashkey="Jails/"+LOCALSYSTEM+"/"; }
      else{ hashkey="Jails/"+request[1]+"/"; }	    
      if(request[2]=="local"){
        hashkey.append("pkg/"+request[3]+"/"+request[4]); // "pkg/<origin>/<variable>"
      }else if(request[2]=="remote"){
	hashkey="Repos/"+HASH->value(hashkey+"repoID")+"/pkg/"+request[3]+"/"+request[4];
      }
    }
  }
  //Now fetch/return the info
  QString val;
  if(hashkey.isEmpty()){ val = "[ERROR] Invalid Information request: \""+request.join(" ")+"\""; }
  else{
    //Check if a sync is running and wait a moment until it is done
    while(isRunning(hashkey)){ pausems(100); } //re-check every 100 ms
    //Now check for info availability
    if(!HASH->contains(hashkey)){ val = "[ERROR] Information not available"; }
    else{
      val = HASH->value(hashkey,"");
      val.replace(LISTDELIMITER, ", ");
      if(val.isEmpty()){ val = " "; } //make sure it has a blank space at the minimum
    }
  }
  return val;
}

// ========
//   PRIVATE
// ========
//Internal pause/syncing functions
bool DB::isRunning(QString key){
  if(!SYNC->isRunning()){ return false; } //no sync going on - all info available
  //A sync is running - check if the current key falls into a section not finished yet
  if(key.startsWith("Jails/")){ return locrun; } //local sync running
  else if(key.startsWith("Repos/")){ return remrun; } //remote sync running
  else if(key.startsWith("PBI/")){ return pbirun; } //pbi sync running
  else{ return false; }
  //jrun and sysrun not used (yet)
}

void DB::pausems(int ms){
  //pause the calling function for a few milliseconds
  QTime time = QTime::currentTime().addMSecs(ms);
  while(QTime::currentTime() < time){
    QCoreApplication::processEvents();
  }
}

// ============
//   PRIVATE SLOTS
// ============
void DB::watcherChange(){
  //Tons of these signals while syncing
    //   - so use a QTimer to compress them all down to a single call (within a short time frame)
  if( !chkTime->isActive()){ chkTime->start(); }
}

void DB::jailSyncFinished(){ 
  jrun = false; 
  //Also reset the list of watched jails
  QStringList jails = watcher->directories().filter("/var/db/pkg");
  jails.removeAll("/var/db/pkg"); //don't remove the local pkg dir - just the jails
  if(!jails.isEmpty()){ watcher->removePaths(jails); }
  jails = HASH->value("JailList").split(LISTDELIMITER);
  for(int i=0; i<jails.length(); i++){
    watcher->addPath(HASH->value("Jails/"+jails[i]+"/jailPath")+"/var/db/pkg"); //watch this jail's pkg database
  }
}

//****************************************
//    SYNCER CLASS
//****************************************

Syncer::Syncer(QObject *parent, QHash<QString,QString> *hash) : QThread(parent){
  HASH = hash;
}

Syncer::~Syncer(){
  stopping = true;
}

//===============
//   PRIVATE
//===============
//System Command functions 
QStringList Syncer::sysCmd(QString cmd){ // ensures only 1 running at a time (for things like pkg)
  //static running = false;
  //while(running){ QThread::msleep(200); } //wait until the current command finishes
  //running=true;
  QStringList out = directSysCmd(cmd);
  //running=false;
  return out;
}

QStringList Syncer::directSysCmd(QString cmd){ //run command immediately
   QProcess p;  
   //Make sure we use the system environment to properly read system variables, etc.
   p.setProcessEnvironment(QProcessEnvironment::systemEnvironment());
   //Merge the output channels to retrieve all output possible
   p.setProcessChannelMode(QProcess::MergedChannels);   
   p.start(cmd);
   while(p.state()==QProcess::Starting || p.state() == QProcess::Running){
     p.waitForFinished(100);
     QCoreApplication::processEvents();
     if(stopping){break;}
   }
   if(stopping){ p.terminate(); return QStringList(); }
   QString tmp = p.readAllStandardOutput();
   if(tmp.contains("database is locked", Qt::CaseInsensitive)){
     return directSysCmd(cmd); //try again - in case the pkg database is currently locked
   }else{
     if(tmp.endsWith("\n")){ tmp.chop(1); }
     return tmp.split("\n");
   }
}

QStringList Syncer::readFile(QString filepath){
  QStringList out;
  QFile file(filepath);
  if(file.exists()){
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
	QTextStream fout(&file);
	while(!fout.atEnd()){
	  out << fout.readLine();
	}
	file.close();
    }
  }
  return out;
}

//Internal Hash maintenance functions
void Syncer::clearRepo(QString repo){
  //Remove All Repo specific info
  QStringList pkeys = HASH->keys();
    pkeys = pkeys.filter("Repos/"+repo+"/");
  for(int i=0; i<pkeys.length(); i++){ HASH->remove(pkeys[i]); }	
}

void Syncer::clearJail(QString jail){
  //Remove All Jail specific info
  QStringList pkeys = HASH->keys();
    pkeys = pkeys.filter("Jails/"+jail+"/");
  for(int i=0; i<pkeys.length(); i++){ HASH->remove(pkeys[i]); }
}

void Syncer::clearLocalPkg(QString pkgprefix){
  QStringList pkeys = HASH->keys();
    pkeys = pkeys.filter(pkgprefix);
  for(int i=0; i<pkeys.length(); i++){ HASH->remove(pkeys[i]); }
}

void Syncer::clearPbi(){
  QStringList pkeys = HASH->keys();
    pkeys = pkeys.filter("PBI/");
  for(int i=0; i<pkeys.length(); i++){ HASH->remove(pkeys[i]); }	
}

bool Syncer::needsLocalSync(QString jail){
  //Checks the pkg database file for modification since the last sync
  if(!HASH->contains("Jails/"+jail+"/lastSyncTimeStamp")){ return true; }
  else{
    //Previously synced - look at the DB modification time
    QString path = "/var/db/pkg/local.sqlite";
    if(jail!=LOCALSYSTEM){ path.prepend( HASH->value("Jails/"+jail+"/jailPath","") ); }
    qint64 mod = QFileInfo(path).lastModified().toMSecsSinceEpoch();
    qint64 stamp = HASH->value("Jails/"+jail+"/lastSyncTimeStamp","").toLongLong();
    return (mod > stamp); //was it modified after the last sync?
  }
}

bool Syncer::needsRemoteSync(QString jail){
  //Checks the pkg repo files for changes since the last sync
  if(!HASH->contains("Jails/"+jail+"/repoID")){ return true; } //no repoID yet
  else if(HASH->value("Jails/"+jail+"/repoID") != generateRepoID(jail) ){ return true; } //repoID changed
  else if( !HASH->contains("Repos/"+HASH->value("Jails/"+jail+"/repoID")+"/lastSyncTimeStamp") ){ return true; } //Repo Never synced
  else{
    QDir pkgdb( HASH->value("Jails/"+jail+"/jailPath","")+"/var/db/pkg" );
    QFileInfoList repos = pkgdb.entryInfoList(QStringList() << "repo-*.sqlite");
    qint64 stamp = HASH->value("Repos/"+HASH->value("Jails/"+jail+"/repoID")+"/lastSyncTimeStamp").toLongLong();
    for(int i=0; i<repos.length(); i++){
      //check each repo database for recent changes
      if(repos[i].lastModified().toMSecsSinceEpoch() > stamp){ return true; }
    }
    return false;
  }
}

bool Syncer::needsPbiSync(){
  //Check the PBI index to see if it needs to be resynced
  if(!HASH->contains("PBI/lastSyncTimeStamp")){ return true; }
  else{
    qint64 mod = QFileInfo("/var/db/pbi/index/PBI_INDEX").lastModified().toMSecsSinceEpoch();
    qint64 stamp = HASH->value("PBI/lastSyncTimeStamp").toLongLong();
    return (mod > stamp);
  }
}


QString Syncer::generateRepoID(QString jail){
  QString cmd = "pkg -v -v";
  if(jail!=LOCALSYSTEM){ cmd = "pkg -j "+HASH->value("Jails/"+jail+"/JID")+" -v -v"; }
  QStringList urls = directSysCmd(cmd).filter(" url ");
  QString ID;
  for(int i=0; i<urls.length(); i++){
    ID.append( urls[i].section(" : ",1,50).simplified() );
  }
  //qDebug() << "RepoID: "<< jail << ID;
  return ID;
}

//===============
//  PRIVATE SLOTS
//===============

//General Sync Functions
void Syncer::performSync(){
  stopping = false;
  qDebug() << "Syncing system information";
  //First do the operations that can potentially lock the pkg database first, but are fast
  if(stopping){ return; }
  syncJailInfo();
  emit finishedJails();
  qDebug() << " - Jails done";
  if(stopping){ return; }
  syncPkgLocal();
  emit finishedLocal();
  qDebug() << " - Local done";
  if(stopping){ return; }
  //Now Load the PBI database (more useful, will not lock system usage, and is fast)
  syncPbi();
  qDebug() << " - PBI done";
  emit finishedPBI();
  //Now do all the remote pkg info retrieval (won't lock the pkg database in 1.3.x?)
   // Note: This can take a little while
  syncPkgRemote();
  qDebug() << " - Remote done";
  emit finishedRemote();
  if(stopping){ return; }
  //Now check for overall system updates (not done yet)
  emit finishedSystem();
  qDebug() << " - Finished data sync";
}

void Syncer::syncJailInfo(){
  //Get the internal list of jails
  QStringList jails = HASH->value("JailList","").split(LISTDELIMITER);
  //Now get the current list of running jails and insert individual jail info
  QStringList info = directSysCmd("jls");
  QStringList found;
  for(int i=1; i<info.length() && !stopping; i++){ //skip the header line
    info[i] = info[i].replace("\t"," ").simplified();
    QString name = info[i].section(" ",2,2); //hostname
    found << name; //add it to the new list
    jails.removeAll(name); //remove from the old list
    HASH->insert("Jails/"+name+"/JID", info[i].section(" ",0,0));
    HASH->insert("Jails/"+name+"/jailIP", info[i].section(" ",1,1));
    HASH->insert("Jails/"+name+"/jailPath", info[i].section(" ",3,3));
  }
  HASH->insert("JailList", found.join(LISTDELIMITER));
  if(stopping){ return; } //catch for if the daemon is stopping
  //Remove any old jails from the hash
  for(int i=0; i<jails.length() && !stopping; i++){ //anything left over in the list
    clearJail(jails[i]); 
  }
  
}

void Syncer::syncPkgLocalJail(QString jail){
  if(jail.isEmpty()){ return; }
 //Sync the local pkg information
 if(needsLocalSync(jail)){
  //qDebug() << "Sync local jail info:" << jail;
  QString prefix = "Jails/"+jail+"/pkg/";
  clearLocalPkg(prefix); //clear the old info from the hash
  //Format: origin, name, version, maintainer, comment, description, website, size, arch, timestamp, message, isOrphan, isLocked
  QString cmd = "pkg query -a";
  QString opt = " PKG::%o::::%n::::%v::::%m::::%c::::%e::::%w::::%sh::::%q::::%t::::%M::::%a::::%k";
  if(jail!=LOCALSYSTEM){
    cmd.replace("pkg ", "pkg -j "+HASH->value("Jails/"+jail+"/JID")+" ");
  }
  if(stopping){ return; }
  QStringList info = directSysCmd(cmd+opt).join("\n").split("PKG::");
  QStringList installed;
  for(int i=0; i<info.length(); i++){
    QStringList line = info[i].split("::::");
    if(line.length()<13){ continue; } //incomplete line
    installed << line[0]; //add to the list of installed pkgs
    HASH->insert(prefix+line[0]+"/origin", line[0]);
    HASH->insert(prefix+line[0]+"/name", line[1]);
    HASH->insert(prefix+line[0]+"/version", line[2]);
    HASH->insert(prefix+line[0]+"/maintainer", line[3]);
    HASH->insert(prefix+line[0]+"/comment", line[4]);
    HASH->insert(prefix+line[0]+"/description", line[5]);
    HASH->insert(prefix+line[0]+"/website", line[6]);
    HASH->insert(prefix+line[0]+"/size", line[7]);
    HASH->insert(prefix+line[0]+"/arch", line[8]);
    HASH->insert(prefix+line[0]+"/timestamp", line[9]);
    HASH->insert(prefix+line[0]+"/message", line[10]);
    if(line[11]=="1"){ HASH->insert(prefix+line[0]+"/isOrphan", "true"); }
    else{ HASH->insert(prefix+line[0]+"/isOrphan", "false"); }
    if(line[12]=="1"){ HASH->insert(prefix+line[0]+"/isLocked", "true"); }
    else{ HASH->insert(prefix+line[0]+"/isLocked", "false"); }
  }
  //Now save the list of installed pkgs
  HASH->insert("Jails/"+jail+"/pkgList", installed.join(LISTDELIMITER));
  //qDebug() << "Jail:" << jail << " Installed pkg list:" << info.length() << installed.length();
  //Now go through the pkgs and get the more complicated/detailed info
  // -- dependency list
  if(stopping){ return; }
  info = directSysCmd(cmd+" %o::::%do");
  installed.clear();
  QString orig;
  for(int i=0; i<info.length(); i++){
    if(orig!=info[i].section("::::",0,0) && !orig.isEmpty()){ 
      HASH->insert(prefix+orig+"/dependencies", installed.join(LISTDELIMITER));
      installed.clear();
    }
    orig = info[i].section("::::",0,0);
    installed << info[i].section("::::",1,1);
  }
  HASH->insert(prefix+orig+"/dependencies", installed.join(LISTDELIMITER)); //make sure to save the last one too
  // -- reverse dependency list
  if(stopping){ return; }
  info = directSysCmd(cmd+" %o::::%ro");
  installed.clear();
  orig.clear();
  for(int i=0; i<info.length(); i++){
    if(orig!=info[i].section("::::",0,0) && !orig.isEmpty()){ 
      HASH->insert(prefix+orig+"/rdependencies", installed.join(LISTDELIMITER));
      installed.clear();
    }
    orig = info[i].section("::::",0,0);
    installed << info[i].section("::::",1,1);
  }
  HASH->insert(prefix+orig+"/rdependencies", installed.join(LISTDELIMITER)); //make sure to save the last one too
  // -- categories
  if(stopping){ return; }
  info = directSysCmd(cmd+" %o::::%C");
  installed.clear();
  orig.clear();
  for(int i=0; i<info.length(); i++){
    if(orig!=info[i].section("::::",0,0) && !orig.isEmpty()){ 
      HASH->insert(prefix+orig+"/categories", installed.join(LISTDELIMITER));
      installed.clear();
    }
    orig = info[i].section("::::",0,0);
    installed << info[i].section("::::",1,1);
  }
  HASH->insert(prefix+orig+"/categories", installed.join(LISTDELIMITER)); //make sure to save the last one too
  // -- files
  if(stopping){ return; }
  info = directSysCmd(cmd+" %o::::%Fp");
  installed.clear();
  orig.clear();
  for(int i=0; i<info.length(); i++){
    if(orig!=info[i].section("::::",0,0) && !orig.isEmpty()){ 
      HASH->insert(prefix+orig+"/files", installed.join(LISTDELIMITER));
      installed.clear();
    }
    orig = info[i].section("::::",0,0);
    installed << info[i].section("::::",1,1);
  }
  HASH->insert(prefix+orig+"/files", installed.join(LISTDELIMITER)); //make sure to save the last one too
  // -- options
  if(stopping){ return; }
  info = directSysCmd(cmd+" %o::::%Ok=%Ov");
  installed.clear();
  orig.clear();
  for(int i=0; i<info.length(); i++){
    if(orig!=info[i].section("::::",0,0) && !orig.isEmpty()){ 
      HASH->insert(prefix+orig+"/options", installed.join(LISTDELIMITER));
      installed.clear();
    }
    orig = info[i].section("::::",0,0);
    installed << info[i].section("::::",1,1);
  }
  HASH->insert(prefix+orig+"/options", installed.join(LISTDELIMITER)); //make sure to save the last one too  
  // -- licenses
  if(stopping){ return; }
  info = directSysCmd(cmd+" %o::::%L");
  installed.clear();
  orig.clear();
  for(int i=0; i<info.length(); i++){
    if(orig!=info[i].section("::::",0,0) && !orig.isEmpty()){ 
      HASH->insert(prefix+orig+"/license", installed.join(LISTDELIMITER));
      installed.clear();
    }
    orig = info[i].section("::::",0,0);
    installed << info[i].section("::::",1,1);
  }
  HASH->insert(prefix+orig+"/license", installed.join(LISTDELIMITER)); //make sure to save the last one too 
  // -- users
  if(stopping){ return; }
  info = directSysCmd(cmd+" %o::::%U");
  installed.clear();
  orig.clear();
  for(int i=0; i<info.length(); i++){
    if(orig!=info[i].section("::::",0,0) && !orig.isEmpty()){ 
      HASH->insert(prefix+orig+"/users", installed.join(LISTDELIMITER));
      installed.clear();
    }
    orig = info[i].section("::::",0,0);
    installed << info[i].section("::::",1,1);
  }
  HASH->insert(prefix+orig+"/users", installed.join(LISTDELIMITER)); //make sure to save the last one too
  // -- groups
  if(stopping){ return; }
  info = directSysCmd(cmd+" %o::::%G");
  installed.clear();
  orig.clear();
  for(int i=0; i<info.length(); i++){
    if(orig!=info[i].section("::::",0,0) && !orig.isEmpty()){ 
      HASH->insert(prefix+orig+"/groups", installed.join(LISTDELIMITER));
      installed.clear();
    }
    orig = info[i].section("::::",0,0);
    installed << info[i].section("::::",1,1);
  }
  HASH->insert(prefix+orig+"/groups", installed.join(LISTDELIMITER)); //make sure to save the last one too
 } //done with local pkg sync
 if(needsRemoteSync(jail) || needsLocalSync(jail)){
  //qDebug() << "Sync jail pkg update availability:" << jail;
  //Now Get jail update status/info
  if(stopping){ return; }
  QString cmd = "pc-updatemanager pkgcheck";
  if(jail!=LOCALSYSTEM){ cmd = "pc-updatemanager -j "+jail+" pkgcheck"; }
  QString log = directSysCmd(cmd).join("\n");
  HASH->insert("Jails/"+jail+"/updateLog", log);
  if(log.contains("To start the upgrade run ")){ HASH->insert("Jails/"+jail+"/hasUpdates", "true"); }
  else{ HASH->insert("Jails/"+jail+"/hasUpdates", "false"); }
 }
  //Now stamp the current time this jail was checked
  HASH->insert("Jails/"+jail+"/lastSyncTimeStamp", QString::number(QDateTime::currentMSecsSinceEpoch()));
}


void Syncer::syncPkgLocal(){
  QStringList jails = HASH->value("JailList","").split(LISTDELIMITER);
  //Do the Local system first
  if(stopping){ return; }
  syncPkgLocalJail(LOCALSYSTEM);
  //Now do any running jails
  for(int i=0; i<jails.length(); i++){
    if(stopping){ return; }
    syncPkgLocalJail(jails[i]);
  }
}

void Syncer::syncPkgRemoteJail(QString jail){
  if(jail.isEmpty()){ return; }
  //Sync the local pkg information
  if(needsRemoteSync(jail)){
    QString repoID = generateRepoID(jail);
    HASH->insert("Jails/"+jail+"/repoID", repoID);
    //Now get all the remote pkg info for this repoID/jail
    clearRepo(repoID);
    //Now fetch remote pkg info for this repoID
    QString prefix = "Repos/"+repoID+"/pkg/";
    QString cmd = "pkg rquery -a ";
    if(jail!=LOCALSYSTEM){ cmd = "pkg -j "+HASH->value("Jails/"+jail+"/JID")+" rquery -a "; }
    QStringList info = directSysCmd(cmd+"PKG::%o::::%n::::%v::::%m::::%w::::%q::::%sh::::%c::::%e::::%M").join("\n").split("PKG::");
    //Format: origin, name, version, maintainer, website, arch, size, comment, description, message
    QStringList pkglist;
    for(int i=0; i<info.length(); i++){
      QStringList pkg = info[i].split("::::");
      if(pkg.length()<9){ continue; } //invalid line
      pkglist << pkg[0];
      HASH->insert(prefix+pkg[0]+"/origin", pkg[0]);
      HASH->insert(prefix+pkg[0]+"/name", pkg[1]);
      HASH->insert(prefix+pkg[0]+"/version", pkg[2]);
      HASH->insert(prefix+pkg[0]+"/maintainer", pkg[3]);
      HASH->insert(prefix+pkg[0]+"/website", pkg[4]);
      HASH->insert(prefix+pkg[0]+"/arch", pkg[5]);
      HASH->insert(prefix+pkg[0]+"/size", pkg[6]);
      HASH->insert(prefix+pkg[0]+"/comment", pkg[7]);
      HASH->insert(prefix+pkg[0]+"/description", pkg[8]);
      HASH->insert(prefix+pkg[0]+"/message", pkg[9]);
    }
    //Now save the list of installed pkgs
    HASH->insert("Repos/"+repoID+"/pkgList", pkglist.join(LISTDELIMITER));
    //Make sure that from now on the default command does not re-check for new repo files
    cmd = cmd.replace(" rquery -a ", " rquery -aU ");
    //Now go through the pkgs and get the more complicated/detailed info
    // -- dependency list
    if(stopping){ return; }
    info = directSysCmd(cmd+" %o::::%do");
    pkglist.clear();
    QString orig;
    for(int i=0; i<info.length(); i++){
      if(orig!=info[i].section("::::",0,0) && !orig.isEmpty()){ 
        HASH->insert(prefix+orig+"/dependencies", pkglist.join(LISTDELIMITER));
        pkglist.clear();
      }
      orig = info[i].section("::::",0,0);
      pkglist << info[i].section("::::",1,1);
    }
    HASH->insert(prefix+orig+"/dependencies", pkglist.join(LISTDELIMITER)); //make sure to save the last one too
    // -- reverse dependency list (DEACTIVATED - can take 5-10 minutes for needless info (use the installed rdependencies instead) )
    /*if(stopping){ return; }
    info = directSysCmd(cmd+" %o::::%ro");
    pkglist.clear();
    orig.clear();
    for(int i=0; i<info.length(); i++){
      if(orig!=info[i].section("::::",0,0) && !orig.isEmpty()){ 
        HASH->insert(prefix+orig+"/rdependencies", pkglist.join(LISTDELIMITER));
        pkglist.clear();
      }
      orig = info[i].section("::::",0,0);
      pkglist << info[i].section("::::",1,1);
    }
    HASH->insert(prefix+orig+"/rdependencies", pkglist.join(LISTDELIMITER)); //make sure to save the last one too
    */
    // -- categories
    if(stopping){ return; }
    info = directSysCmd(cmd+" %o::::%C");
    pkglist.clear();
    orig.clear();
    for(int i=0; i<info.length(); i++){
      if(orig!=info[i].section("::::",0,0) && !orig.isEmpty()){ 
        HASH->insert(prefix+orig+"/categories", pkglist.join(LISTDELIMITER));
        pkglist.clear();
      }
      orig = info[i].section("::::",0,0);
      pkglist << info[i].section("::::",1,1);
    }
    HASH->insert(prefix+orig+"/categories", pkglist.join(LISTDELIMITER)); //make sure to save the last one too
    // -- options
    if(stopping){ return; }
    info = directSysCmd(cmd+" %o::::%Ok=%Ov");
    pkglist.clear();
    orig.clear();
    for(int i=0; i<info.length(); i++){
      if(orig!=info[i].section("::::",0,0) && !orig.isEmpty()){ 
        HASH->insert(prefix+orig+"/options", pkglist.join(LISTDELIMITER));
        pkglist.clear();
      }
      orig = info[i].section("::::",0,0);
      pkglist << info[i].section("::::",1,1);
    }
    HASH->insert(prefix+orig+"/options", pkglist.join(LISTDELIMITER)); //make sure to save the last one too  
    // -- licenses
    if(stopping){ return; }
    info = directSysCmd(cmd+" %o::::%L");
    pkglist.clear();
    orig.clear();
    for(int i=0; i<info.length(); i++){
      if(orig!=info[i].section("::::",0,0) && !orig.isEmpty()){ 
        HASH->insert(prefix+orig+"/license", pkglist.join(LISTDELIMITER));
        pkglist.clear();
      }
      orig = info[i].section("::::",0,0);
      pkglist << info[i].section("::::",1,1);
    }
    HASH->insert(prefix+orig+"/license", pkglist.join(LISTDELIMITER)); //make sure to save the last one too 
  } //end sync of remote information
  //Update the timestamp for this repo
  HASH->insert("Repos/"+HASH->value("Jails/"+jail+"/repoID")+"/lastSyncTimeStamp", QString::number(QDateTime::currentMSecsSinceEpoch()));
}

void Syncer::syncPkgRemote(){
  QStringList jails = HASH->value("JailList","").split(LISTDELIMITER);
  //Do the Local system first
  if(stopping){ return; }
  syncPkgRemoteJail(LOCALSYSTEM);
  //Now do any running jails
  for(int i=0; i<jails.length(); i++){
    if(stopping){ return; }
    syncPkgRemoteJail(jails[i]);
  }
}

void Syncer::syncSysStatus(){
	
}

void Syncer::syncPbi(){
  //Check the timestamp to see if it needs a re-sync
  if(needsPbiSync()){
    clearPbi();
    QStringList info = readFile("/var/db/pbi/index/PBI-INDEX");
    QStringList pbilist, catlist;
    for(int i=0; i<info.length(); i++){
      if(info[i].startsWith("PBI=")){
	//Application Information
	QStringList pbi = info[i].section("=",1,50).split("::::");
	//Line Format (7/30/14):
	// [port, name, +ports, author, website, license, app type, category, tags, 
	//      maintainer, shortdesc, fulldesc, screenshots, related, plugins, conf dir, options, rating]
	if(pbi.length()<18){ continue; } //incomplete line
	QString prefix = "PBI/"+pbi[0]+"/";
	pbilist << pbi[0]; //origin
	HASH->insert(prefix+"origin", pbi[0]);
	HASH->insert(prefix+"name", pbi[1]);
	HASH->insert(prefix+"dependencies", pbi[2].replace(",",LISTDELIMITER) );
	HASH->insert(prefix+"author", pbi[3]);
	HASH->insert(prefix+"website", pbi[4]);
	HASH->insert(prefix+"license", pbi[5].replace(",",LISTDELIMITER));
	HASH->insert(prefix+"type", pbi[6]);
	HASH->insert(prefix+"category", pbi[7]);
	HASH->insert(prefix+"tags", pbi[8].replace(",",LISTDELIMITER));
	HASH->insert(prefix+"maintainer", pbi[9]);
	HASH->insert(prefix+"comment", pbi[10].replace("<br>", " "));
	HASH->insert(prefix+"description", pbi[11].replace("<br>","\n").section("\nWWW: ",0,0) );
	HASH->insert(prefix+"screenshots", pbi[12].replace(",",LISTDELIMITER));
	HASH->insert(prefix+"relatedapps", pbi[13].replace(",",LISTDELIMITER));
	HASH->insert(prefix+"plugins", pbi[14].replace(",",LISTDELIMITER));
	HASH->insert(prefix+"confdir", "/var/db/pbi/index/"+pbi[15]);
	HASH->insert(prefix+"options", pbi[16].replace(",",LISTDELIMITER));
	HASH->insert(prefix+"rating", pbi[17]);
      }else if(info[i].startsWith("Cat=")){
	//Category Information
	QStringList cat = info[i].section("=",1,50).split("::::");
	//Line Format (7/30/14): <name>, <icon>, <summary>, <freebsd category>
	if(cat.length() < 4){ continue; } //incomplete line
	QString prefix = "PBI/cats/"+cat[3]+"/";
	catlist << cat[3]; //freebsd category (origin)
	HASH->insert(prefix+"origin", cat[3]);
	HASH->insert(prefix+"name", cat[0]);
	HASH->insert(prefix+"icon", "/var/db/pbi/index/PBI-cat-icons/"+cat[1]);
	HASH->insert(prefix+"comment", cat[2]);
      }
      //Don't use the PKG= lines, since we already have the full pkg info available
    } //finished  with index lines
    //Insert the complete lists
    HASH->insert("PBI/pbiList", pbilist.join(LISTDELIMITER));
    HASH->insert("PBI/catList", catlist.join(LISTDELIMITER));
    //Now read/save the appcafe info as well
    info = readFile("/var/db/pbi/index/AppCafe-index");
    QStringList newapps, highapps, recapps;
    for(int i=0; i<info.length(); i++){
      //Current syntax (7/30/14): <type>=<pkg origin>::::
      if(info[i].startsWith("New=")){
        newapps << info[i].section("=",1,50).section("::::",0,0);
      }else if(info[i].startsWith("Highlight=")){
	highapps << info[i].section("=",1,50).section("::::",0,0);      
      }else if(info[i].startsWith("Recommended=")){
	recapps << info[i].section("=",1,50).section("::::",0,0);
      }
    }
    //Insert the complete lists
    HASH->insert("PBI/newappList", newapps.join(LISTDELIMITER));
    HASH->insert("PBI/highappList", highapps.join(LISTDELIMITER));
    HASH->insert("PBI/recappList", recapps.join(LISTDELIMITER));
  }
  //Update the timestamp
  HASH->insert("PBI/lastSyncTimeStamp", QString::number(QDateTime::currentMSecsSinceEpoch()));
}
