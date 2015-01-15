#include "mainUI.h"
#include "ui_mainUI.h"

mainUI::mainUI(QWidget *parent) : QMainWindow(parent), ui(new Ui::mainUI){
	  
  qDebug() << "Starting up pc-bootconfig...";
  ui->setupUi(this);  //load the mainUI.ui file
  //Set paths to GRUB files
  file_GRUBdefaults="/usr/local/etc/default/grub";
  file_GRUBentries="/boot/grub/custom.cfg";
  file_pcbsdconf="/usr/local/etc/pcbsd.conf";
  dir_GRUBthemes="/boot/grub/themes/";
  dir_GRUBfonts="/boot/grub/";
  //Make sure that backups exist of the GRUB configuration file
  if(!QFile::exists(file_GRUBdefaults+".old")){ 
    QString cmd = "cp "+file_GRUBdefaults+" "+file_GRUBdefaults+".old";
    QProcess::execute(cmd);
  }
  //initialize the QProcess
  proc = new QProcess(this);
    proc->setProcessChannelMode(QProcess::MergedChannels);
  
  //Load the GRUB defaults file into memory
  G_goodLoad = loadGRUBdefaults(G_themeFile, G_fontFile, G_timer, G_showMenu);//, G_defaultBE);
  //Update the list of available boot environments
  updateBEList();
  //Update the GRUB configuration values
  updateGRUBdefaults();
  //Update the custom GRUB entries
  updateGRUBentries();
  //Load the maxBE value from pcbsd.conf
  loadMaxBE();
  //Setup the Signals/slots for change to GRUB values
  connect(ui->line_GRUBthemefile,SIGNAL(textChanged(QString)),this,SLOT(GRUBchangedefaults()) );
  connect(ui->line_GRUBfontfile,SIGNAL(textChanged(QString)),this,SLOT(GRUBchangedefaults()) );
  connect(ui->spin_GRUBtimer,SIGNAL(valueChanged(int)),this,SLOT(GRUBchangedefaults()) );
  connect(ui->check_GRUBshowcountdown,SIGNAL(stateChanged(int)),this,SLOT(GRUBchangedefaults()) );
  connect(ui->text_GRUBentries,SIGNAL(textChanged()),this,SLOT(GRUBchangeentries()) );
}

mainUI::~mainUI(){
  qDebug() << "Closing down pc-bootconfig normally...";
  delete proc;
}

void mainUI::slotSingleInstance(){
  this->raise();
  this->show();
}
// =====================
// === PRIVATE FUNCTIONS ===
// =====================
//Random utility functions
int mainUI::getSelectedBE(){
  if( ui->tree_BE->topLevelItemCount() < 1 ){ return -1; }
  else{
    return ui->tree_BE->indexOfTopLevelItem( ui->tree_BE->currentItem() );
  }
}

bool mainUI::checkName(QString newname){
  bool ok = true;
  for( int i=0; i<ui->tree_BE->topLevelItemCount(); i++){
    if(ui->tree_BE->topLevelItem(i)->text(0) == newname){ ok=false;  break; }
  }
  if(!ok){
    QMessageBox::warning(this,tr("Invalid Name"), tr("A Boot Environment with that name already exists, please choose a different one."));
  }
  return ok;
}

bool mainUI::validateInput(QString name){
  //Check for invalid name characters (only letters/numbers)
  bool ok = true;
  QStringList validChars; validChars << "-" << "_";
  for(int i=0; i<name.length(); i++){
    if( !name.at(i).isLetter() && !name.at(i).isDigit()  && !validChars.contains(name.at(i)) ){ ok = false; break; }
  }
  if(!ok){
    QMessageBox::warning(this,tr("Invalid Name"), tr("Boot Environment names may only be comprised of letters and numbers") );
  }
  return ok;  
}

void mainUI::runLongCMD(QString cmd, QString info){
	
  if(info.isEmpty()){ info = tr("Performing Boot Environment Changes. Please Wait."); }
  //Initialize the Working Message Box
  QMessageBox wrkmsg(this);
    wrkmsg.setWindowTitle( tr("Working") );
    wrkmsg.setText( info );
    wrkmsg.setStandardButtons(0);
    wrkmsg.setWindowModality(Qt::WindowModal);
  //Show the message
  QCoreApplication::processEvents();
  wrkmsg.show();
  QCoreApplication::processEvents();	
  QCoreApplication::processEvents();	
  //Run the command
  proc->start(cmd);
  while( !proc->waitForFinished(1000) ){
    QCoreApplication::processEvents();
  }
  //Close the message box
  wrkmsg.close();
}

