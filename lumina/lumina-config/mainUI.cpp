//===========================================
//  Lumina-DE source code
//  Copyright (c) 2014, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#include "mainUI.h"
#include "ui_mainUI.h" //the designer *.ui file

MainUI::MainUI() : QMainWindow(), ui(new Ui::MainUI()){
  ui->setupUi(this); //load the designer file
  this->setWindowIcon( LXDG::findIcon("preferences-desktop-display","") );
  PINFO = new LPlugins(); //load the info class
	
  //Be careful about the QSettings setup, it must match the lumina-desktop setup
  QSettings::setPath(QSettings::NativeFormat, QSettings::UserScope, QDir::homePath()+"/.lumina");
  settings = new QSettings( QSettings::UserScope, "LuminaDE", "desktopsettings", this);
  qDebug() << "Settings File:" << settings->fileName();
  desktop = new QDesktopWidget();
  ui->spin_screen->setMinimum(1);
  if(desktop->screenCount() == 1){
    ui->spin_screen->setValue(1);
    //Hide these since no other screens
    ui->group_screen->setVisible(false);
  }else{
    //Make sure this is only allows the current number of screens
    ui->spin_screen->setMaximum(desktop->screenCount());
  }
  sysApps = LXDG::sortDesktopNames( LXDG::systemDesktopFiles() );
  
  //Now finish setting up the UI
  setupIcons();
  setupMenus();
  setupConnections();

  //Start on the Desktop page
  ui->stackedWidget->setCurrentWidget(ui->page_desktop);
  slotChangePage(false);
  QTimer::singleShot(10, this, SLOT(loadCurrentSettings()) );

  //Disable the incomplete pages at the moment
  ui->actionPanels->setEnabled(false);
  ui->actionMenu->setEnabled(false);
  ui->actionShortcuts->setEnabled(false);
  ui->actionDefaults->setEnabled(false);
  ui->actionSession->setEnabled(false);  
}

MainUI::~MainUI(){
	
}

void MainUI::slotSingleInstance(){
  //Make sure this window is visible
  this->showNormal();
  this->activateWindow();
  this->raise();
}

//================
//  PRIVATE FUNCTIONS
//================
void MainUI::setupIcons(){
  //Pull all the icons from the current theme using libLumina (LXDG)
	
  //General UI
  ui->actionDesktop->setIcon( LXDG::findIcon("preferences-desktop-wallpaper","") );
  ui->actionPanels->setIcon( LXDG::findIcon("configure-toolbars","") );
  ui->actionMenu->setIcon( LXDG::findIcon("preferences-desktop-icons","") );
  ui->actionShortcuts->setIcon( LXDG::findIcon("configure-shortcuts","") );
  ui->actionDefaults->setIcon( LXDG::findIcon("preferences-desktop-filetype-association","") );
  ui->actionSession->setIcon( LXDG::findIcon("preferences-system-session-services","") );
  ui->push_save->setIcon( LXDG::findIcon("document-save","") );  
  
	
  //Desktop Page
  ui->tool_desk_addbg->setIcon( LXDG::findIcon("list-add","") );
  ui->tool_desk_rmbg->setIcon( LXDG::findIcon("list-remove","") );
  ui->tool_desk_addplug->setIcon( LXDG::findIcon("list-add","") );
	
  //Panels Page
  ui->tool_panel1_add->setIcon( LXDG::findIcon("list-add","") );
  ui->tool_panel1_rm->setIcon( LXDG::findIcon("list-remove","") );
  ui->tool_panel1_addplugin->setIcon( LXDG::findIcon("list-add","") );
  ui->tool_panel1_rmplugin->setIcon( LXDG::findIcon("list-remove","") );
  ui->tool_panel1_upplug->setIcon( LXDG::findIcon("go-up","") );
  ui->tool_panel1_dnplug->setIcon( LXDG::findIcon("go-down","") );
  ui->tool_panel1_getcolor->setIcon( LXDG::findIcon("preferences-desktop-color","") );
  ui->toolBox_panel1->setItemIcon(0,LXDG::findIcon("preferences-desktop-display",""));
  ui->toolBox_panel1->setItemIcon(1,LXDG::findIcon("preferences-plugin",""));
  ui->tool_panel2_add->setIcon( LXDG::findIcon("list-add","") );
  ui->tool_panel2_rm->setIcon( LXDG::findIcon("list-remove","") );
  ui->tool_panel2_addplugin->setIcon( LXDG::findIcon("list-add","") );
  ui->tool_panel2_rmplugin->setIcon( LXDG::findIcon("list-remove","") );
  ui->tool_panel2_upplug->setIcon( LXDG::findIcon("go-up","") );
  ui->tool_panel2_dnplug->setIcon( LXDG::findIcon("go-down","") );
  ui->tool_panel2_getcolor->setIcon( LXDG::findIcon("preferences-desktop-color","") );
  ui->toolBox_panel2->setItemIcon(0,LXDG::findIcon("preferences-desktop-display",""));
  ui->toolBox_panel2->setItemIcon(1,LXDG::findIcon("preferences-plugin",""));

}

