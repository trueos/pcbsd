//===========================================
//  Lumina-DE source code
//  Copyright (c) 2014, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#include "AppMenu.h"

AppMenu::AppMenu(QWidget* parent) : QMenu(parent){
  appstorelink = "/usr/local/share/applications/softmanager.desktop"; //Default application "store" to display (AppCafe in PC-BSD)
  APPS.clear();
  watcher = new QFileSystemWatcher(this);
    connect(watcher, SIGNAL(directoryChanged(QString)), this, SLOT(watcherUpdate()) );
  QTimer::singleShot(200, this, SLOT(start()) ); //Now start filling the menu
  this->setTitle(tr("Applications"));
}

AppMenu::~AppMenu(){

}

//===========
//  PRIVATE
//===========
void AppMenu::updateAppList(){
  this->clear();
  APPS.clear();
  APPS = LXDG::sortDesktopCats( LXDG::systemDesktopFiles() );
  //Now fill the menu
  bool ok; //for checking inputs
    //--Look for the app store
    XDGDesktop store = LXDG::loadDesktopFile(appstorelink, ok);
    if(ok){ 
      this->addAction( LXDG::findIcon(store.icon, ""), tr("Get Applications"), this, SLOT(launchStore()) );
      this->addSeparator(); 
    }
    //--Now create the sub-menus
    QStringList cats = APPS.keys();
    cats.sort(); //make sure they are alphabetical
    for(int i=0; i<cats.length(); i++){
      //QString cat, icon;
      //if(cats[i] == ""){ name = tr(""); icon = ""; }
      QMenu *menu = new QMenu(cats[i], this);
      connect(menu, SIGNAL(triggered(QAction*)), this, SLOT(launchApp(QAction*)) );
      QList<XDGDesktop> appL = APPS.value(cats[i]);
      for( int a=0; a<appL.length(); a++){
        QAction *act = new QAction(LXDG::findIcon(appL[a].icon, ""), appL[a].name, this);
        act->setToolTip(appL[a].comment);
        act->setWhatsThis(appL[a].filePath);
        menu->addAction(act);
      }
      this->addMenu(menu);
    }
}

//=================
//  PRIVATE SLOTS
//=================
void AppMenu::start(){
  //Setup the watcher
  watcher->addPaths(LXDG::systemApplicationDirs());
  //Now fill the menu the first time
  updateAppList();
}

void AppMenu::watcherUpdate(){
  updateAppList(); //Update the menu listings
}

void AppMenu::launchStore(){
  QProcess::startDetached("lumina-open \""+appstorelink+"\"");
}

void AppMenu::launchApp(QAction *act){
  QString appFile = act->whatsThis();
  QProcess::startDetached("lumina-open \""+appFile+"\"");
}
