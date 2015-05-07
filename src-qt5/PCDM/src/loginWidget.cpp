/* PCDM Login Manager:
*  Written by Ken Moore (ken@pcbsd.org) 2012/2013
*  Copyright(c) 2013 by the PC-BSD Project
*  Available under the 3-clause BSD license
*/

/*
Sub-classed widget for inputting login information
*/

#include "loginWidget.h"

LoginWidget::LoginWidget(QWidget* parent) : QGroupBox(parent)
{
  //Set the default values for the current item
  idL.clear(); //Clear the login names available
  userSelected = false; //focus on the username first
  pwVisible = false; //Have the password box hide the input behind dots
  allowPWVisible = true; //Allow the password button to show the password text
  showUsers = true; //Display the available users on the system
  updating = false; //not currently updating (internal flag for refreshes)
  allowAnon = false;
	
  //Create the Grid layout
  QHBoxLayout* hlayout1 = new QHBoxLayout();
  QHBoxLayout* hlayout2 = new QHBoxLayout();
  QHBoxLayout* hlayout3 = new QHBoxLayout();
  QHBoxLayout* hlayout4 = new QHBoxLayout();
  QVBoxLayout* vlayout = new QVBoxLayout();
  QFormLayout* flayout = new QFormLayout();
  //Create the items
  userIcon = new QToolButton(this);;
  	QAction* tmp = new QAction(this);
	userIcon->setDefaultAction( tmp );
	userIcon->setFocusPolicy(Qt::NoFocus);
  listUserBig = new QListWidget(this);
  	listUserBig->setFocusPolicy(Qt::StrongFocus);
  	listUserBig->setSelectionMode(QAbstractItemView::SingleSelection);
  listUsers = new QComboBox(this);
  	listUsers->setFocusPolicy(Qt::NoFocus); //big one gets keyboard focus instead
  linePassword = new QLineEdit(this);
  	linePassword->setFocusPolicy(Qt::StrongFocus);
  lineDevPassword = new QLineEdit(this);
	lineDevPassword->setFocusPolicy(Qt::StrongFocus);
  lineUsername = new QLineEdit;
	lineUsername->setFocusPolicy(Qt::StrongFocus);
  pushLogin = new QToolButton;
	QAction* tmp1 = new QAction(this);
	pushLogin->setDefaultAction( tmp1 );
	pushLogin->setFocusPolicy(Qt::StrongFocus);
  pushViewPassword = new QToolButton;
	QAction* tmp2 = new QAction(this);
	pushViewPassword->setDefaultAction( tmp2 );
	pushViewPassword->setFocusPolicy(Qt::NoFocus);
  pushUserIcon = new QToolButton;
	QAction* tmp3 = new QAction(this);
	pushUserIcon->setDefaultAction( tmp3 );
	pushUserIcon->setFocusPolicy(Qt::NoFocus);
  listDE = new QComboBox(this);
  deIcon = new QLabel(this);
  devIcon = new QLabel(this);
  nousers = new QLabel(this);
    nousers->setWordWrap(true);
  checkAnon = new QCheckBox(this);
    checkAnon->setChecked(false); //always default to disabled
    checkAnon->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    
  //Add the items to the grid
    vlayout->addWidget(nousers);
    //user not yet selected widgets
      hlayout1->addWidget(userIcon);
    vlayout->addLayout(hlayout1);
    vlayout->addWidget(listUserBig);
    //User selected widgets
        hlayout3->addWidget(listUsers);
        hlayout3->addWidget(lineUsername);
      flayout->addRow(pushUserIcon, hlayout3);
      flayout->addRow(pushViewPassword, linePassword);
      flayout->addRow(devIcon, lineDevPassword);
      flayout->addRow(deIcon,listDE);
    vlayout->addSpacing(15);
    vlayout->addLayout(flayout);
    vlayout->addSpacing(20);
      hlayout2->addWidget(pushLogin);
    vlayout->addLayout(hlayout2);
    //vlayout->addSpacing(10);
      hlayout4->addStretch();
      hlayout4->addWidget(checkAnon);
      hlayout4->addStretch();
      vlayout->addLayout(hlayout4);
    
  //Setup the Signals/Slots
  connect(pushLogin,SIGNAL(clicked()),this,SLOT(slotTryLogin()));
  connect(pushUserIcon,SIGNAL(triggered(QAction*)), this, SLOT(slotUserActivated()));
  connect(userIcon,SIGNAL(triggered(QAction*)), this, SLOT(slotUserActivated()));
  connect(listUsers,SIGNAL(activated(int)),this,SLOT(slotChooseUser(int)));
  connect(listUserBig,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(slotUserSelected()) );
  connect(listUserBig,SIGNAL(currentRowChanged(int)),this,SLOT(slotUserHighlighted(int)) );
  connect(listDE,SIGNAL(currentIndexChanged(int)),this,SLOT(slotDesktopChanged(int)) );
  connect(checkAnon, SIGNAL(stateChanged(int)), this, SLOT(slotAnonChanged()) );
  allowPasswordView(allowPWVisible); //setup signal/slots for pushViewPassword
  //Set this layout for the loginWidget
  this->setLayout(vlayout);
  
  //Run the display setup to ensure appropriate visiblility/usage
  updateWidget();
  
}

