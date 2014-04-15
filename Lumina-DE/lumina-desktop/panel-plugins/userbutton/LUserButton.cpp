//===========================================
//  Lumina-DE source code
//  Copyright (c) 2014, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#include "LUserButton.h"

LUserButton::LUserButton(QWidget *parent) : LTBWidget(parent){
  //Initialize the submenu's
  appMenu = LSession::appMenu();
  closeMenu = new QMenu(this);
  //Initialize the main menu
  mainMenu = new QMenu(this);
  //Initialize the home dir viewer
  homedir = new QDir( QDir::homePath() );
	homedir->setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
	homedir->setSorting(QDir::Name | QDir::LocaleAware);
  //Initialize the settings
  settings = new QSettings("Lumina-DE", "UserButton");

  //Setup the button icon/text
  this->setIcon( LXDG::findIcon("user-identity", ":/images/default-user.png") );
  this->setText( SYSTEM::user() );
  this->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  //this->setMenu(mainMenu);
	
  //Setup the signals/slots
  connect(this, SIGNAL(clicked()), this, SLOT(buttonClicked()) );
  connect(mainMenu, SIGNAL(triggered(QAction*)), this, SLOT(menuButtonClicked(QAction*)) );
}

LUserButton::~LUserButton(){
	
}

// ===========
//       PRIVATE
// ===========
void LUserButton::UpdateMenu(){
  //Clear the main menu and rebuild it
  mainMenu->clear();
    //add the app menu
    mainMenu->addMenu(appMenu);
    mainMenu->addSeparator();
    //Now add the home directory
    QAction *act = new QAction(LXDG::findIcon("user-home", ":/images/default-home.png"), tr("Home"), mainMenu);
	act->setWhatsThis( homedir->absolutePath() );
    mainMenu->addAction(act);
    //Now add the dirs in the home-dir
    QStringList hdirs = homedir->entryList(); //pre-sorted
    QStringList exList = settings->value("home-exclude-list", QStringList() << "GNUStep" << "bin" << "fontconfig").toStringList();
    for(int i=0; i<hdirs.length(); i++){
      if(exList.contains(hdirs[i]) ){ continue; } //this dir is excluded
      act = new QAction( "    "+hdirs[i] ,  mainMenu);
        act->setWhatsThis( homedir->absoluteFilePath(hdirs[i]) );
        //Now determine the icon it gets
        if(hdirs[i].toLower()=="downloads"){
	  act->setIcon(LXDG::findIcon("folder-downloads",":/images/default-netdir.png"));
	}else if(hdirs[i].toLower()=="documents"){
	  act->setIcon(LXDG::findIcon("folder-documents",":/images/default-textdir.png"));
	}else if(hdirs[i].toLower()=="desktop"){
	  act->setIcon(LXDG::findIcon("user-desktop",":/images/default-desktop.png"));
	}else if(hdirs[i].toLower()=="images"){
	  act->setIcon(LXDG::findIcon("folder-image",":/images/default-graphicsdir.png"));
	}else if(hdirs[i].toLower()=="music"){
	  act->setIcon(LXDG::findIcon("folder-sound",":/images/default-audiodir.png"));
	}else if(hdirs[i].toLower()=="videos"){
	  act->setIcon(LXDG::findIcon("folder-video",":/images/default-videodir.png"));
	}else if(hdirs[i].toLower()=="projects"){
	  act->setIcon(LXDG::findIcon("folder-development",":/images/default-dir"));
	}else{
	  act->setIcon(LXDG::findIcon("folder",":/images/default-dir.png"));
	}
      mainMenu->addAction(act);
    }
    mainMenu->addSeparator();
    //Now add the settings menu
    mainMenu->addMenu(LSession::settingsMenu());
    
    //Now add the close menu
    // -- rebuild it in case the icon theme has changed
    closeMenu->clear();
      closeMenu->setIcon( LXDG::findIcon("system-run", ":/images/default-shutdown.png") );
      closeMenu->setTitle( tr("System") );
      closeMenu->addAction( LXDG::findIcon("system-log-out",":/images/user-logout.png"), tr("Logout"), this, SLOT(Logout()) );
      closeMenu->addAction( LXDG::findIcon("system-reboot",":/images/system-restart.png"), tr("Restart"), this, SLOT(Restart()) );
      closeMenu->addAction( LXDG::findIcon("system-shutdown",":/images/system-shutdown.png"), tr("Shutdown"), this, SLOT(Shutdown()) );
    mainMenu->addMenu(closeMenu);
}

void LUserButton::goToDir(QString dir){
  QProcess::startDetached("lumina-open "+dir);
}

// ===============
//     PRIVATE SLOTS
// ===============
void LUserButton::Logout(){
  QCoreApplication::exit(0);
}

void LUserButton::Restart(){
  SYSTEM::restart();
  QCoreApplication::exit(0);
}

void LUserButton::Shutdown(){
  SYSTEM::shutdown();
  QCoreApplication::exit(0);
}
