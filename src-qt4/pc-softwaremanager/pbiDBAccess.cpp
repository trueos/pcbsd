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

// ================================
// =======  SETUP FUNCTIONS =======
// ================================
bool PBIDBAccess::setDBPath(QString fullPath){
  bool ok = FALSE;
  //Make sure the directory exists first
  if(QFile::exists(fullPath)){
    DBPath = fullPath;
    if(!DBPath.endsWith("/")){DBPath.append("/");}
    DBDir->setPath(fullPath);
    //Now read the list of available repos
    reloadRepoList();
    ok = TRUE;
  }
  return ok;
}

void PBIDBAccess::setRootCMDPrefix(QString prefix){
  //All commands need root permissions to run, so this is used 
  //    to add the proper cmd prefix to prompt to run with root permissions
  // Example: "pc-su" or "warden chroot <ip>"
  cmdPrefix = prefix;
  if(!cmdPrefix.endsWith(" ")){ cmdPrefix.append(" "); }
  //qDebug() << "DB command prefix:" << cmdPrefix;
}

void PBIDBAccess::reloadRepoList(){
  repoList.clear();
  if(DBDir->cd(DBPath+"repos")){ //directory exists
    repoList = DBDir->entryList(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::Readable);
  }	
}

bool PBIDBAccess::setRepo(QString repoNum){
  //Make sure the repo is available
  bool ok = DBDir->cd(DBPath+"repos");
  if(ok){ //directory exists
    QStringList rL = DBDir->entryList(QStringList()<<repoNum+"*",QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::Readable);
    if(rL.length() == 1){ //this repo exists
      currentRepoNumber=rL[0].section(".",0,0,QString::SectionSkipEmpty);
      currentRepoID=rL[0].section(".",1,1,QString::SectionSkipEmpty);
      return TRUE;
    }
  }
  return FALSE;
}

bool PBIDBAccess::currentRepoInvalid(){
  //Check to make sure that the current Repo is still valid
  reloadRepoList();
  if(repoList.contains(currentRepoNumber+"."+currentRepoID)){ return FALSE; }
  else{ return TRUE; }
}

QStringList PBIDBAccess::availableRepos(){
  QStringList output;
  for(int i=0; i<repoList.length(); i++){
    output << repoList[i].section(".",0,0,QString::SectionSkipEmpty);
  }
  return output;
}

QStringList PBIDBAccess::repoInfo(QString repoNum){
  //Returns: output=[Name, Master URL]	
  QStringList output;
  QString ID = getIDFromNum(repoNum);
    QStringList lines = Extras::readFile(DBPath+"repos/"+repoNum+"."+ID);
    if(!lines.isEmpty()){
      output <<"" << ""; //make sure there are two entries available
      for(int j=0; j<lines.length(); j++){
      	 if(lines[j].startsWith("URL: ")){ output[1] = lines[j].section("URL: ",1,50).simplified(); }
      	 else if(lines[j].startsWith("Desc: ")){ output[0] = lines[j].section("Desc: ",1,50).simplified(); }
      }
    }
  return output;
}

QStringList PBIDBAccess::repoMirrors(QString repoNum){
  QStringList output;
  QString ID = getIDFromNum(repoNum);
  if(!ID.isEmpty()){
    output = Extras::readFile(DBPath+"mirrors/"+ID);	  
  }
  return output;
}

// ========================================
// =======  PUBLIC ACCESS FUNCTIONS =======
// ========================================
QStringList PBIDBAccess::installed(){
  QStringList output;
  bool ok = DBDir->cd(DBPath+"installed");
  if(ok){
    output = DBDir->entryList( QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::Readable);
  }
  return output;
}

QStringList PBIDBAccess::installedPbiInfo(QString pbiID){
  //Output format: output[ name, version, arch, date created, author, website, installpath, iconpath]
  QStringList output;
  QString path = DBPath+"installed/"+pbiID;
  bool ok = DBDir->cd(path);
  if(ok){
    output << readOneLineFile(path+"/pbi_name");
    output << readOneLineFile(path+"/pbi_version");
    output << readOneLineFile(path+"/pbi_arch");
    output << readOneLineFile(path+"/pbi_mdate");
    output << readOneLineFile(path+"/pbi_author");
    output << readOneLineFile(path+"/pbi_web");
    output << readOneLineFile(path+"/pbi_installedpath");
    if(DBDir->exists("pbi_icon.png")){ output << path+"/pbi_icon.png"; }
    else{ output << ""; }
  }
  return output;
}

