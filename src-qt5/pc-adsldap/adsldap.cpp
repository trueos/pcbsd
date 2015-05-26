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
#include <QTextStream>
#include <QTextBlock>
#include <QString>
#include <QDebug>
#include <QSettings>
#include <pcbsd-utils.h>
#include "adsldap.h"

#define ADCONF QString("/usr/local/etc/pc-activedirectory.conf")
#define LDCONF QString("/usr/local/etc/pc-ldap.conf")

void pcADSLDAP::programInit()
{
  connect(pushClose, SIGNAL(clicked()), this, SLOT(slotClose()));
  connect(pushSave, SIGNAL(clicked()), this, SLOT(slotSave()));

  loadSettings();
}

void pcADSLDAP::slotClose()
{
   saveSettings();
   close();
}

void pcADSLDAP::slotSave()
{
   saveSettings();
   exportSettings();
}

void pcADSLDAP::saveSettings()
{
   // Save ADS settings
   QSettings settings("PCBSD", "ADS");
   settings.setValue("Enabled", groupADS->isChecked());

   settings.setValue("DomainName", lineADSDomainName->text());
   settings.setValue("NetBIOS", lineADSNetBIOS->text());
   settings.setValue("Workgroup", lineADSWorkgroup->text());
   settings.setValue("TrustedDomains", checkADSTrustedDomains->isChecked());
   settings.setValue("AdminName", lineADSAdminName->text());
   settings.setValue("AdminPW", lineADSAdminPW->text());

   QSettings settings2("PCBSD", "LDAP");
   settings2.setValue("Enabled", groupLDAP->isChecked());
   settings2.setValue("AnonBinding", checkLDAnonBinding->isChecked());
   settings2.setValue("EncMode", comboLDEncMode->currentIndex());
   settings2.setValue("PWEnc", comboLDPWEnc->currentIndex());
   settings2.setValue("BaseDN", lineLDBaseDN->text());
   settings2.setValue("GroupSuffix", lineLDGroupSuffix->text());
   settings2.setValue("Hostname", lineLDHostname->text());
   settings2.setValue("MachineSuffix", lineLDMachineSuffix->text());
   settings2.setValue("PWSuffix", lineLDPWSuffix->text());
   settings2.setValue("RootBindDN", lineLDRootBindDN->text());
   settings2.setValue("RootBindPW", lineLDRootBindPW->text());
   settings2.setValue("SelfSignedCert", lineLDSelfSignedCert->text());
   settings2.setValue("UserSuffix", lineLDUserSuffix->text());
   settings2.setValue("Aux", textLDAux->toPlainText());


   
}

void pcADSLDAP::loadSettings()
{
  QSettings settings("PCBSD", "ADS");
  QString tmp = pcbsd::Utils::getConfFileValue("/etc/rc.conf", "pc_activedirectory_enable=", 1) ;
  if ( tmp == "YES" ) 
    groupADS->setChecked(true);
  else
    groupADS->setChecked(false);

  lineADSDomainName->setText(settings.value("DomainName", QString()).toString());
  lineADSNetBIOS->setText(settings.value("NetBIOS", QString()).toString());
  lineADSWorkgroup->setText(settings.value("Workgroup", QString()).toString());
  checkADSTrustedDomains->setChecked(settings.value("TrustedDomains", false).toBool());
  lineADSAdminName->setText(settings.value("AdminName", QString()).toString());
  lineADSAdminPW->setText(settings.value("AdminPW", QString()).toString());
  lineADSAdminPW2->setText(settings.value("AdminPW", QString()).toString());

  QSettings settings2("PCBSD", "LDAP");
  groupLDAP->setChecked(settings2.value("Enabled", false).toBool());
  checkLDAnonBinding->setChecked(settings2.value("AnonBinding", false).toBool());
  comboLDEncMode->setCurrentIndex(settings2.value("EncMode", 0).toInt());
  comboLDPWEnc->setCurrentIndex(settings2.value("PWEnc", 0).toInt());
  lineLDBaseDN->setText(settings2.value("BaseDN", QString()).toString());
  lineLDGroupSuffix->setText(settings2.value("GroupSuffix", QString()).toString());
  lineLDHostname->setText(settings2.value("Hostname", QString()).toString());
  lineLDMachineSuffix->setText(settings2.value("MachineSuffix", QString()).toString());
  lineLDPWSuffix->setText(settings2.value("PWSuffix", QString()).toString());
  lineLDRootBindDN->setText(settings2.value("RootBindDN", QString()).toString());
  lineLDRootBindPW->setText(settings2.value("RootBindPW", QString()).toString());
  lineLDSelfSignedCert->setText(settings2.value("SelfSignedCert", QString()).toString());
  lineLDUserSuffix->setText(settings2.value("UserSuffix", QString()).toString());
  textLDAux->insertPlainText(settings2.value("Aux", QString()).toString());

}

