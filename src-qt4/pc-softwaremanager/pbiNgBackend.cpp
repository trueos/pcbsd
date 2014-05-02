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
 #include "pbiNgBackend.h"

 
 PBIBackend::PBIBackend(QWidget *parent) : QObject(){
   parentWidget = parent;
   sysArch = Extras::getSystemArch();
   sysUser = Extras::getRegularUser();
   autoDE = false; //automatically create desktop entries after an install
   PKGRUN.clear(); //make sure this is empty initially
   appAvailable  = -1; //quick default
   pkgAvailable = -1; //quick default
	 
   pkgProc = new DLProcess(this);
     pkgProc->setParentWidget(parentWidget);
     pkgProc->setDLType("PKG");
     connect(pkgProc, SIGNAL(UpdateMessage(QString)), this, SLOT(procMessage(QString)) );
     connect(pkgProc, SIGNAL(UpdatePercent(QString,QString,QString)), this, SLOT(procPercent(QString,QString,QString)) );
     connect(pkgProc, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(procFinished(int, QProcess::ExitStatus)) );
     
   sysDB = new PBIDBAccess();
   //Now startup the syncing process
   slotSyncToDatabase();
   //QTimer::singleShot(1,this,SLOT(slotSyncToDatabase()) );
 }
 
 // ==============================
 // ====== PUBLIC FUNCTIONS ======
 // ==============================
void PBIBackend::setAutoInstallDesktopEntries(bool aide){
  autoDE = aide; //Auto Install Desktop Entries
}

QStringList PBIBackend::installedList(){
   QStringList KL = APPHASH.keys();
   QStringList out;
     for(int i=0; i<KL.length(); i++){
       if(APPHASH[KL[i]].isInstalled){ out << KL[i]; }
     }
   return out; 
}
 
QStringList PBIBackend::pendingInstallList(){
  QStringList out;
  for(int i=0; i<PENDING.length(); i++){
    if(PENDING[i].contains("pc-pkg add ") || PENDING[i].contains("pbi_add ")){
      out << PENDING[i].section("::::",0,0);
    }
  }
  return out;
}

QStringList PBIBackend::pendingRemoveList(){
  QStringList out;
  for(int i=0; i<PENDING.length(); i++){
    if(PENDING[i].contains("pc-pkg remove ") || PENDING[i].contains("pbi_delete ")){
      out << PENDING[i].section("::::",0,0);
    }
  }
  return out;	
}

QStringList PBIBackend::browserCategories(){
   return QStringList( CATHASH.keys() ); 
}
  
QStringList PBIBackend::browserApps( QString catID ){
  if(!CATHASH.contains(catID)){ return QStringList(); }
  QStringList apps = APPHASH.keys();
  apps.append(PKGHASH.keys());
  apps = apps.filter(catID+"/"); //catID should be the raw port category
  apps.removeDuplicates();
  return apps;
}

QStringList PBIBackend::getRecommendedApps(){ //list all PC-BSD recommended applications
  return RECLIST; //add randomization later
}

QStringList PBIBackend::getHighlightedApps(){ //list highlighted apps
  return HIGHLIST; //add randomization later
}

QStringList PBIBackend::getNewApps(){ //list new applications
  return NEWLIST; //add randomization later
}

bool PBIBackend::safeToQuit(){
  //returns true if there is no pending/current processes
  bool ok = ( PENDING.isEmpty() && PKGRUN.isEmpty() );
  return ok;
}
// ===== Local/Repo Interaction Functions =====
bool PBIBackend::isInstalled(QString appID){
  bool installed = false;
  if(APPHASH.contains(appID)){
    installed = APPHASH[appID].isInstalled;
  }else if(PKGHASH.contains(appID)){
    installed = PKGHASH[appID].isInstalled;
  }
  return installed;
}

QString PBIBackend::upgradeAvailable(QString appID){
  QString output;
  if(APPHASH.contains(appID)){
    if(APPHASH[appID].version != APPHASH[appID].installedversion){
      output = APPHASH[appID].version;
    }
  }else if(PKGHASH.contains(appID)){
    if(PKGHASH[appID].version != PKGHASH[appID].installedversion){
      output = APPHASH[appID].version;
    }	  
  }
  return output;
}

