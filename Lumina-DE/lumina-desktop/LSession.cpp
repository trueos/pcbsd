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

LSession::LSession(int &argc, char ** argv) : QApplication(argc, argv){
  this->setApplicationName("Lumina Desktop Environment");
  this->setApplicationVersion("0.1");
  this->setOrganizationName("LuminaDesktopEnvironment");
  this->setQuitOnLastWindowClosed(false); //since the LDesktop's are not necessarily "window"s
  LuminaSessionTrayID = 0;
  appmenu = new AppMenu();
  settingsmenu = new SettingsMenu();
}

LSession::~LSession(){
  delete settingsmenu;
  delete appmenu;
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
    case MapNotify:      //window mapped (visible)
    case UnmapNotify:    //window unmapped (invisible)
    case VisibilityNotify: //window subsection visibility changed
    case ReparentNotify: //window re-parented
    case DestroyNotify:  //window destroyed
    case CreateNotify:   //window created
    default:
    	emit WindowListEvent();
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