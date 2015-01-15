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
  connect(ui->tool_app_tips, SIGNAL(clicked()), this, SLOT(on_tool_app_rank_clicked()) ); //same functionality
}

MainUI::~MainUI(){
  delete ui;
  delete PBI;
  delete netreply;
}

void MainUI::ProgramInit()
{ 
   if ( QFile::exists("/tmp/.rebootRequired") )
   {
     qDebug() << "Reboot required!" ;
     QMessageBox::warning(this, tr("Reboot Required"),
                                tr("The system is waiting to reboot from previous updates!\n"
                                   "Please restart the computer before installing new software."),
                                QMessageBox::Ok,
                                QMessageBox::Ok);
     exit(1);
   }
   starting = true;
   QSplashScreen *SS = new QSplashScreen(this, QPixmap(":/icons/splash.png"));
     SS->show();
     QCoreApplication::processEvents();
   qDebug("Application starting...");
   //Now startup the backend
   qDebug() << "Startup Backend";
   QApplication::processEvents();
   PBI = new PBIBackend(this, SS);
   //Initialize the Toolbar buttons
   radio_system = new QRadioButton(tr("System")+" ", this);
     radio_system->setChecked(true);
   radio_jail = new QRadioButton(tr("Jail:"), this);
   combo_jails = new QComboBox(this);
   // - now add them to the toolbar
    ui->toolBar->addWidget(radio_system);
    ui->toolBar->addSeparator();
    ui->toolBar->addWidget(radio_jail);
    ui->toolBar->addWidget(combo_jails);
    ui->toolBar->addAction(ui->actionRefresh_Jails);
    ui->toolBar->addAction(ui->actionCaution);
      connect(radio_system, SIGNAL(clicked()), this, SLOT(installOptionChanged()) );
      connect(radio_jail, SIGNAL(clicked()), this, SLOT(installOptionChanged()) );
      connect(combo_jails, SIGNAL(currentIndexChanged(int)), this, SLOT(installOptionChanged()) );
      connect(ui->actionRefresh_Jails, SIGNAL(triggered()), this, SLOT(slotUpdateJailList()) );
	
   //Initialize the Installed tab
   qDebug() << "Initialize Installed Tab";
   initializeInstalledTab();
   //Initialize the PBI Browser
   qDebug() << "Initialize Browser Tab";
   backApps.clear(); backApps << "Home";
   initializeBrowserTab();


     connect(PBI,SIGNAL(LocalPBIChanges()),this,SLOT(slotRefreshInstallTab()) );
     connect(PBI,SIGNAL(JailListChanged()), this, SLOT(slotUpdateJailList()) );
     connect(PBI,SIGNAL(PBIStatusChange(QString)),this,SLOT(slotPBIStatusUpdate(QString)) );
     connect(PBI,SIGNAL(RepositoryInfoReady()),this,SLOT(slotEnableBrowser()) );
     connect(PBI,SIGNAL(RepositoryInfoReady()),this,SLOT(slotRefreshInstallTab()) );
     connect(PBI,SIGNAL(RepositoryInfoUpdated()),this,SLOT(slotRefreshInstallTab()) );
     connect(PBI,SIGNAL(NoRepoAvailable()),this,SLOT(slotDisableBrowser()) );
     connect(PBI,SIGNAL(SearchComplete(QStringList,QStringList)),this,SLOT(slotShowSearchResults(QStringList, QStringList)) );
     connect(PBI,SIGNAL(SimilarFound(QStringList)),this,SLOT(slotShowSimilarApps(QStringList)) );
     //connect(PBI,SIGNAL(SizeFound(QString)), this, SLOT(slotShowSize(QString)) );
     connect(PBI,SIGNAL(Error(QString,QString,QStringList)),this,SLOT(slotDisplayError(QString,QString,QStringList)) );
     connect(PBI,SIGNAL(devMessage(QString)), ui->text_dev_output, SLOT(append(QString)) );
   //Make sure we start on the installed tab
   ui->tabWidget->setCurrentWidget(ui->tab_browse);

   //In the initialization phase, this should already have the installed/repo info available
   qDebug() << "Set Default Install Option";
   slotUpdateJailList(); //will refresh the entire UI
   //installOptionChanged(); 
   //slotRefreshInstallTab();
   //slotEnableBrowser();
   SS->finish(this);
   starting = false;

}

void MainUI::showJail(QString jailname){
  for(int i=0; i<combo_jails->count(); i++){
    if( combo_jails->itemText(i) == jailname){ 
	radio_jail->setChecked(true); 
	combo_jails->setCurrentIndex(i);  //will trigger the installOptionChanged() slot
	break;
    }
  }
}

void MainUI::slotSingleInstance(){
  if(starting){ return; }
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
      PBI->shutdown(); //close down safely
    }else{
      event->ignore();
      return;
    }
  }else{
    PBI->shutdown(); //close down safely
  }
  //Stop the network processes if necessary
  if(netreply->isRunning()){ netreply->abort(); }
  
  this->close();
}
// ========================
// ===== MENU OPTIONS =====
// ========================
void MainUI::on_actionVulnerability_Info_triggered(){
  qDebug() << "Open Freshports Vulnerability Info page";
  QString url = "http://www.freshports.org/vuxml.php?all";
  PBI->runCmdAsUser("xdg-open \'"+url+"\'");
}

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
  if(PBI->safeToQuit()){
    ConfigDialog dlg(this);
    dlg.exec();
    if(dlg.madeChanges){
      this->setEnabled(false);
      QTimer::singleShot(0, PBI, SLOT(UpdateIndexFiles())	);
    }
  }else{
    QMessageBox::warning(this, tr("Please Wait"), tr("You currently have actions pending/running. Please wait until they finish first.") );
  }
}

