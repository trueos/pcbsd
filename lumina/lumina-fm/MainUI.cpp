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
    ui->verticalLayout_browser->insertWidget(0,tabBar);
  currentDir = new QLineEdit(this);
    ui->toolBar->insertWidget(ui->actionBookMark, currentDir);
  fsmod = new QFileSystemModel(this);
    ui->tree_dir_view->setModel(fsmod);
    ui->tree_dir_view->setItemsExpandable(false);
    ui->tree_dir_view->setExpandsOnDoubleClick(false);
    ui->tree_dir_view->setRootIsDecorated(false);
    ui->tree_dir_view->setSortingEnabled(true);
    ui->tree_dir_view->sortByColumn(0,Qt::AscendingOrder);
    ui->tree_dir_view->setContextMenuPolicy(Qt::CustomContextMenu);
  contextMenu = new QMenu(this);
  setupIcons();
  setupConnections();
  loadSettings();
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
  ui->actionClose->setIcon( LXDG::findIcon("application-close","") );
  ui->actionNew_Tab->setIcon( LXDG::findIcon("tab-new-background","") );
  ui->action_Preferences->setIcon( LXDG::findIcon("configure","") );
  ui->actionRefresh->setIcon( LXDG::findIcon("view-refresh","") );
  ui->actionUpDir->setIcon( LXDG::findIcon("go-up","") );
  ui->actionBack->setIcon( LXDG::findIcon("go-previous","") );
  ui->actionHome->setIcon( LXDG::findIcon("go-home","") );
  ui->actionBookMark->setIcon( LXDG::findIcon("bookmarks","") );
  ui->actionBackToBrowser->setIcon( LXDG::findIcon("go-previous","") );
  ui->actionManage_Bookmarks->setIcon( LXDG::findIcon("bookmarks-organize","") );
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
  ui->tool_zfs_restoreAll->setIcon( LXDG::findIcon("edit-undo","") );
  ui->tool_zfs_restoreItem->setIcon( LXDG::findIcon("document-revert","") );
	
}

void MainUI::setupConnections(){
  connect(tabBar, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)) );
  connect(tabBar, SIGNAL(tabCloseRequested(int)), this, SLOT(tabClosed(int)) );
  connect(ui->tree_dir_view, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(OpenContextMenu(const QPoint&)) );
  connect(ui->menuBookmarks, SIGNAL(triggered(QAction*)), this, SLOT(goToBookmark(QAction*)) );
  //Tree Widget interaction
  connect(ui->tree_dir_view, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(ItemRun(const QModelIndex &)) );
	
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
}

void MainUI::loadSettings(){
  //Note: make sure this is run after all the UI elements are created and connected to slots
  // but before the first directory gets loaded
  ui->actionView_Hidden_Files->setChecked( settings->value("showhidden", false).toBool() );
  
  //Create the bookmarks menu
  ui->menuBookmarks->clear();
    ui->menuBookmarks->addAction(ui->actionManage_Bookmarks);
    ui->menuBookmarks->addSeparator();
  QStringList BM = settings->value("bookmarks", QStringList()).toStringList();
  bool changed = false;
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
}

void MainUI::loadBrowseDir(QString dir){
  qDebug() << "Load Browse Dir:" << dir;
}

void MainUI::loadSnapshot(QString dir){
  qDebug() << "Load Snapshot:" << dir;
}

bool MainUI::findSnapshotDir(){
  qDebug() << "Find Snapshot Dir:";
  return false;
}

QString MainUI::getCurrentDir(){
  QString dir = currentDir->text();
	dir.replace("~/", QDir::homePath()+"/");
  return dir;
}

