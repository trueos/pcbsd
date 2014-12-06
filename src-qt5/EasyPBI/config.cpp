#include "config.h"

Config::Config() {
    //Clear internal variables
    badPackages.clear(); exProgStruct.clear(); defaultSettings.clear();
    valueStruct.clear(); detStruct.clear(); TFstruct.clear();
    //Setup internal structures
	//exProgStruct=[ pbi_makeport, pbi_create, su_utility ]
    exProgStruct << "" << "" << "";
	//valueStruct=[ pbiOutputDir, moduleOutputDir, pkgCacheDir, portsDir, iconDir, defaultIcon, sigFile ]
    valueStruct << "" << "" << "" << "" << "" << "" << "";
	//detStruct=[ pbi_makeport, pbi_create, su_utility, portsDir ]
    detStruct << "" << "" << "" << "";
	//defaultSettings=[ current version#, programDir, pbiOutputDir, moduleOutputDir, pkgCacheDir, iconFile, settingsFile, iconDir]
	QString tmp = QDir::homePath()+"/EasyPBI/";
    defaultSettings << "2.4.1" << tmp << tmp+"PBI/" << tmp+"Modules/" << tmp+".cache/" << tmp+"defaulticon.png" << tmp+".preferences" << QDir::homePath();
	//TFstruct=[ is64-bitArch, portsAvailable, useDigitalSig, useTMPFS, usePkgCache, pbi_makeport_available, pbi_create_available, su_available ]
    TFstruct << false << false << false << true << true << false << false << false;
  }

Config::~Config(){
}

void Config::readMachineArch(){
  //set the "is64bit" flag
  QString arch;
  QProcess p;
  p.start("uname -m");
  if (p.waitForFinished()) {
      while (p.canReadLine()) {
          arch = p.readLine().simplified();
      }
  }
  qDebug() << "architecture:" << arch;
  TFstruct[0] = (  arch == "amd64" );	
}

bool Config::checkDirectoryStructure(){
 //Check for EasyPBI directory structure and create it if it is not there
  if( !QDir(defaultSettings[1]).exists() ){
    qDebug() << "Creating the EasyPBI directory in current user's home folder:" << defaultSettings[1];
    QDir dir;
    dir.root();
    if( !dir.mkdir(defaultSettings[1]) ){ //Create the program directory
      //Could not create the Program directory
      qDebug() << " - Error with home directory permissions, could not create directory";
      return false;
    }
  }  
  if( !QDir(defaultSettings[3]).exists() ){
    QDir dir;
    dir.root();
    if( !dir.mkdir(defaultSettings[3]) ){ //module directory
      qDebug() << " - Error with directory permissions, could not create directory" << defaultSettings[3];
      return false;
    }
  }  
  if( !QDir(defaultSettings[2]).exists() ){
    QDir dir;
    dir.root();
    if( !dir.mkdir(defaultSettings[2]) ){ // PBI output directory
      qDebug() << " - Error with directory permissions, could not create directory" << defaultSettings[2];
      return false;
    }
  }  
  if( !QDir(defaultSettings[4]).exists() ){
    QDir dir;
    dir.root();
    if( !dir.mkdir(defaultSettings[4]) ){ // pkg cache
      qDebug() << " - Error with directory permissions, could not create directory" << defaultSettings[4];
      return false;
    }
  }  

  //Check for the defaulticons.png file
  if( !QFile(defaultSettings[5]).exists() ){
    //file does not exist, copy the included resource to the file
    qDebug() << " - Copying the default icon to the EasyPBI directory";
    QFile::copy(":/png/icons/defaulticon.png",defaultSettings[5]);
    
    QFile::setPermissions(defaultSettings[5],QFile::ReadGroup | QFile::WriteGroup | QFile::ReadOwner | QFile::WriteOwner | QFile::ReadOther | QFile::WriteOther);
  }
  return true;
}

