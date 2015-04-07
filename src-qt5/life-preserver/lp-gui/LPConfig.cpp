#include "LPConfig.h"
#include "ui_LPConfig.h"

LPConfig::LPConfig(QWidget *parent) : QDialog(parent), ui(new Ui::LPConfig){
  ui->setupUi(this); //initialize the designer UI
  qDebug() << "Initializing Configuration Dialog";
  //initialize the output variables as necessary
  localChanged = false;
  scrubChanged = false;
  remoteChanged = false;
  //Variables that will be changed when loading the dataset properties
	
  //now connect the buttons
  connect(ui->tool_apply,SIGNAL(clicked()), this,SLOT(slotApplyChanges()) );
  connect(ui->tool_cancel,SIGNAL(clicked()), this, SLOT(slotCancelConfig()) );
  connect(ui->push_scanNetwork, SIGNAL(clicked()), this, SLOT(autoDetectReplicationTargets()) );
}

LPConfig::~LPConfig(){
	
}

void LPConfig::loadDataset(QString ds, bool replicated, bool scrubsched){
  ui->label_dataset->setText(ds);
  loadDatasetConfiguration(ds, replicated, scrubsched);
}

//==========
//     PRIVATE
// ==========
void LPConfig::loadDatasetConfiguration(QString dataset, bool replicated, bool scrubsched){
  qDebug() <<" - Loading dataset configuration:" << dataset;
  //Load the dataset values
  isReplicated = replicated;
  isScrubSched = scrubsched;
  // - Local settings
  if( !LPBackend::datasetInfo(dataset, localSchedule, localSnapshots) ){
    localSchedule = 1; //daily at 1 AM
    localSnapshots = 7;
  }	  
  // - Scrub Settings
  bool ok=false;
  if(isScrubSched){
    ok = LPBackend::scrubInfo(dataset, scrubTime, scrubDay, scrubSchedule);
  }
  if(!ok){
    isScrubSched = false;
  }
  // - Replication settings
  ok=false;
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
    ui->combo_local_schedule->setCurrentIndex(5);	  
  }else if(localSchedule == -10){ //10 minutes
    ui->combo_local_schedule->setCurrentIndex(4);	
  }else if(localSchedule == -30){ //30 minutes
    ui->combo_local_schedule->setCurrentIndex(3);	  
  }else if(localSchedule == -60){ //assume hourly
    ui->combo_local_schedule->setCurrentIndex(2);	 
  }else if(localSchedule > 0 && localSchedule < 24){ //daily @ hour
    ui->combo_local_schedule->setCurrentIndex(1);
    ui->time_local_daily->setTime( QTime(localSchedule, 0) );
  }else{ //assume auto
    localSchedule = -999; //just to make sure it does not match anything else
    ui->combo_local_schedule->setCurrentIndex(0);
  }
  setLocalKeepNumber();

  // - Scrub settings
  ui->groupScrub->setChecked(isScrubSched);
  if(scrubSchedule == "daily"){
    ui->combo_scrub_schedule->setCurrentIndex(0);
  }else if(scrubSchedule == "weekly"){
    ui->combo_scrub_schedule->setCurrentIndex(1);
    if(scrubDay == 1){ ui->combo_scrub_day_week->setCurrentIndex(0); }
    else if(scrubDay == 2) {  ui->combo_scrub_day_week->setCurrentIndex(1); }
    else if(scrubDay == 3) {  ui->combo_scrub_day_week->setCurrentIndex(2); }
    else if(scrubDay == 4) {  ui->combo_scrub_day_week->setCurrentIndex(3); }
    else if(scrubDay == 5) {  ui->combo_scrub_day_week->setCurrentIndex(4); }
    else if(scrubDay == 6) {  ui->combo_scrub_day_week->setCurrentIndex(5); }
    else { ui->combo_scrub_day_week->setCurrentIndex(6); }
  }else{
    ui->combo_scrub_schedule->setCurrentIndex(2);
    ui->spin_scrub_day_month->setValue(scrubDay);
  }
  ui->time_scrub->setTime( QTime(scrubTime, 0) );

  // - Replication settings
  ui->groupReplicate->setChecked(isReplicated);
  ui->lineHostName->setText(remoteHost);
  ui->lineUserName->setText(remoteUser);
  ui->lineRemoteDataset->setText(remoteDataset);
  ui->spinPort->setValue(remotePort);
  if(remoteFreq >=0 && remoteFreq < 24){
    ui->combo_remote_schedule->setCurrentIndex(1); //Daily @
    ui->time_replicate->setTime( QTime(remoteFreq,0) );
  }else if(remoteFreq == -60){
    ui->combo_remote_schedule->setCurrentIndex(2); //Hourly
  }else if(remoteFreq == -30){
    ui->combo_remote_schedule->setCurrentIndex(3); // 30 minutes
  }else if(remoteFreq == -10){
    ui->combo_remote_schedule->setCurrentIndex(4); // 10 minutes
  }else if(remoteFreq == -2){
    ui->combo_remote_schedule->setCurrentIndex(5); // Manual mode
  }else{
    remoteFreq = -999; //just to make sure it is the "other" case
    ui->combo_remote_schedule->setCurrentIndex(0); // Sync
  }
  //Now update the visibility of items appropriately
  on_combo_local_schedule_currentIndexChanged(ui->combo_local_schedule->currentIndex());
  on_combo_scrub_schedule_currentIndexChanged(ui->combo_scrub_schedule->currentIndex());
  on_combo_remote_schedule_currentIndexChanged(ui->combo_remote_schedule->currentIndex());
}

