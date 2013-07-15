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
#include <QProcess>
#include <QTextStream>
#include <QApplication>
#include "mainwindow.h"
#include "../config.h"

void MainWindow::ProgramInit()
{
  tries=3;
  connect(buttonBox, SIGNAL(clicked(QAbstractButton *)), this, SLOT(slotButtonClicked(QAbstractButton *)));
  connect(passwordLineEdit, SIGNAL(returnPressed()), this, SLOT(slotReturnPressed()));
}

void MainWindow::slotReturnPressed()
{
  testPass();
}

void MainWindow::testPass()
{

  QString program = "sudo";
  QStringList arguments;
  arguments << "-S";
  arguments << "-k";
  arguments << "true";

  QProcess *tP = new QProcess(this);
  tP->start(program, arguments);
  tP->write(passwordLineEdit->text().toLatin1() + "\n");
  tP->write(passwordLineEdit->text().toLatin1() + "\n");
  tP->write(passwordLineEdit->text().toLatin1() + "\n");
  while(tP->state() == QProcess::Starting || tP->state() == QProcess::Running ) {
     tP->waitForFinished(500);
     QCoreApplication::processEvents();
     qDebug() << "Waiting...";
  }
  if ( tP->exitCode() != 0 )
  {
     QString tmp;
     tmp.setNum(tries-1);
     labelBadPW->setText(tr("Invalid Password! Tries Left: %1").arg(tmp) );
     tries--;
     if ( tries == 0 )
       exit(1);
  } else {
     startSudo();
  }
}

void MainWindow::startSudo()
{
  setVisible(false);
  QString program = "sudo";
  QStringList arguments;
  arguments << "-S";
  for ( int i = 1; i< qApp->argc() ; i++)
    arguments << qApp->argv()[i];

  sudoProc = new QProcess(this);
  sudoProc->start(program, arguments);
  sudoProc->write(passwordLineEdit->text().toLatin1() + "\n");
  connect( sudoProc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(slotProcDone() ) );
  while(sudoProc->state() == QProcess::Starting ) {
     sudoProc->waitForFinished(500);
     QCoreApplication::processEvents();
  }
}

void MainWindow::slotProcDone()
{
   exit(sudoProc->exitCode());
}

void MainWindow::slotButtonClicked(QAbstractButton *myBut)
{
  if ( buttonBox->buttonRole(myBut) == QDialogButtonBox::AcceptRole ) 
     testPass();
  close();
}