void Config::loadSettingsFile(){
  //Return to the defaults
  returnToDefaults();
  //Make sure the preferences file exists, otherwise just load the defaults
  if( !QFile::exists(defaultSettings[6]) ){ return; }
  //Read the preferences file
  QFile file(defaultSettings[6]);
  //Open the file, otherwise use defaults
  if( !file.open(QIODevice::ReadOnly | QIODevice::Text) ){ 
    qDebug() << "Could not open preferences file for reading:" << defaultSettings[6];
    returnToDefaults();
    return; 
  }
  QTextStream in(&file);
  QString savedversion;
  while( !in.atEnd() ){
    QString line = in.readLine().simplified();
    //Now parse out the values
    if( line.startsWith("<version>") ){ savedversion = readSavedValue("<version>",line); }
    //else if( line.startsWith("<pbioutdir>") ){ valueStruct[0] = readSavedValue("<pbioutdir>",line); }
    //else if( line.startsWith("<moduleoutdir>") ){ valueStruct[1] = readSavedValue("<moduleoutdir>",line); }
    //else if( line.startsWith("<cachedir>") ){ valueStruct[2] = readSavedValue("<cachedir>",line); }
    else if( line.startsWith("<portsdir>") ){ valueStruct[3] = readSavedValue("<portsdir>",line); }
    else if( line.startsWith("<icondir>") ){ valueStruct[4] = readSavedValue("<icondir>",line); }
    else if( line.startsWith("<defaulticon>") ){ valueStruct[5] = readSavedValue("<defaulticon>",line); }
    //else if( line.startsWith("<sigfile>") ){ valueStruct[6] = readSavedValue("<sigfile>",line); }
    //else if( line.startsWith("<badpackages>") ){ badPackages = readSavedValue("<badpackages>",line).split(";",QString::SkipEmptyParts); }
    //else if( line.startsWith("<usesignature>") ){ TFstruct[2] = (readSavedValue("<usesignature>",line).toLower() == "true"); }
    //else if( line.startsWith("<usetmpfs>") ){ TFstruct[3] = (readSavedValue("<usetmpfs>",line).toLower() == "true"); }
    //else if( line.startsWith("<usepkgcache>") ){ TFstruct[4] = (readSavedValue("<usepkgcache>",line).toLower() == "true"); }
    //else if( line.startsWith("<pbi_makeport>") ){ exProgStruct[0] = readSavedValue("<pbi_makeport>",line); }
    //else if( line.startsWith("<pbi_create>") ){ exProgStruct[1] = readSavedValue("<pbi_create>",line); }
    else if( line.startsWith("<su_utility>") ){ exProgStruct[2] = readSavedValue("<su_utility>",line); }
  }
  file.close();
  //Now update any missing values in the structures
  checkStructures();
}

void Config::saveSettingsFile(){
	//exProgStruct=[ pbi_makeport, pbi_create, su_utility ]
	//detStruct=[ pbi_makeport, pbi_create, su_utility, portsDir ]
  //Get the preferences file
  QFile file(defaultSettings[6]);
  //Open the file, otherwise use defaults
  if( !file.open(QIODevice::WriteOnly | QIODevice::Text) ){ 
    qDebug() << "Could not open preferences file for writing:" << defaultSettings[6];
    return; 
  }
  QTextStream out(&file);
  out << "---AUTOMATICALLY GENERATED---\n---DO NOT MODIFY THIS FILE BY HAND---\n";
  out << "<version>"+ defaultSettings[0] +"</version>\n";
  //if(valueStruct[0]!=defaultSettings[2]){ out << "<pbioutdir>"+ valueStruct[0] + "</pbioutdir>\n"; }
  //if(valueStruct[1]!=defaultSettings[3]){out << "<moduleoutdir>"+valueStruct[1]+"</moduleoutdir>\n"; }
  //if(valueStruct[2]!=defaultSettings[4]){out << "<cachedir>"+valueStruct[2]+"</cachedir>\n"; }
  if(valueStruct[3]!=detStruct[3]){out << "<portsdir>"+valueStruct[3]+"</portsdir>\n"; }
  if(valueStruct[4]!=defaultSettings[7]){out << "<icondir>"+valueStruct[4]+"</icondir>\n"; }
  if(valueStruct[5]!=defaultSettings[5]){out << "<defaulticon>"+valueStruct[5]+"</defaulticon>\n"; }
  //if(!valueStruct[6].isEmpty()){ out << "<sigfile>"+valueStruct[6]+"</sigfile>\n"; }
  //if(exProgStruct[0] != detStruct[0]){ out << "<pbi_makeport>"+exProgStruct[0]+"</pbi_makeport>\n"; }
  //if(exProgStruct[1] != detStruct[1]){ out << "<pbi_create>"+exProgStruct[1]+"</pbi_create>\n"; }
  if(exProgStruct[2] != detStruct[2]){ out << "<su_utility>"+exProgStruct[2]+"</su_utility>\n"; }
  //if(TFstruct[2]){ out << "<usesignature>true</usesignature>\n"; }
  //if(TFstruct[3]){ out << "<usetmpfs>true</usetmpfs>\n"; }
  //if(TFstruct[4]){ out << "<usepkgcache>true</usepkgcache>\n"; }
  //if(badPackages.length() >0){ out << "<badpackages>"+badPackages.join(";")+"</badpackages>\n"; }
  //Now close the file
  file.close();
}

