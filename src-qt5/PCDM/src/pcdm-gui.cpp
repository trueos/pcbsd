/* PCDM Login Manager:
*  Written by Ken Moore (ken@pcbsd.org) 2012/2013
*  Copyright(c) 2013 by the PC-BSD Project
*  Available under the 3-clause BSD license
*/

#include <QProcess>
#include <QTimer>
#include <QGraphicsPixmapItem>
#include <QStyle>
#include <pcbsd-utils.h>

#include "pcdm-gui.h"
#include "pcdm-backend.h"
#include "fancySwitcher.h"

bool DEBUG_MODE=false;
QString VIRTUALKBDBIN="/usr/local/bin/xvkbd -compact";

PCDMgui::PCDMgui() : QMainWindow()
{
    //Load the Theme
    loadTheme();
    //Create the GUI based upon the current Theme
    createGUIfromTheme();
    //Put the background image on all the other screens
    fillExtraScreens();
    //Now make sure that the login widget has keyboard focus
    loginW->resetFocus();
    this->setObjectName("PCDM-background");
}

PCDMgui::~PCDMgui()
{
    //delete ui;
}

void PCDMgui::loadTheme(){
  currentTheme = new ThemeStruct();
  QString themeFile = Config::themeFile();
  if(!QFile::exists(themeFile)){ themeFile = ":samples/themes/default/default.theme"; }
  currentTheme->loadThemeFile( themeFile );
  //Check the Theme, using default values as necessary
  QStringList invalid = currentTheme->invalidItems();
  if( !invalid.isEmpty() ){
    ThemeStruct* defaultTheme = new ThemeStruct();
    defaultTheme->loadThemeFile( ":samples/themes/default/default.theme" );
    for( int i=0; i<invalid.length(); i++){
      //Replace the invalid items with the defaults
      qDebug() << "Invalid Theme Item, Using defaults:" << invalid[i];
      currentTheme->importItem( invalid[i] , defaultTheme->exportItem(invalid[i]) );
    }
  }
  
}

