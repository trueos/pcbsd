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
	ui->tool_repoCat->setIcon(Backend::icon("left"));
	  ui->tool_repoCat->setMenu(&menu_validRepoCats);
	  connect(&menu_validRepoCats, SIGNAL(triggered(QAction*)), this, SLOT(slotSetRepoCat(QAction*)) );
	// -- resources tab --
	ui->push_resources_savewrapper->setIcon(Backend::icon("save"));
	connect(ui->listw_resources, SIGNAL(itemSelectionChanged()), this, SLOT(slotResourceChanged()) );
	connect(ui->push_resources_savewrapper, SIGNAL(clicked()), this, SLOT(slotResourceScriptSaved()) );
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
	ui->list_scripts_file->addItems(QStringList() << " ---"+tr("Installation Scripts")+"---" << "pre-portmake.sh" << "post-portmake.sh" << "pre-pbicreate.sh" << "pre-install.sh" << "post-install.sh" << "pre-remove.sh" );
	connect(ui->list_scripts_file, SIGNAL(currentIndexChanged(int)), this, SLOT(slotScriptChanged(int)) );
	// -- External links tab --
	ui->push_el_file->setIcon(Backend::icon("left"));
	//ui->push_el_file->setMenu(&menu_el_bins);
	ui->push_el_file->setEnabled(false);
	//connect(&menu_el_bins,SIGNAL(triggered(QAction*)),this,SLOT(slotELSetFile(QAction*)) );
	ui->push_el_filetype->setIcon(Backend::icon("left"));
	ui->push_el_filetype->setMenu(&menu_elOpts);
	connect(&menu_elOpts,SIGNAL(triggered(QAction*)),this,SLOT(slotELSetType(QAction*)) );
	//Setup PBI Builder
	ui->push_build_stop->setIcon(Backend::icon("close"));
	ui->push_build_save->setIcon(Backend::icon("save"));
	
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
      connect(ui->line_repoCat,SIGNAL(textChanged(QString)),this,SLOT(slotOptionChanged(QString)) );
      connect(ui->line_repoIconURL,SIGNAL(textChanged(QString)),this,SLOT(slotOptionChanged(QString)) );
      connect(ui->spin_repoBuildKey,SIGNAL(valueChanged(QString)),this,SLOT(slotOptionChanged()) );
      connect(ui->spin_repoPriority,SIGNAL(valueChanged(QString)),this,SLOT(slotOptionChanged()) );
      connect(ui->spin_repoRevision,SIGNAL(valueChanged(QString)),this,SLOT(slotOptionChanged()) );
      connect(ui->check_requiresroot, SIGNAL(clicked()),this,SLOT(slotOptionChanged()) );
      connect(ui->check_config_nopkg, SIGNAL(clicked()),this,SLOT(slotOptionChanged()) );
      connect(ui->check_config_notmpfs, SIGNAL(clicked()),this,SLOT(slotOptionChanged()) );
      connect(ui->group_config_overrides, SIGNAL(clicked()), this, SLOT(updateConfigVisibility()) );
      connect(ui->group_config_repo, SIGNAL(clicked()), this, SLOT(updateConfigVisibility()) );
      connect(ui->group_config_repomgmt, SIGNAL(clicked()), this, SLOT(updateConfigVisibility()) );
      // Resources tab
      connect(ui->text_resources_script,SIGNAL(textChanged()),this,SLOT(slotResourceScriptChanged()) );
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
    bool chkOK = (settings->check("isMakeportAvailable") && settings->check("iscreateAvailable") && settings->check("isSUavailable") );
    if( !chkOK ){
	QMessageBox::warning(this, tr("Resources Unavailable"), tr("Some external resources could not be found, so the EasyPBI services that use these resources have been deactivated.")+"\n"+tr("Please open up the EasyPBI settings to correct this deficiency.") );
    }
    //Set a couple more internal flags
    PBI_BUILDING_NOW.clear();
    PBI_BUILD_TERMINATED=FALSE;
    
    updateConfigVisibility(); //on first run, always need to do this manually
}

