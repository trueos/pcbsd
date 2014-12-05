#include "ModuleUtils.h"

// =====================
//    VALID/RECOMMENDATIONS
// =====================
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

QStringList ModuleUtils::validExternalLinkTypes(){
  QStringList out;
  out << "binary" << "linux" << "keep" << "replace" << "nocrash";
  return out;
}

QString ModuleUtils::addExternalLinkType(QString current, QString newtype){
  QStringList types = current.remove(" ").split(",");
  bool ok = false;
  for(int i=0; i<types.length(); i++){
    //Check for special cases
    if( (types[i] == "keep") && (newtype=="replace") ){
      types[i] = newtype;
      ok = true;
      break;
    }else if( (types[i] == "replace") && (newtype=="keep") ){
      types[i] = newtype;
      ok = true;
      break;
    }else if( (types[i]=="binary") && (newtype=="linux") ){
      types[i] = newtype;
      ok = true;
      break;
    }else if( (types[i]=="linux") && (newtype=="binary") ){
      types[i] = newtype;
      ok = true;
      break;
    }else if( types[i] == newtype ){
      ok=true;
      break;
    }
  }
  if(!ok){ types << newtype; } //append the type to the list
  return types.join(",");
}

QStringList ModuleUtils::validRepoCategories(){
  QStringList out;
  out << "Accessibility" << "Archivers" << "Astronomy" << "Audio" << "Benchmarks" << "Biology" \
	<< "CAD" << "Communications" << "Converters" << "Databases" << "Desktop Utilities" \
	<< "Development" << "Editors" << "Emulators" << "File Transfer" << "Finance" \
	<< "Games" << "Graphics" << "IRC" << "Japanese" << "Java" << "Languages" \
	<< "Mail" << "Math" << "Miscellaneous" << "Multimedia" << "Network" << "Network - IM"  \
	<< "Network - Management" << "Network - P2P" << "News" << "Non-Port" << "Polish"  \
	<< "Print" << "Science" << "Security" << "Shells" << "System Utilities" << "Text Processing" \
	<< "Ports - Management" << "Web" << "X11" << "X11 - Clocks" << "X11 - File Managers"  \
	<< "X11 - Fonts" << "X11 - Toolkits" << "X11 - Window Managers";
  return out;
}

QString ModuleUtils::recommendedRepoCategory(QString portCategory){
  QStringList cat = ModuleUtils::validRepoCategories();
  QStringList pcat;  //WARNING: Make sure these correspond exactly to the "valid" categories above
  pcat << "accessibility" << "archivers" << "astro" << "audio" << "benchmarks" << "biology" \
	<< "cad" << "comms" << "converters" << "databases" << "deskutils" \
	<< "devel" << "editors" << "emulators" << "ftp" << "finance" \
	<< "games" << "graphics" << "irc" << "japanese" << "java" << "lang" \
	<< "mail" << "math" << "misc" << "multimedia" << "net" << "net-im" \
	<< "net-mgmt" << "net-p2p" << "news" << "nonport" << "polish" \
	<< "print" << "science" << "security" << "shells" << "sysutils" << "textproc" \
	<< "ports-mgmt" << "www" << "x11" << "x11-clocks" << "x11-fm" \
	<< "x11-fonts" << "x11-toolkits" << "x11-wm";
  int index = pcat.indexOf(portCategory.toLower());
  if( index < 0 || index >= cat.length() ) { index = pcat.indexOf("nonport"); } 
  return cat[index];
}

QStringList ModuleUtils::validRepoTypes(){
  QStringList out;
  out << "Graphical" << "Text" << "Server";
  return out;
}


// =======================
//     XDG SIMPLIFICATIONS
// =======================
bool ModuleUtils::xdgExecUsesRoot(QString exec){
  QString first = exec.section(" ",0,0,QString::SectionSkipEmpty);
  return ( (first == "pc-su") || first.contains("sudo") );
}

QString ModuleUtils::pruneXdgLine(QString exec){
  if(exec.contains("%%PBI_EXEDIR%%") || exec.contains("%%PBI_APPDIR%%") ){
    QString sline = exec.section("%%",2,50);
    if(sline.startsWith("/")){ sline = sline.remove(0,1); }
    return sline;
  }else if(exec.startsWith("pc-su ")){
    return exec.section(" ",1,50); //everything after the pc-su command
  }else{
    return exec;	  
  }
}

