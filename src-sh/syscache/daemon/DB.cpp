#include "DB.h"

/* === Representation of Database ====
  HASH -> Info: [JailList, RepoList] 
	       Categories: [Jails/<jail name>/, Repos/<repoID>]
  
  Jail Info Category -> Info: [JID, jailIP, jailPath, RepoID, hasUpdates, updateLog, pkgList, lastSyncTimeStamp (internal)]
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
  QString repoID = HASH->value("Jail/"+myjail+"/RepoID","");
  QString name = HASH->value("Repo/"+repoID+"/"+mypkg+"/name","");
  
*/

#define LISTDELIMITER QString("::::")
#define LOCALSYSTEM QString("**LOCALSYSTEM**")
#define REBOOT_FLAG QString("/tmp/.rebootRequired")
#define UPDATE_FLAG_CHECK QString("pgrep -F /tmp/.updateInProgress") //returns 0 if active
#define PKG_REPO_FLAG QString("-r pcbsd-major ")

DB::DB(QObject *parent) : QObject(parent){
  HASH = new QHash<QString, QString>;
  SYNC = new Syncer(0, HASH);
	connect(SYNC, SIGNAL(finishedLocal()), this, SLOT(localSyncFinished()) );
	connect(SYNC, SIGNAL(finishedRemote()), this, SLOT(remoteSyncFinished()) );
	connect(SYNC, SIGNAL(finishedPBI()), this, SLOT(pbiSyncFinished()) );
	connect(SYNC, SIGNAL(finishedSystem()), this, SLOT(systemSyncFinished()) );
	connect(SYNC, SIGNAL(finishedJails()), this, SLOT(jailSyncFinished()) );
  syncThread = new QThread;
	SYNC->moveToThread(syncThread);
	syncThread->start(QThread::LowPriority); //don't slow down normal system usage for a sync
  chkTime = new QTimer(this);
	chkTime->setInterval(300000); // 5 minute delay for sync on changes
	chkTime->setSingleShot(true);
	connect(chkTime, SIGNAL(timeout()), this, SLOT(kickoffSync()) );
  maxTime = new QTimer(this);
	maxTime->setInterval(24*60*60*1000); // re-sync every 24 hours
	connect(maxTime, SIGNAL(timeout()), this, SLOT(kickoffSync()) ); 
  watcher = new QFileSystemWatcher(this);
    connect(watcher, SIGNAL(directoryChanged(QString)), this, SLOT(watcherChange(QString)) );
    connect(watcher, SIGNAL(fileChanged(QString)), this, SLOT(watcherChange(QString)) );
  //Setup the watcher to look for the pc-systemflag flags
  if(!QFile::exists("/tmp/.pcbsdflags")){ QProcess::startDetached("pc-systemflag CHECKDIR"); }
  locrun = remrun = pbirun = jrun = sysrun = false;
}

DB::~DB(){
  if(syncThread->isRunning()){ syncThread->quit(); syncThread->wait();}//make sure the sync gets stopped appropriately
  delete HASH;
  delete SYNC;
  delete syncThread;
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
  HASH->clear();
}

