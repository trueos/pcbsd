#ifndef LIFEPW_H
#define LIFEPW_H

/* QT4 */
#include <qfile.h>
#include <qdialog.h>
#include <QProcess>

/* Local Includes */
#include "ui_lifePreserverWizard.h"

/* KDE */

class lifePreserverWizard : public QWizard, private Ui::lifePreserverWizard
{
        Q_OBJECT

public:
        lifePreserverWizard() : QWizard()
        {
          setupUi(this);
        }

   void widgetInit();


public slots:

private slots:
   virtual int nextId() const;
   virtual void accept();
   virtual void slotCheckComplete();
   void slotSSHProcFinished ( int exitCode, QProcess::ExitStatus exitStatus );

private:
    void savePreserver();
    bool doesPreserverExist();
    enum { Page_Intro, Page_RemoteDev, Page_Schedule, Page_Finished };
    bool validatePage();
    QProcess *sshAuthProc;

signals:
    void added();
} ;
#endif // LIFEPW_H

