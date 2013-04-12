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
#include <QSettings>
#include <QString>
#include <QFileDialog>

// Local Includes
#include "softmanager-main.h"
#include "ui_softconfigwidget.h"

void softwareConfigWidget::programInit(QString wDir, QString wIP)
{
  displayRepos();
  wardenDir = wDir;
  wardenIP = wIP;

  // Our buttons / slots
  connect( pushSave, SIGNAL( clicked() ), this, SLOT( slotSaveClicked() ) );
  connect( pushCancel, SIGNAL( clicked() ), this, SLOT( slotCancelClicked() ) );
  connect( pushAddRepo, SIGNAL( clicked() ), this, SLOT( slotAddClicked() ) );
  connect( pushDelRepo, SIGNAL( clicked() ), this, SLOT( slotRemClicked() ) );
  connect( pushRepoDown, SIGNAL( clicked() ), this, SLOT( slotMoveDownClicked() ) );
  connect( pushRepoUp, SIGNAL( clicked() ), this, SLOT( slotMoveUpClicked() ) );

  // Load up the settings
  loadSettings();
}

void softwareConfigWidget::slotCancelClicked()
{
   close();
}

bool softwareConfigWidget::sanityCheckSettings()
{
  return true;
}

void softwareConfigWidget::slotSaveClicked()
{
   if ( sanityCheckSettings() )
   {
     saveSettings();
     emit saved();
     close();
   }
}

void softwareConfigWidget::saveSettings()
{
	// Save the mirrors
	QStringList mirrors;
	for ( int i=0; i<listRepos->count() ; i++)
		mirrors << listRepos->item(i)->text();

        qDebug() << "Updating mirrors...";
        QProcess rpocmd;
        if ( wardenDir.isEmpty() ) 
          rpocmd.start(QString("pc-su"), QStringList() << "pbi_listrepo --mirror " + mirrors.join(",") + " " +  pbiRepos.getID(curRepo)  );
	else 
          rpocmd.start(QString("warden"), QStringList() << "chroot" << wardenIP << "pbi_listrepo --mirror " + mirrors.join(",") + " " +  pbiRepos.getID(curRepo)  );
	setEnabled(false);
        rpocmd.waitForFinished(1000);
        while ( rpocmd.state() != QProcess::NotRunning ) {
                rpocmd.waitForFinished(100);
                QCoreApplication::processEvents();
        }
	
}


void softwareConfigWidget::loadSettings() {
  // Load the user preferences for the System Updater
}

void softwareConfigWidget::loadRepos(Repo tmpRepos, int editRepo)
{
	pbiRepos = tmpRepos;
	curRepo = editRepo;
}

void softwareConfigWidget::displayRepos()
{
	listRepos->clear();
	listRepos->addItems(pbiRepos.getMirrors(curRepo).split(";"));
}

void softwareConfigWidget::slotAddClicked()
{
	bool ok;
	QString url = QInputDialog::getText(this, tr("Add Mirror"),
					tr("Mirror URL:"), QLineEdit::Normal,
					QString(), &ok);
	if ( ok )
		listRepos->addItem(url);
}

void softwareConfigWidget::slotRemClicked()
{
	if ( ! listRepos->currentItem() )
		return;

	delete listRepos->currentItem();
}

void softwareConfigWidget::slotMoveDownClicked()
{
	if ( ! listRepos->currentItem() )
		return;
	if ( listRepos->currentRow() >= listRepos->count() -1 )
		return;

	int cRow = listRepos->currentRow();
	QString tmp = listRepos->item(cRow)->text();
	listRepos->item(cRow)->setText(listRepos->item(cRow + 1)->text());
	listRepos->item(cRow + 1)->setText(tmp);
	listRepos->setCurrentRow(cRow + 1);
}

void softwareConfigWidget::slotMoveUpClicked()
{
	if ( ! listRepos->currentItem() )
		return;
	if ( listRepos->currentRow() <= 0 )
		return;

	int cRow = listRepos->currentRow();
	QString tmp = listRepos->item(cRow)->text();
	listRepos->item(cRow)->setText(listRepos->item(cRow - 1)->text());
	listRepos->item(cRow - 1)->setText(tmp);
	listRepos->setCurrentRow(cRow - 1);
}
