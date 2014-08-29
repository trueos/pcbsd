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
 #include "pbiDBAccess.h"

PBIDBAccess::PBIDBAccess(){
  proc = new QProcess;
  proc->setProcessEnvironment( QProcessEnvironment::systemEnvironment() );
  proc->setProcessChannelMode(QProcess::MergedChannels);
}

PBIDBAccess::~PBIDBAccess(){
  delete proc;	
}

// ========================================
// =======  PUBLIC ACCESS FUNCTIONS =======
// ========================================
void PBIDBAccess::syncDBInfo(QString jailID, bool localreload, bool allreload){
  //The PBI/Cat lists are the same for all jails
  syncLargePkgRepoList(allreload); //reload the base pkg information (large)
  bool synced = syncPkgInstallList(jailID, localreload || allreload); //load the installed PKG list
  syncPbiRepoLists(localreload || allreload || synced); //load the PBI index lists
}

QHash<QString, NGApp> PBIDBAccess::getRawAppList(){ //PBI-apps that can be installed
  return PBIAVAIL;
}

QHash<QString, NGCat> PBIDBAccess::Categories(){  //All categories for ports/pbi's (unified)
  return CATAVAIL;
}

QHash<QString, NGApp> PBIDBAccess::DetailedAppList(){
  return PBIAVAIL;
}

QHash<QString, NGApp> PBIDBAccess::DetailedPkgList(){
   syncLargePkgRepoList(); //Now fill out the details for all available package (can take a while)
  QHash<QString, NGApp> hash = PKGAVAIL;
  QStringList IK = PKGINSTALLED.keys();
  for(int i=0; i<IK.length(); i++){
    //if( IK[i].contains("sudo") ){ qDebug() << "sudo Installed"; }
    hash.insert( IK[i], PKGINSTALLED[IK[i]] ); //replace the pkg entry with the installed entry
  }
  return hash;
}

QHash<QString, NGApp> PBIDBAccess::JailPkgList(QString jailID){
  syncPkgInstallList(jailID, true); //always force reload this (since it is the installed list - which changes regularly)
  return PKGINSTALLED;
}

NGApp PBIDBAccess::getLocalPkgDetails(NGApp app){
  //Simply set the proper bits in the container for locally installed apps
  // NOTE: This is dependant upon which jail is being probed
  QStringList args;
  if( !jailLoaded.isEmpty() ){
    args << "-j" << jailLoaded;
  }
  args << "query" << "%v::::%sh::::%k::::%t::::%a" << app.origin;
  QString out = runCMD("pkg", args);
  if(out.isEmpty()){
    app.isInstalled=false;
  }else{
    app.isInstalled=true;
    app.installedversion = out.section("::::",0,0);
    app.installedsize = out.section("::::",1,1);
    app.isLocked = (out.section("::::",2,2) == "1");
    QString timestamp = out.section("::::",3,3);
      app.installedwhen = QDateTime::fromMSecsSinceEpoch( timestamp.toLongLong() ).toString(Qt::DefaultLocaleShortDate);
    app.isOrphan = (out.section("::::",4,4) == "1");
  }
  return app;
}


QStringList PBIDBAccess::getRawPkgList(){ //All packages that can be installed
  return QStringList(PKGAVAIL.keys()); 
}

QStringList PBIDBAccess::getRawInstalledPackages(){ //Installed Packages on the system
  return QStringList(PKGINSTALLED.keys());
}
	
NGApp PBIDBAccess::updateAppStatus(NGApp app){
  //This function assumes that the internal lists are currently up-to-date
  bool ok = true;
  if( !app.needsPkgs.isEmpty() ){
    //Check for all these package, not just the current pkg
    for(int i=0; i<app.needsPkgs.length(); i++){
      if( !PKGINSTALLED.contains(app.needsPkgs[i]) ){
	ok = false;
	break; //don't bother checking further
      }
    }
  }
  if(ok){
    app = getLocalPkgDetails(app); //this will check the actual app package
  }else{
    app.isInstalled = false;
  }
  return app;
}

QStringList PBIDBAccess::AppMenuEntries(NGApp app){
  if(app.pbiorigin.isEmpty() || !app.hasME ){ return QStringList(); }
  QDir mdir(PBI_DBDIR+app.pbiorigin+"/xdg-menu");
  QStringList files = mdir.entryList(QStringList() << "*.desktop", QDir::Files | QDir::NoDotAndDotDot, QDir::Name);
  for(int i=0; i<files.length(); i++){
    files[i] = mdir.absoluteFilePath(files[i]);
  }
  return files;
}

