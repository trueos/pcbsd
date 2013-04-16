#ifndef LIFERESTORE_H
#define LIFERESTORE_H

/* QT4 */
#include <qfile.h>
#include <qdialog.h>
#include <QProcess>

/* Local Includes */
#include "ui_lifePreserverRestore.h"
#include "ui_lifePreserverRestoreProgress.h"
#include "lifePreserverRestoreProgress.h"
#include "preserver.h"

/* KDE */

class lifeRestore : public QDialog, private Ui::lifeRestore
{
        Q_OBJECT

public:
        lifeRestore() : QDialog()
        {
          setupUi(this);
        }

   void widgetInit(QString preserver, QString targetBackup);


public slots:

private slots:
    void slotCheckReady();
    void slotRelativeChanged();
    void slotRestore();
    void slotCancel();
    void slotSelRelDir();
    void slotStartRestore();

private:
    PRESERVER *restorePreserver;
    lifeRestoreProgress *lprp;
    QString workingBackup;

signals:

} ;
#endif // LIFERESTORE_H

