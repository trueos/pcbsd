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
#include "dialogdisplayoutput.h"


void dialogDisplayOutput::programInit( bool buttonOn )
{
    // Enable or disable the button
    if(! buttonOn)
    {
	pushClose->setHidden(true);
    } else {
	pushClose->setHidden(false);
    }
    
}

void dialogDisplayOutput::setDialogCaption( const QString &text )
{
    setWindowTitle(text);
}

void dialogDisplayOutput::setDialogText( const QString &text )
{
    lineDisplay->setText(text);
}

void dialogDisplayOutput::appendDialogText( const QString &text )
{
    lineDisplay->insertPlainText(text);
    lineDisplay->moveCursor(QTextCursor::End);
}

void dialogDisplayOutput::slotPushClose()
{
    accept();
}

void dialogDisplayOutput::setCloseHide( bool status )
{
    if(status) {
	pushClose->setHidden(true);
    } else {
	pushClose->setHidden(false);
    }
    
}

void dialogDisplayOutput::closeEvent( QCloseEvent *e )
{
    // Ignore a close event until finished
}