bool PBIDBAccess::installedPbiAutoUpdate(QString pbiID){
   bool ok = FALSE;
   if( QFile::exists(DBPath+"installed/"+pbiID+"/autoupdate-enable") ){ ok = TRUE; }
   //qDebug() << "AutoUpdate:" << pbiID << ok;
   return ok;
}

bool PBIDBAccess::installedPbiNeedsRoot(QString pbiID){
  bool ok=FALSE;
  if( QFile::exists(DBPath+"installed/"+pbiID+"/pbi_requiresroot") ){ ok=TRUE; }
  else{
    //Also check who installed the PBI if not flagged directly
    QFileInfo fInfo(DBPath+"installed/"+pbiID);
    if( fInfo.owner() == "root" ){ ok=TRUE; }
  }
  //qDebug() << pbiID << "requires root:" << ok;
  return ok;
}

bool PBIDBAccess::installedPbiHasXdgDesktop(QString installPath){
  if(!installPath.endsWith("/")){ installPath.append("/"); }
  bool ok = DBDir->cd(installPath+".xdg-desktop");
  if(ok){
    if( DBDir->entryList(QStringList()<<"*.desktop",QDir::Files).length() > 0 ){ return TRUE; }	  
  }
  return FALSE;
}

bool PBIDBAccess::installedPbiHasXdgMenu(QString installPath){
  if(!installPath.endsWith("/")){ installPath.append("/"); }
  bool ok = DBDir->cd(installPath+".xdg-menu");
  if(ok){
    if( DBDir->entryList(QStringList()<<"*.desktop",QDir::Files).length() > 0 ){ return TRUE; }	  
  }
  return FALSE;
}

QString PBIDBAccess::indexFilePath(){
  return DBPath+"index/"+currentRepoID+"-index";
}

QString PBIDBAccess::metaFilePath(){
  return DBPath+"index/"+currentRepoID+"-meta";	
}

QStringList PBIDBAccess::parseIndexLine(QString line){
  //output[name, arch, version, datetime, sizeK, isLatest(bool), filename]
  //line format 5/1/2013: [name,arch,version,checksum,datetime,mirrorPathToPBI,?,?,current/active,sizeInK,?]
      // NOTE: last two entries missing quite often
  QStringList lineInfo = line.split(":");
  QStringList output;
  if(lineInfo.length() < 9 ){ return output; } //skip incomplete entries
  output << lineInfo[0]; //name
  output << lineInfo[1]; //architecture
  output << lineInfo[2]; //version
  output << lineInfo[4]; //datetime
  if(lineInfo.length() >= 10){ output << lineInfo[9]; }//Size in KB
  else{ output << ""; }
  if(lineInfo[8].simplified() == "current"){ output << "true"; } //is most recent version
  else{ output << "false"; }  //is an older version
  output << lineInfo[5].section("/",-1); //filename (Example: myapp-0.1-amd64.pbi)
  return output;
}

QStringList PBIDBAccess::parseAppMetaLine(QString line){
  // line format 5/1/2013: [name,category,remoteIcon,author,website,license,apptype,tags,description,requiresroot]
  QStringList output = line.split(";");
  if(output.length() < 10){ output.clear();} //invalid line
  else if(output[9]=="YES"){ output[9]="true"; } //change to the same true/false syntax as elsewhere
  return output;
}

QStringList PBIDBAccess::parseCatMetaLine(QString line){
  // line format 5/1/2013: [name,remoteicon,description,?]
  QStringList output = line.split(";");
  if(output.length() < 3){output.clear(); } //incomplete line
  return output;
}
	
QString PBIDBAccess::remoteToLocalIcon(QString name, QString remoteIconPath){
  QString output = DBPath+"repo-icons/"+currentRepoID+"-"+name+"."+remoteIconPath.section(".",-1);
  //qDebug() << "Remote to Local Icon Path conversion:" << remoteIconPath << output;
  return output;
}

// ===== Database Modification Functions =====
bool PBIDBAccess::addRepoFile(QString rpofilepath){
  if(!QFile::exists(rpofilepath)){ return FALSE; }
  //Generate the command
  QString cmd;
  if(cmdPrefix.isEmpty()){ return FALSE; }
  else{ cmd = cmdPrefix; }
  cmd.append("\"pbi_addrepo "+rpofilepath+"\"");
  qDebug() <<"DB cmd generated:" << cmd;
  //Now run the command
  QStringList result = runCMD(cmd).split("\n");
  if(!result.isEmpty()){
    if(result[result.length()-1].startsWith("Added new repo:")){ 
      //Make sure to prompt the PBI Daemon to refresh the meta/index files now
      runCMD( cmdPrefix+"\"pbid --refresh\"" ); //don't care about output
      return TRUE;
    }
  } 
  qDebug() << "PBI Database Error:";
  qDebug() << " - CMD:"<<cmd;
  qDebug() << " - Error:"<<result;
  return FALSE;
}

