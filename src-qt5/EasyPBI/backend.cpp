#include "backend.h"

QIcon Backend::icon(QString icon){
  icon = icon.toLower();
  //Qt embedded resources (http://www.qtcentre.org/wiki/index.php?title=Embedded_resources)
  QString iconPath = ":/png/icons/";
  if(icon == "new"){ iconPath.append("folder-new.png"); }
  else if(icon=="load"){ iconPath.append("document-open.png"); }
  else if(icon=="refresh"){ iconPath.append("view-refresh.png"); }
  else if(icon=="delete"){ iconPath.append("edit-delete.png"); }
  else if(icon=="left"){ iconPath.append("arrow-left.png"); }
  else if(icon=="right"){ iconPath.append("arrow-right.png"); }
  else if(icon=="up"){ iconPath.append("arrow-up.png"); }
  else if(icon=="down"){ iconPath.append("arrow-down.png"); }
  else if(icon=="save"){ iconPath.append("document-save.png"); }
  else if(icon=="help"){ iconPath.append("help-hint.png"); }
  else if(icon=="open"){ iconPath.append("document-preview.png"); }
  else if(icon=="file"){ iconPath.append("file.png"); }
  else if(icon=="trash"){ iconPath.append("trash-empty.png"); }
  else if(icon=="clear"){ iconPath.append("edit-clear.png"); }
  //EasyPBI embedded resources (EasyPBI.qrc)
  else if(icon=="close"){ iconPath = ":/png/icons/window-close.png"; }
  else if(icon=="easypbi"){ iconPath = ":/png/icons/EasyPBIicon.png"; }
  else if(icon=="freebsd"){ iconPath = ":/png/icons/freebsd.png"; }
  else if(icon=="pcbsd"){iconPath = ":/png/icons/pcbsd.png"; }
  else if(icon=="config"){iconPath = ":/png/icons/config.png"; }
  //onebit iconset icons (http://www.iconfinder.com/search/?q=iconset%3Aonebit)
  else if(icon=="plus"){iconPath = ":/png/icons/plus.png"; }
  else if(icon=="minus"){iconPath = ":/png/icons/minus.png"; }
  else{ iconPath.clear(); }
	
  return QIcon(iconPath);
}

QStringList Backend::getCmdOutput(QString cmd, QString dir){
  QProcess *proc = new QProcess;
  proc->setProcessChannelMode(QProcess::MergedChannels);
  if( !dir.isEmpty() && QFile::exists(dir) ){ proc->setWorkingDirectory(dir); }
  proc->start(cmd);
  while(!proc->waitForFinished(300)){
    QCoreApplication::processEvents();
  }
  QStringList out = QString(proc->readAllStandardOutput()).split("\n");	
  delete proc;	
  return out;
}

bool Backend::writeFile(QString filepath, QStringList contents){
  QString dpath = filepath;
  dpath.chop( dpath.section("/",-1).length()+1 );
  QDir dir;
  if( !dir.mkpath(dpath) ){ return false; } //could not create containing folder
  //Now create the the file
  if(QFile::exists(filepath)){ QFile::remove(filepath); } //remove the old file
  QFile file(filepath);
  if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){ return false; }
  QTextStream out(&file);
  for(int i=0; i<contents.length(); i++){
    out << contents[i];
  }
  out << "\n"; //always make sure to put a newline at the end
  file.close();
  return true;
}

// ====================
//  PACKAGE DATABASE TOOLS
// ====================
QStringList Backend::getPkgList(){
  //Generate an alphabetized list of all available packages/ports on the repo
  //format: <category>/<pkgname> (port format)
  QString cmd = "syscache \"pkg #system remotelist\""; //"pkg rquery -aU %o";
  QStringList result = getCmdOutput(cmd).join("").split(", ");
  result.sort();
  result.removeAll(""); //get rid of empty items
  return result;
}

