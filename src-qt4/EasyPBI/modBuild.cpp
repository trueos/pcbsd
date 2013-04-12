#include "modBuild.h"

ModBuild::ModBuild(){
  // --Clear the internal variables--
  isPortPBI=FALSE; isLocalPBI=FALSE;
  progStruct.clear(); mkStruct.clear(); serverStruct.clear(); portStruct.clear();
  menuStruct.clear(); desktopStruct.clear(); mimeStruct.clear(); linksStruct.clear();
  // --Setup the structures with the proper sizes--
  progStruct <<""<<""<<""<<""<<""<<"";
    //progStruct=[ name, website, author, icon, version, packageDir]
  mkStruct <<""<<""<<""<<"";
    //mkStruct=[ makeport, portbefore, portafter, makeoptions]
  serverStruct <<"NO"<<"00"<<"00"<<"NO";
    //serverStruct=[ needroot, buildkey, priority, noTMPFS]
  menuStruct << ""<<""<<""<<""<<""<<""<<""<<"";
    //menuStruct=[ name, genericname, exec, icon, nodisplay, terminal, categories, mimetype]
  desktopStruct << ""<<""<<""<<""<<""<<""<<"";
    //desktopStruct=[ name, genericname, exec, icon, nodisplay, terminal, mimetype]
  mimeStruct << "" << "" << "";
    //mimeStruct=[ info, type, patterns] (patterns is a " "-delimited list)
  portStruct << "" << "" << "" << "" << "" << "" << "FALSE";
    //portStruct=[ name, author, website, options, binaries, menu-category, isGraphical ]
  validMenuCategories <<"AudioVideo"<<"Development"<<"Education"<<"Game"<<"Graphics"<<"Network"<<"Office"<<"Science"<<"Settings"<<"System"<<"Utility";  
}

ModBuild::~ModBuild(){
}

bool ModBuild::createNewModule(QString modPath, QString modType, QString modBase){
  //Set the module path for all the functions
  modulePath = modPath;	
  //Create the module directory structure if possible
  bool ok = createModuleDir(); //this will check for existing dir and overwrite it if possible
  if(!ok){return FALSE; }	
  //Get the type of PBI this module creates
  isPortPBI=FALSE; isLocalPBI=FALSE;
  if(modType.toLower() == "port"){ isPortPBI=TRUE; }
  else if(modType.toLower() == "local"){ isLocalPBI=TRUE; }
  else{
    qDebug() << "Cannot create module: Invalid module type - "+modType;
    return FALSE;
  }
  //Clear out all the module structures
  currentBins.clear(); currentIcons.clear(); currentMimeTypes.clear();
  loadDesktop(""); loadMenu(""); loadMime("");
  linksStruct.clear();  loadPBIconf();  readPortInformation("");
  //Try to auto-populate the module structures if possible
  if(isPortPBI){
    mkStruct[0] = modBase; //pbi_makeport
  }
  if(isLocalPBI){
    progStruct[0] = modBase.section("/",-1); //pbi_progname
    progStruct[5] = modBase;  //pbi_packagedir
  }
  //Now create the module contents
  writePBIconf();
  writeExternalLinks();
  return TRUE;
}

bool ModBuild::loadModule(QString modPath){
  //verify that the module exists
  QDir dir(modPath);
  if( !( dir.exists() && dir.exists("pbi.conf") ) ){
    qDebug() << "Cannot load module: not a valid module directory - "+modPath;
    return FALSE;
  }
  //Save which module this is
  modulePath = modPath;
  //Load pbi.conf
  loadPBIconf();
  //Determine the type of module by the pbi.conf entries
  isPortPBI=FALSE; isLocalPBI=FALSE;
  if(!progStruct[5].isEmpty()){ isLocalPBI=TRUE; } //check for PBI_PACKAGEDIR
  if(!mkStruct[0].isEmpty()){ isPortPBI=TRUE; } //check for PBI_MAKEPORT
  //Reset the current lists
  currentBins.clear(); currentIcons.clear(); currentMimeTypes.clear();
  //Reset the XDG file structures	
  loadDesktop(""); loadMenu(""); //Just clear these structures
  QStringList tmp = filesAvailable("xdg-mime");
  for(int i=0; i<tmp.length(); i++){
    loadMime(tmp[i]); //This will fill up the currentMimeTypes structure;
  }
  //Get the current resources
  tmp = filesAvailable("resources");
  for(int i=0; i<tmp.length(); i++){
    if(tmp[i].endsWith(".png")){ currentIcons << tmp[i]; }
    if(tmp[i].startsWith("bin/")){ currentBins << tmp[i]; }
  }
  //Load the external-links file
  if(!dir.exists("external-links")){
    linksStruct.clear();
    writeExternalLinks(); //Create the external-links file if it is missing
  }else{
    loadExternalLinks();
  }
  //Reset the ports structure
  readPortInformation(""); //should fill up the currentBins structure
  return TRUE;
}

bool ModBuild::readPortInformation(QString portPath){
    //Clear the data structure and initialize it
    portStruct.clear();
    portStruct << "" << "" << "" << "" << "" << "" << "FALSE";
    //portStruct=[ programName, author, website, options, binaries, menu-category, isGraphical ]
    if(portPath.isEmpty()){ return TRUE; } //catch to just reset the ports structure;
    qDebug() << "Reading port information for:" << portPath;
    //Read the Port files and save the info to the data structure
    QStringList actualopts, bins, listopts; //initialize lists
    bool variableBinFound = FALSE;
    //Load the Makefile(s)
    QDir pDir(portPath);
    QStringList mL = pDir.entryList(QStringList("Make*"),QDir::Files | QDir::NoDotAndDotDot | QDir::Readable);
    for(int i=0; i<mL.length(); i++){
      if( !mL[i].contains(".old") ){ //make sure to ignore any files labelled *.old
        int ok = readMakeFile(portPath+"/"+mL[i]);
        if(ok==-1){ return FALSE; }
        else if(ok==1){ variableBinFound=TRUE;}
      }
    }
    
    //Load pkg-descr file
    if(QFile::exists(portPath+"/pkg-descr")){
      QFile desfile(portPath+"/pkg-descr");
      if(!desfile.open(QIODevice::ReadOnly) ){
        return FALSE;
      }
      QTextStream desin(&desfile);
      while( !desin.atEnd() ){
        QString line = desin.readLine().simplified();
        // ---- Program Website -----
        if( line.startsWith("WWW:") ){
          portStruct[2] = line.replace("\t"," ").section(" ",1,1,QString::SectionSkipEmpty);
	  qDebug() << " - Found website:" << portStruct[2];
        // ---- Program Author -----
        }else if(line.startsWith("Author:") ){
	  portStruct[1] = line.replace("\t"," ").section(" ",1,4,QString::SectionSkipEmpty);
	  qDebug() << " - Found author:" << portStruct[1];
	}
      }
      desfile.close();
    }
    
    //Load the pkg-plist file
    if(QFile::exists(portPath+"/pkg-plist")){
      QFile pfile(portPath+"/pkg-plist");
      if(!pfile.open(QIODevice::ReadOnly) ){
        return FALSE;
      }
      QTextStream pin(&pfile);
      while( !pin.atEnd() ){
        QString line = pin.readLine().simplified();
        if( line.startsWith("bin/") || line.startsWith("sbin/") ){
	  bins << line;
	  if(line.contains("%%")){ variableBinFound=TRUE; }
        }
        if(line.endsWith(".png") || line.endsWith(".jpg") || line.endsWith(".svg")){
          //Images contained in the port, tag it as a GUI app
          if(portStruct[6]=="FALSE"){ qDebug() << " - Detected Graphical Application"; }
          portStruct[6] = "TRUE";
        }
      }
      pfile.close();
      if(!portStruct[4].isEmpty()){ portStruct[4].append("###"); }
      portStruct[4].append( bins.join("###") );
    }

    //Replace any common variables that might be in the bin list if any were found
    if( variableBinFound ){
      qDebug() << " - Attempting to fix binary names";
      QStringList tmpbins = portStruct[4].split("###");
      //Try to replace common variables (add more later)
      tmpbins.replaceInStrings("%%PORTNAME%%",portStruct[0]);
      tmpbins.replaceInStrings("$PORTNAME",portStruct[0]);
      tmpbins.replaceInStrings("${PORTNAME}",portStruct[0]);
      
      //If there are any bins that still have a variable in them, remove it from the list
      bins.clear();
      for(int i=0; i<tmpbins.length(); i++){
	if(!tmpbins[i].contains("%%") && !tmpbins[i].contains("$") ){
	  bins << tmpbins[i];
	}
      }
      //Save the binaries back into the struct
      portStruct[4] = bins.join("###");
    }



    //Assign the program a menu category based on the port category
    portStruct[5] = assignPortMenuCategory(portPath);
    qDebug() << " - Assigned menu category:"<<portStruct[5];

    //Remove any duplicate entries
    QStringList tmp = portStruct[3].split("###");
    tmp.removeDuplicates();
    portStruct[3] = tmp.join(" ");
    tmp = portStruct[4].split("###", QString::SkipEmptyParts);
    tmp.removeDuplicates();
    currentBins << tmp;     //Add the detected binaries to the currently available binaries
    currentBins.removeDuplicates();
    portStruct[4] = tmp.join(" ");

    //For debugging display all info
    qDebug() << " - Make Options:" << portStruct[3].split(" ");
    qDebug() << " - Binaries:" << portStruct[4].split(" ");
    qDebug() << " - Finished Detecting port information";
    savePortDir = portPath;
  
  return TRUE;
}