//Boot Environment Management Functions
void mainUI::beadmActivate(QString name){ 
	//GRUB OVERRIDES THIS TYPE OF ACTIVATION!!! DO NOT USE!
	qDebug() << "Activate BE:" << name;
	QString cmd = "beadm activate "+name;
	runLongCMD(cmd);
}

void mainUI::beadmCreate(QString name){ 
	qDebug() << "Create BE:" << name;
	QString cmd ="beadm create "+name;
	runLongCMD(cmd);
}

void mainUI::beadmCopy(QString name, QString newname){ 
	qDebug() << "Copy BE:" << name + " -> " + newname;
	QString cmd ="beadm create -e "+name+" "+newname;
	runLongCMD(cmd);
}

void mainUI::beadmRemove(QString name){ 
	qDebug() << "Remove BE:" << name;
	QString cmd ="beadm destroy -F "+name;
	runLongCMD(cmd);
}

void mainUI::beadmRename(QString name, QString newname){ 
	qDebug() << "Rename BE:" << name + " -> " + newname;	
	QString cmd ="beadm rename "+name+" "+newname;
	runLongCMD(cmd);
}

void mainUI::beadmMount(QString name, QString mntpt){ 
	qDebug() << "Mount BE:" << name + " on " + mntpt;
	QString cmd ="beadm mount "+name+" "+mntpt;
	runLongCMD(cmd);
}

void mainUI::beadmUnmount(QString name){ 
	qDebug() << "Unmount BE:" << name;
	QString cmd ="beadm umount "+name;
	runLongCMD(cmd);
}

QStringList mainUI::beadmList(){
	//Output format (per line): <name>::<isrunning>::<isactivated>::<mountpoint>::<space>::<datecreated>::<timecreated>
	qDebug() << "List BE's";
	proc->start( "beadm list -H" );
	proc->waitForFinished();
	QStringList lines = QString(proc->readAllStandardOutput()).split("\n");
	QStringList output;
	//Format the output
	for(int i=0; i<lines.length(); i++){
	  if( lines[i].isEmpty() ){ continue; }
	  QString active = lines[i].section("\t",1,1);
	  QString running, activated;
	  if(active=="NR"){ running="Yes"; activated="Yes"; }
	  else if(active=="N"){ running="Yes"; activated="No"; }
	  else if(active=="R"){ running="No"; activated="Yes"; }
	  else{ running="No"; activated="No"; }
	  QString newline = lines[i].replace("\t","::").replace("::"+active+"::", "::"+running+"::"+activated+"::");
	  output << newline;
	}
	return output;
}

//GRUB Configuration Management Functions
bool mainUI::loadGRUBdefaults(QString &themefile, QString &fontfile, int &countdown, bool &showcountdown){//, int &defaultBE){
  qDebug() << "Load GRUB defaults:" << file_GRUBdefaults;
  QFile file(file_GRUBdefaults);
  if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){ return false; }
  QTextStream in(&file);
  //initialize outputs
  themefile.clear(); fontfile.clear();
  int hcount = 0; //initilization
  countdown = 0; //initialization
  showcountdown = false;
  //defaultBE = 0;
  while( !in.atEnd() ){
    QString line = in.readLine();
    QString var = line.section("=",0,0); //variable
    QString val = line.section("=",1,10); //value
    if(var == "GRUB_THEME"){ themefile.append(val); }
    else if(var == "GRUB_FONT"){ fontfile.append(val); }
    else if(var == "GRUB_HIDDEN_TIMEOUT"){ hcount = val.toInt(); }
    else if(var == "GRUB_TIMEOUT"){ countdown = val.toInt(); }
    else if(var == "GRUB_HIDDEN_TIMEOUT_QUIET"){ showcountdown = (val != "true"); }
    //else if(var == "GRUB_DEFAULT"){ defaultBE = val.toInt(); }
  }
  file.close();
  //Check to determine which countdown procedure to use
  //qDebug() << "GRUB defaults:" << themefile << fontfile << countdown << hcount << showcountdown;
  if(!showcountdown){ countdown = hcount; } //use the hidden countdown time if hidden
  return true;	
}

