//===========================================
//  Lumina-DE source code
//  Copyright (c) 2014, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#include "MainUI.h"
#include "ui_MainUI.h"

MainUI::MainUI() : QMainWindow(), ui(new Ui::MainUI){
  ui->setupUi(this);
  //Be careful about the QSettings setup, it must match the lumina-desktop setup
  QSettings::setPath(QSettings::NativeFormat, QSettings::UserScope, QDir::homePath()+"/.lumina");
  settings = new QSettings( QSettings::UserScope, "LuminaDE", "lumina-fm", this);
  //initialize the non-ui widgets
  tabBar = new QTabBar(this);
    tabBar->setTabsClosable(true);
    tabBar->setMovable(true); //tabs are independant - so allow the user to sort them
    tabBar->setShape(QTabBar::RoundedNorth);
    tabBar->setFocusPolicy(Qt::NoFocus);
    ui->verticalLayout_browser->insertWidget(0,tabBar);
  currentDir = new QLineEdit(this);
    ui->toolBar->insertWidget(ui->actionBookMark, currentDir);
    currentDir->setFocusPolicy(Qt::StrongFocus);
  fsmod = new QFileSystemModel(this);
    fsmod->setRootPath("/");
    /*ui->tree_dir_view->setModel(fsmod);
    ui->tree_dir_view->setItemsExpandable(false);
    ui->tree_dir_view->setExpandsOnDoubleClick(false);
    ui->tree_dir_view->setRootIsDecorated(false);
    ui->tree_dir_view->setSortingEnabled(true);
    ui->tree_dir_view->sortByColumn(0,Qt::AscendingOrder);
    ui->tree_dir_view->setContextMenuPolicy(Qt::CustomContextMenu);*/
  dirCompleter = new QCompleter(fsmod, this);
    dirCompleter->setModelSorting( QCompleter::CaseInsensitivelySortedModel );
    currentDir->setCompleter(dirCompleter);
  dirWatcher = new QFileSystemWatcher(this);
    
  snapmod = new QFileSystemModel(this);
    ui->tree_zfs_dir->setModel(snapmod);
    ui->tree_zfs_dir->sortByColumn(0, Qt::AscendingOrder);
  contextMenu = new QMenu(this);
  radio_view_details = new QRadioButton(tr("Detailed List"), this);
  radio_view_list = new QRadioButton(tr("Basic List"), this);
  radio_view_icons = new QRadioButton(tr("Icons"), this);
  detWA = new QWidgetAction(this);
    detWA->setDefaultWidget(radio_view_details);
  listWA = new QWidgetAction(this);
    listWA->setDefaultWidget(radio_view_list);
  icoWA = new QWidgetAction(this);
    icoWA->setDefaultWidget(radio_view_icons);
    ui->menuView->addAction(detWA);
    ui->menuView->addAction(listWA);
    ui->menuView->addAction(icoWA);
  //Setup any specialty keyboard shortcuts
  nextTabLShort = new QShortcut( QKeySequence(tr("Shift+Left")), this);
  nextTabRShort = new QShortcut( QKeySequence(tr("Shift+Right")), this);
  closeTabShort = new QShortcut( QKeySequence(tr("Ctrl+W")), this);
  copyFilesShort = new QShortcut( QKeySequence(tr("Ctrl+C")), this);
  pasteFilesShort = new QShortcut( QKeySequence(tr("Ctrl+V")), this);
  deleteFilesShort = new QShortcut( QKeySequence(tr("Delete")), this);
  //Finish loading the interface
  setupIcons();
  setupConnections();
  loadSettings();
  RebuildBookmarksMenu();
  RebuildDeviceMenu();
  //Make sure we start on the browser page
  goToBrowserPage();
}

MainUI::~MainUI(){
	
}

void MainUI::OpenDirs(QStringList dirs){
  for(int i=0; i<dirs.length(); i++){
    //Add this directory in a new tab
    if(dirs[i].endsWith("/")){ dirs[i].chop(1); }
    qDebug() << "Open Directory:" << dirs[i];
    int index = tabBar->addTab( dirs[i].section("/",-1) );
    tabBar->setTabWhatsThis( index, dirs[i] );
    if(index==0){ setCurrentDir(dirs[i]); }//display this as the current dir
  }
  tabBar->setVisible( tabBar->count() > 1 );
}

//==========
//    PRIVATE
//==========
void MainUI::setupIcons(){
  this->setWindowIcon( LXDG::findIcon("system-file-manager","") );
	
  //Setup all the icons using libLumina
  ui->actionClose->setIcon( LXDG::findIcon("application-exit","") );
  ui->actionNew_Tab->setIcon( LXDG::findIcon("tab-new-background","") );
  //ui->action_Preferences->setIcon( LXDG::findIcon("configure","") );
  ui->actionUpDir->setIcon( LXDG::findIcon("go-up","") );
  ui->actionBack->setIcon( LXDG::findIcon("go-previous","") );
  ui->actionHome->setIcon( LXDG::findIcon("go-home","") );
  ui->actionBookMark->setIcon( LXDG::findIcon("bookmarks","") );
  ui->actionBackToBrowser->setIcon( LXDG::findIcon("go-previous","") );
  ui->actionManage_Bookmarks->setIcon( LXDG::findIcon("bookmarks-organize","") );
  ui->actionScan->setIcon( LXDG::findIcon("system-search","") );
	
  //Browser page
  ui->tool_addToDir->setIcon( LXDG::findIcon("folder-new","") );
  ui->tool_goToImages->setIcon( LXDG::findIcon("fileview-preview","") );
  ui->tool_goToPlayer->setIcon( LXDG::findIcon("applications-multimedia","") );
  ui->tool_goToRestore->setIcon( LXDG::findIcon("document-revert","") );
  //Multimedia Player page
	
  //Slideshow page
  ui->tool_image_goBegin->setIcon( LXDG::findIcon("go-first-view","") );
  ui->tool_image_goEnd->setIcon( LXDG::findIcon("go-last-view","") );
  ui->tool_image_goPrev->setIcon( LXDG::findIcon("go-previous-view","") );
  ui->tool_image_goNext->setIcon( LXDG::findIcon("go-next-view","") );
  
  //ZFS Restore page
  ui->tool_zfs_nextSnap->setIcon( LXDG::findIcon("go-next-view","") );
  ui->tool_zfs_prevSnap->setIcon( LXDG::findIcon("go-previous-view","") );
  ui->tool_zfs_restoreItem->setIcon( LXDG::findIcon("document-revert","") );
}