QString DB::fetchInfo(QStringList request){
  if(HASH->isEmpty()){ 
    startSync();
    QCoreApplication::processEvents();
    pausems(500); //wait 1/2 second for sync to start up
  }
	
  QString hashkey, searchterm, searchjail;
  int searchmin, searchfilter;
  bool sortnames = false;
  qDebug() << "Request:" << request;
  //Determine the internal hash key for the particular request
  if(request.length()==1){
    if(request[0]=="startsync"){ kickoffSync(); return "Starting Sync..."; }
    else if(request[0]=="hasupdates"){ hashkey = "System/hasUpdates"; }
    else if(request[0]=="needsreboot"){ return (QFile::exists(REBOOT_FLAG) ? "true": "false"); }
    else if(request[0]=="isupdating"){ return ( (QProcess::execute(UPDATE_FLAG_CHECK)==0) ? "true": "false"); }
    else if(request[0]=="updatelog"){ hashkey = "System/updateLog"; }
    else if(request[0]=="hasmajorupdates"){ hashkey = "System/hasMajorUpdates"; }
    else if(request[0]=="majorupdatelog"){ hashkey = "System/majorUpdateDetails"; }
    else if(request[0]=="hassecurityupdates"){ hashkey = "System/hasSecurityUpdates"; }
    else if(request[0]=="securityupdatelog"){ hashkey = "System/securityUpdateDetails"; }
    else if(request[0]=="haspcbsdupdates"){ hashkey = "System/hasPCBSDUpdates"; }
    else if(request[0]=="pcbsdupdatelog"){ hashkey = "System/pcbsdUpdateDetails"; }
  }else if(request.length()==2){
    if(request[0]=="jail"){
      if(request[1]=="list"){ hashkey = "JailList"; }
      else if(request[1]=="stoppedlist"){ hashkey = "StoppedJailList"; }
    }
    
  }else if(request.length()==3){
    if(request[0]=="jail"){
      hashkey = "Jails/"+request[1];
      if(request[2]=="id"){ hashkey.append("/JID"); }
      else if(request[2]=="ip"){ hashkey.append("/jailIP"); }
      else if(request[2]=="path"){ hashkey.append("/jailPath"); }
      else{ hashkey.append("/"+request[2]); }
    }else if(request[0]=="pkg"){
      if(request[1]=="search"){
	hashkey="Repos/"; //just to cause the request to wait for sync to finish if needed
	searchterm = request[2];
	searchjail=LOCALSYSTEM;
	searchmin = 10;
      }else if(request[1]=="#system"){ hashkey="Jails/"+LOCALSYSTEM+"/"; }
      else{ hashkey="Jails/"+request[1]+"/"; }
      if(hashkey.startsWith("Jails/")){
        if(request[2]=="installedlist"){ hashkey.append("pkgList"); sortnames=true;}
        else if(request[2]=="hasupdates"){ hashkey.append("hasUpdates"); }
        else if(request[2]=="updatemessage"){ hashkey.append("updateLog"); }
        else if(request[2]=="remotelist"){ hashkey="Repos/"+HASH->value(hashkey+"RepoID")+"/pkgList"; }
        else{ hashkey.clear(); }
      }
    }else if(request[0]=="pbi"){
      if(request[1]=="list"){
        if(request[2]=="allapps"){ hashkey="PBI/pbiList"; sortnames=true;}
	else if(request[2]=="graphicalapps"){ hashkey="PBI/graphicalAppList"; sortnames=true;}
	else if(request[2]=="textapps"){ hashkey="PBI/textAppList"; sortnames=true;}
	else if(request[2]=="serverapps"){ hashkey="PBI/serverAppList"; sortnames=true;}
	else if(request[2]=="allcats"){ hashkey = "PBI/catList"; }
	else if(request[2]=="graphicalcats"){ hashkey = "PBI/graphicalCatList"; }
	else if(request[2]=="textcats"){ hashkey = "PBI/textCatList"; }
	else if(request[2]=="servercats"){ hashkey = "PBI/serverCatList"; }
	else if(request[2]=="new"){ hashkey = "PBI/newappList"; sortnames=true;}
	else if(request[2]=="highlighted"){ hashkey = "PBI/highappList"; sortnames=true;}
	else if(request[2]=="recommended"){ hashkey = "PBI/recappList"; sortnames=true;}
      }else if(request[1]=="search"){
	hashkey="PBI/"; //just to cause the request to wait for sync to finish if needed
	searchterm = request[2];
	searchjail = "pbi";
	searchmin = 10;
	searchfilter=0; //all
      }		
    }
  }else if(request.length()==4){
    if(request[0]=="pbi"){
      if(request[1]=="app"){
        hashkey = "PBI/"+request[2]+"/"+request[3]; //pkg origin and variable
      }else if(request[1]=="cat"){
	hashkey = "PBI/cats/"+request[2]+"/"+request[3]; //pkg origin and variable
      }else if(request[1]=="search"){
	hashkey="PBI/"; //just to cause the request to wait for sync to finish if needed
	searchterm = request[2];
	searchjail = "pbi";
	searchmin = 10;
	if(request[3]=="graphical"){ searchfilter=1; }
	else if(request[3]=="server"){ searchfilter=2;}
	else if(request[3]=="text"){ searchfilter=3;}
	else if(request[3]=="notgraphical"){ searchfilter=-1;}
	else if(request[3]=="notserver"){ searchfilter=-2;}
	else if(request[3]=="nottext"){ searchfilter=-3;}
	else{ searchfilter=0; }//all
      }
    }else if(request[0]=="pkg"){
      if(request[1]=="search"){
	hashkey="Repos/"; //just to cause the request to wait for sync to finish if needed
	searchterm = request[2];
	searchjail = request[3];
	if(searchjail=="#system"){searchjail=LOCALSYSTEM;}
	searchmin = 10;
      }
    }
  }else if(request.length()==5){
    if(request[0]=="pkg"){
      if(request[1]=="search"){
	hashkey="Repos/"; //just to cause the request to wait for sync to finish if needed
	searchterm = request[2];
	searchjail = request[3];
	if(searchjail=="#system"){searchjail=LOCALSYSTEM;}
	bool ok;
	searchmin = request[4].toInt(&ok);
	if(!ok){searchmin = 10;}
      }
      else if(request[1]=="#system"){ hashkey="Jails/"+LOCALSYSTEM+"/"; }
      else{ hashkey="Jails/"+request[1]+"/"; }	
      if(hashkey.startsWith("Jails/")){
        if(request[2]=="local"){
          hashkey.append("pkg/"+request[3]+"/"+request[4]); // "pkg/<origin>/<variable>"
        }else if(request[2]=="remote"){
	  hashkey="Repos/"+HASH->value(hashkey+"RepoID")+"/pkg/"+request[3]+"/"+request[4];
        }
      }
    }else if(request[0]=="pbi"){
      if(request[1]=="search"){
	hashkey="PBI/"; //just to cause the request to wait for sync to finish if needed
	searchterm = request[2];
	searchjail = "pbi";
	if(request[3]=="graphical"){ searchfilter=1; }
	else if(request[3]=="server"){ searchfilter=2;}
	else if(request[3]=="text"){ searchfilter=3;}
	else if(request[3]=="notgraphical"){ searchfilter=-1;}
	else if(request[3]=="notserver"){ searchfilter=-2;}
	else if(request[3]=="nottext"){ searchfilter=-3;}
	else{ searchfilter=0; }//all
	bool ok;
	searchmin = request[4].toInt(&ok);
	if(!ok){searchmin = 10;}
      }
    }
  }
  //qDebug() << "Request Key:" << hashkey;
  //Now fetch/return the info
  QString val;
  if(hashkey.isEmpty()){ val = "[ERROR] Invalid Information request: \""+request.join(" ")+"\""; }
  else{
    //Check if a sync is running and wait a moment until it is done
    while(isRunning(hashkey)){ pausems(500); } //re-check every 500 ms
    //Now check for info availability
    if(!searchterm.isEmpty()){
      val = doSearch(searchterm,searchjail, searchmin, searchfilter).join(LISTDELIMITER);
    }
    else if(!HASH->contains(hashkey)){ val = "[ERROR] Information not available"; }
    else{
      val = HASH->value(hashkey,"");
      if(sortnames && !val.isEmpty()){ val = sortByName(val.split(LISTDELIMITER)).join(LISTDELIMITER); }
    }
    val.replace(LISTDELIMITER, ", ");
    if(val.isEmpty()){ val = " "; } //make sure it has a blank space at the minimum
  }
  return val;
}

