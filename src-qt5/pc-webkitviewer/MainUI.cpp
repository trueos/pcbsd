#include "MainUI.h"

#include <QKeySequence>

MainUI::MainUI(bool debugmode, QString fileURL, QString title, QString iconpath) : QMainWindow(){
  //Setup UI
  DEBUG = debugmode;
  baseURL = fileURL;
  AUTHCOMPLETE = false; //not performed yet
  if(title.isEmpty()){
    if(baseURL.contains("://")){
      this->setWindowTitle(baseURL);
    }else{
      this->setWindowTitle(baseURL.section("/",-1));
    }
  }else{
    this->setWindowTitle(title);	    
  }
  this->resize(1024,600);
  //Check the given icon
  QIcon ico;
  if(!iconpath.isEmpty()){
    if(QFile::exists(iconpath)){ico = QIcon(iconpath); }
    else{ ico = QIcon::fromTheme(iconpath); }
  }
  if(ico.isNull()){ico = QIcon(":icons/webview.png"); }
  this->setWindowIcon( ico);
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
  connect(webview, SIGNAL(urlChanged(const QUrl&)), this, SLOT(LinkClicked(const QUrl&)) );
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
  if(url.toString().startsWith(baseURL) || url.toString().startsWith("file://"+baseURL) ){
    if(url==webview->url()){ return; }
    // Internal page - go there
    webview->load( url );
    webview->show();
  }else{
    //Launch in a web browser
    QProcess::startDetached("xdg-open \""+url.toString()+"\"");
    if(url == webview->url()){ webview->stop(); webview->back(); } // do not allow the webview to go outside the base URL
  }
}

void MainUI::PageStartLoading(){
  if(DEBUG){ qDebug() << "Start Loading Page:" << webview->url().toString(); }
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
  //Now clear the history (if any)
  
  //Now load the page
  if(baseURL.startsWith("/")){
    webview->load( QUrl::fromLocalFile(baseURL) );
  }else{
    webview->load( QUrl(baseURL) );
  }
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