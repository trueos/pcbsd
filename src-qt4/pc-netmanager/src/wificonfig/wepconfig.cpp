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
#include "wepconfig.h"
#include "ui_wepconfig.h"



void wepConfig::setKey( QString Key, int index, bool wephex )
{
    // Set the key on our line edits
    if ( ! Key.isEmpty() )
    {
	lineKey->setText(Key);
	lineKey2->setText(Key);
    }
 
    // Set if we are using plaintext or a hex key
    if ( wephex )
    {
      radioHex->setChecked(true);
      radioPlaintext->setChecked(false);
    } else {
      radioHex->setChecked(false);
      radioPlaintext->setChecked(true);
    }
    
    if ( index == 1 || index == 2 || index == 3 || index == 4)
    {
	spinIndex->setValue(index);
    }
    
}


void wepConfig::slotClose()
{
    //bool ok;
    //lineKey->text().toInt(&ok, 16);
    //if ( ok && ! radioHex->isChecked() )
    //   QMessageBox::warning( this, tr("Hex Key"), tr("Warning: The entered network is a valid hex key, but plain-text mode is enabled.") );
    // 
    //if ( ! ok && radioHex->isChecked() ) {
    //   QMessageBox::warning( this, tr("Hex Key"), tr("Error: The specified key is not a valid hex key.") );
    //   return;
    //}
    

    if ( lineKey->text() != lineKey2->text() )
    {
	QMessageBox::warning( this, "Network Key Error", "Error: The entered network keys do not match!\n" );
    } else {
	if ( radioHex->isChecked() ) {
  	  emit saved(lineKey->text(), spinIndex->value(), true);
        } else {
          emit saved(lineKey->text(), spinIndex->value(), false);
        }
	close();
    }
}

void wepConfig::slotShowKey()
{
   if(checkShowKey->isChecked())
   {
      lineKey2->setEchoMode(QLineEdit::Normal);
      lineKey->setEchoMode(QLineEdit::Normal);
   } else {
      lineKey2->setEchoMode(QLineEdit::Password);
      lineKey->setEchoMode(QLineEdit::Password);
   }
}

