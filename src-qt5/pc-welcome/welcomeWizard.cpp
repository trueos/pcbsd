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
#include "welcomeWizard.h"
#include "ui_welcomeWizard.h"
#include <QDesktopServices>
#include <QCheckBox>
#include <QFile>

void welcomeWizard::programInit( )
{

  checkDisableWelcome = new QCheckBox();
  checkDisableWelcome->setText(tr("Don't show on next startup"));

  setButton(QWizard::CustomButton1, checkDisableWelcome);
  QList<QWizard::WizardButton> layout;
  layout << QWizard::CustomButton1 << QWizard::Stretch << QWizard::BackButton 	 << QWizard::NextButton << QWizard::FinishButton;
  setButtonLayout(layout);

  button(QWizard::NextButton)->setText(tr("&Next"));
  button(QWizard::BackButton)->setText(tr("&Back"));
  button(QWizard::FinishButton)->setText(tr("&Finish"));

  connect(checkDisableWelcome, SIGNAL(clicked()), this, SLOT(slotCheckDisabled()) );
}

void welcomeWizard::slotCheckDisabled()
{
  QString username = QString::fromLocal8Bit(getenv("LOGNAME"));

  QString filename = "/home/" + username + "/.config/hide-welcome";
  QFile tmpFile(filename);

  if ( checkDisableWelcome->isChecked() )
    system("touch " + filename.toLatin1() );
  else
    tmpFile.remove();
   
}
