//===========================================
//  Lumina-DE source code
//  Copyright (c) 2014, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#include "TrayIcon.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>

TrayIcon::TrayIcon(QWidget *parent) : QX11EmbedContainer(parent){
  //this->setStyleSheet("background: grey;");
  this->setBackgroundRole(QPalette::NoRole);
  //this->setPalette(QPalette(
  AID = 0; //nothing attached yet
  connect(this, SIGNAL(clientClosed()), this, SLOT(detachApp()) );
  connect(this, SIGNAL(error(QX11EmbedContainer::Error)), this, SLOT(detachApp()) );
  connect(this, SIGNAL(clientIsEmbedded()), this, SLOT(updateIcon()) );
  connect(this, SIGNAL(clientIsEmbedded()), this, SIGNAL(AppAttached()) );
}

TrayIcon::~TrayIcon(){
  if(AID!=0){
    detachApp();
  }
}

WId TrayIcon::appID(){
  return AID;
}

void TrayIcon::attachApp(WId id){
  if(id==0){ return; } //nothing to attach
  else if(AID!=0){ qWarning() << "Tray Icon is already attached to a window!"; return; }
  AID = id;
  this->embedClient(AID);
  //QTimer::singleShot(0,this,SLOT(slotAttach()) );
}

void TrayIcon::setSizeSquare(int side){
  this->setFixedSize( QSize(side, side) );
}

// ==============
//   PUBLIC SLOTS
// ==============
void TrayIcon::detachApp(){
  if(AID==0){ return; } //already detached
  //Now detach the application window and clean up
  //LX11::UnembedWindow(AID);
  qDebug() << "Detach Client:" << AID; //this->clientWinId();
  this->discardClient();
  //LX11::DestroyWindow(IID);
  IID = 0;
  AID = 0;
  emit AppClosed();
}

// ==============
//   PRIVATE SLOTS
// ==============
void TrayIcon::slotAttach(){
  IID = LX11::CreateWindow( this->winId(), this->rect() );
  if( LX11::EmbedWindow(AID, IID) ){
    LX11::RestoreWindow(AID); //make it visible
    updateIcon();
    qDebug() << "System Tray App:" << AID;
    emit AppAttached();
  }else{
    qWarning() << "Could not Embed Tray Application:" << AID;
    LX11::DestroyWindow(IID);
    IID = 0;
    AID = 0;
    emit AppClosed();
  }
}

void TrayIcon::updateIcon(){
  //Make sure the icon is square
  QSize icosize = this->size();
  //qDebug() << " - size:" << icosize.width() << icosize.height();
  //if(icosize.width() > icosize.height() && icosize.width()<100){ icosize.setHeight(icosize.width()); }
  //else if(icosize.width() != icosize.height()){ icosize.setWidth(icosize.height()); }
  //qDebug() << " - new size:" << icosize.width() << icosize.height();
  //this->setFixedSize(icosize);
  //QRect loc( this->mapToGlobal(QPoint(0,0)), icosize );
  XSetWindowBackground(QX11Info::display(), this->clientWinId(), 200);
  LX11::ResizeWindow(this->clientWinId(),  icosize.width(), icosize.height());
  //LX11::ResizeWindow(AID,  icosize.width(), icosize.height());
  //Load the icon
  /*QIcon ico;// = LX11::WindowIcon(AID);
  if(ico.isNull()){
    //Could not get icon - try image instead
    QPixmap pix = LX11::WindowImage(AID);
    if(pix.isNull()){
      //Could not get the image - grab the window instead
      this->setPixmap( QPixmap::grabWindow(AID).scaled(icosize, Qt::KeepAspectRatio, Qt::SmoothTransformation) );
    }else{
      this->setPixmap(pix.scaled(icosize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
  }else{
    this->setPixmap(ico.pixmap(icosize));	    
  }*/
}

// =============
//     PROTECTED
// =============
void TrayIcon::draw(QPaintEvent *event){
  //Make sure the window is the right size for the icon
  updateIcon();
  event->ignore(); //keep going with propagating this event
}