// ========
//   PRIVATE
// ========
//Search the hash for matches
QStringList DB::doSearch(QString srch, QString jail, int findmin, int filter){
  //Filter Note: [0=all, 1=graphical, -1=!graphical, 2=server, -2=!server, 3=text, -3=!text]
  QStringList out, raw;
  QString prefix;
  if(jail.toLower()=="pbi"){
    //Get the initial list by filter
    switch(filter){
      case 1:
	raw = HASH->value("PBI/graphicalAppList").split(LISTDELIMITER);
        //qDebug() << "Search Graphical apps:" << raw.length();
        break;
      case 2:
	raw = HASH->value("PBI/serverAppList","").split(LISTDELIMITER);
        //qDebug() << "Search Server apps:" << raw.length();
        break;
      case 3:
	raw = HASH->value("PBI/textAppList","").split(LISTDELIMITER);
        //qDebug() << "Search Text apps:" << raw.length();
        break;
      case -1:
	raw = HASH->value("PBI/serverAppList","").split(LISTDELIMITER);
        raw << HASH->value("PBI/textAppList","").split(LISTDELIMITER);
        //qDebug() << "Search non-Graphical apps:" << raw.length();
        break;
      case -2:
	raw = HASH->value("PBI/graphicalAppList","").split(LISTDELIMITER);
        raw << HASH->value("PBI/textAppList","").split(LISTDELIMITER);
        //qDebug() << "Search non-Server apps:" << raw.length();
        break;
      case -3:
	raw = HASH->value("PBI/serverAppList","").split(LISTDELIMITER);
        raw << HASH->value("PBI/graphicalAppList","").split(LISTDELIMITER);
        //qDebug() << "Search non-Text apps:" << raw.length();
        break;
      default:
	raw = HASH->value("PBI/pbiList","").split(LISTDELIMITER);
        //qDebug() << "Search All apps:" << raw.length();
        break;      
    }
    prefix = "PBI/";
  }else{
    //pkg search - no type filter available
    prefix = "Repos/"+HASH->value("Jails/"+jail+"/RepoID","")+"/";
    raw = HASH->value(prefix+"pkgList","").split(LISTDELIMITER);
    prefix.append("pkg/");
    //qDebug() << "Pkg Search:" << prefix << raw.length();
  }
  //qDebug() << "Search For Term:" << srch << raw.length();
  //Now perform the search on the raw list
  if(!raw.isEmpty()){
    QStringList found;
    QStringList exact;
    // - name
    found = raw.filter("/"+srch, Qt::CaseInsensitive);
    if(!found.isEmpty()){
      //Also check for an exact name match and pull that out
      for(int i=0; i<found.length(); i++){
        if(found[i].endsWith("/"+srch, Qt::CaseInsensitive)){ exact << found.takeAt(i); i--;}
      }
      found = sortByName(found);
    }
    // - If not enough matches, also loop through and look for tag/description matches
    if( (found.length()+exact.length()) < findmin){
      QStringList words = srch.split(" ");
      QStringList tagM, sumM, descM, nameM; //tag/summary/desc/name matches
      for(int i=0; i<raw.length(); i++){
        if(exact.contains(raw[i]) || found.contains(raw[i])){ continue; } //already found
	//Also account for multiple-work searches
	//  - Names
	QString tmp = HASH->value(prefix+raw[i]+"/name","");
	if(tmp.contains(srch, Qt::CaseInsensitive)){ nameM << "100::::"+raw[i]; }
	else if(words.length()>1){
	  int wrdcount = 0;
	  for(int j=0; j<words.length(); j++){
	    if(tmp.contains(words[j], Qt::CaseInsensitive)){ wrdcount++; }
	  }
	  if(wrdcount>0){nameM << QString::number(wrdcount)+"::::"+raw[i]; }
	}
	//  - Tags
	tmp = HASH->value(prefix+raw[i]+"/tags","");
	if(tmp.contains(srch, Qt::CaseInsensitive)){ tagM << "100::::"+raw[i]; }
	else if(words.length()>1){
	  int wrdcount = 0;
	  for(int j=0; j<words.length(); j++){
	    if(tmp.contains(words[j], Qt::CaseInsensitive)){ wrdcount++; }
	  }
	  if(wrdcount>0){tagM << QString::number(wrdcount)+"::::"+raw[i]; }
	}
	//  - Comment
	tmp = HASH->value(prefix+raw[i]+"/comment","");
	if(tmp.contains(srch, Qt::CaseInsensitive)){ sumM << "100::::"+raw[i]; }
	else if(words.length()>1){
	  int wrdcount = 0;
	  for(int j=0; j<words.length(); j++){
	    if(tmp.contains(words[j], Qt::CaseInsensitive)){ wrdcount++; }
	  }
	  if(wrdcount>0){sumM << QString::number(wrdcount)+"::::"+raw[i]; }
	}
	//  - Description
	tmp = HASH->value(prefix+raw[i]+"/description","");
	if(tmp.contains(srch, Qt::CaseInsensitive)){ descM << "100::::"+raw[i]; }
	else if(words.length()>1){
	  int wrdcount = 0;
	  for(int j=0; j<words.length(); j++){
	    if(tmp.contains(words[j], Qt::CaseInsensitive)){ wrdcount++; }
	  }
	  if(wrdcount>0){descM << QString::number(wrdcount)+"::::"+raw[i]; }
	}
	/*if(HASH->value(prefix+raw[i]+"/name","").contains(srch, Qt::CaseInsensitive) ){ nameM << "100::::"+raw[i]; }
	else if(HASH->value(prefix+raw[i]+"/tags","").contains(srch, Qt::CaseInsensitive)){ tagM << "100::::"+raw[i]; }
	else if(HASH->value(prefix+raw[i]+"/comment","").contains(srch, Qt::CaseInsensitive) ){ sumM << "100::::"+raw[i]; }
	else if(HASH->value(prefix+raw[i]+"/description","").contains(srch, Qt::CaseInsensitive) ){ descM << "100::::"+raw[i]; }
	*/
      }
      // - Now add them to the found list by priority (tags > summary > description)
      found << sortByName(nameM, true);
      if( (found.length()+exact.length())<findmin){ found << sortByName(tagM, true); }
      if( (found.length()+exact.length())<findmin){ found << sortByName(sumM,true); }
      if( (found.length()+exact.length())<findmin){ found << sortByName(descM,true); }
    }
    //Sort the found list by name
    //Add the exact matches back to the top of the output list
    if(!exact.isEmpty()){ out << exact; }
    out << found;
  }
  //Make sure we don't return duplicate results
  out.removeDuplicates();
  return out;
}


