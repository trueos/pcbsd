#include "dispatcher-client.h"
#include <QFile>
#include <QCoreApplication>

#define DISPATCH QString("/usr/local/share/appcafe/dispatcher")
#define DISPATCHIDFILE QString("/var/tmp/appcafe/dispatch-id")
#define DISPATCHENVVAR QString("PHP_DISID")
#define RANDOMCHARS QString(

DispatcherClient::DispatcherClient(AuthorizationManager *auth, QObject *parent) : QProcess(parent){
    this->setProcessChannelMode(QProcess::MergedChannels);
    AUTH = auth;
}

DispatcherClient::~DispatcherClient(){
}

bool DispatcherClient::setupProcAuth(){
  //First check  that the dispatcher binary actually exists
  if(!QFile::exists(DISPATCH) || AUTH==0){ qWarning() << "AppCafe Dispatcher binary not found:"; return false; }
  //Now check the current authorization key
  QString key = ReadKey();
  if(!AUTH->checkAuth(key) ){
    //Key now invalid - generate a new one (this ensures that the secure key rotates on a regular basis)
    key = AUTH->LoginService(true, "dispatcher");
    //Save the auth key to the file and lock it down
    if(!WriteKey(key)){ 
      qWarning() << "Could not save dispatcher authorization key: **No dispatcher availability**. ";
      AUTH->clearAuth(key); 
      return false; 
    }
  }
  //Now put that key into the process environment for the dispatcher to see/verify
  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("LANG", "C");
    env.insert("LC_ALL", "C");
    env.insert(DISPATCHENVVAR, key);
  this->setProcessEnvironment(env);
  return true;
}

QString DispatcherClient::GetProcOutput(QString args){
  this->start(DISPATCH+" "+args);
  if(!this->waitForStarted(5000)){ return ""; } //process never started - max wait of 5 seconds
  while(!this->waitForFinished(1000)){
    if(this->state() != QProcess::Running){ break; } //somehow missed the finished signal
    QCoreApplication::processEvents();
  }
  return QString(this->readAllStandardOutput());
}

QStringList DispatcherClient::parseInputs(QStringList inputs, AuthorizationManager *auth){
  DispatcherClient client(auth);
  if(!client.setupProcAuth()){ return QStringList(); } //unauthorized
  QStringList outputs;
  for(int i=0; i<inputs.length(); i++){
    outputs << client.GetProcOutput(inputs[i]);
  }
  return outputs;
}

QString DispatcherClient::ReadKey(){
  QFile file(DISPATCHIDFILE);
  if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){ return ""; }
  QString key;
  QTextStream in(&file);
    key = in.readAll();
  file.close();
  return key;
}

bool DispatcherClient::WriteKey(QString key){
  QFile file(DISPATCHIDFILE);
  if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){ return false; }
  QTextStream out(&file);
   out << key;
  file.close();
  //Now lock down the file (root only read/write)
  if(!file.setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner)){
    //Could not lock down the file - this is insecure and should not be used
    file.remove();
    return false;
  }
  return true;
}
