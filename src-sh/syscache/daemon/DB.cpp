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
  chkTime = new QTimer(this);
	chkTime->setInterval(1000);
	chkTime->setSingleShot(true);
	connect(chkTime, SIGNAL(timeout()), this, SLOT(initialSync()) );
  watcher = new QFileSystemWatcher(this);
    connect(watcher, SIGNAL(directoryChanged(QString)), this, SLOT(watcherChange()) );
    connect(watcher, SIGNAL(fileChanged(QString)), this, SLOT(watcherChange()) );
  //Setup the watcher to look for the pc-systemflag flags
  if(!QFile::exists("/tmp/.pcbsdflags")){ directSysCmd("pc-systemflag CHECKDIR"); }
  stopping = false;
  syncing = false;
}

DB::~DB(){
  delete HASH;
}

// ===============
//    PUBLIC
// ===============
void DB::startSync(){
  stopping = false;
  QTimer::singleShot(0,this, SLOT(initialSync()));
}

void DB::shutDown(){
  stopping = true;
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
      else{ hashkey.clear(); }
    }
  }else if(request.length()==5){
    if(request[0]=="pkg"){
      if(request[2]=="local"){
        if(request[1]=="#system"){ hashkey="Jails/"+LOCALSYSTEM+"/"; }
        else{ hashkey="Jails/"+request[1]+"/"; }
        hashkey.append("pkg/"+request[3]+"/"+request[4]); // "pkg/<origin>/<variable>"
      }
    }
  }
  //Now fetch/return the info
  QString val = HASH->value(hashkey,"");
  val.replace(LISTDELIMITER, ", ");
  return val;
}

//===============
//   PRIVATE
//===============
//System Command functions 
QStringList DB::sysCmd(QString cmd){ // ensures only 1 running at a time (for things like pkg)
  //static running = false;
  //while(running){ QThread::msleep(200); } //wait until the current command finishes
  //running=true;
  QStringList out = directSysCmd(cmd);
  //running=false;
  return out;
}

QStringList DB::directSysCmd(QString cmd){ //run command immediately
   QProcess p;  
   //Make sure we use the system environment to properly read system variables, etc.
   p.setProcessEnvironment(QProcessEnvironment::systemEnvironment());
   //Merge the output channels to retrieve all output possible
   p.setProcessChannelMode(QProcess::MergedChannels);   
   p.start(cmd);
   while(p.state()==QProcess::Starting || p.state() == QProcess::Running){
     p.waitForFinished(200);
     QCoreApplication::processEvents();
   }
   QString tmp = p.readAllStandardOutput();
   if(tmp.endsWith("\n")){ tmp.chop(1); }
   return tmp.split("\n");
}

//Internal Hash maintenance functions
void DB::clearRepo(QString repo){
  //Remove All Repo specific info
  QStringList pkeys = HASH->keys();
    pkeys = pkeys.filter("Repos/"+repo+"/");
  for(int i=0; i<pkeys.length(); i++){ HASH->remove(pkeys[i]); }	
}

void DB::clearJail(QString jail){
  //Remove All Jail specific info
  QStringList pkeys = HASH->keys();
    pkeys = pkeys.filter("Jails/"+jail+"/");
  for(int i=0; i<pkeys.length(); i++){ HASH->remove(pkeys[i]); }
}

void DB::clearLocalPkg(QString pkgprefix){
  QStringList pkeys = HASH->keys();
    pkeys = pkeys.filter(pkgprefix);
  for(int i=0; i<pkeys.length(); i++){ HASH->remove(pkeys[i]); }
}

bool DB::needsLocalSync(QString jail){
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

bool DB::needsRemoteSync(QString jail){
  //Checks the pkg repo files for changes since the last sync
  if(!HASH->contains("Jails/"+jail+"/repoID")){ return true; } //no repoID yet
  else if(HASH->value("Jails/"+jail+"/repoID") != generateRepoID(jail) ){ return true; }
  else if( !HASH->contains("Repos/"+HASH->value("Jails/"+jail+"/repoID")+"/lastSyncTimeStamp") ){ return true; }
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

QString DB::generateRepoID(QString jail){
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
void DB::watcherChange(){
  if(syncing){ return; } //tons of these signals while syncing
  //qDebug() << "Watcher found change:" << 	dir;
  if(chkTime->isActive()){ chkTime->stop(); }
  chkTime->start();
}

//General Sync Functions
void DB::initialSync(){
  //just in case multiple sync calls in quick succession - check again in a few seconds
  if(syncing){ return; }
    /*qDebug() << "Sync Request";
    if(chkTime->isActive()){ qDebug() << " - stop timer"; chkTime->stop(); }
    chkTime->start(); 
    return; 
  }*/
  syncing = true;
  //qDebug() << "Syncing system information";
  //First do the operations that can potentially lock the pkg database first, but are fast
  if(stopping){ return; }
  syncJailInfo();
  if(stopping){ return; }
  syncPkgLocal();
  if(stopping){ return; }
  //Now do all the remote pkg info retrieval (won't lock the pkg database in 1.3.x?)
  syncPkgRemote();
  if(stopping){ return; }
  //Now Load the PBI database (more useful, but will not lock system usage)
  syncPbi();
  //Now check for overall system updates
  
  //qDebug() << " - Finished data sync";
  syncing = false; //done syncing
}

void DB::syncJailInfo(){
  //Get the internal list of jails
  // --clear watched directories
  if(!watcher->directories().isEmpty()){ watcher->removePaths( watcher->directories() ); }
    watcher->addPath("/var/db/pkg"); //local system pkg database should always be watched
    watcher->addPath("/tmp/.pcbsdflags"); //local PC-BSD system flags
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
    watcher->addPath(info[i].section(" ",3,3)+"/var/db/pkg"); //watch this jail's pkg database
  }
  HASH->insert("JailList", found.join(LISTDELIMITER));
  if(stopping){ return; } //catch for if the daemon is stopping
  //Remove any old jails from the hash
  for(int i=0; i<jails.length() && !stopping; i++){ //anything left over in the list
    clearJail(jails[i]); 
  }
  
}

void DB::syncPkgLocalJail(QString jail){
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
  QStringList info = directSysCmd(cmd+opt).join("").split("PKG::");
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


void DB::syncPkgLocal(){
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

void DB::syncPkgRemoteJail(QString jail){
  if(jail.isEmpty()){ return; }
  //Sync the local pkg information
  if(needsRemoteSync(jail)){
    QString repoID = generateRepoID(jail);
    HASH->insert("Jails/"+jail+"/repoID", repoID);
    //Now get all the remote pkg info for this repoID/jail
    clearRepo(repoID);
    //Remote pkg info not retrieved yet
	  
  }
  //Update the timestamp for this repo
  HASH->insert("Repos/"+HASH->value("Jails/"+jail+"/repoID")+"/lastSyncTimeStamp", QString::number(QDateTime::currentMSecsSinceEpoch()));
}

void DB::syncPkgRemote(){
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

void DB::syncSysStatus(){
	
}

void DB::syncPbi(){
	
}