LoginWidget::~LoginWidget(){
}

void LoginWidget::updateWidget(){

  
  //Setup the visibility/sizes
  if(listUsers->count() < 1 || idL.isEmpty() ){
    nousers->setVisible(true);
    userIcon->setVisible(false);
    listUserBig->setVisible(false);
    pushUserIcon->setVisible(false);
    listUsers->setVisible(false);
    lineUsername->setVisible(false);
    linePassword->setVisible(false);
    pushLogin->setVisible(false);
    pushViewPassword->setVisible(false); 
    lineDevPassword->setVisible(false);
    checkAnon->setVisible(false);
    devIcon->setVisible(false);
    listDE->setVisible(false);
    deIcon->setVisible(false);
  }else if(userSelected && showUsers){
    nousers->setVisible(false);
    userIcon->setVisible(false);
    listUserBig->setVisible(false);
    pushUserIcon->setVisible(true);
    listUsers->setVisible(true);
    lineUsername->setVisible(false);
    linePassword->setVisible(true);
    //qDebug() << "list Data:" << listUsers->currentData();
    lineDevPassword->setVisible( listUsers->currentData().toString()=="persona" );
    devIcon->setVisible( listUsers->currentData().toString()=="persona" );
    checkAnon->setVisible(allowAnon && listUsers->currentData().toString()!="persona");
    pushLogin->setVisible(true);
    pushViewPassword->setVisible(true);
    if( listDE->count() < 1 ){ listDE->setVisible(false); deIcon->setVisible(false); }
    else{ listDE->setVisible(true); deIcon->setVisible(true); }
  }else if(!showUsers){
    //Do not show either of the user selection widgets
    nousers->setVisible(false);
    userIcon->setVisible(false);
    listUserBig->setVisible(false);
    pushUserIcon->setVisible(true);
    listUsers->setVisible(false);
    lineUsername->setVisible(true);
    linePassword->setVisible(true);
    checkAnon->setVisible(allowAnon);
    lineDevPassword->setVisible( true );
    devIcon->setVisible( true );
    pushLogin->setVisible(true);
    pushViewPassword->setVisible(true);
    if( listDE->count() < 1 ){ listDE->setVisible(false); deIcon->setVisible(false); }
    else{ listDE->setVisible(true); deIcon->setVisible(true); }
  }else{
    //ShowUsers and none selected
    nousers->setVisible(false);
    userIcon->setVisible(true);
    listUserBig->setVisible(true);
    pushUserIcon->setVisible(false);
    listUsers->setVisible(false);
    lineUsername->setVisible(false);
    linePassword->setVisible(false);
    pushLogin->setVisible(false);
    pushViewPassword->setVisible(false); 
    lineDevPassword->setVisible(false);
    checkAnon->setVisible(false);
    devIcon->setVisible(false);
    listDE->setVisible(false);
    deIcon->setVisible(false);
  }
  if(pwVisible){
    linePassword->setEchoMode(QLineEdit::Normal);
    lineDevPassword->setEchoMode(QLineEdit::Normal);	  
  }else{
    linePassword->setEchoMode(QLineEdit::Password);  	  
    lineDevPassword->setEchoMode(QLineEdit::Password);  
  }
  retranslateUi();
}

