/***************************************************************************
 *   Copyright (C) 2011 - iXsystems				 	  *
 *   kris@pcbsd.org  *
 *   tim@pcbsd.org   *
 *   ken@pcbsd.org   *
 *                                                                         *
 *   Permission is hereby granted, free of charge, to any person obtaining *
 *   a copy of this software and associated documentation files (the       *
 *   "Software"), to deal in the Software without restriction, including   *
 *   without limitation the rights to use, copy, modify, merge, publish,   *
 *   distribute, sublicense, and/or sell copies of the Software, and to    *
 *   permit persons to whom the Software is furnished to do so, subject to *
 *   the following conditions:                                             *
 *                                                                         *
 *   The above copyright notice and this permission notice shall be        *
 *   included in all copies or substantial portions of the Software.       *
 *                                                                         *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       *
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    *
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. *
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR     *
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, *
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR *
 *   OTHER DEALINGS IN THE SOFTWARE.                                       *
 ***************************************************************************/
 //ID FORMATS: [<name> = Extras::nameToID(formalName)]
 //  pbiID: <name>-<version>-<arch> (of locally installed PBI)
 //  catID: <name> (of category in repo)
 //  appID: <name> (of app in repo)
 
 #include <unistd.h>
 #include "pbiBackend.h"

 
 PBIBackend::PBIBackend(QWidget *parent) : QObject(){
   parentWidget = parent;
   //initialize the background processes
   PMAN = new ProcessManager();
   connect(PMAN, SIGNAL(ProcessFinished(int)),this,SLOT(slotProcessFinished(int)) );
   connect(PMAN, SIGNAL(ProcessMessage(int, QString)),this,SLOT(slotProcessMessage(int, QString)) );
   connect(PMAN, SIGNAL(ProcessError(int,QStringList)),this,SLOT(slotProcessError(int,QStringList)) );
   PENDINGREMOVAL.clear(); PENDINGDL.clear(); PENDINGINSTALL.clear(); PENDINGUPDATE.clear(); PENDINGOTHER.clear();
   sDownload=FALSE; sInstall=FALSE; sUpdate=FALSE; sRemove=FALSE;
   //setup the base paths
   baseDBDir = "/var/db/pbi/";
   sysDB = new PBIDBAccess();
   noRepo=FALSE;
   wardenMode=FALSE;
   //Default User Preferences
   settingsFile = QDir::homePath()+"/.appcafeprefs";
   baseDlDir = QDir::homePath()+"/Downloads/";
   if(!QFile::exists(baseDlDir)){ baseDlDir = QDir::homePath()+"/"; }
   keepDownloads = FALSE;
   autoXDG.clear(); autoXDG << "desktop" << "menu" << "mime" << "paths";
   currentRepoNum = "001"; //first repo by default
   //Filesystem watcher
   watchTimer = new QTimer(this);
     watchTimer->setSingleShot(true);
     watchTimer->setInterval(500); //1/2 a second intervals (to make sure we do not run it too freqently)
     connect(watchTimer, SIGNAL(timeout()), this, SLOT(slotSyncToDatabase()) );
   watcher = new QFileSystemWatcher();
   connect(watcher,SIGNAL(directoryChanged(QString)),this,SLOT(slotWatcherNotification()) );
   
 }
 
 // ==============================
 // ====== PUBLIC FUNCTIONS ======
 // ==============================
 void PBIBackend::setWardenMode(QString dir, QString ip){
   wardenDir = dir; wardenIP = ip;
   //wardenDir: used for direct access to a directory in the jail
   //wardenIP: used to chroot into the jail to run commands
   if( wardenDir.isEmpty() || wardenIP.isEmpty() ){ wardenMode = FALSE; }
   else{ 
     if(!wardenDir.endsWith("/")){wardenDir.append("/"); } //make sure it has a / on the end
     wardenMode = TRUE; 
   }
 }
 
 bool PBIBackend::start(){
   sysArch = Extras::getSystemArch(); //get the architecture for the current system
   //Setup the base paths for the system database and downloads
   if(wardenMode){ 
     DBDir = wardenDir + baseDBDir;
   }else{ 
     DBDir = baseDBDir;
   }
   if(!DBDir.endsWith("/")){ DBDir.append("/"); }
   if( !loadSettings() ){
     saveSettings();	   
   }
   updateDlDirPath(baseDlDir);
   //Now setup the database access class
   sysDB->setDBPath(DBDir);
   sysDB->setRootCMDPrefix( addRootCMD("",TRUE).remove("\"") );
     //Make sure the deisred repo is valid
     QStringList repos = sysDB->availableRepos();
     if(repos.length() > 0){ 
       if(!repos.contains(currentRepoNum)){currentRepoNum = repos[0];} //make sure the desired repo actually exists
     }else{
       qDebug() << "No PBI Repositories available: disabling the browser";
       emit NoRepoAvailable();
       noRepo = TRUE;
     }
   //Set the repo
   if(!noRepo){ sysDB->setRepo(currentRepoNum); }
   //Now start the filesystem watcher
   startFileSystemWatcher();
   //Now initialize the hash lists with the database info
   QTimer::singleShot(1,this,SLOT(slotSyncToDatabase()) );

   return TRUE;
 }

 QStringList PBIBackend::installedList(){
   return QStringList( PBIHASH.keys() ); 
 }
 
 QStringList PBIBackend::browserCategories(){
   return QStringList( CATHASH.keys() ); 
 }
  
QStringList PBIBackend::browserApps( QString catID ){
  QStringList output;
  if(!CATHASH.contains(catID)){ return output; }
  QStringList apps = APPHASH.keys();
  for(int i=0; i<apps.length(); i++){
    if(Extras::nameToID(APPHASH[apps[i]].category)==catID){ output << apps[i]; }
  }
  return output;
}

QStringList PBIBackend::getRecentApps(){
  //List all new/updated applications in the last 10 days (sorted by oldest first)
  QStringList output;
  //Generate a string for today's date
  QDate date = QDate::currentDate();
  date = date.addDays(-10);
  QString chk = date.toString("yyyyMMdd");
  //Now compare the latest apps to this date
  QStringList apps = APPHASH.keys();
  QStringList  unsorted;
  for(int i=0; i<apps.length(); i++){
    if(Extras::newerDateTime(APPHASH[apps[i]].latestDatetime, chk)){
      unsorted << APPHASH[apps[i]].latestDatetime + "::"+apps[i];	    
    }
  }
  //Now sort them by datetime and then remove the datetime stamp
  unsorted.sort(); //oldest->newest 
  for(int i=(unsorted.length()-1); i>=0; i--){ //go in reverse order
    output << unsorted[i].section("::",1,50);
  }
  return output; //newest->oldest
}

QStringList PBIBackend::getRecommendations(){
  QStringList apps;
  QStringList out = Extras::readFile(":defaultrecommendations.txt"); //make this load a db file later
  for(int i=0; i<out.length(); i++){
    //Make sure they are all valid apps in the repo
    if(APPHASH.contains(out[i])){ apps << out[i]; }
  }
  return apps;
}

bool PBIBackend::safeToQuit(){
  //returns true if there is no pending/current processes
  bool ok = ( PENDINGDL.isEmpty() && PENDINGUPDATE.isEmpty() && PENDINGREMOVAL.isEmpty() \
  	    && PENDINGINSTALL.isEmpty() && PENDINGOTHER.isEmpty() && cDownload.isEmpty() \
  	    && cUpdate.isEmpty() && cRemove.isEmpty() && cInstall.isEmpty() && cOther.isEmpty() );
  return ok;
}
// ===== Local/Repo Interaction Functions =====
QString PBIBackend::isInstalled(QString appID){
  //check if the pbiID was given (quick)
  if(PBIHASH.contains(appID)){
    if(PBIHASH[appID].path.isEmpty()){ return ""; }
    else{ return appID; }
  }
  //Returns pbiID of the installed application
  QString output;
  if(!APPHASH.contains(appID)){
    qDebug() << "Invalid application ID:" << appID;
    return output;
  }
  QStringList pbiID = PBIHASH.keys(); //get list of installed PBI's
  for(int i=0; i<pbiID.length();i++){
    QString pbi = Extras::nameToID(PBIHASH[pbiID[i]].metaID);
    if( (pbi == appID) && !PBIHASH[pbiID[i]].path.isEmpty() ){
      output = pbiID[i];
      break;
    }
  }
  return output;
}

QString PBIBackend::upgradeAvailable(QString pbiID){
  QString output;
  if(!PBIHASH.contains(pbiID)){return output;}
  QString appID = Extras::nameToID(PBIHASH[pbiID].metaID);
  if(APPHASH.contains(appID)){
    if(APPHASH[appID].latestVersion != PBIHASH[pbiID].version){output = APPHASH[appID].latestVersion;}  
  }
  return output;
}