bool ModBuild::loadPBIconf(){
  //Clear the current structures
  progStruct.clear(); mkStruct.clear(); serverStruct.clear();
  //Fill the structures with empty strings (to initialize them)
  progStruct << "" << "" << "" << "" << "" << "";
  mkStruct << "" << "" << "" << "";
  serverStruct << "NO" << "00" << "00" << "NO";
  if(!QFile::exists(modulePath+"/pbi.conf")){ return FALSE; } //only reset the structures if the file does not exist
  //Read the designated pbi.conf and store the variables
  QFile file(modulePath+"/pbi.conf");
  if( !file.open(QIODevice::ReadOnly | QIODevice::Text)){
    qDebug() << "Error: Unable to load "+modulePath+"/pbi.conf";
    return FALSE;
  }
  QTextStream in(&file);
  while(!in.atEnd()){
    //see if the current line conains a desired variable and save it
    QString line = in.readLine();
    if(!line.startsWith("#") && !line.startsWith("export")){ //Ignore commented out lines
      //Pull apart the variable and the value
      QString var,val;
      var = line.section("=",0,0,QString::SectionSkipEmpty).trimmed();
      val = line.section("=",1,50).section(";",0,0,QString::SectionSkipEmpty).trimmed();
      //might need to add check for values that cross multiple lines later (mkStructure entries especially)
      if(var=="PBI_PROGNAME"){ progStruct[0] = val.remove("\""); }
      else if(var=="PBI_PROGWEB"){ progStruct[1] = val.remove("\""); }
      else if(var=="PBI_PROGAUTHOR"){ progStruct[2] = val.remove("\""); }
      else if(var=="PBI_PROGICON"){ progStruct[3] = val.remove("\""); }
      else if(var=="PBI_PROGVERSION"){ progStruct[4] = val.remove("\""); }
      else if(var=="PBI_PACKAGEDIR"){ progStruct[5] = val.remove("\""); }
      else if(var=="PBI_MAKEPORT"){ mkStruct[0] = val.remove("\""); }
      else if(var=="PBI_MKPORTBEFORE"){ 
	QStringList tmpL = val.split(" ");
	while( !val.simplified().endsWith("\"")){
	  val = in.readLine().section(";",0,0,QString::SectionSkipEmpty).simplified();
	  tmpL.append(val.split(" "));
	}
	val = tmpL.join(" ").remove("\""); //put the list back together
	mkStruct[1] = val; 
      }
      else if(var=="PBI_MKPORTAFTER"){ 
	QStringList tmpL = val.split(" ");
	while( !val.simplified().endsWith("\"")){
	  val = in.readLine().section(";",0,0,QString::SectionSkipEmpty).simplified();
	  tmpL.append(val.split(" "));
	}
	val = tmpL.join(" ").remove("\""); //put the list back together
	mkStruct[2] = val; 
      }
      else if(var=="PBI_MAKEOPTS"){ 
	QString tmp = val;
	while( !val.simplified().endsWith("\"")){
	  val = in.readLine().section(";",0,0,QString::SectionSkipEmpty).simplified();
	  tmp.append("\n"+val); //keep the same format for each line
	}
	mkStruct[3] = tmp.remove("\""); //Make sure to remove the quotes
      }
      else if(var=="PBI_REQUIRESROOT"){ serverStruct[0] = val.remove("\""); }
      else if(var=="PBI_BUILDKEY"){ serverStruct[1] = val.remove("\""); }
      else if(var=="PBI_AB_PRIORITY"){ serverStruct[2] = val.remove("\""); }
      else if(var=="PBI_AB_NOTMPFS"){ serverStruct[3] = val.remove("\""); }
      else{} //do nothing for extra lines
    }
  }
  file.close();
  return TRUE;
}

bool ModBuild::createModuleDir(){
//Creates the directory structure for a complete PBI module
  QString modName = modulePath.section("/",-1);
  //Check the base Directory
  QDir dir(modulePath);
  if(dir.exists()){ 
    bool validModule = dir.exists("resources") && dir.exists("xdg-desktop") \
                    && dir.exists("xdg-menu") && dir.exists("xdg-mime") && dir.exists("scripts");
    if( !validModule ){
      qDebug() << "Error: Directory exists, but is not a valid module";
      return FALSE;
    }else{
      qDebug() << "Valid Module directory already exists - overwriting it";
      bool success = emptyDirectory(modulePath); //Remove the module
      if(!success){
        qDebug() << " - Could not remove all old module contents. Please remove them by hand";
	return FALSE;
      }else{
        dir.rmdir(modulePath);
      }
    }
  }
  //Create new module directory
  dir.cd(modulePath);
  dir.cdUp();
  if(!dir.mkdir(modName)){
    qDebug() << "Error: Could not create module directory";
    // return empty path
  }else{
    //If main directory could be created, should be no problem with sub-directories
    dir.cd(modName);
    dir.mkdir("resources");
    dir.mkdir("xdg-desktop");
    dir.mkdir("xdg-menu");
    dir.mkdir("xdg-mime");
    dir.mkdir("scripts");
    qDebug() << "Module directory successfully created";
  }

  return TRUE;
}

