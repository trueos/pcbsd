#ifndef CRASHHANDLER_H
#define CRASHHANDLER_H

#include <qfile.h>
#include <qmessagebox.h>
#include <qdialog.h>
#include "ui_crashhandler.h"
#include "details.h"

class CrashHandler : public QDialog, private Ui::CrashHandler
{
        Q_OBJECT

public:
        CrashHandler() : QDialog()
        {
          setupUi(this);
        }

    void setup( QString name, QString stdout, QString stderr );

public slots:

private slots:
    void showDetails();

private:
    QString stderrTxt;
    QString stdoutTxt;
    Details details;


signals:

} ;
#endif // CRASHHANDLER_H

