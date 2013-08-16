#include "LPConfig.h"
#include "ui_LPConfig.h"

LPConfig::LPConfig(QWidget *parent) : QDialog(parent), ui(new Ui::LPConfig){
  ui->setupUi(this); //initialize the designer UI
  qDebug() << "Initializing Configuration Dialog";
  //initialize the output variables as necessary
  localChanged = false;
  remoteChanged = false;
  //Variables that will be changed when loading the dataset properties
	
  //now connect the buttons
  connect(ui->tool_apply,SIGNAL(clicked()), this,SLOT(slotApplyChanges()) );
  connect(ui->tool_cancel,SIGNAL(clicked()), this, SLOT(slotCancelConfig()) );
}

LPConfig::~LPConfig(){
	
}

void LPConfig::loadDataset(QString ds, bool replicated){
  ui->label_dataset->setText(ds);
  loadDatasetConfiguration(ds, replicated);
}

//==========
//     PRIVATE
// ==========
void LPConfig::loadDatasetConfiguration(QString dataset, bool replicated){
  qDebug() <<" - Loading dataset configuration:" << dataset;
  //Load the dataset values
  isReplicated = replicated;
  // - Local settings
  if( !LPBackend::datasetInfo(dataset, localSchedule, localSnapshots) ){
    localSchedule = 1; //daily at 1 AM
    localSnapshots = 7;
  }	  
  // - Replication settings
  bool ok=false;
  if(isReplicated){
    ok = LPBackend::replicationInfo(dataset, remoteHost, remoteUser, remotePort, remoteDataset, remoteFreq);
  }
  if(!ok){
    isReplicated = false;
    remotePort = 22;
    remoteFreq = -1; //sync
    remoteHost = "";
    remoteUser = "";
    remoteDataset = "";
  }
  //Now put the values into the UI
  // - local settings
  if(localSchedule == -5){ //5 minutes
    ui->combo_local_schedule->setCurrentIndex(4);	  
  }else if(localSchedule == -10){ //10 minutes
    ui->combo_local_schedule->setCurrentIndex(3);	
  }else if(localSchedule == -30){ //30 minutes
    ui->combo_local_schedule->setCurrentIndex(2);	  
  }else if(localSchedule > 0 && localSchedule < 24){ //daily @ hour
    ui->combo_local_schedule->setCurrentIndex(0);
    ui->time_local_daily->setTime( QTime(localSchedule, 0) );
  }else{ //assume hourly
    localSchedule = -60;
    ui->combo_local_schedule->setCurrentIndex(1);	  
  }
  setLocalKeepNumber();
	
  // - Replication settings
  ui->groupReplicate->setChecked(isReplicated);
  ui->lineHostName->setText(remoteHost);
  ui->lineUserName->setText(remoteUser);
  ui->lineRemoteDataset->setText(remoteDataset);
  ui->spinPort->setValue(remotePort);
  if(remoteFreq >=0 && remoteFreq < 24){
    ui->radioRepTime->setChecked(true);
    ui->time_replicate->setTime( QTime(remoteFreq,0) );
  }else{
    ui->radioSYNC->setChecked(true);
  }
  
}

void LPConfig::checkForChanges(){
  //Checks for changes to the settings while also updating the output variables to match the GUI
	
  localChanged = false;
  remoteChanged = false;
  //Local Settings
  int nSchedule;
  int schint = ui->combo_local_schedule->currentIndex();
  if(schint == 0){ nSchedule = ui->time_local_daily->time().hour(); } //daily @ hour
  else if(schint == 1){ nSchedule = -60; } //hourly
  else if(schint == 2){ nSchedule = -30; } //30 min
  else if(schint == 3){ nSchedule = -10; } //10 min
  else{ nSchedule = -5; } //5 min
  int nTotSnaps;
  if( ui->combo_local_keepunits->currentIndex() == 0 && (schint != 0) ){ //days
    int numperday = 1440/(-nSchedule);
    nTotSnaps = numperday * ui->spin_local_numkeep->value();
  }else{ //total number (or daily snapshots)
    nTotSnaps = ui->spin_local_numkeep->value();
  }
  if(nSchedule != localSchedule){localChanged = true; localSchedule = nSchedule; }
  if(nTotSnaps != localSnapshots){ localChanged = true; localSnapshots = nTotSnaps; }
  
  //Replication Settings
  if(isReplicated != ui->groupReplicate->isChecked()){
    remoteChanged = true;
    isReplicated = ui->groupReplicate->isChecked();
  }
  QString tmp = ui->lineHostName->text().simplified();
  if( tmp != remoteHost ){ remoteChanged = true; remoteHost = tmp; }
  tmp = ui->lineUserName->text().simplified();
  if( tmp != remoteUser ){ remoteChanged = true; remoteUser = tmp; }
  tmp = ui->lineRemoteDataset->text().simplified();
  if( tmp != remoteDataset ){ remoteChanged = true; remoteDataset = tmp; }
  if( ui->spinPort->value() != remotePort){ remoteChanged = true; remotePort = ui->spinPort->value(); }
  int nFreq = -1;
  if(ui->radioRepTime->isChecked()){
    nFreq = ui->time_replicate->time().hour();
  }
  if(nFreq < 0){
    if( remoteFreq >= 0 && remoteFreq < 24){remoteChanged = true; remoteFreq = nFreq;}
  }else{
    if( nFreq != remoteFreq ){ remoteChanged = true; remoteFreq = nFreq; }
  }
}

void LPConfig::setLocalKeepNumber(){
  if(localSchedule >=0){
    ui->combo_local_keepunits->setCurrentIndex(0); //num days
    ui->spin_local_numkeep->setValue(localSnapshots);   
  }else{
    int numperday = 1440/(-localSchedule);
    if( localSnapshots % numperday == 0 ){
	//daily
	ui->combo_local_keepunits->setCurrentIndex(0); //num days
	ui->spin_local_numkeep->setValue(localSnapshots/numperday);
    }else{
	//odd number of snapshots - must be total
	ui->combo_local_keepunits->setCurrentIndex(1); //num total
	ui->spin_local_numkeep->setValue(localSnapshots);
    }
  }
}

// =============
//    PRIVATE SLOTS
// =============
void LPConfig::slotApplyChanges(){
  checkForChanges();
  this->close();
}

void LPConfig::slotCancelConfig(){
  //Make sure it is flagged as unchanged before closing
  localChanged = false;
  remoteChanged = false; 
  this->close();
}

void LPConfig::on_combo_local_schedule_currentIndexChanged(int index){
  //Adjust whether the daily time box is visible
  ui->time_local_daily->setVisible( (index == 0) );
}
