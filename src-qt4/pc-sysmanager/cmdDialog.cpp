#include "cmdDialog.h"
#include "ui_cmdDialog.h"

CMDDialog::CMDDialog(QWidget *parent) : QDialog(parent), ui(new Ui::CMDDialog){
  ui->setupUi(this); //load the designer UI file
  //Setup the backend process class
  PROC = new LongProc;
    connect(PROC, SIGNAL(ProcMessage(QString)), this, SLOT(NewMessage(QString)) );
    connect(PROC, SIGNAL(ProcFinished()), this, SLOT(Finished()) );
  //Setup internal connections
  connect(ui->push_close, SIGNAL(clicked()), this, SLOT(CloseUI()) );
  connect(ui->push_stop, SIGNAL(clicked()), this, SLOT(stopProcess()) );
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
    this->setWindowTitle(tr("Getting PC-BSD Ports Tree"));
    //Check for if the port tree needs to be initialized first
    qDebug() << "WARNING: Ports init check still needs to be implemented";
    bool init = true;
    //Now create the commands necessary
    if(init){
      ui->textEdit->setPlainText("Starting Ports Tree Initialization....");
      if(QFile::exists("/usr/ports")){
        cmds << "rm -rf /usr/ports/* /usr/ports/.g*"; info << "Removing the old ports tree"; dirs << ""; //Clean the old ports tree
      }else{
        cmds << "mkdir /usr/ports"; info << "Creating the ports tree"; dirs << ""; //Create the ports tree
      }
      cmds << "git init"; info << "Initialize GIT"; dirs << "/usr/ports"; //setup git
      cmds << "git remote remove origin"; info << ""; dirs <<"/usr/ports";
      cmds << "git remote add origin https://www.github.com/pcbsd/freebsd-ports.git"; info << ""; dirs << "/usr/ports/.git"; //setup PC-BSD git repo
    }else{
      ui->textEdit->setPlainText("Updating the Ports Tree....");
    }
    //Now create the git update commands
    cmds << "git fetch --depth=1"; info << "Fetch new GIT info (this may take a while)"; dirs << "/usr/ports/.git";
    cmds << "git checkout master"; info << "Checkout the tree"; dirs << "/usr/ports"; 

  }else if(cmdtype.toLower() == "source"){
    this->setWindowTitle(tr("Getting PC-BSD Sources"));
    //Check for if the source tree needs to be initialized first
    qDebug() << "WARNING: Source init check still needs to be implemented";
    bool init = true;
    //Now create the commands necessary
    if(init){
      ui->textEdit->setPlainText("Starting Source Tree Initialization....");
      if(QFile::exists("/usr/src")){
	cmds << "rm -rf /usr/src/* /usr/src/.g*"; info << "Removing the old source tree"; dirs << ""; //Clean the old source tree
      }else{
        cmds << "mkdir /usr/src"; info << "Creating the source tree"; dirs << ""; //Create the source tree
      }
      cmds << "git init"; info << "Initialize GIT"; dirs << "/usr/src"; //setup git
      cmds << "git remote remove origin"; info << ""; dirs <<"/usr/src";
      cmds << "git remote add origin https://www.github.com/pcbsd/freebsd.git"; info << ""; dirs << "/usr/src/.git"; //setup PC-BSD git repo
    }else{
      ui->textEdit->setPlainText("Updating the Source Tree....");
    }
    //Now create the git update commands
    cmds << "git fetch --depth=1"; info << "Fetch new GIT info (this may take a while)"; dirs << "/usr/src/.git";
    cmds << "git checkout master"; info << "Checkout the right version of the tree"; dirs << "/usr/src"; 
  }else{
    ui->textEdit->setPlainText("Unknown CMD Type: "+cmdtype);
    ui->push_close->setEnabled(true);
    ui->push_stop->setEnabled(false);
    return;
  }
  PROC->startCMDs(cmds, dirs, info);
  ui->push_stop->setEnabled(PROC->isRunning());
}

bool CMDDialog::isRunning(){
  return PROC->isRunning();
}
// =====================
void CMDDialog::NewMessage(QString msg){
  if(!msg.isEmpty()){
    //if(!msg.startsWith("\n")){ msg.prepend("\n"); } //make sure it is on a new line
    ui->textEdit->appendPlainText(msg); //make sure it is on the next line each time	
  }
}

void CMDDialog::Finished(){
    ui->push_close->setEnabled(true); //enable the close button
    ui->push_stop->setEnabled(PROC->isRunning());
}

void CMDDialog::CloseUI(){
  this->close();
}

void CMDDialog::stopProcess(){
  PROC->stopProc();
  //PROC should emit the finished signal once done stopping
}

void CMDDialog::closeEvent(QCloseEvent *event){
  //Make sure the user does not close this window while a process is running
  event->ignore();
  if(!ui->push_close->isEnabled()){
    QMessageBox::warning(this, tr("Process Running"), tr("Please stop the current process before closing this window.") );
  }else{
    //Make sure this window is only hidden (pbsystemtab needs to make sure it always exists for checks);
    this->hide();
  }
}