void PCDMgui::createGUIfromTheme(){
  QString style;
  QString tmpIcon; //used for checking image files before loading them
  //Set the background image
  if(DEBUG_MODE){ qDebug() << "Setting Background Image"; }
  if( currentTheme->itemIsEnabled("background") ){
    tmpIcon = currentTheme->itemIcon("background");
    if( tmpIcon.isEmpty() || !QFile::exists(tmpIcon) ){ tmpIcon = ":/images/backgroundimage.jpg"; }
    //use "border-image" instead of "background-image" to stretch rather than tile the image
    QString bgstyle = "QMainWindow#PCDM-background {border-image: url(BGIMAGE) stretch;}"; 
    bgstyle.replace("BGIMAGE", tmpIcon);
    style.append(bgstyle);
  }
  //Set the application style sheet
  style.append(" "+ currentTheme->styleSheet() );
  this->setStyleSheet( style.simplified() );

  //Check for whether the desktop switcher is on the toolbar or not
  simpleDESwitcher = (currentTheme->itemValue("desktop") == "simple");
  
  //get the default translation directory
  if(DEBUG_MODE){ qDebug() << "Load Translations"; }
  translationDir = "/usr/local/share/PCDM/i18n/";
  //Fill the translator
  m_translator = new QTranslator();
  //Create the Toolbar
  toolbar = new QToolBar();
  //Add the Toolbar to the window
  if(DEBUG_MODE){ qDebug() << "Create Toolbar"; }
    //use the theme location   
    QString tarea = currentTheme->itemValue("toolbar");
    if(tarea == "left"){
      this->addToolBar( Qt::LeftToolBarArea, toolbar ); 	    
    }else if( tarea=="top"){
      this->addToolBar( Qt::TopToolBarArea, toolbar );  	    
    }else if(tarea=="right"){
      this->addToolBar( Qt::RightToolBarArea, toolbar );   	    
    }else{ //bottom is default
      this->addToolBar( Qt::BottomToolBarArea, toolbar );     	
    }
    //Set toolbar flags
    toolbar->setVisible(true);
    toolbar->setMovable(false);
    toolbar->setFloatable(false);
    toolbar->setContextMenuPolicy(Qt::PreventContextMenu); //make sure no right-click menu
    //Set the default style and icon sizes
    QString tstyle = currentTheme->itemIcon("toolbar").toLower(); //use the theme style
    if(tstyle=="textonly"){ toolbar->setToolButtonStyle(Qt::ToolButtonTextOnly); }
    else if(tstyle=="textbesideicon"){ toolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon); }
    else if(tstyle=="textundericon"){ toolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon); }
    else{ toolbar->setToolButtonStyle(Qt::ToolButtonIconOnly); } //default to icon only
    
    toolbar->setIconSize( currentTheme->itemIconSize("toolbar") ); //use theme size
    toolbar->setFocusPolicy( Qt::NoFocus );
  //Populate the Toolbar with items (starts at leftmost/topmost)
    //----Virtual Keyboard
    if(currentTheme->itemIsEnabled("vkeyboard") ){
      if(DEBUG_MODE){ qDebug() << " - Create Virtual Keyboard Button"; }
      tmpIcon = currentTheme->itemIcon("vkeyboard");
      if(!QFile::exists(tmpIcon) || tmpIcon.isEmpty() ){ tmpIcon=":/images/input-keyboard.png"; }
      virtkeyboardButton = new QAction( QIcon(tmpIcon),tr("Virtual Keyboard"),this );
      toolbar->addAction(virtkeyboardButton);
      connect( virtkeyboardButton, SIGNAL(triggered()), this, SLOT(slotPushVirtKeyboard()) );
    }else{
      virtkeyboardButton = new QAction(this);
    }

    //----Locale Switcher
    if(DEBUG_MODE){ qDebug() << " - Create Locale Button"; }
    tmpIcon = currentTheme->itemIcon("locale");
    if(!QFile::exists(tmpIcon) || tmpIcon.isEmpty() ){ tmpIcon=":/images/language.png"; }
    localeButton = new QAction( QIcon(tmpIcon),tr("Locale"),this );
    toolbar->addAction(localeButton);
    connect( localeButton, SIGNAL(triggered()), this, SLOT(slotChangeLocale()) );
    
    //----Keyboard Layout Switcher
    if(DEBUG_MODE){ qDebug() << " - Create Keyboard Layout Button"; }
    tmpIcon = currentTheme->itemIcon("keyboard");
    if(!QFile::exists(tmpIcon) || tmpIcon.isEmpty() ){ tmpIcon=":/images/keyboard.png"; }
    keyboardButton = new QAction( QIcon(tmpIcon),tr("Keyboard Layout"),this );
    toolbar->addAction(keyboardButton);
    connect( keyboardButton, SIGNAL(triggered()), this, SLOT(slotChangeKeyboardLayout()) );

    //----Add a spacer
    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
    toolbar->addWidget(spacer);
        
    //----System Shutdown/Restart
    if(DEBUG_MODE){ qDebug() << " - Create System Button"; }
    tmpIcon = currentTheme->itemIcon("system");
    if(!QFile::exists(tmpIcon) || tmpIcon.isEmpty() ){ tmpIcon=":/images/system.png"; }
    QAction* act = new QAction( QIcon(tmpIcon),tr("System"),this );
    systemButton = new QToolButton();
    systemButton->setDefaultAction(act);
    systemMenu = new QMenu();
    toolbar->addWidget(systemButton);
    systemButton->setPopupMode( QToolButton::InstantPopup );
    systemButton->setFocusPolicy( Qt::NoFocus );
    
  //Create the grid layout
  QGridLayout* grid = new QGridLayout;
  if(DEBUG_MODE){ qDebug() << "Fill Desktop Area"; }
  //Populate the grid with items
    //----Header Image
    QLabel* header = new QLabel; 
    if( currentTheme->itemIsEnabled("header") ){
      if(DEBUG_MODE){ qDebug() << " - Create Header"; }
      tmpIcon = currentTheme->itemIcon("header");
      if(!QFile::exists(tmpIcon) || tmpIcon.isEmpty() ){ tmpIcon=":/images/banner.png"; }
      QPixmap tmp( tmpIcon );
      header->setPixmap( tmp.scaled( currentTheme->itemIconSize("header") ) );
      grid->addWidget( header, currentTheme->itemLocation("header","row"), \
                      currentTheme->itemLocation("header","col"), \
                      currentTheme->itemLocation("header","rowspan"), \
                      currentTheme->itemLocation("header","colspan"), Qt::AlignCenter);
    }
    
    //Username/Password/Login widget
    if(DEBUG_MODE){ qDebug() << " - Create Login Widget"; }
    loginW = new LoginWidget;
    loginW->setUsernames(Backend::getSystemUsers()); //add in the detected users
    QString lastUser = Backend::getLastUser();
    if(!lastUser.isEmpty()){ //set the previously used user
    	loginW->setCurrentUser(lastUser); 
    } 
    //Set Icons from theme
    tmpIcon = currentTheme->itemIcon("login");
    if(!QFile::exists(tmpIcon) || tmpIcon.isEmpty() ){ tmpIcon=":/images/next.png"; }
    loginW->changeButtonIcon("login",tmpIcon, currentTheme->itemIconSize("login"));
    tmpIcon = currentTheme->itemIcon("user");
    slotUserChanged(loginW->currentUsername()); //Make sure that we have the correct user icon
    tmpIcon = currentTheme->itemIcon("password");
    if(!QFile::exists(tmpIcon) || tmpIcon.isEmpty() ){ tmpIcon=":/images/password.png"; }
    loginW->changeButtonIcon("pwview",tmpIcon, currentTheme->itemIconSize("password"));
    //Enable/disable the password view functionality
    loginW->allowPasswordView( Config::allowPasswordView() );
    loginW->allowUserSelection( Config::allowUserSelection() );
    //Add item to the grid
    grid->addWidget( loginW, currentTheme->itemLocation("login","row"), \
                      currentTheme->itemLocation("login","col"), \
                      currentTheme->itemLocation("login","rowspan"), \
                      currentTheme->itemLocation("login","colspan"), Qt::AlignCenter);
    //Connect the signals/slots
    connect(loginW,SIGNAL(loginRequested(QString,QString)),this,SLOT(slotStartLogin(QString,QString)));
    connect(loginW,SIGNAL(escapePressed()),this,SLOT(slotShutdownComputer()));
    connect(loginW,SIGNAL(UserSelected(QString)), this, SLOT(slotUserSelected(QString)) );
    connect(loginW,SIGNAL(UserChanged(QString)), this, SLOT(slotUserChanged(QString)) );
    
    //----Desktop Environment Switcher
    if(simpleDESwitcher){
      loginW->setDesktopIconSize(currentTheme->itemIconSize("desktop"));
    }else{
      if(DEBUG_MODE){ qDebug() << " - Create DE Switcher"; }
      //Create the switcher
      deSwitcher = new FancySwitcher(this, !currentTheme->itemIsVertical("desktop") );
      QSize deSize = currentTheme->itemIconSize("desktop");
      deSwitcher->setIconSize(deSize.height());
      tmpIcon = currentTheme->itemIcon("nextde");
      if( !tmpIcon.isEmpty() && QFile::exists(tmpIcon) ){ deSwitcher->changeButtonIcon("forward", tmpIcon); }
      tmpIcon = currentTheme->itemIcon("previousde");
      if( !tmpIcon.isEmpty() && QFile::exists(tmpIcon) ){ deSwitcher->changeButtonIcon("back", tmpIcon); }
      //Figure out if we need to smooth out the animation
      deSwitcher->setNumberAnimationFrames("4"); 
      //NOTE: A transparent widget background slows the full animation to a crawl with a stretched background image

      grid->addWidget( deSwitcher, currentTheme->itemLocation("desktop","row"), \
                      currentTheme->itemLocation("desktop","col"), \
                      currentTheme->itemLocation("desktop","rowspan"), \
                      currentTheme->itemLocation("desktop","colspan"), Qt::AlignCenter);
    }
    //----WINDOW SPACERS
    QStringList spacers = currentTheme->getSpacers();
    for(int i=0; i<spacers.length(); i++){
      bool isVertical = (spacers[i].section("::",0,0) == "true");
      int row = spacers[i].section("::",1,1).toInt();
      int col = spacers[i].section("::",2,2).toInt();
      //qDebug() << "Add Spacer:" << isVertical << row << col;
      if(isVertical){
        grid->setRowStretch(row,1);
      }else{ //horizontal
        grid->setColumnStretch(col,1);
      }
    }

  //Connect the grid to the Window
    QWidget* widget = new QWidget;
    widget->setLayout(grid);
    this->setCentralWidget(widget);
    
  //Now translate the UI and set all the text
  if(DEBUG_MODE){ qDebug() << " - Fill GUI with data"; }
  retranslateUi();
  if(DEBUG_MODE){ qDebug() << "Done with initialization"; }

}

