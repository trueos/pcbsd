#include "PBIModule.h"

PBIModule::PBIModule(){
  //Setup the possible values that are recognized
    // 10.x PBI format: 5/20/14
  version = "10.x (5/21/14)";
  //pbi.conf values
  OldValues << "PBI_MAKEPORT" << "PBI_MKPORTAFTER" << "PBI_MKPORTBEFORE";
	
  CTextValues << "PBI_PROGNAME" << "PBI_PROGWEB" << "PBI_PROGAUTHOR" << "PBI_DESC" << "PBI_SHORTDESC" \
		<< "PBI_LICENSE" << "PBI_TAGS" << "PBI_PROGTYPE" << "PBI_ORIGIN";
  CListValues << "PBI_OTHERPKGS" << "PBI_PLUGINS" << "PBI_SCREENSHOTS" << "PBI_RELATED";

  //Valid Scripts
  scriptValues << "post-install.sh" << "pre-remove.sh";
  serviceScripts << "setconfig.sh" << "getconfig.sh" << "doneconfig.sh";
  //valid XDG values
  xdgTextValues << "Value" << "Type" << "Name" << "GenericName" << "Exec" << "Path" << "Icon" << "Categories" << "MimeType";
  xdgBoolValues << "StartupNotify" << "Terminal" << "NoDisplay";
  //valid MIME values
     // NOTE: These values are hard-coded in the file read/write below due to specific file format
  mimeValues << "xmlns" << "type" << "pattern"; 
}

PBIModule::~PBIModule(){}
	
// ==============
//  Initial load
// ==============
bool PBIModule::loadModule(QString confpath){
  if(!confpath.endsWith("/pbi.conf")){ return false; }
  //Get the base Path and make sure it exists
  basePath = confpath.left(confpath.length()-9); //base directory path
  HASH.clear(); //Make sure we always start with a clean slate when loading a module
  if(!QFile::exists(basePath)){ basePath.clear(); return false; }
  return true;
}
	
QString PBIModule::basepath(){
  return basePath;
}

// ==================
//  CONFIGURATION VALUES
// ==================
QString PBIModule::stringVal(QString var){
  if(HASH.contains(var)){ return HASH[var].toString(); }
  else{ return ""; }
}

void PBIModule::setStringVal(QString var, QString val){
  if(CTextValues.contains(var)){ HASH.insert(var, val); }
}

QStringList PBIModule::listVal(QString var){
  if(HASH.contains(var)){ return HASH[var].toStringList(); }
  else{ return QStringList(); }	
}

void PBIModule::setListVal(QString var, QStringList val){
  if(CListValues.contains(var)){ HASH.insert(var, val); }
}

QIcon PBIModule::appIcon(){
  if(QFile::exists(basePath+"/icon.png")){ return QIcon(basePath+"/icon.png"); }
  else{ return QIcon(); }
}
	
bool PBIModule::setAppIcon(QString iconpath){
  //Copy the icon file over to the module
  if(QFile::exists(basePath+"/icon.png")){ QFile::remove(basePath+"/icon.png"); }
  bool ok = QFile::copy(iconpath, basePath+"/icon.png");
  if(ok){ QFile::setPermissions( basePath+"/icon.png",  QFile::ReadOther | QFile::WriteOther | QFile::ReadOwner | QFile::WriteOwner | QFile::ReadGroup | QFile::WriteGroup); }
  return ok;
}

