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
#include "wizardNewJail.h"
#include "pcbsd-utils.h"
#include <QDebug>
#include <QProcess>
#include <QFileDialog>


void wizardNewJail::programInit()
{
    // Connect our checks
    connect(lineRoot, SIGNAL(textChanged ( const QString & )), this, SLOT(slotCheckComplete() ) );
    connect(lineRoot2, SIGNAL(textChanged ( const QString & )), this, SLOT(slotCheckComplete() ) );
    connect(lineIP, SIGNAL(textChanged ( const QString & )), this, SLOT(slotCheckComplete() ) );
    connect(lineIP6, SIGNAL(textChanged ( const QString & )), this, SLOT(slotCheckComplete() ) );
    connect(checkIPv4, SIGNAL(clicked(bool)), this, SLOT(slotCheckChecks()));
    connect(checkIPv6, SIGNAL(clicked(bool)), this, SLOT(slotCheckChecks()));
    connect(lineHost, SIGNAL(editingFinished()), this, SLOT(slotCheckComplete() ) );
    connect(lineLinuxScript, SIGNAL(textChanged ( const QString & )), this, SLOT(slotCheckComplete() ) );
    connect(pushLinuxScript, SIGNAL(clicked()), this, SLOT(slotSelectLinuxScript()) );
    connect(this, SIGNAL(currentIdChanged(int)), this, SLOT(slotCheckComplete()) );
    loadTemplates();
}

void wizardNewJail::loadTemplates()
{
   comboTemplate->clear();
   comboTemplate->addItem(tr("Use system version (default)"));
   // Load any available templates
   QProcess m;
   m.start(QString("warden"), QStringList() << "template" << "list");
   while(m.state() == QProcess::Starting || m.state() == QProcess::Running) {
      m.waitForFinished(200);
      QCoreApplication::processEvents();
   }
   int i = 0;
   QString tmp;
   while (m.canReadLine()) {
     i++;
     tmp = m.readLine().simplified();
     // Skip first two header lines
     if ( i > 2)
       comboTemplate->addItem(tmp);
   }
}

void wizardNewJail::slotCheckChecks()
{
   if ( checkIPv4->isChecked() )
      lineIP->setEnabled(true);
   else
      lineIP->setEnabled(false);

   if ( checkIPv6->isChecked() )
      lineIP6->setEnabled(true);
   else
      lineIP6->setEnabled(false);

   slotCheckComplete();
}

void wizardNewJail::setHostIPUsed(QStringList uH, QStringList uIP)
{
   usedHosts = uH;
   usedIP = uIP;
}

void wizardNewJail::accept()
{
    QString ip4, ip6, tplate;
    if ( checkIPv4->isChecked() )
       ip4 = lineIP->text();
    if ( checkIPv6->isChecked() )
       ip6 = lineIP6->text();

    if ( comboTemplate->currentIndex() != 0 )
    tplate = comboTemplate->currentText().section(" ", 0, 0);
    
    emit create(ip4, ip6, lineHost->text(), radioTraditionalJail->isChecked(), checkPCBSDUtils->isChecked(),
                lineRoot->text(), checkSystemSource->isChecked(), checkPortsTree->isChecked(),
                checkAutostart->isChecked(), radioLinuxJail->isChecked(), lineLinuxScript->text(), tplate);
    close();
    
}

void wizardNewJail::slotClose()
{
  close();
}