void PCDMgui::fillExtraScreens(){
    //Set a background image on any other available screens
    QDesktopWidget *DE = QApplication::desktop();
    screens.clear();
    //Generate the background style sheet
    QString tmpIcon = currentTheme->itemIcon("background");
    if( tmpIcon.isEmpty() || !QFile::exists(tmpIcon) ){ tmpIcon = ":/images/backgroundimage.jpg"; }
    QString bgstyle = "border-image: url(BGIMAGE) stretch;"; 
      bgstyle.replace("BGIMAGE", tmpIcon);
    //Now apply the background to all the other screens    
    for(int i=0; i<DE->screenCount(); i++){
      if(i != DE->screenNumber(this)){
        //Just show a generic QWidget with the proper background image on every other screen
	QWidget *screen = new QWidget(0, Qt::X11BypassWindowManagerHint);
	screen->setGeometry( DE->screenGeometry(i) );
	screen->setStyleSheet(bgstyle);
	screen->show();
	screens << screen;
      }else{
        //Now move the mouse cursor over this window (fix for multi-monitor setups)
        QCursor::setPos( DE->screenGeometry(i).center() );	      
      }
    }	
}

void PCDMgui::slotStartLogin(QString displayname, QString password){
  //Get user inputs
  QString username = Backend::getUsernameFromDisplayname(displayname);
  QString desktop;
  if(simpleDESwitcher){
    desktop = loginW->currentDE();
  }else{
    desktop = deSwitcher->currentItem();
  }
  QLocale currLocale = this->locale();
  QString lang = currLocale.name();
  //Disable user input while confirming login
  loginW->setEnabled(false);
  if(!simpleDESwitcher){ deSwitcher->setEnabled(false); }
  toolbar->setEnabled(false);
  //Try to login
  emit xLoginAttempt(username, password, desktop, lang);
  //Return signals are connected to the slotLogin[Success/Failure] functions
  
}

