#include "syscache-client.h"

SysCacheClient::SysCacheClient(QObject *parent) : QObject(parent){
  curSock = new QLocalSocket(this);
    connect(curSock, SIGNAL(connected()), this, SLOT(startRequest()));
    connect(curSock, SIGNAL(error(QLocalSocket::LocalSocketError)), this, SLOT(connectionError()));
}

SysCacheClient::~SysCacheClient(){
}

void SysCacheClient::parseInputs(QStringList inputs){
  userRequest = inputs;
  //Convert the user request into server request formatting
  servRequest << inputs;
	
  //Now start the connection to the server
  curSock->connectToServer("/var/run/syscache.pipe", QIODevice::ReadWrite | QIODevice::Text);
}

void SysCacheClient::startRequest(){
  QTextStream out(curSock);
  out << servRequest.join("\n");
  connect(curSock, SIGNAL(readyRead()), this, SLOT(requestFinished()) );
}

void SysCacheClient::requestFinished(){
  //qDebug() << "Client Request Finished";
  QTextStream in(curSock);
  bool finished = false;
  while(!in.atEnd()){
    QString line = in.readLine();
    if(line=="[FINISHED]"){ finished=true; break;}
    else{ qDebug() << line; }
  }
  if(finished){
    curSock->disconnectFromServer();
    QCoreApplication::exit(0);
  }
}

void SysCacheClient::connectionError(){
  qDebug() << "Client Connection Error:";
  if(curSock->error()==QLocalSocket::PeerClosedError){
    //requestFinished();
  }else{
    qDebug() << "[ERROR]" << curSock->errorString();
    qDebug() << " - Is the syscache daemon running?";
    exit(1);
  }
}