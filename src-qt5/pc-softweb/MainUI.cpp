#include "MainUI.h"

#include <QKeySequence>
#include <QApplication>
#include <QStringList>
#include <QDebug>

MainUI::MainUI(bool debugmode) : QMainWindow(){
  //Setup UI
  DEBUG = debugmode;
  AUTHCOMPLETE = false; //not performed yet
  this->setWindowTitle(tr("AppCafe"));
  //Need 1024 wide if possible
  this->resize(1024,600);
  this->setWindowIcon( QIcon(":icons/appcafe.png") );
  if(this->centralWidget()==0){ this->setCentralWidget( new QWidget(this) ); }
  this->centralWidget()->setLayout( new QVBoxLayout() );
  this->centralWidget()->layout()->setContentsMargins(0,0,0,0);
  this->setStatusBar(new QStatusBar());
  //Setup the ToolBar
  QToolBar *tb = this->addToolBar("");
    tb->setMovable(false);
    tb->setFloatable(false);
    tb->setContextMenuPolicy(Qt::CustomContextMenu); //disable the built-in visibility context menu
    backA = tb->addAction(QIcon(":icons/back.png"), tr("Back"), this, SLOT(GoBack()) );
    forA = tb->addAction(QIcon(":icons/forward.png"), tr("Forward"), this, SLOT(GoForward()) );
    refA = tb->addAction(QIcon(":icons/refresh.png"), tr("Refresh"), this, SLOT(GoRefresh()) );
    stopA = tb->addAction(QIcon(":icons/stop.png"), tr("Stop"), this, SLOT(GoStop()) );
  // - toolbar spacer
    QWidget *spacer = new QWidget(this);
      spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
      tb->addWidget(spacer);
  // - Progress bar
    progressBar = new QProgressBar(this);
    progressBar->setRange(0,100);
    progA = tb->addWidget(progressBar); //add it to the end of the toolbar
    progA->setVisible(false); //start off invisible
  // - List Button
    listB = new QToolButton(this);
      listB->setIcon( QIcon(":icons/list.png") );
      listB->setToolTip( tr("AppCafe Options") );
      listB->setStyleSheet( "QToolButton::menu-indicator{ image: none; }" );
      listB->setPopupMode(QToolButton::InstantPopup);
    tb->addWidget(listB);
    //Setup the menu for this button
    listMenu = new QMenu();
      listMenu->addAction(QIcon(":icons/configure.png"), tr("Configure"), this, SLOT(GoConfigure() ) );
      listMenu->addAction(QIcon(":icons/list.png"), tr("Save Pkg List"), this, SLOT(Save_pkglist() ) );
      listMenu->addSeparator();
      listMenu->addAction(QIcon(":icons/search.png"), tr("Search For Text"), this, SLOT(openSearch() ) );
      listMenu->addSeparator();
      listMenu->addAction(QIcon(":icons/close.png"), tr("Close AppCafe"), this, SLOT(GoClose() ) );
    listB->setMenu(listMenu);
    //Setup the search options
    group_search = new QFrame(this);
      group_search->setLayout( new QHBoxLayout() );
      group_search->layout()->setContentsMargins(2,2,2,2);
    line_search = new QLineEdit(this);
      group_search->layout()->addWidget(line_search);
    tool_search = new QToolButton(this);
      group_search->layout()->addWidget(tool_search);
      tool_search->setIcon( QIcon(":icons/search.png") );
      group_search->layout()->addItem(new QSpacerItem(0,0,QSizePolicy::Expanding, QSizePolicy::Minimum) );
  //Setup the Main Interface
    webview = new QWebView(this);
    this->centralWidget()->layout()->addWidget(webview);
    if(webview->page()==0){ webview->setPage(new QWebPage(webview)); }
    webview->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    this->centralWidget()->layout()->addWidget(group_search);
  
  //Make sure the search bar is hidden to start with    
  group_search->setVisible(false);
    
  //Create the special keyboard shortcuts
  QKeySequence key(QKeySequence::Find);
  ctrlF = new QShortcut( key, this );
  key = QKeySequence(Qt::Key_Escape);
  esc = new QShortcut( key, this );
    
  //Connect signals/slots
  connect(webview, SIGNAL(linkClicked(const QUrl&)), this, SLOT(LinkClicked(const QUrl&)) );
  connect(webview, SIGNAL(loadStarted()), this, SLOT(PageStartLoading()) );
  connect(webview, SIGNAL(loadProgress(int)), this, SLOT(PageLoadProgress(int)) );
  connect(webview, SIGNAL(loadFinished(bool)), this, SLOT(PageDoneLoading(bool)) );
  connect(webview->page()->networkAccessManager(), SIGNAL(sslErrors(QNetworkReply*, const QList<QSslError>&)), this, SLOT( authenticate(QNetworkReply*) ) );
  connect(tool_search, SIGNAL(clicked()), this, SLOT(GoSearch()) );
  connect(line_search, SIGNAL(returnPressed()), this, SLOT(GoSearch()) );
  connect(ctrlF, SIGNAL(activated()), this, SLOT(openSearch()) );
  connect(esc, SIGNAL(activated()), this, SLOT(closeSearch()) );
  if(DEBUG){
    //connect(webview, SIGNAL(statusBarMessage(const QString&)), this, SLOT(StatusTextChanged(const QString&)) );
    connect(webview->page(), SIGNAL(linkHovered(const QString&, const QString&, const QString&)), this, SLOT(StatusTextChanged(const QString&)) );
  }
  this->statusBar()->setVisible(DEBUG);
  loadHomePage();
  webview->show();
}

