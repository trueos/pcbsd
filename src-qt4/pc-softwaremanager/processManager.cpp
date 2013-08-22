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
  /*DOWNLOAD NOTIFICATION CODES:
  Download complete: "DLDONE"
  Download running: "DLSTAT::<percent>::<total size>::<download speed>"
	-- A value of "??" means that it is unknown
  Download starting: "DLSTART"
  */
  QString out;
  if( line.startsWith("FETCH:") ){ return "DLSTART"; }
  else if( line == "FETCHDONE"){ return "DLDONE"; }
  else if(!line.startsWith("SIZE:")){ return out; }
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
  out = "DLSTAT::";
  //Get percent and totals
  if(totok && curok){
    bool totErr = (tot==cur); //catch for a display error where the cur is always identical to the tot
    if(!totErr){	  
      //Calculate the percentage
      double percent = (cur/tot)*100;
      percent = int(percent*10)/10.0;
      out.append(QString::number(percent)+"::");
      //Now list the total
      out.append( Extras::sizeKToDisplay(QString::number(tot)) +"::" );
    }else{
      //Only Total/Current is known (unknown percentage since not complete yet)
      out.append("??::"+Extras::sizeKToDisplay(QString::number(tot))+"::");
    }	    
  }else if(curok){
    //Only Total/Current is known (unknown percentage since not complete yet)
    out.append("??::"+Extras::sizeKToDisplay(QString::number(cur))+"::");
  }else if(totok){
    //Only Total/Current is known (unknown percentage since not complete yet)
    out.append("??::"+Extras::sizeKToDisplay(QString::number(tot))+"::");
  }else{
    //Unknown Total and Current
    out.append("??::??::");
  }
  //Now get the speed
  if(spdok){
    out.append( Extras::sizeKToDisplay(QString::number(spd))+"/s" );
  }else{
    out.append("??");
  }
  return out;
}

// == UPDATE PROCESS ==
void ProcessManager::slotUpProcMessage(){
  while( upProc->canReadLine() ){
    QString line = upProc->readLine().simplified();
    if(line.isEmpty()){ continue; }
    QString dl = parseDlLine(line);
    if(!dl.isEmpty()){ 
      emit ProcessMessage(UPDATE,dl); //Download status
      if( !dl.startsWith("DLSTAT::") ){
	upLog << line; //not just a status update - add to the log (log download start/stop)    
      }
    }else{ 
      emit ProcessMessage(UPDATE,line); 
      upLog << line; //not a download line - add to the log
    }
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
  if(remProc->exitStatus() != QProcess::NormalExit || remProc->exitCode() != 0){
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
    if(!dl.isEmpty()){ 
      emit ProcessMessage(DOWNLOAD,dl); //Download status
      if( !dl.startsWith("DLSTAT::") ){
	dlLog << line; //not just a status update - add to the log (log download start/stop)    
      }
    }else{ 
      emit ProcessMessage(DOWNLOAD,line); 
      dlLog << line; //not a download line - add to the log
    }
  }
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
  if(inProc->exitStatus() != QProcess::NormalExit || inProc->exitCode() != 0){
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
  if(otProc->exitStatus() != QProcess::NormalExit || otProc->exitCode() != 0){
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