void PBIDBAccess::getAppCafeHomeInfo(QStringList *NEW, QStringList *HIGHLIGHT, QStringList *RECOMMEND){
  NEW->clear(); HIGHLIGHT->clear(); RECOMMEND->clear();
  QStringList info = readAppCafeFile();
  //qDebug() << "AppCafe File contents:\n" << info;
  for(int i=0; i<info.length(); i++){
    if(info[i].startsWith("New=")){ NEW->append( info[i].section("::::",0,0).section("=",1,50).simplified() ); }
    else if(info[i].startsWith("Highlight=")){ HIGHLIGHT->append( info[i].section("::::",0,0).section("=",1,50).simplified() ); }
    else if(info[i].startsWith("Recommended=")){ 
      QString origin = info[i].section("::::",0,0).section("=",1,50).simplified();
      if(PKGAVAIL.contains(origin)){
         NGApp app = PKGAVAIL[origin];
	   app.isRecommended = true;
	 PKGAVAIL.insert(origin, app);
      }
      if(PKGINSTALLED.contains(origin)){
         NGApp app = PKGINSTALLED[origin];
	   app.isRecommended = true;
	 PKGINSTALLED.insert(origin, app);
      }
      if(PBIAVAIL.contains(origin)){
         NGApp app = PBIAVAIL[origin];
	   app.isRecommended = true;
	 PBIAVAIL.insert(origin, app);
      }
      RECOMMEND->append( origin ); 
    }
  }
}

QStringList PBIDBAccess::listJailPackages(QString jailID){
  if(jailID.isEmpty()){ return QStringList(); }
  QStringList args; 
    args << "-j" << jailID;
    args << "query" << "-a" << "APP=%o";
    // [origin, installed version, installed size, isLocked, timestamp, isOrphan
    QStringList out = runCMD("pkg",args).split("APP=");
    for(int i=0; i<out.length(); i++){
      out[i] = out[i].simplified(); //remove any extra whitespace
    }
    return out;
}

QStringList PBIDBAccess::basePackageList(){
  return QStringList();
}


// ========================================
// =======  PRIVATE ACCESS FUNCTIONS ======
// ========================================

//------------------
//  SYNCERS
//------------------
bool PBIDBAccess::syncPkgInstallList(QString jailID, bool reload){
  //qDebug() << "Sync Local PKG Repo";
  bool synced = false;
  if(PKGINSTALLED.isEmpty() || reload || (jailLoaded!=jailID) ){
    PKGINSTALLED.clear();
    QStringList args; 
    if( !jailID.isEmpty() ){ args << "-j" << jailID; }
    args << "query" << "-a" << "APP=%o::::%v::::%sh::::%k::::%t::::%a::::%q";
    // [origin, installed version, installed size, isLocked, timestamp, isOrphan
    QStringList out = runCMD("pkg",args).split("APP=");	
    for(int i=0; i<out.length(); i++){
      QStringList info = out[i].split("::::");
      if(info.length() < 7){ continue; } //invalid
      NGApp app;
      if(PKGAVAIL.contains(info[0])){ app = PKGAVAIL[info[0]]; } //start from the current remote info
      app.origin = info[0];
      app.installedversion = info[1];
      app.installedsize = info[2];
      app.isLocked = (info[3] == "1");
      app.installedwhen = QDateTime::fromTime_t( info[4].toLongLong() ).toString(Qt::DefaultLocaleShortDate);
      app.isOrphan = (info[5] == "1");
      app.installedarch = cleanupArch(info[6]);
      app.isInstalled = true;
      PKGINSTALLED.insert(info[0], app);
      //if(!jailID.isEmpty()){ qDebug() << "Installed:" << out[i]; }
    }
    //Now get the reverse dependancy lists
    args.clear(); 
    if( !jailID.isEmpty() ){ args << "-j" << jailID; }
    args << "query" << "-a" << "APP=%o::::%ro";
    out = runCMD("pkg", args).split("APP=");
    //qDebug() << "Get reverse Deps:" << out;
    for(int i=0; i<out.length();i++){
      QStringList info = out[i].split("::::");
      NGApp app;
      if(PKGINSTALLED.contains(info[0])){ app = PKGINSTALLED[info[0]]; } //Update existing info
      else{ continue; } //invalid
      app.rdependency.append( info[1].simplified() );
      PKGINSTALLED.insert(info[0], app);
    }
    jailLoaded = jailID; //keep track of which jail this list is for
    synced = true;
  }
  //qDebug() << " - end Local PKG Repo Sync";
  return synced;
}

