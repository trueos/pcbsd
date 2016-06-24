#include "backend.h"

QStringList Backend::shortCMD(QString dir, QString cmd, QStringList args){
  //Run a quick command in the proper dir and return the output
  QProcess proc;
  proc.setProcessChannelMode(QProcess::MergedChannels);
  if( !dir.isEmpty() && QFile::exists(dir) ){ proc.setWorkingDirectory(dir); }
  if(args.isEmpty()){ proc.start(cmd); }
  else{ proc.start(cmd, args); }
  while(!proc.waitForFinished(300)){ QCoreApplication::processEvents(); }
  QStringList out;
  while(proc.canReadLine()){
    out << QString( proc.readLine() );
  }
  return out;
}

bool Backend::quickCMD(QString dir, QString cmd, QStringList args){
  //Run a quick command without looking for output other than success/failure
  QProcess proc;
  if( !dir.isEmpty() && QFile::exists(dir) ){ proc.setWorkingDirectory(dir); }
  if(args.isEmpty()){ proc.start(cmd); }
  else{ proc.start(cmd, args); }
  while(!proc.waitForFinished(300)){ QCoreApplication::processEvents(); }
  return (proc.exitCode() == 0);
}


void Backend::emptyDir(QString dir){
  QDir d(dir);
  if(!d.exists()){ return; } //quick check to make sure directory exists first
  //Remove all the files in this directory
  QStringList tmp = d.entryList(QDir::Files | QDir::NoDotAndDotDot);
  for(int i=0; i<tmp.length(); i++){
    d.remove(tmp[i]);
  }
  //Now remove all the directories in this directory (recursive)
  tmp = d.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
  for(int i=0; i<tmp.length(); i++){
    Backend::emptyDir(d.absoluteFilePath(tmp[i])); //Empty this directory first
    d.rmdir(tmp[i]); //Now try to remove it
  }
}

void Backend::generatePortsUpdateCMDS(QStringList &cmds, QStringList &dirs, QStringList &info){
  //Clear the output variables
  cmds.clear(); dirs.clear(); info.clear();
  //Check if the ports directory even exists
  if(!QFile::exists("/usr/ports")){
    cmds << "mkdir /usr/ports"; info << "Creating the ports tree"; dirs << ""; //Create the ports tree
  }
  //Now check if the git directory exists
  QString URL = "https://www.github.com/pcbsd/freebsd-ports.git";
  if(QFile::exists("/usr/ports/.git")){
    //Check if the remote URL is correct
    QString origin = Backend::shortCMD("/usr/ports", "git remote show -n origin").filter("Fetch URL:").join("").section("URL:",1,30).simplified();
    if(origin != URL){
      cmds << "git remote remove origin"; info << "Fix origin URL"; dirs <<"/usr/ports";
      cmds << "git remote add origin "+URL; info << ""; dirs << "/usr/ports/.git"; //setup TrueOS git repo
    }
  }else{
    //new GIT setup
    Backend::emptyDir("/usr/ports");
    //cmds << "rm -rf /usr/ports/*"; info << "Removing the old ports tree"; dirs << ""; //Clean the old ports tree
    cmds << "git init"; info << "Initialize GIT"; dirs << "/usr/ports"; //setup git
    cmds << "git remote add origin "+URL; info << ""; dirs << "/usr/ports/.git"; //setup TrueOS git repo
  }
  //Now update the tree with git
  cmds << "git fetch --depth=1"; info << "Fetch new GIT info (this may take a while)"; dirs << "/usr/ports/.git";
  cmds << "git checkout master"; info << "Checkout the tree"; dirs << "/usr/ports"; 
}

void Backend::generateSourceUpdateCMDS(QString branch, QStringList &cmds, QStringList &dirs, QStringList &info){
  //Clear the output variables
  cmds.clear(); dirs.clear(); info.clear();
  //Check if the source directory even exists
  if(!QFile::exists("/usr/src")){
    cmds << "mkdir /usr/src"; info << "Creating the source tree"; dirs << ""; //Create the ports tree
  }
  //Now check if the git directory exists
  QString URL = "https://www.github.com/pcbsd/freebsd.git";
  if(QFile::exists("/usr/src/.git")){
    //Check if the remote URL is correct
    QString origin = Backend::shortCMD("/usr/src", "git remote show -n origin").filter("Fetch URL:").join("").section("URL:",1,30).simplified();
    if(origin != URL){
      cmds << "git remote remove origin"; info << "Fix origin URL"; dirs <<"/usr/src";
      cmds << "git remote add origin "+URL; info << ""; dirs << "/usr/src/.git"; //setup TrueOS git repo
    }
  }else{
    //new GIT setup
    Backend::emptyDir("/usr/src");
    //cmds << "rm -rf /usr/src/*"; info << "Removing the old source tree"; dirs << ""; //Clean the old ports tree
    cmds << "git init"; info << "Initialize GIT"; dirs << "/usr/src"; //setup git
    cmds << "git remote add origin "+URL; info << ""; dirs << "/usr/src/.git"; //setup TrueOS git repo
  }
  //Now update the tree with git
  cmds << "git fetch --depth=1"; info << "Fetch new GIT info (this may take a while)"; dirs << "/usr/src/.git";
  cmds << "git checkout "+branch; info << "Checkout the tree"; dirs << "/usr/src"; 	
}
