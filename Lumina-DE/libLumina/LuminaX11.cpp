//===========================================
//  Lumina-DE source code
//  Copyright (c) 2014, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#include "LuminaX11.h"

//=====   WindowList() ========
QList<WId> LX11::WindowList(){
  QList<WId> output;
  //Method 1 - EWMH
    //Openbox lists the windows split up between various atoms - combine all of them
    //output << LX11::ActiveWindow();
    output << LX11::GetClientList();
    //output << LX11::GetClientStackingList();
  
  
  //Method 2 - ICCC
  /*output = LX11::findChildren(QX11Info::appRootWindow(),4);
  //Now go through them and filter out all the invalid windows
  //Display *disp = QX11Info::display();

  for(int i=0; i<output.length(); i++){
    //Now filter
    bool good = false;
    //Check if this is a redirect window */
    /*WId leader = LX11::leaderWindow(output[i]);
    if(leader !=0 && leader != output[i]){
      qDebug() << "Leader Found:" << output[i] << " -> " << leader;
      output[i] = leader; //replace this window with the leader
      //now go back one to check this window
      i--;
      continue;
    }*/
    /*
    //Now check for a usable window (either visible/invisible)
    if( LX11::isNormalWindow(output[i],true) && !LX11::WindowClass(output[i]).isEmpty() ){
      good=true;
    }
    
    //Remove item from list if not good
    if(!good){
      output.removeAt(i);
      i--; //make sure we don't miss a window since the list index/length got decreased 
    }else{
      //Check if this is a redirect window
      WId leader = LX11::leaderWindow(output[i]);
      if(leader !=0 && leader != output[i]){
        qDebug() << "Leader Found:" << output[i] << " -> " << leader;
        output[i] = leader; //replace this window with the leader
      }
    }
  }
  */
  
  //Validate windows
  for(int i=0; i<output.length(); i++){
    bool remove=false;
	QString name = LX11::WindowClass(output[i]);
    if(output[i] == 0){ remove=true; }
    else if( name.startsWith("Lumina") || name.isEmpty() ){ remove=true; }
    if(remove){
      output.removeAt(i);
      i--;
    }
  }
  //qDebug() << output;
  //Return the list
  return output;
}

// ===== GetClientList() =====
QList<WId> LX11::GetClientList(){
  QList<WId> output;
  Atom a = XInternAtom(QX11Info::display(), "_NET_CLIENT_LIST", true);
  Atom realType;
  int format;
  unsigned long num, bytes;
  unsigned char *data = 0;
  int status = XGetWindowProperty(QX11Info::display(), QX11Info::appRootWindow(), a, 0L, (~0L),
  	     false, AnyPropertyType, &realType, &format, &num, &bytes, &data);
  if( (status >= Success) && (num > 0) ){
    Window* array =  (Window*) data;
    for(unsigned int i=0; i<num; i++){
       output << array[i];
    }
    XFree(data);
  }
  return output;
}

// ===== GetClientStackingList() =====
QList<WId> LX11::GetClientStackingList(){
  QList<WId> output;
  Atom a = XInternAtom(QX11Info::display(), "_NET_CLIENT_LIST_STACKING", true);
  Atom realType;
  int format;
  unsigned long num, bytes;
  unsigned char *data = 0;
  int status = XGetWindowProperty(QX11Info::display(), QX11Info::appRootWindow(), a, 0L, (~0L),
  	     false, AnyPropertyType, &realType, &format, &num, &bytes, &data);
  if( (status >= Success) && (num > 0) ){
    Window* array =  (Window*) data;
    for(unsigned int i=0; i<num; i++){
       output << array[i];
    }
    XFree(data);
  }
  return output;
}

// ===== findChildren() =====
QList<WId> LX11::findChildren(Window parent, int levels){
  Window rootR, parentR;
  Window *childrenR;
  unsigned int num;
  int stat = XQueryTree(QX11Info::display(), parent, &rootR, &parentR, &childrenR, &num);
  QList<WId> output;
  if(stat != 0 && num > 0){
    for(int i=0; i<int(num); i++){
      output << childrenR[i];
      if(levels > 0){
       output << LX11::findChildren(childrenR[i], levels-1); //call this recursively
      }
    }
    XFree(childrenR);
  }	
  return output;
}

