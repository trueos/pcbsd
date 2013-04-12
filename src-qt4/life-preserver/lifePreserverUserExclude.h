#ifndef PUEXCLUDE_H
#define PUEXCLUDE_H

/* QT4 */
#include <qfile.h>
#include <qdialog.h>

/* Local Includes */
#include "ui_lifePreserverUserExclude.h"
#include "preserver.h"

/* KDE */

class pUserExclude : public QDialog, private Ui::pUserExclude
{
        Q_OBJECT

public:
        pUserExclude() : QDialog()
        {
          setupUi(this);
        }

   void widgetInit(QString, bool);


public slots:

private slots:
    void slotOK();
    void slotCancel();
    void slotAddExclude();
    void slotRemoveExclude();
    void slotFindDirectory();

private:
    void savePreserver();
    bool excludeMode;
    PRESERVER *editPreserver;

signals:

} ;
#endif // PUEXCLUDE_H

