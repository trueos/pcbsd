//===========================================
//  Lumina-DE source code
//  Copyright (c) 2012, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#include "LPanel.h"

LPanel::LPanel(QSettings *file, int scr, int num) : QWidget(){
  //Take care of inputs
  qDebug() << " - Creating Panel:" << scr << num;
  settings = file;
  screennum = scr;
  screen = new QDesktopWidget();
  PPREFIX = "panel"+QString::number(screennum)+"."+QString::number(num)+"/";
  if(settings->value("defaultpanel",QString::number(screen->primaryScreen())+".0").toString()==QString::number(screennum)+"."+QString::number(num) ){ defaultpanel=true;}
  else{defaultpanel=false; }
  //Setup the panel
  qDebug() << " -- Setup Panel";
  this->setContentsMargins(0,0,0,0);
  this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  this->setWindowFlags( Qt::Tool | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint );
  this->setFocusPolicy(Qt::NoFocus);
  this->setWindowTitle("");
  this->setAttribute(Qt::WA_X11NetWmWindowTypeDock);
  this->setAttribute(Qt::WA_AlwaysShowToolTips);
  this->setObjectName("LuminaPanelWidget");
  //LX11::SetAsPanel(this->winId()); //set proper type of window for a panel since Qt can't do it
  LX11::SetAsSticky(this->winId());
  layout = new QHBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->setAlignment(Qt::AlignLeft);
    layout->setSpacing(1);
    //layout->setSizeConstraint(QLayout::SetFixedSize);
  this->setLayout(layout);
  QTimer::singleShot(1,this, SLOT(UpdatePanel()) ); //start this in a new thread
  connect(screen, SIGNAL(resized(int)), this, SLOT(UpdatePanel()) ); //in case the screen resolution changes
}

LPanel::~LPanel(){
	
}

//===========
// PUBLIC SLOTS
//===========
void LPanel::UpdatePanel(){
  //Create/Update the panel as designated in the Settings file
  settings->sync(); //make sure to catch external settings changes
  //First set the geometry of the panel and send the EWMH message to reserve that space
  qDebug() << "Update Panel";
  QString loc = settings->value(PPREFIX+"location","").toString();
  if(loc.isEmpty() && defaultpanel){ loc="top"; }
  int ht = settings->value(PPREFIX+"height", 22).toInt();
  int xoffset=0;
  for(int i=0; i<screennum; i++){
    xoffset = xoffset + screen->screenGeometry(i).width();
  }
  qDebug() << " - set Geometry";
  int xwid = screen->screenGeometry(screennum).width();
  int xhi = screen->screenGeometry(screennum).height();
  if(loc=="top"){
    QSize sz = QSize(xwid, ht);
    this->setMinimumSize(sz);
    this->setMaximumSize(sz);
    this->setGeometry(xoffset,0,xwid, ht );
    LX11::ReservePanelLocation(this->winId(), xoffset, 0, this->width(), ht);
  }else{
    QSize sz = QSize(xwid, ht);
    this->setMinimumSize(sz);
    this->setMaximumSize(sz);
    this->setGeometry(xoffset,screen->screenGeometry(screennum).height()-ht,xwid, ht );
    LX11::ReservePanelLocation(this->winId(), xoffset, xhi-ht, this->width(), ht);
  }
  //Now update the appearance of the toolbar
  QString color = settings->value(PPREFIX+"color", "qlineargradient(spread:pad, x1:0.291182, y1:0, x2:0.693, y2:1, stop:0 rgb(255, 253, 250), stop:1 rgb(210, 210, 210))").toString();
  QString style = "QWidget#LuminaPanelWidget{ background: %1; }";
  style = style.arg(color);
  this->setStyleSheet(style);
  //Then go through the plugins and create them as necessary
  QStringList plugins = settings->value(PPREFIX+"pluginlist", QStringList()).toStringList();
  if(defaultpanel && plugins.isEmpty()){
    plugins << "userbutton" << "desktopbar" << "desktopswitcher" << "taskmanager" << "spacer" << "systemtray" << "clock";
    if(SYSTEM::hasBattery()){ plugins << "battery"; }
  }else if(defaultpanel && !plugins.contains("userbutton") ){
    plugins.prepend("userbutton"); //make sure we have this button since that lets the user logout
  }
  qDebug() << " - Initialize Plugins: " << plugins;
  for(int i=0; i<plugins.length(); i++){
    //See if this plugin is already there or in a different spot
    bool found = false;
    for(int p=0; p<PLUGINS.length(); p++){
      if(PLUGINS[p]->type()==plugins[i]){
        found = true; //already exists
	if(p!=i){ //wrong place in the panel
	  layout->takeAt(p); //remove the item from the current location
	  layout->insertWidget(i, PLUGINS[p]); //add the item into the correct location
	  PLUGINS.move(p,i); //move the identifier in the list to match
	}
	break;
      }
    }
    if(!found){
      //New Plugin
      qDebug() << " -- New Plugin:" << plugins[i];
      LPPlugin *plug = NewPP::createPlugin(plugins[i]);
      if(plug != 0){ 
        PLUGINS.insert(i, plug);
        layout->insertWidget(i, PLUGINS[i]);
      }else{
        //invalid plugin type
	plugins.removeAt(i); //remove this invalid plugin from the list
	i--; //make sure we don't miss the next item with the re-order
      }
    }
  }
  //Now remove any extra plugins from the end
  for(int i=plugins.length(); i<PLUGINS.length(); i++){
    qDebug() << " -- Remove Plugin: " << i;
    layout->takeAt(i); //remove from the layout
    delete PLUGINS.takeAt(i); //delete the actual widget
  }
  this->move(0,0); //this will trigger the move event and ensire that it is put in the proper spot
  this->show(); //make sure the panel is visible now
}

void LPanel::UpdateLocale(){
  //The panel itself has no text to translate, just forward the signal to all the plugins
  for(int i=0; i<PLUGINS.length(); i++){
    QTimer::singleShot(1,PLUGINS[i], SLOT(LocaleChange()));
  }
}

void LPanel::UpdateTheme(){
  //The panel itself has no theme-based icons, just forward the signal to all the plugins
  for(int i=0; i<PLUGINS.length(); i++){
    QTimer::singleShot(1,PLUGINS[i], SLOT(ThemeChange()));
  }	
}


//===========
// PRIVATE SLOTS
//===========

