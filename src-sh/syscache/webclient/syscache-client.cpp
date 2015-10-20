#include <stdio.h>
#include "syscache-client.h"
#include <unistd.h>

#define LINEBREAK QString("<LINEBREAK>")

#define DEBUG 1
SysCacheClient::SysCacheClient(QObject *parent) : QLocalSocket(parent){
    connect(this, SIGNAL(connected()), this, SLOT(startRequest()));
    connect(this, SIGNAL(error(QLocalSocket::LocalSocketError)), this, SLOT(connectionError()));
    connect(this, SIGNAL(readyRead()), this, SLOT(requestFinished()) );
}

SysCacheClient::~SysCacheClient(){
}

QStringList SysCacheClient::parseInputs(QStringList inputs){
  SysCacheClient client;
  client.userRequest = inputs;
  client.servRequest = inputs;
  if(DEBUG){ qDebug() << "Syscache Request:" << inputs; }
  //Convert the user request into server request formatting

  
  //Now start the connection to the server
  client.connectToServer("/var/run/syscache.pipe", QIODevice::ReadWrite | QIODevice::Text);
  QCoreApplication::processEvents();
  usleep(100);
  //Wait for the socket to connect to the server
  while(client.state() != QLocalSocket::ConnectedState){
    QCoreApplication::processEvents();
  }
  //qDebug() << "Syscache connected";
  //Now wait for the server to process the request and send a reply
  while( client.state() != QLocalSocket::UnconnectedState && client.isValid() && client.ans.isEmpty()){
    QCoreApplication::processEvents();
  }
  //qDebug() << " - Syscache disconnected:" << client.ans;
  return client.ans;
}


void SysCacheClient::startRequest(){
  ans.clear();
  QTextStream out(this);
    servRequest.prepend("[NONCLI]"); //put the special non-CLI client flag in place
  out << servRequest.join("\n[/]\n");
  out << "\n[FINISHED]";
  
}

void SysCacheClient::requestFinished(){
  static bool running = false;
  if(running){ return; } //already reading stream
  running = true;
  if(DEBUG){ qDebug() << "Client Request Finished"; }
  QTextStream in(this);
  QString line;
  while(!line.endsWith("[FINISHED]")){
    line.append(in.readLine());
    QCoreApplication::processEvents();
  }
  line.remove("[FINISHED]");
  if(DEBUG){ qDebug() << "Reply:" << line; }
  QStringList output = line.split("[INFOSTART]");
    output.removeAll("[/]");
    output.removeAll("");
  if(DEBUG){ qDebug() << " - In List:" << output; }
      ans = output; //save it for later
  running = false;
  //qDebug() << " - Syscache connection closing" << ans;
  this->disconnectFromServer();
}

void SysCacheClient::connectionError(){
  qDebug() << "Client Connection Error:";
  if(this->error()==QLocalSocket::PeerClosedError){
    //requestFinished();
  }else{
    qDebug() << "[ERROR]" << this->errorString();
    qDebug() << " - Is the syscache daemon running?";
  }
}
