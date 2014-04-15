//===========================================
//  Lumina-DE source code
//  Copyright (c) 2013, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#include "LuminaUtils.h"

int LUtils::runCmd(QString cmd, QStringList args){
  QProcess *proc = new QProcess;
  proc->setProcessChannelMode(QProcess::MergedChannels);
  if(args.isEmpty()){
    proc->start(cmd);
  }else{
    proc->start(cmd, args);
  }
  while(!proc->waitForFinished(300)){
    QCoreApplication::processEvents();
  }
  int ret = proc->exitCode();
  delete proc;
  return ret;
	
}

QStringList LUtils::getCmdOutput(QString cmd, QStringList args){
  QProcess *proc = new QProcess;
  proc->setProcessChannelMode(QProcess::MergedChannels);
  if(args.isEmpty()){
    proc->start(cmd);
  }else{
    proc->start(cmd,args);	  
  }
  while(!proc->waitForFinished(300)){
    QCoreApplication::processEvents();
  }
  QStringList out = QString(proc->readAllStandardOutput()).split("\n");
  delete proc;
  return out;	
}