void PBIModule::loadConfig(){
  //Read the designated pbi.conf and store the variables
  QStringList contents = readFile(basePath+"/pbi.conf");
  for(int i=0; i<contents.length(); i++){
    //see if the current line conains a desired variable and save it
    QString line = contents[i].simplified();
    if(!line.startsWith("#") && !line.startsWith("export") && !line.isEmpty()){ //Ignore commented out lines
      //Pull apart the variable and the value
      QString var = line.section("=\"",0,0,QString::SectionSkipEmpty).trimmed();
      QString val = line.section("=\"",1,50).section("#",0,0,QString::SectionSkipEmpty).section(";",0,0,QString::SectionSkipEmpty).trimmed();
        while( !val.endsWith("\"") && i<(contents.length()-1) ){
	  i++;
	  line = contents[i];
	  val.append( "\n"+line.section("#",0,0).section(";",0,0).trimmed() );
	}
	if(val.endsWith("\"")){ val.chop(1); } //remove the ending quote
      //qDebug() << "var="+var+"\t\tval="+val;

      //Now check for text/bool/int values
      if(var=="PBI_MAKEPORT"){HASH.insert("PBI_ORIGIN", val); }
      else if(OldValues.contains(var)){ 
	QStringList old =  HASH.value("PBI_OTHERPKGS", QStringList()).toStringList();
	  old.append( val.replace("\n", " ").split(" ") );
	HASH.insert("PBI_OTHERPKGS", old);
      }else if(CTextValues.contains(var)){ HASH.insert(var,val); }
      else if(CListValues.contains(var)){ HASH.insert(var, val.replace("\n"," ").split(" ") ); }
      else{} //do nothing for extra lines
    }
  }
}

bool PBIModule::saveConfig(){
//Get the path to the module's pbi.conf
  QString fileName = basePath+"/pbi.conf";

 //Create the contents of the file to write
  QStringList contents;
  QString exportLine = "export";
  contents << "#!/bin/sh";
  contents << "# "+version+" PBING Build Configuration";
  contents << "# Generated using EasyPBI\n";
  QStringList exportVariables;
  //Text Values
  for(int i=0; i<CTextValues.length(); i++){
    //Only set the variable if appropriate
    if(HASH.contains(CTextValues[i])){ 
      QString line = CTextValues[i]+"=\"";
      QString val = HASH[CTextValues[i]].toString();

      if(!val.isEmpty()){
        //special check for PBI_ORIGIN format validity
        if(CTextValues[i]=="PBI_ORIGIN" && val.endsWith("/")){ val.chop(1); }
        line.append( val ); 
	exportVariables << CTextValues[i]; //Make sure this variable is exported
        line.append("\""); //close out the quotes
        contents << line; //add this variable to the file
      }
    }
    
  }
  //Boolian Values
  for(int i=0; i<CListValues.length(); i++){
    if(HASH.contains(CListValues[i])){ 
	QString line = CListValues[i]+"=\""+HASH[CListValues[i]].toStringList().join(" ")+"\"";
        exportVariables << CListValues[i];
        contents << line;   
    }
    
  }
  
  //Export Line
  contents << "";
  contents << "export "+exportVariables.join(" ");
  
  //Create the File
  bool ok = createFile(fileName, contents);
  if(!ok){ qDebug() << "Warning: Could not save "+fileName; }
  return ok;
}

// ==========
//     SCRIPTS
// ==========
QStringList PBIModule::readScript(QString var){
  QStringList out;
  if( scriptValues.contains(var) || serviceScripts.contains(var) || QFile::exists(basePath+"/scripts/"+var) ){
    out = readFile(basePath+"/scripts/"+var);
  }	  
  return out;
}

bool PBIModule::removeScript(QString var){
  return createFile(basePath+"/scripts/"+var, QStringList());  	
}

bool PBIModule::writeScript(QString var,QStringList val){
  bool ok = false;
  if( scriptValues.contains(var) || serviceScripts.contains(var) || QFile::exists(basePath+"/scripts/"+var) ){
    ok = createFile(basePath+"/scripts/"+var, val);
  }
  return ok;
}

QStringList PBIModule::validScripts(){
  return scriptValues;
}

QStringList PBIModule::existingScripts(){
  QStringList out;
  QDir dir(basePath+"/scripts");
  if(dir.exists()){
    out = dir.entryList(QStringList() << "*.sh", QDir::Files | QDir::NoDotAndDotDot, QDir::Name);
  }
  //Make sure the service scripts are not listed here
  for(int i=0; i<serviceScripts.length(); i++){
    out.removeAll(serviceScripts[i]);
  }
  return out;
}

