#include "preferencesDialog.h"
#include "ui_preferencesDialog.h"
#include <QDebug>

PreferencesDialog::PreferencesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferencesDialog)
{
	
       //Setup Initial state of GUI objects and connect signal/slots
       ui->setupUi(this);  //load the main UI from designer
	//Setup the dialog icons
	ui->push_pbioutdir->setIcon(Backend::icon("load"));
	ui->push_sigfile->setIcon(Backend::icon("file"));
	ui->push_clearcache->setIcon(Backend::icon("clear"));
	ui->push_moduledir->setIcon(Backend::icon("load"));
	ui->push_icondir->setIcon(Backend::icon("load"));
	ui->push_defaulticon->setIcon(Backend::icon("file"));
	ui->push_suutility->setIcon(Backend::icon("file"));
	ui->push_makeport->setIcon(Backend::icon("file"));
	ui->push_pbicreate->setIcon(Backend::icon("file"));
	ui->push_resetutils->setIcon(Backend::icon("refresh"));
	ui->tool_otherPortsDir->setIcon(Backend::icon("load"));
	ui->push_updatePorts->setIcon(Backend::icon("refresh"));
	
	//Setup the internal settings structure
	settings = new Config();
	settings->scanForExternalUtilities();	
	settings->loadSettingsFile();
	//initialize the QProcess
	process = new QProcess(this);
	process->setProcessEnvironment( QProcessEnvironment::systemEnvironment() );
	connect(process,SIGNAL(readyReadStandardOutput()),this,SLOT(updateStatusBar()) );
	connect(process,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(processFinished(int,QProcess::ExitStatus)) );
	//Put the current settings into the GUI
	resetGUI();
}

PreferencesDialog::~PreferencesDialog(){
}

//Setup the UI
void PreferencesDialog::resetGUI(){
  //Put the current settings into all the dialog widgets
	//pbi tab
	ui->line_pbioutdir->setText(settings->value("pbidir"));
	ui->check_signpbi->setChecked(settings->check("usesignature"));
	ui->line_sigfile->setText(settings->value("sigfile"));
	ui->check_usetmpfs->setChecked(settings->check("usetmpfs"));
	ui->check_usepkgcache->setChecked(settings->check("usecache"));
	ui->list_badpkg->clear();
	ui->list_badpkg->addItems(settings->list("skippkgs"));
	//paths tab
	ui->line_moduledir->setText(settings->value("moduledir"));
	ui->line_icondir->setText(settings->value("icondir"));
	ui->line_defaulticon->setText(settings->value("defaulticon"));
	ui->line_suutility->setText(settings->value("su_cmd"));
	ui->line_pbicreate->setText(settings->value("pbi_create"));
	ui->line_makeport->setText(settings->value("pbi_makeport"));
	//ports tab
	ui->list_portsDirectories->clear();
	QStringList pDirs; pDirs<< tr("Current User (~/EasyPBI/ports)") << tr("All Users (/usr/ports)") << tr("Other");
	ui->list_portsDirectories->addItems(pDirs);
	QString portsdir = settings->value("portsdir");
	if(portsdir.endsWith("/")){ portsdir.chop(1); }
	if(portsdir == (QDir::homePath()+"/EasyPBI/ports") ){
	  ui->list_portsDirectories->setCurrentIndex(0); //User dir
	}else if(portsdir == "/usr/ports"){
	  ui->list_portsDirectories->setCurrentIndex(1); //system dir	
	}else{
	  ui->list_portsDirectories->setCurrentIndex(2); //other
	  ui->line_otherPortsDir->setText(portsdir);
	}
	ui->progressBar->setMaximum(0); ui->progressBar->setMinimum(0); //default to a "loading" indicator
	updateGUI();
}

