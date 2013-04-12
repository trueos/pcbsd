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
#include "dialogimport.h"


void dialogImport::programInit()
{
    // Connect our checks
    //connect(lineHostname, SIGNAL(textChanged ( const QString & )), this, SLOT(checkLineText( const QString & ) ) );
    //connect(lineIP, SIGNAL(textChanged ( const QString & )), this, SLOT(checkLineText( const QString & ) ) );
    connect(pushCancel, SIGNAL(clicked()), this, SLOT(slotButtonCancel() ) );
    connect(pushOk, SIGNAL(clicked()), this, SLOT(slotButtonOk() ) );
}


void dialogImport::slotButtonCancel()
{
    close();
}


void dialogImport::slotButtonOk()
{
    QString IP, host;
    IP="OFF"; host="OFF";
    if (groupIP->isChecked() )
      IP = "--ip=" + lineIP->text();
    if (groupHostname->isChecked() )
      host = "--host=" + lineHostname->text();
    
    emit import(IP, host);
    close();
    
}


void dialogImport::checkLineText( const QString &text )
{
    // Check that we have a valid IP / Host and enable / disable the creation button
    QString IP, Host;
    
    IP = lineIP->text();
    Host = lineHostname->text();
    
    // Check to make sure we don't have any missing IP fields
    if ( groupIP->isChecked() && (IP.indexOf("..") != -1 || IP.lastIndexOf(".") == (IP.length() - 1)) )
    {
	pushOk->setEnabled(FALSE);
	return;
    }
    
    // Check to make sure the host isn't empty
    if ( groupHostname->isChecked() && Host.isEmpty() )
    {
	pushOk->setEnabled(FALSE);
	return;
    }
    
    pushOk->setEnabled(TRUE);
    
}
