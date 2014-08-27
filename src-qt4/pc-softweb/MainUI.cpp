#include "MainUI.h"

MainUI::MainUI() : QMainWindow(){
  //Setup UI
  DEBUG = true;
  this->setWindowTitle(tr("AppCafe"));
  this->resize(1000,600);
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
  // - web view
    webview = new QWebView(this);
    this->centralWidget()->layout()->addWidget(webview);
    if(webview->page()==0){ webview->setPage(new QWebPage(webview)); }
    webview->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
  // - Progress bar
    progressBar = new QProgressBar(this);
    progressBar->setRange(0,100);
    progA = tb->addWidget(progressBar); //add it to the end of the toolbar
    progA->setVisible(false); //start off invisible
  //Connect signals/slots
  connect(webview, SIGNAL(linkClicked(const QUrl&)), this, SLOT(LinkClicked(const QUrl&)) );
  connect(webview, SIGNAL(loadStarted()), this, SLOT(PageStartLoading()) );
  connect(webview, SIGNAL(loadProgress(int)), this, SLOT(PageLoadProgress(int)) );
  connect(webview, SIGNAL(loadFinished(bool)), this, SLOT(PageDoneLoading(bool)) );
  if(DEBUG){
    //connect(webview, SIGNAL(statusBarMessage(const QString&)), this, SLOT(StatusTextChanged(const QString&)) );
    connect(webview->page(), SIGNAL(linkHovered(const QString&, const QString&, const QString&)), this, SLOT(StatusTextChanged(const QString&)) );
  }
  this->statusBar()->setVisible(DEBUG);
  //Load the main page
  webview->load( QUrl(BASEWEBURL) );
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
  if(url.toString().startsWith(BASEWEBURL)){
    //Internal link - move to that page
    webview->load(url);
    webview->show();
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

void MainUI::StatusTextChanged(const QString &txt){
  //qDebug() << "Show Status Message:" << txt;
  this->statusBar()->showMessage(txt);
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