bool mainUI::saveGRUBdefaults(QString themefile, QString fontfile, int countdown, bool showcountdown){//, int defaultBE){
  qDebug() << "Save GRUB defaults:" << file_GRUBdefaults;
  QFile file(file_GRUBdefaults+".new");
  if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)){ return false; }
  QTextStream out(&file);
  if(!themefile.isEmpty()){ out << "GRUB_THEME="+themefile+"\n"; }
  if(!fontfile.isEmpty()){ out << "GRUB_FONT="+fontfile+"\n"; }
  
  if(showcountdown){ 
    out << "GRUB_HIDDEN_TIMEOUT_QUIET=false\n"; 
    out << "GRUB_TIMEOUT="+ QString::number(countdown)+"\n";
  }else{ 
    out << "GRUB_HIDDEN_TIMEOUT_QUIET=true\n"; 
    out << "GRUB_HIDDEN_TIMEOUT="+ QString::number(countdown)+"\n";
  }
  //if(defaultBE >= ui->tree_BE->topLevelItemCount()){ defaultBE = 0; }

  // Disable this for now, needs to use "beadm" to activate a BE now
  //if(defaultBE >= 0){ out << "GRUB_DEFAULT="+QString::number(defaultBE)+"\n"; }
  file.close();
  QString cmd="mv "+file_GRUBdefaults+".new "+file_GRUBdefaults;
  QProcess::execute(cmd);
  return true;		
}

bool mainUI::loadGRUBcustomentries(QStringList &filecontents){
  qDebug() << "Load GRUB custom entries:" << file_GRUBentries;
  filecontents.clear();
  QFile file(file_GRUBentries);
  if( !file.exists() ){ return true; }
  if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){ return false; }
  QTextStream in(&file);
  while( !in.atEnd() ){
    QString line = in.readLine();
    filecontents << line;
  }
  file.close();
  return true;
}

bool mainUI::saveGRUBcustomentries(QStringList filecontents){
  qDebug() << "Save GRUB custom entries:" << file_GRUBentries;
  QFile file(file_GRUBentries+".new");
  if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)){ return false; }
  QTextStream out(&file);
  for(int i=0; i<filecontents.length(); i++){
    out << filecontents[i]+"\n";
  }
  file.close();
  QString cmd="mv "+file_GRUBentries+".new "+file_GRUBentries;
  QProcess::execute(cmd);
  return true;	
}
	
void mainUI::loadMaxBE(){
  int val = 5; //default value
  //Read the file
  confcontents.clear(); //clear the internal variable
  QFile file(file_pcbsdconf);
  if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
    QTextStream in(&file);
    while(!in.atEnd()){
      QString line = in.readLine();
      if(line.simplified().startsWith("MAXBE:")){
	 val = line.section(":",1,1).simplified().toInt();
      }else{
	 confcontents << line; //save all contents *except* the MAXBE: line
      }
    }
    file.close();
  }
  
  if(val > 10){ val = 10; }
  else if(val < 1){ val = 1; }
  ui->spin_maxbe->setValue(val);
}

// =====================
// ===== PRIVATE SLOTS =====
// =====================
//UI update functions
void mainUI::updateBEList(){
  ui->tree_BE->clear();
  QStringList blist = beadmList();
  for(int i=0; i<blist.length(); i++){
    //List all the data <name>::<isrunning>::<isactivated>::<mountpoint>::<space>::<datecreated>::<timecreated>
    QStringList cols;
      cols << blist[i].section("::",0,0); // [0] name
      cols << blist[i].section("::",1,1); // [1] is running
      cols << blist[i].section("::",2,2); // [2] is default for next boot
      //Don't use GRUB default BE setting - this is itself part of a boot environment and might not be globally correct
      //if(G_defaultBE != i){ G_defaultBE = i; }
      cols << blist[i].section("::",5,6); // [3] date
      cols << blist[i].section("::",3,3); // [4] mountpoints
      cols << blist[i].section("::",4,4); // [5] size
    //Create the new treewidgetitem
    QTreeWidgetItem *item = new QTreeWidgetItem(cols);
	  
    //Add the item to the list
    ui->tree_BE->addTopLevelItem(item);
    //qDebug() << "BE Item:" << cols;
  }
  //Now re-size the columns appropriately
  for(int i=0; i<6; i++){ ui->tree_BE->resizeColumnToContents(i); }
}