void PBIDBAccess::syncLargePkgRepoList(bool reload){
  //Detailed list of packages available on the repo (can take a while)
    //  - use PKGAVAIL as the base template for all the other info classes (save on "pkg" calls)
  //qDebug() << "Sync Remote PKG Repo";
  if(PKGAVAIL.isEmpty() || reload){
    PKGAVAIL.clear();
    QStringList args; args << "rquery" << "-a" << "APP=%o::::%n::::%v::::%m::::%w::::%c::::%e::::%sh::::%q";
    QStringList out = runCMD("pkg",args).split("APP=");	  
    for(int i=0; i<out.length(); i++){
      QStringList info = out[i].split("::::");
       //qDebug() << "PKG:" << info;
	//[ origin, name, version, maintainer, website, comment, description, size]
	if(info.length() < 9){ continue; } //invalid
      NGApp app;
	    app.origin = info[0];
	    app.name = info[1];
	    app.version = info[2];
	    app.maintainer = info[3];
	    app.website = info[4];
	    app.shortdescription = cleanupDescription( info[5].split("\n") );
	    app.description = cleanupDescription( info[6].split("\n") );
	    app.size = info[7];
	    app.arch = cleanupArch(info[8]);
	    app.portcat = info[0].section("/",0,0).simplified();
	    app.type = "pkg"; //always set this by default for raw package
	    //app = getRemotePkgDetails(app);
      PKGAVAIL.insert(info[0], app);
    }
    //Now get all the dependency information for the packages
    args.clear();
    args << "rquery" << "-a" << "APP=%o::::%do";
    out = runCMD("pkg",args).split("APP=");
    for(int i=0; i<out.length(); i++){
      QStringList info = out[i].split("::::");
      if(info.length() < 2){ continue; }
      if(PKGAVAIL.contains(info[0])){
        NGApp app = PKGAVAIL[info[0]];
	      app.dependency.append(info[1].simplified());
	PKGAVAIL.insert(info[0], app);
      }
    }
  }
  //qDebug() << " - end Remote PKG Repo Sync";
}

void PBIDBAccess::syncPbiRepoLists(bool reload){
  // NOTE: Uses the PKGAVAIL and PKGINSTALLED lists  - check your sync order!!
  //qDebug() << "Sync PBI repo";
  //All PBIs/Categories available in the index (jail independant)
  if(PBIAVAIL.isEmpty() || CATAVAIL.isEmpty() || reload){
    PBIAVAIL.clear(); CATAVAIL.clear();
    QStringList index = readIndexFile();
    index.sort(); //make sure that categories are first (Cat=, PBI=)
    for(int i=0; i<index.length(); i++){
      if(index[i].startsWith("Cat=")){
	NGCat cat = parseNgCatLine( index[i].section("=",1,50) );
	if(!cat.portcat.isEmpty()){
	  //qDebug() << "CAT:" << cat.portcat << cat.name;
	  CATAVAIL.insert(cat.portcat, cat);
	}
      }else if(index[i].startsWith("PBI=")){
	
	NGApp app = parseNgIndexLine( index[i].section("=",1,200) );
	//Prune the PBI app based upon package availability
	//qDebug() << "PBI:" << app.origin << PKGAVAILABLE.contains(app.origin);
	if( !app.origin.isEmpty() && PKGAVAIL.contains(app.origin) ){
	  //qDebug() << "PBI Available:" << app.origin << app.name;
	  //Also check for additional required packages
	  bool ok = true;
	  for(int i=0; i<app.needsPkgs.length(); i++){
	    if( !PKGAVAIL.contains(app.needsPkgs[i]) ){ ok = false; qDebug() << "BAD PBI:" << app.origin << app.needsPkgs; break; }
	  }
	  if(!ok && app.isInstalled){ qDebug() <<" - but is installed"; app.isInstalled=false; ok = true; }
	  if(ok){
	    //if(app.isInstalled){ qDebug() << "PBI Installed:" << app.origin << app.name << app.installedversion; }
	    PBIAVAIL.insert(app.origin, app);
	  }
	}
      }else if(index[i].startsWith("PKG=")){
	//Additional PKG info in the PBI index (to save on pkg calls for multi-line info)
	  //Just adds additional info for existing packages
	parseNgPkgLine( index[i].section("=",1,100) );
      }
    }
  } //end sync if necessary
  //qDebug() << " - end PBI repo sync";
}