void pcADSLDAP::exportAD()
{
  // Validate that passwords match
  if ( lineADSAdminPW->text() != lineADSAdminPW2->text() )
  {
     QMessageBox::warning(this, tr("Active Directory Configuration"),
                                tr("The Active Directory passwords specified do not match!"),
                                QMessageBox::Ok,
                                QMessageBox::Ok);
      return;
  }

  QString tmp;

  // Now write ADCONF values
  pcbsd::Utils::setConfFileValue(ADCONF, "ad_domainname =", "ad_domainname = " + lineADSDomainName->text(), -1);
  pcbsd::Utils::setConfFileValue(ADCONF, "ad_adminname =", "ad_adminname = " + lineADSAdminName->text(), -1);
  pcbsd::Utils::setConfFileValue(ADCONF, "ad_adminpw =", "ad_adminpw = " + lineADSAdminPW->text(), -1);
  pcbsd::Utils::setConfFileValue(ADCONF, "ad_workgroup =", "ad_workgroup = " + lineADSWorkgroup->text().toUpper(), -1);
  pcbsd::Utils::setConfFileValue(ADCONF, "ad_netbiosname =", "ad_netbiosname = " + lineADSNetBIOS->text().toUpper(), -1);
  
  if ( checkADSTrustedDomains->isChecked() )
    pcbsd::Utils::setConfFileValue(ADCONF, "ad_allow_trusted_doms =", "ad_allow_trusted_doms = yes", -1);
  else
    pcbsd::Utils::setConfFileValue(ADCONF, "ad_allow_trusted_doms =", "ad_allow_trusted_doms = no", -1);

}