// === PBI Actions ===
void PBIBackend::cancelActions(QStringList appID){
  qDebug() << "Cancel Actions requested for:" << appID;
  for(int i=0; i<appID.length(); i++){
    for(int p=0; p<PENDING.length(); p++){
      if(PENDING[p].startsWith(appID[i]+"::::")){ PENDING.removeAt(p); p--; }
    }
    if( PKGRUN==appID[i] && !PROCCANCELLED ){
      //Currently running, don't stop the process since this can do damage to the database
	//Just make sure the next process that runs reverses the current process
	QString cmd = PKGCMD;
	if(PROCTYPE==0 ){ cmd =cmd.replace("pbi_add ", "pbi_remove "); }
	else if(PROCTYPE==0 && cmd.contains("pc-pkg ") ){ cmd = cmd.replace("pc-pkg add ", "pc-pkg remove "); }
	else if(PROCTYPE==1 && cmd.contains("pc-pkg ") ){ cmd = cmd.replace("pc-pkg remove ", "pc-pkg add "); }
	else if(PROCTYPE==1){ cmd=cmd.replace("pbi_remove ", "pbi_add "); }
        if(PROCTYPE >= 0){ PENDING.prepend(PKGRUN+"::::"+cmd+"::::"+PKGJAIL); }
    }
  }
}

void PBIBackend::removePBI(QStringList appID, QString injail){
  qDebug() << "PBI Removals requested for:" << appID;
  bool jailok = RUNNINGJAILS.contains(injail) && JAILPKGS.contains(injail);
  QStringList cancelList;
  for(int i=0; i<appID.length(); i++){
    NGApp app;
    if(APPHASH.contains(appID[i])){ app = APPHASH[appID[i]]; }
    else if(PKGHASH.contains(appID[i])){ app = PKGHASH[appID[i]]; }
    else{ continue; }
    bool jailpkgok = false;
    if(jailok){ jailpkgok = !JAILPKGS[injail].contains(app.origin); }
      if( !app.isInstalled || jailpkgok ){
	//Not a fully-installed PBI - cancel it instead (probably pending)
	cancelList << appID[i];
      }else if( app.rdependancy.contains("pcbsd-base") ){
	qDebug() << "PC-BSD base dependency:" << appID[i] << " - cannot remove";	      
      }else{
	queueProcess(appID[i], false, injail);
        emit PBIStatusChange(appID[i]);
      }
  }
  //If there are PBIs to cancel, do that too
  if(!cancelList.isEmpty()){ cancelActions(cancelList); }
  //Now check/start the remove process
  QTimer::singleShot(0,this,SLOT(checkProcesses()) );
}

void PBIBackend::installApp(QStringList appID, QString injail){
  qDebug() << "Install App requested for:" << appID;
  bool jailok = RUNNINGJAILS.contains(injail) && JAILPKGS.contains(injail);
  for(int i=0; i<appID.length(); i++){
    NGApp app;
    if(APPHASH.contains(appID[i])){ app = APPHASH[appID[i]]; }
    else if(PKGHASH.contains(appID[i])){ app = PKGHASH[appID[i]]; }
    else{
      qDebug() << appID[i] << "is not a valid application";
      continue; //go to the next item is this one is invalid
    } 
    bool jailpkgok = false;
    if(jailok){ jailpkgok = !JAILPKGS[injail].contains(app.origin); }
    if( !app.isInstalled || jailpkgok ){
      queueProcess(appID[i], true, injail);
      emit PBIStatusChange(appID[i]);
    }else{
      qDebug() << appID[i] << "is already installed!";
    }
  } // end of loop over items
  //Now check/start the remove process
  QTimer::singleShot(0,this,SLOT(checkProcesses()) );
  //Now emit the signal that items have changed or been added
  emit LocalPBIChanges();
}