// Logic checks to see if we are ready to move onto next page
bool wizardNewJail::validatePage()
{
  QColor red = QColor(255, 78, 78);
  QColor white = QColor(255, 255, 255);
  QPalette badPal(red);
  badPal.setColor(QPalette::Window,red);
  badPal.setColor(QPalette::WindowText,red);
  QPalette goodPal(white);
  goodPal.setColor(QPalette::Window,white);
  goodPal.setColor(QPalette::WindowText,white);
  labelMessage->setText(QString());
  bool ok = true;

  switch (currentId()) {
     case Page_IP:
         // Make sure items are not empty.  Also check for invalid characters
         if ( checkIPv4->isChecked() && lineIP->text().isEmpty() ) {
            ok=false;
	 }
         if ( checkIPv6->isChecked() && lineIP6->text().isEmpty() ) {
            ok=false;
	 }
         if ( lineHost->text().isEmpty() ) {
            ok=false;
	 }
         if ( lineHost->text().contains(" ") ) {
            labelMessage->setText(tr("Hostname cannot contain spaces!"));
	    ok=false;
	 }
	 if ( lineHost->text().contains("~") || lineHost->text().contains("`") || lineHost->text().contains("!") || lineHost->text().contains("@") || lineHost->text().contains("#") || lineHost->text().contains("$") || lineHost->text().contains("%") || lineHost->text().contains("^") || lineHost->text().contains("&") || lineHost->text().contains("*") || lineHost->text().contains("(") || lineHost->text().contains(")") || lineHost->text().contains("_") || lineHost->text().contains("+") || lineHost->text().contains("=") || lineHost->text().contains(";") || lineHost->text().contains(":") || lineHost->text().contains("'") || lineHost->text().contains("|") || lineHost->text().contains("?") || lineHost->text().contains("<") || lineHost->text().contains(">") || lineHost->text().contains(".") || lineHost->text().contains("?") || lineHost->text().contains(",") ) {
            button(QWizard::NextButton)->setEnabled(false);
            labelMessage->setText(tr("Hostname cannot contain special characters!"));
            return false;
	 }
	 // Check if this IP / Host is already used
         for (int i = 0; i < usedHosts.size(); ++i) {
            if ( usedHosts.at(i).toLower() == lineHost->text().toLower() ) {
               lineHost->setPalette(badPal);
               labelMessage->setText(tr("Hostname already used!"));
	       ok=false;
	    }
	 }
         for (int i = 0; i < usedIP.size(); ++i)
            if ( usedIP.at(i).contains(lineIP->text() + "/24") ) {
               lineIP->setPalette(badPal);
               labelMessage->setText(tr("IP already used!"));
	       ok=false;
	       break;
	    }

         // Check if we have a good IPV4 or IPV6 address
	 if ( checkIPv4->isChecked() && ! pcbsd::Utils::validateIPV4(lineIP->text())) {
           lineIP->setPalette(badPal);
           labelMessage->setText(tr("Invalid IP address!"));
	   ok=false;
         }

	 if ( checkIPv6->isChecked() && ! pcbsd::Utils::validateIPV6(lineIP6->text()) ) {
           lineIP6->setPalette(badPal);
           labelMessage->setText(tr("Invalid IPv6 address!"));
	   ok=false;
         }
	if(!ok){
	   button(QWizard::NextButton)->setEnabled(false);
	}else{
           // Got to the end, must be good!
           lineIP->setPalette(goodPal);
           lineHost->setPalette(goodPal);
           button(QWizard::NextButton)->setEnabled(true);
	}
     case Page_Root:
	 if ( lineRoot->text() != lineRoot2->text() ) {
           button(QWizard::NextButton)->setEnabled(false);
	   ok = false;
         } else {
           button(QWizard::NextButton)->setEnabled(true);
	 }
         return true;
     case Page_Linux:
	  if ( lineLinuxScript->text().isEmpty() ) {
            button(QWizard::NextButton)->setEnabled(false);
	    ok = false;
	  }
     default:
         button(QWizard::NextButton)->setEnabled(true);
  }

  return ok;
}

int wizardNewJail::nextId() const
{
  switch (currentId()) {
     case Page_Type:
         if (radioPortsJail->isChecked())
           return Page_Opts;
       break;
       case Page_Root:
	 checkSystemSource->setHidden(false); 
	 checkPortsTree->setHidden(false); 
         if (radioLinuxJail->isChecked()) {
	   labelConfirm->setText(tr("Please take a moment and set any other options for this jail."));
           return Page_Linux;
	 } else {
           return Page_Opts;
	 }
       break;
       case Page_Linux:
	   checkSystemSource->setHidden(true); 
	   checkPortsTree->setHidden(true); 
           return Page_Opts;
       break;
       case Page_Opts:
         return -1;
       break;
     default:
        return currentId() + 1;
  }
  return currentId() + 1;
}

void wizardNewJail::slotCheckComplete()
{
   // Validate this page
   validatePage();
}

void wizardNewJail::slotSelectLinuxScript()
{
   lineLinuxScript->setText(QFileDialog::getOpenFileName(this,
   tr("Select Script"), "/usr/local/share/warden/linux-installs", tr("Linux install files (*)")));
}


