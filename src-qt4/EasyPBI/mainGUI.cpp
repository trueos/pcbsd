#include "mainGUI.h"
#include "ui_mainGUI.h"
#include "backend.h"
#include "newModuleDialog.h"
#include "preferencesDialog.h"
#include "aboutDialog.h"

MainGUI::MainGUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainGUI)
{
	// NOTE: "plus"/"minus" icons are set in QtDesigner using the current theme
	//	since none are available in the Qt Embedded resources
	
       //Setup Initial state of GUI objects and connect signal/slots
       ui->setupUi(this);  //load the mainGUI.ui file
	XDGUPDATING=false;
	lastModuleDir.clear();
	// Create the config class
	settings = new Config();
	//Setup the Menu items
	ui->actionExit->setIcon(Backend::icon("close"));
	ui->actionRefresh_Module->setIcon(Backend::icon("refresh"));
	//Setup the pushbutton menu lists
	QStringList tmp = ModuleUtils::validExternalLinkTypes();
	for(int i=0; i<tmp.length(); i++){ menu_elOpts.addAction(tmp[i]); }
	tmp = ModuleUtils::validRepoTypes();
	for(int i=0; i<tmp.length(); i++){ menu_validRepoTypes.addAction(tmp[i]); }
	//Setup Toolbar
	ui->actionNew_Module->setIcon(Backend::icon("new"));
	ui->actionLoad_Module->setIcon(Backend::icon("load"));
	line_module = new QLineEdit(this);
	line_module->setPlaceholderText(tr("No Module Loaded"));
	line_module->setReadOnly(TRUE);
	line_module->setFocusPolicy(Qt::NoFocus);
	ui->toolBar->addWidget(line_module);
	ui->toolBar->addSeparator();
	//Setup Module Editor
	connect(ui->tabWidget, SIGNAL(currentChanged(int)),this,SLOT(slotModTabChanged(int)) ); //setup to refresh each tab once it is selected
	// -- pbi.conf tab --
	ui->push_change_makeport->setIcon(Backend::icon("file"));
	ui->push_config_save->setIcon(Backend::icon("save"));
	ui->tool_repoType->setIcon(Backend::icon("left"));
	  ui->tool_repoType->setMenu(&menu_validRepoTypes);
	  connect(&menu_validRepoTypes, SIGNAL(triggered(QAction*)), this, SLOT(slotSetRepoType(QAction*)) );
	
	// -- XDG entries tab --
	connect(ui->radio_xdg_desktop,SIGNAL(clicked()),this,SLOT(slotXdgTypeChanged()) );
	connect(ui->radio_xdg_menu,SIGNAL(clicked()),this,SLOT(slotXdgTypeChanged()) );
	connect(ui->list_xdg_files,SIGNAL(itemSelectionChanged()), this, SLOT(slotXdgFileChanged()) );
	ui->push_xdg_refresh->setIcon(Backend::icon("refresh"));
	connect(ui->push_xdg_refresh, SIGNAL(clicked()), this, SLOT(slotXdgTypeChanged()) );
	ui->push_xdg_exec->setIcon(Backend::icon("left"));
	//ui->push_xdg_exec->setMenu(&menu_bins);
	ui->push_xdg_exec->setEnabled(false);
	//connect(&menu_bins, SIGNAL(triggered(QAction*)), this, SLOT(slotAddBin(QAction*)) );
	ui->push_xdg_savechanges->setIcon(Backend::icon("save"));
	ui->push_xdg_menu->setIcon(Backend::icon("left"));
	ui->push_xdg_menu->setMenu(&menu_validMenuCats);
	connect(&menu_validMenuCats,SIGNAL(triggered(QAction*)),this,SLOT(slotAddMenuCat(QAction*)) );
	ui->push_xdg_clearentry->setIcon(Backend::icon("file"));
	
	// -- Scripts tab --
	ui->push_scripts_save->setIcon(Backend::icon("save"));
	ui->push_scripts_create->setIcon(Backend::icon("new"));
	ui->list_scripts_file->addItems(QStringList() << " ---"+tr("Installation Scripts")+"---" << "post-install.sh" << "pre-remove.sh" );
	connect(ui->list_scripts_file, SIGNAL(currentIndexChanged(int)), this, SLOT(slotScriptChanged(int)) );
	
      SetupDefaults(); //load program defaults
      refreshGUI("all"); //make items visible/invisible as necessary
      
      //Connect "option changed" signals to the respective slot
      // PBI tab
      connect(ui->line_progname,SIGNAL(textChanged(QString)),this,SLOT(slotOptionChanged(QString)) );
      connect(ui->line_progauthor,SIGNAL(textChanged(QString)),this,SLOT(slotOptionChanged(QString)) );
      connect(ui->line_progversion,SIGNAL(textChanged(QString)),this,SLOT(slotOptionChanged(QString)) );
      connect(ui->line_progweb,SIGNAL(textChanged(QString)),this,SLOT(slotOptionChanged(QString)) );
      connect(ui->line_config_license,SIGNAL(textChanged(QString)),this,SLOT(slotOptionChanged(QString)) );
      connect(ui->line_makeport,SIGNAL(textChanged(QString)),this,SLOT(slotOptionChanged(QString)) );
      connect(ui->list_progicon,SIGNAL(currentIndexChanged(QString)),this,SLOT(slotOptionChanged(QString)) );
      connect(ui->line_repoTags,SIGNAL(textChanged(QString)),this,SLOT(slotOptionChanged(QString)) );
      connect(ui->line_repoType,SIGNAL(textChanged(QString)),this,SLOT(slotOptionChanged(QString)) );
      connect(ui->group_config_overrides, SIGNAL(clicked()), this, SLOT(updateConfigVisibility()) );
      connect(ui->group_config_repo, SIGNAL(clicked()), this, SLOT(updateConfigVisibility()) );
      // XDG tab
      connect(ui->line_xdg_name,SIGNAL(textChanged(QString)),this,SLOT(slotXDGOptionChanged(QString)) );
      connect(ui->line_xdg_mimepatterns,SIGNAL(textChanged(QString)),this,SLOT(slotXDGOptionChanged(QString)) );
      connect(ui->line_xdg_exec,SIGNAL(textChanged(QString)),this,SLOT(slotXDGOptionChanged(QString)) );
      connect(ui->line_xdg_menu,SIGNAL(textChanged(QString)),this,SLOT(slotXDGOptionChanged(QString)) );
      connect(ui->check_xdg_nodisplay,SIGNAL(clicked()),this,SLOT(slotXDGOptionChanged()) );
      connect(ui->check_xdg_requiresroot,SIGNAL(clicked()),this,SLOT(slotXDGOptionChanged()) );
      connect(ui->check_xdg_terminal,SIGNAL(clicked()),this,SLOT(slotXDGOptionChanged()) );
      connect(ui->list_xdg_icon,SIGNAL(currentIndexChanged(QString)),this,SLOT(slotXDGOptionChanged(QString)) );
      // Scripts tab
      connect(ui->text_scripts_edit,SIGNAL(textChanged()),this,SLOT(slotScriptModified()) );
        
}