void MainUI::on_actionRefresh_PBI_Index_triggered(){
  //Check that no pkg actions are running
  if(PBI->safeToQuit()){
    this->setEnabled(false);
    QTimer::singleShot(0,PBI, SLOT(UpdateIndexFiles()) ); //start the update
  }else{
    QMessageBox::warning(this, tr("Please Wait"), tr("You currently have actions pending/running. Please wait until they finish first.") );
  }	  
}

void MainUI::on_actionDeveloper_Mode_triggered(){
  ui->text_dev_output->setVisible(ui->actionDeveloper_Mode->isChecked());
}

void MainUI::on_actionShow_Orphan_Packages_triggered(){
  slotRefreshInstallTab();	
}

void MainUI::on_actionShow_Base_Packages_triggered(){
  slotRefreshInstallTab();	
}

void MainUI::on_actionRaw_Inst_Packages_triggered(){
  slotRefreshInstallTab();
}

void MainUI::on_actionCaution_triggered(){
  QMessageBox::information(this, tr("Jail Installation Warning"), tr("Installing or updating applications into a jail will automatically synchronize the package repository configuration within the jail to the current system configuration.") );
}

//=========
//   SPECIAL
//=========
void MainUI::on_tool_start_updates_clicked(){
  //Check for any pending/running processes first
  if(PBI->safeToQuit()){
    //Get the update stats and ask for verification to start now
    QMessageBox MB(QMessageBox::Question, tr("Start Updates?"), tr("Are you ready to start performing updates?")+"\n\n"+tr("NOTE: Please close any running applications first!!"), QMessageBox::Yes | QMessageBox::No, this);
      MB.setDetailedText(PBI->updateDetails(VISJAIL));
      //NOTE: QMessageBox's *cannot* be resized!!
    if( QMessageBox::Yes != MB.exec() ){
      return; //cancelled
    }
    //Now start the updates
    UpdateDialog dlg(this, PBI->JailID(VISJAIL));
    //this->hide(); //Hide the main UI (causes crash after updates for some reason)
    //this->setEnabled(false); // Don't disable parent, will disable child also
    dlg.exec();
    if(dlg.rebooting){ this->close(); } //reboot triggered, close down the AppCafe
    else{
      //re-check for updates
      this->setEnabled(false); //disable the UI temporarily
      QTimer::singleShot(0, PBI, SLOT(UpdateIndexFiles()) );
    }
  }else{
    QMessageBox::information(this, tr("Stand-Alone Update Procedure"), tr("The update cannot be run while other operations are pending. Please cancel them and try again.") );
  }
}

void MainUI::installOptionChanged(){
  if(radio_system->isChecked()){
    VISJAIL.clear();
    ui->actionCaution->setVisible(false);
    combo_jails->setEnabled(false);
  }else if(radio_jail->isChecked() && combo_jails->count()!=0 ){
    VISJAIL = combo_jails->currentText();
    ui->actionCaution->setVisible(true);
    combo_jails->setEnabled(true);
  }else{
    //invalid setting -> reset to system option
    radio_system->setChecked(true);
    QTimer::singleShot(1, this, SLOT(installOptionChanged())); //re-run this function
    return;
  }
  //Now refresh the UI to reflect the new option
   slotRefreshInstallTab();
   slotEnableBrowser(); //updates home page and category browser
   //Update the currently visible browser page as well
   bool onbrowser = ui->tabWidget->currentWidget()==ui->tab_browse;
   if(ui->stacked_browser->currentWidget()==ui->page_cat){
     slotGoToCategory(cCat);
   }else if(ui->stacked_browser->currentWidget()==ui->page_app){
     slotGoToApp(cApp);
   }else if(ui->stacked_browser->currentWidget()==ui->page_search){
     slotGoToSearch();
   }
   //If the user was originally on the installed tab - make sure he still is
   if(!onbrowser){ ui->tabWidget->setCurrentWidget(ui->tab_installed); }
}

void MainUI::slotUpdateJailList(){
  QString curr;
  if(combo_jails->count()>0){ combo_jails->currentText(); }
  combo_jails->clear();
  this->setEnabled(false); //deactivate the UI for a moment
  QStringList jls = PBI->runningJails();
  combo_jails->addItems(jls);
  
  if(jls.isEmpty()){
    radio_system->setChecked(true);
    radio_jail->setEnabled(false);
    if( !VISJAIL.isEmpty() ){
      VISJAIL.clear();
    }
  }else{
    radio_jail->setEnabled(true); 
  }
  this->setEnabled(true); //re-activate the UI
  if(jls.contains(curr)){ combo_jails->setCurrentIndex( jls.indexOf(curr) ); }
  else{ installOptionChanged(); }
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
    QMenu *lmenu = actionMenu->addMenu( QIcon(":icons/lock.png"), tr("Version Lock") );
	lmenu->addAction( QIcon(":icons/lock.png"), tr("Lock Current Version"), this, SLOT(slotActionLock()) );
	lmenu->addAction( QIcon(":icons/unlock.png"), tr("Unlock Application"), this, SLOT(slotActionUnlock()) );
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
  //Setup the context menu clear timer
  contextTimer = new QTimer(this);
    contextTimer->setSingleShot(true);
    contextTimer->setInterval(500); //1/2 second to clear
    connect(contextTimer, SIGNAL(timeout()), this, SLOT(contextMenuFinished()) );
  //Initialize the context menu
  contextActionMenu = new QMenu(this);
    connect(contextActionMenu, SIGNAL(aboutToHide()), contextTimer, SLOT(start()) );
  //Now setup the action button
  ui->tool_install_performaction->setMenu(actionMenu);
  ui->tool_install_performaction->setPopupMode(QToolButton::InstantPopup);
  //Now setup any defaults for the installed tab
  ui->tree_install_apps->setIconSize(QSize(22,22));
  ui->tree_install_apps->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(ui->tree_install_apps, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(slotCheckSelectedItems()) );
  connect(ui->tree_install_apps, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT( slotInstalledAppRightClicked(const QPoint &)) );

  //slotRefreshInstallTab();
}