void PCDMgui::slotLoginSuccess(){
  /*QString de;
  if(simpleDESwitcher){ de = loginW->currentDE(); }
  else{ de = deSwitcher->currentItem(); }
  saveLastLogin( loginW->currentUsername(), de );*/
  slotClosePCDM(); //now start to close down the PCDM GUI
}

void PCDMgui::slotLoginFailure(){
  //Display an info box that the login failed
  QMessageBox notice(this);
    notice.setWindowTitle(tr("Invalid Username/Password"));
    notice.setIcon(QMessageBox::Warning);
    notice.setText(tr("Username/Password combination is invalid, please try again."));
    notice.setInformativeText("("+tr("Tip: Make sure that caps-lock is turned off.")+")");
    notice.setStandardButtons(QMessageBox::Ok);
    notice.setDefaultButton(QMessageBox::Ok);
    notice.setFocus(Qt::PopupFocusReason);
    notice.exec();
  
  //Re-Enable user input
  loginW->setEnabled(true);
  loginW->setFocus(Qt::ActiveWindowFocusReason); //window is gone - focus back on login widget
  loginW->resetFocus("password");
  if(!simpleDESwitcher){ deSwitcher->setEnabled(true); }
  toolbar->setEnabled(true);
}

void PCDMgui::slotUserChanged(QString newuser){
  if( !newuser.isEmpty() ){
    //Try to load the custom user icon
    QString tmpIcon = Backend::getUserHomeDir(newuser) + "/.loginIcon.png";
    if(!QFile::exists(tmpIcon) ){ tmpIcon= currentTheme->itemIcon("user"); }
    if(!QFile::exists(tmpIcon) || tmpIcon.isEmpty() ){ tmpIcon=":/images/user.png"; }
    loginW->changeButtonIcon("display",tmpIcon, currentTheme->itemIconSize("user"));
  }
}