//-------------------
//   PARSERS
//-------------------
NGApp PBIDBAccess::parseNgIndexLine(QString line){
  //PBI= index line: [port, name, +ports, author, website, license, app type, category, tags, maintainer, shortdesc, fulldesc, screenshots, related, plugins, conf dir]
  // screenshots = list of URL's for screenshots (empty space delimiter? Note "%20"->" " conversion within a single URL)
  // related = list of ports that are similar to this one
  QStringList lineInfo = line.split("::::");
  if(lineInfo.length() < 18){ return NGApp(); } //invalid entry - skip it
  QString orig = lineInfo[0];
  NGApp app;
  if(PKGINSTALLED.contains(orig)){ app = PKGINSTALLED[orig]; } //Try to start with the known info
  else if(PKGAVAIL.contains(orig)){ app = PKGAVAIL[orig]; }
	app.origin = orig;
	app.name = lineInfo[1];
	app.needsPkgs = lineInfo[2].split(" ", QString::SkipEmptyParts);
	app.author = lineInfo[3];
	app.website = lineInfo[4];
	app.license = lineInfo[5];
	app.type = lineInfo[6];
	app.category = app.origin.section("/",0,0);
	app.tags = lineInfo[8].split(",");
	app.maintainer = lineInfo[9];
	app.shortdescription = cleanupDescription( lineInfo[10].split("<br>") );
	app.description = cleanupDescription( lineInfo[11].split("<br>") );
	app.screenshots = lineInfo[12].split(" ", QString::SkipEmptyParts);
	    app.screenshots = app.screenshots.replaceInStrings("%20"," ");
	app.similarApps = lineInfo[13].split(" ", QString::SkipEmptyParts);
	app.possiblePlugins = lineInfo[14].split(" ", QString::SkipEmptyParts);
	app.pbiorigin = lineInfo[15];
        app.buildOptions = lineInfo[16].split(" ",QString::SkipEmptyParts);
	app.rating = lineInfo[17]; //all ratings out of 5 total
	//Now check for different types of shortcuts for this app
	app.hasDE = QFile::exists( PBI_DBDIR+app.pbiorigin+"/xdg-desktop" );
	app.hasME = QFile::exists( PBI_DBDIR+app.pbiorigin+"/xdg-menu" );
	app.hasMT = QFile::exists( PBI_DBDIR+app.pbiorigin+"/xdg-mime" );
	app.hasWiki = true; //PBI-apps have wiki pages
	//Now create the path to the icon in the index
	app.icon = PBI_DBDIR+app.pbiorigin+"/icon.png";
  //qDebug() << "Found App:" << app.name << app.origin;
  return app;
}

NGCat PBIDBAccess::parseNgCatLine(QString line){
  //Cat= index line: [name, icon, description, freebsd category]
  QStringList lineInfo = line.split("::::");
  NGCat cat;
	if(lineInfo.length() < 4){ return cat; } //invalid entry - skip it
	cat.name = lineInfo[0];
	cat.icon = PBI_DBDIR+"PBI-cat-icons/"+lineInfo[1];
	cat.description = cleanupDescription( lineInfo[2].split("<br>") );
	cat.portcat = lineInfo[3].remove(":");
  //qDebug() << "Found Cat:" << cat.name << cat.portcat;
  return cat;
}

void PBIDBAccess::parseNgPkgLine(QString line){
  //Add this additional information to any existing data
  //PKG= [origin, options, licences]
  QStringList info = line.split("::::");
  if(info.length() < 3){ return; }
  if(PBIAVAIL.contains(info[0])){
    NGApp app = PBIAVAIL[info[0]];
      if(!info[1].isEmpty()){ app.buildOptions = info[1].split(" "); }
      if(!info[2].isEmpty()){ app.license = info[2]; }
      PBIAVAIL.insert(info[0], app);
  }
  if(PKGINSTALLED.contains(info[0])){
    NGApp app = PKGINSTALLED[info[0]];
      if(!info[1].isEmpty()){ app.buildOptions = info[1].split(" "); }
      if(!info[2].isEmpty()){ app.license = info[2]; }
      PKGINSTALLED.insert(info[0], app);
  }
  if(PKGAVAIL.contains(info[0])){
    NGApp app = PKGAVAIL[info[0]];
      if(!info[1].isEmpty()){ app.buildOptions = info[1].split(" "); }
      if(!info[2].isEmpty()){ app.license = info[2]; }
      PKGAVAIL.insert(info[0], app);
  }
	
	
}

