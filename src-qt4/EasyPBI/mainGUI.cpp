#include "mainGUI.h"
#include "ui_mainGUI.h"
#include "modBuild.h"
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
	// Create the module class as necessary
	currentModule = new ModBuild();
	// Create the config class
	settings = new Config();
	//Setup the Menu items
	ui->actionExit->setIcon(Backend::icon("close"));
	ui->actionRefresh_Module->setIcon(Backend::icon("refresh"));
	//Setup the pushbutton menu lists
	menu_elOpts.addAction("binary");
	menu_elOpts.addAction("linux");
	menu_elOpts.addAction("keep");
	menu_elOpts.addAction("replace");
	menu_elOpts.addAction("nocrash");
	//Setup Toolbar
	ui->actionNew_Module->setIcon(Backend::icon("new"));
	ui->actionLoad_Module->setIcon(Backend::icon("load"));
	line_module = new QLineEdit(this);
	line_module->setPlaceholderText(tr("No Module Loaded"));
	line_module->setReadOnly(TRUE);
	line_module->setFocusPolicy(Qt::NoFocus);
	ui->toolBar->addWidget(line_module);
	ui->toolBar->addSeparator();
	radio_module_port = new QRadioButton(tr("FreeBSD Port"),this);
	radio_module_port->setEnabled(FALSE);
	ui->toolBar->addWidget(radio_module_port);
	radio_module_local = new QRadioButton(tr("Local Sources"),this);
	radio_module_local->setEnabled(FALSE);
	ui->toolBar->addWidget(radio_module_local);
	//Setup Module Editor
	connect(ui->tabWidget, SIGNAL(currentChanged(int)),this,SLOT(slotModTabChanged(int)) ); //setup to refresh each tab once it is selected
	// -- pbi.conf tab --
	ui->push_change_progdir->setIcon(Backend::icon("load"));
	ui->push_change_makeport->setIcon(Backend::icon("file"));
	ui->push_addmakeopt->setIcon(Backend::icon("left"));
	ui->push_addmakeopt->setMenu(&menu_addOpt);
	connect(&menu_addOpt,SIGNAL(triggered(QAction*)),this,SLOT(slotAddMakeOption(QAction*)) );
	ui->push_config_save->setIcon(Backend::icon("save"));
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
	ui->push_xdg_exec->setMenu(&menu_bins);
	connect(&menu_bins, SIGNAL(triggered(QAction*)), this, SLOT(slotAddBin(QAction*)) );
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
	ui->push_el_file->setMenu(&menu_el_bins);
	connect(&menu_el_bins,SIGNAL(triggered(QAction*)),this,SLOT(slotELSetFile(QAction*)) );
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
      connect(ui->line_progdir,SIGNAL(textChanged(QString)),this,SLOT(slotOptionChanged(QString)) );
      connect(ui->line_makeport,SIGNAL(textChanged(QString)),this,SLOT(slotOptionChanged(QString)) );
      connect(ui->list_progicon,SIGNAL(currentIndexChanged(QString)),this,SLOT(slotOptionChanged(QString)) );
      connect(ui->edit_makeopts,SIGNAL(textChanged()),this,SLOT(slotOptionChanged()) );
      connect(ui->check_requiresroot, SIGNAL(clicked()),this,SLOT(slotOptionChanged()) );
      // Rwsources tab
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
    //Pop up a warning about a missing ports tree
    if( !settings->check("isPortsAvailable") ){
	QMessageBox::warning(this, tr("FreeBSD Ports Missing"), tr("The FreeBSD ports tree is missing from your system.")+"\n"+tr("Please open up the EasyPBI settings to correct this deficiency.") );
    }
    //Set a couple more internal flags
    PBI_BUILDING_NOW.clear();
    PBI_BUILD_TERMINATED=FALSE;
}

bool MainGUI::isValidPort(QString pPath){
  bool ok = FALSE;
  if( QFile::exists(pPath) && QFile::exists(pPath+"/Makefile") && QFile::exists(pPath+"/distinfo") ){
    ok = TRUE;
  }
  return ok;
}

