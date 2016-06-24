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

void CMDDialog::start(QString cmdtype, QString branch){
  if(PROC->isRunning()){
    qDebug() << "ERROR: Process is already working: please wait until it is finished";
    return;
  }
  ui->push_close->setEnabled(false); //make sure they cannot close this while it is running
  ui->textEdit->clear(); //make sure this is cleared
  QStringList cmds, dirs, info; //For generating the necessary input to the backend
  if(cmdtype.toLower() == "ports"){
    this->setWindowTitle(tr("Getting TrueOS Ports Tree"));
    Backend::generatePortsUpdateCMDS(cmds, dirs, info);
	  
  }else if(cmdtype.toLower() == "source"){
    this->setWindowTitle(tr("Getting TrueOS Sources"));
    Backend::generateSourceUpdateCMDS(branch, cmds, dirs, info);
	  
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
