/* PCDM Login Manager:
*  Written by Ken Moore (ken@pcbsd.org) 2012/2013
*  Copyright(c) 2013 by the PC-BSD Project
*  Available under the 3-clause BSD license
*/

#include "pcdm-config.h"
#include "pcdm-backend.h"

//Setup variables that save the current configuration data
QStringList confStruct;


//******** PRIVATE FUNCTIONS *************
void Config::loadDefaults(){
  confStruct.clear();
  confStruct << "/usr/local/share/PCDM/pcdm.theme";	// [0] Theme File Path
  confStruct << "/usr/local/share/xsessions";		// [1] *.desktop files directory
  confStruct << "/usr/local/share/pixmaps";		// [2] Images directory for *.desktop icons
  confStruct << "FALSE";				// [3] Auto-login enabled
  confStruct << "no-username";				// [4] Auto-login username
  confStruct << "no-desktop";				// [5] Auto-login desktop environment
  confStruct << "no-password";				// [6] Auto-login password
  confStruct << "/usr/local/share/PCDM/images/default-splash.png"; // [7] Splash Screen File
  confStruct << "TRUE";					// [8] Password view button enabled
  return;
}

void Config::readConfigFile(QString filePath){
  QFile conf(filePath);
  bool ok = conf.open(QIODevice::ReadOnly | QIODevice::Text);
  if(!ok){ 
    Backend::log("Could not open configuration file: "+filePath+"\nLoading default configuration");
    if(filePath!=":samples/pcdm.conf"){ readConfigFile(":samples/pcdm.conf"); }
    return;
  }
  QTextStream in(&conf);
  while( !in.atEnd() ){
    QString line = in.readLine().simplified();
    //Skip comment lines
    if(!line.startsWith("#")){
      line = line.section("#",0,0).simplified(); //cut any comments off the end of the line
      QString var = line.section("=",0,0).simplified(); // get the variable
      QString val = line.section("=",1,1).simplified(); // get the value
      
      //Parse the Configuration File
      if(var=="THEME_FILE"){ confStruct[0] = val; }
      else if(var=="DE_STARTUP_DIR"){ confStruct[1] = val; }
      else if(var=="DE_STARTUP_IMAGE_DIR"){ confStruct[2] = val; }
      else if(var=="ENABLE_AUTO_LOGIN"){ confStruct[3] = val; }
      else if(var=="AUTO_LOGIN_USER"){ confStruct[4] = val; }
      else if(var=="AUTO_LOGIN_DESKTOP"){ confStruct[5] = val; }
      else if(var=="AUTO_LOGIN_PASSWORD"){ confStruct[6] = val; }
      else if(var=="SPLASHSCREEN_FILE"){ confStruct[7] = val; }
      else if(var=="ENABLE_VIEW_PASSWORD_BUTTON"){ confStruct[8] = val; }
      else{}
      
    }
  }
  conf.close();
}

//******** PUBLIC FUNCTIONS **************
void Config::loadConfigFile(QString filePath){
  loadDefaults();
  if(!QFile::exists(filePath)){
    Backend::log("Configuration file does not exist: "+filePath+"\nLoading default configuration");
    filePath=":samples/pcdm.conf";
  }
  readConfigFile(filePath);
}

QString Config::themeFile(){
  return confStruct[0];
}

QString Config::xSessionsDir(){
  return confStruct[1];
}

QString Config::xSessionsImageDir(){
  return confStruct[2];
}

bool Config::useAutoLogin(){
  //qDebug() << "confStruct[3]:" << confStruct[3];
  if(confStruct[3].toLower()=="true"){ return TRUE; }
  else{ return FALSE; }	
}

QString Config::autoLoginUsername(){
  return confStruct[4];	
}

QString Config::autoLoginDesktop(){
  if(confStruct.startsWith("/")){  return confStruct[5]; } //already an absolute path
  else{ return (confStruct[1]+"/"+confStruct[5]); } //prepend the xsessions directory path
}

QString Config::autoLoginPassword(){
  return confStruct[6];	
}

QString Config::splashscreen(){
  return confStruct[7];	
}

bool Config::allowPasswordView(){
  if(confStruct[8].toLower()=="true"){ return TRUE; }
  else{ return FALSE; }	
}

