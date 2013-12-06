#include "PBImodule.h"

PBIModule::PBIModule(){
  //Setup the possible values that are recognized
    // 10.x PBI format: 12/5/13
  version = "10.x"
  CTextValues << "PBI_PROGNAME" << "PBI_PROGWEB" << "PBI_PROGAUTHOR" << "PBI_PROGICON" \
		<< "PBI_LICENCE" << "PBI_TAGS" << "PBI_PROGTYPE" << "PBI_ICONURL" << "PBI_CATEGORY" \
		<< "PBI_MAKEPORT" << "PBI_MKPORTAFTER" << "PBI_MKPORTBEFORE" << "PBI_MAKEOPTS";
  CBoolValues << "PBI_REQUIRESROOT" << "PBI_AB_NOTMPFS" << "PBI_AB_NOPKGBUILD";
  CIntValues << "PBI_BUILDKEY" << "PBI_PROGREVISION" << "PBI_AB_PRIORITY";
  HASH.clear(); //Make sure the hash is currently empty
}

PBIModule::~PBIModule(){}
	
// ==============
//  Modules READ/WRITE
// ==============
bool PBIModule::loadModule(QString confpath){
  if(!confpath.endsWith("/pbi.conf")){ return false; }
  //Get the base Path and make sure it exists
  basePath = confpath.left(confpath.length()-9); //base directory path
  if(!QFile::exists(basePath)){ basePath.clear(); return false; }
  //This is good to go: now start to load all the different pieces of the module
  this->loadConfig(); //pbi.conf
  
  return true;
}
	
// ==================
//  CONFIGURATION VALUES
// ==================
QStringList PBIModule::textL(QStringList vars){ //a list of individual QString values
  QString out;
  for(int i=0; i<vars.length(); i++){
    out << this->text(vars[i]);
  }
  return out;
}

void PBIModule::setTextL(QStringList vars, QStringList vals){ //set a list of QString values
  if( vars.length() != vals.length() ){ return; }
  for(int i=0; i<<vars.length(); i++){
    this->setText(vars[i], vals[i]);
  }
}

QString PBIModule::text(QString var){ //a single QString value
  if( CTextValues.contains(var) && HASH.contains(var) ){ 
    return HASH[var].toString();
  }else{
    return "";
  }
}

void PBIModule::setText(QString var, QString val){ //set a single QString value
  if( !CTextValues.contains(var) ){ 
    return; 
  }else{
    HASH.insert(var,val);
  }
}

QStringList PBIModule::textValues(){	//list all the possible values
  return CTextValues;
}

bool PBIModule::isEnabled(QString var){
  if(CBoolValues.contains(var) && HASH.contains(var) ){ 
    return HASH[var].toBool();
  }else{ 
    return false; 
  }
}

void PBIModule::setEnabled(QString var, bool val){
  if(CBoolValues.contains(var)){ 
    HASH.insert(var,val);
  }
}

QStringList PBIModule::enableValues(){ //list all the possible enables
  return CBoolValues;
}

int PBIModule::number(QString var){
  if(CIntValues.contains(var) && HASH.contains(var) ){
    return HASH[var].toInt();
  }else{
    return -1;
  }
}

void PBIModule::setNumber(QString var, int val){
  if(CIntValues.contains(var)){
    HASH.insert(var,val);
  }
}

QStringList PBIModule::numberValues(){ //list all possible integer values
  return CIntValues;
}
	
void PBIModule::loadConfig(){
//Read the designated pbi.conf and store the variables
  QFile file(basePath+"/pbi.conf");
  if( !file.open(QIODevice::ReadOnly | QIODevice::Text)){
    qDebug() << "Warning: Unable to load "+basePath+"/pbi.conf";
    return;
  }
  QTextStream in(&file);
  while(!in.atEnd()){
    //see if the current line conains a desired variable and save it
    QString line = in.readLine();
    if(!line.startsWith("#") && !line.startsWith("export")){ //Ignore commented out lines
      //Pull apart the variable and the value
      QString var = line.section("=\"",0,0,QString::SectionSkipEmpty).trimmed();
      QString val = line.section("=\"",1,50).section("#",0,0,QString::SectionSkipEmpty).section(";",0,0,QString::SectionSkipEmpty).trimmed();
        while( !val.endsWith("\"") && !in.atEnd() ){
	  line = in.readLine();
	  val.append("\n"+line.section("#",0,0).section(";",0,0).trimmed();
	}
	if(val.endsWith("\"")){ val.chop(1); } //remove the ending quote
      //Now check for text/bool/int values
      if(CTextValues.contains(var)){ HASH.insert(var,val); }
      else if(CBoolValues.contains(var)){ HASH.insert(var, (val.toLower()=="yes" || val.toLower()=="true") ); }
      else if(CIntValues.contains(var)){ HASH.insert(var, val.toInt()); }
      else{} //do nothing for extra lines
    }
  }
  file.close();	
}

bool PBIModule::saveConfig(){
//Get the path to the module's pbi.conf
  QString fileName = basePath+"/pbi.conf";

 //Create the contents of the file to write
  QStringList contents;
  QString exportLine = "export";
  contents << "#!/bin/sh";
  contents << "# "+version+" PBI Build Configuration";
  contents << "# Generated using EasyPBI\n";
	
  //Text Values
  for(int i=0; i<CTextValues.length(); i++){
    QString line = CTextValues[i]+"=\"";
    if(HASH.contains(CTextValues[i])){ line.append( HASH[CTextValues[i]].toString() ); }
    line.append("\""); //close out the quotes
    contents << line;
  }
  //Boolian Values
  for(int i=0; i<CBoolValues.length(); i++){
    QString line = CBoolValues[i]+"=\"";
    if(HASH.contains(CBoolValues[i]) && HASH[CBoolValues[i]].toBool() ){ line.append("YES"); }
    else{ line.append("NO"); }
    line.append("\""); //close out the quotes
    contents << line;
  }
  //Integer Values
  for(int i=0; i<CIntValues.length(); i++){
    QString line = CIntValues[i]+"=\"";
    int val=-1;
    if(HASH.contains(CIntValues[i])){ val= HASH[CIntValues[i]].toInt(); }
    if(val > 0){ line.append( QString::number(val) ); } //only enter the value if greater than zero
    line.append("\""); //close out the quotes
    contents << line;
  }
  
  //Export Line
  contents << "";
  contents << "export "+CTextValues.join(" ")+" "+CBoolValues.join(" ")+" "+CIntValues.join(" ");
  
  //Create the File
  bool ok = createFile(fileName, contents);
  if(!ok){ qDebug() << "Warning: Could not save "+fileName; }
  return ok;
}

// ==========
//     SCRIPTS
// ==========
	
// =============
//      RESOURCES
// =============
	
// =============
//           XDG
// =============

// =============
//  EXTERNAL-LINKS
// =============

// =============
//  PRIVATE UTILITIES
// =============
bool PBIModule::createFile(QString fileName, QStringList contents){
//fileName = full path to file (I.E. /home/pcbsd/junk/junk.txt)
//contents = list of lines to be written (one line per entry in the list - no newline needed at the end of an entry)
  //Check that the parent directory exists, and create it if necessary
  QDir dir(fileName);
    dir.cdUp();
    if(!dir.exists()){ 
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
  qDebug() << "Created:" << fileName.replace(extra,"~");
  return true;;
}

QStringList PBIModule::readFile(QString filePath){
  QStringList contents;
  //Check that the file exists first
  if(!QFile::exists(filePath)){ 
    qDebug() << "Error: file to read does not exist:" << filePath;
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