bool ModBuild::writePBIconf(){
//Returns TRUE if successful

  //Check if the module directory exists
  QDir testDir(modulePath);
  if(testDir.exists() == FALSE){
    qDebug() << " - pbi.conf creation: Failure -- Module directory does not exist";
    return FALSE;
  }

  //Get the path to the module's pbi.conf
  QString fileName = modulePath+"/pbi.conf";

 //Create the contents of the file to write
  QStringList contents;
  QString exportLine = "export";
  contents << "#!/bin/sh";
  contents << "# PBI Build Configuration\n";
  // -- Program Info
  contents << "# -- Program Information --";
  contents << "PBI_PROGNAME=\""+progStruct[0]+"\"";
  contents << "PBI_PROGWEB=\""+progStruct[1]+"\"";
  contents << "PBI_PROGAUTHOR=\""+progStruct[2]+"\"";
  contents << "PBI_PROGICON=\""+getFilenameFromPath(progStruct[3])+"\"";
  exportLine.append(" PBI_PROGNAME PBI_PROGWEB PBI_PROGAUTHOR PBI_PROGICON");
  if(isLocalPBI){
    contents << "PBI_PROGVERSION=\""+progStruct[4]+"\"";
    contents << "PBI_PACKAGEDIR=\""+progStruct[5]+"\"";
    exportLine.append(" PBI_PROGVERSION PBI_PACKAGEDIR");
  }
  contents << ""; //Add a blank line between sections
  if(isPortPBI){
    // -- Port Info (only add this section if the PBI uses FreeBSD ports)
    contents << "# -- Port Information --";
    contents << "PBI_MAKEPORT=\""+mkStruct[0]+"\"";
    contents << "PBI_MKPORTBEFORE=\""+mkStruct[1]+"\"";
    contents << "PBI_MKPORTAFTER=\""+mkStruct[2]+"\"";
    contents << "PBI_MAKEOPTS=\""+mkStruct[3]+"\"\n"; //adds a blank line after the section
    exportLine.append(" PBI_MAKEPORT PBI_MKPORTBEFORE PBI_MKPORTAFTER PBI_MAKEOPTS");
  }
  // -- Server & Other Info
  contents << "# -- Require Root Permissions to Install PBI --";
  contents << "PBI_REQUIRESROOT=\""+checkYesNo(serverStruct[0].remove("\""))+"\"";
  contents << "# -- Auto-build Configuration Options --";
  contents << "PBI_BUILDKEY=\""+serverStruct[1].remove("\"")+"\"";
  contents << "PBI_AB_PRIORITY=\""+serverStruct[2].remove("\"")+"\"";
  contents << "PBI_AB_NOTMPFS=\""+checkYesNo(serverStruct[3].remove("\""))+"\"\n";
  exportLine.append(" PBI_REQUIRESROOT PBI_BUILDKEY PBI_AB_PRIORITY PBI_NOTMPFS");
  // --Export all the variables used
  contents << exportLine;

  //Create the File
  bool status = createFile(fileName, contents);
  return status;
}

bool ModBuild::loadMenu(QString ifile){
  //Reset the structure
  menuStruct.clear();
  menuStruct << ""<<""<<""<<""<<""<<""<<""<<"";  
  //[ name, genericname, exec, icon, nodisplay, terminal, categories, mimetype]
	
  if(ifile.isEmpty()){ return TRUE; } //Also allow this to be a "reset" function for the currently loaded file
  //Check if the file exists
  saveMenuFile = ifile; //save the current filename that is loaded
  QString filePath = modulePath +"/xdg-menu/"+ ifile;
  if(!QFile::exists(filePath)){ return TRUE; } //Get ready to write a currently non-existant file
  //Read the file and save the data into the structure
  bool status = FALSE;
  QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
      qDebug() << "Error: Could not open xdg-menu entry file:"<<filePath;
    }else{
      QTextStream in(&file);
      while(!in.atEnd()){
        QString line = in.readLine().simplified();
        if(line.startsWith("Name=")){
          menuStruct[0] = line.section("=",1,3,QString::SectionSkipEmpty);
        }else if(line.startsWith("GenericName=")){
          menuStruct[1] = line.section("=",1,3,QString::SectionSkipEmpty);
        }else if(line.startsWith("Exec=")){
          QString exec = line.section("=",1,3,QString::SectionSkipEmpty);
          menuStruct[2] = getFilenameFromPath(exec);
        }else if(line.startsWith("Icon=")){
          QString icon = line.section("=",1,3,QString::SectionSkipEmpty);
          menuStruct[3] = getFilenameFromPath(icon);
	}else if(line.startsWith("NoDisplay=")){
          menuStruct[4] = line.section("=",1,3,QString::SectionSkipEmpty);
	}else if(line.startsWith("Terminal=")){
          menuStruct[5] = line.section("=",1,3,QString::SectionSkipEmpty);
	}else if(line.startsWith("Categories=")){
          menuStruct[6] = line.section("=",1,3,QString::SectionSkipEmpty);
	}else if(line.startsWith("MimeType=")){
          menuStruct[7] = line.section("=",1,3,QString::SectionSkipEmpty);
        }else{
          //Do nothing - ignore this line
        }
      }
      //Check true/false values
      if(menuStruct[4].toLower() != "true"){ menuStruct[4]="false"; }
      if(menuStruct[5].toLower() != "true"){ menuStruct[5]="false"; }
      status = TRUE;
      file.close();
    }
    return status;
}

bool ModBuild::writeMenu(){
  //Get the path to the file
  QString fileName = modulePath + "/xdg-menu/" + saveMenuFile;
  //Create the XDG compliant menu entry
  QStringList contents;
  contents << "#!/usr/bin/env";
  contents << "[Desktop Entry]";
  contents << "Value=1.0";
  contents << "Type=Application";
  contents << "Name="+menuStruct[0];
  contents << "GenericName="+menuStruct[1];
  contents << "Exec=%%PBI_EXEDIR%%/"+getFilenameFromPath(menuStruct[2]);
  contents << "Path=%%PBI_APPDIR%%";
  contents << "Icon=%%PBI_APPDIR%%/"+getFilenameFromPath(menuStruct[3]);
  contents << "StartupNotify=true";
  if(menuStruct[4].toLower()=="yes" || menuStruct[4].toLower()=="true"){
    contents << "NoDisplay=true";
  }
  if(menuStruct[5].toLower()=="yes" || menuStruct[5].toLower()=="true"){
    contents << "Terminal=true";
  }
  contents << "Categories="+menuStruct[6];
  if(!menuStruct[7].isEmpty()){
    contents << "MimeType="+menuStruct[7];
  }
  //Save the contents to the file
  bool status;
  status = createFile(fileName, contents);
  return status;
}

bool ModBuild::removeMenu(){
  //Delete the currently selected menu file
  bool status = QFile::remove(modulePath+"/xdg-menu/"+saveMenuFile);
  return status;
}

