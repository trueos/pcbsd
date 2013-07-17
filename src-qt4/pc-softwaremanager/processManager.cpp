/***************************************************************************
 *   Copyright (C) 2011 - iXsystems				 	  *
 *   kris@pcbsd.org  *
 *   tim@pcbsd.org   *
 *   ken@pcbsd.org   *
 *                                                                         *
 *   Permission is hereby granted, free of charge, to any person obtaining *
 *   a copy of this software and associated documentation files (the       *
 *   "Software"), to deal in the Software without restriction, including   *
 *   without limitation the rights to use, copy, modify, merge, publish,   *
 *   distribute, sublicense, and/or sell copies of the Software, and to    *
 *   permit persons to whom the Software is furnished to do so, subject to *
 *   the following conditions:                                             *
 *                                                                         *
 *   The above copyright notice and this permission notice shall be        *
 *   included in all copies or substantial portions of the Software.       *
 *                                                                         *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       *
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    *
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. *
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR     *
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, *
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR *
 *   OTHER DEALINGS IN THE SOFTWARE.                                       *
 ***************************************************************************/
 #include "processManager.h"

ProcessManager::ProcessManager(){
  //Get the system environment for all the processes
  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("PBI_FETCH_PARSING","YES"); //For readable download notifications
    //Initialize the UPDATE Process
    upProc = new QProcess; upProc->setProcessEnvironment(env);
    upProc->setProcessChannelMode(QProcess::MergedChannels);
    connect(upProc, SIGNAL(readyRead()),this,SLOT(slotUpProcMessage()) );
    connect(upProc, SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(slotUpProcFinished()) );
    //Initialize the REMOVE Process
    remProc = new QProcess; remProc->setProcessEnvironment(env);
    remProc->setProcessChannelMode(QProcess::MergedChannels);
    connect(remProc, SIGNAL(readyRead()),this,SLOT(slotRemProcMessage()) );
    connect(remProc, SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(slotRemProcFinished()) );
    //Initialize the DOWNLOAD Process
    dlProc = new QProcess; dlProc->setProcessEnvironment(env);
    dlProc->setProcessChannelMode(QProcess::MergedChannels);
    connect(dlProc, SIGNAL(readyRead()),this,SLOT(slotDlProcMessage()) );
    connect(dlProc, SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(slotDlProcFinished()) );
    //Initialize the INSTALL Process
    inProc = new QProcess; inProc->setProcessEnvironment(env);
    inProc->setProcessChannelMode(QProcess::MergedChannels);
    connect(inProc, SIGNAL(readyRead()),this,SLOT(slotInProcMessage()) );
    connect(inProc, SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(slotInProcFinished()) );
    //Initialize the OTHER Process
    otProc = new QProcess; otProc->setProcessEnvironment(env);
    connect(otProc, SIGNAL(readyReadStandardOutput()),this,SLOT(slotOtProcMessage()) );
    connect(otProc, SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(slotOtProcFinished()) );
}

ProcessManager::~ProcessManager(){
}

void ProcessManager::goToDirectory(ProcessID ID, QString dir){
  if( ID == UPDATE ){
    upProc->setWorkingDirectory(dir);	  
  }else if( ID == REMOVE ){
    remProc->setWorkingDirectory(dir);	   	  
  }else if( ID == DOWNLOAD ){
    dlProc->setWorkingDirectory(dir);  	  
  }else if( ID == INSTALL ){
    inProc->setWorkingDirectory(dir);   	  
  }else if( ID == OTHER ){
    otProc->setWorkingDirectory(dir);   	  
  }	
}
// =========================
// ===== PUBLIC ACCESS =====
// =========================
void ProcessManager::startProcess(ProcessID ID, QString cmd){
  if( ID == UPDATE ){
    qDebug() << "Update Process Started:" << cmd;
    upLog.clear(); //clear the log for action
    upProc->start(cmd);	  
  }else if( ID == REMOVE ){
    qDebug() << "Removal Process Started:" << cmd;
    remLog.clear();
    remProc->start(cmd);	   	  
  }else if( ID == DOWNLOAD ){
    qDebug() << "Download Process Started:" << cmd;
    dlLog.clear();
    dlProc->start(cmd);	   	  
  }else if( ID == INSTALL ){
    qDebug() << "Install Process Started:" << cmd;
    inLog.clear();
    inProc->start(cmd);	   	  
  }else if( ID == OTHER ){
    qDebug() << "Other Process Started:" << cmd;
    otProc->start(cmd);	   	  
  }
	
}