void MainUI::setupConnections(){
  //General UI
  connect(ui->actionDesktop, SIGNAL(triggered(bool)), this, SLOT( slotChangePage(bool)) );
  connect(ui->actionPanels, SIGNAL(triggered(bool)), this, SLOT( slotChangePage(bool)) );
  connect(ui->actionMenu, SIGNAL(triggered(bool)), this, SLOT( slotChangePage(bool)) );
  connect(ui->actionShortcuts, SIGNAL(triggered(bool)), this, SLOT( slotChangePage(bool)) );
  connect(ui->actionDefaults, SIGNAL(triggered(bool)), this, SLOT( slotChangePage(bool)) );
  connect(ui->actionSession, SIGNAL(triggered(bool)), this, SLOT( slotChangePage(bool)) );
  connect(ui->push_save, SIGNAL(clicked()), this, SLOT(saveCurrentSettings()) );
  connect(ui->spin_screen, SIGNAL(valueChanged(int)), this, SLOT(slotChangeScreen()) );
	
  //Desktop Page
  connect(ui->combo_desk_plugs, SIGNAL(currentIndexChanged(int)), this, SLOT(deskplugchanged()) );
  connect(ui->combo_desk_bg, SIGNAL(currentIndexChanged(int)), this, SLOT(deskbgchanged()) );
  connect(ui->radio_desk_multi, SIGNAL(toggled(bool)), this, SLOT(desktimechanged()) );
  connect(ui->tool_desk_addplug, SIGNAL(clicked()), this, SLOT(deskplugadded()) );
  connect(ui->tool_desk_addbg, SIGNAL(clicked()), this, SLOT(deskbgadded()) );
  connect(ui->tool_desk_rmbg, SIGNAL(clicked()), this, SLOT(deskbgremoved()) );	
	
  //Panels Page
  connect(ui->tool_panel1_add,SIGNAL(clicked()), this, SLOT(addpanel1()) );
  connect(ui->tool_panel2_add,SIGNAL(clicked()), this, SLOT(addpanel2()) );
  connect(ui->tool_panel1_rm,SIGNAL(clicked()), this, SLOT(rmpanel1()) );
  connect(ui->tool_panel2_rm,SIGNAL(clicked()), this, SLOT(rmpanel2()) );
  connect(ui->tool_panel1_getcolor,SIGNAL(clicked()), this, SLOT(getpanel1color()) );
  connect(ui->tool_panel2_getcolor,SIGNAL(clicked()), this, SLOT(getpanel2color()) );
  
}