void MainUI::setupConnections(){
  connect(QApplication::instance(), SIGNAL(focusChanged(QWidget*, QWidget*)), this, SLOT(startEditDir(QWidget*, QWidget*)) );
  connect(tabBar, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)) );
  connect(tabBar, SIGNAL(tabCloseRequested(int)), this, SLOT(tabClosed(int)) );
  connect(ui->menuBookmarks, SIGNAL(triggered(QAction*)), this, SLOT(goToBookmark(QAction*)) );
  connect(ui->menuExternal_Devices, SIGNAL(triggered(QAction*)), this, SLOT(goToDevice(QAction*)) );
  connect(currentDir, SIGNAL(returnPressed()), this, SLOT(goToDirectory()));
  connect(radio_view_details, SIGNAL(toggled(bool)), this, SLOT(viewModeChanged(bool)) );
  connect(radio_view_list, SIGNAL(toggled(bool)), this, SLOT(viewModeChanged(bool)) );
  connect(radio_view_icons, SIGNAL(toggled(bool)), this, SLOT(viewModeChanged(bool)) );
	
  //Tree Widget interaction
  connect(ui->tree_dir_widget, SIGNAL(itemActivated(QTreeWidgetItem*, int)), this, SLOT(ItemRun(QTreeWidgetItem*)) );
  connect(ui->list_dir_widget, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(ItemRun(QListWidgetItem*)) );
  connect(ui->tree_dir_widget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(OpenContextMenu(const QPoint&)) );
  connect(ui->list_dir_widget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(OpenContextMenu(const QPoint&)) );
  connect(dirWatcher, SIGNAL(directoryChanged(QString)), this, SLOT( loadDirectory() ) );
  //Page Switching
  connect(ui->tool_goToPlayer, SIGNAL(clicked()), this, SLOT(goToMultimediaPage()) );
  connect(ui->tool_goToRestore, SIGNAL(clicked()), this, SLOT(goToRestorePage()) );
  connect(ui->tool_goToImages, SIGNAL(clicked()), this, SLOT(goToSlideshowPage()) );
  connect(ui->actionBackToBrowser, SIGNAL(triggered()), this, SLOT(goToBrowserPage()) );
	
  //Slideshow page
  connect(ui->combo_image_name, SIGNAL(currentIndexChanged(int)), this, SLOT(showNewPicture()) );
  connect(ui->tool_image_goBegin, SIGNAL(clicked()), this, SLOT(firstPicture()) );
  connect(ui->tool_image_goEnd, SIGNAL(clicked()), this, SLOT(lastPicture()) );
  connect(ui->tool_image_goNext, SIGNAL(clicked()), this, SLOT(nextPicture()) );
  connect(ui->tool_image_goPrev, SIGNAL(clicked()), this, SLOT(prevPicture()) );
	
  //ZFS Restore page
  connect(snapmod, SIGNAL(directoryLoaded(QString)), this, SLOT(snapshotLoaded()) );
  connect(ui->slider_zfs_snapshot, SIGNAL(valueChanged(int)), this, SLOT(showSnapshot()) );
  connect(ui->tool_zfs_nextSnap, SIGNAL(clicked()), this, SLOT(nextSnapshot()) );
  connect(ui->tool_zfs_prevSnap, SIGNAL(clicked()), this, SLOT(prevSnapshot()) );
  connect(ui->tool_zfs_restoreItem, SIGNAL(clicked()), this, SLOT(restoreItems()) );
  
  //Special Keyboard Shortcuts
  connect(nextTabLShort, SIGNAL(activated()), this, SLOT( prevTab() ) );
  connect(nextTabRShort, SIGNAL(activated()), this, SLOT( nextTab() ) );
  connect(closeTabShort, SIGNAL(activated()), this, SLOT( tabClosed() ) );
  connect(copyFilesShort, SIGNAL(activated()), this, SLOT( CopyItems() ) );
  connect(pasteFilesShort, SIGNAL(activated()), this, SLOT( PasteItems() ) );
  connect(deleteFilesShort, SIGNAL(activated()), this, SLOT( RemoveItem() ) );
}

void MainUI::loadSettings(){
  //Note: make sure this is run after all the UI elements are created and connected to slots
  // but before the first directory gets loaded
  ui->actionView_Hidden_Files->setChecked( settings->value("showhidden", false).toBool() );
  on_actionView_Hidden_Files_triggered(); //make sure to update the models too
  QString view = settings->value("viewmode","details").toString();
  if(view=="icons"){ radio_view_icons->setChecked(true); }
  else if(view=="list"){ radio_view_list->setChecked(true); }
  else{ radio_view_details->setChecked(true); }
}

void MainUI::RebuildBookmarksMenu(){
  //Create the bookmarks menu
  ui->menuBookmarks->clear();
    ui->menuBookmarks->addAction(ui->actionManage_Bookmarks);
    ui->menuBookmarks->addSeparator();
  QStringList BM = settings->value("bookmarks", QStringList()).toStringList();
  ui->menuBookmarks->clear();
    ui->menuBookmarks->addAction(ui->actionManage_Bookmarks);
    ui->menuBookmarks->addSeparator();
  bool changed = false;
  BM.sort(); //Sort alphabetically
  for(int i=0; i<BM.length(); i++){
    if(QFile::exists(BM[i].section("::::",1,1)) ){
      QAction *act = new QAction(BM[i].section("::::",0,0),this);
        act->setWhatsThis(BM[i].section("::::",1,1));
      ui->menuBookmarks->addAction(act);
    }else{
      //Invalid directory - remove the bookmark
      BM.removeAt(i);
      i--;
      changed = true;
    }
  }
  if(changed){ settings->setValue("bookmarks",BM); }
  ui->actionManage_Bookmarks->setEnabled(BM.length()>0);
}

