#include <stdio.h>
#include "syscache-client.h"

#define LINEBREAK QString("<LINEBREAK>")

SysCacheClient::SysCacheClient(QObject *parent) : QObject(parent){
  curSock = new QLocalSocket(this);
    connect(curSock, SIGNAL(connected()), this, SLOT(startRequest()));
    connect(curSock, SIGNAL(disconnected()), this, SLOT(requestFinished()) );
    connect(curSock, SIGNAL(error(QLocalSocket::LocalSocketError)), this, SLOT(connectionError()));
}

SysCacheClient::~SysCacheClient(){
}

void SysCacheClient::parseInputs(QStringList inputs){
  userRequest = inputs;
  if(inputs.isEmpty()){ showUsage(); }
  //Convert the user request into server request formatting
  servRequest << inputs;
  
  //Now start the connection to the server
  curSock->connectToServer("/var/run/syscache.pipe", QIODevice::ReadWrite | QIODevice::Text);
}

void SysCacheClient::showUsage(){
  qDebug() << "[ERROR] Invalid Inputs: (usage text not written yet)";

  exit(1);	
}

void SysCacheClient::startRequest(){
  QTextStream out(curSock);
  out << servRequest.join("\n");
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
  while(!in.atEnd()){
    QString newline = in.readLine();
    newline.replace(LINEBREAK,"\n");
    if(newline.startsWith("[") && !line.isEmpty()){
      fprintf(stdout, "%s\n", qPrintable(line) );
      line.clear();
      if(newline.startsWith("[INFOSTART]")){
	newline = newline.remove(0,11); //remove that internal flag 
      }
    }else if(newline.startsWith("[INFOSTART]") && line.isEmpty()){
	newline = newline.remove(0,11); //remove that internal flag 
    }
    line.append(newline);
    if(line=="[FINISHED]"){ finished=true; break;}
  }
  running = false;
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
  }
  exit(1);
}
