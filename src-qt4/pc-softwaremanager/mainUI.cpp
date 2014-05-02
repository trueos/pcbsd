/***************************************************************************
 *   Copyright (C) 2011 - iXsystems				 	  *
 *   kris@pcbsd.org  *
 *   tim@pcbsd.org   *
 *   ken@pcbsd.org   *
 *                                                                         *
 *   Permission is hereby granted, free of charge, to any person obtaining *
 *   a copy of this software and associated documentation files (the       *
 *   "Software"), to deal in the Software without restriction, including   *
 *   without limitation the rights to use, copy, modify, merge, publish,   *
 *   distribute, sublicense, and/or sell copies of the Software, and to    *
 *   permit persons to whom the Software is furnished to do so, subject to *
 *   the following conditions:                                             *
 *                                                                         *
 *   The above copyright notice and this permission notice shall be        *
 *   included in all copies or substantial portions of the Software.       *
 *                                                                         *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       *
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    *
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. *
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR     *
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, *
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR *
 *   OTHER DEALINGS IN THE SOFTWARE.                                       *
 ***************************************************************************/
#include "mainUI.h"
#include "ui_mainUI.h"

MainUI::MainUI(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainUI){
  //initialization function
  ui->setupUi(this); //load the Qt-Designer file
  defaultIcon = ":/application.png";
  statusLabel = new QLabel();
  ui->statusbar->addWidget(statusLabel);
  netman = new QNetworkAccessManager(this);
    connect(netman, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotScreenshotAvailable(QNetworkReply*)) );
    netreply = 0;
	
  ui->text_dev_output->setVisible(false);
	
  //additional connections to the UI
  connect(ui->actionGraphical_Apps, SIGNAL(triggered(bool)), this, SLOT( browserViewSettingsChanged() ) );
  connect(ui->actionText_Apps, SIGNAL(triggered(bool)), this, SLOT( browserViewSettingsChanged() ) );
  connect(ui->actionServer_Apps, SIGNAL(triggered(bool)), this, SLOT( browserViewSettingsChanged() ) );
  connect(ui->actionRaw_Packages, SIGNAL(triggered(bool)), this, SLOT( browserViewSettingsChanged() ) );
}

void MainUI::ProgramInit()
{ 
   qDebug("Application starting...");
   //Now startup the backend
   qDebug() << "Startup Backend";
   PBI = new PBIBackend();
   //Initialize the Installed tab
   qDebug() << "Initialize Installed Tab";
   initializeInstalledTab();
   //Initialize the PBI Browser
   qDebug() << "Initialize Browser Tab";
   initializeBrowserTab();
   

     connect(PBI,SIGNAL(LocalPBIChanges()),this,SLOT(slotRefreshInstallTab()) );
     connect(PBI,SIGNAL(PBIStatusChange(QString)),this,SLOT(slotPBIStatusUpdate(QString)) );
     connect(PBI,SIGNAL(RepositoryInfoReady()),this,SLOT(slotEnableBrowser()) );
     connect(PBI,SIGNAL(NoRepoAvailable()),this,SLOT(slotDisableBrowser()) );
     connect(PBI,SIGNAL(SearchComplete(QStringList,QStringList)),this,SLOT(slotShowSearchResults(QStringList, QStringList)) );
     connect(PBI,SIGNAL(SimilarFound(QStringList)),this,SLOT(slotShowSimilarApps(QStringList)) );
     connect(PBI,SIGNAL(Error(QString,QString,QStringList)),this,SLOT(slotDisplayError(QString,QString,QStringList)) );
     connect(PBI,SIGNAL(devMessage(QString)), ui->text_dev_output, SLOT(append(QString)) );
   //Make sure we start on the installed tab
   ui->tabWidget->setCurrentWidget(ui->tab_browse);

   //In the initialization phase, this should already have the installed/repo info available
   slotRefreshInstallTab();
   slotEnableBrowser();
}

void MainUI::slotSingleInstance(){
  this->raise();
  this->showNormal();
  this->activateWindow();
}

void MainUI::closeEvent(QCloseEvent *event){
  bool safe = PBI->safeToQuit();
  if(!safe){
    //Verify that they want to continue
    QMessageBox::StandardButton button = QMessageBox::warning(this, tr("AppCafe Processes Running"), tr("The AppCafe currently has actions pending. Do you want to cancel all running processes and quit anyway?"), QMessageBox::Yes | QMessageBox::Cancel,QMessageBox::Cancel);
    if(button == QMessageBox::Yes){ //close down
      PBI->cancelActions( PBI->installedList() ); //close down safely
    }else{
      event->ignore();
      return;
    }
  }
  this->close();
}
// ========================
// ===== MENU OPTIONS =====
// ========================
void MainUI::on_actionImport_PBI_List_triggered(){
  QString file = QFileDialog::getOpenFileName( this, tr("Import PBI File List"), QDir::homePath(), tr("PBI List (*.pbilist)"));
  if(file.isEmpty()){ return; } //action cancelled
  bool ok = PBI->importPbiListFromFile(file);
  if(!ok){ qDebug() << QMessageBox::warning(this,tr("Import Error"),tr("There was an error importing the PBI list")+"\n"+tr("Please make sure that the file has not been corrupted and try again")); }
}

void MainUI::on_actionExport_PBI_List_triggered(){
  QString file = QFileDialog::getSaveFileName( this, tr("Export PBI File List"), QDir::homePath()+"/exportfile.pbilist", tr("PBI List (*.pbilist)"));
  if(file.isEmpty()){ return; } //action cancelled
  bool ok = PBI->exportPbiListToFile(file);
  if(!ok){ qDebug() << QMessageBox::warning(this,tr("Export Error"),tr("There was an error exporting the PBI list")+"\n"+tr("Please make sure that you have the proper directory permissions and try again")); }
}

void MainUI::on_actionQuit_triggered(){
  this->close();
}

void MainUI::on_actionAppCafe_Settings_triggered(){
  //PBI->openConfigurationDialog();
}

void MainUI::on_actionDeveloper_Mode_triggered(){
  ui->text_dev_output->setVisible(ui->actionDeveloper_Mode->isChecked());
}

