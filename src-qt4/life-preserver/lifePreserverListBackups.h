#ifndef LIFELISTBACKUPS_H
#define LIFELISTBACKUPS_H

/* QT4 */
#include <qfile.h>
#include <qdialog.h>

/* Local Includes */
#include "ui_lifePreserverListBackups.h"
#include "ui_lifePreserverRestore.h"
#include "lifePreserverRestore.h"
#include "preserver.h"


/* KDE */

class lifeListBackups : public QDialog, private Ui::lifeListBackups
{
        Q_OBJECT

public:
        lifeListBackups() : QDialog()
        {
          setupUi(this);
        }

   void widgetInit(QString preserver);


public slots:

private slots:
    void slotSelected();
    void slotCancel();
    void slotStartFetch();

private:
    PRESERVER *listPreserver;
    lifeRestore *lpr;

signals:

} ;
#endif // LIFELISTBACKUPS_H