void LoginWidget::keyPressEvent(QKeyEvent *e){
  if(nousers->isVisible()){ return; }
  if( (e->key()==Qt::Key_Enter) || (e->key()==Qt::Key_Return) ){
    if(userSelected || !showUsers){
      slotTryLogin();
    }else{
      slotUserSelected();     
    }
  }else if(e->key() == Qt::Key_Escape){
    if(userSelected && showUsers){
      slotUserUnselected();
    }else{
      emit escapePressed();
    }
  }
}

//-------------------------------------
//    PRIVATE SLOTS
//-------------------------------------
void LoginWidget::slotUserActivated(){
    if(updating){ return; } //internally updating - skip signal
    //Toggle the user box as to what is visible
    if(!userSelected){ 
	slotUserSelected(); 
    }else{
	slotUserUnselected();
    }
}

/*void LoginWidget::slotUserClicked(QListWidgetItem* item){
  slotUserSelected();	
}*/

void LoginWidget::slotUserHighlighted(int row){
  if(updating){ return; } //internally updating - skip signal
  emit UserChanged(idL[row]);	
}

void LoginWidget::slotTryLogin(){
  QString user;
  if(showUsers){
    user = listUsers->currentText();
  }else{
    user = lineUsername->text();
  }
  QString pw = linePassword->text();
  emit loginRequested(user,pw);
  linePassword->setText(""); //always clear the box after a login attempt
}

void LoginWidget::slotChooseUser(int i){
  if(i!=-1){ slotUserSelected(); }	
}

void LoginWidget::slotUserSelected(){
  if(userSelected){ //make sure the big user widget is updated as well
    listUserBig->setCurrentRow( listUsers->currentIndex() );
  }else{ //make sure the small user widget is updated as well
    listUsers->setCurrentIndex( listUserBig->currentRow() );
  }
  userSelected = true;
  pwVisible = false;
  linePassword->clear(); //make sure the password is cleared if the user is changed
  lineDevPassword->clear();
  if(allowAnon){
    checkAnon->setChecked(false); //reset anonymous login selection on user change
    slotAnonChanged(); //update icon as necessary
  }
  updateWidget();
  linePassword->setFocus();
  qDebug() << "User Selected:" << listUsers->currentText();
  emit UserSelected(listUsers->currentText());
}

void LoginWidget::slotUserUnselected(){
  userSelected = false;
  pwVisible = false;
  linePassword->clear(); //make sure the password is cleared if the user is changed
  lineDevPassword->clear();
  updateWidget();
  listUserBig->setFocus();
  emit UserSelected("");
}

void LoginWidget::slotChangePWView(){
  if(pwVisible){
    pwVisible = false;
  }else{
    pwVisible = true;
  }
  updateWidget();
}

void LoginWidget::slotDesktopChanged(int index){
  if(index == -1){
    deIcon->setPixmap(QPixmap(""));
    deIcon->setToolTip("");
  }else{
    deIcon->setPixmap( QPixmap(desktopIcons[index]).scaled(desktopIconSize) );
    deIcon->setToolTip(desktopInfo[index]);
  }
}

void LoginWidget::slotAnonChanged(){
  //Change the login button icon a bit to reflect the new setting
  if(checkAnon->isChecked()){
    pushLogin->setIcon(QIcon(loginAnonIcon));
  }else{
    pushLogin->setIcon(QIcon(loginIcon));
  }
}

