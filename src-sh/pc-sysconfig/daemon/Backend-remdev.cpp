#include "Backend.h"

//REMOVABLE DEVICES (remdev)
QStringList Backend::findActiveDevices(){
  QStringList activeDevs;
  //Now find any active partitions and ignore it and any children of it
  QStringList info = pcbsd::Utils::runShellCommand("gpart show -p");
  info = info.filter("freebsd").filter("[active]");
  for(int i=0; i<info.length(); i++){
    info[i].remove("=>");
    info[i] = info[i].replace("\t"," ").simplified();
    QString dev = info[i].section(" ",2,2,QString::SectionSkipEmpty);
    activeDevs << dev;
    activeDevs << DevCheck::devChildren(dev);
  }
  
  activeDevs.removeDuplicates();
  //qDebug() << "Active Devices:" << activeDevs;
  return activeDevs;
}

QStringList Backend::listAllRemDev(){
  QStringList out;
  
  return out;
}

QString Backend::getRemDevInfo(QString node){
  QString out;
	
  return out;
}

QStringList Backend::disktypeInfo(QString node){
  QStringList out;
	
  return out;
}

QStringList Backend::listMountedNodes(){
  QStringList out;
	
  return out;
}