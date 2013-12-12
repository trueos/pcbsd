#include "ModuleUtils.h"

QStringList ModuleUtils::validXdgCategories(){
  QStringList out;
  out << "AudioVideo"<<"Development"<<"Education"<<"Game"<<"Graphics"<<"Network"<<"Office"<<"Science"<<"Settings"<<"System"<<"Utility";
  return out;
}

QString ModuleUtils::recommendedXdgCategory(QString portCategory){
  QString pc = portCategory.toLower();
  QStringList avcat, devcat, edcat, gamecat, gracat,netcat,offcat,setcat,syscat,scicat;
  avcat << "audio"<<"multimedia";
  devcat << "devel" << "lang" << "java";
  edcat << "astro" << "cad" <<"math";
  scicat <<"biology"<<"science";
  gamecat << "games";
  gracat << "graphics";
  netcat << "ftp"<<"irc"<<"mail"<<"net"<<"net-im"<<"net-p2p"<<"net-mgmt"<<"www";
  offcat << "editors" << "finance" << "news" << "textproc";
  setcat << "accessibility"<<"print";
  syscat << "sysutils" << "benchmarks" << "emulators" << "ports-mgmt";
  
  //Assign the menu category
  QString mc;
  if( avcat.contains(pc) ){ mc="AudioVideo"; }
  else if( devcat.contains(pc) ){ mc="Development"; }
  else if( edcat.contains(pc) ){ mc="Education"; }
  else if( scicat.contains(pc) ){ mc ="Science"; }
  else if( gamecat.contains(pc) ){ mc="Game"; }
  else if( gracat.contains(pc) ){ mc="Graphics"; }
  else if( netcat.contains(pc) ){ mc="Network"; }
  else if( offcat.contains(pc) ){ mc="Office"; }
  else if( setcat.contains(pc) ){ mc="Settings"; }
  else if( syscat.contains(pc) ){ mc="System"; }
  else{ mc = "Utility"; }
  
  return mc;	
}

bool ModuleUtils::xdgExecUsesRoot(QString exec){
  QString first = exec.section(" ",0,0,QString::SectionSkipEmpty);
  return ( (first == "pc-su") || first.contains("sudo") );
}

QString ModuleUtils::pruneXdgLine(QString exec){
  if(exec.contains("%%PBI_EXEDIR%%") || exec.contains("%%PBI_APPDIR%%") ){
    QString sline = exec.section("%%",2,50);
    if(sline.startsWith("/")){ sline = sline.remove(0,1); }
    return sline;
  }else{
    return exec;	  
  }
}

QString ModuleUtils::generateXdgExec(QString shortExec, bool useRoot){
  QString exec = "%%PBI_EXEDIR%%/"+shortExec;
  if(useRoot){ exec.prepend("pc-su "); }
  return exec;
}

QString ModuleUtils::generateXdgPath(QString shortline){
  QString line = "%%PBI_APPDIR%%";
  if(!shortline.isEmpty()){
    line.append("/"+shortline);	  
  }
  return line;
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
  dir.cd(modName);
  //Load the (non-existant) pbi.conf to prep the structure
  MOD.loadModule(dir.canonicalPath()+"/pbi.conf");
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
