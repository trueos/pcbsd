#include "DB.h"

/* === Representation of Database ====
  HASH -> Info: [JailList, RepoList] 
	       Categories: [Jail/<jail name>/, Repo/<repoID>]
  
  Jail Info Category -> Info: [JID, jailIP, jailPath, repoID, isUpdates, updateMessage, pkgList, lastSyncTimeStamp (internal)]
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
  stopping = false;
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
	
}

void DB::clearJail(QString jail){
  //Remove All Jail specific info
  QStringList pkeys = HASH->keys();
    pkeys = pkeys.filter("Jails/"+jail+"/");
  for(int i=0; i<pkeys.length(); i++){ HASH->remove(pkeys[i]); }
}

void DB::clearRemotePkg(QString pkgprefix){
	
}

void DB::clearLocalPkg(QString pkgprefix){
  QStringList pkeys = HASH->keys();
    pkeys = pkeys.filter(pkgprefix);
  for(int i=0; i<pkeys.length(); i++){ HASH->remove(pkeys[i]); }
}

//===============
//  PRIVATE SLOTS
//===============
//General Sync Functions
void DB::initialSync(){
  //First do the operations that can potentially lock the pkg database first, but are fast
  syncJailInfo();
  syncPkgLocal();
  //Now do all the remote pkg info retrieval (won't lock the pkg database in 1.3.x?)
  syncPkgRemote();
  //Now Load the PBI database (more useful, but will not lock system usage)
  syncPbi();
  //Now check for overall system updates
}

void DB::syncJailInfo(){
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

void DB::syncPkgLocalJail(QString jail){
  if(jail.isEmpty()){ return; }
  QString prefix = "Jails/"+jail+"/pkg/";
  clearLocalPkg(prefix); //clear the old info from the hash
  //Format: origin, name, version, maintainer, comment, description, website, size, arch, timestamp, message, isOrphan, isLocked
  QString cmd = "pkg query -a";
  QString opt = " PKG::%o::::%n::::%v::::%m::::%c::::%e::::%w::::%sh::::%q::::%t::::%M::::%a::::%k";
  if(jail!=LOCALSYSTEM){
    cmd.replace("pkg ", "pkg -j "+HASH->value("Jails/"+jail+"/JID")+" ");
  }
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
  //Now Get jail update status/info
  cmd = "pc-updatemanager pkgcheck";
  if(jail!=LOCALSYSTEM){ cmd = "pc-updatemanager -j "+jail+" pkgcheck"; }
  QString log = directSysCmd(cmd).join("\n");
  HASH->insert("Jails/"+jail+"/updateLog", log);
  if(log.contains("To start the upgrade run ")){ HASH->insert("Jails/"+jail+"/hasUpdates", "true"); }
  else{ HASH->insert("Jails/"+jail+"/hasUpdates", "false"); }
}

void DB::syncPkgLocal(){
  QStringList jails = HASH->value("JailList","").split(LISTDELIMITER);
  //Do the Local system first
  syncPkgLocalJail(LOCALSYSTEM);
  //Now do any running jails
  for(int i=0; i<jails.length(); i++){
    syncPkgLocalJail(jails[i]);
  }
}

void DB::syncPkgRemote(){
	
}

void DB::syncSysStatus(){
	
}

void DB::syncPbi(){
	
}