//-----------------------------
//     PUBLIC FUNCTIONS
//-----------------------------
QString LoginWidget::currentUsername(){
  QString id;
  if( listUsers->count()>0 ){ id = listUsers->currentText(); }
  return id;
}

QString LoginWidget::currentPassword(){
  QString pw = linePassword->text();
  return pw;
}

QString LoginWidget::currentDevicePassword(){
  QString pw = lineDevPassword->text();
  return pw;	
}

QString LoginWidget::currentDE(){
  QString de = listDE->currentText();
  return de;
}

bool LoginWidget::isAnonymous(){
  return (allowAnon && checkAnon->isChecked());
}

void LoginWidget::setCurrentUser(QString id){
  if(idL.isEmpty()){ return; }
  
  int index = idL.indexOf(id);
  if(index == -1){
    qDebug() << "LoginWidget: Item does not exist -" << id;
  }else{
    listUsers->setCurrentIndex(index);
    listUserBig->setCurrentRow(index);
    updateWidget();
    emit UserChanged(id);
  }
}

void LoginWidget::setCurrentDE(QString de){
  for(int i=0; i<listDE->count(); i++){
    if( listDE->itemText(i) == de ){
      listDE->setCurrentIndex(i);
      break;
    }
  }
  updateWidget();
}

void LoginWidget::setUsernames(QStringList uList){
  if(uList.isEmpty()){ 
    idL.clear();
    listUsers->clear();
    listUserBig->clear();
  }else{
    updating = true;
    //Make sure that the two user widgets are identical
    listUsers->clear();
    for(int i=0; i<uList.length(); i++){
      if(uList[i].contains("::::")){ 
        //This is a special personacrypt user - also needs additional device password
        listUsers->addItem(uList[i].section("::::",0,0), "persona");  //set internal flag
        uList[i] = uList[i].section("::::",0,0); //remove this flag for other widgets
      }else{
        listUsers->addItem(uList[i], "normal"); //just show this name (nothing special)
      }
    }
    listUserBig->clear();
    listUserBig->addItems(uList);
    idL.clear();
    idL = uList; //save for later
    listUsers->setCurrentIndex(0);
    listUserBig->setCurrentRow(0);
    //Automatically select the user if there is only one
    updating = false; //done
    if(uList.length() == 1){
      //qDebug() << "Single User System Detected";
      slotUserSelected();	 
    }
  }
  updateWidget();
}

void LoginWidget::setDesktops(QStringList text, QStringList icon, QStringList info){
  if((text.length() != icon.length()) && (text.length() != info.length()) ){ qDebug() << "LoginWidget: Unequal text/icon desktop lists"; return; }
  listDE->clear();
  desktopIcons.clear(); desktopInfo.clear();
  desktopIcons = icon; //save for later
  desktopInfo = info; //save for later
  for(int i=0; i<text.length(); i++){
    listDE->addItem(text[i]);
  }
}

void LoginWidget::displayHostName(QString name){
  hostName = name;
}

void LoginWidget::changeButtonIcon(QString button, QString iconFile, QSize iconSize){
  if(!QFile::exists(iconFile)){ qDebug() << "LoginWidget: invalid image file"<<iconFile; return; }
  if(button.toLower() == "display"){ 
    pushUserIcon->setIcon(QIcon(iconFile));
    pushUserIcon->setIconSize(iconSize);
    userIcon->setIcon(QIcon(iconFile));
    userIcon->setIconSize(iconSize);
  }else if(button.toLower() == "login"){ 
    loginIcon = iconFile;
    //pushLogin->setIcon(QIcon(iconFile));
    pushLogin->setIconSize(iconSize);
    slotAnonChanged();
  }else if(button.toLower() == "anonlogin"){
    loginAnonIcon = iconFile;
    slotAnonChanged();
  }else if(button.toLower() == "pwview"){ 
    pushViewPassword->setIcon(QIcon(iconFile));
    pushViewPassword->setIconSize(iconSize);
  }else if(button.toLower() == "device"){
    devIcon->setPixmap( QPixmap(iconFile).scaled(iconSize, Qt::KeepAspectRatio, Qt::SmoothTransformation) );
    devIcon->resize(iconSize);
  }else{ 
    qDebug() << "LoginWidget: Cannot change the icon for button" << button << " - valid buttons are \"display\", \"login\", and \"pwview\""; 
  }
}