void MainUI::RebuildDeviceMenu(){
  //Create the External Devices Menu appropriately
  ui->menuExternal_Devices->clear();
    ui->menuExternal_Devices->addAction( ui->actionScan );
    ui->menuExternal_Devices->addSeparator();
  //Scan for externally mounted devices
  QStringList devs = LOS::ExternalDevicePaths();
    //Output Format: <type>::::<filesystem>::::<path>  (6/24/14 - version 0.4.0 )
        // <type> = [USB, HDRIVE, SDCARD, DVD, UNKNOWN]
	
  //Now add them to the menu appropriately
  for(int i=0; i<devs.length(); i++){
    QAction *act = new QAction(devs[i].section("::::",2,2).section("/",-1),this);
        act->setWhatsThis(devs[i].section("::::",2,2)); //full path to mountpoint
	act->setToolTip( QString(tr("Filesystem: %1")).arg( devs[i].section("::::",1,1) ) );
	//Now set the appropriate icon
	QString type = devs[i].section("::::",0,0);
	if(type=="USB"){ type = "drive-removable-media-usb"; }
	else if(type=="HDRIVE"){ type = "drive-harddisk"; }
	else if(type=="SDCARD"){ type = "media-flash-sd-mmc"; }
	else if(type=="DVD"){ type = "media-optical"; }
	else{ type = "drive-removable-media"; }
	act->setIcon( LXDG::findIcon(type, "") );
      ui->menuExternal_Devices->addAction(act);
  }
}

bool MainUI::checkUserPerms(){
  if(!isUserWritable){
    QMessageBox::warning(this, tr("Invalid Permissions"), tr("You do not have permission to edit this directory!") );
  }
  return isUserWritable;
}

QString MainUI::bytesToText(qint64 bytes){
  QString lab; //label
  double disp; //display number
  if(bytes > 1125899906842580){ lab="PB"; disp = bytes/1125899906842580.0; }
  else if(bytes>1099511627776){ lab="TB"; disp = bytes/1099511627776.0; }
  else if(bytes>1073741824){ lab="GB"; disp = bytes/1073741824.0; }
  else if(bytes>1048576){ lab="MB"; disp = bytes/1048576.0; }
  else if(bytes>1024){ lab="KB"; disp = bytes/1024.0; }
  else{ lab="B"; disp = bytes; }
  //Round to number of decimel places
  if(disp<10){ disp = qRound(disp*100)/(100.0); } //2 decimel places
  else if(disp<100){ disp = qRound(disp*10)/(10.0); } //1 decimel place
  else{ disp = qRound(disp); } // no decimel places
  //Return text
  return QString::number(disp)+" "+lab;
}

QString MainUI::getCurrentDir(){
  return currentDir->whatsThis();
}

void MainUI::setCurrentDir(QString dir){
  if(dir.isEmpty()){ return; }
  QFileInfo info(dir);
  if(!info.isDir() || !info.exists() ){ 
    qDebug() << "Invalid Directory:" << dir;
    //Try to just go up the dir tree one level
    dir.chop(dir.section("/",-1).length());
    if(!QFile::exists(dir)){
      //Still bad dir - try to return to previously shown dir
      if(currentDir->whatsThis().isEmpty()){ return; } //nothing to return to
      else{ dir = currentDir->whatsThis(); }
    }
  }
  //qDebug() << "Show Directory:" << dir;
  //qDebug() << "Dir Info:" << dir;
  //qDebug() << " - RWXLOG:" << info.isReadable() << info.isWritable() << info.isExecutable() << info.isSymLink() << info.ownerId() << info.groupId();
  isUserWritable = info.isWritable();
  if(dir.endsWith("/") && dir!="/" ){ dir.chop(1); }
  QString rawdir = dir;
  //dir.replace(QDir::homePath()+"/", "~/");
  currentDir->setText(dir);  
  //Update the directory viewer and update the line edit
  if(dir!=currentDir->whatsThis()){
    keepFocus = !currentDir->hasFocus();
    currentDir->setWhatsThis(dir); //save the full path internally
    fsmod->setRootPath(dir);
    QTimer::singleShot(0,this, SLOT(loadDirectory()) );
  }
  dirWatcher->removePaths( dirWatcher->directories() );
  dirWatcher->addPath(rawdir); //make sure we are watching this directory
  //Adjust the tab data
  tabBar->setTabWhatsThis( tabBar->currentIndex(), rawdir );
  if(dir!="/"){ tabBar->setTabText( tabBar->currentIndex(), dir.section("/",-1) ); }
  else{ tabBar->setTabText( tabBar->currentIndex(), dir); }
  QStringList history = tabBar->tabData(tabBar->currentIndex()).toStringList();
  if(history.isEmpty() || history.first()!=rawdir){ history.prepend(rawdir); history.removeAll(""); }
  //qDebug() << "History:" << history;
  tabBar->setTabData(tabBar->currentIndex(), history);
  //Now adjust the items as necessary
  QTimer::singleShot(0, this, SLOT(checkForMultimediaFiles()));
  QTimer::singleShot(0, this, SLOT(checkForBackups()));
  QTimer::singleShot(0, this, SLOT(checkForPictures()));
  ui->tool_addToDir->setVisible(isUserWritable);
  ui->actionUpDir->setEnabled(dir!="/");
  ui->actionBack->setEnabled(history.length() > 1);
  ui->actionBookMark->setEnabled( rawdir!=QDir::homePath() && settings->value("bookmarks", QStringList()).toStringList().filter("::::"+rawdir).length()<1 );
  RebuildDeviceMenu(); //keep this refreshed
}

QStringList MainUI::getSelectedItems(){
  QStringList out;
  if(radio_view_details->isChecked()){
    QList<QTreeWidgetItem*> items = ui->tree_dir_widget->selectedItems();
    for(int i=0; i<items.length(); i++){
       out << items[i]->text(0);
    }
  }else{
    QList<QListWidgetItem*> items = ui->list_dir_widget->selectedItems();
    for(int i=0; i<items.length(); i++){
       out << items[i]->text();
    }
  }	
  out.removeDuplicates();
  return out;
}

//==============
//    PRIVATE SLOTS
//==============
//General button check functions
void MainUI::checkForMultimediaFiles(){
  ui->tool_goToPlayer->setVisible(false);
  //Check for multimedia files not implemented yet!	
}

