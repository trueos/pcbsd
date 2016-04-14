/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/
#include "netKey.h"
#include "ui_netKey.h"


void netKey::setSSID(QString SSID){
 //Set the label to show the SSID 
  ssid = SSID;
 labelssid->setText(SSID);
 //Also set the slot/signal connections here (for now)
 connect( pushApply,SIGNAL( clicked() ),this,SLOT( slotApply() ) );
 connect( pushCancel,SIGNAL( clicked() ),this,SLOT( slotCancel() ) );
 connect( checkShowKey, SIGNAL( clicked() ),this,SLOT( slotShowKey() ) );
 slotShowKey();
}

void netKey::showKeyType(bool show){
  group_keytype->setVisible(show);
}

void netKey::slotShowKey(){
  if( checkShowKey->isChecked() ){
    lineKey->setEchoMode(QLineEdit::Normal);
    lineKey2->setEchoMode(QLineEdit::Normal);
  }else{
    lineKey->setEchoMode(QLineEdit::Password);
    lineKey2->setEchoMode(QLineEdit::Password);
  }
}
void netKey::slotApply()
{
    if ( lineKey->text() != lineKey2->text() )
    {
	QMessageBox::warning( this, tr("Network Key Error"), tr("Error: The entered network keys do not match!\n") );
    } else {
	bool useHex = true;
	if( group_keytype->isVisible() && radio_text->isChecked() ){ useHex = false; }
  	emit saved(lineKey->text(), ssid, useHex);
        close();
    }
}

void netKey::slotCancel(){
  close();
}


