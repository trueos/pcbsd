#ifndef MAINWIN_H
#define MAINWIN_H

#include <QDialog>
#include <QMainWindow>
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

class mainWin : public QMainWindow, private Ui::mainWin
{
        Q_OBJECT

public:
        mainWin() : QMainWindow()
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

    void on_actionExit_triggered();

    void on_actionPatchset_triggered();

    void on_actionUpdate_descriptions_triggered();

private:
    void doUpdates();
    bool sanityCheck();
    void checkFBSDUpdates();
    void checkPCUpdates();
    int curUpdate;
    int curUpdateIndex;
    int totUpdate;
    bool doingUpdate;
    QProcess *uProc;
    QList<QStringList> listUpdates;
    QString wDir, wIP;
    QString lastError;

signals:

} ;
#endif // MAINWIN_H

