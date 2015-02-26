#include "MainDaemon.h"

MainDaemon::MainDaemon(QObject *parent) : QObject(parent){
  curSock = 0; //no local socket connected yet
  server = new QLocalServer(this);
    //server->setMaxPendingConnections(10); //should go through them pretty fast, no need for more
    connect(server, SIGNAL(newConnection()), this, SLOT(checkForConnections()));
  SYSTEM = new Backend(this);
}

MainDaemon::~MainDaemon(){
}

//General Start/Stop functions
void MainDaemon::startServer(){
  //Clean up any stale pipes first
  QLocalServer::removeServer("/var/run/pc-sysconfig.pipe");
  //Now start this server
  if( server->listen("/var/run/pc-sysconfig.pipe") ){
    QFile::setPermissions("/var/run/pc-sysconfig.pipe", QFile::ReadUser | QFile::WriteUser | QFile::ReadGroup | QFile::WriteGroup | QFile::ReadOther | QFile::WriteOther);
    qDebug() << "Device Daemon now listening for connections at /var/run/pc-sysconfig.pipe";
  }else{
    qDebug() << "Error: Device Daemon could not create pipe at /var/run/pc-sysconfig.pipe";
    exit(1);
  }
}

void MainDaemon::stopServer(){
  if(server->isListening()){ server->close(); }
  QLocalServer::removeServer("/var/run/pc-sysconfig.pipe"); //clean up
  //DATA->shutDown();
  QCoreApplication::exit(0);
}

//Server/Client connections
void MainDaemon::checkForConnections(){
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

void MainDaemon::answerRequest(){
  //curSock->waitForReadyRead(1000); //wait max 1 second for the request from the client
  static bool working = false;
  if(working || curSock==0){ return; }
  working = true;
  QStringList req, out;
  bool stopdaemon=false;
  QTextStream stream(curSock);
  bool done = false;
  QString user, lang;
  lang = "en_US"; //default value
  while(!stream.atEnd()){
    req = QString(stream.readLine()).split(" ");
    //qDebug() << "Request Received:" << req;
    if(req.join("")=="shutdowndaemon"){ stopdaemon=true; done=true; break; }
    else if(req.join("")=="[FINISHED]"){ done = true; break; }
    else if(req.join("").startsWith("[USERNAME]")){ user = req.join("").section("]",1,1); }
    else if(req.join("").startsWith("[LANG]")){ lang = req.join("").section("]",1,1); }
    else{ 
      //qDebug() << "Client Request:" << user << lang << req;
      QString res = SYSTEM->runRequest(req, user, lang);
      //qDebug() << " - result:" << res;
      //For info not available, try once more time as it can error unexpectedly if it was 
	// stuck waiting for a sync to finish
      //if(res =="[ERROR] Information not available"){ res = DATA->fetchInfo(req); }
      out << "[INFOSTART]"+res;
    }
  }
  //Now write the output to the socket and disconnect it
  //qDebug() << " - Request replied:" << done;
  stream << out.join("\n");
  //curSock->disconnectFromServer();
  if(done){ stream << "\n[FINISHED]\n";}
  working = false;
  //else{ QTimer::singleShot(0,this, SLOT(answerRequest()) ); }
  if(stopdaemon){ QTimer::singleShot(10, this, SLOT(stopServer())); }
}

void MainDaemon::requestFinished(){
  //qDebug() << " - Request Finished";
  curSock=0; //reset the internal pointer
  //Now look for the next request
  QTimer::singleShot(0,this, SLOT(checkForConnections()));
}