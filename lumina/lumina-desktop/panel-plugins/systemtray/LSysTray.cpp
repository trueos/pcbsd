//===========================================
//  Lumina-DE source code
//  Copyright (c) 2012, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#include "LSysTray.h"

LSysTray::LSysTray(QWidget *parent, QString id, bool horizontal) : LPPlugin(parent, id, horizontal){
  //if(horizontal){ this->layout()->setAlignment(Qt::AlignRight | Qt::AlignVCenter); }
  //else{ this->layout()->setAlignment(Qt::AlignHCenter | Qt::AlignTop); }
  this->layout()->setAlignment(Qt::AlignCenter);
  this->setStyleSheet("LPPlugin{ background: black; border: 1px solid grey; border-radius: 3px; }");
  this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
  this->layout()->setSpacing(0);
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
    trayIcons[i]->detachApp();
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
    if(trayIcons[i]->appID() == win){ exists=true; break; }
  }
  if(!exists){
    //qDebug() << " - New Icon";
    TrayIcon *cont = new TrayIcon(this);
      connect(cont, SIGNAL(AppClosed()), this, SLOT(trayAppClosed()) );
      connect(cont, SIGNAL(AppAttached()), this, SLOT(updateStatus()) );
      trayIcons << cont;
      this->layout()->addWidget(cont);
      if(this->layout()->direction()==QBoxLayout::LeftToRight){
        cont->setSizeSquare(this->height()); //horizontal tray
	this->setMaximumSize( trayIcons.length()*this->height(), 10000);
      }else{
	cont->setSizeSquare(this->width()); //vertical tray
	this->setMaximumSize(10000, trayIcons.length()*this->width());
      }
      cont->attachApp(win);
    //this->layout()->update(); //make sure there is no blank space
  }
}

void LSysTray::updateStatus(){
  qDebug() << "System Tray: Client Attached";
  this->layout()->update(); //make sure there is no blank space
  qDebug() << " - Items:" << trayIcons.length();
}

void LSysTray::trayAppClosed(){
  for(int i=0;  i<trayIcons.length(); i++){
    if(trayIcons[i]->appID() == 0){
      qDebug() << "System Tray: Removing icon";
      TrayIcon *cont = trayIcons.takeAt(i);
      this->layout()->removeWidget(cont);
      delete cont;
    }
  }
  //Re-adjust the maximum widget size
  if(this->layout()->direction()==QBoxLayout::LeftToRight){
    this->setMaximumSize( trayIcons.length()*this->height(), 10000);
  }else{
    this->setMaximumSize(10000, trayIcons.length()*this->width());
  }
  this->layout()->update(); //update the layout (no gaps)
  this->update();	
}

