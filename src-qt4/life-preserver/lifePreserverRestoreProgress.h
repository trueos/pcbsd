#ifndef LIFERESTOREPROGRESS_H
#define LIFERESTOREPROGRESS_H

/* QT4 */
#include <qfile.h>
#include <qdialog.h>
#include <QProcess>

/* Local Includes */
#include "ui_lifePreserverRestoreProgress.h"
#include "preserver.h"

/* KDE */

class lifeRestoreProgress : public QDialog, private Ui::lifeRestoreProgress
{
        Q_OBJECT

public:
        lifeRestoreProgress() : QDialog()
        {
          setupUi(this);
        }

   void widgetInit(QString preserver, QString targetBackup, QString restoreFileListString);
   void setRelativeRestore(QString relativeDirString, bool keepPath);
   void startRestore();

public slots:

private slots:
    void slotDone();
    void slotReadProc();
    void slotProcFinished( int exit, QProcess::ExitStatus status);

private:
    PRESERVER *restorePreserver;
    QString workingBackup;
    QString restoreList;
    QString relativeDir;
    bool relKeepPath;
    QProcess *restoreProc;

signals:

} ;
#endif // LIFERESTOREPROGRESS_H

