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
#include "updaterDialog.h"
#include "ui_updaterDialog.h"
#include <QTreeWidget>
#include <QtGui>

void updaterStatus::programInit()
{
    setWindowTitle(tr("Update Progress"));
    listInstallView->clear();
    listInstallView->setColumnHidden(3, true);
    listInstallView->resizeColumnToContents(0);
}



void updaterStatus::closeEvent( QCloseEvent *event )
{
    event->ignore();
}


void updaterStatus::insertStatusListBoxItem( QString &itemName, QString itemSize, QString itemStatus, int id )
{
    QStringList itemData;
    QString tmp;
    itemData << itemName << itemSize << itemStatus << tmp.setNum(id);
    new QTreeWidgetItem ( listInstallView, itemData ) ;
}


void updaterStatus::updateStatusListBoxItem( QString &itemStatus, int id )
{
    QString tmp;
    tmp.setNum(id);

    QTreeWidgetItemIterator it(listInstallView);
    while (*it) {
        if ( (*it)->text(3) == tmp)
        {
            (*it)->setText(2, itemStatus);
        }
        ++it;
    }
}


void updaterStatus::setLabelSysUpdateStatus( QString &text )
{
   textInstallStatusLabel->setText(text);
}


void updaterStatus::setProgressTotalSteps( int totalSteps )
{
  progressInstall->setMaximum(totalSteps);
}


void updaterStatus::setProgressSteps( int currentStep )
{
  progressInstall->setValue(currentStep);
}




void updaterStatus::clearWorkingItems()
{
     listInstallView->clear();
}


void updaterStatus::updateSizeListBoxItem( QString &itemSize, int id )
{
    QString tmp;
    tmp.setNum(id);

    QTreeWidgetItemIterator it(listInstallView);
    while ( *it ) {
        if ( (*it)->text(3) == tmp)
        {
            (*it)->setText(1, itemSize);
        }
        ++it;
    }
}


void updaterStatus::slotJobUpdateProcessedSize( qint64 rec, qint64 total )
{
   totalSize = total;
   processedSize = rec;
   progressInstall->setMaximum(total);
   progressInstall->setValue(rec);
   generateCopyLabel();
}


void updaterStatus::generateCopyLabel()
{
   QString totalSize = QString::number(this->totalSize / 1048576) + "MB";
   QString processedSize = QString::number(this->processedSize / 1048576) + "MB";
   //QString speed = QString::number(this->speed / 1024) + "KB/s";
   //textInstallStatusLabel->setText(processedSize + tr(" of ") + totalSize + " (" + speed + ")");
   textInstallStatusLabel->setText(processedSize + tr(" of ") + totalSize);
}