void Config::scanForExternalUtilities(){
  bool sufound = false;
  //bool pbifound1 = false;
  //bool pbifound2 = false;
  //Setup the commands to look for (lists in order of preference)
  QStringList suCMD;
  suCMD << "pc-su" << "qsu" << "gksu" << "kdesu"; //graphical "switch user" utilities
  //QString pbiCMD1 = "pbi_makeport";  //command to create a PBI from ports
  //QString pbiCMD2 = "pbi_create"; //command to create a PBI from local sources
  
  //Get the current application path
  //QString cpath = QCoreApplication::applicationDirPath();
  //if(cpath.endsWith("/.sbin")){ cpath.chop(6); } //Fix for PBI installation of EasyPBI
  //Set the search paths
  QStringList paths;
  paths << QString(getenv("PATH")).split(":"); //"/usr/local/bin/"<<"/usr/local/sbin/"<<"/usr/bin/"<<"/usr/sbin/"<<cpath+"/bin/"<<cpath+"/sbin/";
  //qDebug() << "Paths:" << paths;
  //Perform the Search
  for(int i=0; i<paths.length(); i++){
    //PBI build commands
    /*if(!pbifound1){
      if(QFile::exists(paths[i]+pbiCMD1)){
        pbifound1 = true;
        detStruct[0] = paths[i]+pbiCMD1; //pbi_makeport
      }
    }
    if(!pbifound2){
      if(QFile::exists(paths[i]+pbiCMD2)){
        pbifound2 = true;
        detStruct[1] = paths[i]+pbiCMD2; //pbi_create
      }
    }*/
    //SU utility
    if(!sufound){
      //qDebug() << "Look for SU:" << paths[i];
      for(int j=0; j<suCMD.length(); j++){
        if(QFile::exists(paths[i]+"/"+suCMD[j])){
      	  sufound = true;
      	  detStruct[2] = paths[i]+"/"+suCMD[j];  //su utility
      	  break;
      	}
      }    	    
    }
  } // end loop over paths
  
  //Now search for the FreeBSD ports tree
    QString ret;
    QStringList portsLocations;
    //Set the locations to search for the ports tree
    portsLocations << "/usr/ports" << defaultSettings[1]+"ports";
    //Search the locations
    for(int i=0; i<portsLocations.size(); i++){
      if( QDir(portsLocations[i]).exists() ){
        if( QFile::exists(portsLocations[i]+"/COPYRIGHT") ){
	  detStruct[3]=portsLocations[i];
	}
      }
    }
    
  //Now Check the structures and set any internal flags
  checkStructures();
}

void Config::returnToDefaults(){
    //Clear the structures
    for(int i=0; i<valueStruct.length(); i++){ valueStruct[i].clear(); }
    for(int i=0; i<exProgStruct.length(); i++){ exProgStruct[i].clear(); }
    for(int i=1; i<TFstruct.length(); i++){ TFstruct[i] = false; } //skip the 64-bit architecture flag
    //Now re-populate the structures from the defaults
    checkStructures();
}

