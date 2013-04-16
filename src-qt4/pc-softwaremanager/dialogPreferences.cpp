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
// QT Includes
#include <QSettings>
#include <QString>

// Local Includes
#include "ui_dialogPreferences.h"
#include "dialogPreferences.h"

void dialogPreferences::programInit()
{
  // Our buttons / slots
  connect( pushOK, SIGNAL( clicked() ), this, SLOT( slotSaveClicked() ) );
  connect( pushCancel, SIGNAL( clicked() ), this, SLOT( slotCancelClicked() ) );

  QSettings settings("PCBSD", "AppCafe");
  checkMenuIcon->setChecked(settings.value("menuIcons", true).toBool());
  checkDesktopIcon->setChecked(settings.value("deskIcons", true).toBool());

  // We can hide this for now, pbi_add auto creates menu icons
  // If this ever changes we can re-enable
  checkMenuIcon->setEnabled(false);
  checkMenuIcon->setVisible(false);
}

void dialogPreferences::slotCancelClicked()
{
   close();
}

void dialogPreferences::slotSaveClicked()
{
   QSettings settings("PCBSD", "AppCafe");
   settings.setValue("menuIcons", checkMenuIcon->isChecked());
   settings.setValue("deskIcons", checkDesktopIcon->isChecked());
   emit saved();
   close();
}