void MainGUI::updateConfigVisibility(){
  //Update the group visibility for the pbi.conf tab
  ui->frame_pkgFix->setVisible(ui->group_config_overrides->isChecked());
  ui->frame_repoInfo->setVisible(ui->group_config_repo->isChecked());
  ui->frame_repoMgmt->setVisible(ui->group_config_repomgmt->isChecked());
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
    // -- check boxes
    ui->check_requiresroot->setChecked( MODULE.isEnabled("PBI_REQUIRESROOT") );
    ui->check_config_nopkg->setChecked( MODULE.isEnabled("PBI_AB_NOPKGBUILD") );
    ui->check_config_notmpfs->setChecked( MODULE.isEnabled("PBI_AB_NOTMPFS") );
    // -- Text Values
    ui->line_progname->setText( MODULE.text("PBI_PROGNAME") );
    ui->line_progversion->setText( MODULE.text("PBI_PROGVERSION") );
    ui->line_progweb->setText( MODULE.text("PBI_PROGWEB") );
    ui->line_progauthor->setText( MODULE.text("PBI_PROGAUTHOR") );
    ui->line_makeport->setText( MODULE.text("PBI_MAKEPORT") );
    ui->line_config_license->setText( MODULE.text("PBI_LICENSE") );
    ui->line_repoTags->setText( MODULE.text("PBI_TAGS") );
    ui->line_repoCat->setText( MODULE.text("PBI_CATEGORY") );
    ui->line_repoIconURL->setText( MODULE.text("PBI_ICONURL") );
    ui->line_repoType->setText( MODULE.text("PBI_PROGTYPE") );
    // -- Combo Boxes (filled with individual items from text)
    ui->list_portafter->clear();
    ui->list_portafter->addItems( MODULE.text("PBI_MKPORTAFTER").split("\n") );
    // -- Integer Values
    ui->spin_repoBuildKey->setValue( MODULE.number("PBI_BUILDKEY") );
    ui->spin_repoPriority->setValue( MODULE.number("PBI_AB_PRIORITY") );
    ui->spin_repoRevision->setValue( MODULE.number("PBI_PROGREVISION") );
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
    //Update the available/recommended repo categories
    menu_validRepoCats.clear();
    QStringList tmp = ModuleUtils::validRepoCategories();
    QString recCat = ModuleUtils::recommendedRepoCategory(MODULE.text("PBI_MAKEPORT").section("/",0,0,QString::SectionSkipEmpty) );
    for(int i=0; i<tmp.length(); i++){
      if(recCat == tmp[i]){ menu_validRepoCats.addAction(QIcon(Backend::icon("start")), tmp[i]); }
      else{ menu_validRepoCats.addAction(tmp[i]); }
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
    //disconnect the signal/slot connection to make sure we don't get an extra phantom refresh
    disconnect(ui->listw_resources, SIGNAL(itemSelectionChanged()), this, SLOT(slotResourceChanged()) );
    //Update the Widget
    ui->listw_resources->clear();
    ui->listw_resources->addItems(rs);
    //re-connect the signal/slot
    connect(ui->listw_resources, SIGNAL(itemSelectionChanged()), this, SLOT(slotResourceChanged()) );
    //Load the file into the viewers
    slotResourceChanged();
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
  //------EXTERNAL-LINKS------
  if( doall || doeditor || (item == "external-links")){
    //nothing to make visible/invisible here
    //Load the external-links file
    ui->tree_el_view->clear();
    //QStringList cLinks = currentModule->externalLinks();
    QStringList files, links, types;
    MODULE.loadExternalLinks(files,links, types);
    //QStringList labels; labels << tr("File") << tr("Link To") << tr("File Type");
    for( int i=0; i<files.length(); i++ ){
      QStringList link; link << files[i] << links[i] << types[i];
      ui->tree_el_view->addTopLevelItem(new QTreeWidgetItem(link));
    }
    //Clear the input boxes
    ui->line_el_file->clear();
    ui->line_el_linkto->clear();
    ui->line_el_filetype->clear();
    //update the available binaries
    /*menu_el_bins.clear();
    QStringList cBins; //Not setup yet in the package framework
    if(!cBins.isEmpty()){
      for(int i=0; i<cBins.length(); i++){
        menu_el_bins.addAction(cBins[i]);
      } 
    }*/
  }
  //------FREENAS PLUGINS-------
  if( doall || doeditor || (item == "freenas")){
    //nothing to make visible/invisible here
  }
  //------PBI BUILDER--------
  if( doall || (item == "pbibuild")){
    if(PBI_BUILDING_NOW.isEmpty()){
      //only change things if there is nothing building at the moment
      ui->push_build_stop->setEnabled(false);
      if(ui->text_build_log->toPlainText().isEmpty()){ ui->push_build_save->setEnabled(FALSE); }
      else{ ui->push_build_save->setEnabled(TRUE); }
    }else{
      ui->push_build_stop->setEnabled(true);
    }
    
    ui->push_build_start->setFocus();//make sure the focus starts out on the build button
  }
  //------OVERALL SETTINGS------
  if( doall || doeditor ){
    //Enable/disable the buttons that require the FreeBSD ports tree
    if( settings->check("isportsavailable") ){
      ui->push_change_makeport->setEnabled(TRUE);
      //ui->push_addportbefore->setEnabled(TRUE);
      ui->push_addportafter->setEnabled(TRUE);
    }else{
      ui->push_change_makeport->setEnabled(FALSE);
      //ui->push_addportbefore->setEnabled(FALSE);
      ui->push_addportafter->setEnabled(FALSE);
    }
    //Check for a 64-bit system to enable the 32-bit build option
    //if( settings->check("is64bit") ){ ui->check_build_32->setVisible(TRUE); }
    //else{ ui->check_build_32->setVisible(FALSE); ui->check_build_32->setChecked(FALSE); }
    
    //Set the default focus on the "load" button
    this->setFocus();
  }
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
   QString target_url = "http://wiki.pcbsd.org/index.php/PBI_Module_Builder_Guide/9.2";
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
  NewModuleDialog* dlg = new NewModuleDialog(this);
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
    	    refreshGUI("pbibuild"); break;
    case 1:
	    refreshGUI("pbiconf"); break;
    case 2:
	    refreshGUI("resources"); break;
    case 3:
	    refreshGUI("xdg"); break;
    case 4:
	    refreshGUI("scripts"); break;
    case 5:
	    refreshGUI("external-links"); break;
    case 6:
	    refreshGUI("freenas"); break;
    default:
	    refreshGUI("editor"); break; //do all the module editor tabs
  }	  
}

