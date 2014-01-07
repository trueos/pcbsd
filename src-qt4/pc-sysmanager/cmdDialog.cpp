#include "cmdDialog.h"
#include "ui_cmdDialog.h"

CMDDialog::CMDDialog(QWidget *parent) : QDialog(parent), ui(new Ui::CMDDialog){
  ui->setupUi(this); //load the designer UI file
  //Setup the backend process class
  PROC = new gitProc;
    connect(PROC, SIGNAL(ProcMessage(QString)), this, SLOT(NewMessage(QString)) );
    connect(PROC, SIGNAL(procFinished(bool)), this, SLOT(Finished(bool)) );
  //Setup internal flags
  initPorts = false; initSource = false; //make sure these are both initially disabled
  connect(ui->push_close, SIGNAL(clicked()), this, SLOT(CloseUI()) );	
}

CMDDialog::~CMDDialog(){
  delete PROC;	
}

void CMDDialog::start(QString cmdtype){
  if(PROC->isRunning()){
    qDebug() << "ERROR: Process is already working: please wait until it is finished";
    return;
  }
  ui->push_close->setEnabled(false); //make sure they cannot close this while it is running
  if(cmdtype.toLower() == "ports"){
    //Check for if the port tree needs to be initialized first
    qDebug() << "WARNING: Ports init check still needs to be implemented";
    bool init = true;
    if(init){
      initPorts = true; //set the internal flag
      ui->textEdit->setPlainText("Starting Ports Tree Initialization....");
      bool ok = PROC->startInitPorts();
      if(!ok){ 
	ui->textEdit->appendPlainText("\nERROR: Could not clean and git init /usr/ports");
	ui->push_close->setEnabled(true); //they can now close the window
      }
    }else{
      ui->textEdit->setPlainText("Fetching the Ports Tree....");
      bool ok = PROC->startPorts();
      if(!ok){ 
	ui->textEdit->appendPlainText("\nERROR: Could not setup git fetch within /usr/ports");
	ui->push_close->setEnabled(true); //they can now close the window
      }
    }
  }else if(cmdtype.toLower() == "source"){
    //Check for if the source tree needs to be initialized first
    qDebug() << "WARNING: Source init check still needs to be implemented";
    bool init = true;
    if(init){
      initSource = true; //set the internal flag
      ui->textEdit->setPlainText("Starting Source Tree Initialization....");
      bool ok = PROC->startInitSource();
      if(!ok){ 
	ui->textEdit->appendPlainText("\nERROR: Could not clean and git init /usr/src");
	ui->push_close->setEnabled(true); //they can now close the window
      }
    }else{
      ui->textEdit->setPlainText("Fetching the Source Tree....");
      bool ok = PROC->startSource();
      if(!ok){ 
	ui->textEdit->appendPlainText("\nERROR: Could not setup git fetch within /usr/src");
	ui->push_close->setEnabled(true); //they can now close the window
      }
    }
  }else{
    ui->textEdit->setPlainText("Unknown CMD Type: "+cmdtype);
    ui->push_close->setEnabled(true);
  }
}

// =====================
void CMDDialog::NewMessage(QString msg){
  ui->textEdit->appendPlainText("\n"+msg); //make sure it is on the next line each time	
}

void CMDDialog::Finished(bool ok){
  if(!ok){
    ui->textEdit->appendPlainText("\n=============\nProcess Completed Unsuccessfully");
    ui->push_close->setEnabled(true); //enabled the close button
  }else if(initPorts){
    initPorts = false; //make sure it doesn't fall into this catch again
    //ports tree initialized successfully - start the fetch process
    ui->textEdit->appendPlainText("\n=============\nFetching the Ports Tree....");
    bool ok = PROC->startPorts();
    if(!ok){ 
      ui->textEdit->appendPlainText("\nERROR: Could not setup git fetch within /usr/ports");
      ui->push_close->setEnabled(true); //they can now close the window
    }
  }else if(initSource){
    initSource = false; //make sure it doesn't fall into this catch again
    //Source tree initialized successfully - start the fetch process
    ui->textEdit->appendPlainText("\n=============\nFetching the Source Tree....");
    bool ok = PROC->startPorts();
    if(!ok){ 
      ui->textEdit->appendPlainText("\nERROR: Could not setup git fetch within /usr/src");
      ui->push_close->setEnabled(true); //they can now close the window
    }
  }else{
    ui->textEdit->appendPlainText("\n=============\nFinished Successfully");
  }
}

void CMDDialog::CloseUI(){
  this->close();
}