MainGUI::~MainGUI()
{
  qDebug() << "Shutting down EasyPBI normally";
}

void MainGUI::loadModule(QString confFile){
  //Load a module that was specified on the CLI
  if(confFile.isEmpty() || !QFile::exists(confFile) ){return;} //invalid file
  bool ok = MODULE.loadModule(confFile);
  if(ok){ 
    qDebug() << "Loaded module:"<<confFile;
    line_module->setText(MODULE.basepath().replace(QDir::homePath(),"~")); 
    lastModuleDir = MODULE.basepath();
  }
  //Move to the pbi.conf tab
  ui->tabWidget->setCurrentWidget(ui->tab_pbi_conf);
  //Refresh the UI
  refreshGUI("pbiconf");
}

void MainGUI::slotSingleInstance(){
  this->showNormal();
  this->activateWindow();
  this->raise();
}

void MainGUI::SetupDefaults(){
    qDebug() << "Starting up EasyPBI (version "+settings->value("version")+")";	
    //Make sure that the program is not being started with root permissions
    QString id = QProcessEnvironment::systemEnvironment().toStringList().filter("HOME=").join(" ").simplified();
    if( id.contains("root") ){
      qDebug() << "EasyPBI should not be run with root permissions! exiting.....";
      exit(1);
    }

    //Load the current configuration settings
    settings->readMachineArch(); //set the 64-bit flag as appropriate
    //Scan for the external utilities needed to build PBI's
    settings->scanForExternalUtilities();
    //Load Preferences file if possible
    settings->loadSettingsFile();
    //create the EasyPBI directory structure if needed
    if( !settings->checkDirectoryStructure() ){
	QMessageBox::warning(this,tr("Permissions Error"), tr("There was an error trying to create the local EasyPBI directory structure. Please check the permissions for your home directory and restart EasyPBI.") );
	exit(1);
    }
    //Pop up a warning box if some external resources are not available
    bool chkOK = settings->check("isSUavailable");
    if( !chkOK ){
	QMessageBox::warning(this, tr("Resources Unavailable"), tr("Some external resources could not be found, so the EasyPBI services that use these resources have been deactivated.")+"\n"+tr("Please open up the EasyPBI settings to correct this deficiency.") );
    }
    
    updateConfigVisibility(); //on first run, always need to do this manually
}

void MainGUI::updateConfigVisibility(){
  //Update the group visibility for the pbi.conf tab
  ui->frame_pkgFix->setVisible(ui->group_config_overrides->isChecked());
  ui->frame_repoInfo->setVisible(ui->group_config_repo->isChecked());
}

