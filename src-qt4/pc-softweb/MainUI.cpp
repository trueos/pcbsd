#include "MainUI.h"

MainUI::MainUI(bool debugmode) : QMainWindow(){
  //Setup UI
  DEBUG = debugmode;
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
  // - Close Button  
    tb->addAction(QIcon(":icons/close.png"), tr("Close AppCafe"), this, SLOT(GoClose()) );
    
  //Setup the Main Interface
    webview = new QWebView(this);
    this->centralWidget()->layout()->addWidget(webview);
    if(webview->page()==0){ webview->setPage(new QWebPage(webview)); }
    webview->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);

    
  //Connect signals/slots
  connect(webview, SIGNAL(linkClicked(const QUrl&)), this, SLOT(LinkClicked(const QUrl&)) );
  connect(webview, SIGNAL(loadStarted()), this, SLOT(PageStartLoading()) );
  connect(webview, SIGNAL(loadProgress(int)), this, SLOT(PageLoadProgress(int)) );
  connect(webview, SIGNAL(loadFinished(bool)), this, SLOT(PageDoneLoading(bool)) );
  connect(webview->page()->networkAccessManager(), SIGNAL(sslErrors(QNetworkReply*, const QList<QSslError>&)), this, SLOT( authenticate(QNetworkReply*) ) );
  if(DEBUG){
    //connect(webview, SIGNAL(statusBarMessage(const QString&)), this, SLOT(StatusTextChanged(const QString&)) );
    connect(webview->page(), SIGNAL(linkHovered(const QString&, const QString&, const QString&)), this, SLOT(StatusTextChanged(const QString&)) );
  }
  this->statusBar()->setVisible(DEBUG);
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
  webview->load( QUrl(baseURL) );
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
