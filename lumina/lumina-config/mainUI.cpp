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
  ppmenu = new QMenu(this); // panel plugin menu
    ui->tool_tb_addplugin->setMenu(ppmenu);
  //Be careful about the QSettings setup, it must match the lumina-desktop setup
  QSettings::setPath(QSettings::NativeFormat, QSettings::UserScope, QDir::homePath()+"/.lumina");
  settings = new QSettings( QSettings::UserScope, "LuminaDE", "desktopsettings", this);
  qDebug() << "Settings File:" << settings->fileName();
  desktop = new QDesktopWidget();
  if(desktop->screenCount() == 1){
    ui->spin_screen->setValue(1);
    //Hide these since no other screens
    ui->group_screen->setVisible(false);
  }else{
    //Make sure this is only allows the current number of screens
    ui->spin_screen->setMaximum(desktop->screenCount());
  }

  //Setup the buttons signal/slot connections
  connect(ui->spin_screen, SIGNAL(valueChanged(int)), this, SLOT(loadCurrentSettings()) );
  connect(ui->push_save, SIGNAL(clicked()), this, SLOT(saveCurrentSettings()) );
  
  // - background tab
  connect(ui->tool_rmbackground, SIGNAL(clicked()), this, SLOT(removeBackground()) );
  connect(ui->tool_addbackground, SIGNAL(clicked()), this, SLOT(addBackground()) );
  connect(ui->radio_rotateBG, SIGNAL(toggled(bool)), SLOT(enableBGRotateTime(bool)) );
  // - toolbar tab
  connect(ui->spin_tb_R, SIGNAL(valueChanged(int)), this, SLOT(colorChanged()) );
  connect(ui->spin_tb_G, SIGNAL(valueChanged(int)), this, SLOT(colorChanged()) );
  connect(ui->spin_tb_B, SIGNAL(valueChanged(int)), this, SLOT(colorChanged()) );
  connect(ppmenu, SIGNAL(triggered(QAction*)), this, SLOT(addPPlugin(QAction*)) );
  connect(ui->tool_tb_leftplugin, SIGNAL(clicked()), this, SLOT(mvLPPlugin()) );
  connect(ui->tool_tb_rightplugin, SIGNAL(clicked()), this, SLOT(mvRPPlugin()) );
  connect(ui->tool_tb_rmplugin, SIGNAL(clicked()), this, SLOT(rmPPlugin()) );
  //Now finish setting up the UI
  setupIcons();
  setupMenus();
  ui->spin_tb_number->setEnabled(false); //not finished yet - disable for now
  ui->tool_tb_addpanel->setEnabled(false); //not finished yet - disable for now
  ui->tool_tb_rmpanel->setEnabled(false); //not finished yet - disable for now
  QTimer::singleShot(10, this, SLOT(loadCurrentSettings()) );
  ui->tabWidget->setCurrentWidget(ui->tab_background);
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
  //  - Menu Items
  ui->actionSave_and_Quit->setIcon( LXDG::findIcon("document-save-all","") );
  ui->actionClose->setIcon( LXDG::findIcon("application-exit","") );
	
  //  - Background tab
  ui->tool_rmbackground->setIcon( LXDG::findIcon("list-remove","") );
  ui->tool_addbackground->setIcon( LXDG::findIcon("list-add","") );
  //  - Toolbar tab
  ui->tool_tb_addplugin->setIcon( LXDG::findIcon("list-add", "") );
  ui->tool_tb_rmplugin->setIcon( LXDG::findIcon("list-remove", "") );
  ui->tool_tb_rightplugin->setIcon( LXDG::findIcon("go-next-view", "") );
  ui->tool_tb_leftplugin->setIcon( LXDG::findIcon("go-previous-view", "") );
  ui->tool_tb_addpanel->setIcon( LXDG::findIcon("list-add", "") );
  ui->tool_tb_rmpanel->setIcon( LXDG::findIcon("list-remove", "") );
  //  - General UI buttons
  ui->push_save->setIcon( LXDG::findIcon("document-save","") );
}

void MainUI::setupMenus(){
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
  
}

void MainUI::checkForChanges(){
  //to see whether to enable the save button
  // -- TO DO (this should make the save button enabled only if changes are available)
	
}

int MainUI::currentDesktop(){
  return ui->spin_screen->value()-1; //backend starts at 0, not 1
}

int MainUI::currentPanel(){
  return ui->spin_tb_number->value()-1; // - TO DO	
}

void MainUI::addNewBackgroundFile(QString filepath){
  QListWidgetItem *item = new QListWidgetItem(ui->list_backgrounds);
	item->setText(filepath.section("/",-1));
	item->setToolTip(filepath);
	item->setWhatsThis(filepath); //save the full path in this variable
	item->setIcon( QIcon(filepath) );
}