QStringList Backend::getPkgInfo(QString port){
  //Function to query the package repository and pull down information about a particular package
  //Output: <name>, <website>, <comment>, <description>, <license>
  QString tmp = " \"pkg #system remote "+port+" %1\"";
  QString cmd = "syscache "+tmp.arg("name")+tmp.arg("website")+tmp.arg("comment")+tmp.arg("description")+tmp.arg("license");
  //QString cmd = "pkg rquery \"%n::::%w::::%c::::%e\" -e %o "+port; //general info
  //QString cmd2 = "pkg rquery %L -e %o "+port; //License info
  QStringList info;
  //Get the general info
  QStringList out = Backend::getCmdOutput(cmd);//.join("\n").split("::::");
  for(int i=0; i<5; i++){
    if(i < out.length()){ info << out[i].replace("<br>","\n"); }
    else{ info << ""; }
  }
  //Now get the licence
  //QString lic = Backend::getCmdOutput(cmd2).join(" ");
  //info << lic;
  return info;
}

QStringList Backend::getPkgOpts(QString port){
  //Function to query the package repository and pull down information about a particular package
  //Output: <option>=<on/off>
  QString cmd = "syscache \"pkg #system remote "+port+" options\"";//"pkg rquery -U \"%Ok=%Ov\" -e %o "+port;
  QStringList out = Backend::getCmdOutput(cmd).join("").split(", ");
  out.removeAll(""); //get rid of empty items
  return out;
}

QStringList Backend::getPkgPList(QString port){
  QStringList out;
  //Check if the pkg is already installed
  QString cmd = "syscache \"pkg #system local "+port+" files\"";
  out = Backend::getCmdOutput(cmd).join("").split(", "); //"pkg query %Fp -e %o "+port);
  out.removeAll("");
  //qDebug() << "Local Pkg plist:" << out;
  //No local copy - need to download the pkg as user
  if(out.isEmpty()){
    // - Create custom pkg.conf
    QString cdir = QDir::homePath()+"/EasyPBI/.cache";
    //QString cmd = "echo \"PKG_CACHEDIR: "+QDir::homePath()+"/EasyPBI/.cache\" > ~/EasyPBI/.cache/.pkgconf";
    bool ok = Backend::writeFile(cdir+"/.pkgconf", QStringList() << "PKG_CACHEDIR: "+cdir );
    if(!ok){ return out; }
    //qDebug() << "Create Conf:" << Backend::getCmdOutput(cmd); //create the config file/directory
    // - Fetch pkg
    QString cmd = "pkg -C "+cdir+"/.pkgconf fetch -U -y "+port;
    QStringList tmp = Backend::getCmdOutput(cmd); //fetch the raw pkg file
    // - get package file name
    tmp = tmp.filter("(100%");
    QString fpath;
    if(tmp.length()>=1){
      fpath = cdir+"/All/"+tmp[0].section("(100%",0,0).simplified()+".txz"; 
    }
    //qDebug() << "fpath:" << fpath;
    // - Read pkg
    if( !fpath.isEmpty() ){
      //fpath = dir.absoluteFilePath(fpath); //make sure it is the full path
      cmd = "pkg info -l -F "+fpath;
      out = Backend::getCmdOutput(cmd);
      //Remove the temporary pkg file after reading it
      QFile::remove(fpath);
    }
    //qDebug() << "Remote plist:" << out;
  }
  return out;
}

QStringList Backend::findPkgPlugins(QString pkgorigin){
  //Turn the pkg origin into a searchable string
  QString srch = pkgorigin.section("/",-1);
  if(srch.endsWith("-devel")){ srch.chop(6); }
  //Now get all the packages that start with the same base name
  QStringList out = Backend::getPkgList();
  out = out.filter("/"+srch+"-");
  // Now make sure we remove any "-devel" pkgs (those are not plugins)
  QStringList dev = out.filter("-devel");
  for(int i=0; i<dev.length(); i++){ out.removeAll(dev[i]); }
  
  return out;
}

//================
//       PORT TOOLS
// ================
QStringList Backend::getPortOpts(QString portPath){
  //output format: <option>:::<on/off>:::<description>
  QString cmd = "make showconfig";
  QStringList list = Backend::getCmdOutput(cmd, portPath);
  //Now parse the output
  QStringList out;
    for(int i=0; i<list.length(); i++){
      if(list[i].startsWith("===>")){ continue; } //comment
      else if(list[i].simplified().isEmpty()){ continue; }
      QString opt = list[i].section("=",0,0).simplified();
      QString val = list[i].section(":",0,0).section("=",1,1).simplified();
      QString desc = list[i].section(":",1,50).simplified();
      out << opt+":::"+val+":::"+desc;
    }
  return out;
}