// =========================
// ===== INSTALLED TAB =====
// =========================
void MainUI::initializeInstalledTab(){
  //Setup the action menu for installed applications
  actionMenu = new QMenu();
    QMenu *dmenu = actionMenu->addMenu( QIcon(":icons/xdg_desktop.png"), tr("Desktop Icons"));
      dmenu->addAction( QIcon(":icons/add.png"),tr("Add"),this,SLOT(slotActionAddDesktop()) );
      dmenu->addAction( QIcon(":icons/remove.png"),tr("Remove"),this,SLOT(slotActionRemoveDesktop()) );
    actionMenu->addSeparator();
    actionMenu->addAction( QIcon(":icons/remove.png"), tr("Uninstall"), this, SLOT(slotActionRemove()) );
    actionMenu->addSeparator();
    actionMenu->addAction( QIcon(":icons/dialog-cancel.png"), tr("Cancel Actions"), this, SLOT(slotActionCancel()) );
  //Setup the shortcuts menu for installed applications
    sDeskMenu = new QMenu(this);
      sDeskMenu->addAction( QIcon(":icons/add.png"),tr("Add Desktop Shortcuts"),this,SLOT(slotActionAddDesktop()) );
      sDeskMenu->addAction( QIcon(":icons/remove.png"),tr("Remove Desktop Shortcuts"),this,SLOT(slotActionRemoveDesktop()) );
  ui->tool_install_shortcuts->setMenu(sDeskMenu);
  //Setup the binary menu for installed applications
  appBinMenu = new QMenu();
  ui->tool_install_startApp->setMenu(appBinMenu);
    connect(appBinMenu, SIGNAL(triggered(QAction*)), this, SLOT(slotStartApp(QAction*)) );
  //Initialize the context menu
  contextActionMenu = new QMenu(this);
    connect(contextActionMenu, SIGNAL(aboutToHide()), this, SLOT(contextMenuFinished()) );
  //Now setup the action button
  ui->tool_install_performaction->setMenu(actionMenu);
  ui->tool_install_performaction->setPopupMode(QToolButton::InstantPopup);
  //Now setup any defaults for the installed tab
  ui->tree_install_apps->setIconSize(QSize(22,22));
  ui->tree_install_apps->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(ui->tree_install_apps, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(slotCheckSelectedItems()) );
  connect(ui->tree_install_apps, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT( slotInstalledAppRightClicked(const QPoint &)) );
  slotRefreshInstallTab();
}

void MainUI::formatInstalledItemDisplay(QTreeWidgetItem *item){
  //simplification function for filling the tree widget item with the appropriate information about the PBI
  QString ID = item->whatsThis(0);
  NGApp app = PBI->singleAppInfo(ID);
  if(app.origin.isEmpty()){ return; } //invalid item
  if(item->text(0).isEmpty()){  //new entry - get everything
    //Fill the item columns [name, version, status]
      item->setText(0,app.name);
      item->setText(1,app.installedversion);
      item->setText(2, PBI->currentAppStatus(ID));
      //for(int i=0; i<vals.length(); i++){ item->setText(i,vals[i]); }
      QString icon = app.icon;
        //Load a default icon if none found
      if(icon.isEmpty() || !QFile::exists(icon) ){ icon = defaultIcon; }
      item->setIcon(0,QIcon(icon) );
      item->setCheckState(0,Qt::Unchecked);
  }else{ // Just update the necesary info
    item->setText(1, app.installedversion);
    item->setText(2, PBI->currentAppStatus(ID) );
  }
}

QStringList MainUI::getCheckedItems(){
  //Return the pbiID's of all the active items
  QStringList output;  
  //Check for whether this is the context menu on the main widget
  if(!cDetails.isEmpty()){
     output << cDetails;
     cDetails.clear();
	  
  //If on the main Installed page, look for checked items only
  }else{
    for(int i=0; i<ui->tree_install_apps->topLevelItemCount(); i++){
      if(ui->tree_install_apps->topLevelItem(i)->checkState(0) == Qt::Checked){
        output << ui->tree_install_apps->topLevelItem(i)->whatsThis(0);
      }
    }
  }
  //qDebug() << "Checked Items:" << output;
  return output;	
}

// === SLOTS ===
void MainUI::slotRefreshInstallTab(){
  //Update the list of installed PBI's w/o clearing the list (loses selections)
   //Get the list we need (in order)
  QStringList installList = PBI->installedList();
  installList.append( PBI->pendingInstallList() );
  installList.removeDuplicates();
  //Quick finish if no items installed/pending
  if(installList.isEmpty()){
    ui->tree_install_apps->clear();
    return;
  }
  //Get the list we have now and handle items as needed
  QStringList cList;
  for(int i=0; i<ui->tree_install_apps->topLevelItemCount(); i++){
    QString item = ui->tree_install_apps->topLevelItem(i)->whatsThis(0);
    //Update item if necessary
    if(installList.contains(item)){ 
	formatInstalledItemDisplay( ui->tree_install_apps->topLevelItem(i) ); 
	installList.removeAll(item); //Remove it from the list - since already handled
    //Remove item if necessary
    }else{
      delete ui->tree_install_apps->takeTopLevelItem(i);
      i--; //make sure to back up once to prevent missing the next item
    }
  }
  //Now add any new items to the list
  for(int i=0; i<installList.length(); i++){
    QTreeWidgetItem *item = new QTreeWidgetItem; //create the item
	//qDebug() << "New Item:" << installList[i];
        item->setWhatsThis(0,installList[i]);
        //Now format the display
        formatInstalledItemDisplay(item);
	if(item->text(0).isEmpty()){
	  //Do not put empty items into the display
	  delete item;
	}else{
          //Now insert this item onto the list
          ui->tree_install_apps->insertTopLevelItem(i,item);
	}
  }
  //Make sure that there is an item selected
  if(ui->tree_install_apps->topLevelItemCount() > 0 ){
    if( ui->tree_install_apps->selectedItems().isEmpty() ){
      ui->tree_install_apps->setCurrentItem( ui->tree_install_apps->topLevelItem(0) );
    }
    //Now re-size the columns to the minimum required width
    for(int i=0; i<3; i++){
      ui->tree_install_apps->resizeColumnToContents(i);
    } 
  }
  //slotUpdateSelectedPBI();; //Update the info boxes
  slotDisplayStats();
  slotCheckSelectedItems();
  //If the browser app page is currently visible for this app
  if( (ui->stacked_browser->currentWidget() == ui->page_app) && ui->page_app->isVisible() ){
    slotGoToApp(cApp);
  }
}

