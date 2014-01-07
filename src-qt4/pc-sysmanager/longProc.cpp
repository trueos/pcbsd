#include "longProc.h"


LongProc::LongProc(){
  //initialize the internal worker process
  process = new QProcess;
  process->setProcessChannelMode(QProcess::MergedChannels);
  connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(parseUpdate()) );
  connect(process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(procDone()) );
  running = false;
  cmdList.clear(); dirList.clear(); infoList.clear();
}

LongProc::~LongProc(){
  //Clean up the internal worker process
  delete process;
}
	
// ========================
//     PUBLIC WRAPPER FUNCTIONS
// ========================
bool LongProc::startCMDs(QStringList cmds, QStringList dirs, QStringList info){
  //Check that it is not already running
  if(running){ return false; }
  //Now check input lengths
  if( (cmds.length() != dirs.length()) || (cmds.length() != info.length()) && !cmds.isEmpty() ){
    qDebug() << "Invalid input lengths: all three inputs must have the same number of strings";
    return false;
  }
  //Now save the inputs internally
  cmdList = cmds; dirList = dirs; infoList = info;
  //Now get the first command running
  currentItem = 0;
  if( !infoList[currentItem].isEmpty() ){ emit ProcMessage("--- "+infoList[currentItem]+" ---"); }
  if( !dirList[currentItem].isEmpty() && QFile::exists(dirList[currentItem]) ){ process->setWorkingDirectory(dirList[currentItem]); }
  process->start(cmdList[currentItem]);
  running = true;
  return running;
}

// ====================
//   STATIC PUBLIC FUNCTIONS
// ====================
bool LongProc::quickCMD(QString dir, QString cmd, QStringList args){
  //Run a quick command without looking for output other than success/failure
  QProcess *proc = new QProcess;
  if( !dir.isEmpty() && QFile::exists(dir) ){ proc->setWorkingDirectory(dir); }
  if(args.isEmpty()){ proc->start(cmd); }
  else{ proc->start(cmd, args); }
  while(!proc->waitForFinished(300)){ QCoreApplication::processEvents(); }
  return (proc->exitCode() == 0);
}

// ==============
//     PRIVATE SLOTS
// ==============
void LongProc::parseUpdate(){
  while(process->canReadLine()){
    QString output = process->readLine().simplified();
    //could add a check here to not send the message for empty lines
    emit ProcMessage(output);
  }
}

void LongProc::procDone(){
  //Start the next command if there is one
  bool success = (process->exitCode() == 0);
  if( currentItem+1 < cmdList.length() && success){
    currentItem++;
    if( !infoList[currentItem].isEmpty() ){ emit ProcMessage("\n--- "+infoList[currentItem]+" ---"); }
    if( !dirList[currentItem].isEmpty() && QFile::exists(dirList[currentItem]) ){ process->setWorkingDirectory(dirList[currentItem]); }
    process->start(cmdList[currentItem]);
  }else{
    //All finished
    if(success){
      emit ProcMessage(" ---- FINISHED ----");
    }else{
      emit ProcMessage(" ---- ERROR ----");
    }
    running = false;
    emit ProcFinished();
  }
}
