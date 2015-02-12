#include "pcdm-logindelay.h"
#include "ui_pcdm-logindelay.h"

#include <QPoint>
#include <QDesktopWidget>

loginDelay::loginDelay(int seconds, QString username) : QDialog(0, Qt::Dialog | Qt::WindowStaysOnTopHint), ui(new Ui::loginDelay){
  ui->setupUi(this); //load the designer files
  continueLogin = false; //in case it is closed early somehow
  //Now setup the display
  ui->label_username->setText(username);
  ui->progressBar->setRange(0,seconds);
  ui->progressBar->setValue(0);
  //Now setup the internal timer
  delay = new QTimer(this);
	delay->setInterval(1000); //1 second intervals
	connect(delay, SIGNAL(timeout()), this, SLOT(updateTimer()) );
  //Now connect the pushbuttons
  connect(ui->pushCancel, SIGNAL(clicked()), this, SLOT(cancelLogin()) );
  connect(ui->pushContinue, SIGNAL(clicked()), this, SLOT(startLogin()) );
  //Center the dialog on the screen
  fillScreens();
}

loginDelay::~loginDelay(){
  //Also clean up all the background fillers
  for(int i=0; i<screens.length(); i++){
    delete screens[i];
  }
}

void loginDelay::start(){
  this->show();
  this->raise();
  delay->start();
}

void loginDelay::updateTimer(){
  ui->progressBar->setValue( ui->progressBar->value()+1 );
  //Check if the timer reached the end yet
  if(ui->progressBar->value() >= ui->progressBar->maximum()){
    delay->stop();
    startLogin();
  }
}

void loginDelay::cancelLogin(){
  delay->stop();
  continueLogin = false;
  this->close();
}

void loginDelay::startLogin(){
  delay->stop();
  continueLogin = true;
  this->close();	
}

void loginDelay::fillScreens(){
    //Set a background image on any other available screens
    QDesktopWidget *DE = QApplication::desktop();
    screens.clear();
    //Generate the background style sheet
    QString tmpIcon = ":/images/backgroundimage.jpg"; //always use the defult PCDM image (don't load theme for auto-login)
    QString bgstyle = "QWidget#BGSCREEN{border-image: url(BGIMAGE) stretch;}"; 
      bgstyle.replace("BGIMAGE", tmpIcon);
    //Now apply the background to all the screens   

    for(int i=0; i<DE->screenCount(); i++){
        //Just show a generic QWidget with the proper background image on every screen
	QWidget *screen = new QWidget(0, Qt::Window | Qt::WindowTransparentForInput | Qt::WindowDoesNotAcceptFocus | Qt::WindowStaysOnBottomHint);
	screen->setObjectName("BGSCREEN");
	screen->setGeometry( DE->screenGeometry(i) );
	screen->setStyleSheet(bgstyle);
	screen->show();
	screens << screen;
    }
    //Now center the login delay widget on the screen
    QPoint ctr = DE->screenGeometry(0).center();
    this->move( ctr.x()-(this->width()/2), ctr.y()-(this->height()/2) );
    QCursor::setPos( ctr );	  
}