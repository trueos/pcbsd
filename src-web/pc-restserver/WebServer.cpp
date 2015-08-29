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


#define DEBUG 1
#define SCLISTDELIM QString("::::") //SysCache List Delimiter
#define PORTNUMBER 12142
//=======================
//              PUBLIC
//=======================
WebServer::WebServer() : QWebSocketServer("pc-restserver", QWebSocketServer::NonSecureMode){
  csock = 0; //no current socket connected
  //Setup all the various settings
  idletimer = new QTimer(this);
    idletimer->setInterval(5000); //every 5 seconds
    idletimer->setSingleShot(true);
  //Any SSL changes
    /*QSslConfiguration ssl = this->sslConfiguration();
      ssl.setProtocol(QSsl::SecureProtocols);
    this->setSslConfiguration(ssl);*/

  //Setup Connections
  connect(idletimer, SIGNAL(timeout()), this, SLOT(checkIdle()) );
  connect(this, SIGNAL(closed()), this, SLOT(ServerClosed()) );
  connect(this, SIGNAL(serverError(QWebSocketProtocol::CloseCode)), this, SLOT(ServerError(QWebSocketProtocol::CloseCode)) );
  connect(this, SIGNAL(newConnection()), this, SLOT(NewSocketConnection()) );
  connect(this, SIGNAL(acceptError(QAbstractSocket::SocketError)), this, SLOT(NewConnectError(QAbstractSocket::SocketError)) );
  connect(this, SIGNAL(originAuthenticationRequired(QWebSocketCorsAuthenticator*)), this, SLOT(OriginAuthRequired(QWebSocketCorsAuthenticator*)) );
  connect(this, SIGNAL(peerVerifyError(const QSslError&)), this, SLOT(PeerVerifyError(const QSslError&)) );
  connect(this, SIGNAL(sslErrors(const QList<QSslError>&)), this, SLOT(SslErrors(const QList<QSslError>&)) );
}

WebServer::~WebServer(){
  //syscacheThread->exit(0);
}

