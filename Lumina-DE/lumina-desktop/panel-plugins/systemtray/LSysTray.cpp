//===========================================
//  Lumina-DE source code
//  Copyright (c) 2012, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#include "LSysTray.h"

LSysTray::LSysTray(QWidget *parent) : LPPlugin(parent, "systemtray"){
  this->layout()->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  isRunning = false;
  start();
}

LSysTray::~LSysTray(){
 if(isRunning){
   this->stop();
 }
}

void LSysTray::start(){
  if(LSession::StartupSystemTray()){
    //Now connect the application signals to the X event parser
    connect(LSession::instance(),SIGNAL(NewSystemTrayApp(WId)),this,SLOT(addTrayIcon(WId)) );
    qDebug() << "System Tray Started Successfully";
    isRunning = true;
  }
}

void LSysTray::stop(){
  if(!isRunning){ return; }
  //Release all the tray applications and delete the containers
  for(int i=(trayIcons.length()-1); i>=0; i--){
    trayIcons[i]->discardClient();
    delete trayIcons.takeAt(i);
  }
  //Now close down the tray
  LSession::CloseSystemTray();

  disconnect(this);
  isRunning = false;
}

// ========================
//    PRIVATE FUNCTIONS
// ========================
void LSysTray::addTrayIcon(WId win){
  if(win == 0 || !isRunning){ return; }
  //qDebug() << "System Tray: Add Tray Icon:" << win;
  bool exists = false;
  for(int i=0; i<trayIcons.length(); i++){
    if(trayIcons[i]->clientWinId() == win){ exists=true; break; }
  }
  if(!exists){
    //qDebug() << " - New Icon";
    QX11EmbedContainer *cont = new QX11EmbedContainer(this);
      cont->setFixedSize(this->height(),this->height());
    //Now embed the client
    cont->embedClient(win);
    connect(cont,SIGNAL(clientIsEmbedded()),this,SLOT(updateStatus()) );
    connect(cont,SIGNAL(clientClosed()),this,SLOT(trayAppClosed()) );
    trayIcons << cont;
    this->layout()->addWidget(cont);
    this->layout()->update(); //make sure there is no blank space
    //Set the background on the client window

  }
}

void LSysTray::updateStatus(){
  qDebug() << "System Tray: Client Attached";
  //Make sure that each icon has the appropriate background color
  /*for(int i=0;  i<trayIcons.length(); i++){
    if(trayIcons[i]->clientWinId() != 0){
      XSetWindowBackgroundPixmap(QX11Info::display(), trayIcons[i]->clientWinId(), ParentRelative);
      XSetWindowBackground(QX11Info::display(), trayIcons[i]->clientWinId(), CopyFromParent);
    }
  }*/
}

void LSysTray::trayAppClosed(){
  for(int i=0;  i<trayIcons.length(); i++){
    if(trayIcons[i]->clientWinId() == 0){
      qDebug() << "System Tray: Removing icon";
      QX11EmbedContainer *cont = trayIcons.takeAt(i);
      this->layout()->removeWidget(cont);
      delete cont;
    }
  }
  this->layout()->update(); //update the layout (no gaps)
  this->update();	
}