void MainGUI::refreshGUI(QString item){
  //Enable/disable the interface appropriately
  
  //Stupid check to make sure that a module is actually loaded
  if( currentModule->path().isEmpty() ){ 
    ui->actionPackage_Module->setEnabled(FALSE);
    ui->actionRefresh_Module->setEnabled(FALSE);
    if(PBI_BUILDING_NOW.isEmpty() ){ui->toolBox->setEnabled(FALSE); return; }
    else{ 
      ui->toolBox->setItemEnabled(0,FALSE);
      ui->toolBox->setItemEnabled(1,TRUE);
      ui->toolBox->setCurrentIndex(1); 
      item="pbibuild";
    }
  }else{ 
    ui->toolBox->setEnabled(TRUE); 
    ui->actionPackage_Module->setEnabled(TRUE);  
    ui->actionRefresh_Module->setEnabled(TRUE);
  }
  //Figure out the type of module that is loaded
  bool isport = radio_module_port->isChecked();
  //See if we should refresh everything (default)  
  bool doall = ( (item == "all") || item.isEmpty() );
  bool doeditor = (item == "editor"); //also add a flag to just refresh the editor
  //Refresh the desired tab(s)
  // ------PBI.CONF------
  if( doall || doeditor || (item == "pbiconf")){
    if(isport){
	ui->line_progversion->setVisible(FALSE); ui->label_progversion->setVisible(FALSE);
	ui->line_progdir->setVisible(FALSE); ui->push_change_progdir->setVisible(FALSE); ui->label_progdir->setVisible(FALSE);
	ui->line_makeport->setVisible(TRUE); ui->push_change_makeport->setVisible(TRUE); ui->label_makeport->setVisible(TRUE);
	ui->edit_makeopts->setVisible(TRUE); ui->push_addmakeopt->setVisible(TRUE); ui->label_makeopts->setVisible(TRUE);
	ui->list_portbefore->setVisible(TRUE); ui->push_addportbefore->setVisible(TRUE); ui->push_rmportbefore->setVisible(TRUE);ui->label_portbefore->setVisible(TRUE);
	ui->list_portafter->setVisible(TRUE); ui->push_addportafter->setVisible(TRUE); ui->push_rmportafter->setVisible(TRUE); ui->label_portafter->setVisible(TRUE);
    }else{
	ui->line_progversion->setVisible(TRUE); ui->label_progversion->setVisible(TRUE);
	ui->line_progdir->setVisible(TRUE); ui->push_change_progdir->setVisible(TRUE); ui->label_progdir->setVisible(TRUE);
	ui->line_makeport->setVisible(FALSE); ui->push_change_makeport->setVisible(FALSE); ui->label_makeport->setVisible(FALSE);
	ui->edit_makeopts->setVisible(FALSE); ui->push_addmakeopt->setVisible(FALSE); ui->label_makeopts->setVisible(FALSE);
	ui->list_portbefore->setVisible(FALSE); ui->push_addportbefore->setVisible(FALSE); ui->push_rmportbefore->setVisible(FALSE);ui->label_portbefore->setVisible(FALSE);
	ui->list_portafter->setVisible(FALSE); ui->push_addportafter->setVisible(FALSE); ui->push_rmportafter->setVisible(FALSE); ui->label_portafter->setVisible(FALSE);
    }
    //Display the variables from the currentModule structure
    // -- check boxes
    QString chk = currentModule->readValue("requiresroot").toLower();
    if( (chk=="true") || (chk=="yes") ){ ui->check_requiresroot->setChecked(TRUE); }
    else{ ui->check_requiresroot->setChecked(FALSE); }
    // -- line edits
    ui->line_progname->setText(currentModule->readValue("progname"));
    ui->line_progversion->setText(currentModule->readValue("progversion"));
    ui->line_progweb->setText(currentModule->readValue("progweb"));
    ui->line_progauthor->setText(currentModule->readValue("progauthor"));
    ui->line_progdir->setText(currentModule->readValue("packagedir"));
    ui->line_makeport->setText(currentModule->readValue("makeport"));
    ui->edit_makeopts->setPlainText(currentModule->readValue("makeoptions"));
    // -- combo boxes (lists)
    ui->list_portbefore->clear(); ui->list_portafter->clear(); ui->list_progicon->clear();
    ui->list_portbefore->addItems( currentModule->readValue("makeportbefore").split(" ") );
    ui->list_portafter->addItems( currentModule->readValue("makeportafter").split(" ") );
    QStringList icons = currentModule->currentIcons;
    if(icons.length() > 0){
      for(int i=0; i<icons.length(); i++){
        ui->list_progicon->addItem(QIcon(currentModule->path()+"/resources/"+icons[i]),icons[i]);
      }	    
      int cicon = icons.indexOf(currentModule->readValue("progicon"));
      if( cicon == -1 ){ 
	      ui->list_progicon->addItem(currentModule->readValue("progicon")); 
	      cicon = icons.indexOf(currentModule->readValue("progicon"));
      }
      ui->list_progicon->setCurrentIndex(cicon);
    }else{
      ui->list_progicon->addItem(currentModule->readValue("progicon"));
    }
    menu_addOpt.clear();
    QStringList opts = currentModule->readValue("portoptions").split(" ",QString::SkipEmptyParts);
    if(!opts.isEmpty()){
      for(int i=0; i<opts.length(); i++){
        menu_addOpt.addAction(opts[i]);
      }
    }
    ui->push_config_save->setEnabled(FALSE);  //disable the save button until something changes
  }
  // -----RESOURCES--------
  if( doall || doeditor || (item == "resources")){
    //Get the all the current files in the resources category and add them to the list
    QStringList rs = currentModule->filesAvailable("resources");
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
  if( doall || doeditor || (item == "scripts")){
    //Update the list of available scripts and determine if one needs to be read
    bool loadScript = FALSE;
    QString path = currentModule->path()+"/scripts/";
    int currentIndex = ui->list_scripts_file->currentIndex();
    for(int i=1; i< ui->list_scripts_file->count(); i++){ //Skip the first item (is nothing)
      if( QFile::exists( path+ui->list_scripts_file->itemText(i) ) ){
        if(i == currentIndex){ loadScript = TRUE; }
        ui->list_scripts_file->setItemIcon(i,Backend::icon("file"));
      }else{
        ui->list_scripts_file->setItemIcon(i,Backend::icon("close"));
      }
    }
    //Update the GUI appropriately
    if(loadScript){
      //Read and display the script
      QStringList contents = ModBuild::readFile(path+ui->list_scripts_file->currentText());
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
    if(currentIndex == 0){ ui->push_scripts_create->setVisible(FALSE); }
    ui->push_scripts_save->setEnabled(FALSE); //disable the save button until something changes
    
  }
  //------EXTERNAL-LINKS------
  if( doall || doeditor || (item == "external-links")){
    //nothing to make visible/invisible here
    //Load the external-links file
    ui->tree_el_view->clear();
    QStringList cLinks = currentModule->externalLinks();
    QStringList labels; labels << tr("File") << tr("Link To") << tr("File Type");
    for( int i=0; i<cLinks.length(); i++ ){
      QStringList link = cLinks[i].split("\t");
      ui->tree_el_view->addTopLevelItem(new QTreeWidgetItem(link));
    }
    //Clear the input boxes
    ui->line_el_file->clear();
    ui->line_el_linkto->clear();
    ui->line_el_filetype->clear();
    //update the available binaries
    menu_el_bins.clear();
    QStringList cBins = currentModule->currentBins;
    if(!cBins.isEmpty()){
      for(int i=0; i<cBins.length(); i++){
        menu_el_bins.addAction(cBins[i]);
      } 
    }
  }
  //------FREENAS PLUGINS-------
  if( doall || doeditor || (item == "freenas")){
    //nothing to make visible/invisible here
  }
  //------PBI BUILDER--------
  if( doall || (item == "pbibuild")){
    if(PBI_BUILDING_NOW.isEmpty()){
      //only change things if there is nothing building at the moment
      if(ui->text_build_log->toPlainText().isEmpty()){ ui->push_build_save->setEnabled(FALSE); }
      else{ ui->push_build_save->setEnabled(TRUE); }
    }
    ui->push_build_start->setFocus();//make sure the focus starts out on the build button
  }
  //------OVERALL SETTINGS------
  if( doall || doeditor ){
    //Enable/disable the buttons that require the FreeBSD ports tree
    if( settings->check("isportsavailable") ){
      ui->push_change_makeport->setEnabled(TRUE);
      ui->push_addportbefore->setEnabled(TRUE);
      ui->push_addportafter->setEnabled(TRUE);
    }else{
      ui->push_change_makeport->setEnabled(FALSE);
      ui->push_addportbefore->setEnabled(FALSE);
      ui->push_addportafter->setEnabled(FALSE);
    }
    //Check for a 64-bit system to enable the 32-bit build option
    if( settings->check("is64bit") && !currentModule->isLocalPBI ){ ui->check_build_32->setVisible(TRUE); }
    else{ ui->check_build_32->setVisible(FALSE); ui->check_build_32->setChecked(FALSE); }
    //Disable tabs that are not used when bulding local PBI's
    if(currentModule->isLocalPBI){
	ui->tabWidget->setTabEnabled(5,FALSE);
    }else{
	ui->tabWidget->setTabEnabled(5,TRUE);
    }
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
  //Now reload the saved settings
  settings->loadSettingsFile();
  //Now refresh the GUI
  refreshGUI("all");
}
void MainGUI::on_actionPackage_Module_triggered(){
   currentModule->compressModule();
   QMessageBox::information(this,tr("Success"),tr("A copy of the current module has been successfully packaged  within the module directory.") );

}

void MainGUI::on_actionRefresh_Module_triggered(){
  QString modSel = currentModule->path(); //re-load the current module
  bool ok = currentModule->loadModule(modSel); 
  if(ok){ 
    qDebug() << "Loaded module:"<<modSel;	  
    line_module->setText(modSel.replace(QDir::homePath(),"~")); 
    if(currentModule->isLocalPBI){ radio_module_local->toggle(); }
    else{ 
      radio_module_port->toggle(); //Port PBI
      if( settings->check("isportsavailable") ){ 
        currentModule->readPortInformation(settings->value("portsdir")+"/"+currentModule->readValue("makeport"));
      }
    }
    refreshGUI("all");
  }
}

void MainGUI::on_actionFreeBSD_Ports_triggered(){
  QString port;
  if(!line_module->text().isEmpty()){
	//Get the currently selected port
	port = currentModule->readValue("makeport");
  }
  QString target_url = "http://www.freshports.org/" + port;
  qDebug() << "Opening URL:" << target_url;  
  target_url.prepend("xdg-open ");
  system(target_url.toUtf8());	
}

void MainGUI::on_actionPBI_Modules_triggered(){
  //Open the PC-BSD wiki to the module builders guide
   QString target_url = "http://wiki.pcbsd.org/index.php/PBI_Module_Builder_Guide";
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
  NewModuleDialog* dlg = new NewModuleDialog(this, settings->value("portsdir"));
  dlg->setDefaultIconFile(settings->value("defaulticon"));
  dlg->exec();
  //Determine if the action was cancelled
  if( dlg->isAccepted ){
    //Now create the new module
    QString modPath, modType, modBase, oldIconPath;
    modType = dlg->moduleType;
    modBase = dlg->moduleData;
    oldIconPath = dlg->moduleIcon;
    modPath = settings->value("moduledir")+"/"+modBase.section("/",-1).toLower();
	  //modPath ->full path, modType->"local"or "port", modBase->makeport or packagedir
    bool ok = currentModule->createNewModule(modPath,modType,modBase);
    if(!ok){
      QMessageBox::warning(this,tr("EasyPBI: Permissions Error"), tr("Could not create PBI module. Please check the directory permissions and try again."));
    }else{
      //MODULE CREATED, NOW FILL IT APPROPRIATELY
      //Icon
      currentModule->addResource(FALSE, oldIconPath); //copy the icon file into the module
      currentModule->writeValue("progicon", oldIconPath.section("/",-1) );
      //Load Port info if appropriate
      if(currentModule->isPortPBI){
        radio_module_port->toggle();
	//Use the port info if possible
	if( settings->check("isportsAvailable") ){
	  currentModule->readPortInformation(settings->value("portsdir")+"/"+modBase);
	  //Now save the detected information into the module
	  currentModule->writeValue("progname", currentModule->readValue("portname"));
	  currentModule->writeValue("progweb", currentModule->readValue("portwebsite"));
	  currentModule->writeValue("progauthor", currentModule->readValue("portauthor"));
	  currentModule->writePBIconf();
	}
      }else if(currentModule->isLocalPBI){
	radio_module_local->toggle();
	      
      }
      //Now let the UI reflect the new module 
      line_module->setText( currentModule->path().replace(QDir::homePath(),"~") );      
    }
  }
  //Refresh the UI
  refreshGUI("all");
  delete dlg;
}

void MainGUI::on_actionLoad_Module_triggered(){
  QString modSel = QFileDialog::getExistingDirectory(this, tr("Select Module"), settings->value("moduledir"));
  if(modSel.isEmpty()){return;} //action cancelled or closed
  bool ok = currentModule->loadModule(modSel);
  if(ok){ 
    qDebug() << "Loaded module:"<<modSel;	  
    line_module->setText(modSel.replace(QDir::homePath(),"~")); 
    if(currentModule->isLocalPBI){ radio_module_local->toggle(); }
    else{ 
      radio_module_port->toggle(); //Port PBI
      if( settings->check("isportsavailable") ){ 
        currentModule->readPortInformation(settings->value("portsdir")+"/"+currentModule->readValue("makeport"));
      }
    }
    refreshGUI("all");
  }
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
    case 4:
	    refreshGUI("external-links"); break;
    case 5:
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
  if( !settings->check("isportsavailable") ){
    //No ports tree available
    QMessageBox::warning(this,tr("EasyPBI: No FreeBSD Ports"), tr("The FreeBSD Ports tree could not be found on your system. You may fetch the ports tree through the EasyPBI menu or manually set the path to the port tree in the EasyPBI preferences if it is installed in a non-standard location."));
    return;
  }
  //Prompt for a new port
  QString portSel = QFileDialog::getExistingDirectory(this, tr("Select Port"), settings->value("portsdir"));
  if(portSel.isEmpty()){return;} //action cancelled or closed	
  //Check if the port is valid
  if( !isValidPort(portSel) ){
    QMessageBox::warning(this,tr("EasyPBI: Invalid Port"), tr("The directory selected is not a valid FreeBSD port. Please select a port directory which contains the appropriate Makefile and distinfo."));
    return;
  }
  //Save the port info to the GUI
  ui->line_makeport->setText(portSel.remove(settings->value("portsdir")+"/"));
  ui->push_config_save->setEnabled(TRUE);
}

void MainGUI::on_push_change_progdir_clicked(){
  //Prompt for a new directory
  QString dirSel = QFileDialog::getExistingDirectory(this, tr("Select Package Directory"), QDir::homePath());
  if(dirSel.isEmpty()){return;} //action cancelled or closed	
  //Save the port info to the GUI
  ui->line_progdir->setText(dirSel);
  ui->push_config_save->setEnabled(TRUE);
}

void MainGUI::slotAddMakeOption(QAction* act){
  qDebug() << "Trying to add build option:" << act->text();
  QString opt = act->text();
  if(opt.isEmpty() ){ return; } //Do nothing if nothing selected
  //Check whther the option is set or unset
  QString typ = opt.section("_",0,0,QString::SectionSkipEmpty).toLower(); //get the type
  opt = opt.section("_",1,50,QString::SectionSkipEmpty); //trim off the type of setting
  QString chk = currentModule->readValue("makeport").section("/",-1);
  bool isSet;
  if(typ == "with"){chk.append("_SET"); isSet=TRUE;}
  else if(typ=="without"){chk.append("_UNSET"); isSet=FALSE;}
  else{ qDebug() << "Error: Unknown option type:"<<typ<<"option:"<<opt; return; }
  //Now read the current option settings
  QStringList curr = ui->edit_makeopts->toPlainText().split("\n"); //split according to lines
  bool found = FALSE;
  for(int i=0; i<curr.length(); i++){
    if(curr[i].contains(chk) ){ //look for optionsNG framework
      found = TRUE;	    
      QStringList cOpts = curr[i].split(" ",QString::SkipEmptyParts);
      if( !cOpts.contains(opt) ){
	//Not found, add it to the end of the list
	cOpts << opt;
	curr[i] = cOpts.join(" ");
      }
    }else if(curr[i].contains(opt)){ //look for legacy options framework
      QStringList cOpts = curr[i].split(" ",QString::SkipEmptyParts);
      int index = cOpts.indexOf(opt);
      if(index != -1){ //option currently set somehow
	found = TRUE; 
        if(isSet){ //option should be set
          if( cOpts[index].contains("WITH_"+opt) ){} //Already set
	  else if( cOpts[index].contains("WITHOUT_"+opt) ){ cOpts[index] = "WITH_"+opt+"=true"; }
	  else if( curr[i].contains(chk.replace("_SET","_UNSET")) ){ cOpts.removeAt(index); found=FALSE;} //Remove the inverse option if it is set
        }else{ //options should be unset
          if( cOpts[index].contains("WITHOUT_"+opt) ){} //Already set
	  else if( cOpts[index].contains("WITH_"+opt) ){ cOpts[index] = "WITHOUT_"+opt+"=true"; }
	  else if( curr[i].contains(chk.replace("_UNSET","_SET")) ){ cOpts.removeAt(index); found=FALSE;} //Remove the inverse option if it is set
	}
        curr[i] = cOpts.join(" "); //put the line back together
      }
    } //end checking different options frameworks
  } // end loop over lines
  if(!found){
    //Append the option to the end on a new line
    curr << chk+"= "+opt;
  }
  //Now put the new options list back onto the GUI
  ui->edit_makeopts->setPlainText(curr.join("\n"));
  ui->push_config_save->setEnabled(TRUE);
}

void MainGUI::on_push_addportbefore_clicked(){
  if( !settings->check("isportsavailable") ){
    //No ports tree available
    QMessageBox::warning(this,tr("EasyPBI: No FreeBSD Ports"), tr("The FreeBSD Ports tree could not be found on your system. You may fetch the ports tree through the EasyPBI menu or manually set the path to the port tree in the EasyPBI preferences if it is installed in a non-standard location."));
  }
  //Prompt for a new port
  QString portSel = QFileDialog::getExistingDirectory(this, tr("Select Port"), settings->value("portsdir"));
  if(portSel.isEmpty()){return;} //action cancelled or closed	
  //Check if the port is valid
  if( !isValidPort(portSel) ){
    QMessageBox::warning(this,tr("EasyPBI: Invalid Port"), tr("The directory selected is not a valid FreeBSD port. Please select a port directory which contains the appropriate Makefile and distinfo files."));
    return;
  }
  //Save the port info to the GUI
  if(ui->list_portbefore->count() == 1 && ui->list_portbefore->currentText().isEmpty() ){ ui->list_portbefore->clear(); }
  ui->list_portbefore->addItem(portSel.remove(settings->value("portsdir")+"/"));
  ui->push_config_save->setEnabled(TRUE);
}

void MainGUI::on_push_rmportbefore_clicked(){
  int index = ui->list_portbefore->currentIndex();
  if(index != -1){
    ui->list_portbefore->removeItem(index);
  }
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
  //Check if the port is valid
  if( !isValidPort(portSel) ){
    QMessageBox::warning(this,tr("EasyPBI: Invalid Port"), tr("The directory selected is not a valid FreeBSD port. Please select a port directory which contains the appropriate Makefile and distinfo files."));
    return;
  }
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
    //global settings for all modules
    currentModule->writeValue("progname",ui->line_progname->text());
    currentModule->writeValue("progweb",ui->line_progweb->text());
    currentModule->writeValue("progauthor",ui->line_progauthor->text());
    currentModule->writeValue("progicon",ui->list_progicon->currentText());
    if(ui->check_requiresroot->isChecked()){ currentModule->writeValue("requiresroot", "true"); }
    else{ currentModule->writeValue("requiresroot","false"); }
    //Module-dependant settings
    bool newport = FALSE;
    if( radio_module_port->isChecked() ){ //FreeBSD port module
	if(currentModule->readValue("makeport") != ui->line_makeport->text()){ newport=TRUE; }
	currentModule->writeValue("makeport",ui->line_makeport->text());
	currentModule->writeValue("makeoptions",ui->edit_makeopts->toPlainText());
	QString tmp;
	for(int i=0; i < ui->list_portbefore->count(); i++){
	  tmp.append(" "+ui->list_portbefore->itemText(i) );
	}
	currentModule->writeValue("makeportbefore",tmp.simplified());
	tmp.clear();
	for(int i=0; i < ui->list_portafter->count(); i++){
	  tmp.append(" "+ui->list_portafter->itemText(i) );
	}
	currentModule->writeValue("makeportafter",tmp.simplified());
	
    }else{ //local sources module
	currentModule->writeValue("packagedir",ui->line_progdir->text());
	currentModule->writeValue("progversion",ui->line_progversion->text());
    }
  //save the new settings to pbi.conf
  bool ok = currentModule->writePBIconf();
  if(!ok){
    //Display a warning that the file could not be saved
    qDebug() << "Error: pbi.conf could not be written:" << currentModule->path()+"/pbi.conf";
    QMessageBox::warning(this,tr("EasyPBI Error"), tr("The PBI configuration file could not be saved. Please check your file permissions before trying again."));
  }else if(newport){
    //Refresh the port information if the main port changed
    currentModule->readPortInformation(settings->value("portsdir")+"/"+ui->line_makeport->text());
    refreshGUI("pbiconf");
  }
}

void MainGUI::slotOptionChanged(QString tmp){
  tmp.clear(); //just to remove compiler warning about unused variable
  ui->push_config_save->setEnabled(TRUE);	
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
    QString path = currentModule->path() + "/resources/";
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
      QStringList contents = ModBuild::readFile(path+cfile);
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
    currentModule->addResource(FALSE,iFiles[i]);
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
  if(currentModule->removeResource(cfile)){
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
  //Now create the new file
  currentModule->addResource(TRUE,cFile);
  //Refresh the GUI
  refreshGUI("resources");
}

void MainGUI::slotResourceScriptSaved(){
  //get the current file and path
  QString cfile;
  if(ui->listw_resources->currentRow() != -1){ cfile = ui->listw_resources->currentItem()->text(); }
  if(cfile.isEmpty()){ return; } // do nothing if no file selected
  QString filePath = currentModule->path() + "/resources/" + cfile;
  //Read the current text for the resource
  QStringList contents = ui->text_resources_script->toPlainText().split("\n");
  //overwrite the resource with the new contents
  ModBuild::createFile(filePath,contents);
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
  //Update the file list appropriately
  if(ui->radio_xdg_desktop->isChecked()){
    //update the files available
    ui->list_xdg_files->clear();
    ui->list_xdg_files->addItems(currentModule->filesAvailable("xdg-desktop"));
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
    ui->list_xdg_files->addItems(currentModule->filesAvailable("xdg-menu"));	  
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
  //QStringList icons = currentModule->currentIcons;

  //Update the buttons that only need a refresh when the type changes (such as menu's)
  //Available binaries pushbuttons
  menu_bins.clear();
  QStringList cBins = currentModule->currentBins;
  if(!cBins.isEmpty()){
    for(int i=0; i<cBins.length(); i++){
      menu_bins.addAction(cBins[i]);
    } 
  }
  //Menu categories
  QString recMenu = currentModule->readValue("portmenucategory");
  QStringList cats = currentModule->validMenuCategories;
  menu_validMenuCats.clear();
  for(int i=0; i<cats.length(); i++){
    if(recMenu==cats[i]){ //Add an icon showing the recommended menu category
      menu_validMenuCats.addAction(Backend::icon("start"),cats[i]);	
    }else{
      menu_validMenuCats.addAction(cats[i]);
    }
  }
  //Now update read the currently selected file
  slotXdgFileChanged();
}

void MainGUI::slotXdgFileChanged(){	
  bool clearUI =FALSE;
  if(ui->list_xdg_files->currentRow() == -1){ clearUI=TRUE; }

  //Reset the icon list
  ui->list_xdg_icon->clear();
  QStringList icons = currentModule->currentIcons;
  if(icons.length() > 0){
      for(int i=0; i<icons.length(); i++){
        ui->list_xdg_icon->addItem(QIcon(currentModule->path()+"/resources/"+icons[i]),icons[i]);
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
    //Make sure we don't have any of the structures loaded from previously
    currentModule->loadDesktop("");
    currentModule->loadMenu("");
    currentModule->loadMime("");
    //Now return
    return; 
  }
  //Now setup the UI as appropriate
  if(ui->radio_xdg_desktop->isChecked()){
    //Load the file
    currentModule->loadDesktop(file);
    //update the file info on the GUI
    ui->line_xdg_name->setText( currentModule->readValue("desktopname") );
    ui->line_xdg_exec->setText( currentModule->readValue("desktopexec") ); 
    //comboboxes 
    int cicon = icons.indexOf(currentModule->readValue("desktopicon"));
    if( cicon == -1 ){ 
      ui->list_xdg_icon->addItem(currentModule->readValue("desktopicon")); 
      cicon = icons.indexOf(currentModule->readValue("desktopicon"));
    }
    ui->list_xdg_icon->setCurrentIndex(cicon);

    //Checkboxes
    QString chk = currentModule->readValue("desktopterminal").toLower();
    if( (chk == "true") || (chk=="yes") ){ ui->check_xdg_terminal->setChecked(TRUE); }
    else{ ui->check_xdg_terminal->setChecked(FALSE); }
    chk = currentModule->readValue("desktopnodisplay").toLower();
    if( (chk == "true") || (chk=="yes") ){ ui->check_xdg_nodisplay->setChecked(TRUE); }
    else{ ui->check_xdg_nodisplay->setChecked(FALSE); }
    chk = currentModule->readValue("desktoprequiresroot").toLower();
    if( (chk == "true") || (chk=="yes") ){ ui->check_xdg_requiresroot->setChecked(TRUE); }
    else{ ui->check_xdg_requiresroot->setChecked(FALSE); }
    	
  }else if(ui->radio_xdg_menu->isChecked()){
    //Load the file
    currentModule->loadMenu(file);
    //update the file info on the GUI
    ui->line_xdg_name->setText( currentModule->readValue("menuname") );
    ui->line_xdg_exec->setText( currentModule->readValue("menuexec") ); 
    ui->line_xdg_menu->setText( currentModule->readValue("menucategories") );
    //comboboxes
	  //icons
    int cicon = icons.indexOf(currentModule->readValue("menuicon"));
    if( cicon == -1 ){ 
      ui->list_xdg_icon->addItem(currentModule->readValue("menuicon")); 
      cicon = icons.indexOf(currentModule->readValue("menuicon"));
    }
    ui->list_xdg_icon->setCurrentIndex(cicon);
    
    //Checkboxes
    QString chk = currentModule->readValue("menuterminal").toLower();
    if( (chk == "true") || (chk=="yes") ){ ui->check_xdg_terminal->setChecked(TRUE); }
    else{ ui->check_xdg_terminal->setChecked(FALSE); }
    chk = currentModule->readValue("menunodisplay").toLower();
    if( (chk == "true") || (chk=="yes") ){ ui->check_xdg_nodisplay->setChecked(TRUE); }
    else{ ui->check_xdg_nodisplay->setChecked(FALSE); }
    chk = currentModule->readValue("menurequiresroot").toLower();
    if( (chk == "true") || (chk=="yes") ){ ui->check_xdg_requiresroot->setChecked(TRUE); }
    else{ ui->check_xdg_requiresroot->setChecked(FALSE); }
    
    //Load the XDG-MIME file associations
    QString mime = currentModule->readValue("menumimetype");
    QStringList mimeavailable = currentModule->filesAvailable("xdg-mime");
    bool found = FALSE;
    for( int i=0; i<mimeavailable.length(); i++){
      currentModule->loadMime(mimeavailable[i]);
      if(currentModule->readValue("mimetype") == mime){
        found = TRUE;
	break;
      }
    }
    //qDebug() << "mimetype detected:" << mime;
    if(found){
      ui->line_xdg_mimepatterns->setText( currentModule->readValue("mimepatterns") ); 
    }else{
      currentModule->loadMime(""); //Make sure we don't have any mime file loaded
      ui->line_xdg_mimepatterns->clear();
    }
    
  }else{
    //do nothing, unknown radio button selected (or none)
  }
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
  if( ui->list_xdg_files->currentItem()->text().isEmpty() ){ return; }
  //Now remove the file
  bool ok = FALSE;
  if( ui->radio_xdg_desktop->isChecked() ){
    ok = currentModule->removeDesktop();
  }else if( ui->radio_xdg_menu->isChecked() ){
    ok = currentModule->removeMenu();
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
  if( ui->radio_xdg_desktop->isChecked() ){
    currentModule->writeValue("desktopname",ui->line_xdg_name->text());
    currentModule->writeValue("desktopgenericname",ui->line_xdg_name->text().toLower());
    currentModule->writeValue("desktopexec",ui->line_xdg_exec->text());
    currentModule->writeValue("desktopicon",ui->list_xdg_icon->currentText());
    QString checked = "false";
    if(ui->check_xdg_terminal->isChecked()){ checked="true"; }
    currentModule->writeValue("desktopterminal",checked);
    checked = "false";
    if(ui->check_xdg_nodisplay->isChecked()){ checked="true"; }
    currentModule->writeValue("desktopnodisplay",checked);
    checked = "false";
    if(ui->check_xdg_requiresroot->isChecked()){ checked="true"; }
    currentModule->writeValue("desktoprequiresroot", checked);
    //Now save the file
    ok = currentModule->writeDesktop();
    
  }else if( ui->radio_xdg_menu->isChecked() ){
    currentModule->writeValue("menuname",ui->line_xdg_name->text());
    currentModule->writeValue("menugenericname",ui->line_xdg_name->text().toLower());
    currentModule->writeValue("menuexec",ui->line_xdg_exec->text());
    currentModule->writeValue("menuicon",ui->list_xdg_icon->currentText());
    currentModule->writeValue("menucategories", ui->line_xdg_menu->text());
    QString checked = "false";
    if(ui->check_xdg_terminal->isChecked()){ checked="true"; }
    currentModule->writeValue("menuterminal",checked);
    checked = "false";
    if(ui->check_xdg_nodisplay->isChecked()){ checked="true"; }
    currentModule->writeValue("menunodisplay",checked);
    checked = "false";
    if(ui->check_xdg_requiresroot->isChecked()){ checked="true"; }
    currentModule->writeValue("menurequiresroot", checked);
    //Setup the mime type associations as appropriate
    checkMime();
    //Now save the file
    ok = currentModule->writeMenu();    
  }
  //Now inform the user if there was an error
  if(!ok){
    QMessageBox::warning(this, tr("EasyPBI Error"), tr("Could not save the changes to the XDG entry.")+"\n"+tr("Check the file permissions and try again") );
  }
  //Now refresh the GUI
  //refreshGUI("xdg"); //refresh the entire tab
  slotXdgFileChanged(); //only update the proper half of the tab
}

void MainGUI::on_push_xdg_savenew_clicked(){
  //Get the filename we want to save
  QString filename;
  //Now check that the filename does not already exist
  QStringList cFiles;
  if(ui->radio_xdg_desktop->isChecked() ){
    filename = ui->line_xdg_exec->text().section("/",-1);
    cFiles = currentModule->filesAvailable("xdg-desktop").replaceInStrings(".desktop","");
  }else if(ui->radio_xdg_menu->isChecked() ){
    filename = ui->line_xdg_exec->text().section("/",-1);
    cFiles = currentModule->filesAvailable("xdg-menu").replaceInStrings(".desktop","");
  }
  if(filename.isEmpty()){ return;}
  
  if(cFiles.indexOf(filename) != -1){
    int num = 1;
    while(cFiles.indexOf(filename+QString::number(num)) != -1){ num++; }
    filename.append( QString::number(num) );
  }
  //save the current UI settings to the module structure then save the file
  bool ok = FALSE;
  if( ui->radio_xdg_desktop->isChecked() ){
    currentModule->loadDesktop(filename+".desktop"); //get the new file ready
    currentModule->writeValue("desktopname",ui->line_xdg_name->text());
    currentModule->writeValue("desktopgenericname",ui->line_xdg_name->text().toLower());
    currentModule->writeValue("desktopexec",ui->line_xdg_exec->text());
    currentModule->writeValue("desktopicon",ui->list_xdg_icon->currentText());
    QString checked = "false";
    if(ui->check_xdg_terminal->isChecked()){ checked="true"; }
    currentModule->writeValue("desktopterminal",checked);
    checked = "false";
    if(ui->check_xdg_nodisplay->isChecked()){ checked="true"; }
    currentModule->writeValue("desktopnodisplay",checked);
    checked = "false";
    if(ui->check_xdg_requiresroot->isChecked()){ checked="true"; }
    currentModule->writeValue("desktoprequiresroot", checked);
    //Now save the file
    ok = currentModule->writeDesktop();
    
  }else if( ui->radio_xdg_menu->isChecked() ){
    currentModule->loadMenu(filename+".desktop"); //get the new file ready
    //Update the MIME associations
    checkMime();
    //Save the MENU values
    currentModule->writeValue("menuname",ui->line_xdg_name->text());
    currentModule->writeValue("menugenericname",ui->line_xdg_name->text().toLower());
    currentModule->writeValue("menuexec",ui->line_xdg_exec->text());
    currentModule->writeValue("menuicon",ui->list_xdg_icon->currentText());
    currentModule->writeValue("menucategories", ui->line_xdg_menu->text());
    QString checked = "false";
    if(ui->check_xdg_terminal->isChecked()){ checked="true"; }
    currentModule->writeValue("menuterminal",checked);
    checked = "false";
    if(ui->check_xdg_nodisplay->isChecked()){ checked="true"; }
    currentModule->writeValue("menunodisplay",checked);
    checked = "false";
    if(ui->check_xdg_requiresroot->isChecked()){ checked="true"; }
    currentModule->writeValue("menurequiresroot", checked);
    //Now save the file
    ok = currentModule->writeMenu();    
  }
  //Now inform the user if there was an error
  if(!ok){
    QMessageBox::warning(this, tr("EasyPBI Error"), tr("Could not save the changes to the XDG entry.")+"\n"+tr("Check the file permissions and try again") );
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
	
    QString menumimetype = currentModule->readValue("menumimetype");
    QString menumimepat = ui->line_xdg_mimepatterns->text();
    QString cmimetype = currentModule->readValue("mimetype");
    QString cmimepat = currentModule->readValue("mimepatterns");
    if( menumimepat != cmimepat ){ //something different about the mime patterns
	//  ---- Change the MIME file as well ----
	  //Delete MIME file
	if(menumimepat.isEmpty()){
	  currentModule->removeMime();
	  currentModule->writeValue("menumimetype",""); //set MENU value
		
	  //Create new MIME file
	}else if( cmimetype.isEmpty() || (ui->line_xdg_exec->text() != currentModule->readValue("menuexec")) ){
	  QString exec = ui->line_xdg_exec->text().simplified();
	  menumimetype = "application/x-"+exec;
	  QString newfile = exec+"-xdg.xml";
	  currentModule->loadMime(newfile); //setup structure
	  currentModule->writeValue("mimetype",menumimetype); //set mime type
	  currentModule->writeValue("mimepatterns",menumimepat); //set mime patterns
	  currentModule->writeMime(); //save the MIME file (using default mime info)
		
	  currentModule->writeValue("menumimetype",menumimetype); //set MENU value
		
	  //Overwrite existing MIME file
	}else{
	  currentModule->writeValue("mimepatterns", menumimepat); //overwrite mime patterns
	  currentModule->writeMime(); //overwrite the MIME file
		
	  currentModule->writeValue("menumimetype",cmimetype); //set MENU value
	}
    }
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
  index=0; //just to remove the warning about unused variables
  refreshGUI("scripts");
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
  QDir dir(currentModule->path()+"/scripts");
  dir.remove(filename);
  //refresh the UI
  refreshGUI("scripts");
}

void MainGUI::on_push_scripts_save_clicked(){
  //Get the current file text
  QStringList contents = ui->text_scripts_edit->toPlainText().split("\n");
  //get the current file
  QString filename = currentModule->path() + "/scripts/"+ui->list_scripts_file->currentText();
  //Save the file
  bool ok = ModBuild::createFile(filename,contents);
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
  //Add the link
  currentModule->addExternalLink(file, linkto, opts.split(",") );
  //Save the file
  currentModule->writeExternalLinks();
  //Refresh the UI
  refreshGUI("external-links");
}

void MainGUI::on_push_el_remove_clicked(){
  //Get the currently selected link
  QTreeWidgetItem *line = ui->tree_el_view->currentItem();
  if(line->columnCount() != 3){ return; }
  QString file = line->text(0);
  QString linkto = line->text(1);
  //Remove the link
  currentModule->removeExternalLink(file,linkto);
  //Save the file
  currentModule->writeExternalLinks();
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
   FREENAS PLUGINS EDITOR OPTIONS
  -------------------------------------------------
*/


/*------------------------------------------------
   PBI BUILDER OPTIONS
  -------------------------------------------------
*/
void MainGUI::on_push_build_start_clicked(){
  //Check GUI to make sure settings are set before running
  bool gostatus = TRUE;
  QString outdir = settings->value("pbidir");
  if (outdir.isEmpty() ){gostatus=FALSE;}
  //QString sigfile = "";ui->linePBIDigSigFile->text(); //this one can be empty
  QString modDir = currentModule->path();
  if(modDir.isEmpty()){gostatus=FALSE;}
  if(!gostatus){
    QMessageBox::warning(this,tr("Error"),tr("Invalid PBI Settings"));
    return;
  }
  
  //Generate the PBI build command
  QString cmd;
  // -- PBI from ports
  if(radio_module_port->isChecked()){
    //Check that the ports tree is available
    if( !settings->check("isportsavailable") ){ 
      qDebug() << "Cannot build a PBI from ports without the FreeBSD ports tree available";
      return;
    }
    //Setup the pbi_makeport command from GUI settings
    cmd = settings->value("pbi_makeport");
    if(ui->check_build_32->isChecked()){ cmd += " -32"; }
    cmd += " -c " + modDir;
    if(settings->value("portsdir") != "/usr/ports"){ cmd += " -d " + settings->value("portsdir"); }
    cmd += " -o " + outdir;
    cmd += " --delbuild";
    if( settings->check("usetmpfs") ){ cmd += " --tmpfs"; }
    if( settings->check("usecache") ){ 
      cmd += " --pkgdir " + settings->value("cachedir");
      //Remove known "bad" packages (ones that routinely cause failures)
      QDir cDir( settings->value("cachedir") );
      QStringList filters = settings->list("skippkgs");
      QStringList badFiles = cDir.entryList(filters, QDir::Files | QDir:: NoDotAndDotDot);
      for(int i=0; i<badFiles.length(); i++){ cDir.remove(badFiles[i]); }
    }
    if( settings->check("usesignature") && QFile::exists(settings->value("sigfile")) ){ cmd += " --sign " + settings->value("sigfile"); }
    
  // -- PBI from local directory
  }else if(radio_module_local->isChecked() ){
	//get the base command
	cmd = settings->value("pbi_create");
	//Setup the ports options if non-standard port location
	if(settings->value("portsdir") != "/usr/ports"){ cmd += " -d " + settings->value("portsdir"); }
	//Setup the output directory
	cmd += " -o "+outdir;
	//Load the module
	cmd += " -c "+modDir;
	//Sign the PBI 
	if( settings->check("usesignature") && QFile::exists(settings->value("sigfile")) ){ cmd += " --sign " + settings->value("sigfile"); }
	//Now setup the directory to package
	cmd += " "+ currentModule->readValue("packagedir");
  }
  //Display the command created in hte terminal
  qDebug() << "Build PBI command created:"<<cmd;
  
  //Receive User verification before beginning build process due to:
  //  -- long time required, internet connection required, root permissions required
  QMessageBox verify(this);
  verify.setText(tr("Are you sure you wish to start the PBI build?"));
  verify.setInformativeText(tr("This requires an active internet connection and root permissions. The time required to build a PBI varies depending upon system specifications and ports to be compiled."));
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
  ui->check_build_32->setEnabled(FALSE);
  ui->push_build_start->setEnabled(FALSE); //disable the button so they do not start more than 1 build at a time
  ui->text_build_log->clear(); //clear the display in case this is not the first run
  ui->line_build_module->setText(currentModule->path());
  ui->line_build_outputdir->setText(settings->value("pbidir"));
  //Setup Process connections
  p = new QProcess(this);
  p->setProcessChannelMode(QProcess::MergedChannels);
  p->setProcessEnvironment( QProcessEnvironment::systemEnvironment() );
  connect(p,SIGNAL(readyReadStandardOutput()),this,SLOT(slotUpdatePBIBuild()) );
  connect(p,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(slotPBIbuildFinished(int,QProcess::ExitStatus)) );
  connect(p,SIGNAL(error(QProcess::ProcessError)),this,SLOT(slotBuildError(QProcess::ProcessError)) );
  //Setup the flag for the finishing checks
  PBI_BUILDING_NOW=outdir+":::"+currentModule->readValue("progname");
  //Start the Process
  qDebug() << "Starting the PBI build process...";
  ui->toolBox->setItemText(0,tr("PBI Builder (Working)"));
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
  if(!tmp.isEmpty()){ ui->text_build_log->append( tmp ); }
  //qDebug() << "Update output: " << tmp;
}

void MainGUI::slotPBIbuildFinished(int exitCode,QProcess::ExitStatus exitStatus){
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
  ui->check_build_32->setEnabled(TRUE);
  p->close();
  PBI_BUILDING_NOW.clear();
  PBI_BUILD_TERMINATED=FALSE;
  ui->toolBox->setItemText(0,tr("PBI Builder (Finished)"));
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
