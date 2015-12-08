//===========================================
//  PC-BSD source code
//  Copyright (c) 2015, PC-BSD Software/iXsystems
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#include "pcbsd-general.h"
//PLEASE: Keep the functions in the same order as listed in pcbsd-general.h

#include "global.h"

using namespace pcUtil;
//=================
// RunCommand() variations
//=================
//Note: environment changes should be listed as such: <variable>=<value>

//Return CLI output (and success/failure)
QString General::RunCommand(bool &success, QString command, QStringList arguments, QString workdir, QStringList env){
  QProcess proc;
    proc.setProcessChannelMode(QProcess::MergedChannels); //need output
  //First setup the process environment as necessary
  QProcessEnvironment PE = QProcessEnvironment::systemEnvironment();
    if(!env.isEmpty()){
      for(int i=0; i<env.length(); i++){
	if(!env[i].contains("=")){ continue; }
        PE.insert(env[i].section("=",0,0), env[i].section("=",1,100));
      }
    }
    proc.setProcessEnvironment(PE);
  //if a working directory is specified, check it and use it
  if(!workdir.isEmpty()){
    proc.setWorkingDirectory(workdir);
  }
  //Now run the command (with any optional arguments)
  if(arguments.isEmpty()){ proc.start(command); }
  else{ proc.start(command, arguments); }
  //Wait for the process to finish (but don't block the event loop)
  while( !proc.waitForFinished(500) ){ QCoreApplication::processEvents(); }
  success = (proc.exitCode()==0); //return success/failure
  return QString(proc.readAllStandardOutput());
}

QString General::RunCommand(QString command, QStringList arguments, QString workdir, QStringList env){
  //Overloaded simplification for text-only output
  bool junk = false;
  return General::RunCommand(junk,command,arguments,workdir,env);
}

bool General::RunQuickCommand(QString command, QStringList arguments,QString workdir, QStringList env){
  //Overloaded simplification for success-only output
  bool success = false;
  General::RunCommand(success, command, arguments, workdir, env);
  return success;
}

//=================
// TEXT FILE INTERACTION
//=================
QStringList General::readTextFile(QString filepath){
  QStringList out;
  QFile file(filepath);
  if(file.open(QIODevice::Text | QIODevice::ReadOnly)){
    QTextStream in(&file);
    while(!in.atEnd()){
      out << in.readLine();
    }
    file.close();
  }
  return out;
}

bool General::writeTextFile(QString filepath, QStringList contents, bool overwrite){
  //Simplification for user directory
  if(filepath.startsWith("~")){ filepath = filepath.replace(0,1,QDir::homePath()); }
  QFile file(filepath);
  //Check existance/validity
  if(file.exists() && !overwrite){ return false; }
  if(!file.exists()){
    //See if we need to create the parent directory as well
    QDir dir = QFileInfo(filepath).dir(); //get the parent directory
    if(!dir.exists()){
      if(!dir.mkpath(dir.absolutePath())){ return false; } //could not create parent directory
    }
  }
  //Make sure that something is there to be written
  // - to delete a file, use QFile::remove() instead of this function
  if(contents.isEmpty()){ contents << "\n"; }
  else if(contents.last().isEmpty()){ contents << "\n"; }
  //Now save the contents to the file
  bool ok = false;
  if( file.open(QIODevice::WriteOnly | QIODevice::Truncate) ){
    QTextStream out(&file);
    out << contents.join("\n");
    file.close();
    ok = true;
  }
  return ok;
}

//===========================
//     SYSCTL ACCESS (might require root)
//===========================
//Retrieve a text-based sysctl
QString General::sysctl(QString var){
   char result[1000];
   size_t len = sizeof(result);
   sysctlbyname(var.toLocal8Bit(), result, &len, NULL, 0);
   result[len] = '\0';
   return QString(result);
}

//Retrieve a number-based sysctl
long long General::sysctlAsInt(QString var){
   long long result = 0;
   size_t len = sizeof(result);
   sysctlbyname(var.toLocal8Bit(), &result, &len, NULL, 0);
   return result;
}