void MainUI::setCurrentDir(QString dir){
  if(dir.isEmpty() || !QFile::exists(dir)){ 
    qDebug() << "Invalid Directory:" << dir;
    return; 
  } //do nothing
  //qDebug() << "Show Directory:" << dir;
  if(dir.endsWith("/") && dir!="/" ){ dir.chop(1); }
  QString rawdir = dir;
  //Update the directory viewer and update the line edit
  ui->tree_dir_view->setRootIndex( fsmod->setRootPath(dir) );
  dir.replace(QDir::homePath()+"/", "~/");
  currentDir->setText(dir);
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
  ui->actionUpDir->setEnabled(dir!="/");
  ui->actionBack->setEnabled(history.length() > 1);
  ui->actionBookMark->setEnabled( rawdir!=QDir::homePath() && settings->value("bookmarks", QStringList()).toStringList().filter("::::"+rawdir).length()<1 );
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
  //Now recursively try to find snapshots of this directory
  /*QString cdir = getCurrentDir();
  QDir dir(cdir);
  bool found = false;
  while(dir.absolutePath()!="/" && !found){
	  
  }
  //Now find the snapshots that contain this directory and save them
  if(found){
	  
  }
  //Now enable the button if any snapshots available
  ui->tool_goToRestore->setVisible(snapDirs.isEmpty());*/
}

void MainUI::checkForPictures(){
  ui->tool_goToImages->setVisible(false);
  //Check for images not implemented yet!
  QDir dir(getCurrentDir());
  QStringList pics = dir.entryList(QStringList() << "*.png" << "*.jpg", QDir::Files | QDir::NoDotAndDotDot, QDir::Name | QDir::IgnoreCase);
  if(!pics.isEmpty()){
    ui->combo_image_name->clear();
    ui->combo_image_name->addItems(pics);
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
  ui->menuEdit->setEnabled(false);
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
  ui->menuEdit->setEnabled(false);
  ui->menuView->setEnabled(false);
  ui->menuBookmarks->setEnabled(false);
  ui->menuExternal_Devices->setEnabled(false);
  //Now go to the file restore page
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
  ui->menuEdit->setEnabled(false);
  ui->menuView->setEnabled(false);
  ui->menuBookmarks->setEnabled(false);
  ui->menuExternal_Devices->setEnabled(false);
  //Now go to the Slideshow player
  //showNewPicture(); //update the image viewer first
  ui->stackedWidget->setCurrentWidget(ui->page_image_view);
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
  ui->menuEdit->setEnabled(true);
  ui->menuView->setEnabled(true);
  ui->menuBookmarks->setEnabled(true);
  ui->menuExternal_Devices->setEnabled(true);
  //Now go to the browser
  ui->stackedWidget->setCurrentWidget(ui->page_browser);	
}
	
//Menu Actions
void MainUI::on_actionNew_Tab_triggered(){
  OpenDirs(QStringList() << QDir::homePath());
}

void MainUI::goToBookmark(QAction *act){
  if(act==ui->actionManage_Bookmarks){
    qDebug() << "Bookmark Manager not implemented yet!";
  }else{
    setCurrentDir(act->whatsThis());
  }
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
  ui->menuBookmarks->clear();
    ui->menuBookmarks->addAction(ui->actionManage_Bookmarks);
    ui->menuBookmarks->addSeparator();
  bool changed = false;
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
  ui->actionBookMark->setEnabled(false); //already bookmarked
}

//Browser Functions
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
  //ui->tree_dir_view->setRootIndex( fsmod->setRootPath(dir) ); //Now update the browser
}

void MainUI::tabClosed(int tab){
  //Remove the tab (will automatically move to a different one);
  qDebug() << "Closing tab:" << tab << tabBar->tabText(tab);
  tabBar->removeTab(tab);
  tabBar->setVisible( tabBar->count() > 1 );
}