void MainUI::slotCheckSelectedItems(){
  bool chkd = false;
  for(int i=0; i<ui->tree_install_apps->topLevelItemCount(); i++){
    if(ui->tree_install_apps->topLevelItem(i)->checkState(0) == Qt::Checked){
      chkd = true; break;
    }
  }
  ui->tool_install_performaction->setEnabled(chkd);
  cDetails.clear(); //Make sure this is cleared since context menu not open
}

void MainUI::slotPBIStatusUpdate(QString pbiID){
  //This will do a full update of a particlar PBI entry
  //	and just update/check the icons for all the other PBI's
  for(int i=0; i<ui->tree_install_apps->topLevelItemCount(); i++){
    QString itemID = ui->tree_install_apps->topLevelItem(i)->whatsThis(0);
    if(itemID == pbiID){
      QString stat = PBI->currentAppStatus(pbiID);
      ui->tree_install_apps->topLevelItem(i)->setText(2,stat);
    }
  }
  //If the browser app page is current for this app
  if( (ui->stacked_browser->currentWidget() == ui->page_app) && (cApp == pbiID) && ui->page_app->isVisible() ){
    slotUpdateAppDownloadButton();
  }
}

void MainUI::on_tool_install_details_clicked(){
  //Get the current item
  QString appID;
  if(ui->tree_install_apps->topLevelItemCount() > 0){
    appID = ui->tree_install_apps->currentItem()->whatsThis(0);
  }
  if(appID.isEmpty()){return;}
  slotGoToApp(appID);
}

void MainUI::on_tool_install_toggleall_clicked(){
  //Determine if the current item is checked or unchecked
  bool checkall = (ui->tree_install_apps->currentItem()->checkState(0) == Qt::Unchecked);
  for(int i=0; i<ui->tree_install_apps->topLevelItemCount(); i++){
    if(checkall){ ui->tree_install_apps->topLevelItem(i)->setCheckState(0,Qt::Checked); }
    else{ui->tree_install_apps->topLevelItem(i)->setCheckState(0,Qt::Unchecked); }
  }
  slotCheckSelectedItems();
}

void MainUI::on_tree_install_apps_itemSelectionChanged(){
  //When an installed PBI is clicked on
  slotCheckSelectedItems();
}

void MainUI::on_tree_install_apps_itemDoubleClicked(QTreeWidgetItem *item){
 //Make sure it is a valid/installed application
 QString appID = item->whatsThis(0);
   if( !PBI->isInstalled(appID) ){ return; }
  qDebug() << "Item Double Clicked:" << appID;
  //Update the info on the details page
  slotGoToApp(appID);
}

void MainUI::on_tool_install_maintainer_clicked(){
  //Get the current item
  QString appID = cApp;
  //Get the maintainer email
  NGApp app = PBI->singleAppInfo(appID);
  QString email = app.maintainer;
  if(email.isEmpty()){ return; }
  //Verify that the user wants to launch their email client
  if(QMessageBox::Yes != QMessageBox::question(this, tr("Launch Email Client?"), tr("Do you want to try launching your default email client? \n You must have this setup within your current desktop environment for this to work properly. If not, you can send an email to the address below manually.")+"\n\n"+email, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) ){
    return;
  }
  qDebug() << "Launching email to:" << email;
  //Get the command from the action
  QString cmd = "mailto:"+email;
  //QStringList info = PBI->PBIInfo(appID, QStringList() << "name" << "date" << "version" << "arch" << "fbsdversion" );
  //Add a sample subject
  cmd.append("?subject="+app.origin+" port question");
  //Add the info to the body of the email
  cmd.append("&body=");
  cmd.append("-----------\nPBI Information:\nName: "+app.name + "\nDate Installed: "+app.installedwhen +"\nVersion: "+app.installedversion );
  //Startup the command externally
  PBI->runCmdAsUser("xdg-open \""+cmd+"\"");
}

void MainUI::slotInstalledAppRightClicked(const QPoint &pt){
  //Get the item under the mouse click
  QTreeWidgetItem *it = ui->tree_install_apps->itemAt(pt);
  if(it==0){ return; } // no item selected
  QString pbiID = it->whatsThis(0);
  qDebug() << "Get context menu for:" << pbiID;
  //Now Update the context menu appropriately
  NGApp info = PBI->singleAppInfo(pbiID);
  //QStringList info = PBI->PBIInfo(pbiID, QStringList() << "hasdesktopicons" << "hasmenuicons" << "hasmimetypes");
  if( info.origin.isEmpty() ){ return; } //invalid application
  bool pending = PBI->isWorking(pbiID);
  contextActionMenu->clear();
  if( (info.version != info.installedversion) &&  !pending){
    //Upgrade is only available if actions not pending
    contextActionMenu->addAction( QIcon(":icons/view-refresh.png"), tr("Update"), this, SLOT(slotActionUpdate()) );
    contextActionMenu->addSeparator();
  }
  if(info.hasDE){
    QMenu *dmenu = contextActionMenu->addMenu( QIcon(":icons/xdg_desktop.png"), tr("Desktop Icons"));
      dmenu->addAction( QIcon(":icons/add.png"),tr("Add"),this,SLOT(slotActionAddDesktop()) );
      dmenu->addAction( QIcon(":icons/remove.png"),tr("Remove"),this,SLOT(slotActionRemoveDesktop()) );
  }
  if(!pending){
    //Remove option is only available if not currently pending actions
    contextActionMenu->addSeparator();
    contextActionMenu->addAction( QIcon(":icons/remove.png"), tr("Uninstall"), this, SLOT(slotActionRemove()) );
  }else{
    //Cancel option is only available if actions are currently pending	  
    contextActionMenu->addSeparator();
    contextActionMenu->addAction( QIcon(":icons/dialog-cancel.png"), tr("Cancel Actions"), this, SLOT(slotActionCancel()) );
  }
  //Now show the menu
  cDetails = pbiID; //save this so we know which app is currently being modified
  contextActionMenu->popup(ui->tree_install_apps->mapToGlobal(pt));
}

