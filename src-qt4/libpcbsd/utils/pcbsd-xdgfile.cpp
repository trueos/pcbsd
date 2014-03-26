#include "pcbsd-xdgfile.h"

XDGFile::XDGFile(){
  //Setup the default values for the internal variables
  hidden = false;
  terminal = false;
  nodisplay = false;
}

XDGFile::~XDGFile(){
}

void XDGFile::LoadDesktopFile(QString filePath){
  if(!filePath.endsWith(".desktop") || !QFile::exists(filePath) ){ return; } //invalid file or path
  filepath = filePath; //valid file - save this value
  QStringList contents = quickRead(); //Now read the file
  //Get the locale code
  QString loc = QString( getenv("LANG") ).section(".",0,0); // <language>_<country>
  QString sloc = loc.section("_",0,0); //short localization code (language only)
  //qDebug() << "Localization Code:" << loc << sloc;
  //Parse the file contents and save the values appropriately
  bool desktopsection = false;
  for(int i=0; i<contents.length(); i++){
    if(contents[i].startsWith("[")){
      if(contents[i]=="[Desktop Entry]"){ desktopsection=true; }
      else{ desktopsection=false; }
    }else if(!desktopsection){ continue; } //skip this section
    if( !contents[i].contains("=") ){ continue; } 
    QString var = contents[i].section("=",0,0);
    QString val = contents[i].section("=",1,50);
    bool isLocalized = ( var.endsWith("["+loc+"]") || var.endsWith("["+sloc+"]") );
    if(!isLocalized && var.contains("[")){ continue; } //invalid localization
    var = var.section("[",0,0); //Chop the localization stuff off the end of the variable
    if(var=="Name"){
	if(isLocalized){ lname = val; }
	else{ name = val; }
    }else if(var=="GenericName"){
	if(isLocalized){ lgname = val; }
	else{ gname = val; }	    
    }else if(var=="Comment"){
	if(isLocalized){ lcomment = val; }
	else{ comment = val; }	    
    }else if(var=="NoDisplay"){
	nodisplay = (val.toLower()=="yes" || val.toLower()=="true");
    }else if(var=="Hidden"){
	hidden = (val.toLower()=="yes" || val.toLower()=="true");    
    }else if(var=="Exec"){
	exec = val;
    }else if(var=="TryExec"){
	tryexec = val;
    }else if(var=="Icon"){
	icon = val;
    }else if(var=="Categories"){
	cats = val;
    }else if(var=="Path"){
	runpath = val;
    }

  } //end parsing loop
  
}

QStringList XDGFile::quickRead(){
  //Read the contents of the current file
  QStringList out;
  QFile F(filepath);
  if( !F.open(QIODevice::ReadOnly | QIODevice::Text) ){ return out; } //unable to open file
  QTextStream in(&F);
  while( !in.atEnd() ){
    out << in.readLine();
  }
  F.close();
  return out;
}