void MainUI::checkForBackups(){
  ui->tool_goToRestore->setVisible(false);
  //Check for ZFS snapshots not implemented yet!
  snapDirs.clear(); //clear the internal variable
  if(!isUserWritable){ return; } //cannot restore files into a non-writable directory
  //Now recursively try to find snapshots of this directory
  QString cdir = getCurrentDir();
  QDir dir(cdir);
  bool found = false;
  while(dir.absolutePath()!="/" && !found){
    if(dir.exists(".zfs/snapshot")){ found = true;}
    else{ dir.cdUp(); }
  }
  //Now find the snapshots that contain this directory and save them
  if(found){
    QString reldir = getCurrentDir();
	  reldir.remove(dir.absolutePath());
    dir.cd(".zfs/snapshot");
    snapDirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Time);
    //Check that the current directory exists in each snapshot
    for(int i=0; i<snapDirs.length(); i++){
      if( !dir.exists(snapDirs[i]+"/"+reldir) ){
        snapDirs.removeAt(i);
	i--;
      }else{
	snapDirs[i] = QFileInfo(dir, snapDirs[i]+"/"+reldir).created().toString("yyyyMMddhhmmsszzz")+"::::"+snapDirs[i];
      }
    }
    snapDirs.sort();
    //Sort the snapshots by time (newest last) and format them
    for(int i=0; i<snapDirs.length(); i++){
      snapDirs[i] = dir.absolutePath()+"/"+snapDirs[i].section("::::",1,50)+"/"+reldir;
    }
    //qDebug() << "Found snapshots:" << snapDirs;
  }
  //Now enable the button if any snapshots available
  ui->tool_goToRestore->setVisible(!snapDirs.isEmpty());
}

void MainUI::checkForPictures(){
  ui->tool_goToImages->setVisible(false);
  //Check for images not implemented yet!
  QDir dir(getCurrentDir());
  QStringList pics = dir.entryList(QStringList() << "*.png" << "*.jpg", QDir::Files | QDir::NoDotAndDotDot, QDir::Name | QDir::IgnoreCase);
  if(!pics.isEmpty()){
    ui->combo_image_name->clear();
    for(int i=0; i<pics.length(); i++){
      ui->combo_image_name->addItem(QIcon(dir.absoluteFilePath(pics[i])), pics[i]);
    }
    ui->tool_goToImages->setVisible(true);	  
  }
	
}

//General page switching
void MainUI::goToMultimediaPage(){
  //Make toolbar items disappear appropriately	
  ui->actionBackToBrowser->setVisible(true);
  ui->actionBack->setVisible(false);
  ui->actionUpDir->setVisible(false);
  ui->actionHome->setVisible(false);
  ui->actionBookMark->setVisible(false);
  currentDir->setEnabled(false);
  //Disable all the UI elements specifically for the Browser side of things
  ui->actionNew_Tab->setEnabled(false);
  //ui->menuEdit->setEnabled(false);
  ui->menuView->setEnabled(false);
  ui->menuBookmarks->setEnabled(false);
  ui->menuExternal_Devices->setEnabled(false);
  //Now go to the Multimedia player
  ui->stackedWidget->setCurrentWidget(ui->page_audioPlayer);
}

void MainUI::goToRestorePage(){
  //Make toolbar items disappear appropriately
  ui->actionBackToBrowser->setVisible(true);	
  ui->actionBack->setVisible(false);
  ui->actionUpDir->setVisible(false);
  ui->actionHome->setVisible(false);
  ui->actionBookMark->setVisible(false);
  currentDir->setEnabled(false);
  //Disable all the UI elements specifically for the Browser side of things
  ui->actionNew_Tab->setEnabled(false);
  //ui->menuEdit->setEnabled(false);
  ui->menuView->setEnabled(false);
  ui->menuBookmarks->setEnabled(false);
  ui->menuExternal_Devices->setEnabled(false);
  //Load all the info into the file restore page
  ui->slider_zfs_snapshot->setRange(1,snapDirs.length());
  ui->slider_zfs_snapshot->setValue(snapDirs.length());
  //Now go to the file restore page
  showSnapshot(); //Make sure it is updated for the current directory
  ui->stackedWidget->setCurrentWidget(ui->page_zfs);	
}

void MainUI::goToSlideshowPage(){
  //Make toolbar items disappear appropriately	
  ui->actionBackToBrowser->setVisible(true);
  ui->actionBack->setVisible(false);
  ui->actionUpDir->setVisible(false);
  ui->actionHome->setVisible(false);
  ui->actionBookMark->setVisible(false);
  currentDir->setEnabled(false);
  //Disable all the UI elements specifically for the Browser side of things
  ui->actionNew_Tab->setEnabled(false);
  //ui->menuEdit->setEnabled(false);
  ui->menuView->setEnabled(false);
  ui->menuBookmarks->setEnabled(false);
  ui->menuExternal_Devices->setEnabled(false);
  //Now go to the Slideshow player
  ui->stackedWidget->setCurrentWidget(ui->page_image_view);
  showNewPicture(); //make sure it is up to date with the widget size
}

void MainUI::goToBrowserPage(){
  //Make toolbar items re-appear appropriately	
  ui->actionBackToBrowser->setVisible(false);
  ui->actionBack->setVisible(true);
  ui->actionUpDir->setVisible(true);
  ui->actionHome->setVisible(true);
  ui->actionBookMark->setVisible(true);
  currentDir->setEnabled(true);
  //Disable all the UI elements specifically for the Browser side of things
  ui->actionNew_Tab->setEnabled(true);
  //ui->menuEdit->setEnabled(true);
  ui->menuView->setEnabled(true);
  ui->menuBookmarks->setEnabled(true);
  ui->menuExternal_Devices->setEnabled(true);
  //Now go to the browser
  ui->stackedWidget->setCurrentWidget(ui->page_browser);	
}
	
//Menu Actions
void MainUI::on_actionNew_Tab_triggered(){
  OpenDirs(QStringList() << QDir::homePath());
  //Now go to that tab (always last)
  tabBar->setCurrentIndex(tabBar->count()-1);
}

