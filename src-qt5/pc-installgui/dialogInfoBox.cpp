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
#include "dialogInfoBox.h"
#include "ui_dialogInfoBox.h"
#include <QTreeWidget>
//#include <QtGui>

void dialogInfoBox::programInit(QString title)
{
  connect(pushClose, SIGNAL(clicked()), this, SLOT(slotClose()));
  setWindowTitle(title);
}

void dialogInfoBox::setInfoText(QString text)
{
  textDisplay->setText(text);
}

void dialogInfoBox::slotClose()
{
  close();
}