void pcADSLDAP::exportLDAP()
{
  pcbsd::Utils::setConfFileValue(LDCONF, "ldap_hostname =", "ldap_hostname = " + lineLDHostname->text(), -1);
  pcbsd::Utils::setConfFileValue(LDCONF, "ldap_basedn =", "ldap_basedn = " + lineLDBaseDN->text(), -1);
  if ( checkLDAnonBinding->isChecked() )
     pcbsd::Utils::setConfFileValue(LDCONF, "ldap_anonbind =", "ldap_anonbind = yes", -1);
  else
     pcbsd::Utils::setConfFileValue(LDCONF, "ldap_anonbind =", "ldap_anonbind = no", -1);
  pcbsd::Utils::setConfFileValue(LDCONF, "ldap_rootbasedn =", "ldap_rootbasedn = " + lineLDRootBindDN->text(), -1);
  pcbsd::Utils::setConfFileValue(LDCONF, "ldap_rootbindpw =", "ldap_rootbindpw = " + lineLDRootBindPW->text(), -1);
  pcbsd::Utils::setConfFileValue(LDCONF, "ldap_rootbindpw =", "ldap_rootbindpw = " + lineLDRootBindPW->text(), -1);
  pcbsd::Utils::setConfFileValue(LDCONF, "ldap_pwencryption =", "ldap_pwencryption = " + comboLDPWEnc->currentText(), -1);
  pcbsd::Utils::setConfFileValue(LDCONF, "ldap_usersuffix =", "ldap_usersuffix = " + lineLDUserSuffix->text(), -1);
  pcbsd::Utils::setConfFileValue(LDCONF, "ldap_groupsuffix =", "ldap_groupsuffix = " + lineLDGroupSuffix->text(), -1);
  pcbsd::Utils::setConfFileValue(LDCONF, "ldap_passwordstuff =", "ldap_passwordstuff = " + lineLDPWSuffix->text(), -1);
  pcbsd::Utils::setConfFileValue(LDCONF, "ldap_machinesuffix =", "ldap_machinesuffix = " + lineLDMachineSuffix->text(), -1);
  pcbsd::Utils::setConfFileValue(LDCONF, "ldap_encryption_mode =", "ldap_encryption_mode = " + comboLDEncMode->currentText().toLower(), -1);
  pcbsd::Utils::setConfFileValue(LDCONF, "ldap_tls_cacertfile =", "ldap_tls_cacertfile = " + lineLDSelfSignedCert->text(), -1);

  // Remove old opts
  QString optName, optVal;
  pcbsd::Utils::setConfFileValue(LDCONF, "opt_", "", -1);
  for ( int i = 0; i < textLDAux->document()->lineCount(); ++i)
  { 

     // Make sure we have a value and key
     if ( textLDAux->document()->findBlockByLineNumber(i).text().section('=', 0, 0).isEmpty() )
       continue;
     if ( textLDAux->document()->findBlockByLineNumber(i).text().section('=', 1).isEmpty() ) 
       continue;

     optName = "opt_" + textLDAux->document()->findBlockByLineNumber(i).text().section('=', 0, 0).simplified();

     optVal = textLDAux->document()->findBlockByLineNumber(i).text().section('=', 1).simplified();

     qDebug() << textLDAux->document()->findBlockByLineNumber(i).text() << optName << optVal;

     // Save the config
     pcbsd::Utils::setConfFileValue(LDCONF, optName + " =", optName + " = " + optVal, -1);
  }
}

void pcADSLDAP::exportSettings()
{
  // Start adding AD.conf values
  if ( groupADS->isChecked() ) {
    exportAD();
    // Restart AD service if enabled   
    qDebug() << "Enabling AD";
    pcbsd::Utils::setConfFileValue("/etc/rc.conf", "pc_activedirectory_enable=", "pc_activedirectory_enable=\"YES\"", -1);
    pcbsd::Utils::setConfFileValue("/etc/rc.conf", "samba_server_enable=", "samba_server_enable=\"YES\"", -1);
    pcbsd::Utils::setConfFileValue("/etc/rc.conf", "winbindd_enable=", "winbindd_enable=\"YES\"", -1);
    pcbsd::Utils::runShellCommand("/usr/local/bin/pc-adctl start");
  } else {
     qDebug() << "Disabling AD";
     pcbsd::Utils::runShellCommand("/usr/local/bin/pc-adctl stop");
     pcbsd::Utils::setConfFileValue("/etc/rc.conf", "pc_activedirectory_enable=", "pc_activedirectory_enable=\"NO\"", -1);
     pcbsd::Utils::setConfFileValue("/etc/rc.conf", "samba_server_enable=", "samba_server_enable=\"NO\"", -1);
     pcbsd::Utils::setConfFileValue("/etc/rc.conf", "winbindd_enable=", "winbindd_enable=\"NO\"", -1);
  }
  
  // Enable the LDAP service
  if ( groupLDAP->isChecked() ) {
    exportLDAP();
     pcbsd::Utils::setConfFileValue("/etc/rc.conf", "pc_ldap_enable=", "pc_ldap_enable=\"YES\"", -1);
     pcbsd::Utils::runShellCommand("/usr/local/bin/pc-ldapctl start");
  } else {
     qDebug() << "Disabling LDAP";
     pcbsd::Utils::runShellCommand("/usr/local/bin/pc-ldapctl stop");
     pcbsd::Utils::setConfFileValue("/etc/rc.conf", "pc_ldap_enable=", "pc_ldap_enable=\"NO\"", -1);
  }
}