void MainGUI::refreshGUI(QString item){
  //Enable/disable the interface appropriately
  
  //Stupid check to make sure that a module is actually loaded
  if( MODULE.basepath().isEmpty() ){ 
    ui->actionPackage_Module->setEnabled(FALSE);
    ui->actionRefresh_Module->setEnabled(FALSE);
    ui->tabWidget->setEnabled(false);
    return;
  }else{ 
    ui->actionPackage_Module->setEnabled(TRUE);  
    ui->actionRefresh_Module->setEnabled(TRUE);
    ui->tabWidget->setEnabled(true);
  }
  //See if we should refresh everything (default)  
  bool doall = ( (item == "all") || item.isEmpty() );
  bool doeditor = (item == "editor"); //also add a flag to just refresh the editor
  //Refresh the desired tab(s)
  // ------PBI.CONF------
  if( doall || doeditor || (item == "pbiconf")){
    //Update options visibility
    updateConfigVisibility();
    //Now reset the options to the current module values
    MODULE.loadConfig();
    // -- Text Values
    ui->line_progname->setText( MODULE.text("PBI_PROGNAME") );
    ui->line_progversion->setText( MODULE.text("PBI_PROGVERSION") );
    ui->line_progweb->setText( MODULE.text("PBI_PROGWEB") );
    ui->line_progauthor->setText( MODULE.text("PBI_PROGAUTHOR") );
    ui->line_makeport->setText( MODULE.text("PBI_MAKEPORT") );
    ui->line_config_license->setText( MODULE.text("PBI_LICENSE") );
    ui->line_repoTags->setText( MODULE.text("PBI_TAGS") );
    ui->line_repoType->setText( MODULE.text("PBI_PROGTYPE") );
    // -- Combo Boxes (filled with individual items from text)
    ui->list_portafter->clear();
    ui->list_portafter->addItems( MODULE.text("PBI_OTHERPKGS").split("\n") );
    ui->combo_plugins->clear();
    ui->combo_plugins->addItems( MODULE.text("PBI_PLUGINS").split("\n") );
    ui->combo_screenshots->clear();
    ui->combo_screenshots->addItems( MODULE.text("PBI_SCREENSHOTS").split("\n") );
    ui->combo_similar->clear();
    ui->combo_similar->addItems( MODULE.text("PBI_RELATED").split("\n") );
    // -- Combo Boxes (Select the proper item only)
    QStringList icons = MODULE.existingResources().filter(".png");
    ui->list_progicon->clear();
    if(icons.length() > 0){
      for(int i=0; i<icons.length(); i++){
        ui->list_progicon->addItem(QIcon(MODULE.basepath()+"/resources/"+icons[i]),icons[i]);
      }	    
      int cicon = icons.indexOf(MODULE.text("PBI_PROGICON"));
      if( cicon == -1 ){ 
	      ui->list_progicon->addItem(MODULE.text("PBI_PROGICON")); 
	      cicon = icons.indexOf(MODULE.text("PBI_PROGICON"));
      }
      ui->list_progicon->setCurrentIndex(cicon);
    }else{
      ui->list_progicon->addItem( MODULE.text("PBI_PROGICON") );
    }
    //Now disable the save button
    ui->push_config_save->setEnabled(FALSE);  //disable the save button until something changes
    //Load the current package information and display it on the UI
    QStringList pkgInfo = Backend::getPkgInfo(MODULE.text("PBI_MAKEPORT"));
    qDebug() << "pkg info:" << pkgInfo;
    if(pkgInfo.length() >= 4){
      if(ui->line_progname->text().isEmpty()){ ui->line_progname->setText( pkgInfo[0] ); }
      ui->line_progversion->setPlaceholderText(pkgInfo[1]);
      ui->line_progweb->setPlaceholderText(pkgInfo[2]);
      ui->line_config_license->setPlaceholderText(pkgInfo[3]);
    }else{
      ui->line_progversion->setPlaceholderText("");
      ui->line_progweb->setPlaceholderText("");
      ui->line_config_license->setPlaceholderText("");
    }

  }
  // -----RESOURCES--------
  if( doall || doeditor || (item == "resources")){
    //Get the all the current files in the resources category and add them to the list
    QStringList rs = MODULE.existingResources(); //currentModule->filesAvailable("resources");
    //Update the Widget
    ui->listw_resources->clear();
    qDebug() << "Loading Resources:" << rs;
    for(int i=0; i<rs.length(); i++){
      if(rs[i].endsWith(".png") || rs[i].endsWith(".jpg")){
	ui->listw_resources->addItem( new QListWidgetItem(QIcon(MODULE.basepath()+"/resources/"+rs[i]), rs[i]) );
      }else{
	ui->listw_resources->addItem(rs[i]);
      }
    }
    //ui->listw_resources->addItems(rs);
    //re-connect the signal/slot
    //connect(ui->listw_resources, SIGNAL(itemSelectionChanged()), this, SLOT(slotResourceChanged()) );
    //Load the file into the viewers
    //slotResourceChanged();
  }
  //------XDG------
  if( doall || doeditor || (item == "xdg")){
    slotXdgTypeChanged();
  }
  //------SCRIPTS-----
  if( doall || doeditor || (item == "scripts")  ){
    //Update the list of available scripts
    QStringList exist = MODULE.existingScripts();
    QStringList valid = MODULE.validScripts();
    QString current="junk";
    if(ui->list_scripts_file->count() > 0){
	current = ui->list_scripts_file->currentText(); //get the current item before clearing it
    }
    ui->list_scripts_file->clear();
    ui->list_scripts_file->addItem("-- Installation Scripts --");
    int index = 0;
    for(int i=0; i<valid.length(); i++){
      if(exist.contains(valid[i])){ 
	 exist.removeAll(valid[i]); 
	 ui->list_scripts_file->addItem(Backend::icon("file"), valid[i]);
      }else{ ui->list_scripts_file->addItem(Backend::icon("close"), valid[i]); }
      if(valid[i] == current){ index = i+1; }
    }
    //Now add the rest of the existing scripts (that are no longer valid)
    for(int i=0; i<exist.length(); i++){
      ui->list_scripts_file->addItem(Backend::icon("delete"), exist[i]);
      if(exist[i] == current){ index = valid.length()+i+1; }
    }
    //Now reset to the appropriate item (will automatically load appropriately)
    ui->list_scripts_file->setCurrentIndex(index);
  }

  //------OVERALL SETTINGS------
  if( doall || doeditor ){
    //Check for a 64-bit system to enable the 32-bit build option
    //if( settings->check("is64bit") ){ ui->check_build_32->setVisible(TRUE); }
    //else{ ui->check_build_32->setVisible(FALSE); ui->check_build_32->setChecked(FALSE); }
    
    //Set the default focus on the "load" button
    this->setFocus();
  }
}

QString MainGUI::getPortPackage(){
  QString portSel;
  /*if( settings->check("isportsavailable") ){
    //Prompt for a new port
    portSel = QFileDialog::getExistingDirectory(this, tr("Select Port"), settings->value("portsdir"));	
  }else{*/
    //Prompt for a package
    pkgSelect dlg(this);
    dlg.exec();
    if(dlg.selected){ portSel = dlg.portSelected; };
  //}
  return portSel;
}

/*----------------------------------
   MENU OPTIONS
  -----------------------------------
*/

void MainGUI::on_actionExit_triggered(){
  qDebug() << "Close EasyPBI requested";
  this->close();
}

void MainGUI::on_actionPreferences_triggered(){
  //Open up the preferences dialog
  PreferencesDialog dlg(this);
  //make the dialog modal and wait for it to close
  dlg.exec(); 
  //Re-check for system commands
  settings->scanForExternalUtilities();
  //Now reload the saved settings
  settings->loadSettingsFile();
  //Now refresh the GUI
  refreshGUI("all");
}

void MainGUI::on_actionPackage_Module_triggered(){
   ModuleUtils::compressModule(MODULE.basepath()); //currentModule->compressModule();
   QMessageBox::information(this,tr("Success"),tr("A copy of the current module has been successfully packaged  within the module directory.") );

}

void MainGUI::on_actionRefresh_Module_triggered(){
  QString modSel = MODULE.basepath(); //currentModule->path(); //re-load the current module
  bool ok = MODULE.loadModule(modSel+"/pbi.conf"); 
  if(ok){ 
    qDebug() << "Loaded module:"<<modSel;	  
    line_module->setText(modSel.replace(QDir::homePath(),"~"));
    refreshGUI("all");
  }
}

void MainGUI::on_actionFreeBSD_Ports_triggered(){
  QString port;
  if(!line_module->text().isEmpty()){
	//Get the currently selected port
	port = MODULE.text("PBI_MAKEPORT");
  }
  QString target_url = "http://www.freshports.org/" + port;
  qDebug() << "Opening URL:" << target_url;  
  target_url.prepend("xdg-open ");
  system(target_url.toUtf8());	
}

