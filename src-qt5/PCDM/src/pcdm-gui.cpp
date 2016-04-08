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
    this->setObjectName("PCDM-background");
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnBottomHint);
    this->setCentralWidget(new QWidget(this));
    QApplication::setActiveWindow(this);
    //Load the Theme
    loadTheme();
    //Create the base widgets for the window and make sure they cover one screen at a time
    fillScreens();
    //Create the GUI based upon the current Theme
    createGUIfromTheme();
    //Now make sure that the login widget has keyboard focus
    loginW->resetFocus();
    loginW->activateWindow();
    pcTimer = new QTimer(this);
	pcTimer->setInterval(15000); //every 15 seconds
	connect(pcTimer, SIGNAL(timeout()), this, SLOT(LoadAvailableUsers()) );
    if(!pcAvail.isEmpty()){ pcTimer->start(); } //LoadAvailableUsers was already run once
    connect(QApplication::desktop(), SIGNAL(screenCountChanged(int)), this, SLOT(slotScreensChanged()) );
   // connect(QApplication::desktop(), SIGNAL(primaryScreenChanged()), this, SLOT(slotScreensChanged()) );
    connect(QApplication::desktop(), SIGNAL(resized(int)), this, SLOT(slotScreensChanged()) );

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
  QWidget *mainscreen = 0;
  qDebug() << "Screens:" << QApplication::desktop()->screenCount() << screens.length();
  if(QApplication::desktop()->primaryScreen() < screens.length()){
    //Primary screen set - use this
    qDebug() << " - Primary Screen:" << QApplication::desktop()->primaryScreen();
    mainscreen = screens[QApplication::desktop()->primaryScreen()];
  }else{
    //No primary set? just use the left-most one
    for(int i=0; i<screens.length(); i++){
      if(screens[i]->x()==0){ 
	qDebug() << " - Use Main screen:" << i;
	mainscreen = screens[i]; 
      }
    }
    if(mainscreen==0){
      mainscreen = screens.first(); //fallback - just use the first screen
    }
  }
  
  //Define the default icon size
  int perc = qRound(mainscreen->height()*0.035); //use 3.5% of the screen height
  defIconSize = QSize(perc,perc);
  //Set the background image
  if(DEBUG_MODE){ qDebug() << "Setting Background Image"; }
  if( currentTheme->itemIsEnabled("background") ){
    tmpIcon = currentTheme->itemIcon("background");
    if( tmpIcon.isEmpty() || !QFile::exists(tmpIcon) ){ tmpIcon = ":/images/backgroundimage.jpg"; }
    //use "border-image" instead of "background-image" to stretch rather than tile the image
    QString bgstyle = "QWidget#BGSCREEN{border-image: url(BGIMAGE) stretch;}"; 
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
  translationDir = "/usr/local/share/pcbsd/i18n/";
  //Fill the translator
  m_translator = new QTranslator();
  //Create the Toolbar
  toolbar = new QToolBar(mainscreen);
  //Add the Toolbar to the window
  QSize tmpsz = currentTheme->itemIconSize("toolbar");
    if(!tmpsz.isValid()){ tmpsz = defIconSize; }
    toolbar->setIconSize( tmpsz ); //use theme size
  if(DEBUG_MODE){ qDebug() << "Create Toolbar"; }
    //use the theme location   
    QString tarea = currentTheme->itemValue("toolbar");
    if(tarea == "left"){
      toolbar->setGeometry(0,0,tmpsz.width(), mainscreen->height());
    }else if( tarea=="top"){
      //this->addToolBar( Qt::TopToolBarArea, toolbar );  	   
      toolbar->setGeometry(0,0,mainscreen->width(), tmpsz.height());
      //toolbar->setFixedWidth(mainscreen->width());
    }else if(tarea=="right"){
      toolbar->setGeometry(mainscreen->width()-tmpsz.width(),0,tmpsz.width(), mainscreen->height());
      //this->addToolBar( Qt::RightToolBarArea, toolbar );   	    
    }else{ //bottom is default
      toolbar->setGeometry(0,mainscreen->height()-tmpsz.height(),mainscreen->width(), tmpsz.height());
      //this->addToolBar( Qt::BottomToolBarArea, toolbar ); 	
      //toolbar->setFixedWidth(mainscreen->width());
    }
    //Now make sure the toolbar cannot be resize by the layout
    toolbar->setFixedSize(toolbar->size());
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
    //Set Icons from theme
    // - login icon
    tmpIcon = currentTheme->itemIcon("login");
    tmpsz = currentTheme->itemIconSize("login");
    if(!tmpsz.isValid()){ tmpsz = defIconSize; }
    if(!QFile::exists(tmpIcon) || tmpIcon.isEmpty() ){ tmpIcon=":/images/next.png"; }
    loginW->changeButtonIcon("login",tmpIcon, tmpsz);
    // - stealth session icon
    tmpIcon = currentTheme->itemIcon("anonlogin");
    tmpsz = currentTheme->itemIconSize("login");
    if(!tmpsz.isValid()){ tmpsz = defIconSize; }
    if(!QFile::exists(tmpIcon) || tmpIcon.isEmpty() ){ tmpIcon=":/images/next-stealth.png"; }
    loginW->changeButtonIcon("anonlogin", tmpIcon, tmpsz);
    // - user/password icons
    tmpIcon = currentTheme->itemIcon("user");
    slotUserChanged(loginW->currentUsername()); //Make sure that we have the correct user icon
    tmpIcon = currentTheme->itemIcon("password");
    tmpsz = currentTheme->itemIconSize("password");
    if(!tmpsz.isValid()){ tmpsz = defIconSize; }
    if(!QFile::exists(tmpIcon) || tmpIcon.isEmpty() ){ tmpIcon=":/images/password.png"; }
    loginW->changeButtonIcon("pwview",tmpIcon, tmpsz);
    // -personacrypt password icon
    tmpIcon = currentTheme->itemIcon("encdevice");
    tmpsz = currentTheme->itemIconSize("device");
    if(!tmpsz.isValid()){ tmpsz = defIconSize; }
    if(!QFile::exists(tmpIcon) || tmpIcon.isEmpty() ){ tmpIcon=":/images/usbdevice.png"; }
    loginW->changeButtonIcon("device", tmpIcon, tmpsz);
    // - refresh button icon
    tmpIcon = currentTheme->itemIcon("refresh");
    tmpsz = currentTheme->itemIconSize("refresh");
    if(!tmpsz.isValid()){ tmpsz = defIconSize; }
    if(!QFile::exists(tmpIcon) || tmpIcon.isEmpty() ){ tmpIcon=":/images/refresh.png"; }
    loginW->changeButtonIcon("refresh", tmpIcon, tmpsz);
    //Enable/disable the password view functionality
    loginW->allowPasswordView( Config::allowPasswordView() );
    loginW->allowUserSelection( Config::allowUserSelection() );
    loginW->allowAnonLogin( Config::allowAnonLogin() );
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
    connect(loginW,SIGNAL(refreshUsers()), this, SLOT(LoadAvailableUsers()) );
    
    //----Desktop Environment Switcher
    if(simpleDESwitcher){
      tmpsz = currentTheme->itemIconSize("desktop");
      if(!tmpsz.isValid()){ tmpsz = defIconSize; }
      loginW->setDesktopIconSize(tmpsz);
    }else{
      if(DEBUG_MODE){ qDebug() << " - Create DE Switcher"; }
      //Create the switcher
      deSwitcher = new FancySwitcher(this, !currentTheme->itemIsVertical("desktop") );
      tmpsz = currentTheme->itemIconSize("desktop");
      if(!tmpsz.isValid()){ tmpsz = defIconSize; }
      deSwitcher->setIconSize(tmpsz.height());
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

  //Connect the grid to the leftmost screen widget
  mainscreen->setLayout(grid);
    
  //Now translate the UI and set all the text
  if(DEBUG_MODE){ qDebug() << " - Fill GUI with data"; }
  //retranslateUi();
  LoadAvailableUsers(); //Note: this is the first time it is run
  if(DEBUG_MODE){ qDebug() << " - Translate GUI"; }
  retranslateUi();
  if(DEBUG_MODE){ qDebug() << "Done with initialization"; }

}

void PCDMgui::fillScreens(){
    //Set a background image on any other available screens
    QDesktopWidget *DE = QApplication::desktop();
    screens.clear();
    //Now apply the background to all the other screens   
    // - Keep track of the total width/height of all screens combined (need to set the QMainWindow to this size)
    int wid, high;
    wid = high = 0;
    QList<QRect> geoms;
    for(int i=0; i<DE->screenCount(); i++){
        //Just show a generic QWidget with the proper background image on every screen
	QRect rec = DE->screenGeometry(i);
	if(geoms.contains(rec)){ continue; } //duplicate geom - skip it
	geoms << rec;
	QWidget *screen = new QWidget(this->centralWidget());
	screen->setObjectName("BGSCREEN");
	//QRect rec = DE->screenGeometry(i);
	//if(geoms.contains(rec)){ continue; } //duplicate geometry - skip it
	qDebug() << "Detected Screen:" << i << rec;
	screen->setGeometry( rec );
	if(rec.height() > high){ high = rec.height(); }
	wid += rec.width();
	screen->show();
	screens << screen;
    }
    this->setGeometry(0,0,wid,high);
    this->activateWindow();
    QCursor::setPos( DE->screenGeometry(0).center() );	  
}

void PCDMgui::slotScreensChanged(){
  //Restart the GUI 
  QApplication::exit(-1); //special code to restart the GUI-only
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
  QString devPassword;
  bool anonymous = loginW->isAnonymous();
  if(!anonymous && pcCurrent.contains(username)){
    //personacrypt user - also pull device password
    devPassword = loginW->currentDevicePassword();
  }
  QLocale currLocale = this->locale();
  QString lang = currLocale.name();
  //Disable user input while confirming login
  loginW->setEnabled(false);
  if(!simpleDESwitcher){ deSwitcher->setEnabled(false); }
  toolbar->setEnabled(false);
  //Try to login
  emit xLoginAttempt(username, password, desktop, lang , devPassword,anonymous);
  //Return signals are connected to the slotLogin[Success/Failure] functions
  
}

void PCDMgui::slotLoginSuccess(){
  /*QString de;
  if(simpleDESwitcher){ de = loginW->currentDE(); }
  else{ de = deSwitcher->currentItem(); }
  saveLastLogin( loginW->currentUsername(), de );*/
  slotUpdateGUI(); //now start to close down the PCDM GUI
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
  this->activateWindow();
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
    QSize tmpsz = currentTheme->itemIconSize("user");
    if(!tmpsz.isValid()){ tmpsz = defIconSize; }
    loginW->changeButtonIcon("display",tmpIcon, tmpsz);
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
    system("shutdown -po now");
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
    system("shutdown -ro now");
    QCoreApplication::exit(1); //flag that this is not a normal GUI close
  }
}