// ===== ActiveWindow() =====
WId LX11::ActiveWindow(){
  Display *disp = QX11Info::display();
  Atom SA = XInternAtom(disp, "_NET_ACTIVE_WINDOW", false);
  Atom type;
  int format;
  unsigned long num, bytes;
  unsigned char *data = 0;
  int status = XGetWindowProperty( disp, QX11Info::appRootWindow() , SA, 0, ~(0L), false, AnyPropertyType,
  	  			&type, &format, &num, &bytes, &data);
  WId window=0;
  if(status >= Success && data){
    Window *array = (Window*) data;
    window = array[0];
    XFree(data);
  }
  return window;  		
}

// ===== CloseWindow() =====
void LX11::CloseWindow(WId win){
XClientMessageEvent msg;
    msg.type = ClientMessage;
    msg.window = win;
    msg.message_type = XInternAtom(QX11Info::display(),"_NET_CLOSE_WINDOW",true);
    msg.format = 32;
    msg.data.l[0] = CurrentTime;
    msg.data.l[1] = 1; //Normal Window state
    msg.data.l[2] = 0;
    msg.data.l[3] = 0;
    msg.data.l[4] = 0;
  XSendEvent(QX11Info::display(), QX11Info::appRootWindow(), False, StructureNotifyMask, (XEvent*)&msg);		
}

// ===== IconifyWindow() =====
void LX11::IconifyWindow(WId win){
  XIconifyWindow(QX11Info::display(), win, QX11Info::appScreen());	
}

// ===== RestoreWindow() =====
void LX11::RestoreWindow(WId win){
  Display *disp = QX11Info::display();
    XMapRaised(disp, win); //make it visible again and raise it to the top
}

// ===== ReservePanelLocation() =====
void LX11::ReservePanelLocation(WId win, int xstart, int ystart, int width, int height){
  unsigned long strut[12];
  for(int i=0; i<12; i++){ strut[i] = 0; } //initialize it to all zeros
  if(ystart==0){
    //top
    strut[2] = height; //top width
    strut[8] = xstart; //top x start
    strut[9] = xstart+width; //top x end
  }else{
    //bottom
    strut[3] = height; //bottom width
    strut[10] = xstart; //bottom x start
    strut[11] = xstart+width; //bottom x end
  }
  Display *disp = QX11Info::display();
  Atom WTYPE = XInternAtom(disp, "_NET_WM_STRUT_PARTIAL", false);
  XChangeProperty( disp, win, WTYPE, XA_CARDINAL, 32, PropModeReplace, (unsigned char *) strut, 12);
  
  //Also set the _NET_WM_STRUT property, just in case the WM does not use the newer type
  WTYPE = XInternAtom(disp, "_NET_WM_STRUT", false);
  XChangeProperty( disp, win, WTYPE, XA_CARDINAL, 32, PropModeReplace, (unsigned char *) strut, 4);
}

// ===== SetAsSticky() =====
void LX11::SetAsSticky(WId win){
  //make this window "stick" to all virtual desktops
  Display *disp = QX11Info::display();
  XEvent ev;
	ev.xclient.type = ClientMessage;
	ev.xclient.message_type = XInternAtom(disp, "_NET_WM_STATE", false);
	ev.xclient.window = win;
	ev.xclient.format = 32;
	ev.xclient.data.l[0] = 1; //Add/set this property
	ev.xclient.data.l[1] = XInternAtom(disp, "_NET_WM_STATE_STICKY", false);
	ev.xclient.data.l[2] = 0;
	ev.xclient.data.l[3] = 1; //message is a normal window message
	
  XSendEvent(disp, QX11Info::appRootWindow(), false, SubstructureRedirectMask | SubstructureNotifyMask, &ev);
}

// ===== SetAsPanel() =====
void LX11::SetAsPanel(WId win){
  //Set this window as the "Dock" type (for showing on top of everthing else)
  long data[1];
  Display *disp = QX11Info::display();
  Atom WTYPE = XInternAtom(disp, "_NET_WM_WINDOW_TYPE", false);
  Atom DOCK = XInternAtom(disp, "_NET_WM_WINDOW_TYPE_DOCK",false);
  data[0] = DOCK;
  XChangeProperty( disp, win, WTYPE, XA_ATOM, 32, PropModeReplace, (unsigned char *) &data, 1L);
}

