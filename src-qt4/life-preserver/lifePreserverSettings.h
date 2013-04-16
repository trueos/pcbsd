#ifndef PSETTINGS_H
#define PSETTINGS_H

/* QT4 */
#include <qfile.h>
#include <qdialog.h>
#include <QProcess>

/* Local Includes */
#include "ui_lifePreserverSettings.h"
#include "preserver.h"

#include "ui_lifePreserverUserExclude.h"
#include "lifePreserverUserExclude.h"


/* KDE */

class pSettings : public QDialog, private Ui::pSettings
{
        Q_OBJECT

public:
        pSettings() : QDialog()
        {
          setupUi(this);
        }

   void widgetInit(QString preserver);


public slots:

private slots:
    void slotOK();
    void slotCancel();
    void slotRadioChanged();
    void slotModifyExcludes();
    void slotModifyIncludes();

private:
    void savePreserver();
    PRESERVER *editPreserver;
    pUserExclude *editExclude;

signals:

} ;
#endif // PSETTINGS_H