// =============
//      XDG
// =============
QStringList PBIModule::validXdgText(){
  return xdgTextValues;
}

QString PBIModule::xdgText(QString var){
  QString out;
  if(xdgTextValues.contains(var) && HASH.contains("XDG_"+var) ){
    out = HASH["XDG_"+var].toString();
  }
  return out;
}

QStringList PBIModule::xdgTextL(QStringList vars){
  QStringList out;
  for(int i=0; i<vars.length(); i++){
    out << xdgText(vars[i]);
  }
  return out;
}

void PBIModule::setXdgText(QString var, QString val){
  if(xdgTextValues.contains(var)){
    HASH.insert("XDG_"+var, val);
  }
}

void PBIModule::setXdgTextL(QStringList vars, QStringList vals){
  if(vars.length() != vals.length()){ 
    qDebug() << "Error: XDG text inputs are unequal lengths:" << vars << vals;
    return; 
  }
  for(int i=0; i<vars.length(); i++){
    setXdgText(vars[i], vals[i]);
  }
}

	
QStringList PBIModule::validXdgEnables(){
  return xdgBoolValues;
}

bool PBIModule::xdgEnabled(QString var){
  bool out = false;
  if(xdgBoolValues.contains(var) && HASH.contains("XDG_"+var)){
    out = HASH["XDG_"+var].toBool();
  }
  return out;
}

void PBIModule::setXdgEnabled(QString var, bool val){
  if(xdgBoolValues.contains(var) ){
    HASH.insert("XDG_"+var, val);
  }
}

	
QStringList PBIModule::listXdgDesktopFiles(){
  QStringList out;
  QDir dir(basePath+"/xdg-desktop");
  if(dir.exists()){
    out = dir.entryList(QStringList() << "*.desktop", QDir::Files | QDir::NoDotAndDotDot, QDir::Name);
  }
  return out;
}

QStringList PBIModule::listXdgMenuFiles(){
  QStringList out;
  QDir dir(basePath+"/xdg-menu");
  if(dir.exists()){
    out = dir.entryList(QStringList() << "*.desktop", QDir::Files | QDir::NoDotAndDotDot, QDir::Name);
  }
  return out;	
}

bool PBIModule::saveXdgDesktop(QString fileName){
  if(fileName.isEmpty()){ return false; }
  if(!fileName.endsWith(".desktop")){ fileName.append(".desktop"); }
  QStringList contents;
  contents << "#!/usr/bin/env";
  contents << "[Desktop Entry]";	
  for(int i=0; i<xdgTextValues.length(); i++){
    if(HASH.contains("XDG_"+xdgTextValues[i])){
      contents << xdgTextValues[i]+"="+HASH["XDG_"+xdgTextValues[i]].toString();
    }
  }
  for(int i=0; i<xdgBoolValues.length(); i++){
    if(HASH.contains("XDG_"+xdgBoolValues[i])){
      if(HASH["XDG_"+xdgBoolValues[i]].toBool()){
        contents << xdgBoolValues[i]+"=true";
      }
    }
  }
  return createFile(basePath+"/xdg-desktop/"+fileName, contents);
}

bool PBIModule::saveXdgMenu(QString fileName){
  if(fileName.isEmpty()){ return false; }
  if(!fileName.endsWith(".desktop")){ fileName.append(".desktop"); }
  QStringList contents;
  contents << "#!/usr/bin/env";
  contents << "[Desktop Entry]";	
  for(int i=0; i<xdgTextValues.length(); i++){
    if(HASH.contains("XDG_"+xdgTextValues[i])){
      contents << xdgTextValues[i]+"="+HASH["XDG_"+xdgTextValues[i]].toString();
    }
  }
  for(int i=0; i<xdgBoolValues.length(); i++){
    if(HASH.contains("XDG_"+xdgBoolValues[i])){
      if(HASH["XDG_"+xdgBoolValues[i]].toBool()){
        contents << xdgBoolValues[i]+"=true";
      }
    }
  }
  return createFile(basePath+"/xdg-menu/"+fileName, contents);	
}