void MainUI::contextMenuFinished(){
  QTimer::singleShot(500, this, SLOT(slotCheckSelectedItems()) );	
}

// === SELECTED PBI ACTIONS ===
void MainUI::slotActionAddDesktop(){
  QStringList items = getCheckedItems();
  PBI->addDesktopIcons(items,FALSE); //only for current user
}

void MainUI::slotActionRemoveDesktop(){
  QStringList items = getCheckedItems();
  PBI->rmDesktopIcons(items,FALSE);  //Only for current user
}

void MainUI::slotActionRemove(){
  QStringList checkedID = getCheckedItems();
  if(!checkedID.isEmpty()){
    //Verify that the user really wants to remove these apps
    checkedID = generateRemoveMessage(checkedID);
    if( !checkedID.isEmpty() ){
      PBI->removePBI(checkedID);
    }
  }
}

void MainUI::slotActionCancel(){
  QStringList checkedID = getCheckedItems();
  if(!checkedID.isEmpty()){
    PBI->cancelActions(checkedID);  
  }
}

void MainUI::slotStartApp(QAction* act){
  qDebug() << "Starting external application:" << act->text();
  //Get the command from the action
  QString desktopfile = act->whatsThis();
  QString cmd = "xdg-open "+desktopfile;
  //Startup the command externally
  PBI->runCmdAsUser(cmd);
}

// ==========================
// ====== BROWSER TAB =======
// ==========================
void MainUI::initializeBrowserTab(){
  ui->tab_browse->setEnabled(FALSE);
  //Always make sure that the browser starts off on the "home" page
  ui->stacked_browser->setCurrentWidget(ui->page_home);
  //With invisible shortcut buttons
  ui->tool_browse_cat->setVisible(FALSE);
  ui->tool_browse_app->setVisible(FALSE);
  //Clear any items left over from the designer form
  clearScrollArea(ui->scroll_br_home_newapps);
  clearScrollArea(ui->scroll_br_home_rec);
  //Search functionality
  searchTimer = new QTimer();
    searchTimer->setSingleShot(TRUE);
    searchTimer->setInterval(300); // 0.3 sec wait before a search
    connect(searchTimer,SIGNAL(timeout()),this,SLOT(slotGoToSearch()) );
  connect(ui->tool_browse_search,SIGNAL(clicked()),this,SLOT(slotGoToSearch()) );
  connect(ui->line_browse_searchbar,SIGNAL(returnPressed()),this,SLOT(slotGoToSearch()) );
  connect(ui->tool_browse_gotocat, SIGNAL(clicked()), this, SLOT(slotGoToCatBrowser()) );
  //Setup the jail menu title widget
  jailLabel = new QLabel(" <b>"+tr("Install in jail:")+"</b> ", this);
	jailLabel->setAlignment(Qt::AlignCenter);
  jailAction = new QWidgetAction(this);
	jailAction->setDefaultWidget(jailLabel);
  //Setup jail install menu
  jailMenu = new QMenu(this);
  connect(jailMenu, SIGNAL(triggered(QAction*)), this, SLOT(installAppIntoJail(QAction*)) );
  //Setup the back button menu
  backMenu = new QMenu(this);
  connect(backMenu, SIGNAL(triggered(QAction*)), this, SLOT(slotBackToApp(QAction*)) );
  ui->tool_br_back->setVisible(false); //no apps to go back to yet
  ui->tool_br_back->setMenu(backMenu);
}

// === SLOTS ===
void MainUI::slotDisableBrowser(bool shownotification){
  if(shownotification){ qDebug() << "No Repo Available: De-activating the Browser"; }
  ui->tabWidget->setCurrentWidget(ui->tab_installed);
  ui->tab_browse->setEnabled(FALSE);
  slotDisplayStats();
}

void MainUI::slotEnableBrowser(){
  qDebug() << "Repo Ready: - generating browser home page";
  //Now create the browser home page
  slotUpdateBrowserHome();
  //And allow the user to go there
  ui->tab_browse->setEnabled(TRUE);
  slotDisplayStats();
}

