//===========================================
//  Lumina-DE source code
//  Copyright (c) 2012, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#include "LDeskBar.h"

LDeskBarPlugin::LDeskBarPlugin(QWidget *parent) : LPPlugin(parent, "desktopbar"){
  //Find the path to the desktop folder
  if(QFile::exists(QDir::homePath()+"/Desktop")){ desktopPath = QDir::homePath()+"/Desktop"; }
  else if(QFile::exists(QDir::homePath()+"/desktop")){ desktopPath = QDir::homePath()+"/desktop"; }
  else{ desktopPath=""; }
  //Setup the filter lists for the different types of files
  audioFilter <<"*.ogg"<<"*.mp3"<<"*.wav"<<"*.aif"<<"*.iff"<<"*.m3u"<<"*.m4a"<<"*.mid"<<"*.mpa"<<"*.ra"<<"*.wma";
  videoFilter <<"*.3g2"<<"*.3gp"<<"*.asf"<<"*.asx"<<"*.avi"<<"*.flv"<<"*.m4v"<<"*.mov"<<"*.mp4"<<"*.mpg"<<"*.rm"<<"*.srt"<<"*.swf"<<"*.vob"<<"*.wmv";
  pictureFilter <<"*.bmp"<<"*.dds"<<"*.gif"<<"*.jpg"<<"*.png"<<"*.psd"<<"*.thm"<<"*.tif"<<"*.tiff"<<"*.ai"<<"*.eps"<<"*.ps"<<"*.svg"<<"*.ico";
  //initialize the desktop bar items
  initializeDesktop();
  //setup the directory watcher
  watcher = new QFileSystemWatcher(this);
  if(!desktopPath.isEmpty()){ 
    watcher->addPath(desktopPath); 
  }
  connect(watcher, SIGNAL(directoryChanged(QString)), this, SLOT(desktopChanged()) );
  QTimer::singleShot(1,this, SLOT(desktopChanged()) ); //make sure to load it the first time
 
}

LDeskBarPlugin::~LDeskBarPlugin(){
  if(!desktopPath.isEmpty()){
    watcher->removePath(desktopPath);
    disconnect(watcher);
  }
  delete watcher;
  
}

// =======================
//   PRIVATE FUNCTIONS
// =======================
void LDeskBarPlugin::initializeDesktop(){
  /*layout = new QHBoxLayout();
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(2);
    layout->setAlignment(Qt::AlignCenter);
  this->setLayout(layout);*/
  //Applications on the desktop
  appB = new LTBWidget(this);
    //connect(appB, SIGNAL(clicked()), appB, SLOT(showMenu()) );
    //connect(appB, SIGNAL(longClicked()), appB, SLOT(showMenu()) ) ;
    appB->setIcon( LXDG::findIcon("favorites", ":/images/default-favorite.png") );
  appM = new QMenu(this);
    appB->setMenu(appM);
    this->layout()->addWidget(appB);
    connect(appM,SIGNAL(triggered(QAction*)),this,SLOT(ActionTriggered(QAction*)) );
  //Directories on the desktop
  dirB = new LTBWidget(this);
    //connect(dirB, SIGNAL(clicked()), dirB, SLOT(showMenu()) );
    //connect(dirB, SIGNAL(longClicked()), dirB, SLOT(showMenu()) );
    dirB->setIcon( LXDG::findIcon("folder", ":/images/default-dir.png") );
  dirM = new QMenu(this);
    dirB->setMenu(dirM);
    this->layout()->addWidget(dirB);
    connect(dirM,SIGNAL(triggered(QAction*)),this,SLOT(ActionTriggered(QAction*)) );
  //Audio Files on the desktop
  audioM = new QMenu(tr("Audio"), this);
    connect(audioM,SIGNAL(triggered(QAction*)),this,SLOT(ActionTriggered(QAction*)) );
    audioM->setIcon( LXDG::findIcon("audio-x-generic",":/images/default-audiofile.png") );
  //Video Files on the desktop
  videoM = new QMenu(tr("Video"), this);
    connect(videoM,SIGNAL(triggered(QAction*)),this,SLOT(ActionTriggered(QAction*)) );
    videoM->setIcon( LXDG::findIcon("video-x-generic",":/images/default-video.png") );
  //Picture Files on the desktop
  pictureM = new QMenu(tr("Pictures"), this);
    connect(pictureM,SIGNAL(triggered(QAction*)),this,SLOT(ActionTriggered(QAction*)) );
    pictureM->setIcon( LXDG::findIcon("image-x-generic",":/images/default-graphicsfile.png") );
  //Other Files on the desktop
  otherM = new QMenu(tr("Other Files"), this);
    connect(otherM,SIGNAL(triggered(QAction*)),this,SLOT(ActionTriggered(QAction*)) );
    otherM->setIcon( LXDG::findIcon("unknown",":/images/default-file.png") );
  //All Files Button
  fileB = new LTBWidget(this);
    //connect(fileB, SIGNAL(clicked()), fileB, SLOT(showMenu()) );
    //connect(fileB, SIGNAL(longClicked()), fileB, SLOT(showMenu()) );
    fileB->setIcon( LXDG::findIcon("user-desktop", ":/images/default-file.png") );
  fileM = new QMenu(this);
    fileB->setMenu(fileM);
    this->layout()->addWidget(fileB);
}

