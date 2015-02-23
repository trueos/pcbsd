#include "MainUI.h"

#include <QKeySequence>

MainUI::MainUI(bool debugmode) : QMainWindow(){
  //Setup UI
  DEBUG = debugmode;
  AUTHCOMPLETE = false; //not performed yet
  this->setWindowTitle(tr("AppCafe"));
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
  if(DEBUG){ qDebug() << "Done Loading Page:" << ok; }
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

  //Load the main page
  baseURL = BASEWEBURL;
  baseURL = baseURL.replace("<port>", port);
  if(usessl){ baseURL = baseURL.replace("http://","https://"); }
  if(DEBUG){ qDebug() << "Base URL:" << baseURL; }
  QString tmpURL = baseURL;
  if(!AUTHCOMPLETE){
    //Only perform the authorization if necessary
    QString authkey = pcbsd::Utils::getLineFromCommandOutput("pc-su /usr/local/share/appcafe/dispatcher-localauth 2>/dev/null").simplified();
    AUTHCOMPLETE = !authkey.isEmpty();
    if ( authkey.indexOf(":") != -1 )
      authkey = authkey.section(":", 1, 1);
    if(AUTHCOMPLETE){ tmpURL.append("/?setDisId="+authkey); }
  }
  //Now clear the history (if any)
  
  //Now load the page
  webview->load( QUrl(tmpURL) );
  webview->show();
}

void MainUI::GoBack(){
  webview->back();
}

void MainUI::GoForward(){
  webview->forward();
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