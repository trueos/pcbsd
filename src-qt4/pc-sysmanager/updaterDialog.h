
#ifndef UPDATERDIALOG_H
#define UPDATERDIALOG_H

#include "ui_updaterDialog.h"
#include <qstring.h>

class updaterStatus : public QDialog, private Ui::updaterStatus
{
	Q_OBJECT

public:
        updaterStatus() : QDialog()
        {
          setupUi(this);
        }

    void programInit();
    void insertStatusListBoxItem( QString &itemName, QString itemSize, QString itemStatus, int id );
    void updateStatusListBoxItem( QString &itemStatus, int id );
    void setLabelSysUpdateStatus( QString &text );
    void setProgressTotalSteps( int totalSteps );
    void setProgressSteps( int currentStep );
    void clearWorkingItems();
    void updateSizeListBoxItem( QString &itemSize, int id );
    void generateCopyLabel();


public slots:
    void slotJobUpdateProcessedSize(qint64 rec, qint64 total);

protected:
    void closeEvent( QCloseEvent *event );

private slots:

private:
    unsigned long speed;
    qint64 totalSize;
    qint64 processedSize;

signals:

} ;
#endif // UPDATERDIALOG_H
