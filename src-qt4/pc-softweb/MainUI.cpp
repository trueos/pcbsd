#include "MainUI.h"

MainUI::MainUI() : QMainWindow(){
  //Setup UI
  bool DEBUG = true;
  this->setWindowTitle(tr("AppCafe"));
  this->setWindowIcon( QIcon(":icons/appcafe.png") );
  if(this->centralWidget()==0){ this->setCentralWidget( new QWidget(this) ); }
  this->centralWidget()->setLayout( new QVBoxLayout() );
  this->centralWidget()->layout()->setContentsMargins(0,0,0,0);
  this->setStatusBar(new QStatusBar());
  // - web view
    webview = new QWebView(this);
    this->centralWidget()->layout()->addWidget(webview);
    if(webview->page()==0){ webview->setPage(new QWebPage(webview)); }
    webview->page()->setLinkDelegationPolicy(QWebPage::DelegateExternalLinks);
  // - Progress bar
    progressBar = new QProgressBar(this);
    progressBar->setRange(0,100);
    this->centralWidget()->layout()->addWidget(progressBar);
    progressBar->setVisible(false); //start off invisible
  //Connect signals/slots
  connect(webview, SIGNAL(linkClicked(const QUrl&)), this, SLOT(LinkClicked(const QUrl&)) );
  connect(webview, SIGNAL(loadStarted()), this, SLOT(PageStartLoading()) );
  connect(webview, SIGNAL(loadProgress(int)), this, SLOT(PageLoadProgress(int)) );
  connect(webview, SIGNAL(loadFinished(bool)), this, SLOT(PageDoneLoading(bool)) );
  if(DEBUG){
    connect(webview, SIGNAL(statusBarMessage(const QString&)), this, SLOT(StatusTextChanged(const QString&)) );
    connect(webview->page(), SIGNAL(statusBarVisibilityChangeRequested(bool)), this, SLOT(SetStatusVisibility(bool)) );
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
  qDebug() << "Link Clicked:" << url.path();
  //if(url.path().startsWith(BASEWEBURL)){
    //Internal link - move to that page
    //webview->load(url);
    //webview->show();
  //}else{
    //Launch in a web browser
    QProcess::startDetached("xdg-open \""+url.path()+"\"");
  //}
}

void MainUI::PageStartLoading(){
  //qDebug() << "Start Loading Page:";
  progressBar->setVisible(true);
  progressBar->setValue(0);
}

void MainUI::PageLoadProgress(int cur){
  //qDebug() << " - load Progress:" << cur;
  progressBar->setValue(cur);
}

void MainUI::PageDoneLoading(bool ok){
  qDebug() << "Done Loading Page:" << ok;
  progressBar->setVisible(false);
  if(!ok){
    qDebug() << "URL:" << webview->url().path();
  }
}

void MainUI::StatusTextChanged(const QString &txt){
  //qDebug() << "Show Status Message:" << txt;
  this->statusBar()->showMessage(txt);
}

void MainUI::SetStatusVisibility(bool visible){
  this->statusBar()->setVisible(visible);
}
