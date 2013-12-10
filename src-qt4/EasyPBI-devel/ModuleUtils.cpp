#include "ModuleUtils.h"

QStringList ModuleUtils::validXdgCategories(){
  QStringList out;
  out << "AudioVideo"<<"Development"<<"Education"<<"Game"<<"Graphics"<<"Network"<<"Office"<<"Science"<<"Settings"<<"System"<<"Utility";
  return out;
}

void ModuleUtils::compressModule(QString modulePath){
  //Compress the module directory
  QString localDir = modulePath.section("/",-1);
  QString cmd = "cd "+modulePath+"/..; tar czf "+localDir+".tar.gz "+localDir;
  qDebug() << "Compressing module:" << modulePath+".tar.gz";
  system( cmd.toUtf8() );
  return;
}

PBIModule ModuleUtils::newModule(QString moduleDir, QString port, QString iconFile){
  PBIModule MOD;
  if(port.isEmpty()){ 
    qDebug() << "Error: No port given for the new module";
    return MOD;
  }
  //Check/create the base module directory
  QDir dir(moduleDir);
  bool ok = false;	
  if(!dir.exists()){
    ok = dir.mkpath(moduleDir);
    if(!ok){
      qDebug() << "Error: Could not create base module directory:" << moduleDir;
      return MOD;
    }
  }
  //Now check/create the specific module directory
  QString modName = port.section("/",-1);
  if(dir.exists(modName)){
    //change the module dir name (append a number)
    int add = 2;
    while( dir.exists(modName+"--"+QString::number(add)) ){
      add++;
    }
    modName = modName+"--"+QString::number(add);
  }
  ok = dir.mkpath(modName);
  if(!ok){
    qDebug() << "Error: Could not create the module directory:" << modName;
    return MOD;
  }
  //Now try to copy over the icon file into the resources dir
  if(iconFile.isEmpty() || !QFile::exists(iconFile)){ iconFile = QDir::homePath()+"/EasyPBI/defaulticon.png"; }
  ok = MOD.addResource(iconFile);
  if(!ok){
    qDebug() << "Warning: Could not copy icon into the new module:" << iconFile;
  }else{
    MOD.setText("PBI_PROGICON", iconFile.section("/",-1) ); //Use this icon for the program
  }
  //Now add the port info and create the pbi.conf file
  MOD.setText("PBI_MAKEPORT", port);
  MOD.saveConfig(); //create the new pbi.conf
  return MOD;
}
