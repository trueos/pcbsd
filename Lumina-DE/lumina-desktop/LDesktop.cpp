//===========================================
//  Lumina-DE source code
//  Copyright (c) 2012, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#include "LDesktop.h"

LDesktop::LDesktop(int deskNum) : QObject(){
	
  DPREFIX = "desktop-"+QString::number(deskNum)+"/";
  desktopnumber = deskNum;
  desktop = new QDesktopWidget();
  defaultdesktop = (deskNum== desktop->primaryScreen());
  xoffset = 0;
  for(int i=0; i<desktopnumber; i++){
    xoffset += desktop->screenGeometry(i).width();
  }
  deskMenu = new QMenu(0);
  appmenu = new AppMenu(0);
  //Setup the internal variables
  settings = new QSettings(QSettings::UserScope, "LuminaDE","desktopsettings", this);
  bgtimer = new QTimer(this);
    bgtimer->setSingleShot(true);
 
  bgWindow = new QWidget(0);
	bgWindow->setObjectName("bgWindow");
	bgWindow->setContextMenuPolicy(Qt::CustomContextMenu);
	LX11::SetAsDesktop(bgWindow->winId());
	bgWindow->setGeometry(xoffset,0,desktop->screenGeometry().width(), desktop->screenGeometry().height());
	connect(bgWindow, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ShowMenu()) );
  
  //Start the update processes
  QTimer::singleShot(1,this, SLOT(UpdateMenu()) );
  QTimer::singleShot(1,this, SLOT(UpdateBackground()) );
  QTimer::singleShot(1,this, SLOT(UpdateDesktop()) );
  QTimer::singleShot(1,this, SLOT(UpdatePanels()) );

}

LDesktop::~LDesktop(){
  delete deskMenu;
  delete appmenu;
  delete bgWindow;
}

// =====================
//     PRIVATE SLOTS 
// =====================
void LDesktop::UpdateMenu(){
  qDebug() << " - Update Menu";
  deskMenu->clear();
  //Add in the system applications menu
  deskMenu->addAction(LXDG::findIcon("utilities-terminal",""), tr("Terminal"), this, SLOT(SystemTerminal()) );
  deskMenu->addMenu( LSession::applicationMenu() );
  deskMenu->addSeparator();
  deskMenu->addMenu( LSession::settingsMenu() );
  //Now add the system quit options
  deskMenu->addSeparator();
  deskMenu->addAction(LXDG::findIcon("system-log-out",""), tr("Log Out"), this, SLOT(SystemLogout()) );
}

void LDesktop::UpdateDesktop(){
  qDebug() << " - Update Desktop";
  QStringList plugins = settings->value(DPREFIX+"pluginlist", QStringList()).toStringList();
  if(defaultdesktop && plugins.isEmpty()){
    plugins << "desktopview";
  }
  for(int i=0; i<plugins.length(); i++){
    //See if this plugin is already there
    LDPlugin *plug = 0;
    for(int p=0; p<PLUGINS.length(); p++){
      if(PLUGINS[p]->type()==plugins[i]){
	plug = PLUGINS[p];
	break;
      }
    }
    if(plug==0){
      //New Plugin
      qDebug() << " -- New Plugin:" << plugins[i];
      plug = NewDP::createPlugin(plugins[i], bgWindow);
      if(plug != 0){ 
        PLUGINS << plug;
      }
    }
    //Update the plugin geometry
    if(plug!=0){
      QString geom = settings->value(DPREFIX+plugins[i]+"/geometry", "").toString();
      if(geom.isEmpty()){
	if(plugins.length()==1 && plugins[i]=="desktopview"){ 
	  geom = "0,0,"+QString::number(desktop->availableGeometry().width())+","+QString::number(desktop->availableGeometry().height());
	}else{
	  geom = QString::number(i*200)+",0,200,"+QString::number(desktop->availableGeometry().height());
	}
      }
      plug->setGeometry(geom.section(",",0,0).toInt(),geom.section(",",1,1).toInt(), geom.section(",",2,2).toInt(), geom.section(",",3,3).toInt() );
      plug->show(); //make sure it is visible
    }
  }
  
}

void LDesktop::UpdatePanels(){
  qDebug() << " - Update Panels";
  int panels = settings->value(DPREFIX+"panels", 0).toInt();
  //if(panels==0 && defaultdesktop){ panels=1; } //need at least 1 panel on the primary desktop
  for(int i=0; i<panels; i++){
    if(i<PANELS.length()){
      qDebug() << " -- Update panel "<< i;
      //panel already exists - just update it
      QTimer::singleShot(1, PANELS[i], SLOT(UpdatePanel()) );
    }else{
      qDebug() << " -- Create panel "<< i;
      //New panel
      PANELS << new LPanel(settings, desktopnumber, i);
    }
  }
  
}

void LDesktop::UpdateBackground(){
  //Get the current Background
  QString cbg = settings->value(DPREFIX+"background/current", "").toString();
  //Get the list of background(s) to show
  QStringList bgL = settings->value(DPREFIX+"background/filelist", "").toStringList();
    //Remove any invalid files
    for(int i=0; i<bgL.length(); i++){ 
      if( !QFile::exists(bgL[i]) || bgL[i].isEmpty()){ bgL.removeAt(i); i--; } 
    }
  //Determine which background to use next
  int index = bgL.indexOf(cbg);
  if( (index < 0) || (index >= bgL.length()-1) ){ index = 0; } //use the first file
  else{ index++; } //use the next file in the list
  QString bgFile;
  if( bgL.isEmpty() && cbg.isEmpty()){ bgFile = "default"; }
  else if( bgL.isEmpty() && QFile::exists(cbg) ){ bgFile = cbg; }
  else if( bgL.isEmpty() ){ bgFile = "default"; }
  else{ bgFile = bgL[index]; }
  //Save this file as the current background
  settings->setValue(DPREFIX+"background/current", bgFile);
  if( (bgFile.toLower()=="default")){ bgFile = "/usr/local/share/Lumina-DE/desktop-background.jpg"; }
  //Now set this file as the current background
  //QString display = QString( getenv("DISPLAY") );
  //display = display.section(".",0,0)+"."+desktopnumber; //only the current screen
  QString style = "QWidget#bgWindow{ border-image:url(%1) stretch;}";
  style = style.arg(bgFile);
  bgWindow->setStyleSheet(style);
  bgWindow->show();
  //QString cmd = "xv +24 -maxp -rmode 5 -quit \""+bgFile+"\"";
  //QProcess::startDetached(cmd);
  //Now reset the timer for the next change (if appropriate)
  if(bgL.length() > 1){
    //get the length of the timer (in minutes)
    int min = settings->value(DPREFIX+"background/minutesToChange",5).toInt();
    //reset the internal timer
    if(bgtimer->isActive()){ bgtimer->stop(); }
    bgtimer->start(min*60000); //convert from minutes to milliseconds
  }
}