void LoginWidget::setDesktopIconSize(QSize iconsize){
  desktopIconSize = iconsize;
}

void LoginWidget::changeStyleSheet(QString item, QString style){
  //Propagate the style sheet to both the buttons and the iconViewer
  if(item.toLower()=="login"){ pushLogin->setStyleSheet(style); }
  else if(item.toLower()=="pwview"){ pushViewPassword->setStyleSheet(style); }
  else if(item.toLower()=="user"){ listUsers->setStyleSheet(style); }
  else if(item.toLower()=="password"){ linePassword->setStyleSheet(style); }
  else{ 
    qDebug() << "LoginWidget: Invalid item to change style" << item << " - valid items are \"login\", \"pwview\", \"user\", and \"password\"";	
  }
}

void LoginWidget::retranslateUi(){
  //Set all the text for the widget (to easily allow changing the locale)
  nousers->setText(tr("Please connect your PersonaCrypt device to start login procedures."));
  pushUserIcon->setText(tr("Select"));	
    pushUserIcon->setToolTip(tr("Select an alternate user and clear the password field"));
  userIcon->setText(tr("Select"));
    userIcon->setToolTip(tr("Select this user"));
  checkAnon->setText(tr("Stealth Session"));
    checkAnon->setToolTip(tr("Use a temporary home directory which is deleted on log out)"));
  pushLogin->setText(tr("Login"));
    pushLogin->setToolTip(tr("Login to the system with the current user and password"));
  pushViewPassword->setText(tr("Password"));
  if(allowPWVisible){
    pushViewPassword->setToolTip(tr("Hold to view the currently entered password"));
  }else{
    pushViewPassword->setToolTip(tr("Login password for the selected user"));	  
  }
  listUsers->setToolTip(tr("Available users"));
  listUserBig->setToolTip(tr("Available users"));
  linePassword->setToolTip(tr("Login password for the selected user"));
  if(showUsers){ lineDevPassword->setToolTip(tr("Device encryption key")); }
  else{ lineDevPassword->setToolTip(tr("Device encryption key (personacrypt users only)")); }
  listDE->setToolTip(tr("Available desktop environments"));
  //Setup the computer/host name display
  if( hostName.isEmpty() ){
    this->setTitle("");
  }else{
    this->setTitle( QString(tr("Login to %1")).arg(hostName) );
  }
}

void LoginWidget::resetFocus(QString item){
  //Check for appropriate action if not specified
  if(idL.isEmpty()){ return; } //no users available
  if(item.isEmpty() && userSelected && showUsers){ item="password"; }
  else if(item.isEmpty() && (!userSelected || !showUsers) ){ item="userlist"; }
  //Set the proper keyboard focus
  if(item == "userlist"){
    if(!showUsers){ lineUsername->setFocus(); }
    else if(userSelected){ listUsers->setFocus(); }
    else{ listUserBig->setFocus(); }
  }else if(item == "password"){
    linePassword->setFocus();
  }else{
    //By default, de-select the user and set focus on the user selection
    slotUserUnselected();    
    listUserBig->setFocus();
  }
}

void LoginWidget::allowPasswordView(bool allow){
  //Enable/disable the password view button as desired
  allowPWVisible = allow;
  pushViewPassword->disconnect(); //disconnect the signal/slot (keep nice appearance)
  if(allow){
    connect(pushViewPassword,SIGNAL(pressed()),this,SLOT(slotChangePWView()));
    connect(pushViewPassword,SIGNAL(released()),this,SLOT(slotChangePWView()));    
  }
}

void LoginWidget::allowUserSelection(bool allow){
  showUsers = allow;
  updateWidget();
}

void LoginWidget::allowAnonLogin(bool allow){
  allowAnon = allow;
  updateWidget();
}