// === PBI Actions ===
void PBIBackend::cancelActions(QStringList pbiID){
  qDebug() << "Cancel Actions requested for:" << pbiID;
  for(int i=0; i<pbiID.length(); i++){
    if(PBIHASH.contains(pbiID[i]) ){
      //Remove any pending actions for this pbiID
      PENDINGDL = removePbiCMD(pbiID[i],PENDINGDL);
      PENDINGINSTALL = removePbiCMD(pbiID[i],PENDINGINSTALL);
      PENDINGREMOVAL = removePbiCMD(pbiID[i],PENDINGREMOVAL);
      PENDINGUPDATE = removePbiCMD(pbiID[i],PENDINGUPDATE);
      PENDINGOTHER = removePbiCMD(pbiID[i],PENDINGOTHER); //doubtful that there will even be anything here
      //Now cancel any current operations for this pbiID
      if(cDownload==pbiID[i]){ sDownload=TRUE; PMAN->stopProcess(ProcessManager::DOWNLOAD); }
      if(cUpdate==pbiID[i]){ 
	      if( lUpdate.startsWith("DLSTAT::") || lUpdate.startsWith("DLSTART") ){
		//In download phase of the update - just kill it
	        PMAN->stopProcess(ProcessManager::UPDATE); 
	      }else{
		//In the install phase of the update
		sUpdate=TRUE; //need to clean up after it finishes
	      }
      }
      if(cRemove==pbiID[i]){ sRemove=TRUE; }
      if(cInstall==pbiID[i]){ sInstall=TRUE; }
      //Ignore OTHER process - those commands are pretty much instant
    }
  }
  //Now refresh the PBIHASH info (remove stale entries, update statuses, etc..)
  slotSyncToDatabase(TRUE);
}

void PBIBackend::upgradePBI(QStringList pbiID){
  qDebug() << "PBI Upgrades requested for:" << pbiID;
  for(int i=0; i<pbiID.length(); i++){
    if( PBIHASH.contains(pbiID[i]) && !isInstalled(pbiID[i]).isEmpty() ){
      if( PBIHASH[pbiID[i]].status == InstalledPBI::UPDATEAVAILABLE ){
	QString cmd = generateUpdateCMD(pbiID[i]);
        PENDINGUPDATE << pbiID[i] + ":::" + cmd;
        PBIHASH[pbiID[i]].setStatus(InstalledPBI::PENDINGUPDATE);
        emit PBIStatusChange(pbiID[i]);
      }else{
      	qDebug() << "No update available for:" << pbiID[i];      
      }
    }else{
      qDebug() << pbiID[i] << "not a valid PBI to update";	    
    }
  }
  //Now check/start the update process
  QTimer::singleShot(0,this,SLOT(checkProcesses()) );
}

void PBIBackend::removePBI(QStringList pbiID){
  qDebug() << "PBI Removals requested for:" << pbiID;
  QStringList xdgrem; xdgrem << "remove-desktop" << "remove-menu" << "remove-mime" << "remove-paths";
  QStringList cancelList;
  for(int i=0; i<pbiID.length(); i++){
	    
    if(PBIHASH.contains(pbiID[i])){
      if( isInstalled(pbiID[i]).isEmpty() ){
	//Not a fully-installed PBI - cancel it instead
	cancelList << pbiID[i];
      }else{
        //Remove XDG entries for this app
        PENDINGREMOVAL << pbiID[i]+":::"+generateXDGCMD(pbiID[i],xdgrem, FALSE);
        //Remove the app itself
        PENDINGREMOVAL << pbiID[i]+":::"+generateRemoveCMD(pbiID[i]);
        //Now update the status
        PBIHASH[pbiID[i]].setStatus(InstalledPBI::PENDINGREMOVAL);
        emit PBIStatusChange(pbiID[i]);
      }
    }else{
      qDebug() << pbiID[i] << "not a valid PBI to remove";	    
    }
  }
  //If there are PBIs to cancel, do that too
  if(!cancelList.isEmpty()){ cancelActions(cancelList); }
  //Now check/start the remove process
  QTimer::singleShot(0,this,SLOT(checkProcesses()) );
}

void PBIBackend::installApp(QStringList appID){
  qDebug() << "Install App requested for:" << appID;
  for(int i=0; i<appID.length(); i++){
    if(!APPHASH.contains(appID[i])){ 
      qDebug() << appID[i] << "is not a valid application";
      continue; //go to the next item is this one is invalid
    } 
    //Find out if app is installed already
    QString pbiID = isInstalled(appID[i]);
    if(pbiID.isEmpty()){
      //Not installed at the moment, queue up the installation
      qDebug() << "Fresh install of appID:" << appID[i];
      queueInstall(appID[i]);
    }else{
      //It is already registered as installed
      if( APPHASH[appID[i]].latestVersion == PBIHASH[pbiID].version ){
        //latest version already installed, revert to backup version
	qDebug() << "Revert install of appID:" << appID[i];
	queueInstall(appID[i], APPHASH[appID[i]].backupVersion);
      }else{
	//older version installed - try to update the app
	qDebug() << "Update install of appID:" << appID[i];
	upgradePBI(QStringList() << pbiID);
      }
    }
  } // end of loop over items
  //Now check/start the remove process
  QTimer::singleShot(0,this,SLOT(checkProcesses()) );
  //Now emit the signal that items have changed or been added
  emit LocalPBIChanges();
}

void PBIBackend::installPBIFromFile(QStringList files){
  qDebug() << "Install PBI files requested for:" << files;
  for(int i=0; i<files.length(); i++){
    if(!files[i].endsWith(".pbi")){ continue; } //invalid file
    //Now load the info about this PBI
    bool root = PBIFileNeedsRoot(files[i]);
    //Now create the commands
    QString cmd = addRootCMD("pc-pbigui "+files[i], root);
    //Don't bother integrating it furthur, the pc-pbigui will handle it all better
    //  and if it actually gets installed it will automatically get detected/added to the AppCafe list
    PENDINGOTHER << "EXTERNAL:::"+cmd;
  }
  QTimer::singleShot(0,this,SLOT(checkProcesses()) );
}

void PBIBackend::addDesktopIcons(QStringList pbiID, bool allusers){ // add XDG desktop icons
  for(int i=0; i<pbiID.length(); i++){
    if( PBIHASH.contains(pbiID[i]) && !isInstalled(pbiID[i]).isEmpty() ){
      //generate the command
      QString cmd = generateXDGCMD(pbiID[i],QStringList()<<"desktop",allusers);
      //Now add it to the queue
      PENDINGOTHER << pbiID[i]+":::"+cmd;
    }
  }
  //Now check/start the process
  QTimer::singleShot(0,this,SLOT(checkProcesses()) );
}

void PBIBackend::addMenuIcons(QStringList pbiID, bool allusers){ // add XDG menu icons
  for(int i=0; i<pbiID.length(); i++){
    if( PBIHASH.contains(pbiID[i]) && !isInstalled(pbiID[i]).isEmpty() ){
      //generate the command
      QString cmd = generateXDGCMD(pbiID[i],QStringList()<<"menu",allusers);
      //Now add it to the queue
      PENDINGOTHER << pbiID[i]+":::"+cmd;
    }
  }
  //Now check/start the process
  QTimer::singleShot(0,this,SLOT(checkProcesses()) );
}

void PBIBackend::addPaths(QStringList pbiID, bool allusers){ // create path links
  for(int i=0; i<pbiID.length(); i++){
    if( PBIHASH.contains(pbiID[i]) && !isInstalled(pbiID[i]).isEmpty() ){
      //generate the command
      QString cmd = generateXDGCMD(pbiID[i],QStringList()<<"paths",allusers);
      //Now add it to the queue
      PENDINGOTHER << pbiID[i]+":::"+cmd;
    }
  }
  //Now check/start the process
  QTimer::singleShot(0,this,SLOT(checkProcesses()) );	
}

void PBIBackend::addMimeTypes(QStringList pbiID, bool allusers){ // remove path links
  for(int i=0; i<pbiID.length(); i++){
    if( PBIHASH.contains(pbiID[i]) && !isInstalled(pbiID[i]).isEmpty() ){
      //generate the command
      QString cmd = generateXDGCMD(pbiID[i],QStringList()<<"mime",allusers);
      //Now add it to the queue
      PENDINGOTHER << pbiID[i]+":::"+cmd;
    }
  }
  //Now check/start the process
  QTimer::singleShot(0,this,SLOT(checkProcesses()) );	
}

void PBIBackend::rmDesktopIcons(QStringList pbiID, bool allusers){ // remove XDG desktop icons
  for(int i=0; i<pbiID.length(); i++){
    if( PBIHASH.contains(pbiID[i]) && !isInstalled(pbiID[i]).isEmpty() ){
      //generate the command
      QString cmd = generateXDGCMD(pbiID[i],QStringList()<<"remove-desktop",allusers);
      //Now add it to the queue
      PENDINGOTHER << pbiID[i]+":::"+cmd;
    }
  }
  //Now check/start the process
  QTimer::singleShot(0,this,SLOT(checkProcesses()) );	
}