void mainUI::updateGRUBdefaults(bool withrebuild){
  if( G_goodLoad ){
    //Load the info into the UI
    ui->line_GRUBthemefile->setText(G_themeFile);
    ui->line_GRUBfontfile->setText(G_fontFile);
    ui->spin_GRUBtimer->setValue(G_timer);
    ui->check_GRUBshowcountdown->setChecked(G_showMenu);
    ui->group_GRUBsettings->setEnabled(true);
  }else{
    //Could not read the GRUB defaults file
    ui->group_GRUBsettings->setEnabled(false);
  }
  if( ui->tree_BE->topLevelItemCount() > 1){
    ui->check_GRUBshowcountdown->setVisible(false);
    if(!G_showMenu){ 
      G_showMenu=true; 
      saveGRUBdefaults(G_themeFile, G_fontFile, G_timer, G_showMenu);//, G_defaultBE);
    }
  }else{
    ui->check_GRUBshowcountdown->setVisible(true);
  }
  if(withrebuild){ on_action_rebuildGRUBmenu_triggered(); }
  //Make sure that the save button is disabled - no changes yet
  ui->tool_GRUBsavedefaults->setEnabled(false);
  ui->tool_GRUBresetdefaults->setEnabled(false);
}

void mainUI::updateGRUBentries(){
  QStringList entries;
  if( loadGRUBcustomentries(entries) ){
    ui->text_GRUBentries->clear();
    if(!entries.isEmpty()){ ui->text_GRUBentries->setPlainText( entries.join("\n") ); }
    ui->group_GRUBentries->setEnabled(true);
  }else{
    ui->group_GRUBentries->setEnabled(false);
  }
  //Make sure that the save button is disabled - no changes yet
  ui->tool_GRUBsaveentries->setEnabled(false);
  ui->tool_GRUBresetentries->setEnabled(false);
}

//UI Buttons - Boot Environments
void mainUI::on_tool_BEadd_clicked(){
  //Check to see if we need to reset the GRUB defaults afterwards
  bool updateGRUB=false;
  if( ui->tree_BE->topLevelItemCount() == 1){updateGRUB=true;} //moving from 1 to 2
  //Get the new name from the user
  bool ok;
  QString newname = QInputDialog::getText( this, tr("New BE name"), tr("Choose a name for the new boot environment"), QLineEdit::Normal, "", &ok,0, Qt::ImhLowercaseOnly | Qt::ImhUppercaseOnly | Qt::ImhDigitsOnly); 
  if(ok && !newname.isEmpty()){
    if( !validateInput(newname) ){
      on_tool_BEadd_clicked(); //try again
      return;
    }
    if( checkName(newname) ){
      if(updateGRUB && !G_showMenu){ 
	G_showMenu=true; 
	saveGRUBdefaults(G_themeFile, G_fontFile, G_timer, G_showMenu);//, G_defaultBE);  
      }
      beadmCreate(newname);
      updateBEList();
      updateGRUBdefaults();
    }
  }	
}

void mainUI::on_tool_BEactivate_clicked(){
  int index = getSelectedBE();
  if(index != -1){
    //Save the new defaults
    //G_defaultBE = index;
    //bool ok = saveGRUBdefaults(G_themeFile, G_fontFile, G_timer, G_showMenu, G_defaultBE);
    //Update GRUB config
    //if(ok){ 
      beadmActivate( ui->tree_BE->currentItem()->text(0) ); //for other bootloaders
      on_action_rebuildGRUBmenu_triggered(); //For GRUB bootloader
    //}
    //Update GUI
    updateBEList();
  }
}

void mainUI::on_tool_BEcp_clicked(){
 int index = getSelectedBE();
  if(index != -1){
    if(ui->tree_BE->topLevelItem(index)->text(1).toLower() == "yes"){
      QMessageBox::warning(this,tr("Running Boot Environment"),tr("You cannot make a copy of the currently running boot environment!") );
      return;
    }
    bool updateGRUB=false;
    if( ui->tree_BE->topLevelItemCount() == 1){updateGRUB=true;} //moving from 1 to 2
    QString name = ui->tree_BE->topLevelItem(index)->text(0);
    //Get the new name from the user
    bool ok;
    QString newname = QInputDialog::getText( this, tr("New BE name"), tr("Choose a name for the new boot environment"), QLineEdit::Normal, name, &ok,0, Qt::ImhDigitsOnly | Qt::ImhUppercaseOnly | Qt::ImhLowercaseOnly); 
    if(ok && !newname.isEmpty()){
      if( !validateInput(newname) ){
        on_tool_BEcp_clicked(); //try again
        return;
      }
      if( checkName(newname) ){
	if(updateGRUB && !G_showMenu){ 
	  G_showMenu=true; 
	  saveGRUBdefaults(G_themeFile, G_fontFile, G_timer, G_showMenu);//, G_defaultBE);  
        }
        beadmCopy(name,newname);
        updateBEList();
	updateGRUBdefaults(true);
      }
    }
  }	
}