void MainUI::on_actionClose_triggered(){
  if(tabBar->count() > 1){
    if(QMessageBox::Yes != QMessageBox::question(this, tr("Verify Quit"), tr("You have multiple tabs open. Are you sure you want to quit?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes ) ){
      return;
    }
  }
  qDebug() << "Closing Down...";
  this->close();
}

void MainUI::on_actionView_Hidden_Files_triggered(){
  if(ui->actionView_Hidden_Files->isChecked()){
    fsmod->setFilter( QDir::AllEntries | QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Hidden );
    snapmod->setFilter( QDir::AllEntries | QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Hidden );
  }else{
    fsmod->setFilter( QDir::AllEntries | QDir::NoDotAndDotDot | QDir::AllDirs );
    snapmod->setFilter( QDir::AllEntries | QDir::NoDotAndDotDot | QDir::AllDirs );
  }
  //Now save this setting for later
  settings->setValue("showhidden", ui->actionView_Hidden_Files->isChecked());
  loadDirectory(); //Reload
}

void MainUI::goToBookmark(QAction *act){
  if(act==ui->actionManage_Bookmarks){
    BMMDialog dlg(this);
      dlg.loadSettings(settings);
      dlg.exec();
    RebuildBookmarksMenu();
  }else{
    setCurrentDir(act->whatsThis());
  }
}

void MainUI::goToDevice(QAction *act){
  if(act==ui->actionScan){
    RebuildDeviceMenu();
  }else{
    setCurrentDir(act->whatsThis());
  }
}

void MainUI::viewModeChanged(bool active){
  if(!active){ return; } //on every view change, all 3 radio buttons will call this function - only run this once though
  if(radio_view_details->isChecked()){
    ui->tree_dir_widget->setVisible(true);
    ui->list_dir_widget->setVisible(false);
	settings->setValue("viewmode","details");
  }else if(radio_view_list->isChecked()){
    ui->tree_dir_widget->setVisible(false);
    ui->list_dir_widget->setVisible(true);
    ui->list_dir_widget->setViewMode( QListView::ListMode );
	settings->setValue("viewmode","list");
  }else{  //icons
    ui->tree_dir_widget->setVisible(false);
    ui->list_dir_widget->setVisible(true);
    ui->list_dir_widget->setViewMode( QListView::IconMode );
	settings->setValue("viewmode","icons");
  }
  loadDirectory(); //reload current directory
	
}

//Toolbar Actions
void MainUI::on_actionBack_triggered(){
  QStringList history = tabBar->tabData(tabBar->currentIndex()).toStringList();
  if(history.length() <= 1){ return; } //need the second item
  history.removeAt(0); //remove the first item (the current dir)
  tabBar->setTabData(tabBar->currentIndex(), history); //re-write the saved history
  setCurrentDir(history.first()); //go to the previous entry in the history
}

void MainUI::on_actionUpDir_triggered(){
  QString dir =  getCurrentDir();
    dir.chop( dir.section("/",-1).length() );
    setCurrentDir(dir);
}

void MainUI::on_actionHome_triggered(){
  setCurrentDir(QDir::homePath());
}

void MainUI::on_actionBookMark_triggered(){
  QString dir = getCurrentDir();
  bool ok = false;
  QString name = QInputDialog::getText(this, tr("New Bookmark"), tr("Name:"), QLineEdit::Normal, dir, \
		&ok, 0, Qt::ImhFormattedNumbersOnly | Qt::ImhUppercaseOnly | Qt::ImhLowercaseOnly);
  if(!ok || name.isEmpty()){ return; } //cancelled
  QStringList BM = settings->value("bookmarks",QStringList()).toStringList();
  if(BM.filter(name+"::::").length() >0){
    QMessageBox::warning(this, tr("Invalid Name"), tr("This bookmark name already exists. Please choose another.") );
    QTimer::singleShot(0,this, SLOT(on_actionBookMark_triggered()));
    return;
  }
  BM.append(name+"::::"+dir);
  BM.sort(); //sort alphabetically by name
  settings->setValue("bookmarks", BM);
  //Now rebuild the bookmarks menu
  RebuildBookmarksMenu();
  ui->actionBookMark->setEnabled(false); //already bookmarked
}

//Browser Functions
void MainUI::startEditDir(QWidget *old, QWidget *now){
  if(now==currentDir){
    //The dir edit just got focus
    QString dir = currentDir->text();
      dir.replace("~/", QDir::homePath()+"/");
      currentDir->setText(dir);
      //Try to move to the end
      currentDir->selectAll();
  }else if(old==currentDir){
    QString dir = currentDir->text();
      setCurrentDir(dir);
  }
}

void MainUI::goToDirectory(){
  QString dir = currentDir->text();
  dir.replace("~/",QDir::homePath()+"/");
  setCurrentDir(dir);
}

void MainUI::loadDirectory(){
  //Disable the widgets
  ui->tree_dir_widget->setEnabled(false);
  ui->list_dir_widget->setEnabled(false);
  ui->label_dir_stats->setText(tr("Loading..."));
  //Load the directory contents and fill the widgets
  QDir dir(getCurrentDir());
  QFileInfoList list;
  if(ui->actionView_Hidden_Files->isChecked()){ list = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Hidden | QDir::AllEntries, QDir::Name | QDir::DirsFirst | QDir::IgnoreCase | QDir::LocaleAware); }
  else{ list = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries, QDir::Name | QDir::DirsFirst | QDir::IgnoreCase | QDir::LocaleAware); }
  //Clear the widgets
  ui->tree_dir_widget->clear();
  ui->list_dir_widget->clear();
  //reset the list widget icons sizes before adding items
  if(radio_view_list->isChecked()){ ui->list_dir_widget->setIconSize( QSize(20,20) ); }
  else{ ui->list_dir_widget->setIconSize( QSize(30,30) ); }
  //Now fill the widgets
  for(int i=0; i<list.length(); i++){
    //Get the appropriate icon
    QIcon ico;
    if(list[i].isDir()){ ico = LXDG::findIcon("folder",""); }
    else if(list[i].isExecutable()){ ico = LXDG::findIcon("application-x-executable",""); }
    else if(list[i].suffix()=="png" || list[i].suffix()=="jpg"){ ico = QIcon(list[i].absoluteFilePath()); }
    else{ ico = LXDG::findMimeIcon(list[i].suffix()); }
    //Add it to the widgets
    if(radio_view_details->isChecked()){
      QString sz;
      if(!list[i].isDir()){ sz = bytesToText(list[i].size()); }
      QTreeWidgetItem *it = new QTreeWidgetItem(QStringList() << list[i].fileName() << sz << list[i].lastModified().toString(Qt::DefaultLocaleShortDate) ); //add other info columns here later
      it->setIcon(0,ico);
      ui->tree_dir_widget->addTopLevelItem(it);
    }else{
      ui->list_dir_widget->addItem( new QListWidgetItem( ico, list[i].fileName() ) );
    }
    
  }
  if(list.length() > 0){ 
    //Make sure the first item is selected
    if(radio_view_details->isChecked()){  
      ui->tree_dir_widget->setCurrentItem( ui->tree_dir_widget->topLevelItem(0) ); 
      for(int i=0; i<3; i++){ ui->tree_dir_widget->resizeColumnToContents(i); }
    }else{ 
      ui->list_dir_widget->setCurrentRow(0); 
    }
  }
  //Re-enable the widgets
  ui->tree_dir_widget->setEnabled(true);
  ui->list_dir_widget->setEnabled(true);
  //Reset the keyboard focus back to the widget if necessary
  if(keepFocus){
    if(radio_view_details->isChecked()){ ui->tree_dir_widget->setFocus(); }
    else{ ui->list_dir_widget->setFocus(); }
  }
  if(isUserWritable){ ui->label_dir_stats->setText(""); }
  else{ ui->label_dir_stats->setText(tr("Limited Access Directory")); }
}