void PBIBackend::rmMenuIcons(QStringList pbiID, bool allusers){ // remove XDG menu icons
  for(int i=0; i<pbiID.length(); i++){
    if( PBIHASH.contains(pbiID[i]) && !isInstalled(pbiID[i]).isEmpty() ){
      //generate the command
      QString cmd = generateXDGCMD(pbiID[i],QStringList()<<"remove-menu",allusers);
      //Now add it to the queue
      PENDINGOTHER << pbiID[i]+":::"+cmd;
    }
  }
  //Now check/start the process
  QTimer::singleShot(0,this,SLOT(checkProcesses()) );	
}

void PBIBackend::rmPaths(QStringList pbiID, bool allusers){ // remove path links
  for(int i=0; i<pbiID.length(); i++){
    if( PBIHASH.contains(pbiID[i]) && !isInstalled(pbiID[i]).isEmpty() ){
      //generate the command
      QString cmd = generateXDGCMD(pbiID[i],QStringList()<<"remove-paths",allusers);
      //Now add it to the queue
      PENDINGOTHER << pbiID[i]+":::"+cmd;
    }
  }
  //Now check/start the process
  QTimer::singleShot(0,this,SLOT(checkProcesses()) );	
}

void PBIBackend::rmMimeTypes(QStringList pbiID, bool allusers){ // remove path links
  for(int i=0; i<pbiID.length(); i++){
    if( PBIHASH.contains(pbiID[i]) && !isInstalled(pbiID[i]).isEmpty() ){
      //generate the command
      QString cmd = generateXDGCMD(pbiID[i],QStringList()<<"remove-mime",allusers);
      //Now add it to the queue
      PENDINGOTHER << pbiID[i]+":::"+cmd;
    }
  }
  //Now check/start the process
  QTimer::singleShot(0,this,SLOT(checkProcesses()) );	
}

void PBIBackend::enableAutoUpdate(QString pbiID, bool enable){
  if(!PBIHASH.contains(pbiID) || isInstalled(pbiID).isEmpty() ){return;}
  //Generate the command
  QString cmd = generateAutoUpdateCMD(pbiID,enable);
  //Now put it in the queue
  PENDINGOTHER << pbiID+":::"+cmd;
  //Now check/start the process
  QTimer::singleShot(0,this,SLOT(checkProcesses()) );
}

 // === Information Retrieval functions ===
QStringList PBIBackend::PBIInfo( QString pbiID, QStringList infoList){
  QStringList output;
  if(!PBIHASH.contains(pbiID)){ return output; }
  for(int i=0; i<infoList.length(); i++){
    if(infoList[i]=="name"){ output << PBIHASH[pbiID].name; }
    else if(infoList[i]=="version"){ output << PBIHASH[pbiID].version; }
    else if(infoList[i]=="author"){ output << PBIHASH[pbiID].author; }
    else if(infoList[i]=="website"){ output << PBIHASH[pbiID].website; }
    else if(infoList[i]=="arch"){ output << PBIHASH[pbiID].arch; }
    else if(infoList[i]=="path"){ output << PBIHASH[pbiID].path; }
    else if(infoList[i]=="icon"){ 
	QString icon = PBIHASH[pbiID].icon;
	if( QFile::exists(icon) ){ output << icon; }
	else{ output << ""; }
    }else if(infoList[i]=="license"){ output << PBIHASH[pbiID].license; }
    else if(infoList[i]=="metaid"){ output << PBIHASH[pbiID].metaID; }
    else if(infoList[i]=="status"){ output << currentAppStatus(pbiID); }
    else if(infoList[i]=="maintainer"){ output << PBIHASH[pbiID].maintainer; }
    else if(infoList[i]=="description"){ output << PBIHASH[pbiID].description; }
    else if(infoList[i]=="date"){ output << PBIHASH[pbiID].mdate; }
    else if(infoList[i]=="fbsdversion"){ output << PBIHASH[pbiID].fbsdversion; }
    //Now the boolians
    else if(infoList[i]=="requiresroot"){ 
      if(PBIHASH[pbiID].rootInstall){output<<"true";}
      else{ output<<"false";}
    }
    else if(infoList[i]=="autoupdate"){ 
      if(PBIHASH[pbiID].autoUpdate){output<<"true";}
      else{ output<<"false";}
    }
    else if(infoList[i]=="hasdesktopicons"){ 
      if(PBIHASH[pbiID].desktopIcons){output<<"true";}
      else{ output<<"false";}
    }
    else if(infoList[i]=="hasmenuicons"){ 
      if(PBIHASH[pbiID].menuIcons){output<<"true";}
      else{ output<<"false";}
    }
    else if(infoList[i]=="hasmimetypes"){ 
      if(PBIHASH[pbiID].mimetypes){output<<"true";}
      else{ output<<"false";}
    }
    else{ output << ""; }
  }
  //qDebug()<< "Info Requested for:" << pbiID << infoList << "Info:" << output;
  return output;
}	

QStringList PBIBackend::CatInfo( QString catID, QStringList infoList){
  QStringList output;
  if(!CATHASH.contains(catID)){ return output; }
  for(int i=0; i<infoList.length(); i++){
    if(infoList[i]=="name"){ output << CATHASH[catID].name; }
    else if(infoList[i]=="icon"){ output << CATHASH[catID].localIcon; }
    else if(infoList[i]=="description"){ output << CATHASH[catID].description; }   
    else{ output << ""; }
  }
  //qDebug()<< "Info Requested for:" << catID << infoList << "Info:" << output;
  return output;
}

QStringList PBIBackend::AppInfo( QString appID, QStringList infoList){
  QStringList output;
  if(!APPHASH.contains(appID)){ return output; }
  for(int i=0; i<infoList.length(); i++){
    if(infoList[i]=="name"){ output << APPHASH[appID].name; }
    else if(infoList[i]=="author"){ output << APPHASH[appID].author; }
    else if(infoList[i]=="website"){ output << APPHASH[appID].website; }
    else if(infoList[i]=="icon"){ output << APPHASH[appID].localIcon; }
    else if(infoList[i]=="license"){ output << APPHASH[appID].license; }
    else if(infoList[i]=="type"){ output << APPHASH[appID].appType; }
    else if(infoList[i]=="description"){ output << APPHASH[appID].description; }
    else if(infoList[i]=="shortdescription"){ output << APPHASH[appID].shortdescription; }
    else if(infoList[i]=="maintainer"){ output << APPHASH[appID].maintainer; }
    else if(infoList[i]=="category"){ output << APPHASH[appID].category; }
    else if(infoList[i]=="latestversion"){ output << APPHASH[appID].latestVersion; }
    else if(infoList[i]=="latestarch"){ output << APPHASH[appID].latestArch; }
    else if(infoList[i]=="latestsize"){ output << APPHASH[appID].latestSizeK; }
    else if(infoList[i]=="backupversion"){ output << APPHASH[appID].backupVersion; }
    else if(infoList[i]=="backuparch"){ output << APPHASH[appID].backupArch; }
    else if(infoList[i]=="backupsize"){ output << APPHASH[appID].backupSizeK; }
    //Now the boolians
    else if(infoList[i]=="requiresroot"){ 
      if(APPHASH[appID].requiresroot){output<<"true";}
      else{ output<<"false";}
    }
    else{ output << ""; }
  }
  //qDebug()<< "Info Requested for:" << appID << infoList << "Info:" << output;
  return output;
}