void PreferencesDialog::updateGUI(){
	//Now adjust the visibility of the necessary elements
	//pbi tab
	if( ui->check_signpbi->isChecked() ){ ui->groupBox_sig->setVisible(TRUE); }
	else{ ui->groupBox_sig->setVisible(FALSE); }
	if( ui->check_usepkgcache->isChecked() ){ ui->groupBox_pkgcache->setVisible(TRUE); }
	else{ ui->groupBox_pkgcache->setVisible(FALSE); }
	//ports tab
	int pIndex = ui->list_portsDirectories->currentIndex();
	if(pIndex == 2){
	  ui->line_otherPortsDir->setVisible(TRUE); ui->tool_otherPortsDir->setVisible(TRUE);
	}else{
	  ui->line_otherPortsDir->setVisible(FALSE); ui->tool_otherPortsDir->setVisible(FALSE);	
	}
	QString pDir;
	switch (pIndex){
	case 0: 
		pDir = QDir::homePath()+"/EasyPBI/ports"; break;
	case 1:
		pDir = "/usr/ports"; break;
	default:
		pDir = ui->line_otherPortsDir->text();
	}	
	QString pUpdate;
	if(!pDir.isEmpty()){ pUpdate = getLastPortsUpdate(pDir); } //check for existance and last update
	if( !pUpdate.isEmpty() ){
	  ui->push_fetchPorts->setVisible(FALSE);
	  ui->push_updatePorts->setVisible(TRUE);
	  ui->label_portsUpdated->setText( pUpdate );
	}else{
	  ui->push_fetchPorts->setVisible(TRUE);
	  ui->push_updatePorts->setVisible(FALSE);
	  ui->label_portsUpdated->setText( tr("Nonexistent") );
	}
	if(process->state() == QProcess::NotRunning){
	  ui->list_portsDirectories->setEnabled(TRUE);
	  ui->line_otherPortsDir->setEnabled(TRUE); ui->tool_otherPortsDir->setEnabled(TRUE);
	  ui->label_status->setVisible(FALSE); ui->progressBar->setVisible(FALSE);
	  ui->push_fetchPorts->setEnabled(TRUE); ui->push_updatePorts->setEnabled(TRUE);
	  ui->buttonBox->setEnabled(TRUE);
	}else{
	  ui->list_portsDirectories->setEnabled(FALSE);
	  ui->line_otherPortsDir->setEnabled(FALSE); ui->tool_otherPortsDir->setEnabled(FALSE);
	  ui->label_status->setVisible(TRUE); ui->progressBar->setVisible(TRUE);
	  ui->push_fetchPorts->setEnabled(FALSE); ui->push_updatePorts->setEnabled(FALSE);
	  ui->buttonBox->setEnabled(FALSE);
	}
}

//Save/Cancel buttons
void PreferencesDialog::on_buttonBox_accepted(){
    //Save the settings in the GUI to the stucture
    settings->setValue("pbidir", ui->line_pbioutdir->text() );
    settings->setCheck("usesignature", ui->check_signpbi->isChecked() );
    settings->setValue("sigfile", ui->line_sigfile->text() );
    settings->setCheck("usetmpfs", ui->check_usetmpfs->isChecked() );
    settings->setCheck("usecache", ui->check_usepkgcache->isChecked() );
    QStringList badpkgs;
    for(int i=0; i<ui->list_badpkg->count(); i++){
      badpkgs << ui->list_badpkg->item(i)->text();
    }
    badpkgs.removeDuplicates();
    settings->setList("skippkgs", badpkgs);
    QString portsDir;
    int pIndex = ui->list_portsDirectories->currentIndex();
    if( pIndex == 0 ){portsDir = QDir::homePath()+"/EasyPBI/ports"; }
    else if( pIndex == 1){ portsDir = "/usr/ports"; }
    else{ portsDir = ui->line_otherPortsDir->text(); }    
    settings->setValue("portsdir", portsDir );
    settings->setValue("moduledir", ui->line_moduledir->text() );
    settings->setValue("icondir", ui->line_icondir->text() );
    settings->setValue("defaulticon", ui->line_defaulticon->text() );
    settings->setValue("su_cmd", ui->line_suutility->text() );
    settings->setValue("pbi_create", ui->line_pbicreate->text() );
    settings->setValue("pbi_makeport", ui->line_makeport->text() );
    //Now save the settings to disk
    settings->saveSettingsFile();
    //Now close the dialog
    this->close();
}

