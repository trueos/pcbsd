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
#include "dialogTemplates.h"
#include "QInputDialog"
#include "QProcess"
#include "QDebug"


void dialogTemplates::programInit()
{
   connect( pushClose, SIGNAL(clicked()), this, SLOT( slotClose() ) );
   connect( pushRemove, SIGNAL(clicked()), this, SLOT( slotRemove() ) );
   connect( pushAdd, SIGNAL(clicked()), this, SLOT( slotAdd() ) );
   loadTemps();
}

void dialogTemplates::loadTemps()
{
   listTemps->clear();

   // Load the templates
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
       new QTreeWidgetItem( listTemps, QStringList() << tmp.section(" ", 0, 0) \
	<< tmp.section(" - ", 1, 1).remove(tmp.section(" - ", 1, 1).lastIndexOf("("), tmp.size() ) \
	<< tmp.section("(", 1,1).replace(")", "") );
   }

}

void dialogTemplates::slotAdd()
{
    bool ok;

    QString type = QInputDialog::getItem( this, "System Type", "System Type", QStringList() << "TrueOS" << "FreeBSD", 0, false);
    if ( type != "FreeBSD" && type != "TrueOS" )
       return;

    QString ver = QInputDialog::getText(this, tr("System Version"),
                                          tr("What version do you wish to fetch?"), QLineEdit::Normal,
                                          "9.1-RELEASE", &ok);
    if (!ok || ver.isEmpty())
       return;

    QString arch = QInputDialog::getItem( this, "System Arch", "System Architecture", QStringList() << "amd64" << "i386", 0, false);
    if ( arch != "amd64" && type != "i386" )
       return;

    QString nick = QInputDialog::getText(this, tr("Template Nickname"),
                                          tr("Enter a nickname for this template:"), QLineEdit::Normal,
                                          "mytemplate", &ok);
    if (!ok || nick.isEmpty())
       return;

   dialogOutput = new dialogDisplayOutput();
   dialogOutput->setModal(true);
   dialogOutput->programInit(false);
   dialogOutput->setDialogCaption("Creating Template:");
   dialogOutput->setDialogText("");
   dialogOutput->show();

   QProcess m;
   m.setProcessChannelMode(QProcess::MergedChannels);
   m.setReadChannel(QProcess::StandardOutput);

   QStringList flags;
   flags << "template" << "create";
   if ( type == "TrueOS" )
     flags << "-trueos" << ver;
   else
     flags << "-fbsd" << ver;

   flags << "-arch" << arch;

   flags << "-nick" << nick;


   m.start(QString("warden"), flags);
   while(m.state() == QProcess::Starting || m.state() == QProcess::Running) {
      m.waitForFinished(200);
      while (m.canReadLine()) {
        dialogOutput->appendDialogText(m.readLine().simplified() + "\n");
      }
      QCoreApplication::processEvents();
   }
   while (m.canReadLine()) {
     dialogOutput->appendDialogText(m.readLine().simplified() + "\n");
   }
   dialogOutput->setCloseHide(false);
   loadTemps();

}

void dialogTemplates::slotRemove()
{
   if ( ! listTemps->currentItem()) 
      return;

   dialogOutput = new dialogDisplayOutput();
   dialogOutput->setModal(true);
   dialogOutput->programInit(false);
   dialogOutput->setDialogCaption("Removing Template: " + listTemps->currentItem()->text(0));
   dialogOutput->setDialogText("");
   dialogOutput->show();

   dialogOutput->appendDialogText("Removing template:" + listTemps->currentItem()->text(0) + "\n");

   QProcess m;
   m.setProcessChannelMode(QProcess::MergedChannels);
   m.setReadChannel(QProcess::StandardOutput);

   m.start(QString("warden"), QStringList() << "template" << "delete" << listTemps->currentItem()->text(0));
   while(m.state() == QProcess::Starting || m.state() == QProcess::Running) {
      m.waitForFinished(200);
      while (m.canReadLine()) {
        dialogOutput->appendDialogText(m.readLine().simplified() + "\n");
      }
      QCoreApplication::processEvents();
   }
   while (m.canReadLine()) {
     dialogOutput->appendDialogText(m.readLine().simplified() + "\n");
   }
   dialogOutput->setCloseHide(false);
   loadTemps();
}

void dialogTemplates::slotClose()
{
  close();
}