void MainGUI::on_actionPBI_Modules_triggered(){
  //Open the PC-BSD wiki to the module builders guide
   QString target_url = "http://wiki.pcbsd.org/index.php/PBI_Module_Builder_Guide/10.0";
   qDebug() << "Opening URL:" << target_url; 
   target_url.prepend("xdg-open ");
   system(target_url.toUtf8());
}

void MainGUI::on_actionAbout_EasyPBI_triggered(){
  aboutDialog dlg(this,settings->value("version"));
  dlg.exec();
}


/*----------------------------------
   TOOLBAR OPTIONS
  -----------------------------------
*/
void MainGUI::on_actionNew_Module_triggered(){
  qDebug() << "New Module triggered";
  //Create and run the new dialog
  QString pDir = "";
  if(settings->check("isportsavailable")){ pDir = settings->value("portsdir"); }
  NewModuleDialog* dlg = new NewModuleDialog(this, pDir);
  dlg->setDefaultIconFile(settings->value("defaulticon"));
  dlg->exec();
  //Determine if the action was cancelled
  if( dlg->isAccepted ){
    //Now create the new module
    QString modPath, modBase, oldIconPath;
    modBase = dlg->moduleData; //port cat/name
    oldIconPath = dlg->moduleIcon; //PNG icon to use for the program
    modPath = settings->value("moduledir"); //base directory to create modules in
    MODULE = ModuleUtils::newModule(modPath, modBase, oldIconPath);
    if(MODULE.basepath().isEmpty()){
      QMessageBox::warning(this,tr("EasyPBI: Permissions Error"), tr("Could not create PBI module. Please check the directory permissions and try again."));
    }else{
      line_module->setText( MODULE.basepath().replace(QDir::homePath(), "~") );
      /*if(dlg->isPort){
	//A couple additional conveniances for port builds
        MODULE.setEnabled("PBI_AB_NOPKGBUILD",true);
	MODULE.saveConfig();
	ui->group_config_ports->setChecked(true);
      }*/
    }
  }
  //Move to the pbi.conf tab
  ui->tabWidget->setCurrentWidget(ui->tab_pbi_conf);
  //Refresh the UI
  refreshGUI("pbiconf");
  delete dlg;
}

void MainGUI::on_actionLoad_Module_triggered(){
  QString openDir = settings->value("moduledir");
  if(!lastModuleDir.isEmpty()){ openDir = lastModuleDir; }
  QString modSel = QFileDialog::getOpenFileName(this, tr("Select PBI Module Configuration"), openDir, tr("PBI Configuration (pbi.conf)") );
  if(modSel.isEmpty()){return;} //action cancelled or closed
  bool ok = MODULE.loadModule(modSel);
  if(ok){ 
    qDebug() << "Loaded module:"<<modSel;
    line_module->setText(MODULE.basepath().replace(QDir::homePath(),"~")); 
    lastModuleDir = MODULE.basepath();
  }
  //Move to the pbi.conf tab
  ui->tabWidget->setCurrentWidget(ui->tab_pbi_conf);
  //Refresh the UI
  refreshGUI("pbiconf");
}

/*----------------------------------
   MODULE EDITOR OPTIONS
  -----------------------------------
*/
void MainGUI::slotModTabChanged(int newtab){
  switch(newtab){
    case 0:
	    refreshGUI("pbiconf"); break;
    case 1:
	    refreshGUI("resources"); break;
    case 2:
	    refreshGUI("xdg"); break;
    case 3:
	    refreshGUI("scripts"); break;
    default:
	    refreshGUI("editor"); break; //do all the module editor tabs
  }	  
}

/*----------------------------------
   PBI.CONF OPTIONS
  -----------------------------------
*/
void MainGUI::on_push_change_makeport_clicked(){
  QString portSel = getPortPackage(); 	  
  if(portSel.isEmpty()){return;}	

  //Save the port info to the GUI
  ui->line_makeport->setText(portSel);
  ui->push_config_save->setEnabled(TRUE);
}

void MainGUI::on_tool_addportafter_clicked(){
  //Prompt for a new port
  QString portSel = getPortPackage();
  if(portSel.isEmpty()){return;} //action cancelled or closed	
  //Save the port info to the GUI
  if(ui->list_portafter->count() == 1 && ui->list_portafter->currentText().isEmpty() ){ ui->list_portafter->clear(); }
  ui->list_portafter->addItem(portSel.remove(settings->value("portsdir")+"/"));
  ui->push_config_save->setEnabled(TRUE);
}

void MainGUI::on_tool_rmportafter_clicked(){
  int index = ui->list_portafter->currentIndex();
  if(index != -1){
    ui->list_portafter->removeItem(index);
  }
  ui->push_config_save->setEnabled(TRUE);
}

void MainGUI::on_push_config_save_clicked(){
  //Save the current settings to the backend structures
  //Text Values
  MODULE.setText("PBI_MAKEPORT", ui->line_makeport->text());
  MODULE.setText("PBI_PROGNAME", ui->line_progname->text());
  MODULE.setText("PBI_PROGVERSION", ui->line_progversion->text());
  MODULE.setText("PBI_PROGWEB", ui->line_progweb->text());
  MODULE.setText("PBI_PROGAUTHOR", ui->line_progauthor->text());
  MODULE.setText("PBI_LICENSE", ui->line_config_license->text());
  MODULE.setText("PBI_TAGS", ui->line_repoTags->text());
  MODULE.setText("PBI_PROGTYPE", ui->line_repoType->text());
  //Combo Boxes
  QStringList addports;
  for(int i=0; i<ui->list_portafter->count(); i++){ addports << ui->list_portafter->itemText(i); }
  MODULE.setText("PBI_OTHERPKGS", addports.join("\n") );
  addports.clear();
  for(int i=0; i<ui->combo_plugins->count(); i++){ addports << ui->combo_plugins->itemText(i); }
  MODULE.setText("PBI_PLUGINS", addports.join("\n") );
  addports.clear();
  for(int i=0; i<ui->combo_screenshots->count(); i++){ addports << ui->combo_screenshots->itemText(i); }
  MODULE.setText("PBI_SCREENSHOTS", addports.join("\n") );
  addports.clear();
  for(int i=0; i<ui->combo_similar->count(); i++){ addports << ui->combo_similar->itemText(i); }
  MODULE.setText("PBI_RELATED", addports.join("\n") );
  MODULE.setText("PBI_PROGICON", ui->list_progicon->currentText() );

  
  //save the new settings to pbi.conf
  bool ok = MODULE.saveConfig();
  if(!ok){
    //Display a warning that the file could not be saved
    QMessageBox::warning(this,tr("EasyPBI Error"), tr("The PBI configuration file could not be saved. Please check your file permissions before trying again."));
    //re-load the current PBI.conf into the backend
    MODULE.loadConfig();
  }else{
    ui->push_config_save->setEnabled(false); //make sure the save button goes inactive once saved
  }
}

