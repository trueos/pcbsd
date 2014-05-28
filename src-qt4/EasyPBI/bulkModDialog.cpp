#include "bulkModDialog.h"
#include "ui_bulkModDialog.h"

BulkModuleDialog::BulkModuleDialog(QWidget *parent) : QDialog(parent), ui(new Ui::BulkModuleDialog){
  ui->setupUi(this); //load the designer file
	isWorking = false;
	stopProc = false;
  pkgList = Backend::getPkgList();
    pkgList.sort();
  //Now setup the UI as needed
  catMenu = new QMenu;
    ui->tool_getcat->setMenu(catMenu);
    // - get the package categories and fill the menu
    QString lastCat;
    for(int i=0; i<pkgList.length(); i++){
      if(pkgList[i].startsWith(lastCat+"/")){ continue; }
      lastCat = pkgList[i].section("/",0,0);
      catMenu->addAction(lastCat);
    }
  //Default values for the running tab
  ui->label_status->setText(tr("Nothing Run Yet"));
  ui->progressBar->setVisible(false);
  //connect the signals/slots
  connect(ui->tool_getdir, SIGNAL(clicked()), this, SLOT(selectBaseDir()) );
  connect(catMenu, SIGNAL(triggered(QAction*)), this, SLOT(selectPkgCategory(QAction*)) );
  connect(ui->push_close, SIGNAL(clicked()), this, SLOT(closeWindow()) );
  connect(ui->push_start, SIGNAL(clicked()), this, SLOT(startWorking()) );
  connect(ui->push_stop, SIGNAL(clicked()), this, SLOT(stopWorking()) );
    // UI update signals
    connect(ui->check_overwrite, SIGNAL(stateChanged(int)), this, SLOT(updateUI()) );
    connect(ui->check_graphical, SIGNAL(stateChanged(int)), this, SLOT(updateUI()) );
    connect(ui->check_other, SIGNAL(stateChanged(int)), this, SLOT(updateUI()) );
    connect(ui->check_server, SIGNAL(stateChanged(int)), this, SLOT(updateUI()) );
    connect(ui->check_text, SIGNAL(stateChanged(int)), this, SLOT(updateUI()) );
    connect(ui->line_basedir, SIGNAL(editingFinished()), this, SLOT(updateUI()) );
    connect(ui->line_category, SIGNAL(editingFinished()), this, SLOT(updateUI()) );
    
  updateUI();
}

BulkModuleDialog::~BulkModuleDialog(){
	
}

//==============
//    PRIVATE SLOTS
//==============
void BulkModuleDialog::updateUI(){
  bool ok = true;
  if(pkgList.filter(ui->line_category->text()+"/").length() <= 0){ ok = false; }
  else if( !QFile::exists(ui->line_basedir->text()) ){ ok = false; }
  else if( isWorking ){ ok = false; }
  ui->push_start->setEnabled(ok);
  ui->push_stop->setEnabled(isWorking);
  ui->push_close->setEnabled(!isWorking);
}