void MainUI::on_tool_addToDir_clicked(){
  bool ok = false;
  QString newdir = QInputDialog::getText(this, tr("New Directory"), tr("Name:"), QLineEdit::Normal, "", \
		&ok, 0, Qt::ImhFormattedNumbersOnly | Qt::ImhUppercaseOnly | Qt::ImhLowercaseOnly);
  if(!ok || newdir.isEmpty()){ return; }
  QString full = getCurrentDir();
  if(!full.endsWith("/")){ full.append("/"); }
  QDir dir(full); //open the current dir
  full.append(newdir); //append the new name to the current dir
  //Verify that the new dir does not already exist
  if(dir.exists(full)){
    QMessageBox::warning(this, tr("Invalid Name"), tr("A file or directory with that name already exists! Please pick a different name."));
    QTimer::singleShot(0,this, SLOT(on_tool_addToDir_clicked()) ); //repeat this function
  }else{
    if(!dir.mkdir(newdir) ){
      QMessageBox::warning(this, tr("Error Creating Directory"), tr("The directory could not be created. Please ensure that you have the proper permissions to modify the current directory."));
    }
  }
}

void MainUI::tabChanged(int tab){
  //Load the directory contained in the new tab
  qDebug() << "Change to Tab:" << tab << tabBar->tabText(tab);
  QString dir = tabBar->tabWhatsThis(tab); //get the full directory
  setCurrentDir(dir); //display this as the current dir
}

void MainUI::tabClosed(int tab){
  if(tabBar->count()==1){ return; } //Can't close the only tab
  if(tab < 0){ tab = tabBar->currentIndex(); }
  //Remove the tab (will automatically move to a different one);
  qDebug() << "Closing tab:" << tab << tabBar->tabText(tab);
  tabBar->removeTab(tab);
  tabBar->setVisible( tabBar->count() > 1 );
}

void MainUI::prevTab(){
  int cur = tabBar->currentIndex();
  if(cur == 0){ tabBar->setCurrentIndex( tabBar->count()-1 ); }
  else{ tabBar->setCurrentIndex( cur-1 ); }
}

void MainUI::nextTab(){
  int cur = tabBar->currentIndex();
  if(cur == (tabBar->count()-1) ){ tabBar->setCurrentIndex(0); }
  else{ tabBar->setCurrentIndex( cur+1 ); }	
}

void MainUI::ItemRun(const QModelIndex &index){
  //This is called when the user double clicks a file/directory
  QString val = fsmod->filePath(index).section("/",-1);
  QString itemPath = getCurrentDir();
  if( !itemPath.endsWith("/")){ itemPath.append("/"); }
  itemPath.append(val);
  if(fsmod->isDir(index)){
    setCurrentDir( itemPath );
  }else{
    //Must be a file, try to run it
    QProcess::startDetached("lumina-open "+itemPath);
  }
}

void MainUI::ItemRun(QTreeWidgetItem *item){
  //This is called when the user double clicks a file/directory
  QString val = item->text(0);
  QString itemPath = getCurrentDir();
  if( !itemPath.endsWith("/")){ itemPath.append("/"); }
  itemPath.append(val);
  if(QFileInfo(itemPath).isDir()){
    setCurrentDir( itemPath );
  }else{
    //Must be a file, try to run it
    QProcess::startDetached("lumina-open "+itemPath);
  }
}

void MainUI::ItemRun(QListWidgetItem *item){
  //This is called when the user double clicks a file/directory
  QString val = item->text();
  QString itemPath = getCurrentDir();
  if( !itemPath.endsWith("/")){ itemPath.append("/"); }
  itemPath.append(val);
  if(QFileInfo(itemPath).isDir()){
    setCurrentDir( itemPath );
  }else{
    //Must be a file, try to run it
    QProcess::startDetached("lumina-open "+itemPath);
  }
}

void MainUI::OpenContextMenu(const QPoint &pt){
  if(radio_view_details->isChecked()){ 
    QTreeWidgetItem *it = ui->tree_dir_widget->itemAt(pt);
    if(it==0){ return;}
    else{ CItem = it->text(0); }
  }else{ 
    QListWidgetItem *it = ui->list_dir_widget->itemAt(pt);
    if(it==0){ return; }
    else{ CItem = it->text(); }
  }
  QString cdir = getCurrentDir();
  if(!cdir.endsWith("/")){ cdir.append("/"); }
  CItem.prepend(cdir);
    //Create the context menu
    contextMenu->clear();
    QFileInfo info(CItem);
    if(info.isDir()){
      contextMenu->addAction(LXDG::findIcon("tab-new-background",""), tr("Open in new tab"), this, SLOT(OpenDir()) );
    }else{
      contextMenu->addAction(LXDG::findIcon("quickopen-file",""), tr("Open"), this, SLOT(OpenItem()) );
      contextMenu->addAction(tr("Open With..."), this, SLOT(OpenItemWith()) );
    }
    contextMenu->addAction(LXDG::findIcon("edit-rename",""), tr("Rename"), this, SLOT(RenameItem()) )->setEnabled(info.isWritable());
    contextMenu->addSeparator();
  //Now add the general selection options
  contextMenu->addAction(LXDG::findIcon("edit-cut",""), tr("Cut Selection"), this, SLOT(CutItems()) )->setEnabled(info.isWritable());
  contextMenu->addAction(LXDG::findIcon("edit-copy",""), tr("Copy Selection"), this, SLOT(CopyItems()) );
  contextMenu->addAction(LXDG::findIcon("edit-paste",""), tr("Paste"), this, SLOT(PasteItems()) )->setEnabled(QApplication::clipboard()->mimeData()->hasFormat("x-special/lumina-copied-files") && isUserWritable);
  contextMenu->addSeparator();
  contextMenu->addAction(LXDG::findIcon("edit-delete",""), tr("Delete Selection"), this, SLOT(RemoveItem()) )->setEnabled(info.isWritable());
  //Now show the menu
  if(radio_view_details->isChecked()){
    contextMenu->popup(ui->tree_dir_widget->mapToGlobal(pt));
  }else{
    contextMenu->popup(ui->list_dir_widget->mapToGlobal(pt));
  }
}

