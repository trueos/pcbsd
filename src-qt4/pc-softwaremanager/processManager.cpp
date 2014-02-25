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
    //Initialize the UPDATE Process
    upProc = new DLProcess(this);
	upProc->setDLType("PBI");
    connect(upProc, SIGNAL(UpdateMessage(QString)), this, SLOT(slotUpProcMessage(QString)) );
    connect(upProc, SIGNAL(UpdatePercent(QString, QString, QString)), this, SLOT(slotUpProcStats(QString,QString, QString)) );
    connect(upProc, SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(slotUpProcFinished()) );
	
    //Initialize the REMOVE Process
    remProc = new DLProcess(this);
	//remProc->setDLType("PBI"); //Does not need download parsing - just use standard message output
    connect(remProc, SIGNAL(UpdateMessage(QString)), this, SLOT(slotRemProcMessage(QString)) );
    connect(remProc, SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(slotRemProcFinished()) );
	
    //Initialize the DOWNLOAD Process
    dlProc = new DLProcess(this);
	dlProc->setDLType("PBI");
    connect(dlProc, SIGNAL(UpdateMessage(QString)), this, SLOT(slotDlProcMessage(QString)) );
    connect(dlProc, SIGNAL(UpdatePercent(QString, QString, QString)), this, SLOT(slotDlProcStats(QString,QString, QString)) );
    connect(dlProc, SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(slotDlProcFinished()) );
	
    //Initialize the INSTALL Process
    inProc = new DLProcess(this);
	//inProc->setDLType("PBI"); //Does not need download parsing - just use standard message output
    connect(inProc, SIGNAL(UpdateMessage(QString)), this, SLOT(slotInProcMessage(QString)) );
    connect(inProc, SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(slotInProcFinished()) );
    
    //Initialize the OTHER Process
    otProc = new DLProcess(this);
	//otProc->setDLType("PBI"); //Does not need download parsing - just use standard message output
    connect(otProc, SIGNAL(UpdateMessage(QString)), this, SLOT(slotOtProcMessage(QString)) );
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
    if( cmd.contains("pbi_delete") ){ remStrictErrChecking = true; }
    else{ remStrictErrChecking=false; }
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
  /*DOWNLOAD NOTIFICATION CODES:
  Download complete: "DLDONE"
  Download running: "DLSTAT::<percent>::<total size>::<download speed>"
	-- A value of "??" means that it is unknown
  Download starting: "DLSTART"
  */

// == UPDATE PROCESS ==
void ProcessManager::slotUpProcMessage(QString msg){
  //Add the message to the log
  upLog << msg;
  
  //Now send it out
  emit ProcessMessage(UPDATE, msg);
}

void ProcessManager::slotUpProcStats(QString percent, QString size, QString speed){
  //Format the display string for output
  QString out = "DLSTAT::"+percent+"::"+size+"::"+speed;
  emit ProcessMessage(UPDATE, out);
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
void ProcessManager::slotRemProcMessage(QString msg){
  //Add the message to the log
  remLog << msg;
  //Now send it out
  emit ProcessMessage(REMOVE, msg);
}

void ProcessManager::slotRemProcFinished(){
  if(remProc->exitStatus() != QProcess::NormalExit || (remProc->exitCode() != 0 && remStrictErrChecking) ){
    qDebug() << "Removal Process Error Log:\n"<<remLog.join("\n");
    emit ProcessError(REMOVE, remLog);
  }else{
    qDebug() << "Removal Process Finished";
    emit ProcessFinished(REMOVE);	  
  }
}

// == DOWNLOAD PROCESS ==
void ProcessManager::slotDlProcMessage(QString msg){
  //Check for DL start/finish messages
  //Add the message to the log
  dlLog << msg;
  //Now send it out
  emit ProcessMessage(DOWNLOAD, msg);
}

void ProcessManager::slotDlProcStats(QString percent, QString size, QString speed){
  //Format the display string for output
  QString out = "DLSTAT::"+percent+"::"+size+"::"+speed;
  emit ProcessMessage(DOWNLOAD, out);
}

void ProcessManager::slotDlProcFinished(){
  if(dlProc->exitStatus() != QProcess::NormalExit || dlProc->exitCode() != 0){
    qDebug() << "Download Process Error Log:\n"<<dlLog.join("\n");
    emit ProcessError(DOWNLOAD, dlLog);
  }else{
    qDebug() << "Download Process Finished";
    emit ProcessFinished(DOWNLOAD);	  
  }
}

// == INSTALL PROCESS ==
void ProcessManager::slotInProcMessage(QString msg){
  //Add the message to the log
  inLog << msg;
  //Now send it out
  emit ProcessMessage(INSTALL, msg);
}

void ProcessManager::slotInProcFinished(){
  if(inProc->exitStatus() != QProcess::NormalExit || inProc->exitCode() != 0){
    qDebug() << "Install Process Error Log:\n"<<inLog.join("\n");
    emit ProcessError(INSTALL, inLog);
  }else{
    qDebug() << "Install Process Finished";
    emit ProcessFinished(INSTALL);	  
  }
}

// == OTHER PROCESS ==
void ProcessManager::slotOtProcMessage(QString msg){
  //send it out (no logging - this channel is for quick commands with almost no output)
  emit ProcessMessage(OTHER, msg);
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

