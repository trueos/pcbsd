#include <stdio.h>
#include "MainClient.h"
#include <QCoreApplication>
#include <unistd.h> //for getlogin()

MainClient::MainClient(QObject *parent) : QObject(parent){
  curSock = new QLocalSocket(this);
    connect(curSock, SIGNAL(connected()), this, SLOT(startRequest()));
    connect(curSock, SIGNAL(disconnected()), this, SLOT(requestFinished()) );
    connect(curSock, SIGNAL(error(QLocalSocket::LocalSocketError)), this, SLOT(connectionError()));
}

MainClient::~MainClient(){
}

bool MainClient::parseInputs(QStringList inputs){
  userRequest = inputs;
  if(inputs.isEmpty() || inputs.contains("-h") || inputs.contains("--help") ){ ShowUsage(); return false;}
  //Convert the user request into server request formatting
  servRequest << inputs;
  
  //Now start the connection to the server
  curSock->connectToServer("/var/run/pc-sysconfig.pipe", QIODevice::ReadWrite | QIODevice::Text);
  return true;
}

void MainClient::ShowUsage(){
  qDebug() << "pc-sysconfig: Simple system configuration utility";
  qDebug() << "Usage: \"pc-sysconfig <command 1> <command 2> ...\"";
  qDebug() << "Available Commands:";
  qDebug() << " \"listremdev\": List all removable devices attached to the system.";
  qDebug() << " \"listmounteddev\": List all removable devices that are currently mounted";
  qDebug() << " \"supportedfilesystems\": List all the filesystems that are currently detected/supported by pc-sysconfig";
  qDebug() << " \"devinfo <device> [skiplabel]\": Fetch device information (Filesystem, Label, Type)";
}

void MainClient::startRequest(){
  QString username = QString(getlogin());
  QString locale = QString(getenv("LANG")).section(".",0,0);
  if(locale.isEmpty()){ locale = QString(getenv("LC_ALL")).section(".",0,0); }
  QTextStream out(curSock);
  out << "[USERNAME]"+username+"\n";
  out << "[LANG]"+locale+"\n";
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
    //qDebug() << "newline:" << newline << "line:" << line;
    if(newline.startsWith("[") && !line.isEmpty()){
      //qDebug() << "Printed Line:" << line;
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
