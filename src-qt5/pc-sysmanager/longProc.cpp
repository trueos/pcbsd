#include "longProc.h"


LongProc::LongProc(){
  //initialize the internal worker process
  process = new QProcess;
  process->setProcessChannelMode(QProcess::MergedChannels);
  connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(parseUpdate()) );
  connect(process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(procDone()) );
  running = false; stopped = false;
  cmdList.clear(); dirList.clear(); infoList.clear();
  timer = new QTimer();
	timer->setSingleShot(true);
	timer->setInterval(10000); //10 second ping to make sure user knows it is still running
  connect(timer, SIGNAL(timeout()), this, SLOT(procTimeout()) );
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
  timer->start();
  process->start(cmdList[currentItem]);
  running = true;
  return running;
}

void LongProc::stopProc(){
  stopped = true;
  process->terminate();
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
  timer->start(); //reset the timer
}

void LongProc::procDone(){
  //Start the next command if there is one
  bool success = (process->exitCode() == 0);
  if( currentItem+1 < cmdList.length() && success && !stopped){
    currentItem++;
    if( !infoList[currentItem].isEmpty() ){ emit ProcMessage("\n--- "+infoList[currentItem]+" ---"); }
    if( !dirList[currentItem].isEmpty() && QFile::exists(dirList[currentItem]) ){ process->setWorkingDirectory(dirList[currentItem]); }
    process->start(cmdList[currentItem]);
    timer->start(); //reset timer
  }else{
    //All finished
    if(stopped){
      emit ProcMessage("\n ---- KILLED ----");
      stopped=false; //reset flag
    }else if(success){
      emit ProcMessage("\n ---- FINISHED ----");
    }else{
      emit ProcMessage("\n ---- ERROR ----");
    }
    running = false;
    emit ProcFinished();
    timer->stop();
  }
}

void LongProc::procTimeout(){
  emit ProcMessage("."); //just to make sure that a quiet and long process still looks active to the user
  timer->start(); //restart the timer
}