bool ModBuild::loadDesktop(QString ifile){
  //Reset the structure
  desktopStruct.clear();
  desktopStruct << ""<<""<<""<<""<<""<<""<<"";
    //[ name, genericname, exec, icon, nodisplay, terminal, mimetypes]
	
  if(ifile.isEmpty()){ return TRUE; } //Also allow this to be a "reset" function for the currently loaded file
  //Check if the file exists
  saveDesktopFile = ifile; //save the current filename that is loaded
  QString filePath = modulePath +"/xdg-desktop/"+ ifile;
  if(!QFile::exists(filePath)){ return TRUE; } //Get ready to write a currently non-existant file
  //Read the file and save the data into the structure
  bool status = FALSE;
  QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
      qDebug() << "Error: Could not open xdg-desktop entry file:"<<filePath;
    }else{
      QTextStream in(&file);
      while(!in.atEnd()){
        QString line = in.readLine().simplified();
        if(line.startsWith("Name=")){
          desktopStruct[0] = line.section("=",1,3,QString::SectionSkipEmpty);
        }else if(line.startsWith("GenericName=")){
          desktopStruct[1] = line.section("=",1,3,QString::SectionSkipEmpty);
        }else if(line.startsWith("Exec=")){
          QString exec = line.section("=",1,3,QString::SectionSkipEmpty);
          desktopStruct[2] = getFilenameFromPath(exec);
        }else if(line.startsWith("Icon=")){
          QString icon = line.section("=",1,3,QString::SectionSkipEmpty);
          desktopStruct[3] = getFilenameFromPath(icon);
	}else if(line.startsWith("NoDisplay=")){
          desktopStruct[4] = line.section("=",1,3,QString::SectionSkipEmpty);
	}else if(line.startsWith("Terminal=")){
          desktopStruct[5] = line.section("=",1,3,QString::SectionSkipEmpty);
	}else if(line.startsWith("MimeType=")){
          desktopStruct[6] = line.section("=",1,3,QString::SectionSkipEmpty);
        }else{
          //Do nothing - ignore this line
        }
      }
      //Check for true/false values
      if(desktopStruct[4].toLower() != "true"){ desktopStruct[4]="false"; }
      if(desktopStruct[5].toLower() != "true"){ desktopStruct[5]="false"; }
      status = TRUE;
      file.close();
    }
    return status;
}

bool ModBuild::writeDesktop(){
  //Get the path to the file
  QString fileName = modulePath + "/xdg-desktop/" + saveDesktopFile;
  //Create the XDG compliant desktop entry
  QStringList contents;
  contents << "#!/usr/bin/env";
  contents << "[Desktop Entry]";
  contents << "Value=1.0";
  contents << "Type=Application";
  contents << "Name="+desktopStruct[0];
  contents << "GenericName="+desktopStruct[1];
  contents << "Exec=%%PBI_EXEDIR%%/"+getFilenameFromPath(desktopStruct[2]);
  contents << "Path=%%PBI_APPDIR%%";
  contents << "Icon=%%PBI_APPDIR%%/"+getFilenameFromPath(desktopStruct[3]);
  contents << "StartupNotify=true";
  if(desktopStruct[4].toLower()=="yes" || desktopStruct[4].toLower()=="true"){
    contents << "NoDisplay=true";
  }
  if(desktopStruct[5].toLower()=="yes" || desktopStruct[5].toLower()=="true"){
    contents << "Terminal=true";
  }
  if(!desktopStruct[6].isEmpty()){
    contents << "MimeType="+desktopStruct[6];
  }
  //Save the contents to the file
  bool status;
  status = createFile(fileName, contents);
  return status;
}

bool ModBuild::removeDesktop(){
  //Delete the currently selected menu file
  bool status = QFile::remove(modulePath+"/xdg-desktop/"+saveDesktopFile);
  return status;
}

bool ModBuild::loadMime(QString ifile){
  //Reset the structure
  mimeStruct.clear();
  mimeStruct << "" << "" << "";  //[ info, type, patterns]
  if(ifile.isEmpty()){ return TRUE; } //Also allow this to be a "reset" function for the currently loaded file
  //Check if the file exists
  saveMimeFile = ifile; //save the current filename that is loaded
  QString filePath = modulePath +"/xdg-mime/"+ ifile;
  if(!QFile::exists(filePath)){ return TRUE; } //Get ready to write a currently non-existant file
  //Read the file and save the data into the structure
  bool status = FALSE;
  QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
      qDebug() << "Error: Could not open xdg-mime entry file:"<<filePath;
    }else{
      QTextStream in(&file);
      QStringList patterns;
      while(!in.atEnd()){
        QString line = in.readLine().simplified();
        if(line.startsWith("<mime-info ")){
          mimeStruct[0] = line.section(" xmlns=",1,1).section("=",0,0).section(">",0,0).remove("\"").remove("\'");
        }else if(line.startsWith("<mime-type ")){
          mimeStruct[1] = line.section(" type=",1,1).section("=",0,0).section(">",0,0).remove("\"").remove("\'");
        }else if(line.startsWith("<glob ") && line.contains("pattern=")){
          patterns << line.section("pattern=",1,1).section("=",0,0).section("/>",0,0).remove("\"").remove("\'");
        }else{
          //Do nothing - ignore this line
        }
      }
      mimeStruct[2] = patterns.join(" "); //make a space-delimited list
      //Make sure the detected mime type is in the current list
      currentMimeTypes << mimeStruct[1];
      currentMimeTypes.removeDuplicates();
      //Return
      status = TRUE;
      file.close();
    }
    return status;
}

bool ModBuild::writeMime(){
  //Stupid check to make sure there is actually file patterns to be saved
  if(mimeStruct[2].isEmpty()){ return FALSE; }
  //Get the path to the file
  QString fileName = modulePath + "/xdg-mime/" + saveMimeFile;
  //Set a default mime-info if none given  
  if(mimeStruct[0].isEmpty()){
    mimeStruct[0] = "http://www.freedesktop.org/standards/shared-mime-info";
  }
  //Set a default mime type based upon the filename if none given 
  if(mimeStruct[1].isEmpty()){
    mimeStruct[1] = "application/x-"+saveMimeFile.section("-xdg.xml",0,0);
  }
  //Make a list of all the patterns to save
  QStringList patterns = mimeStruct[2].split(" ");
  //Create the XDG compliant mime type entry
  QStringList contents;
  contents << "<?xml version=\"1.0\"?>";
  contents << "<mime-info xmlns=\'"+mimeStruct[0]+"\'>";
  contents << "  <mime-type type=\""+mimeStruct[1]+"\">";
  for(int i=0; i<patterns.length(); i++){
    contents << "  <glob weight=\"100\" pattern=\""+patterns[i]+"\"/>";
  }
  contents << " </mime-type>";
  contents << "</mime-info>";
  //Save the contents to the file
  bool status;
  status = createFile(fileName, contents);
  if(status){ 
    //If successful, add the new mimetype to the current list
    currentMimeTypes <<mimeStruct[1]; 
    currentMimeTypes.removeDuplicates(); 
  }
  return status;
}

bool ModBuild::removeMime(){
  //Delete the currently selected mime file
  bool status = QFile::remove(modulePath+"/xdg-mime/"+saveMimeFile);
  //Remove the current Mime entry is successful
  if(status){
    int index = currentMimeTypes.indexOf(mimeStruct[1]);
    if(index >= 0){ currentMimeTypes.removeAt(index); }
  }
  return status;
}

