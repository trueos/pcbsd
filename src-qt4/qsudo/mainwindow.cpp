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
#include <QMessageBox>
#include <QString>
#include <QProcess>
#include <QTextStream>
#include <QApplication>
#include <QDialogButtonBox>
#include <QPushButton>
#include "mainwindow.h"
#include "../config.h"

void MainWindow::ProgramInit()
{
  if( ! checkUserGroup() ) {
    QMessageBox::critical(this, tr("Access Denied"),
                                tr("This user does not have administrator permissions on this system!"),
                                QMessageBox::Ok,
                                QMessageBox::Ok);
    exit(1);
  }

  tries=3;
  connect(buttonBox, SIGNAL(clicked(QAbstractButton *)), this, SLOT(slotButtonClicked(QAbstractButton *)));
  connect(passwordLineEdit, SIGNAL(returnPressed()), this, SLOT(slotReturnPressed()));
  connect(passwordLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotPwdTextChanged(QString)));
  connect(commandGroupBox,  SIGNAL(clicked(bool)), this, SLOT(slotExpandCommandClicked(bool)));

  QPushButton* btn= buttonBox->button(QDialogButtonBox::Ok);
  if (btn)
      btn->setEnabled(false);

  // Set command text
  commandLabel->setVisible(false);
  QString commText;
  for ( int i = 1; i< qApp->argc() ; i++)
  {
      commText+=qApp->argv()[i];
  }
  commandLabel->setText(commText);
}

void MainWindow::slotReturnPressed()
{
    if (passwordLineEdit->text().length())
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
  tP->setProcessChannelMode(QProcess::MergedChannels);
  tP->start(program, arguments);
  tP->write(passwordLineEdit->text().toLatin1() + "\n");
  tP->write(passwordLineEdit->text().toLatin1() + "\n");
  tP->write(passwordLineEdit->text().toLatin1() + "\n");
  while(tP->state() == QProcess::Starting || tP->state() == QProcess::Running ) {
     tP->waitForFinished(500);
     QCoreApplication::processEvents();
  }
  if ( tP->exitCode() != 0 )
  {
     QString tmp;
     tmp.setNum(tries-1);
     labelBadPW->setText(tr("Invalid Password! Tries Left: %1").arg(tmp) );
     tries--;
     if ( tries == 0 )
       exit(1);
     passwordLineEdit->setText("");
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
  connect( sudoProc, SIGNAL(readyReadStandardError()), this, SLOT(slotPrintStdErr() ) );
  connect( sudoProc, SIGNAL(readyReadStandardOutput()), this, SLOT(slotPrintStdOut() ) );
  while(sudoProc->state() == QProcess::Starting ) {
     sudoProc->waitForFinished(500);
     QCoreApplication::processEvents();
  }
}

void MainWindow::slotPrintStdErr()
{
  QTextStream cout(stderr); 
  cout << sudoProc->readAllStandardError();
}

void MainWindow::slotPrintStdOut()
{
  QTextStream cout(stdout); 
  cout << sudoProc->readAllStandardOutput();
}

void MainWindow::slotPwdTextChanged(const QString &text)
{
    QPushButton* btn= buttonBox->button(QDialogButtonBox::Ok);
    if (btn)
        btn->setEnabled(text.length()?true:false);
}

void MainWindow::slotExpandCommandClicked(bool isChecked)
{
    commandLabel->setVisible(isChecked);
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

bool MainWindow::checkUserGroup()
{
   QString loginName = getlogin();
   QString groupName = "wheel"; // group to check
   QStringList gNames;
   if ( loginName == "root" )
     return true;
    
   QString tmp;
   QFile iFile("/etc/group");
   if ( ! iFile.open(QIODevice::ReadOnly | QIODevice::Text))
     return true; //or FALSE?
 
   while ( !iFile.atEnd() ) {
     tmp = iFile.readLine().simplified();
     if ( tmp.indexOf(groupName) == 0 ) {
        gNames = tmp.section(":", 3, 3).split(",");
        break;
     }
   }
   iFile.close();

   if ( gNames.isEmpty() )
      return false;

   for ( int i = 0; i < gNames.size(); ++i )
      if ( gNames.at(i).indexOf(loginName) == 0 )
            return true;

   return false;
}