void MainUI::formatInstalledItemDisplay(QTreeWidgetItem *item){
  //simplification function for filling the tree widget item with the appropriate information about the PBI
  QString ID = item->whatsThis(0);
  NGApp app = PBI->singleAppInfo(ID, VISJAIL);
  //qDebug() << "Item:" << ID << app.origin << app.name << item->text(0);
  if(app.origin.isEmpty()){ item->setWhatsThis(0,""); qDebug() << "Invalid Item:" << ID; return; } //invalid item
    //Fill the item columns [name, version, status, size, date, arch]
      if(app.name.isEmpty()){ item->setText(0, app.origin.section("/",-1)); }
      else{ item->setText(0,app.name); }
      item->setText(1,app.installedversion);
      item->setText(2, PBI->currentAppStatus(ID, VISJAIL));
      if(app.installedsize=="0.00B"){ item->setText(3, PBI->getMetaPkgSize(ID, VISJAIL) ); }
      else{ item->setText(3, app.installedsize); }
      item->setText(4, app.installedwhen);
      item->setText(5, app.installedarch);
      //Application Icon
      QString icon = checkIcon(app.icon, app.type);
        item->setIcon(0,QIcon(icon) );
      //Application Lock status
      icon.clear();
        if(app.isLocked){ icon = ":/icons/lock.png"; }
        item->setIcon(1,QIcon(icon) );
      //Set the application install status
      QColor col(0,0,0,0); //completely transparent
      if(app.isOrphan){ col = QColor(255,250,205,190); } // yellow
      if(!PBI->safeToRemove( app.origin )){ col = QColor(255,10,10,20); } //mostly-transparent red
	item->setBackground(0,QBrush(col));

}

