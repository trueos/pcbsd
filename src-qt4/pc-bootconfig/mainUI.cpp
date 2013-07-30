#include "mainUI.h"
#include "ui_mainUI.h"

mainUI::mainUI(QWidget *parent) : QMainWindow(parent), ui(new Ui::mainUI){
	  
  qDebug() << "Starting up pc-bootconfig...";
  ui->setupUi(this);  //load the mainUI.ui file
  //Set paths to GRUB files
  file_GRUBdefaults="/usr/local/etc/default/grub";
  file_GRUBentries="/boot/grub/custom.cfg";
  dir_GRUBthemes="/boot/grub/themes/";
  dir_GRUBfonts="/boot/grub/";
  //Make sure that backups exist of the GRUB configuration file
  if(!QFile::exists(file_GRUBdefaults+".old")){ 
    QString cmd = "cp "+file_GRUBdefaults+" "+file_GRUBdefaults+".old";
    system(cmd.toUtf8());
  }
  //initialize the QProcess
  proc = new QProcess(this);
    proc->setProcessChannelMode(QProcess::MergedChannels);
  
  //Update the list of available boot environments
  updateBEList();
  //Update the GRUB configuration values
  updateGRUBdefaults();
  //Update the custom GRUB entries
  updateGRUBentries();
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

void mainUI::runLongCMD(QString cmd){
  //Initialize the Working Message Box
  QMessageBox wrkmsg(this);
    wrkmsg.setWindowTitle( tr("Working") );
    wrkmsg.setText( tr("Performing Boot Environment Changes. Please Wait.") );
    wrkmsg.setStandardButtons(0);
    wrkmsg.setWindowModality(Qt::WindowModal);
  //Show the message
  QCoreApplication::processEvents();
  wrkmsg.show();
  QCoreApplication::processEvents();	
  QCoreApplication::processEvents();	
  //Run the command
  system(cmd.toUtf8());
  //Close the message box
  wrkmsg.close();
}


//Boot Environment Management Functions
void mainUI::beadmActivate(QString name){ 
	QString cmd = "beadm activate "+name;
	runLongCMD(cmd);
}

void mainUI::beadmCreate(QString name){ 
	QString cmd ="beadm create "+name;
	runLongCMD(cmd);
}

void mainUI::beadmCopy(QString name, QString newname){ 
	QString cmd ="beadm create -e "+name+" "+newname;
	runLongCMD(cmd);
}

void mainUI::beadmRemove(QString name){ 
	QString cmd ="beadm destroy -F "+name;
	runLongCMD(cmd);
}

void mainUI::beadmRename(QString name, QString newname){ 
	QString cmd ="beadm rename "+name+" "+newname;
	runLongCMD(cmd);
}

void mainUI::beadmMount(QString name, QString mntpt){ 
	QString cmd ="beadm mount "+name+" "+mntpt;
	runLongCMD(cmd);
}

void mainUI::beadmUnmount(QString name){ 
	QString cmd ="beadm umount "+name;
	runLongCMD(cmd);
}

QStringList mainUI::beadmList(){
	//Output format (per line): <name>::<isrunning>::<isactivated>::<mountpoint>::<space>::<datecreated>::<timecreated>
	
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
bool mainUI::loadGRUBdefaults(QString &themefile, QString &fontfile, int &countdown, bool &showcountdown){
  QFile file(file_GRUBdefaults);
  if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){ return false; }
  QTextStream in(&file);
  themefile.clear(); fontfile.clear();
  while( !in.atEnd() ){
    QString line = in.readLine();
    QString var = line.section("=",0,0); //variable
    QString val = line.section("=",1,10); //value
    if(var == "GRUB_THEME"){ themefile.append(val); }
    else if(var == "GRUB_FONT"){ fontfile.append(val); }
    else if(var == "GRUB_HIDDEN_TIMEOUT"){ countdown = val.toInt(); }
    else if(var == "GRUB_HIDDEN_TIMEOUT_QUIET"){ showcountdown = (val != "true"); }
  }
  file.close();
  //qDebug() << "GRUB defaults:" << themefile << fontfile << countdown << showcountdown;
  return true;	
}

bool mainUI::saveGRUBdefaults(QString themefile, QString fontfile, int countdown, bool showcountdown){
  QFile file(file_GRUBdefaults+".new");
  if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)){ return false; }
  QTextStream out(&file);
  if(!themefile.isEmpty()){ out << "GRUB_THEME="+themefile+"\n"; }
  if(!fontfile.isEmpty()){ out << "GRUB_FONT="+fontfile+"\n"; }
  out << "GRUB_HIDDEN_TIMEOUT="+ QString::number(countdown)+"\n";
  if(showcountdown){ out << "GRUB_HIDDEN_TIMEOUT_QUIET=false\n"; }
  else{ out << "GRUB_HIDDEN_TIMEOUT_QUIET=true\n"; }
  file.close();
  QString cmd="mv "+file_GRUBdefaults+".new "+file_GRUBdefaults;
  system(cmd.toUtf8());
  return true;		
}