void PBIBackend::lockApp(QStringList appID, QString injail){
  for(int i=0; i<appID.length(); i++){
    NGApp app;
    if(APPHASH.contains(appID[i])){ app = APPHASH[appID[i]]; }
    else if(PKGHASH.contains(appID[i])){ app = PKGHASH[appID[i]]; }
    else{ continue; }
    if(app.isInstalled && !app.isLocked){
      //Run lock/unlock commands ASAP since they take no time at all, but have to be in the pkg queue
      QString cmd;
      if(injail.isEmpty() || !RUNNINGJAILS.contains(injail) ){ cmd = "pc-pkg lock -y "+appID[i]; injail.clear();}
      else{ cmd = "pc-pkg -j "+RUNNINGJAILS[injail]+" lock -y "+appID[i]; }
      PENDING.prepend(appID[i]+"::::"+cmd+"::::"+injail);
    }
  }
  QTimer::singleShot(0,this,SLOT(checkProcesses()) );
}

void PBIBackend::unlockApp(QStringList appID, QString injail){
  for(int i=0; i<appID.length(); i++){
    NGApp app;
    if(APPHASH.contains(appID[i])){ app = APPHASH[appID[i]]; }
    else if(PKGHASH.contains(appID[i])){ app = PKGHASH[appID[i]]; }
    else{ continue; }
    if(app.isInstalled && app.isLocked){
      //Run lock/unlock commands ASAP since they take no time at all, but have to be in the pkg queue
      QString cmd;
      if(injail.isEmpty() || !RUNNINGJAILS.contains(injail) ){ cmd = "pc-pkg unlock -y "+appID[i]; injail.clear();}
      else{ cmd = "pc-pkg -j "+RUNNINGJAILS[injail]+" unlock -y "+appID[i]; }
      PENDING.prepend(appID[i]+"::::"+cmd+"::::"+injail);
    }
  }
  QTimer::singleShot(0,this,SLOT(checkProcesses()) );	
}

void PBIBackend::addDesktopIcons(QStringList pbiID, bool allusers){ // add XDG desktop icons
  for(int i=0; i<pbiID.length(); i++){
    if( !APPHASH.contains(pbiID[i]) ){continue;}
    if( APPHASH[pbiID[i]].hasDE && APPHASH[pbiID[i]].isInstalled ){
      //generate the command
      QString cmd = "pbi_icon add-desktop "+pbiID[i];
      if(!allusers){
        runCmdAsUser(cmd);
      }else{
	QProcess::startDetached(cmd);      
      }
    }
  }
}

void PBIBackend::rmDesktopIcons(QStringList pbiID, bool allusers){ // remove XDG desktop icons
  for(int i=0; i<pbiID.length(); i++){
    if( !APPHASH.contains(pbiID[i]) ){continue;}
    if( APPHASH[pbiID[i]].hasDE && APPHASH[pbiID[i]].isInstalled ){
      //generate the command
      QString cmd = "pbi_icon del-desktop "+pbiID[i];
      if(!allusers){
        runCmdAsUser(cmd);
      }else{
	QProcess::startDetached(cmd);      
      }
    }
  }
}

// INFO FUNCTIONS
NGApp PBIBackend::singleAppInfo( QString app){
  if(APPHASH.contains(app)){
    return APPHASH[app];
  }else if(PKGHASH.contains(app)){
    return PKGHASH[app];
  }else{
    return NGApp();
  }
}

NGCat PBIBackend::singleCatInfo( QString cat ){
  if(CATHASH.contains(cat)){
    return CATHASH[cat];
  }else{
    return NGCat();
  }
}

QList<NGCat> PBIBackend::CatInfo( QStringList catID ){
  QList<NGCat> output;
  for(int i=0; i<catID.length(); i++){
    if(CATHASH.contains(catID[i])){ output << CATHASH[catID[i]]; }
  }
  return output;
}

QList<NGApp> PBIBackend::AppInfo( QStringList appID){
  QList<NGApp> output;
  for(int i=0; i<appID.length(); i++){
    if(APPHASH.contains(appID[i])){ output << APPHASH[appID[i]]; }
    else if(PKGHASH.contains(appID[i])){ output <<PKGHASH[appID[i]]; }
  }
  return output;
}

