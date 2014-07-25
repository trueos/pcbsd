#include "DB.h"

/* === Representation of Database ====
  HASH -> Info: [JailList, RepoList] 
	       Categories: [Jail/<jail name>/, Repo/<repoID>]
  
  Jail Info Category -> Info: [JID, jailIP, jailPath, repoID, isUpdates, updateMessage, pkgList, lastSyncTimeStamp (internal)]
				Categories: [pkg/<local pkg origin>/]
				
  Repo Info Category -> Info: [pkgList, lastSyncTimeStamp (internal)]
				  Categories: [pkg/<remote pkg origin>/]
				  
  Local Pkg Info: [name, origin, descrption, summary, version, plist, maintainer, website, 
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
  //Remove Jail specific info
  HASH->remove("Jails/"+jail+"/JID");
  HASH->remove("Jails/"+jail+"/jailIP");
  HASH->remove("Jails/"+jail+"/jailPath");
  
  //Remove the pkg sub-category
  QStringList pkglist = HASH->value("Jails/"+jail+"/pkgList","").split(LISTDELIMITER);
  HASH->remove("Jails/"+jail+"/pkgList");
  for(int i=0; i<pkglist.length(); i++){
    clearLocalPkg("Jails/"+jail+"/pkg/"+pkglist[i]);
  }
	
}

void DB::clearRemotePkg(QString pkgprefix){
	
}

void DB::clearLocalPkg(QString pkgprefix){
	
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

void DB::syncPkgLocal(){
	
}

void DB::syncPkgRemote(){
	
}

void DB::syncSysStatus(){
	
}

void DB::syncPbi(){
	
}