bool mainUI::loadGRUBcustomentries(QStringList &filecontents){
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
  QFile file(file_GRUBentries+".new");
  if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)){ return false; }
  QTextStream out(&file);
  for(int i=0; i<filecontents.length(); i++){
    out << filecontents[i]+"\n";
  }
  file.close();
  QString cmd="mv "+file_GRUBentries+".new "+file_GRUBentries;
  system(cmd.toUtf8());
  return true;	
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
      cols << blist[i].section("::",2,2); // [2] is activated for next boot
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

void mainUI::updateGRUBdefaults(){
  QString themefile, fontfile;
  int timeout;
  bool showcountdown;
  if( loadGRUBdefaults(themefile, fontfile, timeout, showcountdown) ){
    //Load the info into the UI
    ui->line_GRUBthemefile->setText(themefile);
    ui->line_GRUBfontfile->setText(fontfile);
    ui->spin_GRUBtimer->setValue(timeout);
    ui->check_GRUBshowcountdown->setChecked(showcountdown);
    ui->group_GRUBsettings->setEnabled(true);
  }else{
    //Could not read the GRUB defaults file
    ui->group_GRUBsettings->setEnabled(false);
  }
  //Make sure that the save button is disabled - no changes yet
  //ui->tool_GRUBsavedefaults->setEnabled(false);
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
  //ui->tool_GRUBsaveentries->setEnabled(false);
}

//UI Buttons - Boot Environments
void mainUI::on_tool_BEadd_clicked(){
  //Get the new name from the user
  bool ok;
  QString newname = QInputDialog::getText( this, tr("New BE name"), tr("Choose a name for the new boot environment"), QLineEdit::Normal, "", &ok); 
  if(ok && !newname.isEmpty()){
    if( checkName(newname) ){
      beadmCreate(newname);
      updateBEList();
    }
  }	
}

void mainUI::on_tool_BEactivate_clicked(){
  int index = getSelectedBE();
  if(index != -1){
    QString name = ui->tree_BE->topLevelItem(index)->text(0);
    beadmActivate(name);
    updateBEList();
  }
}

void mainUI::on_tool_BEcp_clicked(){
 int index = getSelectedBE();
  if(index != -1){
    QString name = ui->tree_BE->topLevelItem(index)->text(0);
    //Get the new name from the user
    bool ok;
    QString newname = QInputDialog::getText( this, tr("New BE name"), tr("Choose a name for the new boot environment"), QLineEdit::Normal, name, &ok); 
    if(ok && !newname.isEmpty()){
      if( checkName(newname) ){
        beadmCopy(name,newname);
        updateBEList();
      }
    }
  }	
}

void mainUI::on_tool_BEmv_clicked(){
  int index = getSelectedBE();
  if(index != -1){
    QString name = ui->tree_BE->topLevelItem(index)->text(0);
    //Get the new name from the user
    bool ok;
    QString newname = QInputDialog::getText( this, tr("New BE name"), tr("Choose a new name for this boot environment"), QLineEdit::Normal, name, &ok); 
    if(ok && !newname.isEmpty()){
      if( checkName(newname) ){
        beadmRename(name,newname);
        updateBEList();
      }
    }
  }
}

void mainUI::on_tool_BErem_clicked(){
  int index = getSelectedBE();
  if(index != -1){
    QString name = ui->tree_BE->topLevelItem(index)->text(0);
    if(ui->tree_BE->topLevelItemCount() == 1){
      QMessageBox::warning(this,tr("Only Boot Environment"), tr("You cannot remove your only boot environment!") );
      return;
    }
    //Verify removal
    if( QMessageBox::Yes == QMessageBox::question(this,tr("Verify BE Removal"),tr("Are you sure you want to delete the following boot environment?")+"\n\n"+name, QMessageBox::Yes | QMessageBox::Cancel, QMessageBox::Cancel) ){
      beadmRemove(name);
      updateBEList();
    }
  }	
}

//UI Buttons - GRUB Config
void mainUI::on_tool_GRUBsavedefaults_clicked(){
  //Get all the values
  QString themefile = ui->line_GRUBthemefile->text();
  QString fontfile = ui->line_GRUBfontfile->text();
  int countdown = ui->spin_GRUBtimer->value();
  bool showcountdown = ui->check_GRUBshowcountdown->isChecked();
	
  //Save the values to the file
  bool ok = saveGRUBdefaults(themefile,fontfile,countdown,showcountdown);
  if(!ok){
    qDebug() << "Unable to save the GRUB defaults to file:" << file_GRUBdefaults+".new";
  }
}

void mainUI::on_tool_GRUBresetdefaults_clicked(){
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
  if(ok){ }//ui->tool_GRUBsaveentries->setEnabled(false); }
  else{
    qDebug() << "Error saving custom grub entries to file:" << file_GRUBentries+".new";
  }
}

void mainUI::on_tool_GRUBresetentries_clicked(){
  updateGRUBentries();
}

//UI Buttons - other
void mainUI::on_tool_close_clicked(){
  this->close();
}
