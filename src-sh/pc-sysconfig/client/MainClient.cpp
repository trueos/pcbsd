#include <stdio.h>
#include "MainClient.h"

MainClient::MainClient(QObject *parent) : QObject(parent){
  curSock = new QLocalSocket(this);
    connect(curSock, SIGNAL(connected()), this, SLOT(startRequest()));
    connect(curSock, SIGNAL(disconnected()), this, SLOT(requestFinished()) );
    connect(curSock, SIGNAL(error(QLocalSocket::LocalSocketError)), this, SLOT(connectionError()));
}

MainClient::~MainClient(){
}

void MainClient::parseInputs(QStringList inputs){
  userRequest = inputs;
  if(inputs.isEmpty()){ showUsage(); }
  //Convert the user request into server request formatting
  servRequest << inputs;
  
  //Now start the connection to the server
  curSock->connectToServer("/var/run/pc-sysconfig.pipe", QIODevice::ReadWrite | QIODevice::Text);
}

void MainClient::startRequest(){
  QTextStream out(curSock);
  out << servRequest.join("\n");
  out << "\n[FINISHED]";
  connect(curSock, SIGNAL(readyRead()), this, SLOT(requestFinished()) );
}

void MainClient::requestFinished(){
  static bool running = false;
  if(running){ return; } //already reading stream
  //qDebug() << "Client Request Finished";
  QTextStream in(curSock);
  bool finished = false;
  static QString line = "";
  running = true;
  while(!in.atEnd()){
    QString newline = in.readLine();
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

void MainClient::connectionError(){
  qDebug() << "Client Connection Error:";
  if(curSock->error()==QLocalSocket::PeerClosedError){
    //requestFinished();
  }else{
    qDebug() << "[ERROR]" << curSock->errorString();
    qDebug() << " - Is the pc-sysconfig daemon running?";
  }
  exit(1);
}
