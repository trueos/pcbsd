#include "pcdm-logindelay.h"
#include "ui_pcdm-logindelay.h"

loginDelay::loginDelay(int seconds, QString username) : QDialog(), ui(new Ui::loginDelay){
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
}

loginDelay::~loginDelay(){
	
}

void loginDelay::start(){
  this->show();
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
