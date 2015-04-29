#include "LPConfig.h"
#include "ui_LPConfig.h"

LPConfig::LPConfig(QWidget *parent) : QDialog(parent), ui(new Ui::LPConfig){
  ui->setupUi(this); //initialize the designer UI
  qDebug() << "Initializing Configuration Dialog";
  //Make sure it is centered on the parent
  if(parent!=0){
    QPoint ctr = parent->geometry().center();
    this->move(ctr.x()-(this->width()/2), ctr.y()-(this->height()/2));
  }
  //initialize the output variables as necessary
  localChanged = false;
  scrubChanged = false;
  remoteChanged = false;
  cRepHost = -1; //internal flag
  //Variables that will be changed when loading the dataset properties
	
  //now connect the buttons
  connect(ui->tool_apply,SIGNAL(clicked()), this,SLOT(slotApplyChanges()) );
  connect(ui->tool_cancel,SIGNAL(clicked()), this, SLOT(slotCancelConfig()) );
  connect(ui->groupScrub, SIGNAL(toggled(bool)), this, SLOT(UpdateScrubUI()) );
  connect(ui->combo_scrub_schedule, SIGNAL(currentIndexChanged(int)), this, SLOT(UpdateScrubUI()) );
  connect(ui->combo_rep_hosts, SIGNAL(currentIndexChanged(int)), this, SLOT(UpdateRepHostInfo()) );
  connect(ui->tool_rep_remhost, SIGNAL(clicked()), this, SLOT(RemRepHost()) );
  connect(ui->tool_rep_addhost, SIGNAL(clicked()), this, SLOT(AddRepHost()) );
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
  qDebug() <<" - Loading dataset configuration:" << dataset << replicated << scrubsched;
  //Load the dataset values
  isReplicated = replicated;
  isScrubSched = scrubsched;
  // - Local settings
  //qDebug() << " - Local Info";
  if( !LPBackend::datasetInfo(dataset, localSchedule, localSnapshots) ){
    localSchedule = 1; //daily at 1 AM
    localSnapshots = 7;
  }	  
  // - Scrub Settings
  //qDebug() << " - Scrub Info";
  bool ok=false;
  if(isScrubSched){
    ok = LPBackend::scrubInfo(dataset, scrubTime, scrubDay, scrubSchedule);
  }
  if(!ok){
    isScrubSched = false;
  }
  //qDebug() << " - Replication Info";
  // - Replication settings
  if(isReplicated){
    remoteHosts = LPBackend::replicationInfo(dataset);
  }
   //qDebug() << " - Load UI";
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
  ui->combo_rep_hosts->clear();
  origHosts.clear();
  for(int i=0; i<remoteHosts.length(); i++){
    ui->combo_rep_hosts->addItem(remoteHosts[i].host());
    origHosts << remoteHosts[i].host(); //save this for comparison later
  }

  //Now update the visibility of items appropriately
  on_combo_local_schedule_currentIndexChanged(ui->combo_local_schedule->currentIndex());
  UpdateRepHostInfo();
  //on_combo_remote_schedule_currentIndexChanged(ui->combo_remote_schedule->currentIndex());
  UpdateScrubUI();
}

