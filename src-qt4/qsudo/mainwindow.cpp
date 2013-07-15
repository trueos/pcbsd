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
#include <QDebug>
#include <QDir>
#include <QString>
#include <QTextStream>
#include "mainwindow.h"
#include "../config.h"

void MainWindow::ProgramInit()
{
  connect(buttonBox, SIGNAL(clicked(QAbstractButton *)), this, SLOT(slotButtonClicked(QAbstractButton *)));
  connect(passwordLineEdit, SIGNAL(returnPressed()), this, SLOT(slotReturnPressed()));
}

void MainWindow::slotReturnPressed()
{
  QTextStream cout(stdout);
  cout << passwordLineEdit->text();
  close();
}

void MainWindow::slotButtonClicked(QAbstractButton *myBut)
{
  if ( buttonBox->buttonRole(myBut) == QDialogButtonBox::AcceptRole ){
    QTextStream cout(stdout);
    cout << passwordLineEdit->text();
  }
  close();
}