void MainGUI::slotOptionChanged(QString tmp){
  tmp.clear(); //just to remove compiler warning about unused variable
  ui->push_config_save->setEnabled(TRUE);	
}

void MainGUI::slotSetRepoType(QAction* act){
  ui->line_repoType->setText(act->text());
}

void MainGUI::on_tool_addplugin_clicked(){
  //Prompt for a new port
  QString portSel = getPortPackage();
  if(portSel.isEmpty()){return;} //action cancelled or closed	
  //Save the port info to the GUI
  if(ui->combo_plugins->count() == 1 && ui->combo_plugins->currentText().isEmpty() ){ ui->combo_plugins->clear(); }
  ui->combo_plugins->addItem(portSel.remove(settings->value("portsdir")+"/"));
  ui->push_config_save->setEnabled(true);
}

void MainGUI::on_tool_rmplugin_clicked(){
  if(ui->combo_plugins->currentIndex() >= 0){
    ui->combo_plugins->removeItem(ui->combo_plugins->currentIndex());
    slotOptionChanged();
  }
}

void MainGUI::on_tool_addscreenshot_clicked(){
  QString url = QInputDialog::getText(this, tr("Screenshot URL"), tr("Screenshot URL:"));
  if(url.isEmpty()){ return; }
  if(ui->combo_screenshots->count() == 1 && ui->combo_screenshots->currentText().isEmpty() ){ ui->combo_screenshots->clear(); }
  ui->combo_screenshots->addItem(url);
  slotOptionChanged();
}

void MainGUI::on_tool_rmscreenshot_clicked(){
  if(ui->combo_screenshots->currentIndex() >= 0){
    ui->combo_screenshots->removeItem(ui->combo_screenshots->currentIndex());
    slotOptionChanged();
  }	
}

void MainGUI::on_tool_addsimilar_clicked(){
  //Prompt for a new port
  QString portSel = getPortPackage();
  if(portSel.isEmpty()){return;} //action cancelled or closed	
  //Save the port info to the GUI
  if(ui->combo_similar->count() == 1 && ui->combo_similar->currentText().isEmpty() ){ ui->combo_similar->clear(); }
  ui->combo_similar->addItem(portSel.remove(settings->value("portsdir")+"/"));
  ui->push_config_save->setEnabled(true);	
}

void MainGUI::on_tool_rmsimilar_clicked(){
  if(ui->combo_similar->currentIndex() >= 0){
    ui->combo_similar->removeItem(ui->combo_similar->currentIndex());
    slotOptionChanged();
  }
}

/*------------------------------------------------
   RESOURCE EDITOR OPTIONS
  -------------------------------------------------
*/
void MainGUI::on_push_resources_add_clicked(){
  //Get the desired file
  QStringList iFiles = QFileDialog::getOpenFileNames(this, tr("Select Resources"), settings->value("icondir") );
  //Check that there were file selected
  if(iFiles.isEmpty()){ return; }
  //Now add these files to the module
  for(int i=0; i<iFiles.length(); i++){
    MODULE.addResource(iFiles[i]);
  }
  //Now update the GUI
  refreshGUI("resources");
}

void MainGUI::on_push_resources_remove_clicked(){
  //Get the currently selected resource
  QString cfile;
  if(ui->listw_resources->currentRow() != -1){ cfile = ui->listw_resources->currentItem()->text(); }
  //Check that there is something selected  
  if(cfile.isEmpty()){ return; }
  //Remove the resource
  if(MODULE.removeResource(cfile)){
      qDebug() << "Removed Resource:" << cfile;
  }else{
      qDebug() << "Error: Could not remove resource:"<<cfile;
  }
  //Refresh the GUI
  refreshGUI("resources");
}

