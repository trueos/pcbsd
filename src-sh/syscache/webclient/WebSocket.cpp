// ===============================
//  PC-BSD REST API Server
// Available under the 3-clause BSD License
// Written by: Ken Moore <ken@pcbsd.org> July 2015
// =================================
#include "WebSocket.h"
#include "syscache-client.h"

#define DEBUG 1
#define SCLISTDELIM QString("::::") //SysCache List Delimiter

WebSocket::WebSocket(QWebSocket *sock, QString ID){
  SockID = ID;
  SOCKET = sock;
  idletimer = new QTimer(this);
    idletimer->setInterval(600000); //10-minute timeout
    idletimer->setSingleShot(true);
  connect(idletimer, SIGNAL(timeout()), this, SLOT(checkIdle()) );
  connect(SOCKET, SIGNAL(textMessageReceived(const QString&)), this, SLOT(EvaluateMessage(const QString&)) );
  connect(SOCKET, SIGNAL(binaryMessageReceived(const QByteArray&)), this, SLOT(EvaluateMessage(const QByteArray&)) );
  connect(SOCKET, SIGNAL(aboutToClose()), this, SLOT(SocketClosing()) );
  idletimer->start();
}

WebSocket::~WebSocket(){
  if(SOCKET!=0){
    SOCKET->close();
  }
  delete SOCKET;
}


QString WebSocket::ID(){
  return SockID;
}
	
//=======================
//             PRIVATE
//=======================
void WebSocket::EvaluateREST(QString msg){
  //Parse the message into it's elements and proceed to the main data evaluation
  RestInputStruct IN(msg);
  //NOTE: All the REST functionality is disabled for the moment, until we decide to turn it on again at a later time (just need websockets right now - not full REST)	

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
    SOCKET->sendTextMessage(out.assembleMessage());
  }else{ 
    EvaluateRequest(IN);
  }
}

void WebSocket::EvaluateRequest(const RestInputStruct &REQ){
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
	  if(DEBUG){ qDebug() << "Parsing Inputs:" << reqs; }
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
	    if(DEBUG){ qDebug() << " - Returns:" << values; }
	    keys.removeAll(reqs[r]); //this key was already processed
	    if(values.length()<2){
	      obj.insert(req,values.join(""));
	    }else{
	      //This is an array of outputs
	      QJsonArray arr;
              for(int i=0; i<values.length(); i++){ arr.append(values[i]); }
	      obj.insert(req,arr);
            }
          }
        } //end of special "request" objects
      
      }else{
        if(DEBUG){ qDebug() << "Object Variables:" << keys; }
        for(int i=0; i<keys.length(); i++){
          qDebug() << keys[i]+"="+JsonValueToString(doc.object().value(keys[i]) );
        }	  
      }
      ret.setObject(obj);
    //Special case for a single syscache input (array of strings)
    }else if(doc.isArray() && REQ.URI.toLower()=="/syscache"){
        QStringList inputs = JsonArrayToStringList(doc.array());
        if(DEBUG){ qDebug() << " syscache inputs:" << inputs; }
        QJsonObject obj;
        QStringList values = SysCacheClient::parseInputs(inputs );
        for(int i=0; i<values.length(); i++){
	  if(values[i].contains(SCLISTDELIM)){
	    //This is an array of values
	    QStringList vals = values[i].split(SCLISTDELIM);
	    vals.removeAll("");
	    QJsonArray arr;
                for(int j=0; j<vals.length(); j++){ arr.append(vals[j]); }
	      obj.insert(inputs[i],arr);
	  }else{
            obj.insert(inputs[i],values[i]);
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
  SOCKET->sendTextMessage(out.assembleMessage());
}

QString WebSocket::JsonValueToString(QJsonValue val){
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

QStringList WebSocket::JsonArrayToStringList(QJsonArray array){
  //Note: This assumes that the array is only values, not additional objects
  QStringList out;
  qDebug() << "Array to List:" << array.count();
  for(int i=0; i<array.count(); i++){
    out << JsonValueToString(array.at(i));
  }
  return out;  
}

// =====================
//       PRIVATE SLOTS
// =====================
void WebSocket::checkIdle(){
  //This function is called automatically every few seconds that a client is connected
  if(SOCKET !=0){
    qDebug() << " - Client Timeout: Closing connection...";
    SOCKET->close(); //timeout - close the connection to make way for others
  }
}

void WebSocket::SocketClosing(){
  qDebug() << "Socket Closing...";
  if(idletimer->isActive()){ idletimer->stop(); }
  //Stop any current requests

  //Reset the pointer
  SOCKET = 0;	
  emit SocketClosed(SockID);
}

void WebSocket::EvaluateMessage(const QByteArray &msg){
  qDebug() << "New Binary Message:";
  if(idletimer->isActive()){ idletimer->stop(); }
  EvaluateREST( QString(msg) );
  idletimer->start(); 
  qDebug() << "Done with Message";
}

void WebSocket::EvaluateMessage(const QString &msg){ 
  qDebug() << "New Text Message:";
  if(idletimer->isActive()){ idletimer->stop(); }
  EvaluateREST(msg);
  idletimer->start(); 
  qDebug() << "Done with Message";
}