void PreferencesDialog::on_buttonBox_rejected(){
    //Don't save any of the settings and just close the dialog
    this->close();
}

// ===== PBI Settings Tab =====
void PreferencesDialog::on_push_pbioutdir_clicked(){
  //Get the desired directory
  QString seldir = QFileDialog::getExistingDirectory(this, tr("Select PBI Output Directory"), QDir::homePath() );
  if(seldir.isEmpty()){ return; } //action cancelled
  //Save the selection to the GUI
  ui->line_pbioutdir->setText(seldir); 
}

void PreferencesDialog::on_push_sigfile_clicked(){
  //Get the desired directory
  QString seldir = QFileDialog::getOpenFileName(this, tr("Select Digital Signature File"), QDir::homePath(), tr("Signature Files (*)") );
  if(seldir.isEmpty()){ return; } //action cancelled
  //Save the selection to the GUI
  ui->line_sigfile->setText(seldir); 
}

void PreferencesDialog::on_push_addpkg_clicked(){
  //Get the desired directory
  QString seldir = QFileDialog::getOpenFileName(this, tr("Select the Package to Ignore"), settings->value("cachedir"), tr("Package Files (*.txz)") );
  if(seldir.isEmpty()){ return; } //action cancelled
  //get just the package name (ignore the version and extension)
  QString pkg = seldir.remove(seldir.section("-",-1));
  pkg.append("*");
  //Save the selection to the GUI
  ui->list_badpkg->addItem(pkg); 
}

void PreferencesDialog::on_push_rmpkg_clicked(){
  //get the currently selected item
  int row = ui->list_badpkg->currentRow();
  if(row == -1){ return; } //nothing selected
  //Remove it from the list
  delete ui->list_badpkg->takeItem(row);
}

void PreferencesDialog::on_push_clearcache_clicked(){
  //Verify that this is what the user really wants to do
  QMessageBox verify(this);
  verify.setText(tr("Are you sure you wish to clear the PBI package cache?"));
  verify.setInformativeText(tr("This will delete all the files and directories in the cache directory. The next time you start a PBI build, it will begin to rebuild all the packages from source."));
  verify.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
  verify.setDefaultButton(QMessageBox::Yes);
  int ret = verify.exec();
  if(ret != QMessageBox::Yes){return;}
  //Remove all files/directories in the EasyPBI cache dir
  QString cmd = "cd "+settings->value("cachedir")+"; rm -rf *";
  system(cmd.toUtf8());
}

void PreferencesDialog::on_check_signpbi_stateChanged(int state){
  if(state == Qt::Checked){ ui->groupBox_sig->setVisible(TRUE); }
  else{ ui->groupBox_sig->setVisible(FALSE); }
}

void PreferencesDialog::on_check_usepkgcache_stateChanged(int state){
  if(state == Qt::Checked){ ui->groupBox_pkgcache->setVisible(TRUE); }
  else{ ui->groupBox_pkgcache->setVisible(FALSE); }
}

// ===== Paths Tab =====  
void PreferencesDialog::on_push_moduledir_clicked(){
  //Get the desired directory
  QString seldir = QFileDialog::getExistingDirectory(this, tr("Select Default Module Directory"), QDir::homePath() );
  if(seldir.isEmpty()){ return; } //action cancelled
  //Save the selection to the GUI
  ui->line_moduledir->setText(seldir); 
}

void PreferencesDialog::on_push_icondir_clicked(){
  //Get the desired directory
  QString seldir = QFileDialog::getExistingDirectory(this, tr("Select Default Directory To Find Icon Files"), QDir::homePath() );
  if(seldir.isEmpty()){ return; } //action cancelled
  //Save the selection to the GUI
  ui->line_icondir->setText(seldir); 
}
void PreferencesDialog::on_push_defaulticon_clicked(){
  //Get the desired directory
  QString seldir = QFileDialog::getOpenFileName(this, tr("Select Default Icon FIle"), QDir::homePath(), tr("PNG Images (*.png)") );
  if(seldir.isEmpty()){ return; } //action cancelled
  //Save the selection to the GUI
  ui->line_defaulticon->setText(seldir); 
}