QStringList MainUI::getCheckedItems(){
  //Return the pbiID's of all the active items
  QStringList output;
  //If the browser app page is visible, use the current App
  if( (ui->stacked_browser->currentWidget()==ui->page_app) && (ui->tabWidget->currentWidget()==ui->tab_browse) ){
    output << cApp; 
	  
  //Check for whether this is the context menu on the main widget
  }else if(!cDetails.isEmpty()){
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
  QStringList installList = PBI->installedList(VISJAIL, ui->actionRaw_Inst_Packages->isChecked(), ui->actionShow_Orphan_Packages->isChecked());
  //qDebug() << "Installed Pkgs:" << installList;
  if( !ui->actionShow_Base_Packages->isChecked() ){
    installList = PBI->filterBasePkgs(installList); //don't show base dependencies
  }
  //Always show apps with pending status
  installList.append( PBI->pendingInstallList() );
  installList.removeDuplicates();
  //Quick finish if no items installed/pending
  if(installList.isEmpty()){
    ui->tree_install_apps->clear();
    return;
  }
  //Get the list we have now and handle items as needed
  //QStringList cList;
  for(int i=0; i<ui->tree_install_apps->topLevelItemCount(); i++){
    QString item = ui->tree_install_apps->topLevelItem(i)->whatsThis(0);
    //Update item if necessary
    if(installList.contains(item)){ 
	formatInstalledItemDisplay( ui->tree_install_apps->topLevelItem(i) ); 
	installList.removeAll(item); //Remove it from the list - since already handled
    //Remove item if necessary
    }else{
      //qDebug() << "Remove Item:" << item;
      delete ui->tree_install_apps->takeTopLevelItem(i);
      i--; //make sure to back up once to prevent missing the next item
    }
  }
  //Now add any new items to the list
  //qDebug() << "New Items:" << installList;
  for(int i=0; i<installList.length(); i++){
    QTreeWidgetItem *item = new QTreeWidgetItem; //create the item
	//qDebug() << "New Item:" << installList[i];
        item->setWhatsThis(0,installList[i]);
        //Now format the display
        formatInstalledItemDisplay(item);
	item->setCheckState(0,Qt::Unchecked); //make sure it is checkable
	//qDebug() << "New Item:" << installList[i] << item->text(0);
	if(item->whatsThis(0).isEmpty()){
	  //Do not put invalid items into the display
	  delete item;
	}else{
          //Now insert this item onto the list
          ui->tree_install_apps->addTopLevelItem(item);
	}
  }
  ui->tree_install_apps->sortItems(0, Qt::AscendingOrder);
  //Make sure that there is an item selected
  if(ui->tree_install_apps->topLevelItemCount() > 0 ){
    if( ui->tree_install_apps->selectedItems().isEmpty() ){
      ui->tree_install_apps->setCurrentItem( ui->tree_install_apps->topLevelItem(0) );
    }
    //Now re-size the columns to the minimum required width
    for(int i=0; i<6; i++){
      ui->tree_install_apps->resizeColumnToContents(i);
    } 
  }
  slotDisplayStats();
  slotCheckSelectedItems();
  if(PBI->checkForUpdates(VISJAIL)){
    ui->group_updates->setVisible(true);
    if(VISJAIL.isEmpty()){ ui->tool_start_updates->setIcon(QIcon(":icons/view-refresh.png")); }
    else{  ui->tool_start_updates->setIcon(QIcon(":icons/view-jail.png")); }
  }else{
    ui->group_updates->setVisible(false);
  }
  //If the browser app page is currently visible for this app
  if( (ui->stacked_browser->currentWidget()==ui->page_app) && (ui->tabWidget->currentWidget()==ui->tab_browse) ){
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
  cmd.append("-----------\\nPBI Information:\\nPort: "+app.origin+"\\nName: "+app.name + "\\nDate Installed: "+app.installedwhen +"\\nVersion: "+app.installedversion+"\\nArchitecture: "+app.installedarch );
  //Startup the command externally
  PBI->runCmdAsUser("xdg-open \'"+cmd+"\'");
}

void MainUI::slotInstalledAppRightClicked(const QPoint &pt){
  //Get the item under the mouse click
  QTreeWidgetItem *it = ui->tree_install_apps->itemAt(pt);
  if(it==0){ return; } // no item selected
  QString pbiID = it->whatsThis(0);
  //Now Update the context menu appropriately
  NGApp info = PBI->singleAppInfo(pbiID);
  //QStringList info = PBI->PBIInfo(pbiID, QStringList() << "hasdesktopicons" << "hasmenuicons" << "hasmimetypes");
  if( info.origin.isEmpty() ){ return; } //invalid application
  bool pending = PBI->isWorking(pbiID);
  contextActionMenu->clear();
  if(info.hasDE && VISJAIL.isEmpty() ){
    QMenu *dmenu = contextActionMenu->addMenu( QIcon(":icons/xdg_desktop.png"), tr("Desktop Icons"));
      dmenu->addAction( QIcon(":icons/add.png"),tr("Add"),this,SLOT(slotActionAddDesktop()) );
      dmenu->addAction( QIcon(":icons/remove.png"),tr("Remove"),this,SLOT(slotActionRemoveDesktop()) );
  }
  if(info.isLocked){
    contextActionMenu->addAction( QIcon(":icons/unlock.png"), tr("Unlock Application"), this, SLOT(slotActionUnlock()) );
  }else{
    contextActionMenu->addAction( QIcon(":icons/lock.png"), tr("Lock Current Version"), this, SLOT(slotActionLock()) );
  }
  if(!pending && PBI->safeToRemove(pbiID)){
    //Remove option is only available if not currently pending actions
    contextActionMenu->addSeparator();
    contextActionMenu->addAction( QIcon(":icons/remove.png"), tr("Uninstall"), this, SLOT(slotActionRemove()) );
  }else if(pending){
    //Cancel option is only available if actions are currently pending	  
    contextActionMenu->addSeparator();
    contextActionMenu->addAction( QIcon(":icons/dialog-cancel.png"), tr("Cancel Actions"), this, SLOT(slotActionCancel()) );
  }
  //Now show the menu
  qDebug() << "Show context menu for:" << pbiID;  
  cDetails = pbiID; //save this so we know which app is currently being modified
  contextActionMenu->popup(ui->tree_install_apps->mapToGlobal(pt));
}

void MainUI::contextMenuFinished(){
  qDebug() << " - Context Menu closed";
  cDetails.clear();	
}

// === SELECTED PBI ACTIONS ===
void MainUI::slotActionAddDesktop(){
  QStringList items = getCheckedItems();
  qDebug() << "Add Desktop Icons:" << items;
  PBI->addDesktopIcons(items,false); //only for current user
}

void MainUI::slotActionRemoveDesktop(){
  QStringList items = getCheckedItems();
  qDebug() << "Rem Desktop Icons:" << items;
  PBI->rmDesktopIcons(items,false);  //Only for current user
}

void MainUI::slotActionRemove(){
  QStringList checkedID = getCheckedItems();
  if(!checkedID.isEmpty()){
    //Verify that the user really wants to remove these apps
    checkedID = generateRemoveMessage(checkedID);
    if( !checkedID.isEmpty() ){
      PBI->removePBI(checkedID, VISJAIL);
    }
  }
}

void MainUI::slotActionCancel(){
  QStringList checkedID = getCheckedItems();
  if(!checkedID.isEmpty()){
    PBI->cancelActions(checkedID);  
  }
}

void MainUI::slotActionLock(){
  QStringList checkedID = getCheckedItems();
  if(!checkedID.isEmpty()){
    PBI->lockApp(checkedID, VISJAIL);  
  }	
}

void MainUI::slotActionUnlock(){
  QStringList checkedID = getCheckedItems();
  if(!checkedID.isEmpty()){
    PBI->unlockApp(checkedID, VISJAIL);  
  }	
}

void MainUI::slotStartApp(QAction* act){
  qDebug() << "Starting external application:" << act->text();
  if( !VISJAIL.isEmpty() ){ qDebug() << " - In Jail:" << VISJAIL; }
  //Get the command from the action
  XDGFile file;
  file.LoadDesktopFile( act->whatsThis() );
  //Now create the command as needed
  QString cmd = file.Exec();
  if(!VISJAIL.isEmpty()){ cmd = "warden chroot "+VISJAIL+" \""+cmd+"\""; }
  //Startup the command externally with user permissions
  PBI->runCmdAsUser(cmd);
}

// ==========================
// ====== BROWSER TAB =======
// ==========================
void MainUI::initializeBrowserTab(){
  ui->tab_browse->setEnabled(false);
  //Always make sure that the browser starts off on the "home" page
  ui->stacked_browser->setCurrentWidget(ui->page_home);
  //With invisible shortcut buttons
  ui->tool_browse_cat->setVisible(false);
  ui->tool_browse_app->setVisible(false);
  //Clear any items left over from the designer form
  clearScrollArea(ui->scroll_br_home_newapps);
  clearScrollArea(ui->scroll_br_home_rec);
  //Search functionality
  searchTimer = new QTimer();
    searchTimer->setSingleShot(true);
    searchTimer->setInterval(500); // 0.5 sec wait before a search
    connect(searchTimer,SIGNAL(timeout()),this,SLOT(slotGoToSearch()) );
  connect(ui->tool_browse_search,SIGNAL(clicked()),this,SLOT(slotGoToSearch()) );
  connect(ui->line_browse_searchbar,SIGNAL(returnPressed()),this,SLOT(slotGoToSearch()) );
  connect(ui->tool_browse_gotocat, SIGNAL(clicked()), this, SLOT(slotGoToCatBrowser()) );
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
  ui->tab_browse->setEnabled(false);
  slotDisplayStats();
}

void MainUI::slotEnableBrowser(){
  this->setEnabled(true);
  qDebug() << "Repo Ready: - generating browser home page";
  //Now create the browser home page
  slotUpdateBrowserHome();
  //And allow the user to go there
  ui->tab_browse->setEnabled(true);
  slotDisplayStats();
}

void MainUI::slotUpdateBrowserHome(){
  //Prevent two processes running this at the same time
  static bool isupdating = false;
  if(isupdating){ return; }
  isupdating = true;
  //Load the Recommendations
  if(homeRec.isEmpty()){ homeRec = PBI->getRecommendedApps(); }
  ui->group_br_recommend->setVisible( fillVerticalAppArea(ui->scroll_br_home_rec, homeRec, false) );
  //Load the application spotlight
  if(homeHigh.isEmpty()){ homeHigh = PBI->getHighlightedApps(); }
  ui->group_br_home_spotlight->setVisible( fillVerticalAppArea(ui->scroll_br_home_spot, homeHigh, false) );
  //Load the newest applications
  clearScrollArea(ui->scroll_br_home_newapps);
  QVBoxLayout *newapplayout = new QVBoxLayout;
  QStringList newapps = PBI->getNewApps();
  QList<NGApp> apps = PBI->AppInfo(newapps);
  for(int i=0; i<apps.length(); i++){
      if(apps[i].name.isEmpty()){ apps[i].name = apps[i].origin.section("/",-1); }
      SmallItemWidget *item = new SmallItemWidget(apps[i].origin, apps[i].name, checkIcon(apps[i].icon, apps[i].type), apps[i].version);
      connect(item,SIGNAL(appClicked(QString)),this,SLOT(slotGoToApp(QString)) );
      newapplayout->addWidget(item);
  }
  newapplayout->addStretch(); //add a spacer to the end
  newapplayout->setContentsMargins(0,0,0,0);
  newapplayout->setSpacing(0);
  ui->scroll_br_home_newapps->widget()->setLayout(newapplayout);
  //Make sure that the newapps scrollarea is the proper fit horizontally (no scrolling)
  int minw = ui->scroll_br_home_newapps->widget()->minimumSizeHint().width()+2;
  if(ui->scroll_br_home_newapps->verticalScrollBar()->isVisible()){
    minw = minw + ui->scroll_br_home_newapps->verticalScrollBar()->width(); //add scrollbar width
  }
  ui->scroll_br_home_newapps->setMinimumWidth(minw);
  
  //Make sure the new apps area is invisible if no items available
  if(newapps.isEmpty()){ ui->group_br_home_newapps->setVisible(false); }
  else{ ui->group_br_home_newapps->setVisible(true); }
  //make sure the home page is visible in the browser (slotGoToHome without changing tabs)
  //ui->stacked_browser->setCurrentWidget(ui->page_home);	
  //Make sure the shortcut buttons are disabled
  ui->tool_browse_cat->setVisible(false);
  ui->tool_browse_app->setVisible(false); 
  
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
  isupdating = false; //done
}

void MainUI::slotGoToHome(){
  if(backApps[0]!="Home"){ backApps.prepend("Home"); }
  //Now rebuild the back menu
    backMenu->clear();
    for(int i=1; i<backApps.length() && i<11; i++){ //only show 10 items max
      backMenu->addAction(backApps[i]);
    }
  ui->tool_br_back->setVisible(backApps.length() > 1);
  ui->tabWidget->setCurrentWidget(ui->tab_browse);
  ui->stacked_browser->setCurrentWidget(ui->page_home);	
  //Make sure the shortcut buttons are disabled
  ui->tool_browse_cat->setVisible(false);
  ui->tool_browse_app->setVisible(false);
  ui->tool_browse_gotocat->setVisible(true);
}

void MainUI::slotGoToCatBrowser(){
  if(backApps[0]!="Categories"){backApps.prepend("Categories"); }
  //Now rebuild the back menu
    backMenu->clear();
    for(int i=1; i<backApps.length() && i<11; i++){ //only show 10 items max
      backMenu->addAction(backApps[i]);
    }
  ui->tool_br_back->setVisible(backApps.length() > 1);
  ui->tabWidget->setCurrentWidget(ui->tab_browse);
  ui->stacked_browser->setCurrentWidget(ui->page_browsecats);
  //Make sure the shortcut buttons are diabled
  ui->tool_browse_cat->setVisible(false);
  ui->tool_browse_app->setVisible(false);
  ui->tool_browse_gotocat->setVisible(false);
}

void MainUI::slotGoToCategory(QString cat){
  qDebug() << "Show Category:" << cat;
  //Save this appID for going back later
  if(backApps[0]!=cat){backApps.prepend(cat); }
  //Now rebuild the back menu
    backMenu->clear();
    for(int i=1; i<backApps.length() && i<11; i++){ //only show 10 items max
      backMenu->addAction(backApps[i]);
    }
  ui->tool_br_back->setVisible(backApps.length() > 1);

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

void MainUI::slotGoToApp(QString appID){
  qDebug() << "Show App:" << appID;
  //Get the general application info
  NGApp data = PBI->singleAppInfo(appID, VISJAIL);
  if(data.origin.isEmpty() && !VISJAIL.isEmpty()){ data = PBI->singleAppInfo(appID); } //not installed in that jail
  if(data.origin.isEmpty()){
    qDebug() << "Invalid App:" << appID;
    return;
  }
  if(backApps[0]!=appID){ backApps.prepend(appID); }
  //Now rebuild the back menu
    backMenu->clear();
    for(int i=1; i<backApps.length() && i<11; i++){ //only show 10 items max
      backMenu->addAction(backApps[i]);
    }
  ui->tool_br_back->setVisible(backApps.length() > 1);
  cApp = appID; //save this for later
  //Start the search for similar apps
  PBI->searchSimilar = appID;
  PBI->searchAll = ui->actionRaw_Packages->isChecked();
  ui->tabWidget_browse_info->setTabEnabled(3, false); //similar apps tab
  QTimer::singleShot(0,PBI,SLOT(startSimilarSearch()));
  //Now Check icon
  //qDebug() << "App Icon:" << data.origin << data.icon << data.type;
  data.icon = checkIcon(data.icon, data.type); //if(data.icon.isEmpty() || !QFile::exists(data.icon)){ data.icon = defaultIcon; }
  //qDebug() << " - fixed icon:" << data.icon;
  //Now fill the UI with the data
  if(data.name.isEmpty()){ ui->label_bapp_name->setText(data.origin); }
  else{ ui->label_bapp_name->setText(data.name); }
  ui->label_bapp_icon->setPixmap(QPixmap(data.icon).scaled(ui->label_bapp_icon->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation) );
  if(data.author.isEmpty()){ ui->tool_app_openweb->setText(tr("Author Unknown")); }
  else{ ui->tool_app_openweb->setText(data.author); }
  ui->tool_app_openweb->setWhatsThis(data.website);
  ui->tool_app_openweb->setEnabled( !data.website.isEmpty() && !(data.website.toLower()=="unknown") );
  ui->tool_app_openweb->setToolTip(data.website);
  if(data.license.isEmpty()){ ui->label_bapp_license->setText(tr("Unknown")); }
  else{ ui->label_bapp_license->setText(data.license); }
  ui->label_bapp_type->setText(data.type);
  ui->text_bapp_description->setText(data.description);
  ui->tool_app_rank->setIcon( QIcon( getRatingIcon(data.rating) ) );
  ui->tool_app_rank->setVisible( data.hasWiki );
  ui->tool_app_tips->setVisible( data.hasWiki );
  ui->tool_bapp_newjail->setVisible(!data.pbiorigin.isEmpty()); //must be a PBI
  QString cVer = data.installedversion;
    ui->label_bapp_version->setText(data.version);
    ui->label_bapp_arch->setText(data.arch);
  //Start the search for the app download size
    //ui->label_bapp_size->setText( tr("Calculating...") );
    //PBI->searchSize = appID;
    //PBI->searchJail = VISJAIL;
    //qDebug() << "Start fetching size:";
    //QTimer::singleShot(20,PBI, SLOT(startSizeSearch()));
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
      ui->tool_install_shortcuts->setVisible(data.hasDE && VISJAIL.isEmpty());
	
  }
  //Now enable/disable the shortcut buttons
  ui->tool_browse_app->setVisible(true);
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
    ui->tool_browse_cat->setVisible(true);
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
    QTimer::singleShot(10,this,SLOT(slotShowFirstScreenshot()) );
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
  QCoreApplication::processEvents(); //to help the tab to refresh while background threads are going
}

void MainUI::slotBackToApp(QAction* act){
  QString app = act->text();
  //Clear the back history up to this entry
  for(int i=0; i<backApps.length(); i++){
    QString item = backApps[i];
    backApps.removeAt(i); i--;
    if(item==app){ break; } //stop here
  }
  if(app.startsWith("Search:")){ 
    ui->line_browse_searchbar->setText(app.section(":",1,50)); //return to that search
    slotGoToSearch(); 
  }else if(app=="Home"){
    slotGoToHome();
  }else if(app=="Categories"){
    slotGoToCatBrowser();
  }else if(app.contains("/")){ 
    slotGoToApp(app);	
  }else if(!app.isEmpty()){ 
    slotGoToCategory(app); 
  }else{ slotGoToHome(); }
}

void MainUI::slotUpdateAppDownloadButton(){
  QString ico;
  QString stat = PBI->currentAppStatus(cApp, VISJAIL);
  ui->label_app_status->setText(stat);
  ui->label_app_status->setVisible( !stat.isEmpty() );
  if( PBI->isWorking(cApp) ){ //app currently pending or actually doing something
    ui->tool_bapp_download->setText( tr("Working") );
    ui->tool_bapp_download->setIcon(QIcon(":icons/working.png"));
    ui->tool_bapp_download->setEnabled(false);
    ui->tool_bapp_newjail->setEnabled(false);
  }else if( !PBI->isInstalled(cApp, VISJAIL) ){ //new installation
    ui->tool_bapp_download->setText(tr("Install Now!"));
    ico = ":icons/app_download.png";
    ui->tool_bapp_download->setEnabled(true);
    ui->tool_bapp_newjail->setEnabled(true);
  }else{ //already installed (no downgrade available)
    ui->tool_bapp_download->setText(tr("Installed"));
    ui->tool_bapp_download->setIcon(QIcon(":icons/dialog-ok.png"));
    ui->tool_bapp_download->setEnabled(false); //only disable if no jail menu
    ui->tool_bapp_newjail->setEnabled(true);
  }
  //Now set the icon appropriately if it requires root permissions
  if(!ico.isEmpty()){
    ui->tool_bapp_download->setIcon(QIcon(ico));
  }
  ui->tool_bapp_download->setWhatsThis(cApp); //set for slot
  ui->tool_bapp_download->setPopupMode( QToolButton::DelayedPopup );
}

void MainUI::slotGoToSearch(){
  searchTimer->stop(); //just in case "return" was pressed to start the search
  QString search = ui->line_browse_searchbar->text();
  //Save this appID for going back later
  if(backApps[0]!="Search:"+search){ backApps.prepend("Search:"+search); }
  //Now rebuild the back menu
    backMenu->clear();
    for(int i=1; i<backApps.length() && i<11; i++){ //only show 10 items max
      backMenu->addAction(backApps[i]);
    }
  ui->tool_br_back->setVisible(backApps.length() > 1);
  if(search.isEmpty()){ return; }
  PBI->searchTerm = search;
  PBI->searchAll = ui->actionRaw_Packages->isChecked();
  QTimer::singleShot(1,PBI,SLOT(startAppSearch()));
  ui->label_bsearch_info->setText( tr("Searching the application database. Please Wait....") );
    ui->label_bsearch_info->setVisible(true);
    ui->group_bsearch_best->setVisible(false);
    ui->group_bsearch_other->setVisible(false);
  
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
    ui->label_bsearch_info->setVisible(true);
    ui->group_bsearch_best->setVisible(false);
    ui->group_bsearch_other->setVisible(false);
  }else{
    ui->label_bsearch_info->setVisible(false);
    ui->group_bsearch_best->setVisible(true);
    //Now fill the other results
    ui->group_bsearch_other->setVisible( fillVerticalAppArea( ui->scroll_bsearch_other, rest, true) );
  }
	
  //need to make sure the search bar still has keyboard focus (just in case)
  ui->tabWidget->setCurrentWidget(ui->tab_browse);
  ui->stacked_browser->setCurrentWidget(ui->page_search);
  
}

/*void MainUI::slotShowSize(QString sz){
  qDebug() << " - Found size:" << sz;
  ui->label_bapp_size->setText(sz);
}*/

void MainUI::slotShowFirstScreenshot(){
  //This is a slot to start the screenshot fetch procedue in a new thread
  showScreenshot(0);
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
  if(searchTimer->isActive()){ searchTimer->stop(); } //make sure it is a full timer reset
  //Live search only after 3 characters have been typed
  if(ui->line_browse_searchbar->text().length() > 2){
    searchTimer->start();	  
  }
}

void MainUI::on_tool_bapp_download_clicked(){
  QString appID = ui->tool_bapp_download->whatsThis();
  if(PBI->isInstalled(appID, VISJAIL)){ return; } //do nothing if already installed
  //Verify the app installation
  QString msg = tr("Are you sure you want to install this application?")+"\n\n"+appID;
  if( !VISJAIL.isEmpty() ){ msg.append( "\n"+ QString(tr("Into jail: %1")).arg(VISJAIL) ); }
  if( QMessageBox::Yes != QMessageBox::question(this,tr("Verify Installation"), msg,QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes)){
    return;
  }
  PBI->installApp(QStringList() << appID, VISJAIL);
  ui->tool_bapp_newjail->setEnabled(false); //make sure it cannot be clicked more than once before page refresh
  //Now show the Installed tab
  //ui->tabWidget->setCurrentWidget(ui->tab_installed);
}

void MainUI::on_tool_bapp_newjail_clicked(){
  QString appID = ui->tool_bapp_download->whatsThis();
  //Verify the app installation
  QString msg = tr("This will install the application into a new jail, separate from the main system.\n Do you wish to continue?")+"\n\n"+appID;
  if( QMessageBox::Yes != QMessageBox::question(this,tr("Verify Installation"), msg,QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes)){
    return;
  }
  PBI->installAppIntoJail(appID);
  ui->tool_bapp_download->setEnabled(false); //make sure it cannot be clicked more than once before page refresh
}

void MainUI::on_group_br_home_newapps_toggled(bool show){
  ui->scroll_br_home_newapps->setVisible(show);
}

void MainUI::on_tool_app_nextScreen_clicked(){
  //Read the current screenshot and go to the previous one
  int cur = ui->tool_app_cScreen->text().section("/",0,0).simplified().toInt();
  showScreenshot(cur); //the viewable number is always 1 greater than the actual number
}

void MainUI::on_tool_app_prevScreen_clicked(){
  //Read the current screenshot and go to the previous one
  int cur = ui->tool_app_cScreen->text().section("/",0,0).simplified().toInt();
  showScreenshot(cur-2); //the viewable number is always 1 greater than the actual number	
}

void MainUI::on_tool_app_cScreen_clicked(){
  //View the current screenshot in a new window - as large as possible 
  SSDialog *dlg = new SSDialog(0, cScreenshot);
  dlg->showMaximized();
  //dlg->show();
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
  if(backApps.length() <= 1){ return; }
  backApps.removeAt(0); //remove the first entry (the currently visible page)
  QString app = backApps[0]; //get the top entry now
  if(app.startsWith("Search:")){ 
    ui->line_browse_searchbar->setText(app.section(":",1,50)); //return to that search
    slotGoToSearch(); 
  }else if(app=="Home"){
    slotGoToHome();
  }else if(app=="Categories"){
    slotGoToCatBrowser();
  }else if(app.contains("/")){ 
    slotGoToApp(app);	
  }else if(!app.isEmpty()){ 
    slotGoToCategory(app); 
  }else{ slotGoToHome(); }
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
    layout->setSpacing(2);
    layout->setContentsMargins(3,1,3,1);
    QList<NGApp> apps = PBI->AppInfo(applist, VISJAIL);
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
  ui->tool_app_cScreen->setText( QString::number(num+1)+"/"+QString::number(app.screenshots.length()) );
  //download the file from the URL given and auto-show it
    // - make sure we don't have a request still running, otherwise cancel it
    if(netreply==0){} // do nothing, not initialized yet
    else if(netreply->isRunning()){ netreply->abort(); }
  netreply = netman->get( QNetworkRequest( QUrl(app.screenshots[num]) ) );
  qDebug() << "Start fetching screenshot:" << netreply->url();
  ui->label_app_screenshot->setText( tr("Please wait. Downloading Screenshot.") );
  ui->tool_app_nextScreen->setEnabled(false);
  ui->tool_app_prevScreen->setEnabled(false);
  ui->tool_app_cScreen->setEnabled(false);
}


void MainUI::slotScreenshotAvailable(QNetworkReply *reply){
  qDebug() << "Screenshot retrieval finished:" << reply->error();
  if(reply->error() == QNetworkReply::NoError){
    QByteArray picdata = reply->readAll();
    cScreenshot = QPixmap(); //clear the current pixmap
      cScreenshot.loadFromData(picdata); //now load the current image
    ui->label_app_screenshot->setText(""); //clear the text
    ui->label_app_screenshot->setPixmap( cScreenshot.scaled(ui->label_app_screenshot->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation) );
    ui->tool_app_cScreen->setEnabled(true);
  }else{
    //Network error, try to get a specific error message
    QString err;
    switch(reply->error()){
      case QNetworkReply::TimeoutError:
	err = tr("Server Timeout"); break;
      case QNetworkReply::ContentAccessDenied:
	err = tr("Access Denied"); break;
      case QNetworkReply::ContentNotFoundError:
	err = tr("Screenshot Not Found"); break;
      default:
	err = tr("Network Error"); break;
    }
    ui->label_app_screenshot->setText( tr("Could not load screenshot")+"\n("+err+")" );
  }
  //Now enable the prev/next buttons as necessary
  QStringList txt = ui->tool_app_cScreen->text().split("/");
  if(txt.length()!=2){ return; } //invalid text for some reason
  if(txt[0]!="1"){ ui->tool_app_prevScreen->setEnabled(true); }
  if(txt[0].toInt() < txt[1].toInt()){ ui->tool_app_nextScreen->setEnabled(true); }
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
  apps.removeAll(""); //make sure no empty entries
  apps = PBI->filterBasePkgs(apps); //cannot remove particular packages
  //QStringList pluspkgs;
  for(int i=0; i<apps.length(); i++){
    NGApp app = PBI->singleAppInfo(apps[i]);
    if(app.name.isEmpty()){ msg.append(app.origin+"\n"); }
    else{ msg.append(app.name+"\n"); }
    //if(app.installedsize=="0.00B" && !app.pbiorigin.isEmpty() ){ 
      //msg.append( " - "+QString(tr("Removes: %1")).arg( app.dependency.join(", ") )+"\n" );
      //pluspkgs << app.dependency;
    //}
    if(!app.rdependency.isEmpty()){ msg.append( " - "+QString(tr("Also Removes: %1")).arg( PBI->listRDependencies(apps[i]).join(", "))+"\n" ); }
  }
  //if( !pluspkgs.isEmpty() ){ apps << pluspkgs; }
  
  if(apps.isEmpty()){
    QMessageBox::warning(this, tr("Invalid Removal"), tr("These applications are required by the base PC-BSD system and cannot be removed") );
  }else{
    if( QMessageBox::Yes != QMessageBox::question(this,tr("Verify PBI Removal"), msg ,QMessageBox::Yes | QMessageBox::Cancel,QMessageBox::Cancel) ){
      apps.clear();
    }
  }
  return apps;
}

QString MainUI::getRatingIcon(QString rating){
  if( rating=="0.00" || rating.isEmpty() ){
    //invalid rating
    return ":/icons/rating-0.png";
  }
  //First round the rating to the nearest whole number
  int rate = qRound(rating.toDouble());
  QString num = QString::number(rate);
  QString ico = ":/icons/rating-%1.png";
  ico = ico.arg(num);
  return ico;
}