void MainUI::slotUpdateBrowserHome(){
  //Load the Recommendations
  ui->group_br_recommend->setVisible( fillVerticalAppArea(ui->scroll_br_home_rec, PBI->getRecommendedApps(), false) );
  //Load the application spotlight
  ui->group_br_home_spotlight->setVisible( fillVerticalAppArea(ui->scroll_br_home_spot, PBI->getHighlightedApps(), false) );
  //Load the newest applications
  clearScrollArea(ui->scroll_br_home_newapps);
  QHBoxLayout *newapplayout = new QHBoxLayout;
  QStringList newapps = PBI->getNewApps();
  QList<NGApp> apps = PBI->AppInfo(newapps);
  for(int i=0; i<apps.length(); i++){
    //QStringList appdata = PBI->AppInfo(newapps[i],QStringList() << "name" << "icon" << "latestversion");
    //if(!appdata.isEmpty()){
      SmallItemWidget *item = new SmallItemWidget(apps[i].origin, apps[i].name, checkIcon(apps[i].icon, apps[i].type), apps[i].version);
      connect(item,SIGNAL(appClicked(QString)),this,SLOT(slotGoToApp(QString)) );
      newapplayout->addWidget(item);
    //}
  }
  newapplayout->addStretch(); //add a spacer to the end
  newapplayout->setContentsMargins(0,0,0,0);
  newapplayout->setSpacing(0);
  ui->scroll_br_home_newapps->widget()->setLayout(newapplayout);
  //Make sure that the newapps scrollarea is the proper fit vertically (no vertical scrolling)
  ui->scroll_br_home_newapps->setMinimumHeight(ui->scroll_br_home_newapps->widget()->minimumSizeHint().height());
  
  //Make sure the new apps area is invisible if no items available
  if(newapps.isEmpty()){ ui->group_br_home_newapps->setVisible(FALSE); }
  else{ ui->group_br_home_newapps->setVisible(TRUE); }
  //make sure the home page is visible in the browser (slotGoToHome without changing tabs)
  ui->stacked_browser->setCurrentWidget(ui->page_home);	
  //Make sure the shortcut buttons are disabled
  ui->tool_browse_cat->setVisible(FALSE);
  ui->tool_browse_app->setVisible(FALSE); 
  
  //Now update the category browser page (since it only needs to be done once like the home menu)
  //Load the Categories
  QStringList catlist = PBI->browserCategories();
  catlist.sort();
  QList<NGCat> cats = PBI->CatInfo(catlist); //all categories
    clearScrollArea(ui->scroll_br_cats);
    QVBoxLayout *catlayout = new QVBoxLayout;
    for(int i=0; i<cats.length(); i++){
        LargeItemWidget *item = new LargeItemWidget(this,cats[i]);
        connect(item,SIGNAL(appClicked(QString)),this,SLOT(slotGoToCategory(QString)) );
        catlayout->addWidget(item);
    }
    catlayout->addStretch(); //add a spacer to the end
    ui->scroll_br_cats->widget()->setLayout(catlayout);
}

void MainUI::slotGoToHome(){
  ui->tool_br_back->setVisible(!backApps.isEmpty());
  ui->tabWidget->setCurrentWidget(ui->tab_browse);
  ui->stacked_browser->setCurrentWidget(ui->page_home);	
  //Make sure the shortcut buttons are disabled
  ui->tool_browse_cat->setVisible(false);
  ui->tool_browse_app->setVisible(false);
  ui->tool_browse_gotocat->setVisible(true);
}

void MainUI::slotGoToCatBrowser(){
  ui->tool_br_back->setVisible(!backApps.isEmpty());
  ui->tabWidget->setCurrentWidget(ui->tab_browse);
  ui->stacked_browser->setCurrentWidget(ui->page_browsecats);
  //Make sure the shortcut buttons are diabled
  ui->tool_browse_cat->setVisible(false);
  ui->tool_browse_app->setVisible(false);
  ui->tool_browse_gotocat->setVisible(false);
}

void MainUI::slotGoToCategory(QString cat){
  qDebug() << "Show Category:" << cat;
  ui->tool_br_back->setVisible(!backApps.isEmpty());
  //Get the apps in this category
  QStringList applist = PBI->browserApps(cat);
    applist.sort();
  QList<NGApp> apps = PBI->AppInfo(applist);
  if( !fillVerticalAppArea(ui->scroll_br_cat_apps, applist, true) ){
    ui->label_br_cat_empty->setVisible(true);
  }else{
    ui->label_br_cat_empty->setVisible(false);
  }

  //Now enable/disable the shortcut buttons
  ui->tool_browse_app->setVisible(false);
  ui->tool_browse_cat->setVisible(false);
  ui->tool_browse_gotocat->setVisible(true);
  //Setup the icon/name for the category display
    NGCat catinfo = PBI->singleCatInfo(cat);
    //QStringList catinfo = PBI->CatInfo(cat,QStringList() << "name" << "icon");
    ui->tool_browse_cat->setText(catinfo.name);
    ui->label_cat_name->setText(catinfo.name);
    if(catinfo.icon.isEmpty() || !QFile::exists(catinfo.icon) ){ catinfo.icon = defaultIcon; }
    ui->tool_browse_cat->setIcon(QIcon(catinfo.icon));
    ui->label_cat_icon->setPixmap( QPixmap(catinfo.icon).scaled(32,32) ); 
  //Now move to the page
  ui->tabWidget->setCurrentWidget(ui->tab_browse);
  ui->stacked_browser->setCurrentWidget(ui->page_cat);
  //Now save that this category is currently displayed
  cCat = cat;
}

