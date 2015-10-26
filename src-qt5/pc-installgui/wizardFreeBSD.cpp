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
#include "wizardFreeBSD.h"
#include "ui_wizardFreeBSD.h"
#include "backend.h"

void wizardFreeBSD::programInit(bool trueOS)
{
   connect(this,SIGNAL(currentIdChanged(int)),this,SLOT(slotCheckComplete()));
   connect(lineRootPW,SIGNAL(textChanged(const QString)),this,SLOT(slotCheckComplete()));
   connect(lineRootPW2,SIGNAL(textChanged(const QString)),this,SLOT(slotCheckComplete()));
   connect(lineName,SIGNAL(textChanged(const QString)),this,SLOT(slotCheckComplete()));
   connect(lineName,SIGNAL(editingFinished()),this,SLOT(slotSuggestUsername()));
   connect(lineUsername,SIGNAL(textChanged(const QString)),this,SLOT(slotCheckComplete()));
   connect(linePW,SIGNAL(textChanged(const QString)),this,SLOT(slotCheckComplete()));
   connect(linePW2,SIGNAL(textChanged(const QString)),this,SLOT(slotCheckComplete()));
   connect(lineHostname,SIGNAL(textChanged(const QString)),this,SLOT(slotCheckComplete()));

   // Load any nics
   QString tmp;
   comboSelectNic->clear();
   comboSelectNic->addItem("AUTO-DHCP-SLAAC");
   comboSelectNic->addItem("AUTO-DHCP");
   comboSelectNic->addItem("IPv6-SLAAC");
   QStringList sysNics = Scripts::Backend::networkDevices();
   for (int i=0; i < sysNics.count(); ++i) {
     tmp = sysNics.at(i);
     tmp.truncate(35);
     comboSelectNic->addItem(tmp);
   } 
   connect(comboSelectNic,SIGNAL(currentIndexChanged(int)), this, SLOT(slotChangedNic()));
   slotChangedNic();


   if ( trueOS ) {
      comboShell->addItem("/usr/local/bin/bash");
   }
}

void wizardFreeBSD::slotChangedNic()
{
  
  /* The labels are always disabled. */
  textIP->setEnabled(false);
  textNetmask->setEnabled(false);
  textDNS->setEnabled(false);
  textDefaultRouter->setEnabled(false);
  textIPv6->setEnabled(false);
  textIPv6DefaultRouter->setEnabled(false);
  textIPv6DNS->setEnabled(false);
  if ( comboSelectNic->currentText() == "AUTO-DHCP" )
  {
    lineIP->setEnabled(false);
    lineNetmask->setEnabled(false);
    lineDNS->setEnabled(false);
    lineDefaultRouter->setEnabled(false);
    lineIPv6->setEnabled(false);
    lineIPv6DefaultRouter->setEnabled(false);
    lineIPv6DNS->setEnabled(false);
  } else if ( comboSelectNic->currentText() == "IPv6-SLAAC" || comboSelectNic->currentText() == "AUTO-DHCP-SLAAC" ) {
    lineIP->setEnabled(false);
    lineNetmask->setEnabled(false);
    lineDNS->setEnabled(false);
    lineDefaultRouter->setEnabled(false);
    lineIPv6->setEnabled(true);
    lineIPv6DefaultRouter->setEnabled(true);
    lineIPv6DNS->setEnabled(true);
    textIPv6->setEnabled(true);
    textIPv6DefaultRouter->setEnabled(true);
    textIPv6DNS->setEnabled(true);
  } else {
    lineIP->setEnabled(true);
    lineNetmask->setEnabled(true);
    lineDNS->setEnabled(true);
    lineDefaultRouter->setEnabled(true);
    lineIPv6->setEnabled(true);
    lineIPv6DefaultRouter->setEnabled(true);
    lineIPv6DNS->setEnabled(true);
    textIP->setEnabled(true);
    textNetmask->setEnabled(true);
    textDNS->setEnabled(true);
    textDefaultRouter->setEnabled(true);
    textIPv6->setEnabled(true);
    textIPv6DefaultRouter->setEnabled(true);
    textIPv6DNS->setEnabled(true);
  }
        
}

void wizardFreeBSD::slotClose()
{
  close();
}

void wizardFreeBSD::accept()
{
  QStringList netSettings;
  netSettings << comboSelectNic->currentText() \
    << lineIP->text() \
    << lineNetmask->text() \
    << lineDNS->text() \
    << lineDefaultRouter->text() \
    << lineIPv6->text() \
    << lineIPv6DefaultRouter->text() \
    << lineIPv6DNS->text();

  QStringList appCafeSettings;
  if ( checkAppCafe->isChecked() )
  {
    appCafeSettings << "TRUE";
  } else {
    appCafeSettings << "FALSE";
  }

  emit saved(lineRootPW->text(), lineName->text(), lineUsername->text(), linePW->text(), comboShell->currentText(), lineHostname->text(), checkSSH->isChecked(), checkPorts->isChecked(), netSettings, appCafeSettings);
  close();
}

void wizardFreeBSD::slotSuggestUsername()
{
  if ( ! lineUsername->text().isEmpty() || lineName->text().isEmpty() )
    return;
  QString name;
  if ( lineName->text().indexOf(" ") != -1 ) {
    name = lineName->text().section(' ', 0, 0).toLower();
    name.truncate(1);
  }
  name = name + lineName->text().section(' ', -1, -1).toLower();
  lineUsername->setText(name);
}

// Logic checks to see if we are ready to move onto next page
bool wizardFreeBSD::validatePage()
{
  switch (currentId()) {
     case Page_Intro:
         button(QWizard::NextButton)->setEnabled(true);
         return true;
     case Page_Root:
         if ( lineRootPW->text().isEmpty() ) {
           button(QWizard::NextButton)->setEnabled(false);
           return true;
         }
         if ( lineRootPW2->text().isEmpty() ) {
           button(QWizard::NextButton)->setEnabled(false);
           return true;
         }
         if ( lineRootPW->text() != lineRootPW2->text() ) {
           button(QWizard::NextButton)->setEnabled(false);
           return false;
         }
         // if we get this far, all the fields are filled in
         button(QWizard::NextButton)->setEnabled(true);
         return true;
     case Page_User:
         if ( lineName->text().isEmpty() ) {
           button(QWizard::NextButton)->setEnabled(false);
           return false;
         }
         if ( lineUsername->text().isEmpty() ) {
           button(QWizard::NextButton)->setEnabled(false);
           return false;
         }
         if ( linePW->text().isEmpty() ) {
           button(QWizard::NextButton)->setEnabled(false);
           return false;
         }
         if ( linePW2->text().isEmpty() ) {
           button(QWizard::NextButton)->setEnabled(false);
           return false;
         }
         if ( linePW->text() != linePW2->text() ) {
           button(QWizard::NextButton)->setEnabled(false);
           return false;
         }
         button(QWizard::NextButton)->setEnabled(true);
         return true;
     case Page_Host:
         if ( lineHostname->text().isEmpty() ) {
           button(QWizard::NextButton)->setEnabled(false);
           return false;
         }
         button(QWizard::NextButton)->setEnabled(true);
         return true;
     case Page_AppCafe:
         // if we get this far, all the fields are filled in
         button(QWizard::NextButton)->setEnabled(true);
         return true;
     case Page_Optional:
         button(QWizard::FinishButton)->setEnabled(true);
         return true;
     default:
         button(QWizard::NextButton)->setEnabled(true);
         return true;
  }

  return true;
}

void wizardFreeBSD::slotCheckComplete()
{
   // Validate this page
   validatePage();
}

