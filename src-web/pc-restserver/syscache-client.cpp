#include <stdio.h>
#include "syscache-client.h"
#include <unistd.h>

#define LINEBREAK QString("<LINEBREAK>")

SysCacheClient::SysCacheClient(QObject *parent) : QObject(parent){
  curSock = new QLocalSocket(this);
    connect(curSock, SIGNAL(connected()), this, SLOT(startRequest()));
    connect(curSock, SIGNAL(disconnected()), this, SLOT(requestFinished()) );
    connect(curSock, SIGNAL(error(QLocalSocket::LocalSocketError)), this, SLOT(connectionError()));
}

SysCacheClient::~SysCacheClient(){
}

QStringList SysCacheClient::parseInputs(QStringList inputs){
  userRequest = inputs;
  qDebug() << "Syscache Request:" << inputs;
  servRequest.clear();
  ans.clear();
  //Convert the user request into server request formatting
  servRequest << inputs;
  
  //Now start the connection to the server
  curSock->connectToServer("/var/run/syscache.pipe", QIODevice::ReadWrite | QIODevice::Text);
  QCoreApplication::processEvents();
  usleep(100);
  //Wait for the socket to connect to the server
  while(curSock->state() != QLocalSocket::ConnectedState){
    QCoreApplication::processEvents();
    usleep(100); //1/10 millisecond
  }
  qDebug() << "Syscache connected";
  //Now wait for the server to process the request and send a reply
  while(curSock->state() != QLocalSocket::UnconnectedState){
    QCoreApplication::processEvents();
    usleep(100); //1/10 millisecond
  }
  qDebug() << " - Syscache disconnected:" << ans;
  return ans;
}


void SysCacheClient::startRequest(){
  ans.clear();
  QTextStream out(curSock);
  out << servRequest.join("\n[/]\n");
  out << "\n[FINISHED]";
  connect(curSock, SIGNAL(readyRead()), this, SLOT(requestFinished()) );
}

void SysCacheClient::requestFinished(){
  static bool running = false;
  if(running){ return; } //already reading stream
  //qDebug() << "Client Request Finished";
  QTextStream in(curSock);
  bool finished = false;
  static QString line = "";
  running = true;
  while(!in.atEnd() && !finished){
    QString newline = in.readLine();
    newline.replace(LINEBREAK,"\n");
    if(newline=="[FINISHED]"){ finished=true; }
    if(newline.startsWith("[") && !line.isEmpty()){
     qDebug() << "syscache reply:" << line << newline;
     ans << line;
      line.clear();
      if(newline.startsWith("[INFOSTART]")){
	newline = newline.remove(0,11); //remove that internal flag 
      }
    }else if(newline.startsWith("[INFOSTART]") && line.isEmpty()){
	newline = newline.remove(0,11); //remove that internal flag 
    }
    line.append(newline);
  }
  running = false;
  if(finished){
    curSock->disconnectFromServer();
  }
}

void SysCacheClient::connectionError(){
  qDebug() << "Client Connection Error:";
  if(curSock->error()==QLocalSocket::PeerClosedError){
    //requestFinished();
  }else{
    qDebug() << "[ERROR]" << curSock->errorString();
    qDebug() << " - Is the syscache daemon running?";
  }
}