void ProcessManager::stopProcess(ProcessID ID){
  if((ID == ALL) || (ID == UPDATE)){
    upProc->terminate();	  
  }
  if((ID == ALL) || (ID == REMOVE)){
    remProc->terminate();	  
  }
  if((ID == ALL) || (ID == DOWNLOAD)){
    dlProc->terminate();	  
  }
  if((ID == ALL) || (ID == INSTALL)){
    inProc->terminate();	  
  }
  if((ID == ALL) || (ID == OTHER)){
    otProc->terminate();	  
  }	
}

QStringList ProcessManager::getProcessLog(ProcessID ID){
  if( ID == UPDATE ){ return upLog; }
  else if( ID == REMOVE ){ return remLog; }
  else if( ID == DOWNLOAD ){ return dlLog; }
  else if( ID == INSTALL ){ return inLog; }
  else{ return QStringList(); }
}

// =========================
// ===== PRIVATE SLOTS =====
// =========================
QString ProcessManager::parseDlLine(QString line){
  QString out;
  if(!line.startsWith("SIZE:")){ return out; }
  //qDebug() << "parse Download Line:" << line;
  //Line format: SIZE:  <KB> DOWNLOADED:  <KB> SPEED:  <KB/s> KB/s
  line = line.simplified();
  line.replace("SIZE: ","");
  line.replace("DOWNLOADED: ", "");
  line.replace("SPEED: ","");
  line.replace("KB/s","");
  bool totok, curok, spdok;
  double tot, cur, spd;
  tot = line.section(" ",0,0).toDouble(&totok);
  cur = line.section(" ",1,1).toDouble(&curok);
  spd = line.section(" ",2,2).toDouble(&spdok);
  //qDebug() << " - DownloadStats:" << tot << cur << spd;
  if(totok && tot==0){totok=FALSE;}
  if(curok && cur==0){curok=FALSE;}
  if(spdok && spd==0){spdok=FALSE;}
  //Now format the output string
  QString stats;
  if(totok && curok){
    bool totErr = (tot==cur); //catch for a display error where the cur is always identical to the tot
    int i=0;
    QStringList lab; lab << "KB" <<"MB"<<"GB"<<"TB"<<"PB";
    while( (tot>1000) && (i<lab.length()) ){
      cur=cur/1024; tot=tot/1024; i++;
    }
    float percent = (cur*100)/tot;
    //round all numbers to one decimel place
    percent = int(percent*10)/10.0;
    cur = int(cur*10)/10.0;
    tot = int(tot*10)/10.0;
    if(totErr){ // cur==tot 
      stats = QString::number(tot)+" "+lab[i];
    }else{
      stats = QString::number(cur)+"/"+QString::number(tot)+" "+lab[i]+" ("+QString::number(percent)+"%)";
    }
    // Format:  <current>/<total> <size label> (<percent>%)
  }else if(curok){
    stats = Extras::sizeKToDisplay(QString::number(cur));
  }
  QString speed;
  if(spdok){
    speed = Extras::sizeKToDisplay(QString::number(spd))+"/s";	  
  }
  //Now put the output string together and return it
  if(stats.isEmpty() && speed.isEmpty()){}
  else if(stats.isEmpty()){ out = speed; }
  else if(speed.isEmpty()){ out = stats; }
  else{ out = QString( tr("%1 at %2") ).arg(stats,speed); }
  //qDebug() << " - Result:" << out;
  return out;
}

