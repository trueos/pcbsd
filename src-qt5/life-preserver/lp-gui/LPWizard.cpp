#include "LPWizard.h"
#include "ui_LPWizard.h"

LPWizard::LPWizard(QWidget *parent) : QWizard(parent), ui(new Ui::LPWizard){
  //Initialize the graphical items
  ui->setupUi(this);  //load the mainUI.ui file
  cancelled = true; //Make sure this is always set by default
  openAdvancedConfig = false;
  connect(this,SIGNAL(accepted()), this,SLOT(slotFinished()) );
  connect(this,SIGNAL(rejected()),this,SLOT(slotCancelled()) );
  connect(ui->combo_scrub_schedule, SIGNAL(currentIndexChanged(int)), this, SLOT(UpdateScrubUI()) );
  connect(ui->groupScrub, SIGNAL(toggled(bool)), this, SLOT(UpdateScrubUI()) );
  UpdateScrubUI();
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
  openAdvancedConfig = ui->check_openAdvanced->isChecked();
  if(ui->radio_automatic->isChecked()){ localTime = -999; }
  else{
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
  }
  enableScrub = ui->groupScrub->isChecked();
  if(enableScrub){
    int scrubschint = ui->combo_scrub_schedule->currentIndex();
    if(scrubschint == 0){
      scrubSchedule = "daily";
      scrubDay = 0;
    }else if(scrubschint == 1){
      scrubSchedule = "weekly";
      if(ui->combo_scrub_day_week->currentIndex() == 0){ scrubDay = 1; }
      else if(ui->combo_scrub_day_week->currentIndex() == 1){ scrubDay = 2; }
      else if(ui->combo_scrub_day_week->currentIndex() == 2){ scrubDay = 3; }
      else if(ui->combo_scrub_day_week->currentIndex() == 3){ scrubDay = 4; }
      else if(ui->combo_scrub_day_week->currentIndex() == 4){ scrubDay = 5; }
      else if(ui->combo_scrub_day_week->currentIndex() == 5){ scrubDay = 6; }
      else{ scrubDay = 7; }
    }else if(scrubschint==2){
      scrubSchedule = "monthly";
      scrubDay = ui->spin_scrub_day_month->value();
    }else{
      scrubSchedule = "days";
      scrubDay = ui->spin_scrub_day_month->value();
    }
    scrubTime = ui->time_scrub->time().hour();
  }

  //Now close the UI
  this->close();
}

void LPWizard::slotCancelled(){
  qDebug() << "Wizard Cancelled";
  cancelled = true; //just to make sure
  this->close();
}

void LPWizard::UpdateScrubUI(){
  int index = ui->combo_scrub_schedule->currentIndex();
  bool active = ui->groupScrub->isChecked();
  //Adjust whether the day of week box is enabled
  ui->combo_scrub_day_week->setEnabled( (index == 1) && active);
  //Adjust whether the day of month box is enabled
  ui->spin_scrub_day_month->setEnabled( (index >= 2) && active);
  // Always make time box enabled if not a daily interval
  ui->time_scrub->setEnabled( (index<3) && active);
  //hide all the "time" UI elements if an interval of days is selected (just looks strange otherwise)
  ui->combo_scrub_day_week->setVisible(index<3);
  ui->time_scrub->setVisible(index<3);
  ui->label_5->setVisible(index<3); //label
  ui->label_7->setVisible(index<3); //label
  ui->label_8->setVisible(index<3); //label
  //Make sure the maximum value for the combo_scrub_day_week widget is right
   ui->spin_scrub_day_month->setMaximum( (index<3) ? 28 : 999);
}

int LPWizard::nextId() const{
	int cpg = currentId();
	if(cpg == 1 && ui->radio_automatic->isChecked() ){
	  //Automatic snapshot schedule - skip the snapshot pruning page (not needed)
	  return 3;
	}else if(cpg==4){ //last page
	  return -1;
	}else{
	  return (cpg+1); //go to the next page
	}
}