QString ModuleUtils::generateXdgExec(QString shortExec, bool useRoot){
  QString exec = shortExec;
  if(useRoot){ exec.prepend("pc-su "); }
  return exec;
}

QString ModuleUtils::generateXdgPath(QString shortline){
  QString line = "";
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

PBIModule ModuleUtils::newModule(QString moduleDir, QString port, QString iconFile, QStringList *plist, bool useplist){
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
  QString pbiname = modName; //save this for later
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
  if(!iconFile.isEmpty() && QFile::exists(iconFile)){
    ok = MOD.setAppIcon(iconFile);
    if(!ok){
      qDebug() << "Warning: Could not copy icon into the new module:" << iconFile;
    }	  
  }

  //Now add the port info and create the pbi.conf file
  MOD.setStringVal("PBI_ORIGIN", port);
  //Auto-generate fields if not a quick module
  if(plist!=0){ 
    MOD.setStringVal("PBI_PROGAUTHOR", "The "+pbiname+" Team"); 
    MOD.setListVal("PBI_PLUGINS", Backend::findPkgPlugins(port));
  }
  //Load the package plist if possible to set other values by default
  if(plist!=0){
    if(!useplist){
      //Don't use the input plist - so fetch it
      plist->clear();
      plist->append(Backend::getPkgPList(port));
    }
    if( !plist->isEmpty() ){
      //Now set as much other info from this as possible
      // - Application Type
      QString type = "Text";
      bool hasbin = (plist->filter("/bin/").length()>0 || plist->filter("/sbin/").length()>0);
      bool hasicons = (plist->filter(".png").length()>0 || plist->filter(".jpg").length()>0 || plist->filter(".svg").length()>0);
      if( hasbin && hasicons){ 
        type = "Graphical"; 
	//Also create desktop/menu entries for the binaries
	QStringList bins = plist->filter("/bin/");
	  bins.append( plist->filter("/sbin/") );
	  for(int i=0; i<bins.length() && i<5; i++){ //only do the top 2 binaries by default
	    MOD.setXdgText("Value", "1.0");
	    MOD.setXdgText("Type", "Application");
	    MOD.setXdgText("Name", bins[i].section("/",-1) );
	    MOD.setXdgText("GenericName", bins[i].section("/",-1) );
	    MOD.setXdgText("Exec", bins[i].section("/",-1) );
	    MOD.setXdgText("Icon", ""); //use the built-in icon
	    MOD.setXdgText("Path", ""); //use default path
	    MOD.setXdgEnabled("StartupNotify", true);
	    MOD.setXdgText("Categories", ""); //ensure the category is empty for the desktop entry
	    //Save the desktop entry
            MOD.saveXdgDesktop(bins[i].section("/",-1));
	    // Now add the menu entry values and save it
	    MOD.setXdgText("Categories", ModuleUtils::recommendedXdgCategory(port.section("/",0,0)) );
	    MOD.saveXdgMenu(bins[i].section("/",-1));    
	  }
      }
      else if(plist->filter("/etc/rc.d/").length() > 0){ type = "Server"; }
      MOD.setStringVal("PBI_PROGTYPE", type);
    }
  }
  MOD.saveConfig(); //create the new pbi.conf
  return MOD;
}

QString ModuleUtils::generatePbiBuildCmd(QString confDir, QString outDir, QString sigFile, bool packageBuild){
  QString cmd = "pbi_makeport"; 
  if(confDir.isEmpty()){ qDebug() << "Warning: a module must be supplied to build a PBII"; return ""; }
  if(!confDir.isEmpty()){ cmd.append(" -c "+confDir); }
  if(!outDir.isEmpty()){ cmd.append(" -o "+outDir); }
  if(!sigFile.isEmpty()){ cmd.append(" --sign "+sigFile); }
  if(packageBuild){ cmd.append(" --pkgbuild"); }
  return cmd;
}

QStringList ModuleUtils::generateWrapperScriptTemplate(){
  QStringList out;
	out <<"#!/bin/sh";
	out << "#This is a sample wrapper script form generated by EasyPBI";
	out << "# REMINDER: Add this file to the external-links for the PBI as well!";
	out << "";
	out << "#Setup some internal script variables";
	out << "PROGDIR=`/usr/local`   #Base PBI directory inside container";
	out << "APPBINARY=bin/SAMPLE   #application binary";
	out << "";
	out << "#--- DO SOMETHING HERE ---";
	out << "";
	out << "#Now start the main application";
	out << "${PROGDIR}/${APPBINARY} $@";
	
  return out;
}