QString PBIBackend::currentAppStatus( QString appID ){
  QString output;
  if(appID == PKGRUN){ output = PKGRUNSTAT; } //currently running
  else{
    for(int i=0; i<PENDING.length(); i++){
      if(PENDING[i].startsWith(appID+"::::")){
        //Currently pending - check which type (install/remove)
	if(PENDING[i].contains("pbi_add") || PENDING[i].contains("pc-pkg add") ){ output = tr("Pending Installation"); }
	else if(PENDING[i].contains("pbi_delete") || PENDING[i].contains("pc-pkg remove") ){ output = tr("Pending Removal"); }
	return output;
      }
    }
    //If it gets here, it is not pending, so check for updates
    NGApp app;
    if(APPHASH.contains(appID)){ app = APPHASH[appID]; }
    else if(PKGHASH.contains(appID)){ app = PKGHASH[appID]; }
    if(!app.origin.isEmpty()){
      if(app.version != app.installedversion && app.isInstalled){
	 output = QString(tr("Update Available: %1")).arg(app.version);
      }
    }
  }
  return output;
}

bool PBIBackend::isWorking(QString pbiID){
  bool working = (pbiID==PKGRUN);
  for(int i=0; i<PENDING.length() && !working; i++){
    working = PENDING[i].startsWith(pbiID+"::::");
  }
  return working;
}

QStringList PBIBackend::appBinList(QString appID){ //<name>::::<*.desktop file path>
  if(!APPHASH.contains(appID)){ return QStringList(); }
  QStringList files = sysDB->AppMenuEntries(APPHASH[appID]);
  for(int i=0; i<files.length(); i++){
    //Get the translated name and format the line
    XDGFile info;
      info.LoadDesktopFile(files[i]);
    files[i].prepend(info.Name()+"::::");
  }
  return files;
}