/*----------------------------------
   PBI.CONF OPTIONS
  -----------------------------------
*/
void MainGUI::on_push_change_makeport_clicked(){
  pkgSelect dlg(this);
  dlg.exec();
  if(!dlg.selected){ return; }
  QString portSel = dlg.portSelected;  	  
  if(portSel.isEmpty()){return;}	

  //Save the port info to the GUI
  ui->line_makeport->setText(portSel);
  ui->push_config_save->setEnabled(TRUE);
}

void MainGUI::on_push_addportafter_clicked(){
  if( !settings->check("isportsavailable") ){
    //No ports tree available
    QMessageBox::warning(this,tr("EasyPBI: No FreeBSD Ports"), tr("The FreeBSD Ports tree could not be found on your system. You may fetch the ports tree through the EasyPBI menu or manually set the path to the port tree in the EasyPBI preferences if it is installed in a non-standard location."));
  }
  //Prompt for a new port
  QString portSel = QFileDialog::getExistingDirectory(this, tr("Select Port"), settings->value("portsdir"));
  if(portSel.isEmpty()){return;} //action cancelled or closed	
  //Save the port info to the GUI
  if(ui->list_portafter->count() == 1 && ui->list_portafter->currentText().isEmpty() ){ ui->list_portafter->clear(); }
  ui->list_portafter->addItem(portSel.remove(settings->value("portsdir")+"/"));
  ui->push_config_save->setEnabled(TRUE);
}