void MainUI::slotGoToApp(QString appID, bool goback){
  qDebug() << "Show App:" << appID;
  //Get the general application info
  NGApp data = PBI->singleAppInfo(appID);
  if(data.origin.isEmpty()){
    qDebug() << "Invalid App:" << appID;
    return;
  }
  //Save this appID for going back later
  if(!goback){ //is not a return to an old page
    if(backApps.length() > 0){
      backApps.removeAll(appID);
      backApps.prepend(appID);
    }else{
      backApps.prepend(appID);
    }
    //Now rebuild the back menu
    backMenu->clear();
    for(int i=0; i<backApps.length() && i<10; i++){ //only show 10 items max
      backMenu->addAction(backApps[i]);
    }
  }
  ui->tool_br_back->setVisible( backApps.length() > 1 );
  cApp = appID; //save this for later
  //Start the search for similar apps
  PBI->searchSimilar = appID;
  ui->tabWidget_browse_info->setTabEnabled(3, false); //similar apps tab
  QTimer::singleShot(0,PBI,SLOT(startSimilarSearch()));
  //Now Check icon
  //qDebug() << "App Icon:" << data.origin << data.icon << data.type;
  data.icon = checkIcon(data.icon, data.type); //if(data.icon.isEmpty() || !QFile::exists(data.icon)){ data.icon = defaultIcon; }
  //qDebug() << " - fixed icon:" << data.icon;
  //Now fill the UI with the data
  ui->label_bapp_name->setText(data.name);
  ui->label_bapp_icon->setPixmap(QPixmap(data.icon).scaled(ui->label_bapp_icon->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation) );
  ui->label_bapp_authorweb->setText(data.author);
  ui->tool_app_openweb->setWhatsThis(data.website);
  ui->tool_app_openweb->setVisible( !data.website.isEmpty() && !(data.website.toLower()=="unknown") );
  ui->label_bapp_authorweb->setToolTip(data.website);
  ui->label_bapp_license->setText(data.license);
  ui->label_bapp_type->setText(data.type);
  ui->text_bapp_description->setText(data.description);
  if(data.rating.isEmpty()){
    ui->tool_app_rank->setText("?? / 5");
  }else{
    ui->tool_app_rank->setText( data.rating+" / 5");
  }
  QString cVer = data.installedversion;
    ui->label_bapp_version->setText(data.version);
    ui->label_bapp_arch->setText(data.arch);
    if(data.size.isEmpty()){ ui->label_bapp_size->setText(tr("Unknown")); }
    else{ ui->label_bapp_size->setText( data.size ); }
  //Now update the download button appropriately
  slotUpdateAppDownloadButton();
  ui->group_app_installed->setVisible(data.isInstalled);
  if(data.isInstalled){
    //Now update the application buttons
    //Start Application binaries
	QStringList bins = PBI->appBinList(appID);
        appBinMenu->clear();
        for(int i=0; i<bins.length(); i++){
          QAction *act = new QAction(this);
	    act->setText(bins[i].section("::::",0,0)); //set name
	    act->setWhatsThis(bins[i].section("::::",1,10)); //set command string
          appBinMenu->addAction(act);
        }
	ui->tool_install_startApp->setVisible(!appBinMenu->isEmpty());
    //Maintainer button
      ui->tool_install_maintainer->setVisible( data.maintainer.contains("@") );
    //Shortcuts button
      ui->tool_install_shortcuts->setVisible(data.hasDE);
	
  }
  //Now enable/disable the shortcut buttons
  ui->tool_browse_app->setVisible(TRUE);
    ui->tool_browse_app->setText(data.name);
    ui->tool_browse_app->setIcon(QIcon(data.icon));
    bApp = appID; //button app ID
  NGCat catinfo;
  if(!data.category.isEmpty()){ catinfo = PBI->singleCatInfo(data.category); }
  else{ catinfo = PBI->singleCatInfo(data.portcat); }
    bCat = catinfo.portcat; //current button category ID
  //QStringList catinfo = PBI->CatInfo(Extras::nameToID(data[7]),QStringList() << "name" << "icon");
  //qDebug() << "Show App Category:" << bCat;
  if(!catinfo.name.isEmpty()){
    ui->tool_browse_gotocat->setVisible(false);
    ui->tool_browse_cat->setVisible(TRUE);
    ui->tool_browse_cat->setText(catinfo.name);
    if(catinfo.icon.isEmpty() || !QFile::exists(catinfo.icon) ){ catinfo.icon = defaultIcon; }
    ui->tool_browse_cat->setIcon(QIcon(catinfo.icon));
  }
  ui->tabWidget->setCurrentWidget(ui->tab_browse);
  ui->stacked_browser->setCurrentWidget(ui->page_app);
  ui->tabWidget_browse_info->setCurrentWidget(ui->tab_app_desc);
  //Screenshots tab
  if(data.screenshots.isEmpty()){
    ui->tabWidget_browse_info->setTabEnabled(1,false);
  }else{
    ui->tabWidget_browse_info->setTabEnabled(1,true);
    //still need to load the first screenshot
    showScreenshot(0);
  }
  //Plugins tab
  //qDebug() << "plugins:" << data.possiblePlugins;
  ui->tabWidget_browse_info->setTabEnabled(2, fillVerticalAppArea( ui->scroll_app_plugins, data.possiblePlugins, false));
  //Build Options tab
  //qDebug() << "Build Options:" << data.buildOptions;
  if(data.buildOptions.isEmpty()){
    ui->tabWidget_browse_info->setTabEnabled(4,false);
  }else{
    ui->tabWidget_browse_info->setTabEnabled(4,true);
    ui->list_app_buildopts->clear();
    ui->list_app_buildopts->addItems(data.buildOptions);
  }
	
}

void MainUI::slotBackToApp(QAction* act){
  slotGoToApp(act->text(), true);	
}

void MainUI::slotUpdateAppDownloadButton(){
  QString ico;
  QStringList goodjails = PBI->jailsWithoutPkg(cApp);
  if( PBI->isWorking(cApp) ){ //app currently pending or actually doing something
    ui->tool_bapp_download->setText( PBI->currentAppStatus(cApp) );
    ui->tool_bapp_download->setIcon(QIcon(":icons/working.png"));
    ui->tool_bapp_download->setEnabled(false);
  }else if( !PBI->isInstalled(cApp) ){ //new installation
    ui->tool_bapp_download->setText(tr("Install Now!"));
    ico = ":icons/app_download.png";
    ui->tool_bapp_download->setEnabled(true);
  }else{ //already installed (no downgrade available)
    ui->tool_bapp_download->setText(tr("Installed"));
    ui->tool_bapp_download->setIcon(QIcon(":icons/dialog-ok.png"));
    ui->tool_bapp_download->setEnabled(!goodjails.isEmpty()); //only disable if no jail menu
  }
  //Now set the icon appropriately if it requires root permissions
  if(!ico.isEmpty()){
    ui->tool_bapp_download->setIcon(QIcon(ico));
  }
  ui->tool_bapp_download->setWhatsThis(cApp); //set for slot
  //Now set the button menu appropriately
  if(goodjails.isEmpty()){
    ui->tool_bapp_download->setMenu(0); //remove the menu
    ui->tool_bapp_download->setPopupMode( QToolButton::DelayedPopup );
  }else{
    //Re-create the menu with the valid jails
    jailMenu->clear();
      jailMenu->addAction( jailAction );
      jailMenu->addSeparator();
      for(int i=0; i<goodjails.length(); i++){
        jailMenu->addAction(goodjails[i]);
      }
    //now add the menu to the button
    ui->tool_bapp_download->setMenu(jailMenu); 
    ui->tool_bapp_download->setPopupMode( QToolButton::MenuButtonPopup );
  }
}

