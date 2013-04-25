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
#include "dialogConfirm.h"
#include "ui_dialogConfirm.h"

void dialogConfirm::programInit(QString title)
{
  setWindowTitle(title);

  connect( pushOK, SIGNAL(clicked()), this, SLOT(slotOK()) );
  connect( pushCancel, SIGNAL(clicked()), this, SLOT(slotCancel()) );
}

void dialogConfirm::setInfoText(QString text)
{
  textConfirm->setText(text);
}

void dialogConfirm::slotCancel()
{
  close();
}

void dialogConfirm::slotOK()
{
  emit ok();
  close();
}
