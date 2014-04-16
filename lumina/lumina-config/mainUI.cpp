#include "mainUI.h"
#include "ui_mainUI.h" //the designer *.ui file

MainUI::MainUI() : QMainWindow(), ui(new Ui::MainWindow()){
  ui->setupUi(this); //load the designer file
	
}

MainUI::~MainUI(){
	
}

void MainUI::slotSingleInstance(){
  //Make sure this window is visible
  this->showNormal();
  this->activateWindow();
  this->raise();
}