// ===== SetAsDesktop() =====
void LX11::SetAsDesktop(WId win){
  //Set this window as the "Dock" type (for showing on top of everthing else)
  long data[1];
  Display *disp = QX11Info::display();
  Atom WTYPE = XInternAtom(disp, "_NET_WM_WINDOW_TYPE", false);
  Atom DOCK = XInternAtom(disp, "_NET_WM_WINDOW_TYPE_DESKTOP",false);
  data[0] = DOCK;
  XChangeProperty( disp, win, WTYPE, XA_ATOM, 32, PropModeReplace, (unsigned char *) &data, 1L);
}

// ===== WindowClass() =====
QString LX11::WindowClass(WId win){
  XClassHint hint;
  QString clss;
  if(0 != XGetClassHint(QX11Info::display(), win, &hint) ){
    clss = QString(hint.res_class); //class is often capitalized properly, while name is not
    XFree(hint.res_name);
    XFree(hint.res_class);
  }
  return clss;
}

// ===== WindowName() =====
QString LX11::WindowName(WId win){
  QString nm = LX11::getNetWMProp(win, "_NET_WM_NAME");
  if(nm.isEmpty()){ 
    char *txt;
    if( XFetchName(QX11Info::display(), win, &txt) != 0){
      nm = QString(txt);
    }
    XFree(txt);
  }
  return nm;
}

// ===== WindowVisibleName() =====
QString LX11::WindowVisibleName(WId win){
  return LX11::getNetWMProp(win, "_NET_WM_VISIBLE_NAME");	
}

// ===== WindowIconName() =====
QString LX11::WindowIconName(WId win){
  return LX11::getNetWMProp(win, "_NET_WM_ICON_NAME");	
}

// ===== WindowVisibleIconName() =====
QString LX11::WindowVisibleIconName(WId win){
  return LX11::getNetWMProp(win, "_NET_WM_VISIBLE_ICON_NAME");	
}

// ===== WindowPixmap() =====
QPixmap LX11::WindowPixmap(WId win){
  //Use the _NET_WM_ICON value instead of the WMHints pixmaps
	// - the pixmaps are very unstable and erratic
  QPixmap pix;
  Display *disp = QX11Info::display();
  Atom type;
  Atom SA = XInternAtom(disp, "_NET_WM_ICON", false);
  int format;
  unsigned long num, bytes;
  unsigned char *data = 0;
  int status = XGetWindowProperty( disp, win, SA, XA_CARDINAL, ~(0L), false, AnyPropertyType,
  	  			&type, &format, &num, &bytes, &data);
  if(status != 0 && data != 0){
    //Now convert it into a Qt image
    // - first 2 elements are width and height
    // - data in rows from left to right and top to bottom
    QImage image(data[0], data[1], QImage::Format_ARGB32); //initial setup
	for(int i=0; i<image.byteCount()/4; ++i){
	  ((uint*)image.bits())[i] = data[i+2]; //remember the first 2 element offset
	}
    pix = QPixmap::fromImage(image); //Now convert it into the pixmap
    XFree(data);
  }
  
  if(pix.isNull()){
    //Fall back on the old method
    XWMHints *hints = XGetWMHints(QX11Info::display(), win);
    if(hints != 0){
      if( hints->flags & IconWindowHint ){
        pix = QPixmap::grabWindow( hints->icon_window );
      }
      else if( hints->flags & IconPixmapHint ){
        pix = QPixmap::fromX11Pixmap( hints->icon_pixmap ) ;
      }
      if( hints->flags & IconMaskHint ){
        pix.setMask( QPixmap::fromX11Pixmap(hints->icon_mask).createHeuristicMask() );
      }
      XFree(hints);
    }
  }
  return pix;
}