void MainUI::slotGoToSearch(){
  searchTimer->stop(); //just in case "return" was pressed to start the search
  QString search = ui->line_browse_searchbar->text();
  if(search.isEmpty()){ return; }
  PBI->searchTerm = search;
  QTimer::singleShot(1,PBI,SLOT(startAppSearch()));
  ui->label_bsearch_info->setText( tr("Searching the application database. Please Wait....") );
    ui->label_bsearch_info->setVisible(TRUE);
    ui->group_bsearch_best->setVisible(FALSE);
    ui->group_bsearch_other->setVisible(FALSE);
  
}
	
void MainUI::slotShowSimilarApps(QStringList apps){
  qDebug() << " - Similar applications:" << apps;
  ui->tabWidget_browse_info->setTabEnabled(3, fillVerticalAppArea( ui->scroll_app_similar, apps, true) );
    //ui->page_app->updateGeometry();
}

void MainUI::slotShowSearchResults(QStringList best, QStringList rest){
  //Now display the search results
  if( !fillVerticalAppArea( ui->scroll_bsearch_best, best, true) ){
    ui->label_bsearch_info->setText( QString(tr("No Search Results Found for the term: %1")).arg(ui->line_browse_searchbar->text()) );
    ui->label_bsearch_info->setVisible(TRUE);
    ui->group_bsearch_best->setVisible(FALSE);
    ui->group_bsearch_other->setVisible(FALSE);
  }else{
    ui->label_bsearch_info->setVisible(FALSE);
    ui->group_bsearch_best->setVisible(TRUE);
    //Now fill the other results
    ui->group_bsearch_other->setVisible( fillVerticalAppArea( ui->scroll_bsearch_other, rest, true) );
  }
	
  //need to make sure the search bar still has keyboard focus (just in case)
  ui->tabWidget->setCurrentWidget(ui->tab_browse);
  ui->stacked_browser->setCurrentWidget(ui->page_search);
  
}

void MainUI::on_tabWidget_currentChanged(){
  if(ui->tabWidget->currentWidget() == ui->tab_browse){
    //Refresh the app page if that is the one currently showing
    if(ui->stacked_browser->currentWidget() == ui->page_app){ on_tool_browse_app_clicked(); }	  
  }
}

void MainUI::on_tool_browse_home_clicked(){
  slotGoToHome();
}

void MainUI::on_tool_browse_cat_clicked(){
  if(cCat == bCat){ //already loaded - just show it (prevents resetting scroll positions)
    ui->stacked_browser->setCurrentWidget(ui->page_cat);
    //Now enable/disable the shortcut buttons
    ui->tool_browse_app->setVisible(false);
    ui->tool_browse_cat->setVisible(false);
    ui->tool_browse_gotocat->setVisible(true);
  }else{ //load and show the category
    slotGoToCategory(bCat);  //button cat id
  }
}

void MainUI::on_tool_browse_app_clicked(){
  slotGoToApp(bApp); //button app id
}

void MainUI::on_line_browse_searchbar_textChanged(){
  //Connect this to a singleshot timer, so the search functionality is only
  //  run once after a short wait rather than every time a new character is typed
  
  //Live search only after 3 characters have been typed
  if(ui->line_browse_searchbar->text().length() > 2){
    searchTimer->start();
  }else{
    searchTimer->stop();	  
  }
}

void MainUI::on_tool_bapp_download_clicked(){
  QString appID = ui->tool_bapp_download->whatsThis();
  if(PBI->isInstalled(appID)){ return; } //do nothing if already installed
  //Verify the app installation
  if( QMessageBox::Yes != QMessageBox::question(this,tr("Verify Installation"), tr("Are you sure you want to install this application?")+"\n\n"+appID,QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes)){
    return;
  }
  PBI->installApp(QStringList() << appID);
  ui->tool_bapp_download->setEnabled(FALSE); //make sure it cannot be clicked more than once before page refresh
  //Now show the Installed tab
  //ui->tabWidget->setCurrentWidget(ui->tab_installed);
}

void MainUI::on_group_br_home_newapps_toggled(bool show){
  ui->scroll_br_home_newapps->setVisible(show);
}

void MainUI::on_tool_app_nextScreen_clicked(){
  //Read the current screenshot and go to the previous one
  int cur = ui->label_app_cScreen->text().section("/",0,0).simplified().toInt();
  showScreenshot(cur); //the viewable number is always 1 greater than the actual number
}

void MainUI::on_tool_app_prevScreen_clicked(){
  //Read the current screenshot and go to the previous one
  int cur = ui->label_app_cScreen->text().section("/",0,0).simplified().toInt();
  showScreenshot(cur-2); //the viewable number is always 1 greater than the actual number	
}

void MainUI::on_tool_app_rank_clicked(){
  qDebug() << "Open PC-BSD wiki ratings page";
  QString url = "http://wiki.pcbsd.org/index.php/AppCafe/"+cApp;
  PBI->runCmdAsUser("xdg-open \'"+url+"\'");
}

void MainUI::on_tool_app_openweb_clicked(){
  qDebug() << "Open application website";
  QString url = ui->tool_app_openweb->whatsThis();
  PBI->runCmdAsUser("xdg-open \'"+url+"\'");
}

void MainUI::on_tool_br_back_clicked(){
  if(backApps.length() <= 0){ return; }
  QString app;
  if(ui->stacked_browser->currentWidget()==ui->page_app){
    //make sure to get the one after the currently showing app
    for(int i=0; i<backApps.length(); i++){
      if(backApps[i]==cApp && (i+1)<backApps.length() ){
        app = backApps[i+1];
	break;
      }
    }
  }
  if(app.isEmpty()){
    //just get the first app in the back list (the most recent)
    app = backApps[0];
  }
  slotGoToApp(app, true);
}

void MainUI::browserViewSettingsChanged(){
  //Update the currently visible browser page if necessary
  QWidget* page = ui->stacked_browser->currentWidget();
  if(page == ui->page_cat){
    slotGoToCategory(cCat);
  }else if(page == ui->page_search){
    slotGoToSearch();
  }  
}

void MainUI::installAppIntoJail(QAction *act){
  QString jail = act->text();
  qDebug() << "Installation into Jail requested:" << cApp << jail;
  qDebug() << "Check for current jail package not implemented yet";
  PBI->installApp(QStringList() << cApp, jail);
}

