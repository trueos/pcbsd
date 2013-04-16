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
  userSelected = FALSE; //focus on the username first
  pwVisible = FALSE; //Have the password box hide the input behind dots
  allowPWVisible = TRUE; //Allow the password button to show the password text
  
  //Create the Grid layout
  QHBoxLayout* hlayout1 = new QHBoxLayout();
  QHBoxLayout* hlayout2 = new QHBoxLayout();
  QVBoxLayout* vlayout = new QVBoxLayout();
  QFormLayout* flayout = new QFormLayout();
  //Create the items
  userIcon = new QToolButton;
  	QAction* tmp = new QAction(this);
	userIcon->setDefaultAction( tmp );
	userIcon->setFocusPolicy(Qt::NoFocus);
  listUserBig = new QListWidget;
  	listUserBig->setFocusPolicy(Qt::StrongFocus);
  	listUserBig->setSelectionMode(QAbstractItemView::SingleSelection);
  listUsers = new QComboBox;
  	listUsers->setFocusPolicy(Qt::NoFocus); //big one gets keyboard focus instead
  linePassword = new QLineEdit;
  	linePassword->setFocusPolicy(Qt::StrongFocus);
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
  //Add the items to the grid
    //user not yet selected widgets
      hlayout1->addWidget(userIcon);
    vlayout->addLayout(hlayout1);
    vlayout->addWidget(listUserBig);
    //User selected widgets
      flayout->addRow(pushUserIcon, listUsers);
      flayout->addRow(pushViewPassword, linePassword);
    vlayout->addLayout(flayout);
      hlayout2->addWidget(pushLogin);
    vlayout->addLayout(hlayout2);
    
  //Setup the Signals/Slots
  connect(pushLogin,SIGNAL(clicked()),this,SLOT(slotTryLogin()));
  connect(pushUserIcon,SIGNAL(triggered(QAction*)), this, SLOT(slotUserActivated(QAction*)));
  connect(userIcon,SIGNAL(triggered(QAction*)), this, SLOT(slotUserActivated(QAction*)));
  connect(listUsers,SIGNAL(activated(int)),this,SLOT(slotChooseUser(int)));
  connect(listUserBig,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(slotUserClicked(QListWidgetItem*)) );
  connect(listUserBig,SIGNAL(currentRowChanged(int)),this,SLOT(slotUserHighlighted(int)) );
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
  if(userSelected){
    userIcon->setVisible(FALSE);
    listUserBig->setVisible(FALSE);
    pushUserIcon->setVisible(TRUE);
    listUsers->setVisible(TRUE);
    linePassword->setVisible(TRUE);
    pushLogin->setVisible(TRUE);
    pushViewPassword->setVisible(TRUE);
  }else{
    userIcon->setVisible(TRUE);
    listUserBig->setVisible(TRUE);
    pushUserIcon->setVisible(FALSE);
    listUsers->setVisible(FALSE);
    linePassword->setVisible(FALSE);
    pushLogin->setVisible(FALSE);
    pushViewPassword->setVisible(FALSE);    
  }
  if(pwVisible){
    linePassword->setEchoMode(QLineEdit::Normal);	  
  }else{
    linePassword->setEchoMode(QLineEdit::Password);  	  
  }
  retranslateUi();
}

void LoginWidget::keyPressEvent(QKeyEvent *e){
  if( (e->key()==Qt::Key_Enter) || (e->key()==Qt::Key_Return) ){
    if(userSelected){
      slotTryLogin();
    }else{
      slotUserSelected();     
    }
  }else if(e->key() == Qt::Key_Escape){
    if(userSelected){
      slotUserUnselected();
    }else{
      emit escapePressed();
    }
  }
}

//-------------------------------------
//    PRIVATE SLOTS
//-------------------------------------
void LoginWidget::slotUserActivated(QAction* act){
    //Toggle the user box as to what is visible
    if(!userSelected){ 
	slotUserSelected(); 
    }else{
	slotUserUnselected();
    }
}

void LoginWidget::slotUserClicked(QListWidgetItem* item){
  slotUserSelected();	
}

void LoginWidget::slotUserHighlighted(int row){
  emit UserChanged(idL[row]);	
}

void LoginWidget::slotTryLogin(){
  QString user = listUsers->currentText();
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
  userSelected = TRUE;
  pwVisible = FALSE;
  linePassword->clear(); //make sure the password is cleared if the user is changed
  updateWidget();
  linePassword->setFocus();
  emit UserSelected(listUsers->currentText());
}

void LoginWidget::slotUserUnselected(){
  userSelected = FALSE;
  pwVisible = FALSE;
  linePassword->clear(); //make sure the password is cleared if the user is changed
  updateWidget();
  listUserBig->setFocus();
  emit UserSelected("");
}

void LoginWidget::slotChangePWView(){
  if(pwVisible){
    pwVisible = FALSE;
  }else{
    pwVisible = TRUE;
  }
  updateWidget();
}

//-----------------------------
//     PUBLIC FUNCTIONS
//-----------------------------
QString LoginWidget::currentUsername(){
  QString id = listUsers->currentText();
  return id;
}

QString LoginWidget::currentPassword(){
  QString pw = linePassword->text();
  return pw;
}

void LoginWidget::setCurrentUser(QString id){
  int index = idL.indexOf(id);
  if(index == -1){
    qDebug() << "LoginWidget: Item does not exist -" << id;
  }else{
    listUsers->setCurrentIndex(index);
    listUserBig->setCurrentRow(index);
    emit UserChanged(id);
  }
}

void LoginWidget::setUsernames(QStringList uList){
  if(uList.isEmpty()){ return; }
  //Make sure that the two user widgets are identical
  listUsers->clear();
  listUsers->addItems(uList);
  listUserBig->clear();
  listUserBig->addItems(uList);
  idL.clear();
  idL = uList; //save for later
  listUsers->setCurrentIndex(0);
  listUserBig->setCurrentRow(0);
  //Automatically select the user if there is only one
  if(uList.length() == 1){
    qDebug() << "Single User System Detected";
    slotUserSelected();	 
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
    pushLogin->setIcon(QIcon(iconFile));
    pushLogin->setIconSize(iconSize);
  }else if(button.toLower() == "pwview"){ 
    pushViewPassword->setIcon(QIcon(iconFile));
    pushViewPassword->setIconSize(iconSize);
  }else{ 
    qDebug() << "LoginWidget: Cannot change the icon for button" << button << " - valid buttons are \"display\", \"login\", and \"pwview\""; 
  }
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
  pushUserIcon->setText(tr("Select"));	
  pushUserIcon->setToolTip(tr("Select an alternate user and clear the password field"));
  userIcon->setText(tr("Select"));
  userIcon->setToolTip(tr("Select this user"));
 
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
  //Setup the computer/host name display
  if( hostName.isEmpty() ){
    this->setTitle("");
  }else{
    this->setTitle( QString(tr("Login to %1")).arg(hostName) );
  }
}

void LoginWidget::resetFocus(QString item){
  //Check for appropriate action if not specified
  if(item.isEmpty() && userSelected ){ item="password"; }
  else if(item.isEmpty() && !userSelected ){ item="userlist"; }	  
  //Set the proper keyboard focus
  if(item == "userlist"){
    if(userSelected){ listUsers->setFocus(); }
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