//Sort a list of pkg origins by name
QStringList DB::sortByName(QStringList origins, bool haspriority){
  if(haspriority){
    //This is another recursive layer for sorting, origins should be "<priority number>::::<origin>"
    QStringList orgs = origins;
    origins.clear();
    orgs.sort();
    while( !orgs.isEmpty() ){
      QStringList priority = orgs.filter(orgs[0].section("::::",0,0)+"::::");
      for(int j=0; j<priority.length(); j++){
        orgs.removeAll(priority[j]);
	priority[j] = priority[j].section("::::",1,50); //now return it to just the origin (strip off the priority)
      }
      if(!priority.isEmpty()){
        origins << sortByName(priority, false); //now do the name sorting
      }
    }
  }else{
    QStringList names  = origins;
    for(int i=0; i<origins.length(); i++){ origins[i] = origins[i].section("/",-1)+":::"+origins[i]; }
    origins.sort();
    for(int i=0; i<origins.length(); i++){ origins[i] = origins[i].section(":::",1,1); }
  }
  return origins;
}


//Internal pause/syncing functions
bool DB::isRunning(QString key){
  if(!sysrun && !jrun){ return false; } //no sync going on - all info available
  //A sync is running - check if the current key falls into a section not finished yet
  if(key.startsWith("Jails/")){ return locrun; } //local sync running
  else if(key.startsWith("Repos/")){ return remrun; } //remote sync running
  else if(key.startsWith("PBI/")){ return pbirun; } //pbi sync running
  else if(key.startsWith("System/")){ return sysrun; } //system sync running
  else{ return jrun; }
  //sysrun not used (yet)
}