// ================================
// ==========  OTHER ==============
// ================================
void MainUI::clearScrollArea(QScrollArea* area){
  QWidget *wgt = area->takeWidget();
  delete wgt; //delete the widget and all children
  area->setWidget( new QWidget() ); //create a new widget in the scroll area
  area->widget()->setContentsMargins(0,0,0,0);
}

bool MainUI::fillVerticalAppArea( QScrollArea* area, QStringList applist, bool filter){
  //clear the scroll area first
  clearScrollArea(area);
  bool ok = false; //returns whether any apps were shown after filtering
  //Re-create the layout
  QVBoxLayout *layout = new QVBoxLayout;
    QList<NGApp> apps = PBI->AppInfo(applist);
    for(int i=0; i<apps.length(); i++){
	bool goodApp = false;
	if(apps[i].type.toLower()=="graphical"){goodApp = ui->actionGraphical_Apps->isChecked(); }
	else if(apps[i].type.toLower()=="text"){goodApp = ui->actionText_Apps->isChecked(); }
	else if(apps[i].type.toLower()=="server"){goodApp = ui->actionServer_Apps->isChecked(); }
	else{goodApp = ui->actionRaw_Packages->isChecked(); }
	if( !filter || goodApp){
          LargeItemWidget *item = new LargeItemWidget(this,apps[i], checkIcon(apps[i].icon, apps[i].type) );
          connect(item,SIGNAL(appClicked(QString)),this,SLOT(slotGoToApp(QString)) );
          layout->addWidget(item); 
	  ok = true;
	}
    }
    layout->addStretch();
    area->widget()->setLayout(layout);
    return ok;
}

void MainUI::slotDisplayError(QString title,QString message,QStringList log){
  QMessageBox *dlg = new QMessageBox(this);
    dlg->setWindowTitle(title);
    dlg->setText(message);
    dlg->setDetailedText(log.join("\n"));
  dlg->show();
}

void MainUI::showScreenshot(int num){
  //Get the screenshots available
  NGApp app = PBI->singleAppInfo(cApp); //currently shown app
  if(app.screenshots.isEmpty()){ return; } //no screenshots available
  if(app.screenshots.length() <= num){ num = 0; } //go to first
  else if(num < 0){ num = app.screenshots.length()-1; } //go to last
  //Get the current screenshot number
  ui->label_app_cScreen->setText( QString::number(num+1)+"/"+QString::number(app.screenshots.length()) );
  //download the file from the URL given and auto-show it
    // - make sure we don't have a request still running, otherwise cancel it
    if(netreply==0){} // do nothing, not initialized yet
    else if(netreply->isRunning()){ netreply->abort(); }
  netreply = netman->get( QNetworkRequest( QUrl(app.screenshots[num]) ) );
  qDebug() << "Start fetching screenshot:" << netreply->url();
  ui->label_app_screenshot->setText( tr("Please wait. Downloading Screenshot.") );
  ui->tool_app_nextScreen->setEnabled(false);
  ui->tool_app_prevScreen->setEnabled(false);
}

void MainUI::slotScreenshotAvailable(QNetworkReply *reply){
  qDebug() << "Screenshot retrieval finished:" << reply->error();
  if(reply->error() == QNetworkReply::NoError){
    QByteArray picdata = reply->readAll();
    QPixmap pix;
      pix.loadFromData(picdata);
    ui->label_app_screenshot->setText(""); //clear the text
    ui->label_app_screenshot->setPixmap( pix.scaled(ui->label_app_screenshot->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation) );
  }else{
    //Network error
    ui->label_app_screenshot->setText( tr("Could not load screenshot (network error)") );
  }
  //Now enable the prev/next buttons as necessary
  QStringList txt = ui->label_app_cScreen->text().split("/");
  if(txt.length()!=2){ return; } //invalid text for some reason
  if(txt[0]!="1"){ ui->tool_app_nextScreen->setEnabled(true); }
  if(txt[0] != txt[1]){ ui->tool_app_prevScreen->setEnabled(true); }
}

QString MainUI::checkIcon(QString icon, QString type){
  QString ico;
  if( !QFile::exists(icon) || icon.isEmpty()){
    if(type.toLower()=="graphical"){ ico = ":/icons/default-graphical.png"; }
    else if(type.toLower()=="text"){ ico = ":/icons/default-text.png"; }
    else if(type.toLower()=="server"){ ico = ":/icons/default-server.png"; }
    else{ ico = ":/icons/default-pkg.png"; }
  }else{
    ico = icon;
  }
  return ico;
}

void MainUI::slotDisplayStats(){
  int avail = PBI->appAvailable;
  int pkg = PBI->pkgAvailable;
  QString text;
  if(avail > 0){
    text = QString(tr("Applications Available: %1")).arg(QString::number(avail))+"\t\t";  
  }
  if(pkg > 0){
    text.append( QString(tr("Total Packages: %1")).arg(QString::number(pkg)) );
  }
  //Get the number of installed/available applications and display it 
  statusLabel->setText(text);	
}

QStringList MainUI::generateRemoveMessage(QStringList apps){

  QString msg = tr("Please verify the following removals:")+"\n\n";
  for(int i=0; i<apps.length(); i++){
    NGApp app = PBI->singleAppInfo(apps[i]);
    if(app.rdependancy.contains("pcbsd-base")){ apps.removeAt(i); i--; continue; } //skip it - cannot remove
    msg.append(app.name+"\n");
    if(!app.rdependancy.isEmpty()){ msg.append( " - "+QString(tr("Also Removes: %1")).arg(app.rdependancy.join(", "))+"\n" ); }
  }
  
  if(apps.isEmpty()){
    QMessageBox::warning(this, tr("Invalid Removal"), tr("These applications are required by the base PC-BSD system and cannot be removed") );
  }else{
    if( QMessageBox::Yes != QMessageBox::question(this,tr("Verify PBI Removal"), msg ,QMessageBox::Yes | QMessageBox::Cancel,QMessageBox::Cancel) ){
      apps.clear();
    }
  }
  return apps;
}