/*------------------------------------------------
   XDG EDITOR OPTIONS
  -------------------------------------------------
*/
void MainGUI::slotXdgTypeChanged(){
  XDGUPDATING=true;
  //Update the file list appropriately	
  if(ui->radio_xdg_desktop->isChecked()){
    //update the files available
    ui->list_xdg_files->clear();
    ui->list_xdg_files->addItems(MODULE.listXdgDesktopFiles());
    //Set the visibility
        //Current file
	ui->label_xdg_name->setVisible(TRUE); ui->line_xdg_name->setVisible(TRUE);
	ui->label_xdg_exec->setVisible(TRUE); ui->line_xdg_exec->setVisible(TRUE); ui->push_xdg_exec->setVisible(TRUE);
	ui->label_xdg_icon->setVisible(TRUE); ui->list_xdg_icon->setVisible(TRUE);
	ui->label_xdg_menu->setVisible(FALSE); ui->line_xdg_menu->setVisible(FALSE); ui->push_xdg_menu->setVisible(FALSE);
	ui->check_xdg_nodisplay->setVisible(TRUE);
	ui->check_xdg_terminal->setVisible(TRUE);
	ui->label_xdg_mimepatterns->setVisible(FALSE); ui->line_xdg_mimepatterns->setVisible(FALSE);
	
  }else if(ui->radio_xdg_menu->isChecked()){
    //update the files available
    ui->list_xdg_files->clear();
    ui->list_xdg_files->addItems(MODULE.listXdgMenuFiles());	  
    //Set the visibility
	//Current file
	ui->label_xdg_name->setVisible(TRUE); ui->line_xdg_name->setVisible(TRUE);
	ui->label_xdg_exec->setVisible(TRUE); ui->line_xdg_exec->setVisible(TRUE); ui->push_xdg_exec->setVisible(TRUE);
	ui->label_xdg_icon->setVisible(TRUE); ui->list_xdg_icon->setVisible(TRUE);
	ui->label_xdg_menu->setVisible(TRUE); ui->line_xdg_menu->setVisible(TRUE); ui->push_xdg_menu->setVisible(TRUE);
	ui->check_xdg_nodisplay->setVisible(TRUE);
	ui->check_xdg_terminal->setVisible(TRUE);
	ui->label_xdg_mimepatterns->setVisible(TRUE); ui->line_xdg_mimepatterns->setVisible(TRUE);
	
  }
  //Select the first file in the list if one is available
  if( ui->list_xdg_files->count() > 0){ ui->list_xdg_files->setCurrentRow(0); }
  //Update the program icon list for new entries
  ui->list_xdg_icon->clear();
  QStringList icons = MODULE.existingResources().filter(".png");
  if(icons.length() > 0){
      for(int i=0; i<icons.length(); i++){
        ui->list_xdg_icon->addItem(QIcon(MODULE.basepath()+"/resources/"+icons[i]),icons[i]);
      }	    
  }

  //Update the buttons that only need a refresh when the type changes (such as menu's)
  //Available binaries pushbuttons
  /*menu_bins.clear();
  QStringList cBins; //not re-implemented yet
  if(!cBins.isEmpty()){
    for(int i=0; i<cBins.length(); i++){
      menu_bins.addAction(cBins[i]);
    } 
  }*/
  //Menu categories
  QString recMenu = ModuleUtils::recommendedXdgCategory(MODULE.text("PBI_MAKEPORT").section("/",0,0) );
  QStringList cats = ModuleUtils::validXdgCategories();
  menu_validMenuCats.clear();
  for(int i=0; i<cats.length(); i++){
    if(recMenu==cats[i]){ //Add an icon showing the recommended menu category
      menu_validMenuCats.addAction(Backend::icon("start"),cats[i]);	
    }else{
      menu_validMenuCats.addAction(cats[i]);
    }
  }
  XDGUPDATING=false;
  //Now update read the currently selected file
  slotXdgFileChanged();
}

void MainGUI::slotXdgFileChanged(){	
  if(XDGUPDATING){ return; }
  bool clearUI =FALSE;
  if(ui->list_xdg_files->currentRow() == -1){ clearUI=TRUE; }

  //Reset the icon list
  ui->list_xdg_icon->clear();
  QStringList icons = MODULE.existingResources().filter(".png");
  if(icons.length() > 0){
      for(int i=0; i<icons.length(); i++){
        ui->list_xdg_icon->addItem(QIcon(MODULE.basepath()+"/resources/"+icons[i]),icons[i]);
      }	    
  }
  //Get the selected file
  QString file;
  if( !clearUI ){ file = ui->list_xdg_files->currentItem()->text(); }
  if( clearUI || file.isEmpty() ){
    //Clear the UI lines
    ui->line_xdg_name->clear();
    ui->line_xdg_exec->clear();
    ui->line_xdg_menu->clear();
    ui->line_xdg_mimepatterns->clear();
    ui->check_xdg_terminal->setChecked(FALSE);
    ui->check_xdg_nodisplay->setChecked(FALSE);
    ui->check_xdg_requiresroot->setChecked(FALSE);
    ui->push_xdg_savechanges->setEnabled(FALSE);
    ui->push_xdg_savenew->setEnabled(FALSE);
    //Now return
    return; 
  }
  //Now setup the UI as appropriate
  if(ui->radio_xdg_desktop->isChecked()){
    //Load the file
    MODULE.loadXdgDesktop(file);
    	
  }else if(ui->radio_xdg_menu->isChecked()){
    //Load the file
    MODULE.loadXdgMenu(file);
    
    //Load the XDG-MIME file associations
    QString mime = MODULE.xdgText("MimeType");
    QStringList mimeavailable = MODULE.listMimeFiles();
    bool found = FALSE;
    for( int i=0; i<mimeavailable.length(); i++){
      MODULE.loadMimeFile(mimeavailable[i]);
      if(MODULE.mimeText("type") == mime){
        found = TRUE;
	break;
      }
    }
    //qDebug() << "mimetype detected:" << mime;
    if(found){
      ui->line_xdg_mimepatterns->setText( MODULE.mimeText("pattern") ); 
    }else{
      MODULE.clearMimeData(); //Make sure we don't have any mime file loaded
      ui->line_xdg_mimepatterns->clear();
    }
    
  }else{
    MODULE.clearXdgData(); //do nothing, unknown radio button selected (or none)
  }
  //update the file info on the GUI
  ui->line_xdg_name->setText( MODULE.xdgText("Name") );
  ui->line_xdg_exec->setText( ModuleUtils::pruneXdgLine(MODULE.xdgText("Exec")) );
  ui->line_xdg_menu->setText( MODULE.xdgText("Categories") );
  //comboboxes 
  QString icon = ModuleUtils::pruneXdgLine(MODULE.xdgText("Icon"));
  int cicon = icons.indexOf(icon);
  if( cicon == -1 ){ 
    ui->list_xdg_icon->addItem(icon); 
    cicon = icons.indexOf(icon);
  }
  ui->list_xdg_icon->setCurrentIndex(cicon);

  //Checkboxes
  ui->check_xdg_terminal->setChecked( MODULE.xdgEnabled("Terminal") );
  ui->check_xdg_nodisplay->setChecked( MODULE.xdgEnabled("NoDisplay") );
  ui->check_xdg_requiresroot->setChecked( ModuleUtils::xdgExecUsesRoot(MODULE.xdgText("Exec")) );
    
    
  ui->push_xdg_savechanges->setEnabled(FALSE);
  ui->push_xdg_savenew->setEnabled(FALSE);
}

void MainGUI::slotAddMenuCat(QAction* act){
  //get the selected menu category
  QString newcat = act->text();
  //Read the current categories
  QStringList oldcats = ui->line_xdg_menu->text().split(";", QString::SkipEmptyParts);
  for(int i=0; i<oldcats.length(); i++){
    oldcats[i] = oldcats[i].simplified();
    if(oldcats[i] == newcat){return;} //category already in the list, do nothing
  }
  //Add the item
  oldcats << newcat;
  //Now replace the line onto the GUI
  ui->line_xdg_menu->setText(oldcats.join("; "));
}

