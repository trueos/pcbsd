#include "updateDialog.h"
#include "ui_updateDialog.h"

UpdateDialog::UpdateDialog(QWidget* parent, QString jailID) : QDialog(parent), ui(new Ui::UpdateDialog){
  ui->setupUi(this); //load the designer file
  rebooting = false; //output variable
	
  //Setup the process
  proc = new DLProcess(this);
	proc->setParentWidget(this);
	proc->setDLType("PKG"); //pkg download message system

  //Initial UI setup
  //ui->text_log->setVisible(false);
  //ui->check_viewlog->setChecked(false);
  //ui->frame->setVisible(false);
  ui->progressBar->setVisible(false);
	
  //Connect the signals/slots
  connect(proc, SIGNAL(UpdateMessage(QString)), this, SLOT(procMessage(QString)) );
  connect(proc, SIGNAL(UpdatePercent(QString, QString, QString)), this, SLOT(procUpdate(QString,QString,QString)) );
  connect(proc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(procFinished(int, QProcess::ExitStatus)) );
  connect(ui->push_done, SIGNAL(clicked()), this, SLOT(closeDialog()) );
  connect(ui->push_reboot, SIGNAL(clicked()), this, SLOT(rebootsystem()) );
  //connect(ui->check_viewlog, SIGNAL(clicked()), this, SLOT(logview()) );
  //Hide the close buttons
  ui->push_done->setVisible(false);
  ui->push_reboot->setVisible(false);
  installphase=false;
  //Now start the process
  if(jailID.isEmpty()){
    proc->start("pc-updatemanager pkgupdate");
  }else{
    proc->start("pc-updatemanager -j "+jailID+" pkgupdate");
  }
}

UpdateDialog::~UpdateDialog(){

}

void UpdateDialog::procMessage(QString msg){
  //ui->frame->setVisible(true);
  ui->text_log->append(msg);
  //Do some quick parsing of the message for better messages
  //qDebug() << "Message: " << msg;
  if(msg.simplified().startsWith("[")){
    //qDebug() << " - Found Install msg";
    installphase=true;
    //Is installing, get the percent
    QString tmp = msg.section("]",0,0).section("[",1,1).simplified();
    QString summary = msg.section("]",1,50).section("...",0,0).simplified();
    double percent = tmp.section("/",0,0).toDouble()/tmp.section("/",-1).toDouble();
    percent = percent*100;
      ui->label_progress->setText( summary );
      ui->progressBar->setValue( (int) percent );
      ui->progressBar->setVisible(true);
  }else if(msg.contains("boot-environment", Qt::CaseInsensitive)){
    //qDebug() << " - Found BE msg";
      ui->label_progress->setText(msg);
      ui->progressBar->setVisible(false);
  }else if(msg.contains("Checking integrity..")){
      ui->label_progress->setText(msg);
      ui->progressBar->setVisible(false);	  
  }else if(msg.contains("desktop overlay data")){
      ui->label_progress->setText(tr("Finishing update checks"));
      ui->progressBar->setVisible(false);
  }
  
}

void UpdateDialog::procUpdate(QString percent, QString fsize, QString fname){
  //ui->frame->setVisible(true);
  //qDebug() << "Process Update Msg";
  if(installphase){ return; }
  ui->label_progress->setText( QString(tr("Downloading %1 (%2)")).arg(fname, fsize) );
  ui->progressBar->setValue( percent.section(".",0,0).toInt() );
  ui->progressBar->setVisible(true);
}

void UpdateDialog::procFinished(int ret, QProcess::ExitStatus stat){
  //ui->frame->setVisible(false);
  if(ret !=0 || stat!=QProcess::NormalExit){
    //Error
    ui->label_info->setText(tr("Failure!")+"\n"+tr("Please check the log for details."));
    ui->label_info->setStyleSheet("background: rgba(240,0,0,120); border-radius: 5px");
  }else{
    //Success
    ui->label_info->setText(tr("Success!")+"\n"+tr("Restarting your computer now is generally recommended."));
    ui->label_info->setStyleSheet("background: rgba(0,200,100,120); border-radius: 5px");
  }
	
  //Show the close buttons
  ui->push_done->setVisible(true);
  ui->push_reboot->setVisible(true);
}

/*void UpdateDialog::logview(){
  ui->text_log->setVisible(ui->check_viewlog->isChecked());
}*/

void UpdateDialog::rebootsystem(){
  QProcess::startDetached("shutdown -r now");
  rebooting = true;
  this->close();
}

void UpdateDialog::closeDialog(){
  rebooting = false;
  this->close();
}