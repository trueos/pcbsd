#include "syscache-daemon.h"

SysCacheDaemon::SysCacheDaemon(QObject *parent) : QObject(parent){
  curSock = 0; //no local socket connected yet
  server = new QLocalServer(this);
    //server->setMaxPendingConnections(10); //should go through them pretty fast, no need for more
    connect(server, SIGNAL(newConnection()), this, SLOT(checkForConnections()));
  DATA = new DB(this);
    QTimer::singleShot(300000, DATA, SLOT(startSync()) ); //wait five minutes to start the initial sync
}

SysCacheDaemon::~SysCacheDaemon(){
}

//General Start/Stop functions
void SysCacheDaemon::startServer(){
  if( server->listen("/var/run/syscache.pipe") ){
    QFile::setPermissions("/var/run/syscache.pipe", QFile::ReadUser | QFile::WriteUser | QFile::ReadGroup | QFile::WriteGroup | QFile::ReadOther | QFile::WriteOther);
    qDebug() << "SysCacheDaemon now listening for connections at /var/run/syscache.pipe";
  }else{
    qDebug() << "Error: SysCacheDaemon could not create pipe at /var/run/syscache.pipe";
  }
  
}

void SysCacheDaemon::stopServer(){
  if(server->isListening()){ server->close(); }
  QLocalServer::removeServer("/var/run/syscache.pipe"); //clean up
  DATA->shutDown();
  QCoreApplication::exit(0);
}

//Server/Client connections
void SysCacheDaemon::checkForConnections(){
  //qDebug() << "Check for Connections..." << curSock;
  static bool checking = false;
  if(checking){ return; }
  checking = true;
  //Check if we are ready for the next request
  bool getnext = (curSock==0);
  if(!getnext){ 
    getnext = !curSock->isValid(); 
    if(!getnext){ QTimer::singleShot(0, this, SLOT(answerRequest())); }
  }
  if(getnext){ getnext = server->hasPendingConnections(); }
  
  //Now get the next request if appropriate
  if(getnext){
    curSock = server->nextPendingConnection();
    //qDebug() << " - Found connection:" << curSock;
    connect(curSock, SIGNAL(disconnected()), this, SLOT(requestFinished()) );
    connect(curSock, SIGNAL(readyRead()), this, SLOT(answerRequest()) );
    QTimer::singleShot(0,this, SLOT(answerRequest()) );
  }
  checking = false;
}

void SysCacheDaemon::answerRequest(){
  //curSock->waitForReadyRead(1000); //wait max 1 second for the request from the client
  static bool working = false;
  if(working || curSock==0){ return; }
  working = true;
  QStringList req, out;
  bool stopdaemon=false;
  QTextStream stream(curSock);
  bool done = false;
  while(!stream.atEnd()){
    req = QString(stream.readLine()).split(" ");
    //qDebug() << " - Request:" << req;
    //qDebug() << "Request Received:" << req;
    if(req.join("")=="shutdowndaemon"){ stopdaemon=true; done=true; break; }
    if(req.join("")=="[FINISHED]"){ done = true; break; }
    else{ 
	
      QString res = DATA->fetchInfo(req);
      //For info not available, try once more time as it can error unexpectedly if it was 
	// stuck waiting for a sync to finish
      if(res =="[ERROR] Information not available"){ res = DATA->fetchInfo(req); }
      out << "[INFOSTART]"+ res;
    }
  }
  //Now write the output to the socket and disconnect it
  //qDebug() << " - Request replied:" << done;
  stream << out.join("\n");
  //curSock->disconnectFromServer();
  working = false;
  if(done){ stream << "\n[FINISHED]"; }
  else{ QTimer::singleShot(0,this, SLOT(answerRequest()) ); }
  if(stopdaemon){ QTimer::singleShot(10, this, SLOT(stopServer())); }
}

void SysCacheDaemon::requestFinished(){
  //qDebug() << " - Request Finished";
  curSock=0; //reset the internal pointer
  //Now look for the next request
  QTimer::singleShot(0,this, SLOT(checkForConnections()));
}