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
#include "dialogworking.h"


void dialogWorking::programInit()
{
   progressBarWorking->reset();
   progressBarWorking->setMinimum(0);
   progressBarWorking->setMaximum(0);
}


void dialogWorking::setDialogTitle( QString &title )
{
    setWindowTitle(title);
}


void dialogWorking::setDialogText( QString &text )
{
    textText->setText(text);
}


void dialogWorking::closeEvent( QCloseEvent *e )
{
    // Ignore the close button
}