bool ModBuild::loadExternalLinks(){
    //reset the structure
    linksStruct.clear();
    //Read the new file
    QFile file(modulePath+"/external-links");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
      qDebug() << "Error: Could not open external-links file:"<<modulePath+"/external-links";
      return FALSE;
    }
    QTextStream in(&file);
    QString original, linkto, types;
    while(!in.atEnd()){
      QString line = in.readLine();
      line.replace("\t"," ").simplified();
      if(!line.startsWith("#") && !line.isEmpty()){
        original = line.section(" ",0,0,QString::SectionSkipEmpty);
        linkto =  line.section(" ",1,1,QString::SectionSkipEmpty);
        types =  line.section(" ",2,2,QString::SectionSkipEmpty);
	//Add the link to the structure
	linksStruct << original+"::"+linkto+"::"+types;
      }else{
        //Do nothing - ignore this line
      }
    }
    file.close();
    return TRUE;
}

bool ModBuild::writeExternalLinks(){
  //File to write
  QString fileName = modulePath+"/external-links";
  //Get the contents to write to the file
  QStringList contents;
  //Standard Header for file
  contents << "# Files to be Sym-Linked into the default LOCALBASE";
  contents << "# One per-line, relative to %%PBI_APPDIR%% and LOCALBASE";
  contents << "# Defaults to keeping any existing files in LOCALBASE";
  contents << "# Use action \"binary\" for binaries that need wrapper functionality\n";
  contents << "# TARGET             LINK IN LOCALBASE         ACTION";
  //add the desired binaries
  for(int i=0; i<linksStruct.length(); i++){
    QStringList link = linksStruct[i].split("::");
    qDebug() << "save link:" << link;
    contents << link[0] +"     "+link[1]+"     "+link[2];
  }
  //Create the external-links file
  bool status = createFile(fileName, contents);
  return status;
}

void ModBuild::addExternalLink(QString file, QString linkto, QStringList types){
  QString chk = file+"::"+linkto+"::"; //try to match the file and link, not types
  for(int i=0; i<linksStruct.length(); i++){
    if( linksStruct[i].startsWith(chk) ){
      //This link already exists -> update the types and return
      linksStruct[i] = chk+types.join(",");
      return;
    }
  }	
  //This is a new link, add it to the end
  linksStruct << chk+types.join(",");
}

void ModBuild::removeExternalLink(QString file, QString linkto){
  QString chk = file+"::"+linkto+"::"; //try to match the file and link, not types
  for(int i=0; i<linksStruct.length(); i++){
    if( linksStruct[i].startsWith(chk) ){
      //This link already exists -> remove it
      linksStruct.removeAt(i);
      return;
    }
  }	
}

QStringList ModBuild::externalLinks(){
  //Return the currently available links
  QStringList output = linksStruct;
  output.replaceInStrings("::","\t");
  return output;
}

bool ModBuild::addResource(bool isNewWrapper, QString resourcePath){
  if(resourcePath.isEmpty()){
    qDebug() << "Warning: no resource selected";
    return TRUE;  // Do not flag this as an error in the function
  }
  //Determine the type of file that is being added
  bool isIcon=FALSE;
  if(resourcePath.endsWith(".png") && !isNewWrapper){ isIcon = TRUE; }
  //Create the full path to the location within the module  
  QString newResourcePath = modulePath + "/resources/";
  if(isNewWrapper){ 
    newResourcePath.append("bin/"); 
    //Make sure the "bin" directory exists
    QDir dir(newResourcePath);
    if(!dir.exists()){ dir.cdUp(); dir.mkdir("bin"); }  
  }else{
    //resourcePath *MUST* be an absolute path (start with "/")
    if( !resourcePath.startsWith("/") ){
      qDebug() << "Error: Path to desired resource is not an absolute path!";
      return FALSE;
    }
    //Check that the desired resource exists
    if( !QFile::exists(resourcePath) ){
      qDebug() << "Error: Desired resource does not exist: " << resourcePath;
      return FALSE;
    }
  }
  QString resource = getFilenameFromPath(resourcePath);
  newResourcePath.append(resource);
  //Check if the resource already exists in the module
  if( QFile::exists(newResourcePath) ){
    qDebug() << "Overwriting existing module resource:" << resource;
    QFile::remove(newResourcePath); 
  }
  //Copy the resource into the module
  bool status=FALSE;
  if(isNewWrapper){
    //Create a standard wrapper script in the module
    QStringList form;
    form << "#!/bin/sh";
    form << "#This is a sample wrapper script form generated by EasyPBI";
    form << "# REMINDER: Add this file to the external-links for the PBI as well!";
    form << "\n#Setup some internal script variables";
    form << "PROGDIR=`pwd | cut -d / -f 1-4`   #Base PBI directory (/usr/pbi/myapplication-<arch>)";
    form << "APPBINARY=bin/SAMPLE   #application binary";
    form << "\n#--- DO SOMETHING HERE ---\n";
    form << "#Now start the main application";
    form << "${PROGDIR}/${APPBINARY} $@";
    status = createFile(newResourcePath,form);
    //Make sure the new wrapper is executable
    QFile::setPermissions(newResourcePath, QFile::ReadOwner | QFile::ExeOwner | QFile::WriteOwner  | QFile::ReadGroup | QFile::ExeGroup  | QFile::WriteGroup | QFile::ReadOther | QFile::ExeOther);
  }else{
    //Copy the given file into the module
    if( !QFile::copy(resourcePath,newResourcePath) ){
      qDebug() << "Error copying resource into the module";
    }else{
      status=TRUE;
    }
  }
  //Add the item to the appropriate lists
  if(status && isIcon){ currentIcons << resource;  currentIcons.removeDuplicates();}
  else if(status && isNewWrapper){ currentBins << "bin/"+resource; currentBins.removeDuplicates();}
  
  return status;
}

bool ModBuild::removeResource(QString filePath){
  //filePath is relative to the module's resource directory
	
  //Determine if it is a wrapper script, icon, or other
  bool isWrapper=FALSE; bool isIcon=FALSE;
  if(filePath.startsWith("bin/")){ isWrapper=TRUE; }
  else if(filePath.endsWith(".png")){ isIcon=TRUE; }
  //Check if the file exists
  bool status=FALSE;
  QDir dir(modulePath+"/resources");
  if(dir.exists(filePath)){
    status = dir.remove(filePath);
  }else{
    qDebug() << "Error removing non-existant resource:"+filePath;
  }
  //Remove the item from the appropriate lists
  if(status && isIcon){ currentIcons.removeAll(filePath); }
  else if(status && isWrapper){ 
    currentBins.removeAll(filePath); 
    dir.cd("bin");
    if(dir.entryList(QDir::Files | QDir::NoDotAndDotDot).isEmpty() ){ 
      dir.cdUp();
      dir.rmdir("bin");
    }	    
  }
  
  return status;
}

