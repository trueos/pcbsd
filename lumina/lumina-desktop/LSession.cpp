//===========================================
//  Lumina-DE source code
//  Copyright (c) 2012-2014, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#include "LSession.h"

//Private/global variables (for static function access)
WId LuminaSessionTrayID;
AppMenu *appmenu;
SettingsMenu *settingsmenu;
QTranslator *currTranslator;

LSession::LSession(int &argc, char ** argv) : QApplication(argc, argv){
  this->setApplicationName("Lumina Desktop Environment");
  this->setApplicationVersion("0.4.2");
  this->setOrganizationName("LuminaDesktopEnvironment");
  this->setQuitOnLastWindowClosed(false); //since the LDesktop's are not necessarily "window"s
  //Enabled a few of the simple effects by default
  this->setEffectEnabled( Qt::UI_AnimateMenu, true);
  this->setEffectEnabled( Qt::UI_AnimateCombo, true);
  this->setEffectEnabled( Qt::UI_AnimateTooltip, true);
  LuminaSessionTrayID = 0;
}

LSession::~LSession(){
  delete settingsmenu;
  delete appmenu;
}

void LSession::setupSession(){
  qDebug() << "Initializing Session";
  loadStyleSheet();
  //Setup the QSettings default paths
  QSettings::setPath(QSettings::NativeFormat, QSettings::UserScope, QDir::homePath()+"/.lumina");
  
  checkUserFiles(); //copy/initialize any files necessary
	
  //Initialize the global menus
  appmenu = new AppMenu();
  settingsmenu = new SettingsMenu();
}

bool LSession::LoadLocale(QString langCode){
    QTranslator translator;
    if ( ! QFile::exists(SYSTEM::installDir()+"i18n/lumina-desktop_" + langCode + ".qm" ) )  langCode.truncate(langCode.indexOf("_"));
    bool ok = translator.load( QString("lumina-desktop_") + langCode, SYSTEM::installDir()+"i18n/" );
    if(ok){
      //Remove any old translator
      if(currTranslator != 0){ this->removeTranslator(currTranslator); }
      //Insert the new translator
      currTranslator = &translator;
      this->installTranslator( currTranslator );
      qDebug() << "Loaded Locale:" << langCode;
    }else{
      qDebug() << "Invalid Locale:" << langCode;
    }
    emit LocaleChanged();
    return ok;
}

void LSession::launchStartupApps(){
  QString startfile = QDir::homePath()+"/.lumina/startapps";
  if(!QFile::exists(startfile)){ startfile = "/usr/local/share/Lumina-DE/startapps"; }
  if(!QFile::exists(startfile)){ return; }
  qDebug() << "Launching startup applications";
  QFile file(startfile);
  if( file.open(QIODevice::ReadOnly | QIODevice::Text) ){
    QTextStream in(&file);
    while(!in.atEnd()){
      QString entry = in.readLine();
      if(entry.startsWith("#") || entry.isEmpty()){ continue; }
      //Might put other sanity checks here
      QProcess::startDetached(entry);
    }
    file.close();
  }
}

void LSession::checkUserFiles(){
  //Check for the desktop settings file
  QString dset = QDir::homePath()+"/.lumina/LuminaDE/desktopsettings.conf";
  bool firstrun = false;
  if(!QFile::exists(dset)){
    firstrun = true;
    if(QFile::exists(SYSTEM::installDir()+"desktopsettings.conf")){
      if( QFile::copy(SYSTEM::installDir()+"desktopsettings.conf", dset) ){
        QFile::setPermissions(dset, QFile::ReadUser | QFile::WriteUser | QFile::ReadOwner | QFile::WriteOwner);
      }
    }
  }
  //Check for the default applications file for lumina-open
  dset = QDir::homePath()+"/.lumina/LuminaDE/lumina-open.conf";
  if(!QFile::exists(dset)){
    firstrun = true;
    if(QFile::exists(SYSTEM::installDir()+"defaultapps.conf")){
      if( QFile::copy(SYSTEM::installDir()+"defaultapps.conf", dset) ){
        QFile::setPermissions(dset, QFile::ReadUser | QFile::WriteUser | QFile::ReadOwner | QFile::WriteOwner);
      }
    }
  }
  if(firstrun){ qDebug() << "First time using Lumina!!"; }
}

void LSession::loadStyleSheet(){
  QString ss = QDir::homePath()+"/.lumina/stylesheet.qss";
  if(!QFile::exists(ss)){ ss = SYSTEM::installDir()+"stylesheet.qss"; }
  if(!QFile::exists(ss)){ return; } //no default stylesheet on the system
  //Now read/apply the custom stylesheet
  QFile file(ss);
  if( file.open(QIODevice::ReadOnly | QIODevice::Text) ){
    QTextStream in(&file);
    QString sheet = in.readAll();
    file.close();
    //Now fix/apply the sheet
      sheet.replace("\n"," "); //make sure there are no newlines
    this->setStyleSheet(sheet);
  }
  
}

bool LSession::x11EventFilter(XEvent *event){
  //Detect X Event types and send the appropriate signal(s)
   switch(event->type){
  // -------------------------
    case ClientMessage:
    	//Only check if the client is the system tray, otherwise ignore
    	if(event->xany.window == LuminaSessionTrayID){
    	  //qDebug() << "SysTray: ClientMessage";
    	  parseClientMessageEvent(&(event->xclient));
    	}
    	break;
    case SelectionClear:
    	if(event->xany.window == LuminaSessionTrayID){
    	  //qDebug() << "SysTray: Selection Clear";
    	  this->CloseSystemTray();
    	}
    	break;
    case PropertyNotify:
	//qDebug() << "Property Event:";
	if(LX11::ValidWindowEvent(event->xproperty.atom)){
	  //qDebug() << " - Emit Window Event";
	  emit WindowListEvent();
	}
	break;
  }
  // -----------------------
  //Now continue on with the event handling (don't change it)
  return false;
}

//=================
//   SYSTEM TRAY
//=================
bool LSession::StartupSystemTray(){
  if(LuminaSessionTrayID != 0){ return false; } //already have one running
  LuminaSessionTrayID = LX11::startSystemTray();
  return (LuminaSessionTrayID != 0);
}

bool LSession::CloseSystemTray(){
  LX11::closeSystemTray(LuminaSessionTrayID);
  LuminaSessionTrayID = 0;
  return true; //no additional checks for success at the moment
}

void LSession::parseClientMessageEvent(XClientMessageEvent *event){
  unsigned long opcode = event->data.l[1];
  switch(opcode){
    case SYSTEM_TRAY_REQUEST_DOCK:
        emit NewSystemTrayApp(event->data.l[2]); //Window ID
        break;
    case SYSTEM_TRAY_BEGIN_MESSAGE:
        //Let the window manager handle the pop-up messages for now
        break;    	    
    case SYSTEM_TRAY_CANCEL_MESSAGE:
        //Let the window manager handle the pop-up messages for now
        break;
    /*default:
    	//Unknown system tray operation code
    	opcode=1; //junk operation for compiling purposes*/
  }
}

//===============
//  SYSTEM ACCESS
//===============
AppMenu* LSession::applicationMenu(){
  return appmenu;
}

SettingsMenu* LSession::settingsMenu(){
  return settingsmenu;
}

void LSession::systemWindow(){
  SystemWindow win;
  win.exec();
}