void MainGUI::slotAddBin(QAction* act){
  //replace the current executable with the new one
  ui->line_xdg_exec->setText(act->text().section("/",-1) );	
}

void MainGUI::on_push_xdg_remove_clicked(){
  //Figure out if a file is selected
  QString desktopFile = ui->list_xdg_files->currentItem()->text();
  if( desktopFile.isEmpty() ){ return; }
  //Now remove the file
  bool ok = FALSE;
  if( ui->radio_xdg_desktop->isChecked() ){
    ok = MODULE.removeXdgDesktop(desktopFile);
  }else if( ui->radio_xdg_menu->isChecked() ){
    //Check for a MIME type to remove as well
    ui->line_xdg_mimepatterns->setText(""); //make sure we remove any MIME file
    MODULE.loadXdgMenu(desktopFile); //load the file's mimetype
    checkMime(); //perform the removal if appropriate
    ok = MODULE.removeXdgMenu(desktopFile);
  }
  //Now inform the user if there was an error
  if(!ok){
    QMessageBox::warning(this, tr("EasyPBI Error"), tr("Could not remove the XDG entry.")+"\n"+tr("Check the file permissions and try again") );
  }
  //Now refresh the GUI
  refreshGUI("xdg");
  
}

void MainGUI::on_push_xdg_savechanges_clicked(){
  //save the current UI settings to the module structure then save the file
  bool ok = FALSE;
  QString desktopFile = ui->list_xdg_files->currentItem()->text();
  if(desktopFile.isEmpty()){ return; }
  //Make sure the backend is currently set for this file
  if( ui->radio_xdg_desktop->isChecked() ){ MODULE.loadXdgDesktop(desktopFile); }
  else if(ui->radio_xdg_menu->isChecked() ){ MODULE.loadXdgMenu(desktopFile); }
  //Save the values into the backend module structure
  MODULE.setXdgText("Value", "1.0");
  MODULE.setXdgText("Type", "Application");
  MODULE.setXdgText("Name", ui->line_xdg_name->text());
  MODULE.setXdgText("GenericName", ui->line_xdg_name->text().toLower());
  MODULE.setXdgText("Exec", ModuleUtils::generateXdgExec(ui->line_xdg_exec->text(), ui->check_xdg_requiresroot->isChecked()) );
  MODULE.setXdgText("Icon", ModuleUtils::generateXdgPath(ui->list_xdg_icon->currentText()) );
  MODULE.setXdgText("Path", ModuleUtils::generateXdgPath("") );
  MODULE.setXdgEnabled("Terminal",ui->check_xdg_terminal->isChecked());
  MODULE.setXdgEnabled("NoDisplay",ui->check_xdg_nodisplay->isChecked());
  MODULE.setXdgEnabled("StartupNotify", true);
  if( ui->radio_xdg_desktop->isChecked() ){
    //clear the menu entry values
    MODULE.setXdgText("Categories", "");
    MODULE.setXdgText("MimeType", "");
    //Now save the file
    ok = MODULE.saveXdgDesktop(desktopFile);
  }else if( ui->radio_xdg_menu->isChecked() ){
    //Setup the menu entry values
    MODULE.setXdgText("Categories", ui->line_xdg_menu->text());
    //Setup the mime type associations as appropriate
    checkMime();
    //Now save the file
    ok = MODULE.saveXdgMenu(desktopFile);    
  }
  //Now inform the user if there was an error
  if(!ok){
    QMessageBox::warning(this, tr("EasyPBI Error"), tr("Could not save the changes to the XDG entry.")+"\n"+tr("Check the file permissions and try again") );
  }
  //Now refresh the GUI
  slotXdgFileChanged(); //only update the proper half of the tab
}

void MainGUI::on_push_xdg_savenew_clicked(){
  //Get the filename we want to save
  QString filename;
  //Now check that the filename does not already exist
  QStringList cFiles;
  if(ui->radio_xdg_desktop->isChecked() ){
    filename = ui->line_xdg_exec->text().section("/",-1);
    cFiles = MODULE.listXdgDesktopFiles().replaceInStrings(".desktop","");
  }else if(ui->radio_xdg_menu->isChecked() ){
    filename = ui->line_xdg_exec->text().section("/",-1);
    cFiles = MODULE.listXdgMenuFiles().replaceInStrings(".desktop","");
  }
  if(filename.isEmpty()){ return; }
  
  if(cFiles.indexOf(filename) != -1){
    int num = 1;
    while(cFiles.indexOf(filename+QString::number(num)) != -1){ num++; }
    filename.append( QString::number(num) );
  }
  //save the current UI settings to the module structure then save the file
  bool ok = FALSE;
  if(filename.isEmpty()){ return; }
  //Make sure the backend structure is currently empty
  MODULE.clearXdgData();
  //Save the values into the backend module structure
  MODULE.setXdgText("Value", "1.0");
  MODULE.setXdgText("Type", "Application");
  MODULE.setXdgText("Name", ui->line_xdg_name->text());
  MODULE.setXdgText("GenericName", ui->line_xdg_name->text().toLower());
  MODULE.setXdgText("Exec", ModuleUtils::generateXdgExec(ui->line_xdg_exec->text(), ui->check_xdg_requiresroot->isChecked()) );
  MODULE.setXdgText("Icon", ModuleUtils::generateXdgPath(ui->list_xdg_icon->currentText()) );
  MODULE.setXdgText("Path", ModuleUtils::generateXdgPath("") );
  MODULE.setXdgEnabled("Terminal",ui->check_xdg_terminal->isChecked());
  MODULE.setXdgEnabled("NoDisplay",ui->check_xdg_nodisplay->isChecked());
  MODULE.setXdgEnabled("StartupNotify", true);
  if( ui->radio_xdg_desktop->isChecked() ){
    //clear the menu entry values
    MODULE.setXdgText("Categories", "");
    MODULE.setXdgText("MimeType", "");
    //Now save the file
    ok = MODULE.saveXdgDesktop(filename);
  }else if( ui->radio_xdg_menu->isChecked() ){
    //Setup the menu entry values
    MODULE.setXdgText("Categories", ui->line_xdg_menu->text());
    //Setup the mime type associations as appropriate
    checkMime();
    //Now save the file
    ok = MODULE.saveXdgMenu(filename);    
  }
  //Now inform the user if there was an error
  if(!ok){
    QMessageBox::warning(this, tr("EasyPBI Error"), tr("Could not create the XDG entry.")+"\n"+tr("Check the file permissions and try again") );
  }
  //Now refresh the GUI
  refreshGUI("xdg");
}