bool PBIModule::removeXdgDesktop(QString fileName){
  return createFile(basePath+"/xdg-desktop/"+fileName, QStringList() );
}

bool PBIModule::removeXdgMenu(QString fileName){
  return createFile(basePath+"/xdg-menu/"+fileName, QStringList() );	
}

bool PBIModule::loadXdgDesktop(QString fileName){
  clearXdgData();
  QStringList contents = readFile(basePath+"/xdg-desktop/"+fileName);
  if(contents.isEmpty()){ return false; }
  QStringList extraLines;
  for(int i=0; i<contents.length(); i++){
    //Ignore specific/special lines
    if(contents[i].startsWith("#") || contents[i].startsWith("[Desktop Entry]") || contents[i].isEmpty() ){ continue; }
    //Now check for known variables
    QString var = contents[i].section("=",0,0).simplified();
    QString val = contents[i].section("=",1,50).simplified();
    if(xdgTextValues.contains(var)){
      HASH.insert("XDG_"+var, val);
    }else if(xdgBoolValues.contains(var)){
      HASH.insert("XDG_"+var, val.toLower() == "true" );
    }else{
      extraLines << contents[i];
    }
  }
  if( !extraLines.isEmpty() ){
    HASH.insert("XDG_EXTRALINES",extraLines.join("\n"));
  }
  return true;
}

bool PBIModule::loadXdgMenu(QString fileName){
  clearXdgData();
  QStringList contents = readFile(basePath+"/xdg-menu/"+fileName);
  if(contents.isEmpty()){ return false; }
  QStringList extraLines;
  for(int i=0; i<contents.length(); i++){
    //Ignore specific/special lines
    if(contents[i].startsWith("#") || contents[i].startsWith("[Desktop Entry]") || contents[i].isEmpty() ){ continue; }
    //Now check for known variables
    QString var = contents[i].section("=",0,0).simplified();
    QString val = contents[i].section("=",1,50).simplified();
    if(xdgTextValues.contains(var)){
      HASH.insert("XDG_"+var, val);
    }else if(xdgBoolValues.contains(var)){
      HASH.insert("XDG_"+var, val.toLower() == "true" );
    }else{
      extraLines << contents[i];
    }
  }
  if( !extraLines.isEmpty() ){
    HASH.insert("XDG_EXTRALINES",extraLines.join("\n"));
  }
  return true;	
}

	
QStringList PBIModule::listMimeFiles(){
  QStringList out;
  QDir dir(basePath+"/xdg-mime");
  if(dir.exists()){
    out = dir.entryList(QStringList() << "*.xml", QDir::Files | QDir::NoDotAndDotDot, QDir::Name);
  }
  return out;	
}

QStringList PBIModule::validMimeValues(){
  return mimeValues;
}

QString PBIModule::mimeText(QString var){
  QString out;
  if(mimeValues.contains(var) && HASH.contains("MIME_"+var)){
    out = HASH["MIME_"+var].toString();
  }
  return out;
}

QStringList PBIModule::mimeTextL(QStringList vars){
  QStringList out;
  for(int i=0; i<vars.length(); i++){
    out << mimeText(vars[i]);
  }
  return out;
}

void PBIModule::setMimeText(QString var, QString val){
  if(mimeValues.contains(var)){
    HASH.insert("MIME_"+var, val);
  }
}

void PBIModule::setMimeTextL(QStringList vars, QStringList vals){
  if(vars.length() != vals.length()){ return; }
  for(int i=0; i<vars.length(); i++){
    setMimeText(vars[i], vals[i]);
  }
}