// ===== Utilities Tab =====
void PreferencesDialog::on_push_suutility_clicked(){
  //Get the desired directory
  QString seldir = QFileDialog::getOpenFileName(this, tr("Select Graphical Switch-User Utility"), QDir::homePath() );
  if(seldir.isEmpty()){ return; } //action cancelled
  //Save the selection to the GUI
  ui->line_suutility->setText(seldir); 
}

void PreferencesDialog::on_push_pbicreate_clicked(){
  //Get the desired directory
  QString seldir = QFileDialog::getOpenFileName(this, tr("Select 'pbi_create' File Location"), QDir::homePath(), "(pbi_create)" );
  if(seldir.isEmpty()){ return; } //action cancelled
  //Save the selection to the GUI
  ui->line_pbicreate->setText(seldir); 
}

void PreferencesDialog::on_push_makeport_clicked(){
  //Get the desired directory
  QString seldir = QFileDialog::getOpenFileName(this, tr("Select 'pbi_makeport' File Location"), QDir::homePath(), "(pbi_makeport)" );
  if(seldir.isEmpty()){ return; } //action cancelled
  //Save the selection to the GUI
  ui->line_makeport->setText(seldir); 
}

void PreferencesDialog::on_push_resetutils_clicked(){
  //Verify that this is what the user really wants to do
  QMessageBox verify(this);
  verify.setText(tr("Are you sure you wish to return to the automatically detected utility paths?"));
  verify.setInformativeText(tr("This will remove any customized path settings for the external utilities."));
  verify.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
  verify.setDefaultButton(QMessageBox::Yes);
  int ret = verify.exec();
  if(ret != QMessageBox::Yes){return;}
  //Rescan for external utilities
  settings->scanForExternalUtilities();
  //Reset the values in the structure (will revert to detected values if any)
  settings->setValue("pbi_makeport","");
  settings->setValue("pbi_create","");
  settings->setValue("su_cmd","");
  //Reset the utilities on the GUI	
  ui->line_suutility->setText(settings->value("su_cmd"));
  ui->line_pbicreate->setText(settings->value("pbi_create"));
  ui->line_makeport->setText(settings->value("pbi_makeport"));
}

// ======  Ports Tab ======
void PreferencesDialog::on_list_portsDirectories_currentIndexChanged(int index){
  updateGUI();	
}

void PreferencesDialog::on_line_otherPortsDir_textChanged(QString text){
  updateGUI();	
}
void PreferencesDialog::on_tool_otherPortsDir_clicked(){
  //Get the desired directory
  QString seldir = QFileDialog::getExistingDirectory(this, tr("Select FreeBSD Ports Directory"), QDir::homePath() );
  if(seldir.isEmpty()){ return; } //action cancelled
  //Save the selection to the GUI
  ui->line_otherPortsDir->setText(seldir);
  updateGUI();
}

void PreferencesDialog::on_push_fetchPorts_clicked(){
  int pIndex = ui->list_portsDirectories->currentIndex();
  QString pDir;
    if( pIndex == 0 ){pDir = QDir::homePath()+"/EasyPBI/ports"; }
    else if( pIndex == 1){ pDir = "/usr/ports"; }
    else{ pDir = ui->line_otherPortsDir->text(); }
  fetchPorts(pDir,TRUE); //is a new ports directory
  updateGUI();
}

void PreferencesDialog::on_push_updatePorts_clicked(){
  int pIndex = ui->list_portsDirectories->currentIndex();
  QString pDir;
    if( pIndex == 0 ){pDir = QDir::homePath()+"/EasyPBI/ports"; }
    else if( pIndex == 1){ pDir = "/usr/ports"; }
    else{ pDir = ui->line_otherPortsDir->text(); }
  fetchPorts(pDir,FALSE); //is an existing ports directory
  updateGUI();
}
    
