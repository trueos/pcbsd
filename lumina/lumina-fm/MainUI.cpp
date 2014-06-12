#include "MainUI.h"
#include "ui_MainUI.h"

MainUI::MainUI() : QMainWindow(), ui(new Ui::MainUI){
  ui->setupUi(this);
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
  //tabBar->setTabsClosable( tabBar->count() > 1);
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
  //Browser page
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
  //Tree Widget interaction
  connect(ui->tree_dir_view, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(ItemRun(const QModelIndex &)) );
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
  ui->actionUpDir->setEnabled(dir!="/");
  ui->actionBack->setEnabled(history.length() > 1);
}

//==============
//    PRIVATE SLOTS
//==============

//General page switching
void MainUI::goToMultimediaPage(){
	
}

void MainUI::goToRestorePage(){
	
}

void MainUI::goToSlideshowPage(){
	
}

void MainUI::goToBrowserPage(){
	
}
	
//Menu Actions
void MainUI::on_actionNew_Tab_triggered(){
  OpenDirs(QStringList() << QDir::homePath());
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
	
}

//Browser Functions
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
  contextMenu->clear();
  if(fsmod->isDir(CItem)){
    contextMenu->addAction(LXDG::findIcon("tab-new-background",""), tr("Open in new tab"), this, SLOT(OpenDir()) );
  }else{
    contextMenu->addAction(LXDG::findIcon("quickopen-file",""), tr("Open"), this, SLOT(OpenItem()) );
    contextMenu->addAction(tr("Open With..."), this, SLOT(OpenItemWith()) );
  }
  contextMenu->popup(ui->tree_dir_view->mapToGlobal(pt));
}

void MainUI::OpenItem(){
  if(!CItem.isValid()){ return; }
  QString fname = fsmod->fileName(CItem);
  QString baseDir = getCurrentDir();
  if(!baseDir.endsWith("/")){ baseDir.append("/"); }
  baseDir.append(fname);
  qDebug() << "Opening File:" << baseDir;
  QProcess::startDetached("lumina-open "+baseDir);
}

void MainUI::OpenItemWith(){
  if(!CItem.isValid()){ return; }
  QString fname = fsmod->fileName(CItem);
  QString baseDir = getCurrentDir();
  if(!baseDir.endsWith("/")){ baseDir.append("/"); }
  baseDir.append(fname);
  qDebug() << "Opening File:" << baseDir;
  QProcess::startDetached("lumina-open -select "+baseDir);	
}

void MainUI::OpenDir(){
  if(!CItem.isValid()){ return; }
  QString fname = fsmod->fileName(CItem);
  QString baseDir = getCurrentDir();
  if(!baseDir.endsWith("/")){ baseDir.append("/"); }
  baseDir.append(fname);
  OpenDirs(QStringList() << baseDir);		
}

void MainUI::RunInMediaPlayer(){ //open in the media player
	
}
void MainUI::RunInSlideShow(){ //open in slideshow viewer
	
}