void LPConfig::checkForChanges(){
  //Checks for changes to the settings while also updating the output variables to match the GUI
	
  localChanged = false;
  scrubChanged = false;
  remoteChanged = false;

  //Local Settings
  int nSchedule;
  int schint = ui->combo_local_schedule->currentIndex();
  if(schint == 0){ nSchedule = -999; } //Auto
  else if(schint == 1){ nSchedule = ui->time_local_daily->time().hour(); } //daily @ hour
  else if(schint == 2){ nSchedule = -60; } //hourly
  else if(schint == 3){ nSchedule = -30; } //30 min
  else if(schint == 4){ nSchedule = -10; } //10 min
  else{ nSchedule = -5; } //5 min
  int nTotSnaps;
  if( ui->combo_local_keepunits->currentIndex() == 0 && (schint != 1) ){ //days
    int numperday = 1440/(-nSchedule);
    nTotSnaps = numperday * ui->spin_local_numkeep->value();
  }else{ //total number (or daily snapshots)
    nTotSnaps = ui->spin_local_numkeep->value();
  }
  if(nSchedule != localSchedule){localChanged = true; localSchedule = nSchedule; }
  if(nTotSnaps != localSnapshots){ localChanged = true; localSnapshots = nTotSnaps; }

  //Scrub Settings
  QString nScrubSchedule;
  int nScrubDay = scrubDay;
  int nScrubTime = scrubTime;

  if(isScrubSched != ui->groupScrub->isChecked()){
    scrubChanged = true;
  }
  isScrubSched = ui->groupScrub->isChecked();

  if(isScrubSched){
    int scrubschint = ui->combo_scrub_schedule->currentIndex();
    if(scrubschint == 0){
      nScrubSchedule = "daily";
      nScrubDay = 0;
    }else if(scrubschint == 1){
      nScrubSchedule = "weekly";
      if(ui->combo_scrub_day_week->currentIndex() == 0){ nScrubDay = 1; }
      else if(ui->combo_scrub_day_week->currentIndex() == 1){ nScrubDay = 2; }
      else if(ui->combo_scrub_day_week->currentIndex() == 2){ nScrubDay = 3; }
      else if(ui->combo_scrub_day_week->currentIndex() == 3){ nScrubDay = 4; }
      else if(ui->combo_scrub_day_week->currentIndex() == 4){ nScrubDay = 5; }
      else if(ui->combo_scrub_day_week->currentIndex() == 5){ nScrubDay = 6; }
      else{ nScrubDay = 7; }
    }else{
      nScrubSchedule = "monthly";
      nScrubDay = ui->spin_scrub_day_month->value();
    }

    nScrubTime = ui->time_scrub->time().hour();

    if(nScrubSchedule != scrubSchedule){ scrubChanged = true; scrubSchedule = nScrubSchedule; }
    if(nScrubDay != scrubDay){ scrubChanged = true; scrubDay = nScrubDay; }
    if(nScrubTime != scrubTime){ scrubChanged = true; scrubTime = nScrubTime; }
  }

  //Replication Settings
  bool updateSSHKey = false;
  if(isReplicated != ui->groupReplicate->isChecked()){
    remoteChanged = true;
  }
  isReplicated = ui->groupReplicate->isChecked();
  if(isReplicated && remoteChanged){ updateSSHKey = true; }
  QString tmp = ui->lineHostName->text().simplified();
  //Only change the backend remoteHost value if the replication is still enabled (so de-activation will work properly)
  if( tmp != remoteHost && isReplicated){ remoteChanged = true; remoteHost = tmp; updateSSHKey=true;}
  tmp = ui->lineUserName->text().simplified();
  if( tmp != remoteUser ){ remoteChanged = true; remoteUser = tmp; updateSSHKey=true;}
  tmp = ui->lineRemoteDataset->text().simplified();
  if( tmp != remoteDataset ){ remoteChanged = true; remoteDataset = tmp; }
  if( ui->spinPort->value() != remotePort){ remoteChanged = true; remotePort = ui->spinPort->value(); updateSSHKey=true;}
  
  int nFreq = ui->combo_remote_schedule->currentIndex();
  if(nFreq == 0){
    nFreq = -999; //Sync
  }else if(nFreq==5){
    nFreq = -2; // Manual mode
  }else if(nFreq==1){
    nFreq = ui->time_replicate->time().hour(); //Daily @
  }else if(nFreq==2){
    nFreq = -60; //Hourly
  }else if(nFreq==3){
    nFreq = -30; //30 minutes
  }else{
    nFreq = -10; //10 minutes
  }
  if( nFreq != remoteFreq ){ remoteChanged = true; remoteFreq = nFreq; }
  
  if(updateSSHKey && isReplicated && remoteChanged){
    //Prompt for the SSH key generation
    LPBackend::setupSSHKey(remoteHost, remoteUser, remotePort);
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
  scrubChanged = false;
  remoteChanged = false; 
  this->close();
}

void LPConfig::on_combo_local_schedule_currentIndexChanged(int index){
  //Adjust whether the daily time box is visible
  ui->time_local_daily->setVisible( (index == 1) );
  //Show the Number to keep options if applicable
  ui->spin_local_numkeep->setVisible( (index!=0) );
  ui->label_local_keep->setVisible( (index!=0) );
  ui->combo_local_keepunits->setVisible( (index!=0) );
}

void LPConfig::on_combo_scrub_schedule_currentIndexChanged(int index){
  //Adjust whether the day of week box is enabled
  ui->combo_scrub_day_week->setEnabled( (index == 1) );
  ui->combo_scrub_day_week->setDisabled( (index != 1) );
  //Adjust whether the day of month box is enabled
  ui->spin_scrub_day_month->setEnabled( (index == 2) );
  ui->spin_scrub_day_month->setDisabled( (index != 2) );
}

void LPConfig::on_combo_remote_schedule_currentIndexChanged(int index){
  //Adjust whether the daily time box is visible
  ui->time_replicate->setVisible( (index == 1) );
}

void LPConfig::autoDetectReplicationTargets(){
  QStringList targs = LPGUtils::scanNetworkSSH(); // <name>:::<address>:::<port>
  if(targs.isEmpty()){
    QMessageBox::warning(this,tr("No Network Targets"), tr("We could not find any systems on the local network with SSH availability (port 22)") );
    return;
  }
  //Ask the user to select a target
  QStringList targets;
  for(int i=0; i<targs.length(); i++){
    targets << targs[i].section(":::",0,0);
  }
  bool ok;
  QString target = QInputDialog::getItem(this, tr("Select Replication Target"), tr("Hostname:"), targets, 0, false, &ok);
  if(!ok || target.isEmpty() ){ return; } //cancelled
  //Now look for that target in the list of info
  for(int i=0; i<targs.length(); i++){
    if(targs[i].startsWith(target+":::")){
      ui->lineHostName->setText(targs[i].section(":::",1,1));
      ui->spinPort->setValue( targs[i].section(":::",2,2).toInt() );
      break;
    }
  }
}