bool PBIModule::saveMimeFile(QString fileName){
  if(fileName.isEmpty()){ return false; }
  if(!fileName.endsWith(".xml")){ fileName.append(".xml"); }
  //Check for type/pattern(s) first
  if( !HASH.contains("MIME_type") || !HASH.contains("MIME_pattern") ){
    qDebug() << "Error: Not enough information to create MIME file:" << fileName;
    return false;
  }
  //Now create the file contents
  QStringList contents;
  contents << "<?xml version=\"1.0\"?>";
  QString xmlns = "http://www.freedesktop.org/standards/shared-mime-info";
  if(HASH.contains("MIME_xmlns")){ xmlns = HASH["MIME_xmlns"].toString(); }
  contents <<"<mime-info xmlns=\'"+xmlns+"\'>";
  contents <<" <mime-type type=\""+HASH["MIME_type"].toString()+"\">";
  QStringList patterns = HASH["MIME_pattern"].toString().split(" ");
  for(int i=0; i<patterns.length(); i++){
    contents << "  <glob weight=\"100\" pattern=\""+patterns[i]+"\"/>";
  }
  contents << " </mime-type>";
  contents << "</mime-info>";
  //Now create the file
  return createFile(basePath+"/xdg-mime/"+fileName, contents);
}

bool PBIModule::removeMimeFile(QString fileName){
  return createFile(basePath+"/xdg-mime/"+fileName, QStringList() );
}

bool PBIModule::loadMimeFile(QString fileName){
  QStringList contents = readFile(basePath+"/xdg-mime/"+fileName);
  if(contents.isEmpty()){ return false; }
  QStringList patterns;
  for(int i=0; i<contents.length(); i++){
    if(contents[i].contains("xml version=")){
      continue; //ignore this line
    }else if(contents[i].contains("<mime-info xmlns=\'")){
      HASH.insert("MIME_xmlns", contents[i].section("=",1,1).section(">",0,0).remove("\'").remove("\"") );
    }else if(contents[i].contains("<mime-type type=")){
      HASH.insert("MIME_type", contents[i].section("=",1,1).section(">",0,0).remove("\'").remove("\"") );
    }else if(contents[i].contains("<glob ")){
      patterns << contents[i].section("=",2,2).section("/>",0,0).remove("\'").remove("\"");
    }
  }
  if(!patterns.isEmpty()){
    HASH.insert("MIME_pattern", patterns.join(" ") );
  }
  return true;
}

// ===================
//  SERVICE CONFIGURATION
// ===================
bool PBIModule::saveServiceConfig(){
  QStringList contents;
  for(int i=0; i<ServiceOptions.length(); i++){
    contents << ServiceOptions[i].convertToText(i);
    contents << "";
  }
  if(!contents.isEmpty()){
    //Put the appropriate tags around the entries
    contents.prepend("<?");
    contents.append("?>");
  }
  return PBIModule::createFile(basePath+"/service-configfile",contents);
}

void PBIModule::loadServiceConfig(){
  QStringList contents = PBIModule::readFile(basePath+"/service-configfile");
  //qDebug() << "Service Config:\n" << contents;
  ServiceOptions.clear();
  if(!contents.isEmpty()){
    bool keepgoing = true;
    int i = 0;
    while(keepgoing){
      ServiceOption opt;
      opt.LoadFromText(contents, i);
      if(opt.isValid()){
	ServiceOptions << opt; //save it into the array
	i++; //go on to the next number
      }else{
	break; //quit now (no more options listed)
      }
    }
  }
}