MainUI::~MainUI(){}
	
	
//===============
//  PRIVATE FUNCTIONS
//===============
bool MainUI::sameUrls(QUrl U1, QUrl U2){
  //qDebug() << "Comparing URLs:" << U1 << U2;
  if(U1==U2){ return true; }
  else{
    //Check the actual strings for differences which can be ignored
    QString S1 = U1.toString();
    QString S2 = U2.toString();
    QString diff;
    //qDebug() << "Compare Strings:\n- "+S1+"\n- "+S2;
    if(S2.length() > S1.length()){ 
      if(S2.startsWith(S1)){ diff = S2.remove(0,S1.length()); }
      else{ diff = S2.replace(S1,""); }
    }else{ 
      if(S1.startsWith(S2)){ diff = S1.remove(0,S2.length()); }
      else{ diff = S1.replace(S2,""); }
    }
    //qDebug() << " -DIFF:" << diff;
    if(diff.isEmpty()){ return true; } //should never happen because of the earlier check
    else if(diff==LOCALUI){ return true; } //just the special flag we put in
    else if(diff.startsWith("&installApp=")){ return true; } //just an install command
    else if(diff.startsWith("&deleteApp=")){ return true; }// just a remove command
    return false;
  }
}

bool MainUI::actionUrl(QUrl U1){
  //check for the particular "action" strings in the URL (don't want to repeat them when going forward/back)
  bool act = false;
  QString st = U1.toString();
  act = st.contains("&installApp=") || st.contains("&deleteApp=");
  return act;
}

//============
//  PRIVATE SLOTS
//============
void MainUI::slotSingleInstance(){
  this->show();
  this->raise();
}

void MainUI::LinkClicked(const QUrl &url){
  if(DEBUG){ qDebug() << "Link Clicked:" << url.toString(); }
  if(url.toString().startsWith(baseURL)){
    // Internal page - go there
    if(url.toString().contains(LOCALUI)){ webview->load( url ); }
    else{ webview->load( QUrl(url.toString()+LOCALUI) ); } //make sure to always append the special localUI flag
    webview->show();
  }else if(url.toString().startsWith("appcafe:")){
    //Run this command as user on the system
    QString cmd = url.toString().section("appcafe:",0,0,QString::SectionSkipEmpty);
    qDebug() << "Running command:" << cmd;
    QProcess::startDetached(cmd);
  }else{
    //Launch in a web browser
    QProcess::startDetached("xdg-open \""+url.toString()+"\"");
  }
}

void MainUI::PageStartLoading(){
  if(DEBUG){ qDebug() << "Start Loading Page:"; }
  backA->setEnabled(webview->history()->canGoBack());
  forA->setEnabled(webview->history()->canGoForward());
  refA->setVisible(false);
  stopA->setVisible(true);
  progA->setVisible(true);
  progressBar->setValue(0);
}