void mainUI::on_tool_BEmv_clicked(){
  int index = getSelectedBE();
  if(index != -1){
    /*if(ui->tree_BE->topLevelItem(index)->text(0).toLower() == "default"){
      QMessageBox::warning(this,tr("Base Boot Environment"), tr("You cannot rename the base environment that all other BE's need to function!") );
      return;
    }*/
    if(ui->tree_BE->topLevelItem(index)->text(1).toLower() == "yes"){
      QMessageBox::warning(this,tr("Running Boot Environment"), tr("You cannot rename a boot environment that you are currently running!") );
      return;
    }
    if(ui->tree_BE->topLevelItem(index)->text(2).toLower() == "yes"){
      QMessageBox::warning(this,tr("Default Boot Environment"), tr("You cannot rename the boot environment that is currently set as the default!") );
      return;
    }
    QString name = ui->tree_BE->topLevelItem(index)->text(0);
    //Get the new name from the user
    bool ok;
    QString newname = QInputDialog::getText( this, tr("New BE name"), tr("Choose a new name for this boot environment"), QLineEdit::Normal, name, &ok,0, Qt::ImhLowercaseOnly | Qt::ImhUppercaseOnly | Qt::ImhDigitsOnly ); 
    if(ok && !newname.isEmpty()){
      if( !validateInput(newname) ){
        on_tool_BEmv_clicked(); //try again
        return;
      }
      if( checkName(newname) ){
        beadmRename(name,newname);
        updateBEList();
	updateGRUBdefaults(true); //Make sure to rebuild the GRUB menu with the new name
      }
    }
  }
}

void mainUI::on_tool_BErem_clicked(){
  int index = getSelectedBE();
  if(index != -1){
    //qDebug() << "BE Rem num:" << ui->tree_BE->topLevelItemCount();
    QString name = ui->tree_BE->topLevelItem(index)->text(0);
    if(ui->tree_BE->topLevelItemCount() == 1){
      QMessageBox::warning(this,tr("Single Boot Environment"), tr("You cannot remove your only boot environment!") );
      return;
    }
    /*if(ui->tree_BE->topLevelItem(index)->text(0).toLower() == "default"){
      QMessageBox::warning(this,tr("Base Boot Environment"), tr("You cannot remove the base environment that all other BE's need to function!") );
      return;
    }*/
    if(ui->tree_BE->topLevelItem(index)->text(1).toLower() == "yes"){
      QMessageBox::warning(this,tr("Running Boot Environment"), tr("You cannot remove the boot environment that you are currently running!") );
      return;
    }
    if(ui->tree_BE->topLevelItem(index)->text(2).toLower() == "yes"){
      QMessageBox::warning(this,tr("Default Boot Environment"), tr("You cannot remove the boot environment that is currently set as the default!") );
      return;
    }
    //Verify removal
    if( QMessageBox::Yes == QMessageBox::question(this,tr("Verify BE Removal"),tr("Are you sure you want to delete the following boot environment?")+"\n\n"+name, QMessageBox::Yes | QMessageBox::Cancel, QMessageBox::Cancel) ){
      /*if(index <= G_defaultBE){ 
	 if(index < G_defaultBE){ G_defaultBE--; }//make sure we keep the same default BE
	 else{ G_defaultBE=0; } //Removing the default BE
	 saveGRUBdefaults(G_themeFile, G_fontFile, G_timer, G_showMenu);//, G_defaultBE);  
      }*/
      beadmRemove(name);
      updateBEList();
      updateGRUBdefaults(true);
    }
  }	
}

//UI Buttons - GRUB Config
void mainUI::on_tool_GRUBsavedefaults_clicked(){
  //Get all the values
  G_themeFile = ui->line_GRUBthemefile->text();
  G_fontFile = ui->line_GRUBfontfile->text();
  G_timer = ui->spin_GRUBtimer->value();
  G_showMenu = ui->check_GRUBshowcountdown->isChecked();
  //always show the countdown if more than one BE
  if( ui->tree_BE->topLevelItemCount() > 1 ){ G_showMenu=true; }
	
  //Save the values to the file
  bool ok = saveGRUBdefaults(G_themeFile,G_fontFile,G_timer,G_showMenu);//, G_defaultBE);
  if(!ok){
    qDebug() << "Unable to save the GRUB defaults to file:" << file_GRUBdefaults+".new";
    G_goodLoad = loadGRUBdefaults(G_themeFile, G_fontFile, G_timer, G_showMenu);//, G_defaultBE); //reset values
  }else{
    on_action_rebuildGRUBmenu_triggered();
    ui->tool_GRUBsavedefaults->setEnabled(false);
    ui->tool_GRUBresetdefaults->setEnabled(false);    
  }
}