void LPConfig::checkForChanges(){
  //Checks for changes to the settings while also updating the output variables to match the GUI
	
  localChanged = false;
  scrubChanged = false;
  //remoteChanged = false;

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
  UpdateRepHostInfo(); //catch any changes in the UI that have not been saved yet
  
  //Compile the lists of new/removed hosts
  newHosts.clear(); remHosts.clear();
  for(int i=0; i<origHosts.length(); i++){
    bool found = false;
    for(int j=0; j<remoteHosts.length(); j++){
      if( origHosts[i] == remoteHosts[j].host() ){ found = true; break;}
    }
    if(!found){ remHosts << origHosts[i]; } //Was removed
  }
  for(int j=0; j<remoteHosts.length(); j++){
    if( !origHosts.contains(remoteHosts[j].host()) ){ newHosts << remoteHosts[j].host(); }
  }
  newHosts.removeDuplicates();
  
  /*for(int i=0; i<newHosts.length(); i++){
    //Get the settings for this new host
    for(int j=0; j<remoteHosts.length(); j++){
      if(remoteHosts[j].host()==newHosts[i]){
        //Prompt for the SSH key generation
        LPBackend::setupSSHKey(remoteHosts[j].host(), remoteHosts[j].user(), remoteHosts[j].port());
      }
    }
  }*/
  
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

void LPConfig::UpdateScrubUI(){
  int index = ui->combo_scrub_schedule->currentIndex();
  bool active = ui->groupScrub->isChecked();
  //Adjust whether the day of week box is enabled
  ui->combo_scrub_day_week->setEnabled( (index == 1) && active);
  //Adjust whether the day of month box is enabled
  ui->spin_scrub_day_month->setEnabled( (index == 2) && active);
  // Always make time box enabled
  ui->time_scrub->setEnabled(active);
}

void LPConfig::on_combo_remote_schedule_currentIndexChanged(int index){
  //Adjust whether the daily time box is visible
  ui->time_replicate->setVisible( (index == 1) );
}

/*void LPConfig::autoDetectReplicationTargets(){
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
}*/

void LPConfig::UpdateRepHostInfo(){
  if(cRepHost>=0 && cRepHost < ui->combo_rep_hosts->count() ){
    //Also Verify that the array host matches the UI host
    int index = -1;
    for(int i=0; i<remoteHosts.length(); i++){
      if(remoteHosts[i].host() == ui->combo_rep_hosts->itemText(cRepHost)){
        index = i;
        break;
      }
    }
    //Save the current info into the array first
    if(remoteHosts[index].user() != ui->lineUserName->text()){
      remoteHosts[index].setUser( ui->lineUserName->text() ); remoteChanged = true;
    }
    if(remoteHosts[index].dataset() != ui->lineRemoteDataset->text()){
      remoteHosts[index].setDataset( ui->lineRemoteDataset->text() ); remoteChanged = true;
    }
    if(remoteHosts[index].port() != ui->spinPort->value()){
      remoteHosts[index].setPort( ui->spinPort->value() ); remoteChanged = true;
    }
    int freq = ui->combo_remote_schedule->currentIndex();
    if(freq==0){ freq = -1; } //Sync
    else if(freq==1){ freq  = ui->time_replicate->time().hour(); } //daily
    else if(freq==2){ freq = -60; } //hourly
    else if(freq==3){ freq = -30; } //30 minutes
    else if(freq==4){ freq = -10; } //10 minutes
    else{ freq = -2; } //manual
    if(remoteHosts[index].freq() != freq){
      remoteHosts[index].setFreq(freq); remoteChanged = true;
    }
  }
  
  int index = -1;
  if( ui->combo_rep_hosts->currentIndex()>=0){
    for(int i=0; i<remoteHosts.length(); i++){
       if(remoteHosts[i].host() == ui->combo_rep_hosts->currentText()){
	 index = i;
	 break;
       }
    }
  }
  ui->groupReplicate->setEnabled(index>=0);
  if(index>=0){
    //Now display the info for the selected host
    ui->lineUserName->setText(remoteHosts[index].user());
    ui->lineRemoteDataset->setText(remoteHosts[index].dataset());
    ui->spinPort->setValue(remoteHosts[index].port());
    if(remoteHosts[index].freq() >=0 && remoteHosts[index].freq() < 24){
      ui->combo_remote_schedule->setCurrentIndex(1); //Daily @
      ui->time_replicate->setTime( QTime(remoteHosts[index].freq(),0) );
    }else if(remoteHosts[index].freq() == -60){
      ui->combo_remote_schedule->setCurrentIndex(2); //Hourly
    }else if(remoteHosts[index].freq() == -30){
      ui->combo_remote_schedule->setCurrentIndex(3); // 30 minutes
    }else if(remoteHosts[index].freq() == -10){
      ui->combo_remote_schedule->setCurrentIndex(4); // 10 minutes
    }else if(remoteHosts[index].freq() == -2){
      ui->combo_remote_schedule->setCurrentIndex(5); // Manual mode
    }else{
      ui->combo_remote_schedule->setCurrentIndex(0); // Sync
    }
    //make sure the time widget is visible appropriately
    on_combo_remote_schedule_currentIndexChanged(ui->combo_remote_schedule->currentIndex());
  }
  cRepHost = ui->combo_rep_hosts->currentIndex(); //save this flag for later
}

void LPConfig::AddRepHost(){
  QStringList targs = LPGUtils::scanNetworkSSH(); // <name>:::<address>:::<port>
  //Ask the user to select a target
  QStringList targets;
  for(int i=0; i<targs.length(); i++){
    targets << targs[i].section(":::",0,0);
  }
  if(targets.isEmpty()){ targets << ""; }
  bool ok;
  QString target = QInputDialog::getItem(this, tr("Identify Replication Target"), tr("Detected Hostname or custom IP:"), targets, 0, true, &ok);
  if(!ok || target.isEmpty() ){ return; } //cancelled
  //Now look for that target in the list of info
  LPRepHost H;
  for(int i=0; i<targs.length(); i++){
    if(targs[i].startsWith(target+":::")){
      H.setHost( targs[i].section(":::",1,1) );
      H.setPort( targs[i].section(":::",2,2).toInt() );
      break;
    }
  }
  if(H.host().isEmpty()){ 
    H.setHost( target ); //custom host selected
    H.setPort( 22 ); //Default SSH port on most systems
  }
  H.setFreq( -1 ); //Set to the "Sync" frequency by default
  //Add this host to the info
  remoteHosts << H;
  ui->combo_rep_hosts->addItem( H.host() );
  ui->combo_rep_hosts->setCurrentIndex( ui->combo_rep_hosts->count()-1 ); //go to the new item
}

void LPConfig::RemRepHost(){
  QString host = ui->combo_rep_hosts->currentText();
  bool found = false;
  for(int i=0; i<remoteHosts.length(); i++){
    if(remoteHosts[i].host() == host){
      remoteHosts.removeAt(i);
      found = true;
      break;
    }
  }
  if(!found){ return; } //did not do anything
  cRepHost = -1; //Make sure we don't save any of the settings being removed
  ui->combo_rep_hosts->removeItem(ui->combo_rep_hosts->currentIndex());
}