// ===== GetWindowState() =====
LX11::WINDOWSTATE LX11::GetWindowState(WId win, bool forDisplay){
  //forDisplay lets the function know whether it needs to follow the TaskBar/Pager ignore rules
	
  //OPENBOX DOES NOT SUPPORT THE _NET_WM_STATE VALUES (4/7/14)
  Display *disp = QX11Info::display(); /*
  Atom SA = XInternAtom(disp, "_NET_WM_STATE", false);
  Atom ATTENTION = XInternAtom(disp, "_NET_WM_STATE_DEMANDS_ATTENTION", false);
  Atom SKIPP = XInternAtom(disp, "_NET_WM_STATE_SKIP_PAGER", false);
  Atom HIDDEN = XInternAtom(disp, "_NET_WM_STATE_HIDDEN", false);
  Atom SKIPT = XInternAtom(disp, "_NET_WM_STATE_SKIP_TASKBAR", false);
  Atom MODAL = XInternAtom(disp, "_NET_WM_STATE_MODAL", false); */
  //Atom type;
  //int format;
  //unsigned long num, bytes;
  //unsigned char *data = 0;
  /*
  int status = XGetWindowProperty( disp, win, SA, 0, ~(0L), false, AnyPropertyType,
  	  			&type, &format, &num, &bytes, &data);
	
  LX11::WINDOWSTATE state = LX11::VISIBLE;
  if(status >= Success && data){
    Atom *array = (Atom*) data;
    for(unsigned int i=0; i<num; i++){
      if(forDisplay && (array[i] == SKIPP || array[i]==SKIPT || array[i]==MODAL) ){
      	state = LX11::IGNORE;
      	break;
      }else if(array[i]==HIDDEN){
	qDebug() << "Hidden Window:" << win;
	state = LX11::INVISIBLE;
      }else if(array[i]==ATTENTION){
	qDebug() << "Attention Window: " << win;
	state = LX11::ATTENTION;
      }
    }
    XFree(data);
  }
  */
  LX11::WINDOWSTATE state = LX11::VISIBLE;
  if(state==LX11::VISIBLE){
    //Use another method for detecting whether the window is actually mapped (more reliable)
    /*Atom STATE = XInternAtom(disp, "WM_STATE", false);
    //re-use the other variables
    data = 0;
    if( 0 != XGetWindowProperty( disp, win, STATE, 0, ~(0L), false, AnyPropertyType, &type, &format, &num, &bytes, &data) ){
      qint32 *array = (qint32 *) data;
      if(array[0]==NormalState){ state = LX11::VISIBLE; }
      else if(array[0]==IconicState){ state = LX11::INVISIBLE; }
      else{
        qDebug() << "Unknown State:" << win;
	state = LX11::IGNORE;
      }
    }
    */
    XWindowAttributes attr;
    if( 0 != XGetWindowAttributes(disp, win, &attr) ){
      if(attr.map_state==IsUnmapped || attr.map_state==IsUnviewable){
	state = LX11::INVISIBLE;
      }
    }
  }
  //If visible, check whether it is the active window
  if(state == LX11::VISIBLE){
    if(win == LX11::ActiveWindow()){
      state = LX11::ACTIVE;
    }	    
  }
  return state;  	
}

WId LX11::leaderWindow(WId win){
  //Get the client leader for this window if it has one
  Display *disp = QX11Info::display();
  Atom SA = XInternAtom(disp, "WM_CLIENT_LEADER", false);
  Atom type;
  int format;
  unsigned long num, bytes;
  unsigned char *data = 0;
  WId leader = 0;
  int status = XGetWindowProperty( disp, win, SA, 0, ~(0L), false, AnyPropertyType,
  	  			&type, &format, &num, &bytes, &data);
  if(status >= Success && data){
    Window *array = reinterpret_cast<Window*> (data);
    if(array!=0){
      leader = array[0];
    }
    XFree(data);
  }
  return leader;
}

// ===== isNormalWindow() =====
bool LX11::isNormalWindow(WId win, bool includeDialogs){
  //Check to see if it is a "normal" window (as opposed to tooltips, popups, menus, etc)
  Display *disp = QX11Info::display();
  Atom SA = XInternAtom(disp, "_NET_WM_WINDOW_TYPE", false);
  Atom NORMAL = XInternAtom(disp, "_NET_WM_WINDOW_TYPE_NORMAL", false);
  Atom DIALOG = XInternAtom(disp, "_NET_WM_WINDOW_TYPE_DIALOG", false);
  Atom type;
  int format;
  unsigned long num, bytes;
  unsigned char *data = 0;
  int status = XGetWindowProperty( disp, win, SA, 0, ~(0L), false, AnyPropertyType,
  	  			&type, &format, &num, &bytes, &data);
	
  bool isNormal = true; //assume normal is true if unlisted (the standard use)
  if(status >= Success && data){
    for(unsigned int i=0; i<num; i++){
      if( data[i] == NORMAL ){ isNormal = true; break; }
      else if(data[i]==DIALOG && includeDialogs){ isNormal=true; break; }
      else{ isNormal = false; } //don't break here, might be explicitly listed next
    }
    XFree(data);
  }
  return isNormal;
}