//Slideshow Functions
void MainUI::showNewPicture(){
  if( !ui->label_image->isVisible() ){ return; } //don't update if not visible - can cause strange resizing issues
  QString file = getCurrentDir();
  if(!file.endsWith("/")){ file.append("/"); }
  file.append(ui->combo_image_name->currentText());
  if(!file.endsWith(".png") && !file.endsWith(".jpg")){ return; } //invalid - no change
  //qDebug() << "Show Image:" << file;
  QPixmap pix(file);
  if(pix.size().width() > ui->label_image->contentsRect().width() || pix.size().height() > ui->label_image->contentsRect().height()){ 
    pix = pix.scaled(ui->label_image->contentsRect().size(), Qt::KeepAspectRatio, Qt::SmoothTransformation); 
  }
  ui->label_image->setPixmap(pix);
  //Now set/load the buttons
  ui->tool_image_goBegin->setEnabled(ui->combo_image_name->currentIndex()>0);
  ui->tool_image_goPrev->setEnabled(ui->combo_image_name->currentIndex()>0);
  ui->tool_image_goEnd->setEnabled(ui->combo_image_name->currentIndex()<(ui->combo_image_name->count()-1));
  ui->tool_image_goNext->setEnabled(ui->combo_image_name->currentIndex()<(ui->combo_image_name->count()-1));
  ui->label_image_index->setText( QString::number(ui->combo_image_name->currentIndex()+1)+"/"+QString::number(ui->combo_image_name->count()) );
}

void MainUI::firstPicture(){
  ui->combo_image_name->setCurrentIndex(0);
}

void MainUI::prevPicture(){
  ui->combo_image_name->setCurrentIndex( ui->combo_image_name->currentIndex()-1 );
}

void MainUI::nextPicture(){
  ui->combo_image_name->setCurrentIndex( ui->combo_image_name->currentIndex()+1 );
}

void MainUI::lastPicture(){
  ui->combo_image_name->setCurrentIndex( ui->combo_image_name->count()-1 );
}

//ZFS Restore Functions
void MainUI::snapshotLoaded(){
  ui->tree_zfs_dir->resizeColumnToContents(0);
}

void MainUI::showSnapshot(){
  ui->tool_zfs_prevSnap->setEnabled(ui->slider_zfs_snapshot->value()!=1);
  ui->tool_zfs_nextSnap->setEnabled(ui->slider_zfs_snapshot->value()!=ui->slider_zfs_snapshot->maximum());
  ui->label_zfs_snap->setText( snapDirs[ui->slider_zfs_snapshot->value()-1].section("/.zfs/snapshot/",1,1).section("/",0,0) );
  //Load the dir contents
  ui->tree_zfs_dir->setRootIndex(snapmod->setRootPath(snapDirs[ui->slider_zfs_snapshot->value()-1]));
}

void MainUI::prevSnapshot(){
  ui->slider_zfs_snapshot->setValue(ui->slider_zfs_snapshot->value()-1);
}

void MainUI::nextSnapshot(){
  ui->slider_zfs_snapshot->setValue(ui->slider_zfs_snapshot->value()+1);
}

void MainUI::restoreItems(){
   //Get the selected items
   QStringList sel = getSelectedItems();
   QModelIndexList items = ui->tree_zfs_dir->selectionModel()->selectedIndexes();
   for(int i=0; i<items.length(); i++){
     sel << snapmod->filePath(items[i]).section("/",-1);
   }
   sel.removeDuplicates();
   if(sel.isEmpty()){ return; } //nothing selected
   if(!checkUserPerms()){ return; }
   //Get the directories
   QString snapdir = snapDirs[ui->slider_zfs_snapshot->value()-1];
   QString basedir = getCurrentDir();
   if(!basedir.endsWith("/")){ basedir.append("/"); }
   if(!snapdir.endsWith("/")){ snapdir.append("/"); }
   //Fill out the lists appropriately
   QStringList resto;
   qDebug() << "Items Selected:" << sel;
   for(int i=0; i<sel.length(); i++){
     resto << basedir+sel[i];
     sel[i] = snapdir+sel[i];
   }
   qDebug() << "Restore Items:" << sel << "\n" << resto;
   //Restore the items
   FODialog dlg(this);
	dlg.setOverwrite(ui->check_zfs_overwrite->isChecked());
	dlg.RestoreFiles(sel, resto);
   dlg.exec();
   if(dlg.noerrors){
     QMessageBox::information(this, tr("Success"), tr("Successfully restored selection") );
   }
}

// Context Menu Actions
void MainUI::OpenItem(){
  if(CItem.isEmpty()){ return; }
  qDebug() << "Opening File:" << CItem;
  QProcess::startDetached("lumina-open \""+CItem+"\"");
}

void MainUI::OpenItemWith(){
  if(CItem.isEmpty()){ return; }
  qDebug() << "Opening File:" << CItem;
  QProcess::startDetached("lumina-open -select \""+CItem+"\"");	
}

void MainUI::OpenDir(){
  if(CItem.isEmpty()){ return; }
  OpenDirs(QStringList() << CItem);		
}

