#include "syscache-daemon.h"

SysCacheDaemon::SysCacheDaemon(QObject *parent) : QObject(parent){
  curSock = 0; //no local socket connected yet
  server = new QLocalServer(this);
    //server->setMaxPendingConnections(10); //should go through them pretty fast, no need for more
    connect(server, SIGNAL(newConnection()), this, SLOT(checkForConnections()));
}

SysCacheDaemon::~SysCacheDaemon(){
}

//General Start/Stop functions
void SysCacheDaemon::startServer(){
  if( server->listen("/var/run/syscache.pipe") ){
    qDebug() << "SysCacheDaemon now listening for connections at /var/run/syscache.pipe";
  }else{
    qDebug() << "Error: SysCacheDaemon could not create pipe at /var/run/syscache.pipe";
  }
}

void SysCacheDaemon::stopServer(){
  if(server->isListening()){ server->close(); }
  QLocalServer::removeServer("/var/run/syscache.pipe"); //clean up
  QCoreApplication::exit(0);
}

//Server/Client connections
void SysCacheDaemon::checkForConnections(){
  //Check if we are ready for the next request
  bool getnext = (curSock==0);
  if(!getnext){ getnext = !curSock->isValid(); }
  if(getnext){ getnext = server->hasPendingConnections(); }
  
  //Now get the next request if appropriate
  if(getnext){
    curSock = server->nextPendingConnection();
    connect(curSock, SIGNAL(disconnected()), this, SLOT(requestFinished()) );
    QTimer::singleShot(0,this, SLOT(answerRequest()));
  }
}

void SysCacheDaemon::answerRequest(){
  curSock->waitForReadyRead(1000); //wait max 1 second for the request from the client
  QStringList req, out;
  bool stopdaemon=false;
  QTextStream stream(curSock);
  while(!stream.atEnd()){
    req = QString(stream.readLine()).split(" ");
    qDebug() << "Request Received:" << req;
    if(req.join("")=="shutdowndaemon"){ stopdaemon=true; break; }
    out << "[ERROR] No DB integration implemented yet";
  }
  //Now write the output to the socket and disconnect it
  stream << out.join("\n");
  //curSock->disconnectFromServer();
  stream << "\n[FINISHED]";
  if(stopdaemon){ QTimer::singleShot(10, this, SLOT(stopServer())); }
}

void SysCacheDaemon::requestFinished(){
  curSock=0; //reset the internal pointer
  //Now look for the next request
  QTimer::singleShot(0,this, SLOT(checkForConnections()));
}