void MainGUI::on_push_rmportafter_clicked(){
  int index = ui->list_portafter->currentIndex();
  if(index != -1){
    ui->list_portafter->removeItem(index);
  }
  ui->push_config_save->setEnabled(TRUE);
}

void MainGUI::on_push_config_save_clicked(){
  //Save the current settings to the backend structures
  //Text Values
  MODULE.setText("PBI_PROGNAME", ui->line_progname->text());
  MODULE.setText("PBI_PROGVERSION", ui->line_progversion->text());
  MODULE.setText("PBI_PROGWEB", ui->line_progweb->text());
  MODULE.setText("PBI_PROGAUTHOR", ui->line_progauthor->text());
  MODULE.setText("PBI_LICENSE", ui->line_config_license->text());
  MODULE.setText("PBI_TAGS", ui->line_repoTags->text());
  MODULE.setText("PBI_CATEGORY", ui->line_repoCat->text());
  MODULE.setText("PBI_ICONURL", ui->line_repoIconURL->text());
  MODULE.setText("PBI_PROGTYPE", ui->line_repoType->text());
  //True/False values
  MODULE.setEnabled("PBI_REQUIRESROOT", ui->check_requiresroot->isChecked());
  MODULE.setEnabled("PBI_AB_NOPKGBUILD", ui->check_config_nopkg->isChecked());
  MODULE.setEnabled("PBI_AB_NOTMPFS", ui->check_config_notmpfs->isChecked());
  //Combo Boxes
  QStringList addports;
  for(int i=0; i<ui->list_portafter->count(); i++){ addports << ui->list_portafter->itemText(i); }
  MODULE.setText("PBI_MKPORTAFTER", addports.join("\n") );
  MODULE.setText("PBI_PROGICON", ui->list_progicon->currentText() );
  //Number values
  MODULE.setNumber("PBI_BUILDKEY", ui->spin_repoBuildKey->value() );
  MODULE.setNumber("PBI_AB_PRIORITY", ui->spin_repoPriority->value() );
  MODULE.setNumber("PBI_PROGREVISION", ui->spin_repoRevision->value() );
  
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

void MainGUI::slotSetRepoCat(QAction* act){
  ui->line_repoCat->setText(act->text());
}

/*------------------------------------------------
   RESOURCE EDITOR OPTIONS
  -------------------------------------------------
*/
void MainGUI::slotResourceChanged(){
  //Get the current file selected
  QString cfile;
  if(ui->listw_resources->currentRow() != -1){ cfile = ui->listw_resources->currentItem()->text(); }
  if(cfile.isEmpty()){ //no item selected
    ui->push_resources_savewrapper->setVisible(FALSE);
    ui->text_resources_script->setVisible(FALSE);
    ui->label_resources_description->setVisible(FALSE);
    ui->label_resources_icon->setVisible(FALSE);
	  
  }else{ //item selected
    QString path = MODULE.basepath() + "/resources/";
    if( cfile.endsWith(".png") || cfile.endsWith(".jpg") ){
      //Image file, show full size
      QPixmap img(path+cfile);
      ui->label_resources_icon->setPixmap(img);
      ui->label_resources_icon->setVisible(TRUE);
      QString desc = QString::number(img.width()) +" x "+QString::number(img.height())+" "+cfile.section(".",-1).toUpper();
      ui->label_resources_description->setText(desc);
      ui->label_resources_description->setVisible(TRUE);
      //hide the other widgets
      ui->push_resources_savewrapper->setVisible(FALSE);
      ui->text_resources_script->setVisible(FALSE);
    }else{
      ui->push_resources_savewrapper->setVisible(TRUE);
      QStringList contents = PBIModule::readFile(path+cfile);
      ui->text_resources_script->clear();
      ui->text_resources_script->setText(contents.join("\n"));
      ui->text_resources_script->setVisible(TRUE);
      //hide the other widgets
      ui->label_resources_description->setVisible(FALSE);
      ui->label_resources_icon->setVisible(FALSE);
    }
  }
  ui->push_resources_savewrapper->setEnabled(FALSE);
  
}

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

void MainGUI::on_push_resources_mkwrapper_clicked(){
  //Get the desired filename for the wrapper script
  bool ok;
  QString cFile = QInputDialog::getText(this, tr("New Wrapper Script"), tr("Filename")+":", QLineEdit::Normal, "",&ok);
  //Check for a valid input
  if(!ok || cFile.isEmpty()){ return; }
  if(!cFile.endsWith(".sh")){ cFile.append(".sh"); }
  //Now create the new file
  MODULE.createFile(MODULE.basepath()+"/resources/bin/"+cFile, ModuleUtils::generateWrapperScriptTemplate());
  //Refresh the GUI
  refreshGUI("resources");
}

void MainGUI::slotResourceScriptSaved(){
  //get the current file and path
  QString cfile;
  if(ui->listw_resources->currentRow() != -1){ cfile = ui->listw_resources->currentItem()->text(); }
  if(cfile.isEmpty()){ return; } // do nothing if no file selected
  //Read the current text for the resource
  QStringList contents = ui->text_resources_script->toPlainText().split("\n");
  //overwrite the resource with the new contents
  MODULE.writeScript(cfile,contents);
  ui->push_resources_savewrapper->setEnabled(FALSE);
}

void MainGUI::slotResourceScriptChanged(){
  ui->push_resources_savewrapper->setEnabled(TRUE);	
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
    ui->check_requiresroot->setChecked(FALSE);
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
/*------------------------------------------------
   EXTERNAL-LINKS EDITOR OPTIONS
  -------------------------------------------------
*/
void MainGUI::on_push_el_add_clicked(){
  //Read the user inputs
  QString file = ui->line_el_file->text();
  QString linkto = ui->line_el_linkto->text();
  QString opts = ui->line_el_filetype->text();
  //check for valid inputs
  if( file.isEmpty() || linkto.isEmpty() || opts.isEmpty() ){ return; }
  QStringList bins, links, types;
  MODULE.loadExternalLinks(bins,links,types);
  //Check for an identical link to just update
  bool found = false;
  for(int i=0; i<bins.length(); i++){
    if(bins[i] == file && links[i]==linkto){
      types[i] = opts;
      found = true;
      break;
    }
  }
  if(!found){
    bins << file; links << linkto; types << opts;
  }
  MODULE.saveExternalLinks(bins,links,types);
  //Refresh the UI
  refreshGUI("external-links");
}

void MainGUI::on_push_el_remove_clicked(){
  //Get the currently selected link
  QTreeWidgetItem *line = ui->tree_el_view->currentItem();
  if(line->columnCount() != 3){ return; }
  QString file = line->text(0);
  QString linkto = line->text(1);
  //check for valid inputs
  if( file.isEmpty() || linkto.isEmpty() ){ return; }
  QStringList bins, links, types;
  MODULE.loadExternalLinks(bins,links,types);
  //Check for an identical link to remove
  for(int i=0; i<bins.length(); i++){
    if(bins[i] == file && links[i]==linkto){
      bins.removeAt(i); links.removeAt(i); types.removeAt(i);
      break;
    }
  }
  MODULE.saveExternalLinks(bins,links,types);
  //Refresh the UI
  refreshGUI("external-links");
}

void MainGUI::slotELSetFile(QAction* act){
  //get the selected file and set it in the UI
  ui->line_el_file->setText( act->text() );
  ui->line_el_linkto->setText( act->text() );
}

void MainGUI::slotELSetType(QAction* act){
  //Get the current types
  QStringList types = ui->line_el_filetype->text().split(",", QString::SkipEmptyParts);
  //get the new type
  QString ntype  = act->text();
  //Now properly add the type as appropriate
  bool ok = FALSE;
  for(int i=0; i<types.length(); i++){
    //Check for special cases
    if( (types[i] == "keep") && (ntype=="replace") ){
      types[i] = ntype;
      ok = TRUE;
      break;
    }else if( (types[i] == "replace") && (ntype=="keep") ){
      types[i] = ntype;
      ok = TRUE;
      break;
    }else if( (types[i]=="binary") && (ntype=="linux") ){
      types[i] = ntype;
      ok = TRUE;
      break;
    }else if( (types[i]=="linux") && (ntype=="binary") ){
      types[i] = ntype;
      ok = TRUE;
      break;
    }else if( types[i] == ntype ){
      ok=TRUE;
      break;
    }
  }
  if(!ok){ types << ntype; } //append the type to the list
  //Add the new types to the UI
  ui->line_el_filetype->setText( types.join(",") );
}

/*------------------------------------------------
   PBI BUILDER OPTIONS
  -------------------------------------------------
*/
void MainGUI::on_push_build_start_clicked(){
  //Check GUI to make sure settings are set before running
  bool gostatus = TRUE;
  QString outdir = settings->value("pbidir");
  if (outdir.isEmpty() ){gostatus=FALSE;}
  QString modDir = MODULE.basepath();
  if(modDir.isEmpty()){gostatus=FALSE;}
  if(!gostatus){
    QMessageBox::warning(this,tr("Error"),tr("Invalid PBI Settings"));
    return;
  }
  
  //Generate the PBI build command
  QString sigFile;
  if( settings->check("usesignature") && QFile::exists(settings->value("sigfile")) ){ sigFile = settings->value("sigfile"); }
  QString cmd = ModuleUtils::generatePbiBuildCmd(MODULE.basepath(), outdir, sigFile);
  //Display the command created in hte terminal
  qDebug() << "Build PBI command created:"<<cmd;
  
  //Receive User verification before beginning build process due to:
  //  -- long time required, internet connection required, root permissions required
  QMessageBox verify(this);
  verify.setText(tr("Are you sure you wish to start the PBI build?"));
  verify.setInformativeText(tr("This requires an active internet connection and administrator privileges."));
  verify.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
  verify.setDefaultButton(QMessageBox::Yes);
  int ret = verify.exec();
  if(ret != QMessageBox::Yes){return;}

  //Add the necessary glue to the command to run the pbi build
  cmd.prepend(settings->value("su_cmd")+" \"");
  cmd.append("\"");
  qDebug() << "Actual command used:" << cmd;

  //Setup the displays
  ui->push_build_stop->setEnabled(TRUE);
  ui->push_build_save->setEnabled(FALSE);
  ui->push_build_start->setEnabled(FALSE); //disable the button so they do not start more than 1 build at a time
  ui->text_build_log->clear(); //clear the display in case this is not the first run
  ui->line_build_module->setText( MODULE.basepath().replace(QDir::homePath(), "~") );
  ui->line_build_outputdir->setText(settings->value("pbidir"));
  //Setup Process connections
  p = new QProcess(this);
  p->setProcessChannelMode(QProcess::MergedChannels);
  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  env.remove("LD_LIBRARY_PATH");
  p->setProcessEnvironment( env );
  p->setWorkingDirectory(QDir::homePath());
  connect(p,SIGNAL(readyReadStandardOutput()),this,SLOT(slotUpdatePBIBuild()) );
  connect(p,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(slotPBIbuildFinished(int,QProcess::ExitStatus)) );
  connect(p,SIGNAL(error(QProcess::ProcessError)),this,SLOT(slotBuildError(QProcess::ProcessError)) );
  //Setup the flag for the finishing checks
  PBI_BUILDING_NOW=outdir+":::"+MODULE.text("PBI_PROGNAME").remove(" ").toLower();
  //Start the Process
  qDebug() << "Starting the PBI build process...";
  ui->tabWidget->setTabText(0,tr("Build PBI (Running)"));
  p->start(cmd);
}

void MainGUI::on_push_build_stop_clicked(){
  slotKillPBIBuild();
}

void MainGUI::on_push_build_save_clicked(){
  //Have user select a filename/location to save the log
  QString filename = QFileDialog::getSaveFileName(this,tr("Save Log"),settings->value("progdir")+"/build.log",tr("Log Files")+" (*.log *.txt)");
  if(filename.isEmpty()){return;} //User cancelled the process
  //Open the file (as new)
  QFile *file = new QFile(filename);
  file->open(QIODevice::WriteOnly);
  //Save the log into the file
  file->write(ui->text_build_log->toPlainText().toUtf8());
  file->close();
}

void MainGUI::slotUpdatePBIBuild(){
  QString tmp = p->readAllStandardOutput();
  if( tmp.startsWith("\n") ){tmp.remove(0,0);} //remove newline at the beginning (too much whitespace in log)
  if( tmp.endsWith("\n") ){tmp.chop(1);} //remove newline at the end (already accounted for by appending)
  //clear the display  if it is starting a new port compilation (prevent the log getting too large)
  if(tmp.toLower().startsWith("compiling port: ")){ ui->text_build_log->clear(); }
  if(!tmp.isEmpty()){ ui->text_build_log->append( tmp ); }
  //qDebug() << "Update output: " << tmp;
}

void MainGUI::slotPBIbuildFinished(int exitCode,QProcess::ExitStatus exitStatus){
  //Make sure that the QProcess is actually finished
  if( p->state() !=  QProcess::NotRunning ){
    qDebug() << "QProcess finished signal, but the process is still running";
    return;
  }
	
  //Check to see if the PBI process finished successfully
  qDebug() << "PBI build process Finished" << exitStatus << exitCode;
  //Check that the new PBI exists
  QDir outdir( PBI_BUILDING_NOW.section(":::",0,0) );
  QString pbiname= PBI_BUILDING_NOW.section(":::",1,1).toLower().remove(" ");
  QFileInfoList fL = outdir.entryInfoList( QStringList() << pbiname+"*.pbi" ,QDir::Files | QDir::NoSymLinks);
  bool success = FALSE;
  for(int i=0; i<fL.length(); i++){
  	  qint64 msdiff = QDateTime::currentMSecsSinceEpoch() - fL[i].created().toMSecsSinceEpoch();
  	  if(msdiff < 30000){ success = true; break; } //if less than a 30sec since creation of file - success
  }
  
  if(exitCode == 0 && success){
      QMessageBox::information(this,tr("PBI Build Success"),tr("The PBI finished building successfully"));
  }else if(exitCode == 0 && PBI_BUILD_TERMINATED){
      //The user killed the process - No Message
  }else{
    QMessageBox::warning(this,tr("PBI Build Failure"),tr("The PBI failed to build.")+"\n"+tr("Please check the build log to find the cause of the failure and adjust the module accordingly"));
  }
  ui->push_build_start->setEnabled(TRUE);
  ui->push_build_save->setEnabled(TRUE);
  ui->push_build_stop->setEnabled(FALSE);
  p->close();
  PBI_BUILDING_NOW.clear();
  PBI_BUILD_TERMINATED=FALSE;
  ui->tabWidget->setTabText(0,tr("Build PBI (Done)"));
  delete p;
}

void MainGUI::slotKillPBIBuild(){
  if( p->state()==QProcess::Running ){
    //qDebug() << "Process PID:" << p->pid(); 
    PBI_BUILD_TERMINATED=TRUE;    
    p->terminate();
    ui->text_build_log->append("---PBI Build Terminated---");
  }
}

void MainGUI::slotBuildError(QProcess::ProcessError error){
  qDebug() << "QProcess PBI Build error#" << error;
}
