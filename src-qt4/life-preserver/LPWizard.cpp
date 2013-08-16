#include "LPWizard.h"
#include "ui_LPWizard.h"

LPWizard::LPWizard(QWidget *parent = 0) : QWizard(parent), ui(new Ui::LPWizard){
  //Initialize the graphical items
  ui->setupUi(this);  //load the mainUI.ui file
  cancelled = true; //Make sure this is always set by default
  connect(this,SIGNAL(accepted()), this,SLOT(slotFinished()) );
  connect(this,SIGNAL(rejected()),this,SLOT(slotCancelled()) );
}

LPWizard::~LPWizard(){
	
}

void LPWizard::setDataset(QString ds){
  ui->label_dataset->setText(ds);
}
	
void LPWizard::slotFinished(){
  qDebug() << "Wizard Finished";
  cancelled = false; //use the values from the UI
  //Now load the information from the UI
  enableReplication = ui->groupReplicate->isChecked();
  if(enableReplication){
    remotePort = ui->spinPort->value();
    if(ui->radioSYNC->isChecked()){ 
      remoteTime = -1;
    }else{
      remoteTime = ui->time_replicate->time().hour();
    }
    remoteHost = ui->lineHostName->text();
    remoteUser = ui->lineUserName->text();
    remoteDataset = ui->lineRemoteDataset->text();
  }
  if(ui->radioDaily->isChecked()){ localTime = ui->timeEdit->time().hour(); }
  else if( ui->radio10Min->isChecked()){ localTime = -10; }
  else if( ui->radio30Min->isChecked()){ localTime = -30; }
  else if( ui->radio5Min->isChecked()){ localTime = -5; }
  else{ localTime = -60; } //hourly
  if( ui->radioKeepTotal->isChecked() ){
    totalSnapshots = ui->spin_keepTotal->value();
  }else{
    if(localTime >0){ totalSnapshots = ui->spin_keepDays->value(); }
    else{ totalSnapshots = ui->spin_keepDays->value() * (1440/(-localTime)); } //convert to number of snapshots a day
  }
  //Now close the UI
  this->close();
}

void LPWizard::slotCancelled(){
  qDebug() << "Wizard Cancelled";
  cancelled = true; //just to make sure
  this->close();
}
