#include "mainUI.h"
#include "ui_mainUI.h" //the designer *.ui file

MainUI::MainUI() : QMainWindow(), ui(new Ui::MainUI()){
  ui->setupUi(this); //load the designer file
  //Be careful about the QSettings setup, it must match the lumina-desktop setup
  QSettings::setPath(QSettings::NativeFormat, QSettings::UserScope, QDir::homePath()+"/.lumina");
  settings = new QSettings( QSettings::UserScope, "LuminaDE", "desktopsettings", this);
  qDebug() << "Settings File:" << settings->fileName();
  desktop = new QDesktopWidget();
  if(desktop->screenCount() == 1){
    ui->spin_screen->setValue(1);
    //Hide these since no other screens
    ui->label_screen->setVisible(false);
    ui->spin_screen->setVisible(false);
  }else{
    //Make sure this is only allows the current number of screens
    ui->spin_screen->setMaximum(desktop->screenCount());
  }

  //Setup the buttons signal/slot connections
  connect(ui->spin_screen, SIGNAL(valueChanged(int)), this, SLOT(loadCurrentSettings()) );
  connect(ui->tool_rmbackground, SIGNAL(clicked()), this, SLOT(removeBackground()) );
  connect(ui->tool_addbackground, SIGNAL(clicked()), this, SLOT(addBackground()) );
  
  //Now finish setting up the UI
  setupIcons();
  QTimer::singleShot(10, this, SLOT(loadCurrentSettings()) );
}

MainUI::~MainUI(){
	
}

void MainUI::slotSingleInstance(){
  //Make sure this window is visible
  this->showNormal();
  this->activateWindow();
  this->raise();
}

//================
//  PRIVATE FUNCTIONS
//================
void MainUI::setupIcons(){
  //Pull all the icons from the current theme using libLumina (LXDG)
  //  - Menu Items
  ui->actionSave_and_Quit->setIcon( LXDG::findIcon("document-save-all","") );
  ui->actionClose->setIcon( LXDG::findIcon("application-exit","") );
	
  //  - Background tab
  ui->tool_rmbackground->setIcon( LXDG::findIcon("list-remove","") );
  ui->tool_addbackground->setIcon( LXDG::findIcon("list-add","") );

  //  - General UI buttons
  ui->push_cancel->setIcon( LXDG::findIcon("dialog-cancel","") );
  ui->push_save->setIcon( LXDG::findIcon("document-save","") );
}

void MainUI::checkForChanges(){
  //to see whether to enable the save button
  // -- TO DO (this should make the save button enabled only if changes are available)
	
}

int MainUI::currentDesktop(){
  return ui->spin_screen->value()-1; //backend starts at 0, not 1
}

int MainUI::currentPanel(){
  return 0; // - TO DO	
}

void MainUI::addNewBackgroundFile(QString filepath){
  QListWidgetItem *item = new QListWidgetItem(ui->list_backgrounds);
	item->setText(filepath.section("/",-1));
	item->setToolTip(filepath);
	item->setWhatsThis(filepath); //save the full path in this variable
	item->setIcon( QIcon(filepath) );
}

//================
//    PRIVATE SLOTS
//================
//General Utility Functions
void MainUI::loadCurrentSettings(){
  settings->sync();
  QString DPrefix = "desktop-"+QString::number(currentDesktop())+"/";
  QString PPrefix = "panel"+QString::number(currentDesktop())+"."+QString::number(currentPanel())+"/";
  //The Background tab
    //Setup the bg preview image size (correct aspect ratio for the current screen)
      int icoWidth = ui->list_backgrounds->width()/3;
      double ratio = desktop->screenGeometry(currentDesktop()).height()/ ( (double) desktop->screenGeometry(currentDesktop()).width() );
      ui->list_backgrounds->setIconSize( QSize(icoWidth, icoWidth*ratio) );
    //Load the background files
    QStringList bgs = settings->value(DPrefix+"background/filelist", QStringList()<<"default").toStringList();
    qDebug() << "Backgrounds:" << DPrefix << bgs;
    ui->list_backgrounds->clear();
    for(int i=0; i<bgs.length(); i++){
      if(bgs[i]=="default"){ bgs[i]=DEFAULTBG; }
      if(QFile::exists(bgs[i])){
        addNewBackgroundFile(bgs[i]);
      }
    }
    if(bgs.length() <= 1){ ui->radio_singleBG->setChecked(true); }
    else{ ui->radio_rotateBG->setChecked(true); }
    ui->spin_bgRotateMin->setValue( settings->value(DPrefix+"background/minutesToChange", 5).toInt() );
    
}

//Background Tab Functions
void MainUI::addBackground(){
	
}

void MainUI::removeBackground(){
	
}