void MainUI::setupMenus(){
  //Desktop Plugin Menu
  ui->combo_desk_plugs->clear();
  QStringList plugs = PINFO->desktopPlugins();
  for(int i=0; i<plugs.length(); i++){ 
    LPI info = PINFO->desktopPluginInfo(plugs[i]);
    ui->combo_desk_plugs->addItem( LXDG::findIcon(info.icon,""), info.name, plugs[i]);
  }
  ui->tool_desk_addplug->setEnabled(!plugs.isEmpty());
	
	
  /*//Panel Plugin Menu
  ppmenu->clear();
  QStringList plugs = PINFO->panelPlugins();
  plugs.sort();
  for(int i=0; i<plugs.length(); i++){
    LPI info = PINFO->panelPluginInfo(plugs[i]);
    QAction *act = new QAction(  LXDG::findIcon(info.icon,""), info.name, this);
	  act->setWhatsThis(info.ID);
	  act->setToolTip(info.description);
    ppmenu->addAction(act);
  }
  //Menu Plugin Menu
  mpmenu->clear();
  plugs = PINFO->menuPlugins();
  plugs.sort();
  for(int i=0; i<plugs.length(); i++){
    LPI info = PINFO->menuPluginInfo(plugs[i]);
    QAction *act = new QAction(  LXDG::findIcon(info.icon,""), info.name, this);
	  act->setWhatsThis(info.ID);
	  act->setToolTip(info.description);
    mpmenu->addAction(act);
  }
  */
}

void MainUI::checkForChanges(){
  //to see whether to enable the save button
  // -- TO DO (this should make the save button enabled only if changes are available)
	
}

int MainUI::currentDesktop(){
  return ui->spin_screen->value()-1; //backend starts at 0, not 1
}

QString MainUI::getColorStyle(QString current){
  QString out;
  //Convert the current color string into a QColor
  QStringList col = current.section(")",0,0).section("(",1,1).split(",");
  if(col.length()!=4){ col.clear(); col << "255" << "255" << "255" << "255"; }
  QColor ccol = QColor(col[0].toInt(), col[1].toInt(), col[2].toInt(), col[3].toInt()); //RGBA
  QColor ncol = QColorDialog::getColor(ccol, this, tr("Select Panel Color"), QColorDialog::ShowAlphaChannel);
  //Now convert the new color into a usable string and return
  if(ncol.isValid()){ //if the dialog was not cancelled
    out = "rgba("+QString::number(ncol.red())+","+QString::number(ncol.green())+","+QString::number(ncol.blue())+","+QString::number(ncol.alpha())+")";
  }
  return out;
}

QString MainUI::getNewPanelPlugin(){
  QString out;
  //Now let the user select a new panel plugin
	
  return out;
}

//================
//    PRIVATE SLOTS
//================
void MainUI::slotChangePage(bool enabled){
  //Do not allow the user to de-select a button (make them act like radio buttons)
  //qDebug() << "Page Change:" << enabled;
  bool showScreen = false; //set this for pages that have per-screen settings
  if(!enabled){
    //Re-enable the current button
    ui->actionDesktop->setChecked(ui->stackedWidget->currentWidget()==ui->page_desktop);
    ui->actionPanels->setChecked(ui->stackedWidget->currentWidget()==ui->page_panels);
    ui->actionMenu->setChecked(ui->stackedWidget->currentWidget()==ui->page_menu);
    ui->actionShortcuts->setChecked(ui->stackedWidget->currentWidget()==ui->page_shortcuts);
    ui->actionDefaults->setChecked(ui->stackedWidget->currentWidget()==ui->page_defaults);
    ui->actionSession->setChecked(ui->stackedWidget->currentWidget()==ui->page_session);
    showScreen = (ui->actionDesktop->isChecked() || ui->actionPanels->isChecked());
    //Ask if they want to reset any changes on the current page
	  
  }else{
    //Check if there are changes on the current page first
    
    //uncheck the button associated with the currently open page
    if(ui->stackedWidget->currentWidget()==ui->page_desktop){ ui->actionDesktop->setChecked(false); }
    if(ui->stackedWidget->currentWidget()==ui->page_panels){ ui->actionPanels->setChecked(false); }
    if(ui->stackedWidget->currentWidget()==ui->page_menu){ ui->actionMenu->setChecked(false); }
    if(ui->stackedWidget->currentWidget()==ui->page_shortcuts){ ui->actionShortcuts->setChecked(false); }
    if(ui->stackedWidget->currentWidget()==ui->page_defaults){ ui->actionDefaults->setChecked(false); }
    if(ui->stackedWidget->currentWidget()==ui->page_session){ ui->actionSession->setChecked(false); }
    //switch to the new page
    if(ui->actionDesktop->isChecked()){ ui->stackedWidget->setCurrentWidget(ui->page_desktop); showScreen=true;}
    else if(ui->actionPanels->isChecked()){ ui->stackedWidget->setCurrentWidget(ui->page_panels); showScreen=true; }
    else if(ui->actionMenu->isChecked()){ ui->stackedWidget->setCurrentWidget(ui->page_menu); }
    else if(ui->actionShortcuts->isChecked()){ ui->stackedWidget->setCurrentWidget(ui->page_shortcuts); }
    else if(ui->actionDefaults->isChecked()){ ui->stackedWidget->setCurrentWidget(ui->page_defaults); }
    else if(ui->actionSession->isChecked()){ ui->stackedWidget->setCurrentWidget(ui->page_session); }
  }
  ui->group_screen->setVisible(showScreen && (ui->spin_screen->maximum()>1) );
  
}