void MainUI::ItemRun(const QModelIndex &index){
  //This is called when the user double clicks a file/directory
  QString val = fsmod->fileName(index);
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

void MainUI::OpenContextMenu(const QPoint &pt){
  CItem = ui->tree_dir_view->indexAt(pt);
  if(!CItem.isValid()){ return; }
  //Create the context menu for this particular item
  contextMenu->clear();
  if(fsmod->isDir(CItem)){
    contextMenu->addAction(LXDG::findIcon("tab-new-background",""), tr("Open in new tab"), this, SLOT(OpenDir()) );
  }else{
    contextMenu->addAction(LXDG::findIcon("quickopen-file",""), tr("Open"), this, SLOT(OpenItem()) );
    contextMenu->addAction(tr("Open With..."), this, SLOT(OpenItemWith()) );
  }
  contextMenu->addSeparator();
  contextMenu->addAction(LXDG::findIcon("list-remove",""), tr("Delete"), this, SLOT(RemoveItem()) );
  contextMenu->popup(ui->tree_dir_view->mapToGlobal(pt));
}

//Slideshow Functions
void MainUI::showNewPicture(){
  QString file = getCurrentDir();
  if(!file.endsWith("/")){ file.append("/"); }
  file.append(ui->combo_image_name->currentText());
  if(!file.endsWith(".png") && !file.endsWith(".jpg")){ return; } //invalid - no change
  //qDebug() << "Show Image:" << file;
  QPixmap pix(file);
  if(pix.size().width() > ui->label_image->size().width() || pix.size().height() > ui->label_image->size().height()){ 
    pix = pix.scaled(ui->label_image->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation); 
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

// Context Menu Actions
void MainUI::OpenItem(){
  if(!CItem.isValid()){ return; }
  QString fname = fsmod->fileName(CItem);
  QString baseDir = getCurrentDir();
  if(!baseDir.endsWith("/")){ baseDir.append("/"); }
  baseDir.append(fname);
  qDebug() << "Opening File:" << baseDir;
  QProcess::startDetached("lumina-open \""+baseDir+"\"");
}

void MainUI::OpenItemWith(){
  if(!CItem.isValid()){ return; }
  QString fname = fsmod->fileName(CItem);
  QString baseDir = getCurrentDir();
  if(!baseDir.endsWith("/")){ baseDir.append("/"); }
  baseDir.append(fname);
  qDebug() << "Opening File:" << baseDir;
  QProcess::startDetached("lumina-open -select \""+baseDir+"\"");	
}

void MainUI::OpenDir(){
  if(!CItem.isValid()){ return; }
  QString fname = fsmod->fileName(CItem);
  QString baseDir = getCurrentDir();
  if(!baseDir.endsWith("/")){ baseDir.append("/"); }
  baseDir.append(fname);
  OpenDirs(QStringList() << baseDir);		
}

void MainUI::RemoveItem(){
  if(!CItem.isValid()){ return; }
  QModelIndex index = CItem; //save this for the moment - since the CItem might get cleared in a couple seconds
  QString fname = fsmod->fileName(index);
  QString baseDir = getCurrentDir();
  if(!baseDir.endsWith("/")){ baseDir.append("/"); }
  baseDir.append(fname);
  //Verify permanent removal of file/dir
  if(QMessageBox::Yes != QMessageBox::question(this, tr("Verify Removal"), tr("WARNING: This will permanently delete the file/directory from the system!")+"\n"+tr("Are you sure you want to continue?")+"\n\n"+fname, QMessageBox::Yes | QMessageBox::No, QMessageBox::No) ){
    return; //cancelled
  }
  //Now remove the file/dir
  bool ok = false;
  qDebug() << "Delete: "<<baseDir;
  if(fsmod->isDir(index)){
    qDebug() << " - Note: Recursive directory deletion not implemented yet!";
    ok = fsmod->rmdir(index);
  }else{
    ok = fsmod->remove(index);
  }
  if(!ok){
    QMessageBox::warning(this, tr("Removal Failure"), tr("The file or directory could not be deleted. Please check that you have the proper permissions to delete it."));
  }
}

void MainUI::RunInMediaPlayer(){ //open in the media player
	
}
void MainUI::RunInSlideShow(){ //open in slideshow viewer
	
}
