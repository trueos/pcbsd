#include "Backend.h"
#include <QDateTime>

QStringList Backend::listNetworkDrives(){
  //output format: [<Name> (<IP>)]
  QStringList out;
	
  return out;
}

QStringList Backend::listMountedNetDrives(){
  static QStringList results;
  static QDateTime lastcheck;
  if(!lastcheck.isNull() && (lastcheck > QFileInfo("/etc/hosts").lastModified()) ){
    return results; //nothing changed recently - just return the same as last time
  }
  //output format: [<Name> (<IP>) on <dir>]
  lastcheck = QDateTime::currentDateTime(); //save this for later
  QStringList contents = readFile("/etc/hosts");
  results.clear();
  for(int i=0; i<contents.length(); i++){
    if(contents[i].startsWith("#") || contents[i].contains("localhost")){ continue; }
    contents[i] = contents[i].replace("\t", " ").simplified();
    results << contents[i].section(" ",1,1)+" ("+contents[i].section(" ",0,0)+") on /net/"+contents[i].section(" ",1,1);
  }
  return results;
}

QString Backend::autoMountNetDrive(QString driveIP, QString name){
  QStringList contents = readFile("/etc/hosts");
  if(!contents.filter(driveIP).isEmpty()){
    //Need to replace an existing entry
    QStringList lines = contents.filter(driveIP);
    bool changed = false;
    for(int i=0; i<lines.length(); i++){
      int index = contents.indexOf(lines[i]);
      if(index<0){ continue; }
      if(contents[index].startsWith("#")){ continue; } //skip lines which are commented out
      //Valid Line - need to make an adjustment
      if(!changed){ contents[i] = driveIP+"\t"+name; changed = true; } //replace the line
      else{ contents.removeAt(i); } //duplicate entry for the same IP - remove it
    }
    //no valid lines changed, just add a new entry to the end
    if(!changed){ contents << driveIP+"\t"+name; }
  }else{
    //No entry for this IP yet - add one
    contents << driveIP+"\t"+name;
  }
  bool ok = writeFile("/etc/hosts",contents,true);
  return (ok ? "[SUCCESS]": "[FAILURE]");
}

QString Backend::autoUnmountNetDrive(QString driveIP){
  QStringList contents = readFile("/etc/hosts");
  if(!contents.filter(driveIP).isEmpty()){
    //Need to replace an existing entry
    QStringList lines = contents.filter(driveIP);
    bool changed = false;
    for(int i=0; i<lines.length(); i++){
      int index = contents.indexOf(lines[i]);
      if(index<0){ continue; }
      if(contents[index].startsWith("#")){ continue; } //skip lines which are commented out
      //Valid Line - need to remove it
      contents.removeAt(i);
      changed = true;
    }
    if(!changed){ return "[SUCCESS]"; } //no need to re-save the file - nothing changed
  }else{
    //No entry for this IP yet - no changes required
    return "[SUCCESS]";
  }
  bool ok = writeFile("/etc/hosts",contents,true);
  return (ok ? "[SUCCESS]": "[FAILURE]");	
}