// === Import/Export PBI Lists ===
bool PBIBackend::exportPbiListToFile(QString filepath){
  bool ok = FALSE;
  //get the currently installed PBI's
  QStringList list = APPHASH.keys();
  QStringList installed;
  for(int i=0; i<list.length(); i++){
    if(APPHASH[list[i]].isInstalled){ installed << list[i]; }	  
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
    if(inlist[i].isEmpty() || inlist[i].startsWith("#")){ continue; } //skip it
    if(APPHASH.contains(inlist[i])){
      if( !APPHASH[inlist[i]].isInstalled ){ good << inlist[i]; }
      else{ current << inlist[i]; }
    }else{
      bad << inlist[i];
    }
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

QStringList PBIBackend::runningJails(){
  QStringList jails = RUNNINGJAILS.keys();	
  return jails;
}

QStringList PBIBackend::jailsWithoutPkg(QString appID){
  //return the list of current jails which do not have the given pkg installed
  QStringList out;
  QStringList jails = JAILPKGS.keys();
  for(int i=0; i<jails.length(); i++){
    //qDebug() << jails[i] << JAILPKGS[jails[i]] << appID;
    if( !JAILPKGS[jails[i]].contains(appID) ){ out << jails[i]; }
  }
  qDebug() << "Valid jails:" << out;
  return out;
}

void PBIBackend::runCmdAsUser(QString cmd){
  if(sysUser.isEmpty()){ return; }
  QStringList  args; args << "-m" << sysUser << "-c" << cmd;
  QProcess::startDetached("su",args); //run as the user, not as root
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
void PBIBackend::queueProcess(QString origin, bool install, QString injail){
  QString cmd;
  if(install && APPHASH.contains(origin) ){ cmd = "pbi_add "; }
  else if(install && PKGHASH.contains(origin) ){ cmd = "pc-pkg add "; }
  else if(APPHASH.contains(origin)){ cmd = "pbi_delete "; }
  else if(PKGHASH.contains(origin)){ cmd = "pc-pkg remove "; }
  if(cmd.isEmpty()){ return; } //invalid app
  if(!injail.isEmpty() && RUNNINGJAILS.contains(injail)){
    cmd.append("-j "+RUNNINGJAILS[injail]+" "); //Make sure to use the Jail ID number
  }else{injail.clear(); }
  cmd.append(origin);
  PENDING << origin+"::::"+cmd+"::::"+injail;
}
	
 void PBIBackend::checkForJails(QString jail){
  if(jail.isEmpty() || !RUNNINGJAILS.contains(jail)){
    //Re-sync to currently running jails
    QStringList out = Extras::getCmdOutput("jls");
    RUNNINGJAILS.clear();
    JAILPKGS.clear();
    //qDebug() << "Jail Check:" << out;
    for(int i=1; i<out.length(); i++){ //skip the first line since it is just the header
      if(out[i].isEmpty()){ continue; }
      out[i] = out[i].replace("\t"," ").simplified();
      QString jail = out[i].section(" ",2,2);
      QString ID = out[i].section(" ",0,0);
      //qDebug() << "jail:"<<jail<<"ID:" << ID;
      if( !jail.isEmpty() && !ID.isEmpty() ){
        RUNNINGJAILS.insert( jail, ID ); // <name, ID>
	QStringList pkgs = sysDB->listJailPackages(ID);
	qDebug() << "Jail Pkgs:" << jail << ID << pkgs;
        JAILPKGS.insert(jail, pkgs);
      }
    }
  }else{
    //Just update the installed list for the given jail
    QStringList pkgs = sysDB->listJailPackages(RUNNINGJAILS[jail]);
    JAILPKGS.insert(jail, pkgs);
  }
}
 
 // ===============================
 // ======   PRIVATE SLOTS   ======
 // ===============================
 // Internal Process Management
 void PBIBackend::checkProcesses(){
   //Check whether to startup the next pending process
   if( !PKGRUN.isEmpty() || PENDING.isEmpty() ){ return; } //currently running or nothing to do

   //Parse the next PENDING command
   PKGRUN = PENDING[0].section("::::",0,0);
   PKGCMD = PENDING[0].section("::::",1,1);
   PKGJAIL = PENDING[0].section("::::",2,2);
   PENDING.removeAt(0); //remove this from the pending list
   if( PKGCMD.startsWith("pbi_add") || PKGCMD.startsWith("pc-pkg add") ){ PROCTYPE = 0; } //install
   else if( PKGCMD.startsWith("pbi_delete") || PKGCMD.startsWith("pc-pkg remove") ){ PROCTYPE = 1; } //remove
   else{ PROCTYPE = -1; } //other type of command (no special checks later)
   
   PROCCANCELLED = false;
   PKGRUNSTAT.clear();
   PROCLOG.clear();
   bool injail = !PKGJAIL.isEmpty();
   //Check that this is a valid entry/command
   bool skip = false; //need to skip this PENDING entry for some reason
   if( !APPHASH.contains(PKGRUN) && !PKGHASH.contains(PKGRUN) ){ skip = true; qDebug() << "pkg not on repo";} //invalid pkg on the repo
   else if( PROCTYPE==0 && APPHASH[PKGRUN].isInstalled && !injail ){ skip = true; qDebug() << "already installed"; } //already installed
   else if( PROCTYPE==1 && !APPHASH[PKGRUN].isInstalled && !injail ){ skip = true; qDebug() << "already uninstalled"; } //not installed
   if(skip){
    qDebug() << "Requested Process Invalid:" << PKGRUN << PKGCMD;
    PKGRUN.clear();
    PKGCMD.clear();
    QTimer::singleShot(1,this,SLOT(checkProcesses()) ); //restart this function to check the next command
    return;
   }
   //Now run any pre-remove commands (if not an in-jail removal, or raw pkg mode)
   if(PROCTYPE==1 && !injail && PKGCMD.startsWith("pc-pkg ") ){
     Extras::getCmdOutput("pbi_icon del-desktop del-menu del-mime "+PKGRUN); //don't care about result
   }
   qDebug() << "Starting Process:" << PKGRUN << PKGCMD;
   //Set the new status
   if(PROCTYPE==0){ PKGRUNSTAT=tr("Starting Installation"); }
   else if(PROCTYPE==1){ PKGRUNSTAT=tr("Starting Removal"); }
   else{ PKGRUNSTAT.clear(); }
   emit PBIStatusChange(PKGRUN);
   
   //Now start the command
   emit devMessage( "** Starting Process: \""+PKGCMD+"\" **" );
   pkgProc->start(PKGCMD);

}
 
void PBIBackend::procMessage(QString msg){
  qDebug() << "MSG:" << msg;
  PROCLOG << msg;   //save full message to the log for later
  QString tmp;
  //Do some quick parsing of the message for better messages
  if(msg.startsWith("[")){
    //Is installing, get the percent
    tmp = msg.section("]",0,0).remove("[").simplified();
    double percent = tmp.section("/",0,0).toDouble()/tmp.section("/",-1).toDouble();
    percent = percent*100;
    if(PROCTYPE==0){
      tmp = QString(tr("Installing: %1")).arg(QString::number( (int) percent )+"%");
    }else if(PROCTYPE==1){
      tmp = QString(tr("Removing: %1")).arg(QString::number( (int) percent )+"%");
    }
  }
  if(!tmp.isEmpty()){
    PKGRUNSTAT = tmp; //set this as the current status (might want to do some parsing/translation later)
    emit PBIStatusChange(PKGRUN);
  }
  emit devMessage(msg);
}

void PBIBackend::procPercent(QString percent, QString size, QString filename){ //percent, file size, filename
  PKGRUNSTAT = QString( tr("Downloading %1 (%2 of %3)")).arg(filename, percent, size);
  qDebug() << "MSG:" << PKGRUNSTAT;
  //don't save this to the log - can get tons of these types of messages for every percent update
  emit PBIStatusChange(PKGRUN);
}

void PBIBackend::procFinished(int ret, QProcess::ExitStatus stat){
  emit devMessage("** Process Finished **");
  if(stat != QProcess::NormalExit){
    //Process Crashed
    emit Error(tr("Process Crashed"), QString(tr("The process for %1 has quit unexpectedly. Please restart this operation at a later time.")).arg(PKGRUN), PROCLOG);
  }else if( ret != 0 ){
    //Failure
    QString title, msg;
      if(PROCTYPE==0){ 
	title = tr("Installation Failure"); 
	msg = QString(tr("The following application installation experienced an error: %1")+"\n\n"+tr("Please try again later.")).arg(APPHASH[PKGRUN].name);
      }else if(PROCTYPE==1){ 
	title = tr("Removal Failure"); 
	msg = QString(tr("The following application removal experienced an error: %1")+"\n\n"+tr("Please try again later.")).arg(APPHASH[PKGRUN].name);
      }
      if(!msg.isEmpty()){ emit Error(title, msg, PROCLOG); }
  }else{
    //Success - perform any cleanup operations
    if(PROCTYPE==0 && PKGCMD.contains("pbi_") && !PROCCANCELLED){ //if new installation on main system
      Extras::getCmdOutput("pbi_icon add-menu add-mime "+PKGRUN); //don't care about result
      if(autoDE && APPHASH[PKGRUN].hasDE){ runCmdAsUser("pbi_icon add-desktop "+PKGRUN); }
    }
	  
  }
  //Now clean up the process variables and update the app status
  if(PKGJAIL.isEmpty()){
    //update the local system info
    slotSyncToDatabase(true);
  }else{
    //Just update the pkg list for the jail
    checkForJails(PKGJAIL);
  }
  QString origin = PKGRUN; //temporary
  PKGCMD.clear();
  PKGRUN.clear();
  PKGJAIL.clear();
  //Emit the proper signals
  emit PBIStatusChange(origin);
  emit LocalPBIChanges(); //so that it knows to look for a different install list
  //Now check for the next command to run
  QTimer::singleShot(1, this, SLOT(checkProcesses()) );
}
	

 // === Database Synchronization ===
 void PBIBackend::slotSyncToDatabase(bool localChanges){
   qDebug() << "Sync Database with local changes:" << localChanges;
   sysDB->syncDBInfo("", localChanges);
   PKGHASH.clear();
   APPHASH.clear();
   CATHASH.clear();
   sysDB->getAppCafeHomeInfo( &NEWLIST, &HIGHLIST, &RECLIST);
   //qDebug() << "Load APPHASH";
   PKGHASH = sysDB->DetailedPkgList(); // load the pkg info
   APPHASH = sysDB->DetailedAppList(); // load the pbi info

   //qDebug() << "Load CATHASH";
   CATHASH = sysDB->Categories(); // load all the different categories info
   //qDebug() << "Check Jails";
   checkForJails(); //Update the RUNNINGJAILS
   //qDebug() << "Update Stats";
   updateStatistics();
   //qDebug() << "Emit result";
   if(APPHASH.isEmpty()){
     emit NoRepoAvailable();
   }else{
     emit RepositoryInfoReady();
   }
}
 
void PBIBackend::updateStatistics(){
  QStringList avail = APPHASH.keys();
    appAvailable = avail.length();
  avail = PKGHASH.keys();
    pkgAvailable = avail.length();
}
