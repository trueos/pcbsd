#include "migrateUI.h"
#include "ui_migrateUI.h"

MigrateUI::MigrateUI() : QMainWindow(), ui(new Ui::MigrateUI){
  ui->setupUi(this); //load the designer file	
  ui->group_reboot->setVisible(false);
  proc = new QProcess(this);
	proc->setProcessChannelMode(QProcess::MergedChannels);
  connect(proc, SIGNAL(finished(int, QProcess:ExitStatus)), this, SLOT(procFinished()) );
  connect(proc, SIGNAL(readyReadStandardOutput()), this, SLOT(updateProgress()) );
  connect(ui->push_start, SIGNAL(clicked()), this, SLOT(startMigration()) );
  connect(ui->push_reboot, SIGNAL(clicked()), this, SLOT(systemReboot()) );
  connect(ui->actionClose, SIGNAL(triggered()), this, SLOT(closeWindow()) );
}
	
void MigrateUI::slotSingleInstance(){
  this->raise();
  this->showNormal();
  this->activateWindow();
}

void MigrateUI::closeWindow(){
  this->close();	
}
	
void MigrateUI::startMigration(){
  ui->group_migrate->setVisible(false);
  proc->start("pbi migrate");
}
	
void MigrateUI::procFinished(){
  ui->group_reboot->setVisible(true);
}
	
void MigrateUI::updateProgress(){
  QString output = proc->readAllStandardOutput();
  ui->text_progress->append(output);
}
	
void MigrateUI::restartSystem(){
  QProcess::startDetached("shutdown -r now");
  this->close();
}
	