// ===== startSystemTray() =====
WId LX11::startSystemTray(){
  qDebug() << "Starting System Tray";
  //Setup the freedesktop standards compliance
  Display *disp = QX11Info::display();
  Window root = QX11Info::appRootWindow();
  
  //Get the appropriate atom for this screen
  QString str = QString("_NET_SYSTEM_TRAY_S%1").arg(DefaultScreen(disp));
  qDebug() << "Default Screen Atom Name:" << str;
  Atom _NET_SYSTEM_TRAY_S = XInternAtom(disp,str.toAscii(),false);
  //Make sure that there is no other system tray running
  if(XGetSelectionOwner(disp, _NET_SYSTEM_TRAY_S) != None){
    qWarning() << "An alternate system tray is currently in use";
    return 0;
  }
  //Create a simple window to register as the tray (not visible)
  Window LuminaSessionTrayID = XCreateSimpleWindow(disp, root,-1,-1,1,1,0,0,0);
  //register this widget as the system tray
  XSetSelectionOwner(disp, _NET_SYSTEM_TRAY_S, LuminaSessionTrayID, CurrentTime);
  //Make sure that it was registered properly
  if(XGetSelectionOwner(disp, _NET_SYSTEM_TRAY_S) != LuminaSessionTrayID){
    qWarning() << "Could not register the system tray";
    XDestroyWindow(disp, LuminaSessionTrayID);
    return 0;
  }
  //Now register the orientation of the system tray
  //int horz = _NET_SYSTEM_TRAY_ORIENTATION_HORZ;
  //XChangeProperty(disp, trayID, XInternAtom(disp,"_NET_SYSTEM_TRAY_ORIENTATION",true),
  	  	//XA_CARDINAL, 32, PropModeReplace, (unsigned char*) &horz, 1);
  //Now get the visual ID for the system tray
  XVisualInfo *XVI = new XVisualInfo;
    XVI->screen = QX11Info::appScreen();
    XVI->depth = 32;
    XVI->c_class = TrueColor;
  int num;
  XVI = XGetVisualInfo(disp, VisualScreenMask | VisualDepthMask | VisualClassMask , XVI, &num);
  VisualID vis = 0;
  if(XVI != 0){
    XRenderPictFormat *fmt;
    for(int i=0; i<num; i++){
      fmt = XRenderFindVisualFormat(disp, XVI[i].visual);
      if( (fmt !=0) && (fmt->type == PictTypeDirect) && (fmt->direct.alphaMask!=0) ){
        vis = XVI[i].visualid;
        break;
      }
    }
  }
  XFree(XVI); //done with this - clean it up
  //Now register the visual ID
  if(vis!=0){
    XChangeProperty(disp, LuminaSessionTrayID, XInternAtom(disp,"_NET_SYSTEM_TRAY_VISUAL",true),
  	  	XA_VISUALID, 32, PropModeReplace, (unsigned char*) &vis, 1);	  
  }
  //Finally, send out an X event letting others know that the system tray is up and running
  XClientMessageEvent msg;
    msg.type = ClientMessage;
    msg.window = root;
    msg.message_type = XInternAtom(disp,"MANAGER",true);
    msg.format = 32;
    msg.data.l[0] = CurrentTime;
    msg.data.l[1] = _NET_SYSTEM_TRAY_S;
    msg.data.l[2] = LuminaSessionTrayID;
    msg.data.l[3] = 0;
    msg.data.l[4] = 0;
  XSendEvent(disp, root, False, StructureNotifyMask, (XEvent*)&msg);
  
  //Success
  return LuminaSessionTrayID;
}

// ===== closeSystemTray() =====
void LX11::closeSystemTray(WId trayID){
  XDestroyWindow(QX11Info::display(), trayID);
}

// ===== getNetWMProp() =====
QString LX11::getNetWMProp(WId win, QString prop){
  Display *disp = QX11Info::display();
  Atom NA = XInternAtom(disp, prop.toUtf8(), false);
  Atom utf = XInternAtom(disp, "UTF8_STRING", false);
  Atom type;
  int format;
  unsigned long num, bytes;
  unsigned char *data = 0;
  int status = XGetWindowProperty( disp, win, NA, 0, 65536, false, utf,
  	  			&type, &format, &num, &bytes, &data);
  QString property;
  if(status >= Success && data){
    property = QString::fromUtf8( (char *) data);
    XFree(data);
  }
  return property;
}