void mainUI::on_tool_GRUBresetdefaults_clicked(){
  G_goodLoad = loadGRUBdefaults(G_themeFile, G_fontFile, G_timer, G_showMenu);//, G_defaultBE);
  updateGRUBdefaults();
}

void mainUI::on_tool_GRUBthemefile_clicked(){
  QString thfile = QFileDialog::getOpenFileName(this, tr("Select a GRUB Theme File"), dir_GRUBthemes,tr("Theme File (theme.txt)") );
  if( !thfile.isEmpty() ){
    ui->line_GRUBthemefile->setText(thfile);
  }
}

void mainUI::on_tool_GRUBfontfile_clicked(){
  QString thfile = QFileDialog::getOpenFileName(this, tr("Select a GRUB Font File"), dir_GRUBfonts,tr("Font File (*.pf2)") );
  if( !thfile.isEmpty() ){
    ui->line_GRUBfontfile->setText(thfile);
  }	
}

void mainUI::on_tool_GRUBsaveentries_clicked(){
  //Get the current text
  QStringList contents = ui->text_GRUBentries->toPlainText().split("\n");
  bool ok = saveGRUBcustomentries(contents);
  if(ok){ 
	  ui->tool_GRUBsaveentries->setEnabled(false);
	  ui->tool_GRUBresetentries->setEnabled(false);
  }else{
    qDebug() << "Error saving custom grub entries to file:" << file_GRUBentries+".new";
  }
}

void mainUI::on_tool_GRUBresetentries_clicked(){
  updateGRUBentries();
}

//UI Buttons - other
void mainUI::on_actionClose_triggered(){
  this->close();
}

void mainUI::on_action_rebuildGRUBmenu_triggered(){
  //Rebuild the GRUB menu
  QString cmd = "grub-mkconfig -o /boot/grub/grub.cfg";
  QString info = tr("Rebuilding GRUB menu system. Please wait.");
  runLongCMD(cmd, info);
}

void mainUI::on_action_restoreGRUBdefaults_triggered(){
  QString cmd = "cp "+file_GRUBdefaults+".old "+file_GRUBdefaults;
  QProcess::execute(cmd);
  //Now refresh the UI
  updateGRUBdefaults(); //Update the display (load the file)
  on_tool_GRUBsaveentries_clicked(); //make sure the defaults are valid for the number of BE's
}

void mainUI::GRUBchangedefaults(){
  ui->tool_GRUBsavedefaults->setEnabled(true);
  ui->tool_GRUBresetdefaults->setEnabled(true);
}

void mainUI::GRUBchangeentries(){
    ui->tool_GRUBsaveentries->setEnabled(true);
    ui->tool_GRUBresetentries->setEnabled(true);
}

void mainUI::on_spin_maxbe_valueChanged(int num){
  //Set the variable
  QStringList contents = confcontents;
  contents << "MAXBE: "+QString::number(num);
  //Save the file
  QFile file(file_pcbsdconf+".tmp");
  if(file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)){
    QTextStream out(&file);
    for(int i=0; i<contents.length(); i++){
      out << contents[i]+"\n";
    }
    file.close();
    file.setPermissions( QFile::permissions(file_pcbsdconf) ); //reset to original permissions
  }
  QProcess::execute("mv "+file_pcbsdconf+".tmp "+file_pcbsdconf);
}

// ===================
// ===== PROTECTED =====
// ===================
void mainUI::closeEvent(QCloseEvent *event){
  bool changes = ui->tool_GRUBsavedefaults->isEnabled() || ui->tool_GRUBsaveentries->isEnabled();
  if(changes){
    //Verify that they want to continue
    QMessageBox::StandardButton button = QMessageBox::warning(this, tr("Unsaved Changes"), tr("You currently have unsaved changes in the GRUB configuration. Do you want to quit anyway?"), QMessageBox::Yes | QMessageBox::Cancel,QMessageBox::Cancel);
    if(button != QMessageBox::Yes){ 
      //Ignore close request
      event->ignore();
      return;
    }
  }
  this->close();
}