// == UPDATE PROCESS ==
void ProcessManager::slotUpProcMessage(){
  while( upProc->canReadLine() ){
    QString line = upProc->readLine().simplified();
    if(line.isEmpty()){ continue; }
    // Change status back to "Updating..."
    emit ProcessMessage(UPDATE,QString());
    QString dl = parseDlLine(line);
    if(!dl.isEmpty()){ emit ProcessMessage(UPDATE,dl); }
    else{ upLog << line; } //not a download line - add to the log
  }
}

void ProcessManager::slotUpProcFinished(){
  if(upProc->exitStatus() != QProcess::NormalExit || upProc->exitCode() != 0){
    //Emit the command log
    qDebug() << "Update Process Error Log:\n"<<upLog.join("\n");
    emit ProcessError(UPDATE, upLog);
  }else{
    qDebug() << "Update Process Finished";
    emit ProcessFinished(UPDATE);	  
  }
}

// == REMOVE PROCESS ==
void ProcessManager::slotRemProcMessage(){
  while( remProc->canReadLine() ){
    QString line = remProc->readLine().simplified();
    if(!line.isEmpty()){ 
      remLog << line; 
      emit ProcessMessage(REMOVE,line);
    }
  }
}

void ProcessManager::slotRemProcFinished(){
  if(remProc->exitStatus() != QProcess::NormalExit){
    qDebug() << "Removal Process Error Log:\n"<<remLog.join("\n");
    emit ProcessError(REMOVE, remLog);
  }else{
    qDebug() << "Removal Process Finished";
    emit ProcessFinished(REMOVE);	  
  }
}

// == DOWNLOAD PROCESS ==
void ProcessManager::slotDlProcMessage(){
  while( dlProc->canReadLine() ){
    QString line = dlProc->readLine().simplified();
    if(line.isEmpty()){ continue; }
    QString dl = parseDlLine(line);
    if(!dl.isEmpty()){ emit ProcessMessage(DOWNLOAD,dl); }
    else{ dlLog << line; } //not a download line - add to the log
  }
}

void ProcessManager::slotDlProcFinished(){
  if(dlProc->exitStatus() != QProcess::NormalExit){
    qDebug() << "Download Process Error Log:\n"<<dlLog.join("\n");
    emit ProcessError(DOWNLOAD, dlLog);
  }else{
    qDebug() << "Download Process Finished";
    emit ProcessFinished(DOWNLOAD);	  
  }
}

// == INSTALL PROCESS ==
void ProcessManager::slotInProcMessage(){
  while( inProc->canReadLine() ){
    QString line = inProc->readLine().simplified();
    if(!line.isEmpty()){ 
      inLog << line; 
      emit ProcessMessage(INSTALL,line);
    }
  }
}

void ProcessManager::slotInProcFinished(){
  if(inProc->exitStatus() != QProcess::NormalExit){
    qDebug() << "Install Process Error Log:\n"<<inLog.join("\n");
    emit ProcessError(INSTALL, inLog);
  }else{
    qDebug() << "Install Process Finished";
    emit ProcessFinished(INSTALL);	  
  }
}

// == OTHER PROCESS ==
void ProcessManager::slotOtProcMessage(){
  QString msg = otProc->readAllStandardOutput();
  emit ProcessMessage(OTHER,msg);
}

void ProcessManager::slotOtProcFinished(){
  if(otProc->exitStatus() != QProcess::NormalExit){
    QString msg = otProc->readAllStandardError();
    if(msg.isEmpty()){ msg = otProc->readAllStandardOutput(); }
    if(msg.isEmpty()){ msg = tr("Unknown Error"); }
    qDebug() << "Other Process Error:"<<msg;
    emit ProcessError(OTHER, QStringList(msg));
  }else{
    qDebug() << "Other Process Finished";
    emit ProcessFinished(OTHER);	  
  }
}