QString PBIBackend::currentAppStatus( QString appID, bool rawstatus ){
  QString output;
  int status = -999;
  QString metaID;
  //pbiID given (quicker)
  if(PBIHASH.contains(appID)){ status = PBIHASH[appID].status; metaID = PBIHASH[appID].metaID; }
  else{
    //appID given
    if(!APPHASH.contains(appID)){ return ""; }
    QStringList pbilist = PBIHASH.keys();
    bool active = false;
    for(int i=0; i<pbilist.length(); i++){
      //Be careful about more than one pbiID matching the given appID - grab the active one
      if(PBIHASH[pbilist[i]].metaID == appID){ 
        status = PBIHASH[pbilist[i]].status; 
	metaID=appID;
	active = !(status==InstalledPBI::UPDATEAVAILABLE || status==InstalledPBI::NONE);
      }
      if(active){ break; }
    }
  }
  //Determine if the app is currently in a pending/running state
  if(rawstatus){ //output the raw status for active processes
    switch (status){
	case InstalledPBI::DOWNLOADING:
	  output = lDownload;
	  if(output.isEmpty()){ output = "Downloading"; }
	  break;
	case InstalledPBI::INSTALLING:
	  output = lInstall; 
	  if(output.isEmpty()){ output = "Installing"; }
	  break;
	case InstalledPBI::REMOVING:
	  output = lRemove; 
	  if(output.isEmpty()){ output = "Removing"; }
	  break;
	case InstalledPBI::UPDATING:
	  output = lUpdate; 
	  if(output.isEmpty()){ output = "Updating"; }
	  break;
	case InstalledPBI::NONE:
	case InstalledPBI::UPDATEAVAILABLE:
	  output.clear(); break;
	default:
	  output = "Pending";
    }
  }else{
    switch (status){
        case InstalledPBI::DOWNLOADING:
	  if(sDownload){ output = tr("Download Canceled"); }
	  if(lDownload.startsWith("DLSTAT::")){
	    QString percent = lDownload.section("::",1,1);
	    QString spd = lDownload.section("::",3,3);
	    if(spd=="??"){
	      output = QString(tr("Downloading: %1%")).arg( percent );
	    }else{
	      output = QString(tr("Downloading: %1% @ %2")).arg( percent, spd );
	    }
	  }else{
	    output = lDownload;
	  }
	  break;
        case InstalledPBI::INSTALLING:
	  if(sInstall){ output = tr("Install Canceled (will remove)"); }
	  else{ output = tr("Installing"); }
	  break;
        case InstalledPBI::REMOVING:
	  if(sRemove){ output = tr("Removal Canceled (will reinstall)"); }
          else{ output = tr("Removing Application"); }
	  break;
        case InstalledPBI::UPDATING:
	  if(sUpdate){ output = tr("Update's cannot be canceled"); }
	  else if(lUpdate.startsWith("DLSTAT::")){
	    QString percent = lUpdate.section("::",1,1);
	    output = QString(tr("Update Downloading: %1%")).arg( percent );
	  }else if(lUpdate == "DLDONE"){
	    output = tr("Starting Update");
	  }else if(lUpdate == "DLSTART"){
	    output = tr("Starting Download");
	  }else{
            output = tr("Updating");
	  }
	  break;
        case InstalledPBI::PENDINGDOWNLOAD:
          output = tr("Pending Download"); break;
        case InstalledPBI::PENDINGINSTALL:
          output = tr("Pending Install"); break;
        case InstalledPBI::PENDINGREMOVAL:
          output = tr("Pending Removal"); break;
        case InstalledPBI::PENDINGUPDATE:
          output = tr("Pending Update"); break;
	case InstalledPBI::UPDATEAVAILABLE:
	  output = QString(tr("Update Available: %1")).arg(APPHASH[metaID].latestVersion);
	  break;
        default: //do nothing for the rest
          output.clear();
    }
  }
  return output;
}

bool PBIBackend::isWorking(QString pbiID){
  if( !PBIHASH.contains(pbiID) ){ return FALSE; }

  bool notworking = (PBIHASH[pbiID].status == InstalledPBI::UPDATEAVAILABLE || PBIHASH[pbiID].status == InstalledPBI::NONE );
  return !notworking;
}

QStringList PBIBackend::pbiBinList(QString pbiID){
  QStringList output;
  //qDebug() << "Start Search for Binaries:" << pbiID;
  if( !PBIHASH.contains(pbiID) ){ return output; }
  //Now prod the database to see what binaries are currently available to run 
  QDir bDir(PBIHASH[pbiID].path+"/.xdg-menu/");  
  if(bDir.exists()){
    QStringList bList = bDir.entryList(QStringList() << "*.desktop");
    for(int i =0; i<bList.length(); i++){
      QStringList contents = Extras::readFile(bDir.absoluteFilePath(bList[i]));
	//qDebug() << " - Check file:" << bList[i] << contents;
      //Make sure that it is a visible entry
      if(!contents.contains("NoDisplay=true")){
	//qDebug() << " -- visible entry";
	QString name;
	//Get the locale code
	QString loc = QLocale::system().name();
	bool lName=false;
	for(int j=0; j<contents.length(); j++){
	  if(contents[j].startsWith("Name=") && !lName){ name = contents[j].section("=",1,10);  }
	  else if(contents[j].startsWith("Name["+loc+"]=")){ name = contents[j].section("=",1,10);  lName=true; }
	}
	//Format the output string
	if( !name.isEmpty() ){
	  output << name.simplified()+"::::"+bDir.absoluteFilePath(bList[i]);
	}
      }
    } //end loop over files
  }
  return output;
}

QString PBIBackend::pbiToAppID(QString pbiID){
  QString appID;
  if( PBIHASH.contains(pbiID) ){ appID = PBIHASH[pbiID].metaID; }	
  return appID;
}

// === Configuration Management ===
void PBIBackend::openConfigurationDialog(){
  //temporarily disable the filesystem watcher (causes issues with repo changes)
  stopFileSystemWatcher();
  //Now setup the UI
  ConfigDialog *cfg = new ConfigDialog(parentWidget);
  cfg->xdgOpts = autoXDG;
  cfg->keepDownloads = keepDownloads;
  cfg->downloadDir = baseDlDir;
  cfg->DB = sysDB; //for access to database operations
  cfg->setupDone();
  //Now run it
  cfg->exec(); //makes it modal and waits for it to finish
  //Now see if there are changes to save
  if(cfg->applyChanges){
    autoXDG = cfg->xdgOpts;
    keepDownloads = cfg->keepDownloads;
    updateDlDirPath(cfg->downloadDir);
    //Now save the configuration data to file
    saveSettings();

  }
  //Now re-enable the filesystem watcher
  startFileSystemWatcher();
  //Now re-sync the repo info
  syncCurrentRepo();
}

// === Import/Export PBI Lists ===
bool PBIBackend::exportPbiListToFile(QString filepath){
  bool ok = FALSE;
  //get the currently installed PBI's
  QStringList list = PBIHASH.keys();
  QStringList installed;
  for(int i=0; i<list.length(); i++){
    if(!PBIHASH[list[i]].path.isEmpty()){ installed << PBIHASH[list[i]].metaID; }	  
  }
  qDebug() << "Export List:" << installed;
  //Now save the list
  if(installed.isEmpty()){ ok = TRUE; }
  else{ ok = Extras::writeFile(filepath,installed); }
  return ok;
}