bool ModBuild::createFile(QString fileName, QStringList contents){
//fileName = full path to file (I.E. /home/pcbsd/junk/junk.txt)
//contents = list of lines to be written (one line per entry in the list - no newline needed at the end of an entry)

  //Remove any existing file with that name/location
  if( QFile::exists(fileName) ){
    if( !QFile::remove(fileName) ){
      qDebug() << fileName+": Error -- Could not overwrite existing file";
      return FALSE;
    }
  }
  //Open the file with .tmp extension
  QFile file(fileName+".tmp");
  if( !file.open(QIODevice::WriteOnly | QIODevice::Text) ){
    qDebug() << fileName+".tmp: Failure -- Could not open file";
    return FALSE;
  }
  //Write the file
  QTextStream ofile(&file); //start the output stream
  for(int i=0; i<contents.length(); i++){
    ofile << contents[i];
    ofile << "\n";
  }
  //Close the File
  file.close();
  //Move the temporary file into its final location
  if( !file.rename(fileName) ){
    qDebug() << fileName+": Error: Could not rename "+fileName+".tmp as "+fileName;
    return FALSE;
  }
  //Return success
  QString extra = QDir::homePath() + "/EasyPBI/Modules/"; //remove this from the filename display
  qDebug() << "Created:" << fileName.remove(extra);
  return TRUE;
}