QString Config::value(QString var){
//defaultSettings=[ current version#, programDir, pbiOutputDir, 
	//moduleOutputDir, pkgCacheDir, iconFile, settingsFile, iconDir]
    var = var.toLower();
    if(var=="pbi_makeport"){ return exProgStruct[0]; }
    else if(var=="pbi_create"){ return exProgStruct[1]; }
    else if(var=="su_cmd"){ return exProgStruct[2]; }
    else if(var=="pbidir"){ return valueStruct[0]; }
    else if(var=="moduledir"){ return valueStruct[1]; }
    else if(var=="cachedir"){ return valueStruct[2]; }
    else if(var=="portsdir"){ return valueStruct[3]; }
    else if(var=="icondir"){ return valueStruct[4]; }
    else if(var=="defaulticon"){ return valueStruct[5]; }
    else if(var=="sigfile"){ return valueStruct[6]; }
    else if(var=="version"){ return defaultSettings[0]; }
    else if(var=="progdir"){ return defaultSettings[1]; }
    else{ 
      qDebug() << "invalid value requested:" << var;
      return ""; 
    }
}

void Config::setValue(QString var, QString val){
    var = var.toLower();
    if( !val.isEmpty() ){
      if( !QFile::exists(val) ){ 
        qDebug() << "WARNING! \""+var+"\" setting could not be changed: desired file does not exist:" + val;
      }
      else if(var=="pbi_makeport"){ exProgStruct[0]=val; }
      else if(var=="pbi_create"){ exProgStruct[1]=val; }
      else if(var=="su_cmd"){ exProgStruct[2]=val; }
      else if(var=="pbidir"){ valueStruct[0]=val; }
      else if(var=="moduledir"){ valueStruct[1]=val; }
      else if(var=="cachedir"){ valueStruct[2]=val; }
      else if(var=="portsdir"){ valueStruct[3]=val; }
      else if(var=="icondir"){ valueStruct[4]=val; }
      else if(var=="defaulticon"){ valueStruct[5]=val; }
      else if(var=="sigfile"){ valueStruct[6]=val; }
      else{ qDebug() << "invalid setvalue requested:" << var; }
      
    }else{ //reset default value
	//detStruct=[ pbi_makeport, pbi_create, su_utility, portsDir ]
      if(var=="pbi_makeport"){ exProgStruct[0]=detStruct[0]; }
      else if(var=="pbi_create"){ exProgStruct[1]=detStruct[1]; }
      else if(var=="su_cmd"){ exProgStruct[2]=detStruct[2]; }
      else if(var=="pbidir"){ valueStruct[0]=defaultSettings[2]; }
      else if(var=="moduledir"){ valueStruct[1]=defaultSettings[3]; }
      else if(var=="cachedir"){ valueStruct[2]=defaultSettings[4]; }
      else if(var=="portsdir"){ valueStruct[3]=detStruct[3]; }
      else if(var=="icondir"){ valueStruct[4]=defaultSettings[7]; }
      else if(var=="defaulticon"){ valueStruct[5]=defaultSettings[5]; }
      else if(var=="sigfile"){ valueStruct[6]=""; }
      else{ qDebug() << "invalid setvalue requested:" << var; }	    
    }
  checkStructures();
}

QStringList Config::list(QString var){
    var = var.toLower();
    if(var=="skippkgs"){ return badPackages; }
    else{ 
      qDebug() << "invalid list requested:"<<var;
      return QStringList();
    }
}

void Config::setList(QString var, QStringList val){
  var=var.toLower();
  if(var=="skippkgs"){ badPackages = val; }
  else{ qDebug() << "invalid setlist requested:"<<var; }
}

