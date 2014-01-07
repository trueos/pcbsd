#include "cmdDialog.h"
#include "ui_cmdDialog.h"

CMDDialog::CMDDialog(QWidget *parent) : QDialog(parent), ui(new Ui::CMDDialog){
  ui->setupUi(this); //load the designer UI file
  //Setup the backend process class
  PROC = new LongProc;
    connect(PROC, SIGNAL(ProcMessage(QString)), this, SLOT(NewMessage(QString)) );
    connect(PROC, SIGNAL(ProcFinished()), this, SLOT(Finished()) );
  //Setup internal flags
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
  ui->textEdit->clear(); //make sure this is cleared
  QStringList cmds, dirs, info; //For generating the necessary input to the backend
  if(cmdtype.toLower() == "ports"){
    //Check for if the port tree needs to be initialized first
    qDebug() << "WARNING: Ports init check still needs to be implemented";
    bool init = true;
    //Now create the commands necessary
    if(init){
      ui->textEdit->setPlainText("Starting Ports Tree Initialization....");
      if(QFile::exists("/usr/ports")){
        cmds << "rm -rf /usr/ports/*"; info << "Removing the old ports tree"; dirs << ""; //Clean the old ports tree
      }else{
	cmds << "mkdir /usr/ports"; info << "Creating the ports tree"; dirs << ""; //Create the source tree
      }
      cmds << "git init"; info << "Initialize GIT"; dirs << "/usr/ports"; //setup git
      cmds << "git remote add origin https://www.github.com/pcbsd/freebsd-ports.git"; info << ""; dirs << "/usr/ports/.git"; //setup PC-BSD git repo
    }else{
      ui->textEdit->setPlainText("Updating the Ports Tree....");
    }
    //Now create the git update commands
    cmds << "git fetch --depth=1"; info << "Fetch new GIT info"; dirs << "/usr/ports/.git";
    cmds << "git checkout -t origin/master"; info << "Checkout the tree"; dirs << "/usr/ports/.git"; 

  }else if(cmdtype.toLower() == "source"){
    //Check for if the source tree needs to be initialized first
    qDebug() << "WARNING: Source init check still needs to be implemented";
    bool init = true;
    //Now create the commands necessary
    if(init){
      ui->textEdit->setPlainText("Starting Source Tree Initialization....");
      if(QFile::exists("/usr/src")){
	cmds << "rm -rf /usr/src/*"; info << "Removing the old source tree"; dirs << ""; //Clean the old source tree
      }else{
	cmds << "mkdir /usr/src"; info << "Creating the source tree"; dirs << ""; //Create the source tree
      }
      cmds << "git init"; info << "Initialize GIT"; dirs << "/usr/src"; //setup git
      cmds << "git remote add origin https://www.github.com/pcbsd/freebsd.git"; info << ""; dirs << "/usr/src/.git"; //setup PC-BSD git repo
    }else{
      ui->textEdit->setPlainText("Updating the Source Tree....");
    }
    //Now create the git update commands
    cmds << "git fetch --depth=1"; info << "Fetch new GIT info"; dirs << "/usr/src/.git";
    cmds << "git checkout -t origin/master"; info << "Checkout the tree"; dirs << "/usr/src/.git"; 
  }else{
    ui->textEdit->setPlainText("Unknown CMD Type: "+cmdtype);
    ui->push_close->setEnabled(true);
    return;
  }
  PROC->startCMDs(cmds, dirs, info);
}

// =====================
void CMDDialog::NewMessage(QString msg){
  ui->textEdit->appendPlainText("\n"+msg); //make sure it is on the next line each time	
}

void CMDDialog::Finished(){
    ui->push_close->setEnabled(true); //enabled the close button
}

void CMDDialog::CloseUI(){
  this->close();
}