void BulkModuleDialog::startWorking(){
  stopProc = false;
  isWorking = true;
  skipped = invalid = gnew = snew = tnew = onew = 0; //reset the counters
  ui->progressBar->setVisible(true);
  //Disable parts of the UI
  ui->group_setup->setEnabled(false);
  ui->push_start->setEnabled(false);
  ui->push_stop->setEnabled(true);
  ui->push_close->setEnabled(false);
  ui->tabWidget->setCurrentWidget(ui->tab_run);
  //Get the list of packages/modules to check
  QStringList pkgs = pkgList.filter(ui->line_category->text()+"/");
  ui->progressBar->setRange(0,pkgs.length());
  //Now start checking each pkg/module
  QString modBaseCat = ui->line_basedir->text() + "/"+ ui->line_category->text();
  for(int i=0; i<pkgs.length() && !stopProc; i++){
    //Update statistics
    ui->label_graphicalnum->setText( QString::number(gnew) );
    ui->label_invalidnum->setText( QString::number(invalid) );
    ui->label_servernum->setText( QString::number(snew) );
    ui->label_skipnum->setText( QString::number(skipped) );
    ui->label_textnum->setText( QString::number(tnew) );
    ui->label_othernum->setText( QString::number(onew) );
    ui->label_status->setText( QString(tr("Checking %1")).arg(pkgs[i]) );
    ui->progressBar->setValue(i+1);
    //Check if this module already exists
    if( QFile::exists(modBaseCat+"/"+pkgs[i].section("/",-1)) ){
      if(ui->check_overwrite->isChecked()){
        //Remove the currently existing module
	ui->label_status->setText( QString(tr("%1: Removing old module")).arg(pkgs[i]) );
	QString cmd = "rm -rf "+modBaseCat+"/"+pkgs[i].section("/",-1);
	system(cmd.toUtf8());
      }else{
	//Already exists - skip this module
	skipped++;
	continue;
      }
    }
    //Check type of pkg and act appropriately
    //ui->label_status->setText( QString(tr("%1: Retrieving package file list")).arg(pkgs[i]) );
    QStringList plist = Backend::getPkgPList(pkgs[i]);
    bool hasBinaries = (plist.filter("/bin/").length()>0 || plist.filter("/sbin/").length()>0);
    bool hasIcons = (plist.filter(".png").length() > 0 || plist.filter(".jpg").length() > 0 || plist.filter(".svg").length() > 0 );
    if( hasBinaries && hasIcons && ui->check_graphical->isChecked() ){
      //New Graphical App Module
      ui->label_status->setText( QString(tr("%1: New graphical module")).arg(pkgs[i]) );
      PBIModule MOD = ModuleUtils::newModule(modBaseCat, pkgs[i], "", &plist, true);
      gnew++;
    }else if( (plist.filter("/etc/rc.d/").length()>0) && ui->check_server->isChecked() ){
      //New Server App Module
      ui->label_status->setText( QString(tr("%1: New server module")).arg(pkgs[i]) );
      PBIModule MOD = ModuleUtils::newModule(modBaseCat, pkgs[i], "", &plist, true);
      snew++;	
    }else if(  hasBinaries && ui->check_text->isChecked() ){
      //New Text App Module
      ui->label_status->setText( QString(tr("%1: New text module")).arg(pkgs[i]) );
      PBIModule MOD = ModuleUtils::newModule(modBaseCat, pkgs[i], "", &plist, true);
      tnew++;	    
    }else if(ui->check_other->isChecked()){
      //New Other App Module
      ui->label_status->setText( QString(tr("%1: New other module")).arg(pkgs[i]) );
      PBIModule MOD = ModuleUtils::newModule(modBaseCat, pkgs[i], "", &plist, true);
      onew++;	    
    }else{
      //invalid pkg (does not match selected options)
      invalid++;
    }
  }
  //Now re-enable the UI
  ui->group_setup->setEnabled(true);
  ui->push_close->setEnabled(true);
  if(stopProc){
    ui->label_status->setText( QString(tr("Category Stopped: %1")).arg(ui->line_category->text()) );	  
  }else{
    ui->label_status->setText( QString(tr("Category Finished: %1")).arg(ui->line_category->text()) );
  }
  isWorking = false;
  updateUI();
}

void BulkModuleDialog::stopWorking(){
  stopProc = true;
}

void BulkModuleDialog::closeWindow(){
  stopProc = true; //just in case it is running - should stop it quickly
  this->close();
}

void BulkModuleDialog::selectBaseDir(){
  QString dir = QFileDialog::getExistingDirectory(this, tr("Base Module Directory"), QDir::homePath());
  if(dir.isEmpty()){ return; }
  ui->line_basedir->setText(dir);
  updateUI();
}

void BulkModuleDialog::selectPkgCategory(QAction* act){
  ui->line_category->setText( act->text() );
  updateUI();
}