void DB::pausems(int ms){
  //pause the calling function for a few milliseconds
  QTime time = QTime::currentTime().addMSecs(ms);
  int udiv = ms*100; //cut into 10 parts for checks (microseconds)
  while(QTime::currentTime() < time){
    QCoreApplication::processEvents();
     QObject().thread()->usleep(udiv);
  }
}

void DB::writeToLog(QString message){
  QFile file("/var/log/pc-syscache.log");
    if(file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append) ){
      QTextStream out(&file);
	out << message +"\n";
      file.close();
    }
}
// ============
//   PRIVATE SLOTS
// ============
void DB::watcherChange(QString change){
  //Tons of these signals while syncing
    //   - so use a QTimer to compress them all down to a single call (within a short time frame)
  bool now = false;
  //Check if this is the special flag to resync now
  if(change.startsWith("/tmp/.pcbsdflags")){
    QDir dir("/tmp/.pcbsdflags");
     QFileInfoList list = dir.entryInfoList(QStringList() << "syscache-sync-*", QDir::Files | QDir::NoDotAndDotDot, QDir::Time);
     QDateTime ctime = QDateTime::currentDateTime().addSecs(-2); // go back 2 seconds
     for(int i=0; i<list.length(); i++){
       if(list[i].created() > ctime || list[i].lastModified() > ctime){ now = true; break; }
     }
  }

  if(change.contains("/var/db/pkg") && locrun){ return; } //Local sync running - ignore these for the moment
  QString log = "Watcher Ping: "+change+" -> Sync "+ (now ? "Now": "in 5 Min");
  writeToLog(log);
  if(!now){
    //General pkg/system change - use the timer before resync
   if(chkTime->isActive()){ chkTime->stop(); } //reset back to full time
    chkTime->start();
  }else{
    //Special pc-systemflag change: resync now
    kickoffSync();
  }
  
}

void DB::kickoffSync(){
  if(sysrun){ return; } //already running a sync (sysrun is the last one to be finished)
  writeToLog("Starting Sync: "+QDateTime::currentDateTime().toString(Qt::ISODate) );
  locrun = remrun = pbirun = jrun = sysrun = true; //switch all the flags to running
  if(!syncThread->isRunning()){ syncThread->start(); } //make sure the other thread is running
  QTimer::singleShot(0,SYNC, SLOT(performSync()));
}

void DB::jailSyncFinished(){ 
  jrun = false; 
  writeToLog(" - Jail Sync Finished:"+QDateTime::currentDateTime().toString(Qt::ISODate));
  //Also reset the list of watched jails
  QStringList jails = watcher->directories().filter("/var/db/pkg");
  jails.removeAll("/var/db/pkg"); //don't remove the local pkg dir - just the jails
  if(!jails.isEmpty()){ watcher->removePaths(jails); }
  jails = HASH->value("JailList").split(LISTDELIMITER);
  for(int i=0; i<jails.length(); i++){
    //qDebug() << "Start Watching Jail:" << jails[i];
    watcher->addPath(HASH->value("Jails/"+jails[i]+"/jailPath")+"/var/db/pkg"); //watch this jail's pkg database
  }
  //qDebug() << "Watcher paths:" << watcher->directories();
}

//****************************************
//    SYNCER CLASS
//****************************************

Syncer::Syncer(QObject *parent, QHash<QString,QString> *hash) : QObject(parent){
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
   //QTimer time(this);
    //time.setSingleShot(true);
    //time.start(5000); //5 second timeout
   while(p.state()==QProcess::Starting || p.state() == QProcess::Running){
     /*if(!time.isActive()){
       p.terminate(); //hung process - kill it
     }*/
     p.waitForFinished(500); //1/2 second timeout check
     QCoreApplication::processEvents();
     if(stopping){break;}
   }
   //if(time.isActive()){ time.stop(); }
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
    
    if(jail==LOCALSYSTEM){ 
      QString path = "/var/db/pkg/local.sqlite";
      qint64 mod = QFileInfo(path).lastModified().toMSecsSinceEpoch();
      qint64 stamp = HASH->value("Jails/"+jail+"/lastSyncTimeStamp","").toLongLong();
      if(mod > stamp){ return true; }//was it modified after the last sync?
      //Otherwise check if the installed pkg list if different (sometimes timestamps don't get updated properly on files)
      return (HASH->value("Jails/"+jail+"/pkgList","") != directSysCmd("pkg query -a %o").join(LISTDELIMITER) );
    }else{
      //This is inside a jail - need different method
      QString path = HASH->value("Jails/"+jail+"/jailPath","") + "/var/db/pkg/local.sqlite";
      qint64 mod = QFileInfo(path).lastModified().toMSecsSinceEpoch();
      qint64 stamp = HASH->value("Jails/"+jail+"/lastSyncTimeStamp","").toLongLong();
      if(mod > stamp){ return true; }//was it modified after the last sync?
      //Otherwise check if the installed pkg list if different (sometimes timestamps don't get updated properly on files)
      return (HASH->value("Jails/"+jail+"/pkgList","") != directSysCmd("pkg -j "+HASH->value("Jails/"+jail+"/JID","")+" query -a %o").join(LISTDELIMITER) );
    }
  }
}

