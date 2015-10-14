// ===============================
//  PC-BSD REST API Server
// Available under the 3-clause BSD License
// Written by: Ken Moore <ken@pcbsd.org> July 2015
// =================================
#include "WebServer.h"

#include <QCoreApplication>
#include <QUrl>
#include <QFile>
#include <QTextStream>
#include <QProcess>

#define DEBUG 1

#define PORTNUMBER 12142

#define APPCAFEWORKING QString("/var/tmp/appcafe/dispatch-queue.working")

//=======================
//              PUBLIC
//=======================
WebServer::WebServer() : QWebSocketServer("syscache-webclient", QWebSocketServer::NonSecureMode){
  //Setup all the various settings
  //Any SSL changes
    /*QSslConfiguration ssl = this->sslConfiguration();
      ssl.setProtocol(QSsl::SecureProtocols);
    this->setSslConfiguration(ssl);*/
  AUTH = new AuthorizationManager();
  watcher = new QFileSystemWatcher(this);
    
  //Setup Connections
  connect(this, SIGNAL(closed()), this, SLOT(ServerClosed()) );
  connect(this, SIGNAL(serverError(QWebSocketProtocol::CloseCode)), this, SLOT(ServerError(QWebSocketProtocol::CloseCode)) );
  connect(this, SIGNAL(newConnection()), this, SLOT(NewSocketConnection()) );
  connect(this, SIGNAL(acceptError(QAbstractSocket::SocketError)), this, SLOT(NewConnectError(QAbstractSocket::SocketError)) );
  connect(this, SIGNAL(originAuthenticationRequired(QWebSocketCorsAuthenticator*)), this, SLOT(OriginAuthRequired(QWebSocketCorsAuthenticator*)) );
  connect(this, SIGNAL(peerVerifyError(const QSslError&)), this, SLOT(PeerVerifyError(const QSslError&)) );
  connect(this, SIGNAL(sslErrors(const QList<QSslError>&)), this, SLOT(SslErrors(const QList<QSslError>&)) );
  connect(watcher, SIGNAL(fileChanged(const QString&)), this, SLOT(WatcherUpdate(QString)) );
  connect(watcher, SIGNAL(directoryChanged(const QString&)), this, SLOT(WatcherUpdate(QString)) );
}

WebServer::~WebServer(){
  delete AUTH;
}

bool WebServer::startServer(){
  bool ok = this->listen(QHostAddress::Any, PORTNUMBER);
  if(ok){ 
    QCoreApplication::processEvents();
    qDebug() << "Server Started:" << QDateTime::currentDateTime().toString(Qt::ISODate);
    qDebug() << " Name:" << this->serverName() << "Port:" << this->serverPort();
    qDebug() << " URL:" << this->serverUrl().toString() << "Remote Address:" << this->serverAddress().toString();
    if(!QFile::exists(APPCAFEWORKING)){ QProcess::execute("touch "+APPCAFEWORKING); }
    qDebug() << " Dispatcher Events:" << APPCAFEWORKING;
    watcher->addPath(APPCAFEWORKING);
    WatcherUpdate(APPCAFEWORKING); //load it initially
  }else{ qCritical() << "Could not start server - exiting..."; }
  return ok;
}

void WebServer::stopServer(){
  this->close();
}

//===================
//     PRIVATE
//===================
QString WebServer::generateID(){
  int id = 0;
  for(int i=0; i<OpenSockets.length(); i++){
    if(OpenSockets[i]->ID().toInt()>=id){ id = OpenSockets[i]->ID().toInt()+1; }
  }
  return QString::number(id);
}

QString WebServer::readFile(QString path){
  QFile file(path);
  if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){ return ""; }
  QTextStream in(&file);
  QString contents = in.readAll();
  file.close();
  if(contents.endsWith("\n")){ contents.chop(1); }
  return contents;  
}

//=======================
//       PRIVATE SLOTS
//=======================
// Overall Server signals
void WebServer::ServerClosed(){
  qDebug() << "Server Closed:" << QDateTime::currentDateTime().toString(Qt::ISODate);
  QCoreApplication::exit(0);
}

void WebServer::ServerError(QWebSocketProtocol::CloseCode code){
  qWarning() << "Server Error["+QString::number(code)+"]:" << this->errorString();
}

// New Connection Signals
void WebServer::NewSocketConnection(){
  if(!this->hasPendingConnections()){ return; }
  qDebug() << "New Socket Connection";	
  //if(idletimer->isActive()){ idletimer->stop(); }
  QWebSocket *csock = this->nextPendingConnection();
  if(csock == 0){ qWarning() << " - new connection invalid, skipping..."; QTimer::singleShot(10, this, SLOT(NewSocketConnection())); return; }
  qDebug() <<  " - Accepting connection:" << csock->origin();
  WebSocket *sock = new WebSocket(csock, generateID(), AUTH);
  connect(sock, SIGNAL(SocketClosed(QString)), this, SLOT(SocketClosed(QString)) );
  connect(this, SIGNAL(DispatchStatusUpdate(QString)), sock, SLOT(AppCafeStatusUpdate(QString)) );
  sock->setLastDispatch(lastDispatch); //make sure this socket is aware of the latest notification
  OpenSockets << sock;
}

void WebServer::NewConnectError(QAbstractSocket::SocketError err){
  //if(csock!=0){
    //qWarning() << "New Connection Error["+QString::number(err)+"]:" << csock->errorString();
    //csock->close();
  //}else{            
    qWarning() << "New Connection Error["+QString::number(err)+"]:" << this->errorString();
  //}
  //csock = 0; //remove the current socket
  QTimer::singleShot(0,this, SLOT(NewSocketConnection()) ); //check for a new connection
  
}

// SSL/Authentication Signals
void WebServer::OriginAuthRequired(QWebSocketCorsAuthenticator *auth){
  qDebug() << "Origin Auth Required:" << auth->origin();
  //if(auth->origin() == this->serverAddress().toString()){
  // TO-DO: Provide some kind of address filtering routine for which to accept/reject
    qDebug() << " - Allowed";
    auth->setAllowed(true);
  //}else{
    //qDebug() << " - Not Allowed";
    //auth->setAllowed(false);
  //}
	
}

void WebServer::PeerVerifyError(const QSslError &err){
  qDebug() << "Peer Verification Error:" << err.errorString();
	
}

void WebServer::SslErrors(const QList<QSslError> &list){
  qWarning() << "SSL Errors:";
  for(int i=0; i<list.length(); i++){
    qWarning() << " - " << list[i].errorString();
  }
}

void WebServer::SocketClosed(QString ID){
  for(int i=0; i<OpenSockets.length(); i++){
    if(OpenSockets[i]->ID()==ID){ delete OpenSockets.takeAt(i); break; }
  }
  QTimer::singleShot(0,this, SLOT(NewSocketConnection()) ); //check for a new connection
}

void WebServer::WatcherUpdate(QString path){
  if(path==APPCAFEWORKING){
    //Read the file contents
    QString stat = readFile(APPCAFEWORKING);
    if(stat.simplified().isEmpty()){ stat = "idle"; }
    //qDebug() << "Dispatcher Update:" << stat;
    lastDispatch = stat; //save for later
    //Forward those contents on to the currently-open sockets
    emit DispatchStatusUpdate(stat);
  }
  //Make sure this file/dir is not removed from the watcher
  if(!watcher->files().contains(path) && !watcher->directories().contains(path)){
    watcher->addPath(path); //re-add it to the watcher. This happens when the file is removed/re-created instead of just overwritten
  }
}