void MainUI::PageLoadProgress(int cur){
  //qDebug() << " - load Progress:" << cur;
  progressBar->setValue(cur);
}

void MainUI::PageDoneLoading(bool ok){
  if(DEBUG){ qDebug() << "Done Loading Page:" << ok << webview->url(); }
  progA->setVisible(false);
  backA->setEnabled(webview->history()->canGoBack());
  forA->setEnabled(webview->history()->canGoForward());
  refA->setVisible(true);
  stopA->setVisible(false);
  if(!ok && DEBUG){
    qDebug() << " - URL:" << webview->url().toString();
  }
}

void MainUI::authenticate(QNetworkReply *reply){
  if(DEBUG){ qDebug() << "Authenticating SSL connection..."; }
  reply->ignoreSslErrors();
}

void MainUI::StatusTextChanged(const QString &txt){
  //qDebug() << "Show Status Message:" << txt;
  this->statusBar()->showMessage(txt);
}

void MainUI::loadHomePage(){
  //Load the settings for the webUI
  QStringList settings;
  QFile file("/usr/local/etc/appcafe.conf");
  if(file.exists()){
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
      QTextStream in(&file);
      while(!in.atEnd()){ settings << in.readLine(); }
      file.close();
    }
  }
  QString port = "8885";
  bool usessl = false;
  for(int i=0; i<settings.length(); i++){
    if(settings[i].startsWith(";")){ continue; }
    if(settings[i].startsWith("port =")){
      port = settings[i].section("=",1,1).simplified();
    }else if(settings[i].startsWith("ssl =")){
      usessl = settings[i].section("=",1,1).simplified()=="true";
    }
  }

  if( QFile::exists("/tmp/.rebootRequired") ){
    //System waiting to reboot - put up a notice and disable the web viewer
    webview->setHtml("<p><strong>"+tr("System needs to reboot to finish applying updates!")+"</strong></p>");
    backA->setVisible(false);
    forA->setVisible(false);
    refA->setVisible(false);
    stopA->setVisible(false);
    progA->setVisible(false);
    return;	  
  }
  
  //Load the main page
  baseURL = BASEWEBURL;
  baseURL = baseURL.replace("<port>", port);
  if(usessl){ baseURL = baseURL.replace("http://","https://"); }
  if(DEBUG){ qDebug() << "Base URL:" << baseURL; }
  QString tmpURL = baseURL;
  if( !AUTHCOMPLETE ){
    //Only perform the authorization if necessary
    QString authkey = pcbsd::Utils::getLineFromCommandOutput("/usr/local/bin/pc-su /usr/local/share/appcafe/dispatcher-localauth 2>/dev/null").simplified();
    AUTHCOMPLETE = !authkey.isEmpty();
    if(DEBUG){ qDebug() << "Got Auth Key:" << AUTHCOMPLETE << authkey; }
    if ( authkey.indexOf(":") != -1 )
      authkey = authkey.section(":", 1, 1);
    if(AUTHCOMPLETE){ tmpURL.append("/?setDisId="+authkey); }
  }
  //Now clear the history (if any)
  
  //Now load the page
  if(AUTHCOMPLETE){
    webview->load( QUrl(tmpURL) );
    webview->show();
  }else{
    //System waiting to reboot - put up a notice and disable the web viewer
    webview->setHtml("<p><strong>"+tr("You are not authorized to view the AppCafe. Please contact a system administrator for assistance.")+"</strong></p>");
    backA->setEnabled(false);
    forA->setEnabled(false);
    refA->setVisible(false);
    stopA->setVisible(false);
    progA->setVisible(false);	  
  }

}

void MainUI::GoBack(){
  //Make sure that we skip any repeated history items (automatic page refreshes)
  //QWebHistoryItem cit = webview->history()->currentItem();
  if(DEBUG){ qDebug() << "---BACK CLICKED---"; }
  QList<QWebHistoryItem> bits = webview->history()->backItems(50); //max 50 items
  //First remove any action URLs (*never* repeat them)
  for(int i=0; i<bits.length(); i++){
    if( actionUrl(bits[i].url()) ){ bits.removeAt(i); i--; }
  }
  QUrl cpage = webview->url();
  for(int i=bits.length()-1; i>=0; i--){
    
    if( !sameUrls(bits[i].url(),cpage) || !sameUrls(bits[i].url(), bits[i].originalUrl()) ){ //not a page refresh
      webview->history()->goToItem(bits[i]);
      return;
    }
    if(DEBUG){ qDebug() << "Back History Item Skipped:" << bits[i].url(); }
  }
  //fallback in case something above did not work
  webview->back();
}