bool PBIBackend::importPbiListFromFile(QString filepath){
  bool ok = FALSE;
  if(!QFile::exists(filepath)){ return ok; }
  QStringList inlist = Extras::readFile(filepath);
  if(inlist.isEmpty()){ return ok; }
  qDebug() << "Import List:" << inlist;
  //Now sort the apps based on validity
  QStringList good, bad, current;  
  for(int i=0; i<inlist.length(); i++){
    QString app = inlist[i].toLower();
    if(app.isEmpty()){continue;}
    else if(!APPHASH.contains(app)){ bad << inlist[i]; }
    else if( !isInstalled(app).isEmpty() ){ current << inlist[i]; }
    else{ good << app; } //make sure to use the appID here
  }
  ok = TRUE;
  //List the results
  if(good.isEmpty()){
    QString results = tr("No applications to install from this list.")+"\n"+tr("Results:")+"\n";
    if(!bad.isEmpty()){ results.append( QString(tr("Unavailable Apps: %1")).arg(bad.join(",  "))+"\n" ); }
    if(!current.isEmpty()){ results.append( QString(tr("Currently Installed: %1")).arg(current.join(",  "))+"\n" ); }
    QMessageBox::information(0,tr("Import Results"),results);
  }else{
    QString results = tr("Are you sure you wish to install these applications?")+"\n\n"+good.join(", ");
    if( QMessageBox::question(0,tr("Import Results"),results,QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes){
      //Go ahead and install these applications
      installApp(good);
    }
  }
  return ok;
}

 // ==========================
 // ====== PUBLIC SLOTS ======
 // ==========================
 // NOTE: These functions should only be called from a QTimer::singleShot()
 //    Because outputs come via different signals (due to time for these functions to run)
 
void PBIBackend::startAppSearch(){
 //  Outputs come via the "SearchComplete(QStringList best,QStringList rest)" signal
 QString search = searchTerm; //This public variable needs to be set beforehand by the calling process
 QStringList best, rest;
 //Now perform the search and categorize it
 search = search.toLower();
 QStringList namematch, tagmatch, descmatch;
 QStringList app = APPHASH.keys();
 for(int i=0; i<app.length(); i++){
   if(APPHASH[app[i]].name.toLower() == search){ best << app[i]; } //exact match - top of the "best" list
   else if(APPHASH[app[i]].name.toLower().contains(search)){ namematch << app[i]; }
   else if(APPHASH[app[i]].tags.contains(search)){ tagmatch << app[i]; }
   else if(APPHASH[app[i]].description.contains(search)){ descmatch << app[i]; }
 }
 //Now sort the lists and assign a priority
 namematch.sort(); tagmatch.sort(); descmatch.sort();
 best << namematch;
 if(best.isEmpty()){ best << tagmatch; }
 else{ rest << tagmatch; }
 if(best.isEmpty()){ best << descmatch; }
 else{ rest << descmatch; }

 //Now emit the signal with the results
 emit SearchComplete(best,rest);
}

void PBIBackend::startSimilarSearch(){
  //  Outputs come via the "SimilarFound(QStringList results)" signal
  QString sID = searchSimilar; // this public variable needs to be set beforehand by the calling process
  QStringList output;  
  if(!APPHASH.contains(sID)){ return; } 
  //Now find the tags on the given ID
  QStringList stags = APPHASH[sID].tags;
  QStringList apps = APPHASH.keys();
  QStringList unsorted;
  int maxMatch=0;
  for(int i=0; i<apps.length(); i++){
    if(apps[i]==sID){continue;} //skip the app we were given for search parameters
    QStringList tags = APPHASH[apps[i]].tags;
    int match=0;
    for(int j=0; j<stags.length(); j++){
       if(tags.indexOf(stags[j]) != -1){ match++; }
    }
    if(match > 1){ unsorted << QString::number(match)+"::"+apps[i]; }
    if(match > maxMatch){ maxMatch = match; }
  }
  unsorted.sort();
  for(int i=unsorted.length(); i>0; i--){
    if( unsorted[i-1].section("::",0,0).toInt() > (maxMatch-1) ){
      output << unsorted[i-1].section("::",1,50);
    }else if( output.length() < 5 ){ //not enough matches, grab the next set too
      output << unsorted[i-1].section("::",1,50);
      maxMatch--;
    }else{ break; } //already sorted and the rest are even lower match rating
  }
  //Now emit the signal with the results
  emit SimilarFound(output);
}

 // ===============================
 // ====== PRIVATE FUNCTIONS ======
 // ===============================
bool PBIBackend::saveSettings(){
  //Generate the file layout
  QStringList file;
  file << "POSTINSTALL="+autoXDG.join(",").simplified();
  file << "REPONUMBER="+sysDB->currentRepo();
  if(keepDownloads){ file << "KEEPDOWNLOADS=TRUE"; }
  else{ file << "KEEPDOWNLOADS=FALSE"; }
  file << "DOWNLOADDIR="+baseDlDir;
  //Now save the file
  bool ok = Extras::writeFile(settingsFile, file);
  return ok;
}

bool PBIBackend::loadSettings(){
  if(!QFile::exists(settingsFile)){ return FALSE; }
  QStringList file = Extras::readFile(settingsFile);
  if(file.isEmpty()){ return FALSE; }
  for(int i=0; i<file.length(); i++){
    if(file[i].startsWith("POSTINSTALL=")){ autoXDG = file[i].section("=",1,1).split(","); }
    else if(file[i].startsWith("REPONUMBER=")){ sysDB->setRepo(file[i].section("=",1,1)); }
    else if(file[i].startsWith("KEEPDOWNLOADS=")){ 
      if(file[i].section("=",1,1) == "TRUE"){ keepDownloads = TRUE; }
      else{ keepDownloads = FALSE; }
    }else if(file[i].startsWith("DOWNLOADDIR=")){ updateDlDirPath(file[i].section("=",1,1)); }
  }
  return TRUE;
}
 
 QString PBIBackend::addRootCMD(QString cmd, bool needRoot){
   //Check for warden and root permissions and adjust the command accordingly
   if(wardenMode){
     cmd.prepend("warden chroot "+wardenIP+" \""); cmd.append("\"");
   }else if( needRoot ){
     cmd.prepend("pc-su \""); cmd.append("\"");	     
   }
   return cmd;
 }
 
 QString PBIBackend::generateUpdateCMD(QString pbiID){
   QString output;
   if(!PBIHASH.contains(pbiID)){ return output; }
   output = "pbi_update "+pbiID;
   output = addRootCMD(output, PBIHASH[pbiID].rootInstall);
   return output;
 }
 
 QString PBIBackend::generateRemoveCMD(QString pbiID){
   QString output;
   if(!PBIHASH.contains(pbiID)){ return output; }
   output = "pbi_delete "+pbiID;
   output = addRootCMD(output, PBIHASH[pbiID].rootInstall);
   return output;   	 
 }
 
 QString PBIBackend::generateAutoUpdateCMD(QString pbiID, bool enable){
   QString output;
   if(!PBIHASH.contains(pbiID)){ return output; }
   output = "pbi_update";
   if(enable){ output.append(" --enable-auto"); }
   else{ output.append(" --disable-auto"); }
   output.append(" "+PBIHASH[pbiID].metaID);
   output = addRootCMD(output, PBIHASH[pbiID].rootInstall);
   return output;   	 
 }

 QString PBIBackend::generateXDGCMD(QString pbiID, QStringList actions, bool allusers){
   QString output;
   if(!PBIHASH.contains(pbiID)){ return output; }
   output = "pbi_icon";
   if(actions.contains("desktop")){ output.append(" add-desktop"); }
   if(actions.contains("menu")){ output.append(" add-menu"); }
   if(actions.contains("paths")){ output.append(" add-pathlnk"); }
   if(actions.contains("mime")){ output.append(" add-mime"); }
   if(actions.contains("remove-desktop")){ output.append(" del-desktop"); }
   if(actions.contains("remove-menu")){ output.append(" del-menu"); }
   if(actions.contains("remove-paths")){ output.append(" del-pathlnk"); }
   if(actions.contains("remove-mime")){ output.append(" del-mime"); }
   //Check command actions
   if(output == "pbi_icon"){ 
     output.clear(); //no actions - do nothing
   }else{
     output.append(" "+pbiID);
     output = addRootCMD(output,allusers);
   }
   return output;   	 
 }
 
 QString PBIBackend::generateDownloadCMD(QString appID, QString version){
   if(!APPHASH.contains(appID)){ return ""; }
   QString output = "pbi_add -R --repo "+sysDB->currentRepo();
   if(!version.isEmpty()){ output.append(" --rVer "+version); }
   output.append(" "+appID);
   return output;
 }
 
 QString PBIBackend::generateInstallCMD(QString appID, QString filename){
   QString output = "pbi_add --licagree "+filename;
   output = addRootCMD(output, PBIHASH[appID].rootInstall);
   return output;
 }
 
QStringList PBIBackend::removePbiCMD(QString pbiID, QStringList list){
   //Used for removing any pending CMD's for a particular pbiID
   QStringList output;
   for(int i=0; i<list.length(); i++){
     if(!list[i].startsWith(pbiID)){ output << list[i]; }	   
   }
   return output;
}

bool PBIBackend::PBIFileNeedsRoot(QString filepath){
  //Used to grab information from a stand-along *.pbi file
  bool root = false;
  QStringList info = Extras::getCmdOutput("pbi_add -i "+filepath);
  //Only check if it needs root
  root = !info.filter("RootInstall:").join("").contains("NO");
  return root;
}

void PBIBackend::queueInstall(QString appID, QString version){
  //This function assumes that the new app/version combination is not already installed on the system
  //  and that upgrading is not an option (fresh download/install)	
  if( !APPHASH.contains(appID) ){ return; }
  //verify that the version is available
  if( version.isEmpty() ){ version = APPHASH[appID].latestVersion; }
  bool useLatest = false;
  if( version == APPHASH[appID].latestVersion){ useLatest=true; }
  else if( version != APPHASH[appID].backupVersion ){ return; } //invalid version
  //Check to see if the file is already downloaded
  QString dlFile, arch;
  if(useLatest){ dlFile = APPHASH[appID].latestFilename; arch = APPHASH[appID].latestArch; }
  else{ dlFile = APPHASH[appID].backupFilename; arch = APPHASH[appID].backupArch; }
  //Generate PBI ID
  QString newID = appID+"-"+version+"-"+arch;
  QString oldID = isInstalled(appID); //look for existing installation of this app

  //Create Commands and add them to the proper queue
  if( QFile::exists(dlDir+dlFile) ){
    if(!oldID.isEmpty()){
      //Remove the old application first
      PENDINGINSTALL << oldID+":::"+generateRemoveCMD(oldID);
    }
    PENDINGINSTALL << newID+":::"+generateInstallCMD(appID,dlFile);
  }else{
    //Generate download command
    PENDINGDL << newID+":::"+generateDownloadCMD(appID, version);
  }
  //Setup the HASH entry for the new PBI
  PBIHASH.insert(newID, InstalledPBI());
  PBIHASH[newID].metaID = appID;
  PBIHASH[newID].version = version;
  PBIHASH[newID].arch = arch;
  PBIHASH[newID].downloadfile = dlFile;
  syncPBI(newID,FALSE);
  
}
 
 // ===============================
 // ======   PRIVATE SLOTS   ======
 // ===============================
 void PBIBackend::updateDlDirPath(QString newbase){
   baseDlDir = newbase;
   if(wardenMode){ 
     dlDir = wardenDir + baseDlDir;
   }else{ 
     dlDir = baseDlDir; 
   }
   if(!dlDir.endsWith("/")){ dlDir.append("/"); } 
 }
 
 void PBIBackend::slotWatcherNotification(){
  watchTimer->start();	 
 }
 
 // Internal Process Management
 void PBIBackend::checkProcesses(){
   bool again=FALSE;
   if( cDownload.isEmpty() && !PENDINGDL.isEmpty() ){
     //internal management
     cDownload = PENDINGDL[0].section(":::",0,0); //should be a pbiID -ONLY-
     QString cmd = PENDINGDL[0].section(":::",1,50);
     PENDINGDL.removeAt(0);	  
     if( !cmd.isEmpty() && PBIHASH.contains(cDownload) ){
       //Update the PBI status
       PBIHASH[cDownload].setStatus(InstalledPBI::DOWNLOADING);
       emit PBIStatusChange(cDownload);
       //Start the process
       PMAN->goToDirectory(ProcessManager::DOWNLOAD,dlDir);
       PMAN->startProcess(ProcessManager::DOWNLOAD,cmd);
     }else{
       cDownload.clear();
       again=TRUE; //Move to the next pending download
     }
   }
   if( cInstall.isEmpty() && !PENDINGINSTALL.isEmpty() ){
     //internal management
     cInstall = PENDINGINSTALL[0].section(":::",0,0); //should be a pbiID -ONLY-
     QString cmd = PENDINGINSTALL[0].section(":::",1,50);
     PENDINGINSTALL.removeAt(0);	  
     if( !cmd.isEmpty() && PBIHASH.contains(cInstall) ){
       //Update the PBI status
       PBIHASH[cInstall].setStatus(InstalledPBI::INSTALLING);
       emit PBIStatusChange(cInstall);
       //Start the process
       PMAN->goToDirectory(ProcessManager::INSTALL,dlDir);
       PMAN->startProcess(ProcessManager::INSTALL,cmd);
     }else{
       cInstall.clear();
       again=TRUE; //Move to the next pending install
     }
   }
   if( cRemove.isEmpty() && !PENDINGREMOVAL.isEmpty() ){
     //internal management
     cRemove = PENDINGREMOVAL[0].section(":::",0,0); //should be a pbiID -ONLY-
     QString cmd = PENDINGREMOVAL[0].section(":::",1,50);
     PENDINGREMOVAL.removeAt(0);	  
     if( !cmd.isEmpty() && PBIHASH.contains(cRemove) ){
       //Update the PBI status
       PBIHASH[cRemove].setStatus(InstalledPBI::REMOVING);
       emit PBIStatusChange(cRemove);
       //Start the process
       PMAN->startProcess(ProcessManager::REMOVE,cmd);
     }else{
       cRemove.clear();
       again=TRUE; //Move to the next pending removal
     }
   }
   if( cUpdate.isEmpty() && !PENDINGUPDATE.isEmpty() ){
     //internal management
     cUpdate = PENDINGUPDATE[0].section(":::",0,0); //should be a pbiID -ONLY-
     QString cmd = PENDINGUPDATE[0].section(":::",1,50);
     PENDINGUPDATE.removeAt(0);	  
     if( !cmd.isEmpty() && PBIHASH.contains(cUpdate) ){
       //Update the PBI status
       PBIHASH[cUpdate].setStatus(InstalledPBI::UPDATING);
       emit PBIStatusChange(cUpdate);
       //Start the process
       PMAN->startProcess(ProcessManager::UPDATE,cmd);
     }else{
       cUpdate.clear();
       again=TRUE; //Move to the next pending update
     }
   }
   if( cOther.isEmpty() && !PENDINGOTHER.isEmpty() ){
     //internal management
     cOther = PENDINGOTHER[0].section(":::",0,0); //should be a pbiID -ONLY-
     QString cmd = PENDINGOTHER[0].section(":::",1,50);
     PENDINGOTHER.removeAt(0);	  
     if( !cmd.isEmpty() && (PBIHASH.contains(cOther) || cOther=="EXTERNAL") ){
       //Update the PBI status
       if(cOther!="EXTERNAL"){
         PBIHASH[cOther].setStatus(InstalledPBI::WORKING);
         emit PBIStatusChange(cOther);
       }
       //Start the process
       PMAN->startProcess(ProcessManager::OTHER,cmd);
     }else{
       cOther.clear();
       again=TRUE; //Move to the next pending command
     }
   }
   if(again){ QTimer::singleShot(10,this,SLOT(checkProcesses()) ); }
 }
 
 void PBIBackend::slotProcessFinished(int ID){
   bool resync = FALSE;
   if(ID == ProcessManager::UPDATE){
     if(sUpdate){
       //Update stopped during installation of new version: re-install old version
	//get metaID of app
	/*QString metaID = PBIHASH[cUpdate].metaID;
	slotSyncToDatabase(TRUE);
	sleep(2);
	installApp(QStringList() << metaID); //will install backup version if available*/
     }
     //Update the PBIHASH for installed versions
     resync=TRUE;
     cUpdate.clear(); //remove that it is finished
     lUpdate.clear();
     sUpdate=FALSE;
     resync=TRUE;
   }else if(ID == ProcessManager::REMOVE){
     if(sRemove){
       //Removal Cancelled: Re-install the PBI
       QString metaID = PBIHASH[cRemove].metaID; //get the metaID
       slotSyncToDatabase(TRUE);
       sleep(2);
       installApp(QStringList() << metaID);
     }
     sRemove=FALSE;
     cRemove.clear(); //remove that it is finished	
     lRemove.clear();
   }else if(ID == ProcessManager::INSTALL){
     //Add XDG commands to the queue
     if(sInstall){
       //Installation Cancelled: remove the PBI now that the install is complete
       QString cmd = generateRemoveCMD(cInstall);
       PENDINGREMOVAL << cInstall+":::"+cmd;
     }else{  
       // Installation NOT canceled
       qDebug() << "Installation Finished:" << cInstall;
       if(!keepDownloads){ QFile::remove(dlDir+PBIHASH[cInstall].downloadfile); }
       //Generate XDG commands
       QString cmd = generateXDGCMD(cInstall, autoXDG, FALSE);
       PENDINGOTHER << cInstall+":::"+cmd;
     }
     sInstall = FALSE;
     cInstall.clear(); //remove that it is finished
     lInstall.clear();
     resync=TRUE; //make sure to reload local files
   }else if(ID == ProcessManager::DOWNLOAD){
     //Make sure the download was successful
     //qDebug() << "dlDir:" << dlDir << "file:" << PBIHASH[cDownload].downloadfile;
     if(sDownload){
       //Download Cancelled: remove the (partially) downloaded file
       QString fPath = dlDir+PBIHASH[cDownload].downloadfile;
       if(QFile::exists(fPath)){
	 QFile::remove(fPath);
       }
     }else{
       //Download not cancelled
       if(!QFile::exists(dlDir+PBIHASH[cDownload].downloadfile)){
         qDebug() << "Download Error:" << cDownload << PBIHASH[cDownload].downloadfile;
         QString title = QString(tr("%1 Download Error:")).arg(PBIHASH[cDownload].name);
         QString err = tr("The PBI could not be downloaded, please try again later");
	 QStringList log = PMAN->getProcessLog(ProcessManager::DOWNLOAD);
         emit Error(title,err,log);
       }else{
         //Now Check to see if an alternate version needs to be removed
         QString otherID = isInstalled( PBIHASH[cDownload].metaID );
         QString cmd;
         if(!otherID.isEmpty()){
           cmd = generateRemoveCMD(otherID);
           PENDINGINSTALL << otherID+":::"+cmd; //make sure it happens before the install, so put it in the same queue
         }
         //Now add the installation of this PBI to the queue
         cmd = generateInstallCMD(cDownload, PBIHASH[cDownload].downloadfile);
         PENDINGINSTALL << cDownload+":::"+cmd;
       }
     }
     sDownload = FALSE;
     cDownload.clear(); //remove that it is finished	
     lDownload.clear();
   }else if(ID == ProcessManager::OTHER){
     //qDebug() << "Other Finished:" << cOther;
     //emit PBIStatusChange(cOther);
     cOther.clear();	   
   }
   //Get the next processes going
   slotSyncToDatabase(resync); //update internal database with/without reading local files again
   QTimer::singleShot(0,this,SLOT(checkProcesses()) ); //look for more processes to start
 }
 
void PBIBackend::slotProcessMessage(int ID, QString info){
   if(ID == ProcessManager::UPDATE){
     //PBIHASH[cUpdate].setStatus(InstalledPBI::UPDATING, dlinfo); 
     lUpdate = info;
     emit PBIStatusChange(cUpdate);
   }else if(ID == ProcessManager::DOWNLOAD){
     //PBIHASH[cDownload].setStatus(InstalledPBI::DOWNLOADING, dlinfo); 
     lDownload = info;
     emit PBIStatusChange(cDownload);
   }else if( ID == ProcessManager::REMOVE){
     lRemove = info;
     emit PBIStatusChange(cRemove);
   }else if( ID == ProcessManager::INSTALL){
     lInstall = info;
     emit PBIStatusChange(cInstall);
   }
}

void PBIBackend::slotProcessError(int ID, QStringList log){
   QString title;
   QString name;
   QString message;
   if(ID == ProcessManager::UPDATE){
     if(!sUpdate){ //not stopped manually
       //Try to do the update through full download instead
       if(PBIHASH.contains(cUpdate)){
	 QString metaID = PBIHASH[cUpdate].metaID;
	 if(APPHASH.contains(metaID)){
	  QString version = APPHASH[metaID].latestVersion;
	  queueInstall(metaID, version);
	 }else{
	  QString name = PBIHASH[cUpdate].name;
          title = QString(tr("%1 Update Error:")).arg(name); 
          message = tr("The update process experienced an error and could not be completed");
	 }
       }
     }
   }
   else if(ID == ProcessManager::INSTALL){ 
     if(!sInstall){ //not stopped manually
       if(APPHASH.contains(cInstall)){name = APPHASH[cInstall].name; }
       title = QString(tr("%1 Installation Error:")).arg(name);
       message = tr("The installation process experienced an error and could not be completed");
     }
   }
   else if(ID == ProcessManager::REMOVE){ 
     if(!sRemove){ //not stopped manually
       if(PBIHASH.contains(cRemove)){name = PBIHASH[cRemove].name; }
       title = QString(tr("%1 Removal Error:")).arg(name);
       message = tr("The removal process experienced an error and could not be completed");
     }
   }
   else if(ID == ProcessManager::DOWNLOAD){ 
       //The ProcessFinished function has a bit more robust check for download failures
       //       - use it instead to prevent duplicate error messages
   }
   else if(ID == ProcessManager::OTHER){ 
     if(PBIHASH.contains(cOther)){name = PBIHASH[cOther].name; }
     title = QString(tr("%1 PBI Error:")).arg(name); 
     message = tr("The process experienced an error and could not be completed");
   }
   if(!title.isEmpty() && !message.isEmpty()){
     qDebug() << "Process Error:" << title << log;
     emit Error(title,message,log); //send error signal
   }
   slotProcessFinished(ID); //clean up
}

 // === Database Synchronization ===
 void PBIBackend::slotSyncToDatabase(bool localChanges){
   //qDebug() << "Sync Database with local changes:" << localChanges;
   //Locally Installed PBI Changes 
   QStringList currInst = installedList();
   QStringList sysList = sysDB->installed();
   numInstalled = sysList.length();
   //All locally installed applications
   for(int i=0; i<sysList.length(); i++){
     int index = currInst.indexOf(sysList[i]);
     if( index == -1){ //New entry
       PBIHASH.insert(sysList[i],InstalledPBI()); //add a blank entry
       syncPBI(sysList[i],TRUE); //Now update the info
       //Add it to the watcher
       watcher->addPath(DBDir+"installed/"+sysList[i]);
     }else{  //Existing entry - remove it from the currInst list
       if(localChanges){ syncPBI(sysList[i],TRUE); } //synchronize the data with local file changes
       else{ updateStatus(sysList[i]); } //just update the status
       currInst.removeAt(index);
     }
   }
   //Non-Installed applications
   for(int i=0; i<currInst.length(); i++){
     updateStatus(currInst[i]); //update status
     InstalledPBI::PBISTATUS stat = PBIHASH[currInst[i]].status;
     bool actionPending = (stat != InstalledPBI::NONE) && (stat != InstalledPBI::UPDATEAVAILABLE);
     if( !actionPending ){ PBIHASH.remove(currInst[i]); }
     else{} //do nothing here
   }
   emit LocalPBIChanges(); //Let others know that the local PBI's have been updated
   //Repo Changes
   //qDebug() << "noRepo" << noRepo << "Invalid Repo:" << sysDB->currentRepo() << sysDB->currentRepoInvalid();
   if( noRepo || sysDB->currentRepoInvalid() ){
     //Change to the first repo available
     qDebug() << "Try to find an alternate Repo:";
     QStringList repos = sysDB->availableRepos();
     if(repos.length() > 0){ 
       sysDB->setRepo(repos[0]);
     }
     syncCurrentRepo();
   }else if( CATHASH.isEmpty() && APPHASH.isEmpty() ){
     qDebug() << "Load Repo Information";
     //If successful, the repo data should only be loaded once
     syncCurrentRepo(); 
     //If the sync was successful, re-run the PBI sync process to update the license info
     if( !APPHASH.isEmpty() ){  	     
       QTimer::singleShot(10,this,SLOT(slotSyncToDatabase())); 
     }else{
       QTimer::singleShot(60000,this,SLOT(slotSyncToDatabase())); //try again in a minute  
     }
   }
   //Update the system flag for whether updates are available
   bool updates = false;
   for(int i=0; i<sysList.length(); i++){
     //Check if there are updates available for this PBI
     updates = (updates || !upgradeAvailable(sysList[i]).isEmpty() );
   }
   if(updates){
     QProcess::startDetached("pc-systemflag PBIUPDATE UPDATE");
   }else if(!noRepo){
     QProcess::startDetached("pc-systemflag PBIUPDATE SUCCESS");
   }
 }
 
 void PBIBackend::syncPBI(QString pbiID, bool useDB){
   //This function is mainly used for initializing the PBIHASH entry
   //  but is also used for updating the entry if the local installation settings change (useDB=true)
 	 
   //useDB: pull info from the locally installed database (pbiID MUST be installed locally)
   //Get the PBI structure
   if( !PBIHASH.contains(pbiID) ){ return; }
   InstalledPBI pbi = PBIHASH[pbiID];
   //Get the associated appID
   QString appID = pbi.metaID;
   QStringList info = sysDB->installedPbiInfo(pbiID);
   if(useDB && !info.isEmpty()){
     //Now get additional database info
     bool autoUp = sysDB->installedPbiAutoUpdate(pbiID);
     bool root = sysDB->installedPbiNeedsRoot(pbiID);
     bool desktop = sysDB->installedPbiHasXdgDesktop(info[6]);
     bool menu = sysDB->installedPbiHasXdgMenu(info[6]);
     bool mime = sysDB->installedPbiHasXdgMime(info[6]);
     //Now add this info to the PBI structure
     pbi.name    = info[0];
     pbi.version = info[1];
     pbi.arch    = info[2];
     pbi.mdate   = info[3];
     pbi.author  = info[4];
     pbi.website = info[5];
     if(pbi.website.endsWith("/")){ pbi.website.chop(1); }
     pbi.path    = info[6];
     pbi.icon    = info[7];
     pbi.maintainer = info[8];
     pbi.description = info[9];
     pbi.fbsdversion = info[10];
     if(appID.isEmpty()){ 
       appID = Extras::nameToID(pbi.name); 
       pbi.metaID = appID;
     } //for new item initialization
     if(APPHASH.contains(appID)){
       pbi.license = APPHASH[appID].license;	   
     }else{
       pbi.license = tr("Unknown");
     }
     pbi.rootInstall = root;
     pbi.autoUpdate  = autoUp;
     pbi.desktopIcons= desktop;
     pbi.menuIcons   = menu;
     pbi.mimetypes = mime;
     //Clean up the mdate to make it human-readable
     QDate date(pbi.mdate.left(4).toInt(), pbi.mdate.mid(4,2).toInt(), pbi.mdate.right(2).toInt() );
     pbi.mdate =date.toString(Qt::SystemLocaleShortDate); //put it in the current locale format (short version)
   }else{
     //Pull basic info from the pre-loaded App database instead
     // This is for application entries still in a pending state and not fully installed
     if(!APPHASH.contains(appID)){ return; }
     pbi.name = APPHASH[appID].name;
     //Do not change the version or arch - this is usually why the app is in a pending state
     pbi.author = APPHASH[appID].author;
     pbi.website = APPHASH[appID].website;
     pbi.icon = APPHASH[appID].localIcon;
     pbi.license = APPHASH[appID].license;
     pbi.rootInstall = APPHASH[appID].requiresroot;
     pbi.autoUpdate=FALSE;
     pbi.desktopIcons=FALSE;
     pbi.menuIcons=FALSE;
     if(pbi.metaID.isEmpty()){ pbi.metaID = appID; }
   }
   //Now add this pbi structure back into the hash
   PBIHASH.insert(pbiID, pbi);  
   //Now update the status
   updateStatus(pbiID);
 }
 
 void PBIBackend::slotUpdateAllStatus(){
   QStringList pbiID = PBIHASH.keys();
   for(int i=0; i<pbiID.length(); i++){
     updateStatus(pbiID[i]);	 
   }
 }
 
 void PBIBackend::updateStatus(QString pbiID){
   if(!PBIHASH.contains(pbiID)){ return; }
   QString upgrade = upgradeAvailable(pbiID);
   QString chk = pbiID+":::"; //for list checking
   QString iIndex = PENDINGINSTALL.filter(chk).join(" "); //special case for install list
   if(cDownload == pbiID){PBIHASH[pbiID].setStatus(InstalledPBI::DOWNLOADING);}
   else if(cInstall == pbiID){PBIHASH[pbiID].setStatus(InstalledPBI::INSTALLING);}
   else if(cRemove == pbiID){PBIHASH[pbiID].setStatus(InstalledPBI::REMOVING);}
   else if(cUpdate == pbiID){PBIHASH[pbiID].setStatus(InstalledPBI::UPDATING);}
   //Look through the pending lists
   else if(PENDINGDL.join(" ").contains(chk)){PBIHASH[pbiID].setStatus(InstalledPBI::PENDINGDOWNLOAD);}
   else if(!iIndex.isEmpty()){ //install queue can also have special-case removals
     if(iIndex.contains("pbi_delete")){ PBIHASH[pbiID].setStatus(InstalledPBI::PENDINGREMOVAL); }
     else{ PBIHASH[pbiID].setStatus(InstalledPBI::PENDINGINSTALL); }
   }else if(PENDINGREMOVAL.join(" ").contains(chk)){PBIHASH[pbiID].setStatus(InstalledPBI::PENDINGREMOVAL);}
   else if(PENDINGUPDATE.join(" ").contains(chk)){PBIHASH[pbiID].setStatus(InstalledPBI::PENDINGUPDATE);}
   //else if(PENDINGOTHER.join(" ").contains(chk)){PBIHASH[pbiID].setStatus(InstalledPBI::WORKING);}
   else if( !upgrade.isEmpty() ){PBIHASH[pbiID].setStatus(InstalledPBI::UPDATEAVAILABLE); }
   else{ PBIHASH[pbiID].setStatus(InstalledPBI::NONE); }
 }
 
 void PBIBackend::syncCurrentRepo(){
  //Calling this function will automatically clear and re-populate the APPHASH and CATHASH fields
  APPHASH.clear(); CATHASH.clear();
  QString mFile = sysDB->metaFilePath(); 
  QString iFile = sysDB->indexFilePath();
  if( QFile::exists(mFile) && QFile::exists(iFile) ){
   //First do the meta data (app/cat info)
   QStringList metaFile = Extras::readFile(mFile);
   QStringList catsUsed, catsAvail;
   //qDebug() << "Sync Meta File Info";
   for(int i=0; i<metaFile.length(); i++){
     if(metaFile[i].startsWith("App=")){
       QStringList info = sysDB->parseAppMetaLine(metaFile[i].section("=",1,50,QString::SectionSkipEmpty));
       //qDebug() << "App Meta data:" << info;
       if(!info.isEmpty()){ //Make sure the info list is valid
         //info[name,category,remoteIcon,author,website,license,apptype,tags,description,requiresroot] (5/1/2013)
         //Simplify a couple pieces of info
         QString metaID = Extras::nameToID(info[0]);
         QString localIcon = sysDB->remoteToLocalIcon(info[0],info[2]);
         //Create the container and fill it
         MetaPBI app;
         app.name=info[0]; app.category=info[1]; app.remoteIcon=info[2];
         app.localIcon=localIcon; app.author=info[3]; app.website=info[4];
         app.license=info[5]; app.appType=info[6]; app.tags=info[7].toLower().split(","); 
         app.description=info[8];
         if(info[9]=="true"){ app.requiresroot=TRUE; }
         else{ app.requiresroot=FALSE; }
	 app.dateadded=info[10];
	 app.maintainer=info[11];
	 app.shortdescription=info[12];
         //Fix the website if needed
         if(app.website.endsWith("/")){ app.website.chop(1); }
         //Add it to the hash
         APPHASH.insert(metaID,app);
       }
     }else if(metaFile[i].startsWith("Cat=")){
       QStringList info = sysDB->parseCatMetaLine(metaFile[i].section("=",1,50,QString::SectionSkipEmpty));
       //qDebug() << "Cat Meta Data:" << info;
       if(info.length() > 2){ //Make sure the info list is complete
         //info[name,remoteicon,description,?] (5/1/2013)
         QString catID = Extras::nameToID(info[0]);
         QString localIcon = sysDB->remoteToLocalIcon(info[0],info[1]);
         //Create the container and fill it
         MetaCategory cat;
         cat.name=info[0]; cat.remoteIcon=info[1]; cat.localIcon=localIcon; cat.description=info[2];
         //Add it to the available list
         catsAvail << catID;
         //Add it to the hash
         CATHASH.insert(catID,cat);
       }
     }
     //qDebug() << " - done with meta line";
   }
   //qDebug() << "Sync Index File Info";
   //Then do the list of available PBI's
   QStringList indexFile = Extras::readFile(iFile);
   bool sys64 = (sysArch=="amd64");
   for(int i=0; i<indexFile.length(); i++){
     QStringList info = sysDB->parseIndexLine(indexFile[i]);
       //info[name, arch, version, datetime, size, isLatest(true/false)]
     if(!info.isEmpty()){
       QString metaID = Extras::nameToID(info[0]);
       if(APPHASH.contains(metaID)){
       	 bool islatest = (info[5]=="true");
       	 bool is64 = (info[1] == "amd64");
       	 bool save=FALSE;
       	 //Determine whether to save the data or not
       	 if( !sys64 && is64 ){}   // do not save 64-bit apps on a 32-bit system
       	 else if(info[1] == sysArch){ save=TRUE; }  // high priority for identical architecture
       	 else{ //lower priority for 32-bit App on 64-bit system
       	   if(islatest){ 
       	     if(APPHASH[metaID].latestDatetime.isEmpty()){ save=TRUE; } //nothing saved yet, go ahead
       	     else if(APPHASH[metaID].latestArch==sysArch){} // do not save over an app that is the proper arch
       	     else if(Extras::newerDateTime(info[3], APPHASH[metaID].latestDatetime) ){ save=TRUE; } //save over older app
       	   }else{
       	     if(APPHASH[metaID].latestVersion==info[2]){} //do not save the same backup PBI as the latest PBI 
	     else if(APPHASH[metaID].backupDatetime.isEmpty()){ save=TRUE; } //nothing saved yet, go ahead
       	     else if(APPHASH[metaID].backupArch==sysArch){} // do not save over an app that is the proper arch
       	     else if(Extras::newerDateTime(info[3], APPHASH[metaID].backupDatetime) ){ save=TRUE; } //save over older app
       	   }
       	 }
       	 //Save the data if appropriate
       	 if(save && islatest){
       	   APPHASH[metaID].latestVersion=info[2];
       	   APPHASH[metaID].latestDatetime=info[3];
       	   APPHASH[metaID].latestArch=info[1];  
       	   APPHASH[metaID].latestSizeK=info[4];
       	   APPHASH[metaID].latestFilename = info[6];
       	 }else if(save){
       	   APPHASH[metaID].backupVersion=info[2];
       	   APPHASH[metaID].backupDatetime=info[3];
       	   APPHASH[metaID].backupArch=info[1]; 
       	   APPHASH[metaID].backupSizeK=info[4];
       	   APPHASH[metaID].backupFilename=info[6];
       	 } 
       	 //if(save){ qDebug() << "APP:" << metaID << info[1] << info[2] << info[3] << info[4] << info[5]; }
       } //end check for ID in hash
     } //end check for info available
   } //end loop over index file
   
   //Now clean up the APPHASH to remove any entries that do not have a PBI associated with them
   QStringList apps = APPHASH.keys();
   for(int i=0; i<apps.length(); i++){
     if(APPHASH[apps[i]].latestVersion.isEmpty()){
       //Make sure there is not a backup version we can use instead
       if(APPHASH[apps[i]].backupVersion.isEmpty()){
         APPHASH.remove(apps[i]);	     
       }else{
         //Move over the backup to the latest
	 APPHASH[apps[i]].latestVersion=  APPHASH[apps[i]].backupVersion;
       	 APPHASH[apps[i]].latestDatetime= APPHASH[apps[i]].backupDatetime;
       	 APPHASH[apps[i]].latestArch=     APPHASH[apps[i]].backupArch;  
       	 APPHASH[apps[i]].latestSizeK=    APPHASH[apps[i]].backupSizeK;
       	 APPHASH[apps[i]].latestFilename= APPHASH[apps[i]].backupFilename;
	 //Now clear the backup
	 APPHASH[apps[i]].backupVersion.clear();
       	 APPHASH[apps[i]].backupDatetime.clear();
       	 APPHASH[apps[i]].backupArch.clear(); 
       	 APPHASH[apps[i]].backupSizeK.clear();
       	 APPHASH[apps[i]].backupFilename.clear();
       }
     }else{
       //Category being used, remove this category from the list
       int catID = catsAvail.indexOf( Extras::nameToID(APPHASH[apps[i]].category) );
       if(catID != -1){ catsAvail.removeAt(catID); }
     }
   }
   //Now remove any empty categories (ones left over in catAvail list)
   for(int i=0; i<catsAvail.length(); i++){
     if( CATHASH.contains(catsAvail[i]) ){
       //qDebug() << " - Empty category:" << catsAvail[i];
       CATHASH.remove(catsAvail[i]); 
     }
   }
  } // end check for both files existing
  
  if(APPHASH.isEmpty() && CATHASH.isEmpty()){
    numAvailable = -1;
    emit NoRepoAvailable();   	   
  }else{
    numAvailable = QStringList(APPHASH.keys()).length(); //update the number of apps available
    emit RepositoryInfoReady();
  }
 }
 
void PBIBackend::startFileSystemWatcher(){
   QStringList watched = watcher->directories();
   if(!watched.isEmpty()){ watcher->removePaths(watched); }//clear the watcher first
   watcher->addPath( DBDir+"installed" ); //look for installed app changes
   watcher->addPath( DBDir+"repos" ); //look for repo changes
   watcher->addPath( DBDir+"index" ); //look for index/meta file changes	 
}

void PBIBackend::stopFileSystemWatcher(){
   QStringList watched = watcher->directories();
   if(!watched.isEmpty()){ watcher->removePaths(watched); }//clear the watcher first	 
}
