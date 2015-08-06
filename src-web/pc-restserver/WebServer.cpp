// ===============================
//  PC-BSD REST API Server
// Available under the 3-clause BSD License
// Written by: Ken Moore <ken@pcbsd.org> July 2015
// =================================
#include "WebServer.h"

#include <QCoreApplication>
#include <QUrl>
#include <QDebug>
#include <QtDebug> //for better syntax of qDebug() / qWarning() / qCritical() / qFatal()
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonObject>
//=======================
//              PUBLIC
//=======================
WebServer::WebServer() : QWebSocketServer("pc-restserver", QWebSocketServer::SecureMode){
  csock = 0; //no current socket connected
  //Setup all the various settings
  syscache = new SysCacheClient(this);
  //Any SSL changes
    QSslConfiguration ssl = this->sslConfiguration();
      ssl.setProtocol(QSsl::SecureProtocols);
    this->setSslConfiguration(ssl);

  //Setup Connections
  connect(this, SIGNAL(closed()), this, SLOT(ServerClosed()) );
  connect(this, SIGNAL(serverError(QWebSocketProtocol::CloseCode)), this, SLOT(ServerError(QWebSocketProtocol::CloseCode)) );
  connect(this, SIGNAL(newConnection()), this, SLOT(NewSocketConnection()) );
  connect(this, SIGNAL(acceptError(QAbstractSocket::SocketError)), this, SLOT(NewConnectError(QAbstractSocket::SocketError)) );
  connect(this, SIGNAL(originAuthenticationRequired(QWebSocketCorsAuthenticator*)), this, SLOT(OriginAuthRequired(QWebSocketCorsAuthenticator*)) );
  connect(this, SIGNAL(peerVerifyError(const QSslError&)), this, SLOT(PeerVerifyError(const QSslError&)) );
  connect(this, SIGNAL(sslErrors(const QList<QSslError>&)), this, SLOT(SslErrors(const QList<QSslError>&)) );
}

WebServer::~WebServer(){
	
}

bool WebServer::startServer(){
  bool ok = this->listen();
  if(ok){ 
    qDebug() << "Server Started:" << QDateTime::currentDateTime().toString(Qt::ISODate);
    qDebug() << " Name:" << this->serverName() << "Port:" << this->serverPort() << "URL:" << this->serverUrl() << "Address:" << this->serverAddress().toString();
  }else{ qCritical() << "Could not start server - exiting..."; }
  return ok;
}

void WebServer::stopServer(){
  this->close();
}

//=======================
//             PRIVATE
//=======================
void WebServer::EvaluateREST(QString msg){
  qDebug() << "REST Message:" << msg;
  //Parse the message into it's elements and proceed to the main data evaluation
	
  EvaluateJSON(QJsonDocument::fromRawData(msg.toUtf8(), msg.length()) );
}

void WebServer::EvaluateJSON(QJsonDocument doc){
  qDebug() << "JSON Message:" << doc.toJson();	
  //parse the message and do something
  QJsonDocument ret; //return message
  if(doc.isArray()){
    //use doc.array() for access to QJsonArray
    for(int i=0; i<doc.array().count(); i++){
      switch( doc.array().at(i).type() ){
	case QJsonValue::String:
		
	  break;
	case QJsonValue::Array:
	  
	  break;
	case QJsonValue::Object:
	  
	  break;
	default:
	  qDebug() << "Unknown type of input";
      }
    }

  }else if(doc.isObject()){
    //use doc.object() for access to QJsonObject
    
  }
  
  //Return any information
  csock->sendBinaryMessage(ret.toBinaryData());
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
  if(csock!=0){ qDebug() << " - Placed in queue"; return;}
  csock = this->nextPendingConnection();
  connect(csock, SIGNAL(textMessageReceived(const QString&)), this, SLOT(EvaluateMessage(const QString&)) );
  connect(csock, SIGNAL(binaryMessageReceived(const QByteArray&)), this, SLOT(EvaluateMessage(const QByteArray&)) );
  if(csock == 0){ qWarning() << " - new connection invalid, skipping..."; QTimer::singleShot(10, this, SLOT(NewSocketConnection())); return; }
  qDebug() <<  " - Accepting connection:" << csock->origin();
  //QTimer::singleShot(0,this, SLOT(EvaluateConnection()));
}

void WebServer::NewConnectError(QAbstractSocket::SocketError err){
  if(csock!=0){
    qWarning() << "New Connection Error["+QString::number(err)+"]:" << csock->errorString();
    csock->close();
  }else{
    qWarning() << "New Connection Error["+QString::number(err)+"]:" << this->errorString();
  }
  csock = 0; //remove the current socket
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

void WebServer::EvaluateMessage(const QByteArray &msg){
  //needs a current socket (csock), unsets it when done
  qDebug() << "New Binary Message:";
  EvaluateREST( QString(msg) );
  csock->close();
  csock = 0; //Done with the socket, free it up and re-check for more
  QTimer::singleShot(0,this, SLOT(NewSocketConnection()));	
}

void WebServer::EvaluateMessage(const QString &msg){ 
  //needs a current socket (csock), unsets it when done
  qDebug() << "New Text Message:";
  //Now convert it from a REST message into the 
  EvaluateREST(msg);
  csock->close();
  csock = 0; //Done with the socket, free it up and re-check for more
  QTimer::singleShot(0,this, SLOT(NewSocketConnection()));
}