void PCDMgui::slotUserSelected(QString newuser){
  if(DEBUG_MODE){ qDebug() << "User selection changed:" << newuser; }
  if(newuser.isEmpty()){
    if(!simpleDESwitcher){ deSwitcher->setVisible(false); }
  }else{
    if(!simpleDESwitcher){ deSwitcher->setVisible(true); }
    //Try to load the custom user icon
    slotUserChanged(newuser);
  }
  retranslateUi();
}

void PCDMgui::slotShutdownComputer(){
  QMessageBox verify;
  verify.setWindowTitle(tr("System Shutdown"));
  verify.setText(tr("You are about to shut down the system."));
  verify.setInformativeText(tr("Are you sure?"));
  verify.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
  verify.setDefaultButton(QMessageBox::No);
  int ret = verify.exec();

  if(ret == QMessageBox::Yes){
    Backend::log("PCDM: Shutting down computer");
    system("shutdown -p now");
    QCoreApplication::exit(1); //flag that this is not a normal GUI close
  }
}

void PCDMgui::slotRestartComputer(){
  QMessageBox verify;
  verify.setWindowTitle(tr("System Restart"));
  verify.setText(tr("You are about to restart the system."));
  verify.setInformativeText(tr("Are you sure?"));
  verify.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
  verify.setDefaultButton(QMessageBox::No);
  int ret = verify.exec();

  if(ret == QMessageBox::Yes){
    Backend::log("PCDM: Restarting computer");
    system("shutdown -r now");
    QCoreApplication::exit(1); //flag that this is not a normal GUI close
  }
}

void PCDMgui::slotClosePCDM(){
  system("killall -9 xvkbd"); //be sure to close the virtual keyboard
  for(int i=0; i<screens.length(); i++){ screens[i]->close(); } //close all the other screens
  QCoreApplication::exit(0);
  close();
}

void PCDMgui::slotChangeLocale(){
  //Open the selector
  wLoc = new widgetLocale();
  QLocale currLocale = this->locale();
  Backend::log("Current PCDM Locale: " + currLocale.name() );
  wLoc->setCurrentLocale(currLocale.name());
  wLoc->setWindowModality(Qt::ApplicationModal);
  wLoc->show();
  wLoc->raise();

  //Connect the language changed signal
  connect(wLoc,SIGNAL(languageChanged(QString)),this,SLOT(slotLocaleChanged(QString)) );
}

void PCDMgui::slotLocaleChanged(QString langCode){
  //Get the valid Locale code
  QString translationFile;
  if(!QFile::exists(translationDir+"PCDM_"+langCode+".qm") && langCode!="en_US"){
    langCode = langCode.section("_",0,0);
    if(!QFile::exists(translationDir+"PCDM_"+langCode+".qm")){
      if(!QFile::exists(translationDir+"qt_"+langCode+".qm")){
      	Backend::log("Desired locale is not a valid translation: " + langCode);
      	return;
      }else{
        translationFile = translationDir+"qt_"+langCode+".qm";	    
      }
    }else{
      translationFile = translationDir+"PCDM_"+langCode+".qm";	    
    }
  }else{
    translationFile = translationDir+"PCDM_"+langCode+".qm";
  }	
	
  Backend::log("Changing localization to " + langCode);

  //Alternate method for changing Locale
  QLocale locale(langCode);
  this->setLocale(locale);
  //this->setDefaultLocale(locale);
  //Change the translator
  if( !m_translator->isEmpty() ){
    Backend::log("Remove the translator");
    QCoreApplication::removeTranslator(m_translator);
  }

  if(m_translator->load(translationFile)){
    Backend::log("Install the new translator: "+translationFile);
    QCoreApplication::installTranslator(m_translator);	  
  }
  //Re-draw the interface
  retranslateUi();
  
  Backend::log("Current Locale after change: " + this->locale().name() );
  //Now save the locale to the system file
  QString lang, kMod, kLay, kVar;
  Backend::readDefaultSysEnvironment(lang,kMod,kLay,kVar); //need the keyboard settings
  Backend::saveDefaultSysEnvironment(langCode,kMod,kLay,kVar);
}