bool Syncer::needsRemoteSync(QString jail){
  //Checks the pkg repo files for changes since the last sync
  if(!HASH->contains("Jails/"+jail+"/RepoID")){ return true; } //no repoID yet
  else if(HASH->value("Jails/"+jail+"/RepoID") != generateRepoID(jail) ){ return true; } //repoID changed
  else if( !HASH->contains("Repos/"+HASH->value("Jails/"+jail+"/RepoID")+"/lastSyncTimeStamp") ){ return true; } //Repo Never synced
  else{
    QDir pkgdb( HASH->value("Jails/"+jail+"/jailPath","")+"/var/db/pkg" );
    QFileInfoList repos = pkgdb.entryInfoList(QStringList() << "repo-*.sqlite");
    qint64 stamp = HASH->value("Repos/"+HASH->value("Jails/"+jail+"/RepoID")+"/lastSyncTimeStamp").toLongLong();
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
    qint64 dayago = QDateTime::currentDateTime().addDays(-1).toMSecsSinceEpoch();
    return (mod > stamp || stamp < dayago );
  }
  
}

bool Syncer::needsSysSync(){
  //Check how log the 
  if(!HASH->contains("System/lastSyncTimeStamp")){ return true; }
  else{
    qint64 stamp = HASH->value("System/lastSyncTimeStamp").toLongLong();
    qint64 dayago = QDateTime::currentDateTime().addDays(-1).toMSecsSinceEpoch();
    return ( stamp < dayago );
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
  ID.remove("\"");
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
  qDebug() << " - Starting Jail Sync:" << QDateTime::currentDateTime().toString(Qt::ISODate);
  syncJailInfo();
  emit finishedJails();
  qDebug() << "   - Jails done";
  if(stopping){ return; }
  qDebug() << " - Starting Local Pkg Sync";
  syncPkgLocal();
  emit finishedLocal();
  qDebug() << "   - Local done";
  if(stopping){ return; }
  //Now Load the PBI database (more useful, will not lock system usage, and is fast)
  qDebug() << " - Starting PBI Sync";
  syncPbi();
  qDebug() << "   - PBI done";
  emit finishedPBI();
  //Now do all the remote pkg info retrieval (won't lock the pkg database in 1.3.x?)
   // Note: This can take a little while
  qDebug() << " - Starting Remote Pkg Sync";
  syncPkgRemote();
  qDebug() << "   - Remote done";
  emit finishedRemote();
  if(stopping){ return; }
  //Now check for overall system updates (not done yet)
  qDebug() << " - Starting System Sync";
  syncSysStatus();
  emit finishedSystem();
  qDebug() << "  - Finished all syncs";
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
  //Now also fetch the list of inactive jails on the system
  info = directSysCmd("warden list -v");
  QStringList inactive;
  info = info.join("----").simplified().split("id: ");
  //qDebug() << "Warden Jail Info:" << info;
  for(int i=0; i<info.length(); i++){
    if(info[i].isEmpty()){ continue; }
    QStringList tmp = info[i].split("----");
    //qDebug() << "tmp:" << tmp;
    //Create the info strings possible
    QString ID, HOST, IPV4, AIPV4, BIPV4, ABIPV4, ROUTERIPV4, IPV6, AIPV6, BIPV6, ABIPV6, ROUTERIPV6, AUTOSTART, VNET, TYPE;
    bool isRunning = false;
    for(int j=0; j<tmp.length(); j++){
      //Now iterate over all the info for this single jail
      if(j==0 && !tmp[j].contains(":")){ ID = tmp[j].simplified(); }
      else if(tmp[j].startsWith("host:")){ HOST = tmp[j].section(":",1,50).simplified(); }
      else if(tmp[j].startsWith("ipv4:")){ IPV4 = tmp[j].section(":",1,50).simplified(); }
      else if(tmp[j].startsWith("alias-ipv4:")){ AIPV4 = tmp[j].section(":",1,50).simplified(); }
      else if(tmp[j].startsWith("bridge-ipv4:")){ BIPV4 = tmp[j].section(":",1,50).simplified(); }
      else if(tmp[j].startsWith("alias-bridge-ipv4:")){ ABIPV4 = tmp[j].section(":",1,50).simplified(); }
      else if(tmp[j].startsWith("defaultrouter-ipv4:")){ ROUTERIPV4 = tmp[j].section(":",1,50).simplified(); }
      else if(tmp[j].startsWith("ipv6:")){ IPV6 = tmp[j].section(":",1,50).simplified(); }
      else if(tmp[j].startsWith("alias-ipv6:")){ AIPV6 = tmp[j].section(":",1,50).simplified(); }
      else if(tmp[j].startsWith("bridge-ipv6:")){ BIPV6 = tmp[j].section(":",1,50).simplified(); }
      else if(tmp[j].startsWith("alias-bridge-ipv6:")){ ABIPV6 = tmp[j].section(":",1,50).simplified(); }
      else if(tmp[j].startsWith("defaultrouter-ipv6:")){ ROUTERIPV6 = tmp[j].section(":",1,50).simplified(); }
      else if(tmp[j].startsWith("autostart:")){ AUTOSTART = (tmp[j].section(":",1,50).simplified()=="Enabled") ? "true" : "false"; }
      else if(tmp[j].startsWith("vnet:")){ VNET = tmp[j].section(":",1,50).simplified(); }
      else if(tmp[j].startsWith("type:")){ TYPE = tmp[j].section(":",1,50).simplified(); }
      else if(tmp[j].startsWith("status:")){ isRunning = (tmp[j].section(":",1,50).simplified() == "Running"); }
    }
    if(!HOST.isEmpty()){
      //Save this info into the hash
      QString prefix = "Jails/"+HOST+"/";
      if(!isRunning){ inactive << HOST; } //only save inactive jails - active are already taken care of
      HASH->insert(prefix+"WID", ID); //Warden ID
      HASH->insert(prefix+"ipv4", IPV4);
      HASH->insert(prefix+"alias-ipv4", AIPV4);
      HASH->insert(prefix+"bridge-ipv4", BIPV4);
      HASH->insert(prefix+"alias-bridge-ipv4", ABIPV4);
      HASH->insert(prefix+"defaultrouter-ipv4", ROUTERIPV4);
      HASH->insert(prefix+"ipv6", IPV6);
      HASH->insert(prefix+"alias-ipv6", AIPV6);
      HASH->insert(prefix+"bridge-ipv6", BIPV6);
      HASH->insert(prefix+"alias-bridge-ipv6", ABIPV6);
      HASH->insert(prefix+"defaultrouter-ipv6", IPV6);
      HASH->insert(prefix+"autostart", AUTOSTART);
      HASH->insert(prefix+"vnet", VNET);
      HASH->insert(prefix+"type", TYPE);      
    }
  }
  HASH->insert("StoppedJailList",inactive.join(LISTDELIMITER));
  
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
    HASH->insert(prefix+line[0]+"/description", line[5].replace("\n","<br>").section("WWW: ",0,0));
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
  QString cmd = "pkg upgrade -n";
  if(jail!=LOCALSYSTEM){ cmd = "pkg -j "+HASH->value("Jails/"+jail+"/JID")+" upgrade -n"; }
  QString log = directSysCmd(cmd).join("<br>");
  HASH->insert("Jails/"+jail+"/updateLog", log);
  if(log.contains("Your packages are up to date")){ HASH->insert("Jails/"+jail+"/hasUpdates", "false"); }
  else{ HASH->insert("Jails/"+jail+"/hasUpdates", "true"); }
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
    //qDebug() << "Sync Remote Jail:" << jail << repoID;
    HASH->insert("Jails/"+jail+"/RepoID", repoID);
    //Now fetch remote pkg info for this repoID
    QString prefix = "Repos/"+repoID+"/pkg/";
    QString cmd = "pkg rquery -a " + PKG_REPO_FLAG;
    if(jail!=LOCALSYSTEM){ cmd = "pkg -j "+HASH->value("Jails/"+jail+"/JID")+" rquery -a " + PKG_REPO_FLAG; }
    QStringList info = directSysCmd(cmd+"PKG::%o::::%n::::%v::::%m::::%w::::%q::::%sh::::%c::::%e::::%M").join("\n").split("PKG::");
    if(info.length() < 3){
      qDebug() << "[ERROR] Remote info fetch for jail:" << jail<<"\n"<<info;
      return;
    }
    //qDebug() << "Info:" << info;
    //Format: origin, name, version, maintainer, website, arch, size, comment, description, message
    QStringList pkglist;
    clearRepo(repoID); //valid info found
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
      HASH->insert(prefix+pkg[0]+"/description", pkg[8].replace("\n","<br>").section("WWW: ",0,0));
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
  HASH->insert("Repos/"+HASH->value("Jails/"+jail+"/RepoID")+"/lastSyncTimeStamp", QString::number(QDateTime::currentMSecsSinceEpoch()));
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
  if(needsSysSync()){
    QStringList info = directSysCmd("pc-updatemanager check");
    //Save the raw output for later
    HASH->insert("System/updateLog", info.join("<br>"));
    //Determine the number/types of updates listed
    QStringList ups;
    QString cup;
    for(int i=0; i<info.length(); i++){
      if(cup.isEmpty() && info[i].contains("NAME: ") ){
	 //Starting a new update 
	 cup = info[i];     
      }else if(!cup.isEmpty()){
	//in the middle up an update - add the text
	cup.append("\n"+info[i]);
	if(info[i].contains("To install: ") || info[i].startsWith("Install: \"") ){ //last line of text for this update
	  ups << cup; //save the complete entry to the array
	  cup.clear(); //Clear it for the next one
	}
      }
    }
    //Now go through all the types of update and set flags appropriately
    // - Major system updates (10.0 -> 10.1 for example)
    QStringList tmp = ups.filter("TYPE: SYSTEMUPDATE");
    HASH->insert("System/hasMajorUpdates", !tmp.isEmpty() ? "true": "false" );
    HASH->insert("System/majorUpdateDetails", tmp.join("\n----------\n").replace("\n","<br>") );
    // - (Ignore package updates  - already taken care of with pkg details itself)
    tmp = ups.filter("TYPE: PKGUPDATE");
    for(int i=0; i<tmp.length(); i++){
      ups.removeAll(tmp[i]); //Remove these updates from the total list
    }
    // - Freebsd/security updates
    tmp = ups.filter("TYPE: SECURITYUPDATE");
    HASH->insert("System/hasSecurityUpdates", !tmp.isEmpty() ? "true": "false" );
    HASH->insert("System/securityUpdateDetails", tmp.join("\n----------\n").replace("\n","<br>") );
    // - PC-BSD patches
    tmp = ups.filter("TYPE: PATCH");
    HASH->insert("System/hasPCBSDUpdates", !tmp.isEmpty() ? "true": "false" );
    HASH->insert("System/pcbsdUpdateDetails", tmp.join("\n----------\n").replace("\n","<br>") );

    //Now save whether updates are available
    bool hasupdates = ups.length() > 0;
    HASH->insert("System/hasUpdates", hasupdates ? "true": "false" );

    //Now save the last time this was updated
    HASH->insert("System/lastSyncTimeStamp", QString::number(QDateTime::currentMSecsSinceEpoch()) );
  }
}

void Syncer::syncPbi(){
  //Check the timestamp to see if it needs a re-sync
  if(needsPbiSync()){
    directSysCmd("pbi_updateindex"); //Make sure to update it
    clearPbi();
    QStringList info = readFile("/var/db/pbi/index/PBI-INDEX");
    if(info.length() < 5){ return; } //exit without saving a timestamp - did not get index
    QStringList pbilist, catlist;
    QStringList gcats, tcats, scats; //graphical/text/server categories
    QStringList gapps, tapps, sapps; //graphical/text/server apps
    for(int i=0; i<info.length(); i++){
      if(info[i].startsWith("PBI=")){
	//Application Information
	QStringList pbi = info[i].section("=",1,200).split("::::");
	//Line Format (7/30/14):
	// [port, name, +ports, author, website, license, app type, category, tags, 
	//      maintainer, shortdesc, fulldesc, screenshots, related, plugins, conf dir, options, rating]
	if(pbi.length()<18){ 
	  //qDebug() << "Invalid PBI Line:" << info[i];
	  //qDebug() << " - Length:" << pbi.length() << pbi;
		continue; } //incomplete line
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
	HASH->insert(prefix+"description", pbi[11].section("\nWWW: ",0,0) );
	HASH->insert(prefix+"screenshots", pbi[12].replace(",",LISTDELIMITER));
	HASH->insert(prefix+"relatedapps", pbi[13].replace(",",LISTDELIMITER));
	HASH->insert(prefix+"plugins", pbi[14].replace(",",LISTDELIMITER));
	HASH->insert(prefix+"confdir", "/var/db/pbi/index/"+pbi[15]);
	HASH->insert(prefix+"options", pbi[16].replace(",",LISTDELIMITER));
	HASH->insert(prefix+"rating", pbi[17]);
	//Keep track of which category this type falls into
	if(pbi[6].toLower()=="graphical"){ gcats << pbi[0].section("/",0,0); gapps << pbi[0]; }
	else if(pbi[6].toLower()=="server"){ scats << pbi[0].section("/",0,0); sapps << pbi[0]; }
	else{ tcats << pbi[0].section("/",0,0); tapps << pbi[0]; }
	
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
    //Now setup the category lists
    gcats.removeDuplicates(); gcats.sort();
    tcats.removeDuplicates(); tcats.sort();
    scats.removeDuplicates(); scats.sort();
    HASH->insert("PBI/graphicalCatList",gcats.join(LISTDELIMITER));
    HASH->insert("PBI/textCatList",tcats.join(LISTDELIMITER));
    HASH->insert("PBI/serverCatList",scats.join(LISTDELIMITER));
    HASH->insert("PBI/graphicalAppList",gapps.join(LISTDELIMITER));
    HASH->insert("PBI/textAppList",tapps.join(LISTDELIMITER));
    HASH->insert("PBI/serverAppList",sapps.join(LISTDELIMITER));
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