void PreferencesDialog::updateStatusBar(){
  QString tmp = process->readAllStandardOutput();
  if(tmp.endsWith("\n")){ tmp.chop(1); }
  tmp = tmp.section("\n",-1);
  tmp = tmp.remove("\n").simplified();
  //qDebug() << "Status Update:" << tmp;
  ui->label_status->clear();
  ui->label_status->setText(tmp);
}

void PreferencesDialog::processFinished(int exitCode,QProcess::ExitStatus status){
  qDebug() << " - Ports update finished:" << exitCode << status;
  //Inform the user of the result
  if(exitCode == 0){
    //Finished successfully
    QMessageBox::information(this, tr("Success"), tr("The FreeBSD ports tree has been successfully updated") );
  }else{
    //Finished with error
    QString msg = process->readAllStandardError();
    if(msg.isEmpty()){ msg = process->readAllStandardOutput(); }
    QMessageBox::information(this, tr("Failure"), QString(tr("The FreeBSD ports tree has failed to update.")+"\n"+tr("Last Message: %1") ).arg(msg) );
  }
  updateGUI(); //Update the interface times and such
}

QString PreferencesDialog::getLastPortsUpdate(QString ports){
  QDir dir(ports);
  QFileInfoList list = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs, QDir::Time); //sort by most recently changed
  QString out;
  if(list.length() > 2){ //good ports tree
    out = list[0].lastModified().toString();
  }
  return out;
}

void PreferencesDialog::fetchPorts(QString portsDir, bool isNew){
  bool needRoot = TRUE;
  QString localPSDB = QDir::homePath()+"/EasyPBI/.portSnapDB";
  if( portsDir.startsWith( QDir::homePath() )){ needRoot = FALSE; }
  
  // == Generate the System Command ==
  QString cmd;
  bool usePortsnap = FALSE;
  //Graphical SU utility as needed
  if(needRoot){ 
    ui->label_status->setText(tr("Requesting Root Access")); //Put placeholder info
    cmd = ui->line_suutility->text() + " \""; //use the switch-user utility
  }
  //Figure out whether to use portsnap or not
  if( !isNew && QFile::exists(portsDir+"/.svn") ){
    //Use Subversion
    cmd.append("svn update "+portsDir);
  }else{
    //Use Portsnap
    cmd.append("portsnap fetch ");
    if(isNew){ cmd.append("extract "); }
    else{ cmd.append("update "); }
    cmd.append("-p "+portsDir);
    if(!needRoot){ cmd.append(" -d "+localPSDB); }
    usePortsnap=TRUE;
    
  }
  if(needRoot){ cmd.append("\""); } //close the quotes
  
  //If using portsnap, make sure the intermediate file is there in the EasyPBI directory
  if( usePortsnap ){
    QString pspath = QDir::homePath()+"/EasyPBI/.portsnapNI.sh"; // Non-Interactive (NI) version of portsnap
    if( !QFile::exists(pspath) ){
      //Create the Non-Interactive version of portsnap if it does not exist yet
      QString pscmd ="echo \"#!/bin/sh \n#Non-Interactive portsnap workaround for EasyPBI \ncat /usr/sbin/portsnap | sed 's|! -t 0|-z '1'|g' | /bin/sh -s \\$@\" > "+pspath+"; chmod 755 "+pspath;
      //qDebug() << "system call:" << pscmd;
      system(pscmd.toUtf8());
    }
    if(!needRoot && !QFile::exists(localPSDB)){
      //Create the local portsnap database directory
      QString tmpcmd = "mkdir "+localPSDB;
      system(tmpcmd.toUtf8());
    }
    cmd.replace("portsnap",pspath);
  }
  //Now run the command
  process->start(cmd);
  qDebug() << "Ports fetch started: CMD:" << cmd;	
}