void MainUI::RemoveItem(){
  //Only let this run if viewing the browser page
  if(ui->stackedWidget->currentWidget()!=ui->page_browser){ return; }
   if(!checkUserPerms()){ return; }
   //Get the selected items
   QStringList sel, names;
   names = getSelectedItems();
   //QModelIndexList items = ui->tree_dir_view->selectionModel()->selectedIndexes();
   QString baseDir = getCurrentDir();
   if(!baseDir.endsWith("/")){ baseDir.append("/"); }
   for(int i=0; i<names.length(); i++){
     sel << baseDir+names[i];
   }
   if(sel.isEmpty()){ return; } //nothing selected
  //Verify permanent removal of file/dir
  if(QMessageBox::Yes != QMessageBox::question(this, tr("Verify Removal"), tr("WARNING: This will permanently delete the file(s) from the system!")+"\n"+tr("Are you sure you want to continue?")+"\n\n"+names.join("\n"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No) ){
    return; //cancelled
  }
  //Now remove the file/dir
  qDebug() << "Delete: "<<sel;
  FODialog dlg(this);
    dlg.RemoveFiles(sel);
    dlg.exec();
}

void MainUI::RenameItem(){
  //Only let this run if viewing the browser page
  if(ui->stackedWidget->currentWidget()!=ui->page_browser){ return; }
  if(CItem.isEmpty()){ return; }
  if(!checkUserPerms()){ return; }
  QString fname = CItem;
  QString path = fname;
    fname = fname.section("/",-1); //turn this into just the file name
    path.chop(fname.length()); 	//turn this into the base directory path (has a "/" at the end)
  //Now prompt for the new filename
  bool ok = false;
  QString nname = QInputDialog::getText(this, tr("Rename File"),tr("New Name:"), QLineEdit::Normal, fname, &ok);
  if(!ok || nname.isEmpty()){ return; } //cancelled
  //Now check for a file extension and add it if necessary
  QString oext = fname.section(".",-1);
    if(oext==fname){ oext.clear(); } //no extension
  QString next = nname.section(".",-1);
    if(next==nname){ next.clear(); } //no extension
  if(next.isEmpty() && !oext.isEmpty()){
    nname.append( "."+oext );
  }
  //Check if this filename already exists
  bool overwrite = QFile::exists(path+nname);
  if(overwrite){
    if(QMessageBox::Yes != QMessageBox::question(this, tr("Overwrite File?"), tr("An existing file with the same name will be replaced. Are you sure you want to proceed?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No) ){
      return; //cancelled
    }
  }
  //Now perform the move
  qDebug() << "Rename:" << path+fname << "->" << path+nname;
  FODialog dlg(this);
    dlg.setOverwrite(overwrite);
    dlg.MoveFiles(QStringList() << path+fname, QStringList() << path+nname);
    dlg.exec();
}

void MainUI::CutItems(){
  //Only let this run if viewing the browser page
  if(ui->stackedWidget->currentWidget()!=ui->page_browser){ return; }
  if(!checkUserPerms()){ return; }
  //Get all the selected Items 
  QStringList sel = getSelectedItems();
  /*QModelIndexList items = ui->tree_dir_view->selectionModel()->selectedIndexes();
  for(int i=0; i<items.length(); i++){
    sel << fsmod->filePath(items[i]);
  }
  sel.removeDuplicates();*/
  if(sel.isEmpty()){ return; } //nothing selected
  qDebug() << "Cut Items:" << sel;
  //Format the data string
  for(int i=0; i<sel.length(); i++){
    sel[i] = sel[i].prepend("cut::::");
  }
  
  //Now save that data to the global clipboard
  QMimeData *dat = new QMimeData;
	dat->clear();
	dat->setData("x-special/lumina-copied-files", sel.join("\n").toLocal8Bit());
  QApplication::clipboard()->clear();
  QApplication::clipboard()->setMimeData(dat);
}

void MainUI::CopyItems(){
  //Only let this run if viewing the browser page
  if(ui->stackedWidget->currentWidget()!=ui->page_browser){ return; }
  //Get all the selected Items 
  QStringList sel = getSelectedItems();
  /*QModelIndexList items = ui->tree_dir_view->selectionModel()->selectedIndexes();
  for(int i=0; i<items.length(); i++){
    sel << fsmod->filePath(items[i]);
  }
  sel.removeDuplicates();*/
  if(sel.isEmpty()){ return; } //nothing selected
  qDebug() << "Copy Items:" << sel;
  //Format the data string
  for(int i=0; i<sel.length(); i++){
    sel[i] = sel[i].prepend("copy::::");
  }
  //Now save that data to the global clipboard
  QMimeData *dat = new QMimeData;
	dat->clear();
	dat->setData("x-special/lumina-copied-files", sel.join("\n").toLocal8Bit());
  QApplication::clipboard()->clear();
  QApplication::clipboard()->setMimeData(dat);	
}

void MainUI::PasteItems(){
  //Only let this run if viewing the browser page
  if(ui->stackedWidget->currentWidget()!=ui->page_browser){ return; }
  const QMimeData *dat = QApplication::clipboard()->mimeData();
  if(!dat->hasFormat("x-special/lumina-copied-files")){ return; } //nothing to paste
  if(!checkUserPerms()){ return; }
  QStringList cut, copy, newcut, newcopy;
  QStringList raw = QString(dat->data("x-special/lumina-copied-files")).split("\n");
  QString base = getCurrentDir();
  if(!base.endsWith("/")){ base.append("/"); }
  for(int i=0; i<raw.length(); i++){
    if(raw[i].startsWith("cut::::")){ 
	cut << raw[i].section("::::",1,50);
	newcut << base+raw[i].section("::::",1,50).section("/",-1);
    }
    else if(raw[i].startsWith("copy::::")){ 
	copy << raw[i].section("::::",1,50); 
	newcopy<< base+raw[i].section("::::",1,50).section("/",-1);
    }
  }
  bool errs = false;
  //Perform the copy/move operations
  if(!copy.isEmpty()){ 
    qDebug() << "Paste Copy:" << copy << "->" << newcopy;
    FODialog dlg(this);
      dlg.CopyFiles(copy, newcopy);
      dlg.exec();
      errs = errs || !dlg.noerrors;
  }
  if(!cut.isEmpty()){
    qDebug() << "Paste Cut:" << cut << "->" << newcut;
    FODialog dlg(this);
      dlg.MoveFiles(cut, newcut);
      dlg.exec();
      errs = errs || !dlg.noerrors;
  }
  //Modify the clipboard appropriately
  if(!errs && !cut.isEmpty()){
    //Now clear the clipboard since those old file locations are now invalid
    QApplication::clipboard()->clear(); 
    if(!copy.isEmpty()){
      //There were also files copied: save those files back into the clipboard
       QMimeData *dat = new QMimeData;
	dat->clear();
	dat->setData("x-special/lumina-copied-files", raw.filter("copy::::").join("\n").toLocal8Bit());
	QApplication::clipboard()->setMimeData(dat);
    }
  }
	
}
