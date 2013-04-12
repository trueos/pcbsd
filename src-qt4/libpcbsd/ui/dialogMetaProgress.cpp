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
#include "dialogMetaProgress.h"
#include "ui_dialogMetaProgress.h"
#include <QtGui>
#include <QMessageBox>

void dialogMetaProgress::programInit( QString desc )
{
    setWindowTitle(tr("Update Progress"));
    labelDescription->setText(desc);
    progressTotal->setRange(0,0);
    progressTotal->setValue(0);
    progressSub->setRange(0,0);
    progressSub->setValue(0);
    progressDownload->setRange(0,0);
    progressDownload->setValue(0);
    setDLVisible(false);
}

void dialogMetaProgress::setTotalDesc( QString desc )
{
    labelTotal->setText(desc);
}

void dialogMetaProgress::setSubDesc( QString desc )
{
    labelSub->setText(desc);
}

void dialogMetaProgress::setTotalRange( int min, int max )
{
    progressTotal->setRange(min, max);
}

void dialogMetaProgress::setTotalVal( int val )
{
    progressTotal->setValue(val);
}

int dialogMetaProgress::getTotalVal( )
{
    return progressTotal->value();
}

void dialogMetaProgress::setSubRange( int min, int max )
{
    progressSub->setRange(min, max);
}

void dialogMetaProgress::setSubVal( int val )
{
    progressSub->setValue(val);
}

int dialogMetaProgress::getSubVal( )
{
    return progressSub->value();
}

void dialogMetaProgress::closeEvent(QCloseEvent *event)
{
   Q_UNUSED(event);

   int ret = QMessageBox::warning(this, tr("Meta-Package Changes"),
                                tr("Warning: Packages are still being modifed!\n" 
				"If you cancel now the packages may be corrupted. Cancel anyway?"),
                                QMessageBox::Yes | QMessageBox::No,
                                QMessageBox::No);

   if ( ret == QMessageBox::Yes) {
     emit canceled();
     close();
     return;
   }

   event->ignore();
}

void dialogMetaProgress::setDLVisible( bool isVis)
{
   labelDownload->setVisible(isVis);
   progressDownload->setVisible(isVis);
}

void dialogMetaProgress::setDLDesc( QString desc )
{
    labelDownload->setText(desc);
}

void dialogMetaProgress::setDLRange( int min, int max )
{
    progressDownload->setRange(min, max);
}

void dialogMetaProgress::setDLVal( int val )
{
    progressDownload->setValue(val);
}

int dialogMetaProgress::getDLVal( )
{
    return progressDownload->value();
}
