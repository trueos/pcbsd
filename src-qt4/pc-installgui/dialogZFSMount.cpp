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
#include <QComboBox>
#include <QMessageBox>

#include "dialogZFSMount.h"
#include "backend.h"

void dialogZFSMount::dialogInit()
{
  // Connnect our slots
  connect(pushCancel, SIGNAL(clicked()), this, SLOT(slotPushCancel()));
  connect(pushAdd, SIGNAL(clicked()), this, SLOT(slotPushAdd()));
}

void dialogZFSMount::slotPushCancel()
{
  close();
}

void dialogZFSMount::slotPushAdd()
{
  QString tmp;
  tmp = lineZFSMount->text();
  if ( tmp.indexOf("/") != 0 )
  {
     QMessageBox::warning(this, tr("ZFS Filesystem"), tr("Invalid mount point, must begin with '/'."),
                                                QMessageBox::Ok,
                                                QMessageBox::Ok);
     return;
  }

  if ( tmp.indexOf(" ") != -1 )
  {
     QMessageBox::warning(this, tr("ZFS Filesystem"), tr("Invalid mount point, whitespace disallowed."),
                                                QMessageBox::Ok,
                                                QMessageBox::Ok);
     return;
  }

  // Setup /home -> /usr/home
  if ( tmp.indexOf("/home") == 0 )
     tmp.replace("/home", "/usr/home");

  emit saved(lineZFSMount->text());
  close();
}
