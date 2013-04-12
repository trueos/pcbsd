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
#include <QInputDialog>
#include <QProcess>
#include <QString>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include "pcbsd-utils.h"

// Local Includes
#include "dialogEditIP.h"

void dialogEditIP::programInit(QString jIP, QStringList IPs)
{
  wardenIP = jIP;
  listIP->clear();
  for ( int i=0; i<IPs.count() ; i++)
     if ( ! IPs.at(i).isEmpty() )
	listIP->addItem(IPs.at(i));

  // Our buttons / slots
  connect( pushSave, SIGNAL( clicked() ), this, SLOT( slotSaveClicked() ) );
  connect( pushCancel, SIGNAL( clicked() ), this, SLOT( slotCancelClicked() ) );
  connect( pushAdd, SIGNAL( clicked() ), this, SLOT( slotAddClicked() ) );
  connect( pushRemove, SIGNAL( clicked() ), this, SLOT( slotRemClicked() ) );
}

void dialogEditIP::slotCancelClicked()
{
   close();
}

bool dialogEditIP::sanityCheckSettings()
{
  return true;
}

void dialogEditIP::slotSaveClicked()
{
   if ( sanityCheckSettings() )
   {
     saveSettings();
     emit saved();
     close();
   }
}

void dialogEditIP::saveSettings()
{
	QStringList IPs;
	for ( int i=0; i<listIP->count() ; i++)
		IPs << listIP->item(i)->text();

        qDebug() << "Updating IPs...";
        QProcess ipcmd;
        if ( IPs.count() == 0 )
          ipcmd.start(QString("warden"), QStringList() << "set" << "ip" << wardenIP << "" );
        else
          ipcmd.start(QString("warden"), QStringList() << "set" << "ip" << wardenIP << IPs.join(",") );

	setEnabled(false);
        ipcmd.waitForFinished(1000);
        while ( ipcmd.state() != QProcess::NotRunning ) {
                ipcmd.waitForFinished(100);
                QCoreApplication::processEvents();
        }
	
}

void dialogEditIP::slotAddClicked()
{
	bool ok;
	QString url = QInputDialog::getText(this, tr("Add IP"),
					tr("IP Address:"), QLineEdit::Normal,
					QString(), &ok);
	if ( ok ) {
          if ( Utils::validateIPV4(url) || Utils::validateIPV6(url) )
	    listIP->addItem(url);
          else
	    QMessageBox::critical(this, tr("Warden"), \
                                tr("Please enter a valid IPV4 or IPV6 address!"), \
                                QMessageBox::Ok, \
                                QMessageBox::Ok);
        }
}

void dialogEditIP::slotRemClicked()
{
	if ( ! listIP->currentItem() )
		return;

	delete listIP->currentItem();
}