QAction* LDeskBarPlugin::newAction(QString filepath, QString name, QString iconpath){
  return newAction(filepath, name, QIcon(iconpath));
}

QAction* LDeskBarPlugin::newAction(QString filepath, QString name, QIcon icon){
  QAction *act = new QAction(icon, name, this);
    act->setWhatsThis(filepath);
  return act;	 
}

void LDeskBarPlugin::updateMenu(QMenu* menu, QFileInfoList files, bool trim){
  menu->clear();
  //re-create the menu (since it is hidden from view)
  for(int i=0; i<files.length(); i++){
    qDebug() << "New Menu Item:" << files[i].fileName();
    if(trim){ totals.removeAll(files[i]); }
    menu->addAction( newAction( files[i].canonicalFilePath(), files[i].fileName(), "") );
  }
}

// =======================
//     PRIVATE SLOTS
// =======================
void LDeskBarPlugin::ActionTriggered(QAction* act){
 //Open up the file with the appropriate application
 QString cmd = "lumina-open "+act->whatsThis();
 qDebug() << "Open File:" << cmd;
 QProcess::startDetached(cmd);
}

void LDeskBarPlugin::desktopChanged(){
  if(!desktopPath.isEmpty()){
    QDir dir(desktopPath);
    totals = dir.entryInfoList( QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot, QDir::Name);
    //Update all the special menus (trimming the totals list as we go)
    updateMenu(dirM, dir.entryInfoList( QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name) );
    updateMenu(audioM, dir.entryInfoList( audioFilter, QDir::Files, QDir::Name) );
    updateMenu(videoM, dir.entryInfoList( videoFilter, QDir::Files, QDir::Name) );
    updateMenu(pictureM, dir.entryInfoList( pictureFilter, QDir::Files, QDir::Name) );
    //Now update the launchers
    QFileInfoList exe = dir.entryInfoList( QStringList() << "*.desktop", QDir::Files, QDir::Name );
      // - Get a complete list of apps (in alphabetical order)
      QList<XDGDesktop> exeList;
      for(int i=0; i<exe.length(); i++){
      	totals.removeAll(exe[i]); //Remove this item from the totals
      	bool ok = false;
        XDGDesktop df = LXDG::loadDesktopFile(exe[i].canonicalFilePath(), ok);
        if(ok){
          if( LXDG::checkValidity(df) && !df.isHidden ){ exeList << df; }
        }
      }
      exeList = LXDG::sortDesktopNames(exeList);
      // - Now re-create the menu with the apps
      appM->clear();
      for(int i=0; i<exeList.length(); i++){
        appM->addAction( newAction(exeList[i].filePath, exeList[i].name, LXDG::findIcon(exeList[i].icon, ":/images/default-application.png")) );
      }
    //Now update the other menu with everything else that is left
    updateMenu(otherM, totals, false);
    //Now update the file menu as appropriate
    fileM->clear();
    if(!audioM->isEmpty()){ fileM->addMenu(audioM); }
    if(!pictureM->isEmpty()){ fileM->addMenu(pictureM); }
    if(!videoM->isEmpty()){ fileM->addMenu(videoM); }
    if(!otherM->isEmpty()){ fileM->addMenu(otherM); }
    //Check for a single submenu, and skip the main if need be
    if(fileM->actions().length()==1){
      if(!audioM->isEmpty()){ fileB->setMenu(audioM); }
      else if(!pictureM->isEmpty()){ fileB->setMenu(pictureM); }
      else if(!videoM->isEmpty()){ fileB->setMenu(videoM); }
      else if(!otherM->isEmpty()){ fileB->setMenu(otherM); }
    }else{
      fileB->setMenu(fileM);	    
    }
  }	
  //Setup the visibility of the buttons
  appB->setVisible( !appM->isEmpty() );
  dirB->setVisible( !dirM->isEmpty() );
  fileB->setVisible( !fileM->isEmpty() );
  //Clear the totals list (since no longer in use)
  totals.clear();
}
	