QStringList ModBuild::readFile(QString filePath){
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

QStringList ModBuild::filesAvailable(QString group){
  QStringList fileList;  
  //Check to make sure that a module has been selected first
  if(modulePath.isEmpty()){ return fileList; }
  QDir dir(modulePath);
  group = group.toLower();
  if(group=="resources"){
    dir.cd(group);
    //Get all the files in the main directory
    fileList = dir.entryList(QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden);
    //Get all the files in any sub-directories
    QStringList subdirs = dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
    for(int i=0; i<subdirs.length(); i++){
      dir.cd(subdirs[i]);
      QStringList dirL = dir.entryList(QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden);
      for(int j=0; j<dirL.length(); j++){
        fileList << subdirs[i]+"/"+dirL[j];
      }
      dir.cdUp();
    }
  }else if(group=="xdg-desktop"){
    dir.cd(group);
    //Get all the files in the main directory
    fileList = dir.entryList(QStringList("*.desktop") , QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden);
  }else if(group=="xdg-menu"){
    dir.cd(group);
    //Get all the files in the main directory
    fileList = dir.entryList(QStringList("*.desktop"), QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden);	  
  }else if(group=="xdg-mime"){
    dir.cd(group);
    //Get all the files in the main directory
    fileList = dir.entryList(QStringList("*.xml"), QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden);	  
  }else if(group=="scripts"){
    dir.cd(group);
    //Get all the files in the main directory
    fileList = dir.entryList(QStringList("*.sh"), QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden);	  
  }
  return fileList; //This will return nothing if an invalid group is given
}

QString ModBuild::readValue(QString variable){
//progStruct=[ name, website, author, icon, version, packageDir]
//mkStruct=[ makeport, portbefore, portafter, makeoptions]
//serverStruct=[ needroot, buildkey, priority, noTMPFS]
//menuStruct=[ name, genericname, exec, icon, nodisplay, terminal, categories, mimetype]
//desktopStruct=[ name, genericname, exec, icon, nodisplay, terminal, mimetype]
//mimeStruct=[ info, type, patterns] (patterns is a " "-delimited list)
//portStruct=[ name, author, website, options, binaries, menu-category, isGraphical ]	
  QString val;
  QString var = variable.toLower();
  if(var=="progname"){ val = progStruct[0] ;}
  else if(var=="progweb"){ val = progStruct[1]; }
  else if(var=="progauthor"){ val = progStruct[2]; }
  else if(var=="progicon"){ val = progStruct[3]; }
  else if(var=="progversion"){ val = progStruct[4]; }
  else if(var=="packagedir"){ val = progStruct[5]; }
  else if(var=="makeport"){ val = mkStruct[0]; }
  else if(var=="makeportbefore"){ val = mkStruct[1]; }
  else if(var=="makeportafter"){ val = mkStruct[2]; }
  else if(var=="makeoptions"){ val = mkStruct[3]; }
  else if(var=="requiresroot"){ val = serverStruct[0]; }
  else if(var=="buildkey"){ val = serverStruct[1]; }
  else if(var=="priority"){ val = serverStruct[2]; }
  else if(var=="notmpfs"){ val = serverStruct[3]; }
  else if(var=="menuname"){ val = menuStruct[0]; }
  else if(var=="menugenericname"){ val = menuStruct[1]; }
  else if(var=="menuexec"){ val = menuStruct[2]; }
  else if(var=="menuicon"){ val = menuStruct[3]; }
  else if(var=="menunodisplay"){ val = menuStruct[4] ;}
  else if(var=="menuterminal"){ val = menuStruct[5]; }
  else if(var=="menucategories"){ val = menuStruct[6]; }
  else if(var=="menumimetype"){ val = menuStruct[7]; }
  else if(var=="desktopname"){ val = desktopStruct[0]; }
  else if(var=="desktopgenericname"){ val = desktopStruct[1]; }
  else if(var=="desktopexec"){ val = desktopStruct[2]; }
  else if(var=="desktopicon"){ val = desktopStruct[3]; }
  else if(var=="desktopnodisplay"){ val = desktopStruct[4]; }
  else if(var=="desktopterminal"){ val = desktopStruct[5]; }
  else if(var=="desktopmimetype"){ val = desktopStruct[6]; }
  else if(var=="mimeinfo"){ val = mimeStruct[0]; }
  else if(var=="mimetype"){ val = mimeStruct[1]; }
  else if(var=="mimepatterns"){ val = mimeStruct[2]; }
  else if(var=="portname"){ val = portStruct[0]; }
  else if(var=="portauthor"){ val = portStruct[1]; }
  else if(var=="portwebsite"){ val = portStruct[2]; }
  else if(var=="portoptions"){ val = portStruct[3]; }
  else if(var=="portbinaries"){ val = portStruct[4]; }
  else if(var=="portmenucategory"){ val = portStruct[5]; }
  else if(var=="portisgraphical"){ val = portStruct[6]; }
  else{
    qDebug() << "Error: Invalid variable name to read";
  }
  return val;
}

bool ModBuild::writeValue(QString variable, QString value){
//progStruct=[ name, website, author, icon, version, packageDir]
//mkStruct=[ makeport, portbefore, portafter, makeoptions]
//serverStruct=[ needroot, buildkey, priority, noTMPFS]
//menuStruct=[ name, genericname, exec, icon, nodisplay, terminal, categories, mimetype]
//desktopStruct=[ name, genericname, exec, icon, nodisplay, terminal, mimetype]
//mimeStruct=[ info, type, patterns] (patterns is a " "-delimited list)
  QString var = variable.toLower();
  if(var=="progname"){ progStruct[0] = value; }
  else if(var=="progweb"){ progStruct[1] = value; }
  else if(var=="progauthor"){ progStruct[2] = value; }
  else if(var=="progicon"){ progStruct[3] = value; }
  else if(var=="progversion"){ progStruct[4] = value; }
  else if(var=="packagedir"){ progStruct[5] = value; }
  else if(var=="makeport"){ mkStruct[0] = value; }
  else if(var=="makeportbefore"){ mkStruct[1] = value; }
  else if(var=="makeportafter"){ mkStruct[2] = value; }
  else if(var=="makeoptions"){ mkStruct[3] = value; }
  else if(var=="requiresroot"){ serverStruct[0] = value; }
  else if(var=="buildkey"){ serverStruct[1] = value; }
  else if(var=="priority"){ serverStruct[2] = value; }
  else if(var=="notmpfs"){ serverStruct[3] = value; }
  else if(var=="menuname"){ menuStruct[0] = value; }
  else if(var=="menugenericname"){ menuStruct[1] = value; }
  else if(var=="menuexec"){ menuStruct[2] = value; }
  else if(var=="menuicon"){ menuStruct[3] = value; }
  else if(var=="menunodisplay"){ menuStruct[4] = value; }
  else if(var=="menuterminal"){ menuStruct[5] = value; }
  else if(var=="menucategories"){ menuStruct[6] = value; }
  else if(var=="menumimetype"){ menuStruct[7] = value; }
  else if(var=="desktopname"){ desktopStruct[0] = value; }
  else if(var=="desktopgenericname"){ desktopStruct[1] = value; }
  else if(var=="desktopexec"){ desktopStruct[2] = value; }
  else if(var=="desktopicon"){ desktopStruct[3] = value; }
  else if(var=="desktopnodisplay"){ desktopStruct[4] = value; }
  else if(var=="desktopterminal"){ desktopStruct[5] = value; }
  else if(var=="desktopmimetype"){ desktopStruct[6] = value; }
  else if(var=="mimeinfo"){ mimeStruct[0] = value; }
  else if(var=="mimetype"){ mimeStruct[1] = value; }
  else if(var=="mimepatterns"){ mimeStruct[2] = value; }
  else{
    qDebug() << "Error: Invalid variable name to read";
    return FALSE;
  }
  return TRUE;
}

void ModBuild::compressModule(){
  //Compress the module directory
  QString localDir = getFilenameFromPath(modulePath);
  QString cmd = "cd "+modulePath+"/..; tar czf "+localDir+".tar.gz "+localDir;
  qDebug() << "Compressing module:" << modulePath+".tar.gz";
  system( cmd.toUtf8() );
  return;
}

QString ModBuild::getFilenameFromPath(QString fullPath){
  if(fullPath.isEmpty()){return "";}
  return fullPath.section("/",-1);
}

int ModBuild::readMakeFile(QString makefile){
  bool variableBinFound=FALSE;
  QStringList bins, actualopts, listopts;

    QFile mkfile(makefile);
    if(!mkfile.open(QIODevice::ReadOnly) ){
      return -1;
    }
    QTextStream mkin(&mkfile);
    while( !mkin.atEnd() ){
      QString line = mkin.readLine().simplified();
      // ---- Program Name -----
      if( line.startsWith("PORTNAME") ){
        portStruct[0] = line.replace("\t"," ").section(" ",1,1,QString::SectionSkipEmpty);
	qDebug() << " - Found portname:" << portStruct[0];
      // ---- Port Maintainer -----
      }else if( line.startsWith("MAINTAINER") ){
        portStruct[1] = line.replace("\t"," ").section(" ",1,1,QString::SectionSkipEmpty);
	qDebug() << " - Found maintainer:"<< portStruct[1];
      // ---- OptionsNG Framework --------
      }else if( line.startsWith("OPTIONS_DEFINE") || line.startsWith("OPTIONS_SINGLE_") || line.startsWith("OPTIONS_MULTI_") ){
	listopts << line.replace("\t"," ").section("=",1,50,QString::SectionSkipEmpty).simplified().split(" ");	
	while( line.simplified().endsWith("\\")){
	  line = mkin.readLine().replace("\t"," ");
	  listopts << line.split(" ");
	}
      }else if( line.startsWith("OPTIONS_DEFAULT") ){
	QStringList defaults = line.replace("\t"," ").section("=",1,50,QString::SectionSkipEmpty).simplified().split(" ");	
	for(int i=0; i<listopts.length(); i++){
          if(defaults.contains(listopts[i].simplified()) ){ 
            actualopts << "WITHOUT_"+listopts[i].simplified();
          }else{
            actualopts << "WITH_"+listopts[i].simplified();
          }
        }
      // ---- Legacy Options Framework -------
      }else if( line.startsWith("OPTIONS") && !line.startsWith("OPTIONS_") ){
	listopts << line.replace("\t"," ").section(" ",1,1,QString::SectionSkipEmpty);	
	while( line.simplified().endsWith("\\")){
	  line = mkin.readLine().replace("\t"," ");
	  listopts << line.section(" ",0,0,QString::SectionSkipEmpty).simplified();
	}
      }else if( line.startsWith(".if defined(") || line.startsWith(".if !defined(") ){
        //Get the variable used (we need WITH_* or WITHOUT_*)
        QString var = line.section("(",1,1).section(")",0,0).simplified();
	//qDebug() << line << var << var.section("_",1,1);
	if( var.startsWith("WITH") && listopts.contains( var.section("_",1,1) ) ){
	  actualopts << var;
	}
      // ---- Program Website -----
      }else if( line.startsWith("WWW") ){
          portStruct[2] = line.section(" ",1,1,QString::SectionSkipEmpty);
	  qDebug() << " - Found website:"<< portStruct[2];
      // ---- Program Binaries/Files ----
      }else if( line.startsWith("PLIST_FILES") ){ //catch PLIST_FILES= and PLIST_FILES+=
	QStringList tmpL = line.section(" ",1,60).split(" ",QString::SkipEmptyParts);
	while( line.simplified().endsWith("\\") ){
	  line = mkin.readLine().replace("\t"," ");
	  tmpL.append( line.split(" ",QString::SkipEmptyParts) );
	}
	for(int i=0; i<tmpL.length(); i++){
	  if(tmpL[i].endsWith("\\")){ tmpL[i].chop(1); }
          tmpL[i].simplified();
	  if( tmpL[i].startsWith("bin/") || tmpL[i].startsWith("sbin/") ){
	    bins << tmpL[i];
	    if(tmpL[i].contains("$")){ variableBinFound=TRUE; }
	  }  
          if(tmpL[i].endsWith(".png") || line.endsWith(".jpg") || line.endsWith(".svg")){
            //Images contained in the port, tag it as a GUI app
            if(portStruct[6]=="FALSE"){ qDebug() << " - Detected Graphical Application"; }
            portStruct[6] = "TRUE";
          }
	}
      }else{
	//do nothing - skip this line
      }
    }
    //qDebug() << "lo:" << listopts << "\nao:" << actualopts;
    mkfile.close();

    if(!portStruct[3].isEmpty()){portStruct[3].append("###");}
    if(!portStruct[4].isEmpty()){portStruct[4].append("###");}
    portStruct[3].append(actualopts.join("###"));
    portStruct[4].append(bins.join("###"));

    if(variableBinFound){return 1;}
    else{return 0;}
}

QString ModBuild::assignPortMenuCategory(QString portDir){
  //Assign a menu category based upon the port category
  
  //Get the port category
  QString pc = portDir.section("ports/",1,1).section("/",0,0);
  QString mc;
  //qDebug() << portDir << pc;
  //Create the port -> menu category conversions
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

bool ModBuild::emptyDirectory(QString fullPath){
  if(fullPath.endsWith("/")){ fullPath.chop(1); }
  QDir rDir(fullPath);
  bool success = TRUE;
  //Remove files
  QStringList cleanFiles = rDir.entryList(QDir::Files | QDir::NoDotAndDotDot);
  for(int j=0; j<cleanFiles.length(); j++){ 
    bool done = rDir.remove(cleanFiles[j]); 
    if(!done) { 
      qDebug() << " - File could not be deleted:" << fullPath+"/"+cleanFiles[j]; 
      success=FALSE;
    }  
  }
  //Remove Directories
  cleanFiles = rDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
  for(int j=0; j<cleanFiles.length(); j++){ 
  bool isempty = emptyDirectory(fullPath+"/"+cleanFiles[j]); //This is recursive - be careful!!
  if(isempty){ rDir.rmdir(cleanFiles[j]); } //only try to remove it if it is empty
    else{ success=FALSE; }
  }
  //Return the result status
  return success;
}

QString ModBuild::checkYesNo(QString chk){
  QString ret = "NO";
  chk = chk.toLower();
  if( (chk=="true") || (chk=="yes") ){ ret = "YES"; }
  return ret;
}

bool ModBuild::writeSampleFreenasControl(){
    //convert the program name to the proper format
    QString progname = progStruct[0];
    progname.toLower().remove(" ").remove("\t").simplified();
    //Now create the sample file contents
    QStringList contents;
    contents << "#!/bin/python";
    contents << "import os";
    contents << "import platform";
    contents << "import re";
    contents << "import sys";
    contents << "import stat";
    contents << "import signal" + QString("\n"); //add an extra line
    contents << "from flup.server.fcgi import WSGIServer" + QString("\n"); //add extra line
    contents << "HERE = os.path.abspath(os.path.dirname(__file__))";
    contents << "sys.path.insert(0, os.path.join(HERE, \"lib/python2.7/site-packages\"))" +QString("\n"); //add extra line
    contents << "#Define useful variables for this script";
    contents << progname+"_fcgi_pidfile = \"/var/run/"+progname+"_fcgi_server.pid\"";
    contents << progname+"_pbi_path = \"/usr/pbi/"+progname+"-\" + platform.machine()";
    contents << progname+"_etc_path = os.path.join("+progname+"_pbi_path, \"etc\")";
    contents << progname+"_mnt_path = os.path.join("+progname+"_pbi_path, \"mnt\")";
    contents << progname+"_www_path = os.path.join("+progname+"_pbi_path, \"www\")";
    contents << progname+"_opts = os.path.join("+progname+"_etc_path, \"options\")";
    contents << progname+"_control = \"/usr/local/etc/rc.d/"+progname+"\"" + "\n"; //add extra line
    contents << "#Define Start function";
    contents << "def "+progname+"_fcgi_start(args):";
    contents << "  if len(args) < 2:";
    contents << "     return False" + QString("\n"); //add extra line
    contents << "  ip = args[0]";
    contents << "  port = long(args[1])" + QString("\n"); //add extra line
    contents << "  pid = os.fork()";
    contents << "  if pid < 0:";
    contents << "     return False";
    contents << "  if pid != 0:";
    contents << "     sys.exit(0)" +QString("\n"); //add extra line
    contents << "  os.setsid()";
    contents << "  os.environ['DJANGO_SETTINGS_MODULE'] = '"+progname+"UI.settings'";
    contents << "  import django.core.handlers.wsgi";
    contents << "  app = django.core.handlers.wsgi.WSGIHandler()"+QString("\n"); //add extra line
    contents << "  res = False";
    contents << "  with open("+progname+"_fcgi_pidfile, \"wb\") as fp:";
    contents << "     fp.write(str(os.getpid()))";
    contents << "     fp.close()"+QString("\n"); //add extra line
    contents << "     res = WSGIServer(app, bindAddress=(ip, port)).run()" + QString("\n"); //add extra line
    contents << "  return res" + QString("\n"); //add extra line
    contents << "#Define Stop function";
    contents << "def "+progname+"_fcgi_stop(args):";
    contents << "  res = False";
    contents << "  if os.access("+progname+"_fcgi_pidfile, os.F_OK):";
    contents << "     with open("+progname+"_fcgi_pidfile, \"r\") as fp:";
    contents << "        pid = long(fp.read())";
    contents << "        fp.close()" +QString("\n"); //add extra line
    contents << "        os.kill(pid, signal.SIGHUP)";
    contents << "        res = True"+QString("\n"); //add extra line
    contents << "  if os.access("+progname+"_fcgi_pidfile, os.F_OK):";
    contents << "     os.unlink("+progname+"_fcgi_pidfile)"+"\n";//add extra line
    contents << "  return res"+QString("\n");//add extra line
    contents << "#Define Status function";
    contents << "def "+progname+"_fcgi_status(args):";
    contents << "  res = False";
    contents << "  if os.access("+progname+"_fcgi_pidfile, os.F_OK):";
    contents << "     with open("+progname+"_fcgi_pidfile, \"r\") as fp:";
    contents << "        pid = long(fp.read())";
    contents << "        fp.close()";
    contents << "        res = True"+QString("\n");//add extra line
    contents << "  return res"+QString("\n");//add extra line
    contents << "#Define Configure function";
    contents << "def "+progname+"_fcgi_configure(args):";
    contents << "  return True"+QString("\n");//add extra line
    contents << "#Define Main function";	
    contents << "def main(argc, argv):";
    contents << "  if argc < 2:";
    contents << "     sys.exit(1)"+QString("\n");//add extra line
    contents << "  commands = {";
    contents << "     'start': "+progname+"_fcgi_start,";
    contents << "     'stop': "+progname+"_fcgi_stop,";
    contents << "     'status': "+progname+"_fcgi_status,";
    contents << "     'configure': "+progname+"_fcgi_configure";
    contents << "  }"+QString("\n");//add extra line	
    contents << "  if not commands.has_key(argv[0]):";
    contents << "     sys.exit(1)"+QString("\n");//add extra line	
    contents << "  if not commands[argv[0]](argv[1:]):";
    contents << "     sys.exit(1)"+QString("\n");//add extra line
    contents << "  sys.exit(0)"+QString("\n");
    contents << "if __name__ == '__main__':";
    contents << "  main(len(sys.argv), sys.argv[1:])";
    //now save the sample file
    QString filepath = this->path()+"/resources/control";
    bool status = createFile(filepath, contents);
    return status;
  
	
}

bool ModBuild::writeSampleFreenasTweakRC(){
    //convert the program name to the proper format
    QString progname = progStruct[0];
    progname.toLower().remove(" ").remove("\t").simplified();
    //Now create the sample file contents
    QStringList contents;
    contents << "#!/bin/sh\n"; //add an extra line after this
    contents << "#Setup the temporary program variables";
    contents << "program_name="+progname;
    contents << "program_path=/usr/pbi/${program_name}-$(uname -m)"+QString("\n"); //add an extra line
    contents << "#Perform the modification to /etc/rc.conf";
    contents << "tmpfile=$(mktemp /tmp/.XXXXXX)";
    contents << "grep -v '${program_name}_' /etc/rc.conf > ${tmpfile}";
    contents << "cat ${program_path}/etc/rc.conf >> ${tmpfile}";
    contents << "mv ${tmpfile} /etc/rc.conf";
    //now save the sample file
    QString filepath = this->path()+"/resources/tweak-rcconf";
    bool status = createFile(filepath, contents);
    return status;
}
