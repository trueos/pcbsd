#ifndef MAINWIN_H
#define MAINWIN_H

#include <QDialog>
#include <QFile>
#include <QMenu>
#include <QMessageBox>
#include <QProcess>
#include <QProgressDialog>
#include <QSocketNotifier>
#include <QStringList>
#include <QTextStream>
#include <QTimer>

#include <pcbsd-utils.h>
#include "ui_mainWin.h"
#include "../config.h"

/* Change this to switch the default patch tmpdir */
const QString PATCHTMPDIR_DEFAULT( PREFIX + "/tmp" );
#define SYSTRIGGER "/tmp/.sysupdatetraytrigger"

class mainWin : public QDialog, private Ui::mainWin
{
        Q_OBJECT

public:
        mainWin() : QDialog()
        {
          setupUi(this);
        }

    QMap <QString, QString> codeMap;

public slots:
    void ProgramInit(QString, QString);
    void slotSingleInstance();

private slots:
    void slotDisplayUpdates();
    void slotInstallClicked();
    void slotRescanUpdates();
    void slotReadUpdateData();
    void slotSelectAllClicked();
    void slotUpdateLoop();
    void slotReadUpdateOutput();
    void slotUpdateFinished();
    void slotListClicked();
    void slotListDoubleClicked(QListWidgetItem *);
    void slotCloseClicked();

    // Package updates
    void slotRescanPkgsClicked();
    void slotUpdatePkgsClicked();
    void slotReadPkgUpdateOutput();
    void slotUpdatePkgDone();
    void slotReadEventPipe(int fd);

private:
    void doUpdates();
    bool sanityCheck();
    void checkFBSDUpdates();
    void checkMPKGUpdates();
    void checkPCUpdates();
    int curUpdate;
    int curUpdateIndex;
    int totUpdate;
    bool dPackages;
    bool uPackages;
    bool doingUpdate;
    QProcess *uProc;
    QList<QStringList> listUpdates;
    QString wDir, wIP;
    QString lastError;
    QSocketNotifier *eSock;

signals:

} ;
#endif // MAINWIN_H