// ===============
//  GENERAL UTILITIES
// ===============
bool PBIModule::createFile(QString fileName, QStringList contents){
//fileName = full path to file (I.E. /home/pcbsd/junk/junk.txt)
//contents = list of lines to be written (one line per entry in the list - no newline needed at the end of an entry)
	
  //Check if this is actually a file removal
  if(contents.isEmpty() && QFile::exists(fileName)){
    bool good = QFile::remove(fileName);
    if(!good){
      qDebug() << fileName+": Could not be deleted";
    }
    return good;
  }else if(contents.isEmpty()){
    return true; //File to delete already does not exist 
  }
  
  //Check that the parent directory exists, and create it if necessary
  QString pDir = fileName;
  pDir.chop( fileName.section("/",-1).length() );
  QDir dir(pDir);
    if(!dir.exists()){ 
      qDebug() << "Create parent Directory:" << dir.absolutePath();
      if( !dir.mkpath(dir.absolutePath()) ){
        qDebug() << fileName+": Could not create containing directory";
	return false;
      }
    }

  //Open the file with .tmp extension
  QFile file(fileName+".tmp");
  if( !file.open(QIODevice::WriteOnly | QIODevice::Text) ){
    qDebug() << fileName+".tmp: Failure -- Could not open file";
    return false;
  }
  //Write the file
  QTextStream ofile(&file); //start the output stream
  for(int i=0; i<contents.length(); i++){
    ofile << contents[i];
    ofile << "\n";
  }
  //Close the File
  file.close();
  //Remove any existing file with the final name/location
  if( QFile::exists(fileName) ){
    if( !QFile::remove(fileName) ){
      qDebug() << fileName+": Error -- Could not overwrite existing file";
      QFile::remove(fileName+".tmp");
      return false;
    }
  }
  //Move the temporary file into its final location
  if( !file.rename(fileName) ){
    qDebug() << fileName+": Error: Could not rename "+fileName+".tmp as "+fileName;
    return false;
  }
  //Return success
  QString extra = QDir::homePath(); //remove this from the filename display
  qDebug() << "Saved:" << fileName.replace(extra,"~");
  return true;;
}

QStringList PBIModule::readFile(QString filePath){
  QStringList contents;
  //Check that the file exists first
  if(!QFile::exists(filePath)){ 
    qDebug() << "Warning: file to read does not exist:" << filePath;
    return contents; 
  }
  //Open the file for reading
  QFile file(filePath);
  if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){ 
    qDebug() << "Error: file could not be opened for reading:"<< filePath;
    return contents; 
  }
  QTextStream in(&file);
  //Save the contents as a QStringList
  while(!in.atEnd()){
    contents << in.readLine();
  }
  //Return the contents (one entry per line)
  return contents;
}

QStringList PBIModule::filesInDir(QString dirPath){
  //This is a recursive function for listing all the files in a directory (or subdirectories)
  QStringList out;
  QDir dir(dirPath);
  if(dir.exists(dirPath)){
    QStringList subdirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);
    out << dir.entryList(QDir::Files | QDir::NoDotAndDotDot,QDir::Name);
    for(int i=0; i<subdirs.length(); i++){
      QStringList subFiles = filesInDir(dirPath+"/"+subdirs[i]);
      for(int j=0; j<subFiles.length(); j++){
        //Make sure to prepend the subdirectory name
        out << subdirs[i]+"/"+subFiles[j];
      }
  }
  }
  return out;	
}

void PBIModule::clearXdgData(){
  for(int i=0; i<xdgTextValues.length(); i++){
    if(HASH.contains("XDG_"+xdgTextValues[i])){
      HASH.remove("XDG_"+xdgTextValues[i]);
    }
  }
  for(int i=0; i<xdgBoolValues.length(); i++){
    if(HASH.contains("XDG_"+xdgBoolValues[i])){
      HASH.remove("XDG_"+xdgBoolValues[i]);
    }
  }
  if(HASH.contains("XDG_EXTRALINES")){
    HASH.remove("XDG_EXTRALINES");
  }
}

void PBIModule::clearMimeData(){
  for(int i=0; i<mimeValues.length(); i++){
    if(HASH.contains("MIME_"+mimeValues[i])){
      HASH.remove("MIME_"+mimeValues[i]);
    }
  }	  
}