//================
//    PRIVATE SLOTS
//================
//General Utility Functions
void MainUI::loadCurrentSettings(){
  settings->sync();
  QString DPrefix = "desktop-"+QString::number(currentDesktop())+"/";
  QString PPrefix = "panel"+QString::number(currentDesktop())+"."+QString::number(currentPanel())+"/";
  //The Background tab
    //Setup the bg preview image size (correct aspect ratio for the current screen)
      int icoWidth = ui->list_backgrounds->width()/3;
      double ratio = desktop->screenGeometry(currentDesktop()).height()/ ( (double) desktop->screenGeometry(currentDesktop()).width() );
      ui->list_backgrounds->setIconSize( QSize(icoWidth, icoWidth*ratio) );
    //Load the background files
    QStringList bgs = settings->value(DPrefix+"background/filelist", QStringList()<<"default").toStringList();
    //qDebug() << "Backgrounds:" << DPrefix << bgs;
    ui->list_backgrounds->clear();
    for(int i=0; i<bgs.length(); i++){
      if(bgs[i]=="default"){ bgs[i]=DEFAULTBG; }
      if(QFile::exists(bgs[i])){
        addNewBackgroundFile(bgs[i]);
      }
    }
    if(bgs.length() <= 1){ ui->radio_singleBG->setChecked(true); }
    else{ ui->radio_rotateBG->setChecked(true); }
    ui->spin_bgRotateMin->setValue( settings->value(DPrefix+"background/minutesToChange", 5).toInt() );
    
  //Now load the current panel settings
  loadPanelSettings();
}

void MainUI::saveCurrentSettings(){
  QString DPrefix = "desktop-"+QString::number(currentDesktop())+"/";
  QString PPrefix = "panel"+QString::number(currentDesktop())+"."+QString::number(currentPanel())+"/";
  //Now save the current settings from the UI
    // Background Tab
    QStringList bgs; //get the list of backgrounds to use
    if(ui->radio_rotateBG->isChecked()){
      for(int i=0; i<ui->list_backgrounds->count(); i++){
	bgs << ui->list_backgrounds->item(i)->whatsThis();
      }
    }else{
	QListWidgetItem *it = ui->list_backgrounds->currentItem();
	if(it != 0){ bgs << it->whatsThis(); }
	else if(ui->list_backgrounds->count() > 0){ bgs << ui->list_backgrounds->item(0)->whatsThis(); }
    }
    if(bgs.isEmpty()){ bgs << "default"; }
    settings->setValue(DPrefix+"background/filelist", bgs);
    settings->setValue(DPrefix+"background/minutesToChange", ui->spin_bgRotateMin->value());

    //Panels tab
    savePanelSettings();
    //All done - make sure the changes get saved to file right now
    settings->sync();
    
}

//Background Tab Functions
void MainUI::addBackground(){
  //Prompt the user to find an image file to use for a background
  QString dir = "/usr/local/share/wallpapers";
  if( !QFile::exists(dir) ){ dir = QDir::homePath(); }
  QStringList bgs = QFileDialog::getOpenFileNames(this, tr("Find Background Image(s)"), dir, "Images (*.png *.xpm *.jpg)");
  for(int i=0; i<bgs.length(); i++){
    addNewBackgroundFile(bgs[i]);
  }
  
}

void MainUI::removeBackground(){
  //Remove the currently selected background from the list
  int item = ui->list_backgrounds->currentRow();
  if(item >= 0){
    delete ui->list_backgrounds->takeItem(item);
  }
}

void MainUI::enableBGRotateTime(bool enable){
  ui->spin_bgRotateMin->setEnabled(enable);
}

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
  QString tbbg = settings->value(pprefix+"color", "rgb(255,250,250)").toString();
  tbbg = tbbg.section("(",1,1).section(")",0,0).simplified();
  ui->spin_tb_R->setValue(tbbg.section(",",0,0).toInt());
  ui->spin_tb_G->setValue(tbbg.section(",",1,1).toInt());
  ui->spin_tb_B->setValue(tbbg.section(",",2,2).toInt());
  // - height
  ui->spin_tb_height->setValue( settings->value(pprefix+"height",22).toInt() );
  // - plugins
  QStringList plugs = settings->value(pprefix+"pluginlist",QStringList()).toStringList();
  if(plugs.isEmpty() && defaultpanel){ plugs << "userbutton" << "desktopbar" << "desktopswitcher" << "spacer" << "clock"; }
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
  QString color = "rgb(%1,%2,%3)";
  color = color.arg(QString::number(ui->spin_tb_R->value()), QString::number(ui->spin_tb_G->value()), QString::number(ui->spin_tb_B->value()) );
  settings->setValue(pprefix+"color", color);
  // - height
  settings->setValue(pprefix+"height", ui->spin_tb_height->value());
  // - plugins
  QStringList plugs;
  for(int i=0; i<ui->list_tb_plugins->count(); i++){ plugs << ui->list_tb_plugins->item(i)->whatsThis(); }
  settings->setValue(pprefix+"pluginlist", plugs);
}

void MainUI::colorChanged(){
  //Get the RGB color
  QString color = "rgb(%1,%2,%3)";
  color = color.arg(QString::number(ui->spin_tb_R->value()), QString::number(ui->spin_tb_G->value()), QString::number(ui->spin_tb_B->value()) );
  //Now change the color of the label for example
  ui->label_tb_color->setStyleSheet("background: "+color+";");
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