void PCDMgui::slotChangeKeyboardLayout(){
  //Fill a couple global variables
  QStringList kModels = Backend::keyModels();
  QStringList kLayouts = Backend::keyLayouts();
  //Startup the GUI
  wKey = new widgetKeyboard();
  wKey->programInit(kModels, kLayouts);
  wKey->setWindowModality(Qt::ApplicationModal);
  wKey->show();
  wKey->raise();
}

// Start xvkbd
void PCDMgui::slotPushVirtKeyboard(){
   QString cmd = "killall -9 "+VIRTUALKBDBIN.section(" ",0,0).section("/",-1)+"; "+VIRTUALKBDBIN+" &";
   qDebug() << "Starting Virtual Keyboard:";
   qDebug() << " - CMD: "+cmd;
   system( cmd.toUtf8() );
   loginW->resetFocus("password");
}

void PCDMgui::retranslateUi(){
  //All the text-setting for the main interface needs to be done here
  //virtual keyboard button
  //if(currentTheme->itemIsEnabled("vkeyboard")){
    virtkeyboardButton->setToolTip(tr("Virtual Keyboard"));
    virtkeyboardButton->setText(tr("Virtual Keyboard"));
  //}
  //locale switcher button
  localeButton->setToolTip(tr("Change locale"));
  localeButton->setText(tr("Locale"));
  //keyboard layout button
  keyboardButton->setToolTip(tr("Change Keyboard Layout"));
  keyboardButton->setText(tr("Keyboard Layout"));
  //system button
  systemButton->defaultAction()->setToolTip(tr("Shutdown the computer"));
  systemButton->defaultAction()->setText(tr("System"));
  //Menu entries for system button
    systemMenu->clear();	
    systemMenu->addAction( tr("Restart"),this, SLOT(slotRestartComputer()) );
    systemMenu->addAction( tr("Shut Down"), this, SLOT(slotShutdownComputer()) );
    if(DEBUG_MODE){systemMenu->addAction( tr("Close PCDM"), this, SLOT(slotClosePCDM()) ); }
    systemButton->setMenu(systemMenu);
  //The main login widget
  if(hostname.isEmpty()){
    //Find the system hostname
    hostname = pcbsd::Utils::runShellCommand("hostname").join(" ").simplified();
    loginW->displayHostName(hostname);	  
  }
  loginW->retranslateUi();
  
  //The desktop switcher
  
    //Get the new desktop list (translated)
    QStringList deList = Backend::getAvailableDesktops();
    QString lastDE = Backend::getLastDE(loginW->currentUsername());
    //Organize the desktop list alphabetically
    QStringList DEL;
    for(int i=0; i<deList.length(); i++){
      //Check the icon
        QString deIcon = Backend::getDesktopIcon(deList[i]);
	if( deIcon.isEmpty() ){ deIcon = currentTheme->itemIcon("desktop"); } //set the default icon if none given
        if( !QFile::exists(deIcon) ){ deIcon = ":/images/desktop.png"; }
      QString entry = deList[i] +":::"+deIcon+":::"+Backend::getDesktopComment(deList[i]);
      DEL << entry;
      if(lastDE.isEmpty()){ lastDE = deList[i]; } //grab the highest-priority DE if empty
    }
    DEL.sort(); //make it alphabetical
    //Now fill the switcher
    if(!simpleDESwitcher){
      deSwitcher->removeAllItems();
      for(int i=0; i<DEL.length(); i++){
	//Now add the item
	deSwitcher->addItem( DEL[i].section(":::",0,0), DEL[i].section(":::",1,1), DEL[i].section(":::",2,2) );
      }
      //Set the switcher to the last used desktop environment
      if( !lastDE.isEmpty() ){ deSwitcher->setCurrentItem(lastDE); }

    }else{
      //Simple switcher on the login widget
      QStringList deNames, deIcons, deInfo;
      for(int i=0; i<DEL.length(); i++){
        deNames << DEL[i].section(":::",0,0);
	deIcons << DEL[i].section(":::",1,1);
	deInfo << DEL[i].section(":::",2,2);
      }
      loginW->setDesktops(deNames, deIcons, deInfo);
      //Set the switcher to the last used desktop environment
      loginW->setCurrentDE(lastDE);
    }

}