//----------------------
//   UTILITIES
//-----------------------
QString PBIDBAccess::readOneLineFile(QString path){
  QFile file(path);
  if(!file.exists()){ return ""; } //Return nothing for missing file
  //Now read the file
  QString output;
  if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
    QTextStream in(&file);
    while(!in.atEnd()){
      if(!output.isEmpty()){ output.append("\n"); }
      output.append( in.readLine() ); 
    }
    file.close();
  }
  return output;
}

QString PBIDBAccess::runCMD(QString cmd, QStringList args){
  //Small function to run quick database modification commands or queries
  QString output;
  if(args.isEmpty()){
    proc->start(cmd);
  }else{
    proc->start(cmd, args);
  }
  if(proc->waitForFinished(30000)){
    output = proc->readAll();	   
  }else{
    proc->terminate();
    output = "Process timed out (30 sec)";
  }
  if(output.endsWith("\n")){ output.chop(1); }
  output = output.simplified();
  return output;
}

QStringList PBIDBAccess::cmdOutput(QString cmd, QStringList args){
  //Small function to run quick database modification commands
  QString output;
  if(args.isEmpty()){
    proc->start(cmd);
  }else{
    proc->start(cmd, args);
  }
  if(proc->waitForFinished(60000)){
    output = proc->readAll();	   
  }else{
    proc->terminate();
    output = "Process timed out (60 sec)";
  }
  if(output.endsWith("\n")){ output.chop(1); }
  output = output.simplified();
  return output.split("\n");
}

QString PBIDBAccess::cleanupDescription(QStringList tmp){
  for(int i=1; i<tmp.length(); i++){
    //tmp[i-1] = tmp[i-1].simplified();
    tmp[i] = tmp[i].simplified();
    if( tmp[i].startsWith("WWW: ") ){
      //Remove the website URL from the end, it is already accounted for elsewhere
      tmp.removeAt(i);
      i--;
    }else if(tmp[i-1].isEmpty() || tmp[i].isEmpty() ){}
    else if(tmp[i-1].endsWith(".") || tmp[i-1].endsWith(":") || tmp[i-1].endsWith(";") || tmp[i-1].endsWith("?") || tmp[i-1].endsWith("!") ){}
    else if( tmp[i].startsWith("*") || tmp[i].startsWith("0") || tmp[i].startsWith("-") || tmp[i].startsWith("o ") ){}
    else{
      //if(DEBUG){ qDebug() << " - Bad Line Break:\n" << tmp[i-1] << "<br>" << tmp[i]; }
      //Bad line break, combine it with the previous line
      tmp[i-1].append(" "+tmp[i]);
      tmp.removeAt(i);
      i--;
    }
  }
  QString desc = tmp.join("\n");
  desc.remove("\\\\"); //Remove any double backslashes
  return desc;
}

QString PBIDBAccess::cleanupArch(QString arch){
	//Sample: freebsd:10:x86:64  -> 64-bit
  arch = arch.simplified();
  QString out = arch.section(":",1,1)+"-";
  if(arch.endsWith("x86:64")){ out.append("64bit"); }
  else if(arch.endsWith("x86")){ out.append("32bit"); }
  else{ out.append(arch.section(":",2,5)); }
  return out;
}

QStringList PBIDBAccess::readIndexFile(){
  QFile file(PBI_DBDIR+"PBI-INDEX");
  if(!file.exists()){ return QStringList(); } //Return nothing for missing file
  //Now read the file
  QStringList output;
  if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
    QTextStream in(&file);
    while(!in.atEnd()){
      output << in.readLine(); 
    }
    file.close();
  }
  return output;
}

QStringList PBIDBAccess::readAppCafeFile(){
  QFile file(PBI_DBDIR+"AppCafe-index");
  if(!file.exists()){ qDebug() << "Missing AppCafe-index file..."; return QStringList(); } //Return nothing for missing file
  //Now read the file
  QStringList output;
  if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
    QTextStream in(&file);
    while(!in.atEnd()){
      output << in.readLine(); 
    }
    file.close();
  }
  return output;	
}