void MainUI::GoForward(){
  QList<QWebHistoryItem> bits = webview->history()->forwardItems(50); //max 50 items
  //First remove any action URLs (*never* repeat them)
  for(int i=0; i<bits.length(); i++){
    if( actionUrl(bits[i].url()) ){ bits.removeAt(i); i--; }
  }
  //Now go through and find the proper URL to go to
  QUrl cit = webview->url(); //current page URL
  if(DEBUG){ qDebug() << "---FORWARD CLICKED---" << bits.length(); }
  //Go to the last page-refresh (if any) for the next URL
  int got = -1; // go to index
  for(int i=0; i<bits.length() && got<0; i++){ //i=0 is the desired item
    if( !sameUrls(cit, bits[i].url()) ){
      got = i; //go to this URL (different from current page)
      //Now fast forward to the end of the page-refresh chain (if there is one) for this URL
      for(int j=i+1; j<bits.length(); j++){
        if( sameUrls( bits[i].url(), bits[j].url()) ){ got = j; }
	else{ break; }
      }
    }else if(i==(bits.length()-1)){
      got = i; //last item - go ahead and load it
    }
    if(DEBUG && got < 0){ qDebug() << "Forward History Item Skipped:" << i << bits[i].url(); }
  }
  //fallback in case something above did not work
  if(got<0){ webview->forward(); }
  else{ 
    webview->history()->goToItem(bits[got]); 
  }
}

void MainUI::GoRefresh(){
  webview->reload();
}

void MainUI::GoStop(){
  webview->stop();
}

void MainUI::GoClose(){
  this->close();
}

void MainUI::GoConfigure(){
  //Open up the configuration dialog
  ConfigDlg dlg(this);
  dlg.exec();
  if(dlg.savedChanges){ loadHomePage(); }
  else{ loadHomePage(); }
}

void MainUI::GoSearch(){
  //Search through the current page for the search term
  QString term = line_search->text();
  //qDebug() << "Searching:";
  if(term!=lastsearch){ webview->findText(""); } //start over
  if( !webview->findText(term) ){
    //qDebug() << "Search term not found:" << term;
    webview->findText(""); //go back to the top
    webview->findText(term);
  }
  lastsearch = term;
}

void MainUI::openSearch(){
  group_search->setVisible(true);
  line_search->setFocus();
}

void MainUI::closeSearch(){
  group_search->setVisible(false);	
}

void MainUI::Save_pkglist(){
  //Save the list of installed pkgs (top-level only - no reverse dependencies)
  qDebug() << "Save PKG list";
  QStringList allInstalled = pcbsd::Utils::runShellCommand("syscache \"pkg #system installedlist\"").join("").split(", ");
  //Assemble the list of top-level pkgs (do it with 1 syscache process command)
  QString tmp = " \"pkg #system local %1 rdependencies\"";
  QString cmd = "syscache";
  for(int i=0; i<allInstalled.length(); i++){ cmd.append( tmp.arg(allInstalled[i]) ); }
  QStringList rdeps = pcbsd::Utils::runShellCommand(cmd);
  //Evaluate the list results and pull out the top-level ones
  QStringList topList;
  for(int i=0; i<rdeps.length() && i<allInstalled.length(); i++){
    //qDebug() << "Line:" << allInstalled[i] << rdeps[i];
    if(rdeps[i].startsWith("[")){
      //empty list - this is a top-level pkg
      //qDebug() << "Found Item:" << allInstalled[i] << rdeps[i];
      topList << allInstalled[i];
    }
  }
  qDebug() << "Found top-level pkgs:" << topList;
  QString filepath = QDir::homePath()+"/installed-pkg-"+QDateTime::currentDateTime().toString("yyyy-MM-dd_HH-mm-ss")+".pkglist";
  pcbsd::Utils::writeTextFile(filepath, topList.join("\n"));
  QMessageBox::information(this, tr("List Created"), tr("Your list of current top-level packages was just created:")+"\n\n"+filepath);
}