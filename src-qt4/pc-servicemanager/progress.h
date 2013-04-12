#ifndef PROGRESS_H
#define PROGRESS_H

/* QT4 */
#include <qfile.h>
#include <qdialog.h>
#include <QProcess>

/* Local Includes */
#include "ui_progress.h"

class progressUI : public QDialog, private Ui::progressUI
{
        Q_OBJECT

public:
        progressUI() : QDialog()
        {
          setupUi(this);
        }

   void startServ(QString cmd, QStringList args);

public slots:

private slots:
    void slotDone();
    void slotReadProc();
    void slotProcFinished( int exit, QProcess::ExitStatus status);

private:
    QProcess *servProc;

signals:

} ;
#endif // {ROGRESS_H