bool Config::check(QString var){
  var = var.toLower();
  if(var=="is64bit"){ return TFstruct[0]; }
  else if(var=="isportsavailable"){ return TFstruct[1]; }
  else if(var=="usesignature"){ return TFstruct[2]; }
  else if(var=="usetmpfs"){ return TFstruct[3]; }
  else if(var=="usecache"){ return TFstruct[4]; }
  else if(var=="ismakeportavailable"){ return TFstruct[5]; }
  else if(var=="iscreateavailable"){ return TFstruct[6]; }
  else if(var=="issuavailable"){ return TFstruct[7]; }
  else{ 
    qDebug() << "invalid check requested:" << var;
    return false; 
  }
}

void Config::setCheck(QString var, bool val){
  var = var.toLower();
  if(var=="usesignature"){ TFstruct[2] = val; }
  else if(var=="usetmpfs"){ TFstruct[3] = val; }
  else if(var=="usecache"){ TFstruct[4] = val; }
  else{ qDebug() << "invalid setcheck requested:" << var; }
}

// *********************************************************
// -----------PRIVATE FUNCTIONS FOR INTERNAL USE-----------
//**********************************************************

QString Config::readSavedValue(QString var, QString line){
  //var = "<something>"
  //line = line with the value on it
  QString varend = var;
  varend.replace("<","</");
  QString out = line.section(var,1,1).section(varend,0,0).simplified();
  return out;
}

void Config::checkStructures(){
  //Check and replace any empty variables with the defaults	
    //exProgStruct=[ pbi_makeport, pbi_create, su_utility ]
    //valueStruct=[ pbiOutputDir, moduleOutputDir, pkgCacheDir, portsDir, iconDir, defaultIcon, sigFile ]
    //detStruct=[ pbi_makeport, pbi_create, su_utility, portsDir ]
    //defaultSettings=[ current version#, programDir, pbiOutputDir, moduleOutputDir, pkgCacheDir, iconFile, settingsFile, iconDir]
    //TFstruct=[ is64-bitArch, portsAvailable, useDigitalSig, useTMPFS, usePkgCache, pbi_makeport_available, pbi_create_available, su_available ]

  //Internal Variables
  // -- PBI output dir
  if( valueStruct[0].isEmpty() ){ valueStruct[0] = defaultSettings[2]; }
  // -- Module output dir
  if( valueStruct[1].isEmpty() ){ valueStruct[1] = defaultSettings[3]; }
  // -- Package Cache dir
  if( valueStruct[2].isEmpty() ){ valueStruct[2] = defaultSettings[4]; }
  // -- FreeBSD ports tree
  if( valueStruct[3].isEmpty() ){ 
    if(detStruct[3].isEmpty()){ TFstruct[1] = false; }
    else{ valueStruct[3] = detStruct[3]; TFstruct[1]=true; }
  }else if( QFile::exists(valueStruct[3]+"/COPYRIGHT") ){ 
    TFstruct[1]=true; 
  }
  
  // -- Icon dir
  if( valueStruct[4].isEmpty() ){ valueStruct[4] = defaultSettings[7]; }
  // -- default icon file
  if( valueStruct[5].isEmpty() ){ valueStruct[5] = defaultSettings[5]; }
  
  //External utilities
  // -- pbi_makeport
  /*if( exProgStruct[0].isEmpty() ){ 
    if(detStruct[0].isEmpty()){ TFstruct[5] = false; }
    else{ exProgStruct[0] = detStruct[0]; TFstruct[5]=true; }
  }
  // -- pbi_create
  if( exProgStruct[1].isEmpty() ){ 
    if(detStruct[1].isEmpty()){ TFstruct[6] = false; }
    else{ exProgStruct[1] = detStruct[1]; TFstruct[6]=true; }
  }*/
  // -- graphical su utility
  //qDebug() << "Checking SU utility:" << exProgStruct[2] << detStruct[2] << TFstruct[7];
  if( exProgStruct[2].isEmpty() ){ 
    if(detStruct[2].isEmpty()){ TFstruct[7] = false; }
    else{ exProgStruct[2] = detStruct[2]; TFstruct[7]=true; }
  }else{ TFstruct[7] = true; }
}