bool PBIDBAccess::removeRepo(QString repoNum){
  //Generate the command
  QString cmd;
  if(cmdPrefix.isEmpty()){ return FALSE; }
  else{ cmd = cmdPrefix; }
  cmd.append("\"pbi_deleterepo "+repoNum+"\"");
  qDebug() <<"DB cmd generated:" << cmd;
  //Now run the command
  QStringList result = runCMD(cmd).split("\n");
  if(!result.isEmpty()){
    //qDebug() << "Repo Removed:" << result;
    if(result[result.length()-1].startsWith("Deleted Repository")){ return TRUE; }
  } 
  qDebug() << "PBI Database Error:";
  qDebug() << " - CMD:"<<cmd;
  qDebug() << " - Error:"<<result;
  return FALSE;	
}

bool PBIDBAccess::moveRepoUp(QString repoNum){
  //Generate the command
  QString cmd;
  if(cmdPrefix.isEmpty()){ return FALSE; }
  else{ cmd = cmdPrefix; }
  cmd.append("\"pbi_listrepo --up "+repoNum+"\"");
  qDebug() <<"DB cmd generated:" << cmd;
  //Now run the command
  QStringList result = runCMD(cmd).split("\n");
  if(!result.isEmpty()){
    return TRUE; //no special check for this - will need to re-load repos anyway
  } 
  return FALSE;
}

bool PBIDBAccess::moveRepoDown(QString repoNum){
  //Generate the command
  QString cmd;
  if(cmdPrefix.isEmpty()){ return FALSE; }
  else{ cmd = cmdPrefix; }
  cmd.append("\"pbi_listrepo --down "+repoNum+"\"");
  qDebug() <<"DB cmd generated:" << cmd;
  //Now run the command
  QStringList result = runCMD(cmd).split("\n");
  if(!result.isEmpty()){
    return TRUE; //no special check for this - will need to re-load repos anyway
  } 
  return FALSE;
}

bool PBIDBAccess::setRepoMirrors(QString repoNum, QStringList mirrors){
  QString cmd;
  if(cmdPrefix.isEmpty()){ return FALSE; }
  else{ cmd = cmdPrefix; }
  cmd.append("\"pbi_listrepo --mirror "+mirrors.join(",")+" "+repoNum+"\"");
  qDebug() <<"DB cmd generated:" << cmd;
  //Now run the command
  QStringList result = runCMD(cmd).split("\n");
  if(!result.isEmpty()){
    if(mirrors.isEmpty()){
      if(result[result.length()-1].startsWith("Mirror(s):")){ return TRUE; }
    }else{
      if(result[result.length()-1] == mirrors[mirrors.length()-1] ){ return TRUE; }
    }
  } 
  qDebug() << "PBI Database Error:";
  qDebug() << " - CMD:"<<cmd;
  qDebug() << " - Error:"<<result;
  return FALSE;		
}

// ========================================
// =======  PRIVATE ACCESS FUNCTIONS ======
// ========================================
QString PBIDBAccess::readOneLineFile(QString path){
  QFile file(path);
  if(!file.exists()){ return ""; } //Return nothing for missing file
  //Now read the file
  QString output;
  if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
    QTextStream in(&file);
    while(!in.atEnd()){
      output.append(in.readLine());
    }
    file.close();
  }
  return output;
}

QString PBIDBAccess::getIDFromNum(QString repoNum){
  QString output;
  for(int i=0; i<repoList.length(); i++){
    if(repoList[i].startsWith(repoNum+".")){
      output = repoList[i].section(".",1,1);
      break;	    
    }
  }	
  return output;
}

QString PBIDBAccess::runCMD(QString cmd){
  //Small function to run quick database modification commands
  QString output;
  proc->start(cmd);
  if(proc->waitForFinished(30000)){
    output = proc->readAll();	   
  }else{
    proc->terminate();
    output = "Process timed out (30 sec)";
  }
  if(output.endsWith("\n")){ output.chop(1); }
  output.simplified();
  return output;
}