void PCDMgui::slotClosePCDM(){
  //Same as UpdateGUI, but stops the daemon as well for debugging purposes
  system("killall -9 xvkbd"); //be sure to close the virtual keyboard
  for(int i=0; i<screens.length(); i++){ screens[i]->close(); } //close all the other screens
  QProcess::execute("touch /tmp/.PCDMstop"); //turn off the daemon as well
  QCoreApplication::exit(0);
  close();
}

void PCDMgui::slotUpdateGUI(){
  system("killall -9 xvkbd"); //be sure to close the virtual keyboard
  for(int i=0; i<screens.length(); i++){ screens[i]->close(); } //close all the other screens
  //QProcess::execute("touch /tmp/.PCDMstop"); //turn off the daemon as well
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
  }else if(langCode=="en_US"){
    m_translator = new QTranslator(); //make sure it is completely empty
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

void PCDMgui::LoadAvailableUsers(){
  if(DEBUG_MODE){ qDebug() << "Update Users:"; }
  pcAvail = Backend::getRegisteredPersonaCryptUsers();
  //qDebug() << "Loading Users:" << pcAvail << sysAvail << pcCurrent;
  QStringList userlist = Backend::getSystemUsers(false);
  if(userlist.isEmpty()){ 
    //Fallback method in case no valid system users could be found
    Backend::allowUidUnder1K(true); 
    userlist = Backend::getSystemUsers(false);
  }
  //qDebug() << " - System:" << userlist;
  QString lastUser;
  if(!pcAvail.isEmpty()){ 
    QStringList pcnow = Backend::getAvailablePersonaCryptUsers(); 
    if(DEBUG_MODE){ qDebug() << "PC (avail, now):" << pcAvail << pcnow; }
    if(pcnow.length() > pcCurrent.length()){
      //New personacrypt user available - switch to that
      for(int i=0; i<pcnow.length(); i++){
        if( !pcCurrent.contains(pcnow[i]) ){ lastUser = pcnow[i]; break; }
      }
    }
    if(DEBUG_MODE){ qDebug() << " - Now:" << pcnow << lastUser; }
    //Start with the system users
    //userlist = sysAvail; //personacrypt users will always be included in the system users
    for(int i=0; i<pcAvail.length(); i++){
      if(!pcnow.contains(pcAvail[i]) && !Config::allowAnonLogin()){
        //Device is not connected - hide this user (no anonymous logins either)
	userlist.removeAll(pcAvail[i]);
      }
    }
    pcCurrent = pcnow; //for comparison later
    //Need to convert all the usernames into the Display Names now
    for(int i=0; i<userlist.length(); i++){
      if(pcnow.contains(userlist[i])){
	//Device connected - put the special flag on the end
        userlist[i] = Backend::getDisplayNameFromUsername(userlist[i])+"::::personacrypt";
      }else{
	userlist[i] = Backend::getDisplayNameFromUsername(userlist[i]);
      }
    }
  }else{
    userlist = Backend::getSystemUsers(); //Just pull the entire display name list
  }
  if(DEBUG_MODE){ qDebug() << "UserList (names):" << userlist << sysAvail; }
  //Add the usernames to the login widget (if different)
  if(userlist != sysAvail || sysAvail.isEmpty() ){
    loginW->setUsernames(userlist); //add in the detected users
    sysAvail = userlist; //save for later
    //Whenever we reset the internal list, also need to reset which user has focus
    if(lastUser.isEmpty()){ lastUser = Backend::getLastUser(); }
    if(!lastUser.isEmpty()){ //set the previously used user
    	loginW->setCurrentUser(Backend::getDisplayNameFromUsername(lastUser)); 
    }	  
  }
  
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
  localeButton->setToolTip( QString(tr("Change locale (%1)")).arg(this->locale().name()) );
  localeButton->setText(this->locale().name());
  //keyboard layout button
  keyboardButton->setToolTip(tr("Change Keyboard Layout"));
  keyboardButton->setText(tr("Keyboard Layout"));
  //system button
  systemButton->defaultAction()->setToolTip(tr("Shutdown the computer"));
  systemButton->defaultAction()->setText(tr("System"));
  //Menu entries for system button
    systemMenu->clear();	
    systemMenu->addAction( tr("Refresh PCDM"), this, SLOT(slotUpdateGUI()) );
    systemMenu->addSeparator();
    systemMenu->addAction( tr("Restart"),this, SLOT(slotRestartComputer()) );
    systemMenu->addAction( tr("Shut Down"), this, SLOT(slotShutdownComputer()) );
    if(DEBUG_MODE){systemMenu->addAction( tr("Close PCDM"), this, SLOT(slotClosePCDM()) ); }
    systemButton->setMenu(systemMenu);
  //The main login widget
  if(hostname.isEmpty()){
    if(DEBUG_MODE){ qDebug() << "Finding Hostname..."; }
    //Find the system hostname
    hostname = pcbsd::Utils::runShellCommand("hostname").join(" ").simplified();
    if(DEBUG_MODE){ qDebug() << " - Host:" << hostname; }
    loginW->displayHostName(hostname);	  
  }
  if(DEBUG_MODE){ qDebug() << "Translate Login Widget"; }
  loginW->retranslateUi();
  
  //The desktop switcher
  
    //Get the new desktop list (translated)
    QStringList deList = Backend::getAvailableDesktops(); //priority ordered
    QString lastDE;
    if(!loginW->currentUsername().isEmpty()){ lastDE = Backend::getLastDE(loginW->currentUsername()); }
    if(DEBUG_MODE){ qDebug() << "DE's:" << deList << lastDE; }
    if(lastDE.isEmpty()){ lastDE = deList[0]; }
    //Organize the desktop list alphabetically by filename
    deList.removeDuplicates();
    QStringList DEL;
    for(int i=0; i<deList.length(); i++){
      //Check the icon
        QString deIcon = Backend::getDesktopIcon(deList[i]);
	if( deIcon.isEmpty() ){ deIcon = currentTheme->itemIcon("desktop"); } //set the default icon if none given
        if( !QFile::exists(deIcon) ){ deIcon = ":/images/desktop.png"; }
      QString entry = deList[i] +":::"+deIcon+":::"+Backend::getDesktopComment(deList[i]);
      DEL << entry;
    }
    DEL.sort(); //make it alphabetical
    //Now fill the switcher
    if(!simpleDESwitcher){
      deSwitcher->removeAllItems();
      for(int i=0; i<DEL.length(); i++){
	//Now add the item
	deSwitcher->addItem( DEL[i].section(":::",0,0), DEL[i].section(":::",1,1), DEL[i].section(":::",2,2) );
      }
      if(DEBUG_MODE){ qDebug() << "Last used DE:" << lastDE; }
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
      if(DEBUG_MODE){ qDebug() << "Loading DE's into the login widget"; }
      loginW->setDesktops(deNames, deIcons, deInfo);
      //Set the switcher to the last used desktop environment
      if(DEBUG_MODE){ qDebug() << "Last used DE:" << lastDE; }
      loginW->setCurrentDE(lastDE);
    }

}

