#include "mainUI.h"
#include "ui_mainUI.h" //the designer *.ui file
#include <QDir>

MainUI::MainUI() : QMainWindow(), ui(new Ui::MainUI()){
  ui->setupUi(this); //load the designer file
  settings = new QSettings( QDir::homePath()+"/.lumina/settings/Lumina-DE/desktopsettings.conf");
	
}

MainUI::~MainUI(){
	
}

void MainUI::slotSingleInstance(){
  //Make sure this window is visible
  this->showNormal();
  this->activateWindow();
  this->raise();
}