void MainGUI::on_push_xdg_clearentry_clicked(){
  //Clear the UI
	//reset the item selected to nothing
	ui->list_xdg_files->setCurrentRow(-1);
	//De-activate the "save" button
	ui->push_xdg_savechanges->setEnabled(FALSE);
	//Now clear the 
	slotXdgFileChanged();
}

void MainGUI::checkMime(){
    //This function will check the UI for differences in the MIME patterns
      // and update/create the MIME file appropriately
      // The MENU mime type will also be appropriately set
	
    //Get the current/desired values
    QString newPatterns = ui->line_xdg_mimepatterns->text().simplified();
    QString cmimetype = MODULE.xdgText("MimeType"); //in the desktop/menu file
	
    //Determine the action necessary
    int stat = 0; //do nothing by default
    if( newPatterns.isEmpty() && !cmimetype.isEmpty() ){ stat = 1; } //remove old mime file
    else if( !newPatterns.isEmpty() && !cmimetype.isEmpty() ){stat = 2; } //update mime file
    else if( !newPatterns.isEmpty() && cmimetype.isEmpty() ){stat = 3; } //new mime file
    qDebug() << "CheckMime:" << QString::number(stat);
    //Now perform the actions as appropriate
    MODULE.clearMimeData(); //Make sure this backend is currently empty    
    QString fileName;
    if(stat == 0){ return; } //no action needed
    else if(stat == 1 || stat == 2){ //need to find current MIME file
      QStringList MF = MODULE.listMimeFiles();
      for(int i=0; i<MF.length(); i++){
        MODULE.loadMimeFile(MF[i]);
	if(MODULE.mimeText("type") == cmimetype){
	  if(stat == 1){ //remove old mime file
	    MODULE.removeMimeFile(MF[i]);
	    MODULE.clearMimeData();
	    MODULE.setXdgText("MimeType","");
	    return; //done now
	  }else{ //stat ==2, update mime file
	    //Check if there is anything that actually needs to be done
	    if( MODULE.mimeText("pattern") == newPatterns ){ return; } //nothing to change
	    fileName = MF[i]; //save this filename for later use
          }
	  break; //stop here, the file was already found and loaded
	}
      }
    }else{
      QString bin = ui->line_xdg_exec->text().section(" ",0,0,QString::SectionSkipEmpty).simplified();
      if(bin.isEmpty()){ return; } //should never hit this - but just to make sure
      //Generate a new mimetype
      cmimetype = "application/x-"+bin;
      MODULE.setXdgText("MimeType", cmimetype);
      //Generate a new filename for this mime file
      fileName = bin+"-xdg.xml";
    }
    //Now need to save the current values into the file (new or old, depending on earlier)
    MODULE.setMimeText("type", cmimetype);
    MODULE.setMimeText("pattern", newPatterns);
    //don't worry about xmlns - it will be auto generated if not set when creating the file, otherwise will use the current setting
    MODULE.saveMimeFile(fileName);
}

void MainGUI::slotXDGOptionChanged(QString tmp){
  tmp.clear(); //remove warning about unused variables
  ui->push_xdg_savechanges->setEnabled(TRUE);
  ui->push_xdg_savenew->setEnabled(TRUE);
}

/*------------------------------------------------
   SCRIPTS EDITOR OPTIONS
  -------------------------------------------------
*/
void MainGUI::slotScriptChanged(int index){
  //Update the GUI appropriately
    QStringList exist = MODULE.existingScripts();
    if( exist.contains(ui->list_scripts_file->currentText()) ){
      //Read and display the script
      QStringList contents = MODULE.readScript(ui->list_scripts_file->currentText());
      ui->text_scripts_edit->setPlainText(contents.join("\n"));
      //Setup display items
      ui->push_scripts_create->setVisible(FALSE);
      ui->push_scripts_remove->setVisible(TRUE);
      ui->push_scripts_save->setVisible(TRUE);
      ui->text_scripts_edit->setVisible(TRUE);
    }else{
      //Setup display items
      ui->push_scripts_create->setVisible(TRUE);
      ui->push_scripts_remove->setVisible(FALSE);
      ui->push_scripts_save->setVisible(FALSE);
      ui->text_scripts_edit->setVisible(FALSE);
    }
    if(index == 0){ ui->push_scripts_create->setVisible(FALSE); }
    ui->push_scripts_save->setEnabled(FALSE); //disable the save button until something changes
}

void MainGUI::on_push_scripts_create_clicked(){
  //Setup display items
  ui->push_scripts_create->setVisible(FALSE);
  ui->push_scripts_remove->setVisible(FALSE);
  ui->push_scripts_save->setVisible(TRUE);
  ui->text_scripts_edit->setVisible(TRUE);
  ui->text_scripts_edit->clear();
  ui->push_scripts_save->setEnabled(FALSE); //disable the save button until something changes
}

void MainGUI::on_push_scripts_remove_clicked(){
  //Get the current file
  QString filename = ui->list_scripts_file->currentText();
  //Remove the file
  MODULE.removeScript(filename);
  //refresh the UI
  refreshGUI("scripts");
}

void MainGUI::on_push_scripts_save_clicked(){
  //Get the current file text
  QStringList contents = ui->text_scripts_edit->toPlainText().split("\n");
  //get the current file
  QString filename = ui->list_scripts_file->currentText();
  //Save the file
  bool ok = MODULE.writeScript(filename,contents);
  //display a warning if error
  if(!ok){
    QMessageBox::warning(this,tr("Error Saving File"), tr("Could not save the script")+"\n"+tr("Please check the file permissions and try again") );
  }
  //Now refresh the UI
  refreshGUI("scripts");
}

void MainGUI::slotScriptModified(){
  ui->push_scripts_save->setEnabled(TRUE);	
}