bool WebServer::startServer(){
  bool ok = this->listen(QHostAddress::Any, PORTNUMBER);
  if(ok){ 
    QCoreApplication::processEvents();
    qDebug() << "Server Started:" << QDateTime::currentDateTime().toString(Qt::ISODate);
    qDebug() << " Name:" << this->serverName() << "Port:" << this->serverPort();
    qDebug() << " URL:" << this->serverUrl().toString() << "Remote Address:" << this->serverAddress().toString();
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
  //Parse the message into it's elements and proceed to the main data evaluation
  RestInputStruct IN(msg);
  if(DEBUG){
    qDebug() << "New REST Message:";
    qDebug() << "  VERB:" << IN.VERB << "URI:" << IN.URI;
    qDebug() << "  HEADERS:" << IN.Header;
    qDebug() << "  BODY:" << IN.Body;
  }
  //Now check for the REST-specific verbs/actions
  if(IN.VERB == "OPTIONS" || IN.VERB == "HEAD"){
    RestOutputStruct out;	  
      out.CODE = RestOutputStruct::OK;
      if(IN.VERB=="HEAD"){
	
      }else{ //OPTIONS
	out.Header << "Allow: HEAD, GET";
	out.Header << "Hosts: /syscache";	      
      }
      out.Header << "Accept: text/json";
      out.Header << "Content-Type: text/json; charset=utf-8";
    csock->sendTextMessage(out.assembleMessage());
  }else{
    EvaluateRequest(IN);
  }
}

void WebServer::EvaluateRequest(const RestInputStruct &REQ){
  RestOutputStruct out;
  if(REQ.VERB != "GET"){
    //Non-supported request (at the moment) - return an error message
    out.CODE = RestOutputStruct::BADREQUEST;
  }else{
    //GET request
    //Now check the body of the message and do what it needs
    QJsonDocument doc = QJsonDocument::fromJson(REQ.Body.toUtf8());
    if(doc.isNull()){ qWarning() << "Empty JSON Message Body!!" << REQ.Body.toUtf8(); }
    //Define the output structures
    QJsonDocument ret; //return message
    //parse the message and do something
    //Objects contain other key/value pairs - this is 99% of cases
    if(doc.isObject()){
      QJsonObject obj;
      QStringList keys = doc.object().keys();
      if(REQ.URI.toLower()=="/syscache"){
        QStringList reqs = keys.filter("request",Qt::CaseInsensitive);
        if(!reqs.isEmpty()){
	  qDebug() << "Parsing Inputs:" << reqs;
	  for(int r=0; r<reqs.length(); r++){
	    QString req =  JsonValueToString(doc.object().value(reqs[r]));
	    qDebug() << "  ["+reqs[r]+"]="+req;
	    QStringList values = SysCacheClient::parseInputs( QStringList() << req ); 
	    values.removeAll("");
	    //Quick check if a list of outputs was returned
	    if(values.length()==1){
	      values = values[0].split(SCLISTDELIM); //split up the return list (if necessary)
	      values.removeAll("");
	    }
	    qDebug() << " - Returns:" << values;
	    keys.removeAll(reqs[r]); //this key was already processed
	    if(values.length()<2){
	      obj.insert(reqs[r],values.join(""));
	    }else{
	      //This is an array of outputs
	      QJsonArray arr;
              for(int i=0; i<values.length(); i++){ arr.append(values[i]); }
	      obj.insert(reqs[r],arr);
            }
          }
        } //end of special "request" objects
      
      }else{
        qDebug() << "Object Variables:" << keys;
        for(int i=0; i<keys.length(); i++){
          qDebug() << keys[i]+"="+JsonValueToString(doc.object().value(keys[i]) );
        }	  
      }
      ret.setObject(obj);
    //Special case for a single syscache input (array of strings)
    }else if(doc.isArray() && REQ.URI.toLower()=="/syscache"){
        QStringList inputs = JsonArrayToStringList(doc.array());
        qDebug() << " syscache inputs:" << inputs;
        QJsonObject obj;
        QStringList values = SysCacheClient::parseInputs(inputs );
        for(int i=0; i<values.length(); i++){
	  if(values[i].contains(SCLISTDELIM)){
	    //This is an array of values
	    QStringList vals = values[i].split(SCLISTDELIM);
	    vals.removeAll("");
	    QJsonArray arr;
                for(int j=0; j<vals.length(); j++){ arr.append(vals[j]); }
	      obj.insert("Value"+QString::number(i),arr);
	  }else{
            obj.insert("Value"+QString::number(i),values[i]);
	  }
        }
      ret.setObject(obj);
    }
    //Assemble the outputs for this "GET" request
    out.CODE = RestOutputStruct::OK;
    out.Body = ret.toJson();
    out.Header << "Content-Type: text/json; charset=utf-8";
  }
  //Return any information
  csock->sendTextMessage(out.assembleMessage());
}

QString WebServer::JsonValueToString(QJsonValue val){
  //Note: Do not use this on arrays - only use this on single-value values
  QString out;
  switch(val.type()){
    case QJsonValue::Bool:
	out = (val.toBool() ? "true": "false"); break;
    case QJsonValue::Double:
	out = QString::number(val.toDouble()); break;
    case QJsonValue::String:
	out = val.toString(); break;
    case QJsonValue::Array:
	out = "\""+JsonArrayToStringList(val.toArray()).join("\" \"")+"\"";
    default:
	out.clear();
  }
  return out;
}

QStringList WebServer::JsonArrayToStringList(QJsonArray array){
  //Note: This assumes that the array is only values, not additional objects
  QStringList out;
  qDebug() << "Array to List:" << array.count();
  for(int i=0; i<array.count(); i++){
    out << JsonValueToString(array.at(i));
  }
  return out;  
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

void WebServer::checkIdle(){
  //This function is called automatically every few seconds that a client is connected
  if(csock !=0){
    qDebug() << " - Client Timeout: Closing connection...";
    csock->close(); //timeout - close the connection to make way for others
  }
}

// New Connection Signals
void WebServer::NewSocketConnection(){
  if(!this->hasPendingConnections()){ return; }
  qDebug() << "New Socket Connection";	
  if(csock!=0){ qDebug() << " - Placed in queue"; return;}
  if(idletimer->isActive()){ idletimer->stop(); }
  csock = this->nextPendingConnection();
  connect(csock, SIGNAL(textMessageReceived(const QString&)), this, SLOT(EvaluateMessage(const QString&)) );
  connect(csock, SIGNAL(binaryMessageReceived(const QByteArray&)), this, SLOT(EvaluateMessage(const QByteArray&)) );
  connect(csock, SIGNAL(aboutToClose()), this, SLOT(SocketClosing()) );
  connect(csock, SIGNAL(disconnected()), this, SLOT(NewSocketConnection()) );
  if(csock == 0){ qWarning() << " - new connection invalid, skipping..."; QTimer::singleShot(10, this, SLOT(NewSocketConnection())); return; }
  qDebug() <<  " - Accepting connection:" << csock->origin();
  idletimer->start();
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

void WebServer::SocketClosing(){
  qDebug() << "Socket Closing...";
  if(idletimer->isActive()){ idletimer->stop(); }
  //Stop any current requests

  //Reset the pointer
  csock = 0;	
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
  if(idletimer->isActive()){ idletimer->stop(); }
  EvaluateREST( QString(msg) );
  idletimer->start(); 
  qDebug() << "Done with Message";
}

void WebServer::EvaluateMessage(const QString &msg){ 
  //needs a current socket (csock), unsets it when done
  qDebug() << "New Text Message:";
  if(idletimer->isActive()){ idletimer->stop(); }
  EvaluateREST(msg);
  idletimer->start(); 
  qDebug() << "Done with Message";
}