void MainUI::slotChangeScreen(){
  static int cscreen = 0; //current screen
  int newscreen = currentDesktop();
  if(cscreen!=newscreen){
    if(ui->push_save->isEnabled()){
      if(QMessageBox::Yes == QMessageBox::question(this, tr("Save Changes?"), tr("You currently have unsaved changes. Do you want to save them first?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) ){
        saveCurrentSettings();
      }
    }
    loadCurrentSettings();
    cscreen = newscreen; //save that this screen is current now
  }
}

void MainUI::saveAndQuit(){
  saveCurrentSettings();
  this->close();	
}

//General Utility Functions
void MainUI::loadCurrentSettings(){
  loading = true;
  settings->sync();
  int cdesk = currentDesktop();
  QString DPrefix = "desktop-"+QString::number(cdesk)+"/";
  //QString PPrefix = "panel"+QString::number(currentDesktop())+"."+QString::number(currentPanel())+"/";
	
  //Desktop Page
  QStringList bgs = settings->value(DPrefix+"background/filelist", QStringList()<<"default").toStringList();
  ui->combo_desk_bg->clear();
  for(int i=0; i<bgs.length(); i++){
    if(bgs[i]=="default"){ ui->combo_desk_bg->addItem( QIcon(DEFAULTBG), tr("System Default"), bgs[i] ); }
    else{ ui->combo_desk_bg->addItem( QIcon(bgs[i]), bgs[i].section("/",-1), bgs[i] ); }
  }
  if(bgs.length()>1){ ui->radio_desk_multi->setChecked(true); }
  else{ ui->radio_desk_single->setChecked(true); }
  ui->spin_desk_min->setValue( settings->value(DPrefix+"background/minutesToChange", 5).toInt() );
  desktimechanged(); //ensure the display gets updated (in case the radio selection did not change);
  ui->label_desk_res->setText( tr("Best Resolution:")+"\n"+QString::number(desktop->screenGeometry(cdesk).width())+"x"+QString::number(desktop->screenGeometry(cdesk).height()) );
  
  //Panels Page
  
  
  //Now disable the save button since nothing has changed yet
  ui->push_save->setEnabled(false);
  loading = false;
}

void MainUI::saveCurrentSettings(){
  QString DPrefix = "desktop-"+QString::number(currentDesktop())+"/";
  //QString PPrefix = "panel"+QString::number(currentDesktop())+"."+QString::number(currentPanel())+"/";


    // Desktop Page
    QStringList bgs; //get the list of backgrounds to use
    if(ui->radio_desk_multi->isChecked()){
      for(int i=0; i<ui->combo_desk_bg->count(); i++){
	bgs << ui->combo_desk_bg->itemData(i).toString();
      }
    }else if(ui->combo_desk_bg->count() > 0){
	bgs << ui->combo_desk_bg->itemData( ui->combo_desk_bg->currentIndex() ).toString();
	bgs.removeAll("default");
    }
    settings->setValue(DPrefix+"background/filelist", bgs);
    settings->setValue(DPrefix+"background/minutesToChange", ui->spin_desk_min->value());


    //All done - make sure the changes get saved to file right now
    settings->sync();
    ui->push_save->setEnabled(false); //wait for new changes
}


//===============
//    DESKTOP PAGE
//===============
void MainUI::deskplugchanged(){
  //NOTE: This is not a major change and will not enable the save button
  if(ui->combo_desk_plugs->count()==0){
    //No plugins available
    ui->label_desk_pluginfo->setText("");
    return;
  }
  //Load the new plugin summary
  QString plug = ui->combo_desk_plugs->itemData( ui->combo_desk_plugs->currentIndex() ).toString();
  LPI info = PINFO->desktopPluginInfo(plug);
  ui->label_desk_pluginfo->setText( info.description );
}

void MainUI::deskbgchanged(){
  //Load the new image preview
  if(ui->combo_desk_bg->count()==0){
    ui->label_desk_bgview->setPixmap(QPixmap());
    ui->label_desk_bgview->setText(tr("No Background")+"\n"+tr("(use system default)"));
  }else{
    QString path = ui->combo_desk_bg->itemData( ui->combo_desk_bg->currentIndex() ).toString();
    if(path=="default"){ path = DEFAULTBG; }
    if(QFile::exists(path)){
      QSize sz = ui->label_desk_bgview->size();
      sz.setWidth( sz.width() - (2*ui->label_desk_bgview->frameWidth()) );
      sz.setHeight( sz.height() - (2*ui->label_desk_bgview->frameWidth()) );
      ui->label_desk_bgview->setPixmap( QPixmap(path).scaled(sz, Qt::KeepAspectRatio, Qt::SmoothTransformation) );
    }else{
      ui->label_desk_bgview->setPixmap(QPixmap());
      ui->label_desk_bgview->setText(tr("File does not exist"));
    }
  }
  //See if this constitues a change to the current settings and enable the save button
  if(!loading && ui->radio_desk_single->isChecked()){ ui->push_save->setEnabled(true); }
  //Disable the background rotation option if only one background selected
  if(ui->combo_desk_bg->count()<2){
    ui->radio_desk_single->setChecked(true);
    ui->radio_desk_multi->setEnabled(false);
  }else{
    ui->radio_desk_multi->setEnabled(true);
  }
  //Disable the bg remove button if no backgrounds loaded
  ui->tool_desk_rmbg->setEnabled(ui->combo_desk_bg->count()>0);
}

void MainUI::desktimechanged(){
  ui->spin_desk_min->setEnabled(ui->radio_desk_multi->isChecked());
  if(!loading){ ui->push_save->setEnabled(true); }
}

void MainUI::deskbgremoved(){
  if(ui->combo_desk_bg->count()<1){ return; } //nothing to remove
  ui->combo_desk_bg->removeItem( ui->combo_desk_bg->currentIndex() );
}

void MainUI::deskbgadded(){
  //Prompt the user to find an image file to use for a background
  QString dir = "/usr/local/share/wallpapers/Lumina-DE";
  if( !QFile::exists(dir) ){ dir = QDir::homePath(); }
  QStringList bgs = QFileDialog::getOpenFileNames(this, tr("Find Background Image(s)"), dir, "Images (*.png *.xpm *.jpg)");
  if(bgs.isEmpty()){ return; }
  for(int i=0; i<bgs.length(); i++){
    ui->combo_desk_bg->addItem( QIcon(bgs[i]), bgs[i].section("/",-1), bgs[i]);
  }
  //Now move to the last item in the list (the new image(s));
  ui->combo_desk_bg->setCurrentIndex( ui->combo_desk_bg->count()-1 );
  ui->push_save->setEnabled(true); //this is definitely a change
}

void MainUI::deskplugadded(){
	
}


//=============
//  PANELS PAGE
//=============
void MainUI::addpanel1(){
  ui->toolBox_panel1->setVisible(true);
  checkpanels();
}

void MainUI::addpanel2(){
  ui->toolBox_panel2->setVisible(true);
  checkpanels();	
}

void MainUI::rmpanel1(){
  ui->toolBox_panel1->setVisible(false);
  checkpanels();	
}

void MainUI::rmpanel2(){
  ui->toolBox_panel2->setVisible(false);
  checkpanels();	
}

void MainUI::checkpanels(){
  //This checks the primary panel buttons/visibility
  ui->tool_panel1_add->setVisible(!ui->toolBox_panel1->isVisible());
  ui->tool_panel1_rm->setVisible(ui->toolBox_panel1->isVisible());
  if(ui->tool_panel1_add->isVisible()){
    //No panels at all yet - disable the 2nd panel options
    ui->tool_panel2_add->setVisible(false);
    ui->tool_panel2_rm->setVisible(false);
    ui->toolBox_panel2->setVisible(false);
    ui->label_panel2->setVisible(false);
    ui->gridLayout_panels->setColumnStretch(2,1);
    //ui->horizontalSpacer_panel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
  }else{
    //Panel 1 is visible - also show options for panel 2 appropriately
    ui->tool_panel2_add->setVisible(!ui->toolBox_panel2->isVisible());
    ui->tool_panel2_rm->setVisible(ui->toolBox_panel2->isVisible());
    ui->label_panel2->setVisible(true);
    ui->tool_panel1_rm->setVisible(!ui->toolBox_panel2->isVisible());
    ui->gridLayout_panels->setColumnStretch(2,0);
    //ui->horizontalSpacer_panel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  }
}

void MainUI::adjustpanel1(){
  //Adjust panel 1 to complement a panel 2 change
	
}

void MainUI::adjustpanel2(){
  //Adjust panel 2 to complement a panel 1 change
	
}

	
void MainUI::getpanel1color(){
  QString color = getColorStyle(ui->label_panel1_sample->whatsThis());
  if(color.isEmpty()){ return; } //nothing selected
  ui->label_panel1_sample->setStyleSheet("background: "+color);
  ui->label_panel1_sample->setWhatsThis(color);
}

void MainUI::getpanel2color(){
  QString color = getColorStyle(ui->label_panel2_sample->whatsThis());
  if(color.isEmpty()){ return; } //nothing selected
  ui->label_panel2_sample->setStyleSheet("background: "+color);
  ui->label_panel2_sample->setWhatsThis(color);	
}

void MainUI::addpanel1plugin(){
  QString pan = getNewPanelPlugin();
  if(pan.isEmpty()){ return; } //nothing selected
  
}

void MainUI::addpanel2plugin(){
	
}

void MainUI::rmpanel1plugin(){
	
}

void MainUI::rmpanel2plugin(){
	
}


/*
//ToolBar Tab Functions
void MainUI::loadPanelSettings(){
  //Get the current screen/panel number
  QString pprefix = "panel"+QString::number(currentDesktop())+"."+QString::number(currentPanel())+"/";
  bool defaultpanel = (pprefix=="panel0.0/");
  //Now read the values and set them appropriately
  // - location
  QString loc = settings->value(pprefix+"location","t").toString();
  if(loc.isEmpty() && defaultpanel){ loc="top"; }
  if(loc=="top"){ ui->combo_tb_location->setCurrentIndex(0); }
  else if(loc=="bottom"){ ui->combo_tb_location->setCurrentIndex(1); }
  // - background color
  panelcolor = settings->value(pprefix+"color", "qlineargradient(spread:pad, x1:0.291182, y1:0, x2:0.693, y2:1, stop:0 rgb(255, 253, 250), stop:1 rgb(210, 210, 210))").toString();
  // - height
  ui->spin_tb_height->setValue( settings->value(pprefix+"height",22).toInt() );
  // - plugins
  QStringList plugs = settings->value(pprefix+"pluginlist",QStringList()).toStringList();
  if(plugs.isEmpty() && defaultpanel){ plugs << "userbutton" << "desktopbar" << "desktopswitcher" << "taskmanager" << "spacer" << "systemtray" << "clock"; }
  ui->list_tb_plugins->clear();
  for(int i=0; i<plugs.length(); i++){
    LPI info = PINFO->panelPluginInfo(plugs[i]);
    if(info.ID.isEmpty()){ continue; } //invalid plugin
    QListWidgetItem *item = new QListWidgetItem();
      item->setWhatsThis( info.ID );
      item->setIcon( LXDG::findIcon(info.icon,"") );
      item->setText( info.name );
      item->setToolTip( info.description );
    ui->list_tb_plugins->addItem(item);
  }
  
  //Now update the color shown
  colorChanged();
}

void MainUI::savePanelSettings(){
  //Get the current screen/panel number
  QString pprefix = "panel"+QString::number(currentDesktop())+"."+QString::number(currentPanel())+"/";
  //qDebug() << "Save Panel Settings:" << pprefix;
  //Now read the values and set them appropriately
  // - location
  QString loc = ui->combo_tb_location->currentText().toLower();
  settings->setValue(pprefix+"location",loc);
  // - background color
  settings->setValue(pprefix+"color", panelcolor);
  // - height
  settings->setValue(pprefix+"height", ui->spin_tb_height->value());
  // - plugins
  QStringList plugs;
  for(int i=0; i<ui->list_tb_plugins->count(); i++){ plugs << ui->list_tb_plugins->item(i)->whatsThis(); }
  settings->setValue(pprefix+"pluginlist", plugs);
}

void MainUI::getNewColor(){
  //Convert the current color string into a QColor
  QStringList col = panelcolor.section(")",0,0).section("(",1,1).split(",");
  if(col.length()!=3){ col.clear(); col << "255" << "255" << "255"; }
  QColor ccol = QColor::fromRgb(col[0].toInt(), col[1].toInt(), col[2].toInt());
  QColor ncol = QColorDialog::getColor(ccol, this, tr("Select Panel Color"));
  //Now convert the new color into a usable string
  panelcolor = "rgb("+QString::number(ncol.red())+","+QString::number(ncol.green())+","+QString::number(ncol.blue())+")";
  //Now update the sample widget background
  colorChanged();
}

void MainUI::colorChanged(){
  //Now change the color of the label for an example
  ui->label_tb_colorsample->setStyleSheet("background: "+panelcolor+";");
}

void MainUI::addPPlugin(QAction *act){
  QListWidgetItem *item = new QListWidgetItem();
    item->setWhatsThis( act->whatsThis() );
    item->setIcon( act->icon() );
    item->setText( act->text() );
    item->setToolTip( act->toolTip() );
  ui->list_tb_plugins->addItem(item);
}

void MainUI::rmPPlugin(){
  if(ui->list_tb_plugins->currentRow()<0){ return; } //no selection
  delete ui->list_tb_plugins->takeItem( ui->list_tb_plugins->currentRow() );
}

void MainUI::mvLPPlugin(){
  int row = ui->list_tb_plugins->currentRow();
  if(row<=0){ return; } //no selection or already 0
  ui->list_tb_plugins->insertItem(row-1, ui->list_tb_plugins->takeItem(row));
  ui->list_tb_plugins->setCurrentRow(row-1);
}

void MainUI::mvRPPlugin(){
  int row = ui->list_tb_plugins->currentRow();
  if(row<0 || row==(ui->list_tb_plugins->count()-1) ){ return; } //no selection or already at end
  ui->list_tb_plugins->insertItem(row+1, ui->list_tb_plugins->takeItem(row));
  ui->list_tb_plugins->setCurrentRow(row+1);
}

//==================
//     Menu Tab Functions
//==================
void MainUI::loadMenuSettings(){
  // NOTE: These are screen-independent
  //Default terminal binary
  ui->line_menu_terminal->setText( settings->value("default-terminal","xterm").toString() );
  //Menu Items
  QStringList items = settings->value("menu/itemlist", QStringList() ).toStringList();
  if(items.isEmpty()){ items << "terminal" << "filemanager" << "applications" << "line" << "settings"; }
  //qDebug() << "Menu Items:" << items;
  ui->list_menu_items->clear();
  for(int i=0; i<items.length(); i++){
    LPI info = PINFO->menuPluginInfo(items[i]);
    if(items[i].startsWith("app::::")){
      bool ok = false;
      XDGDesktop desk = LXDG::loadDesktopFile(items[i].section("::::",1,1), ok);
      if(!ok){ continue; } //invalid application file (no longer installed?)
      QListWidgetItem *item = new QListWidgetItem();
        item->setWhatsThis( items[i] );
        item->setIcon( LXDG::findIcon(desk.icon) );
        item->setText( desk.name );
        item->setToolTip( desk.comment );
      ui->list_menu_items->addItem(item);
      continue; //now go to the next item
    }
    if(info.ID.isEmpty()){ continue; } //invalid plugin
    //qDebug() << "Add Menu Item:" << info.ID;
    QListWidgetItem *item = new QListWidgetItem();
      item->setWhatsThis( info.ID );
      item->setIcon( LXDG::findIcon(info.icon,"") );
      item->setText( info.name );
      item->setToolTip( info.description );
    ui->list_menu_items->addItem(item);
  }
}

void MainUI::saveMenuSettings(){
  //Default terminal binary
  settings->setValue( "default-terminal", ui->line_menu_terminal->text() );
  //Menu Items
  QStringList items;
  for(int i=0; i<ui->list_menu_items->count(); i++){
    items << ui->list_menu_items->item(i)->whatsThis();
  }
  settings->setValue("menu/itemlist", items);
}

void MainUI::findTerminalBinary(){
  QString chkpath = "/usr/local/bin";
  if(!QFile::exists(chkpath)){ chkpath = QDir::homePath(); }
  QString bin = QFileDialog::getOpenFileName(this, tr("Set Default Terminal Application"), chkpath, tr("Application Binaries (*)") );
  if( bin.isEmpty() || !QFile::exists(bin) ){ return; } //cancelled
  if( !QFileInfo(bin).isExecutable() ){ 
    QMessageBox::warning(this, tr("Invalid Binary"), tr("The selected file is not executable!"));
    return;
  }
  ui->line_menu_terminal->setText(bin);
}

void MainUI::addMenuItem(QAction* act){
  if(act->whatsThis()=="app"){
    //Need to prompt for the exact application to add to the menu
    // Note: whatsThis() format: "app::::< *.desktop file path >"
    QStringList apps;
    for(int i=0; i<sysApps.length(); i++){
      if(sysApps[i].comment.isEmpty()){ apps << sysApps[i].name; }
      else{ apps << sysApps[i].name + " ("+sysApps[i].comment+")"; }
    }
    QString app = QInputDialog::getItem(this, tr("Select Application"), tr("App Name:"), apps, false);
    int index = apps.indexOf(app);
    if(app.isEmpty() || index < 0){ return; } //nothing selected
    //Now add this item to the  list
    QListWidgetItem *item = new QListWidgetItem();
      item->setWhatsThis( act->whatsThis()+"::::"+sysApps[index].filePath );
      item->setIcon( LXDG::findIcon(sysApps[index].icon) );
      item->setText( sysApps[index].name );
      item->setToolTip( sysApps[index].comment );
    ui->list_menu_items->addItem(item);
  }else{
    QListWidgetItem *item = new QListWidgetItem();
      item->setWhatsThis( act->whatsThis() );
      item->setIcon( act->icon() );
      item->setText( act->text() );
      item->setToolTip( act->toolTip() );
    ui->list_menu_items->addItem(item);
  }
}

void MainUI::rmMenuItem(){
  if(ui->list_menu_items->currentRow()<0){ return; } //no selection
  delete ui->list_menu_items->takeItem( ui->list_menu_items->currentRow() );
}

void MainUI::upMenuItem(){
  int row = ui->list_menu_items->currentRow();
  if(row<=0){ return; } //no selection or already 0
  ui->list_menu_items->insertItem(row-1, ui->list_menu_items->takeItem(row));
  ui->list_menu_items->setCurrentRow(row-1);	
}

void MainUI::downMenuItem(){
  int row = ui->list_menu_items->currentRow();
  if(row<0 || row==(ui->list_menu_items->count()-1) ){ return; } //no selection or already at end
  ui->list_menu_items->insertItem(row+1, ui->list_menu_items->takeItem(row));
  ui->list_menu_items->setCurrentRow(row+1);	
}
*/
