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
#include "dialogwpaenterprise.h"
#include "ui_dialogwpaenterprise.h"
#include <qfiledialog.h>


void dialogWPAEnterprise::slotTypeChanged()
{
    
    if ( radioEAPTLS->isChecked() ) {
	textClientCert->setHidden(false);
	lineClientCert->setHidden(false);
	pushSelectClientCert->setHidden(false);
	textPrivateKeyFile->setHidden(false);
	linePrivateKeyFile->setHidden(false);
	pushSelectPrivateKeyFile->setHidden(false);	
    }
    
    if ( radioEAPTTLS->isChecked() ) {
	textClientCert->setHidden(true);
	lineClientCert->setHidden(true);
	pushSelectClientCert->setHidden(true);
	textPrivateKeyFile->setHidden(true);
	linePrivateKeyFile->setHidden(true);
	pushSelectPrivateKeyFile->setHidden(true);	
    }
    
    if ( radioEAPPEAP->isChecked() ) {
	textClientCert->setHidden(true);
	lineClientCert->setHidden(true);
	pushSelectClientCert->setHidden(true);
	textPrivateKeyFile->setHidden(true);
	linePrivateKeyFile->setHidden(true);
	pushSelectPrivateKeyFile->setHidden(true);
    }

}


void dialogWPAEnterprise::slotClose()
{
    int type = 0;
    
    if ( radioEAPTLS->isChecked() )
    {
	type = 1;
    }
    
    if ( radioEAPTTLS->isChecked() )
    {
	type = 2;
    }	
    
    if ( radioEAPPEAP->isChecked() )
    {
	type = 3;
    }
    
    
    
      if ( linePrivateKeyPassword->text() != linePrivateKeyPassword2->text() )
    {
	QMessageBox::warning( this, "Network Key Error", "Error: The entered password keys do not match!\n" );
    } else {
	emit saved( type, lineEAPIdentity->text(), lineCACert->text(), lineClientCert->text(), linePrivateKeyFile->text(), linePrivateKeyPassword->text(), comboKeyMgmt->currentIndex() );
	close();
    }
}


void dialogWPAEnterprise::slotSelectCACert()
{
        QString file = QFileDialog::getOpenFileName( 0, 
                    "Choose a Certificate file",
                    "/home",
                    "Certificate File (*)" );
	
	lineCACert->setText(file);
	
}


void dialogWPAEnterprise::slotSelectClientCert()
{
       QString file = QFileDialog::getOpenFileName( 0,
                    "Choose a client certificate file",
                    "/home",
                    "Certificate File (*)" );
	
	lineClientCert->setText(file);
}


void dialogWPAEnterprise::slotSelectPrivateKeyFile()
{
       QString file = QFileDialog::getOpenFileName( 0,
                    "Choose a private key file",
                    "/home",
                    "Certificate File (*)");
	
	linePrivateKeyFile->setText(file);
}


void dialogWPAEnterprise::setVariables( int type, QString EAPIdent, QString CACert, QString ClientCert, QString PrivKeyFile, QString Password, int keyMgmt )
{
    // Load the existing settings for this configuration
    
    if ( type == 1)
    {
	radioEAPTLS->setChecked(true);
    } else if (type == 2) {
	radioEAPTTLS->setChecked(true);
    } else if (type == 3) {
	radioEAPPEAP->setChecked(true);
    }

    comboKeyMgmt->setCurrentIndex(keyMgmt);

    lineEAPIdentity->setText(EAPIdent);
    lineCACert->setText(CACert);
    lineClientCert->setText(ClientCert);
    linePrivateKeyFile->setText(PrivKeyFile);
    linePrivateKeyPassword->setText(Password);
    linePrivateKeyPassword2->setText(Password);
    
    slotTypeChanged();
    
}

void dialogWPAEnterprise::slotShowKey()
{
   if(checkShowKey->isChecked())
   {
      linePrivateKeyPassword->setEchoMode(QLineEdit::Normal);
      linePrivateKeyPassword2->setEchoMode(QLineEdit::Normal);
   } else {
      linePrivateKeyPassword->setEchoMode(QLineEdit::Password);
      linePrivateKeyPassword2->setEchoMode(QLineEdit::Password);
   }
}
