
#ifndef DIALOGWARDEN_H
#define DIALOGWARDEN_H

#define WARDENDIR QString("/usr/local/share/warden")

#include "ui_dialogwarden.h"
#include "dialogTemplates.h"
#include "dialogimport.h"
#include "dialogEditIP.h"
#include "wizardNewJail.h"
#include <pcbsd-utils.h>
#include <pcbsd-ui.h>
#include <QMenu>

class dialogWarden : public QMainWindow, private Ui::dialogWarden
{
	Q_OBJECT

public:
        dialogWarden() : QMainWindow()
        {
          setupUi(this);
        }

    void programInit();

public slots:
    void slotSingleInstance();

private slots:
    void helpAbout();
    void refreshJails();
    void readConfig();
    void slotOpenConfig();
    void slotExit();

    void slotCheckJailStatus();
    void slotMonitorJailStatus();
    void slotCheckStatusReturn();

    void slotCheckJailDetails();
    void slotMonitorJailDetails();
    void slotCheckDetailsReturn();

    void slotJailRightClicked();
    void slotStopJail();
    void slotStartJail();
    void slotExportJail();
    void slotListJailPkgs();
    void slotDeleteJail();
    void slotFinishedWorking();
    void slotFinishedOutput();
    void slotReadPkgsOutput();
    void slotClickedNewJail();
    void slotCreateNewJail( const QString &, const QString &, const QString &, bool, bool, const QString &, bool, bool, bool, bool, const QString &, const QString &);
    void slotReadCreateOutput();
    void slotFinishedJailCreate();
    void slotReadUserAddOutput();
    void slotFinishedUserAdd();
    void slotImportJailClicked();
    void slotReadImportOutput();
    void slotFinishedJailImport();
    void slotToggleAutostartClicked();
    void slotImportConfigDone(const QString &JailName, const QString &IP, const QString &Host);
    void slotCurrentJailChanged();
    void slotTerminal();
    void slotUserAdmin();
    void slotPushEditIP();
    void slotPushPackage();
    void slotUpdate();
    void slotShowDialogCloseButton();
    void slotServiceGUI();
    void slotReadExportOutput();
    void slotFinishedExport();

    void slotCheckForUpdates();
    void slotCheckUpdateReturn();
    void slotCheckNic();

    // Snapshot support
    void slotSnapSliderChanged(int newVal);
    void slotLoadSnapshots();
    void slotCreateSnap();
    void slotRestoreSnap();
    void slotRemoveSnap();
    void slotMakeClone();
    void slotCronSnapshotChanged();

    void slotTemplateOpen();

private:
    void refreshJailDetailsView();
    void runCommand( QString command );
    bool hasClone(QString);
    QString getSnapDateReadable(QString time);
    QString ProgDir;
    QString WorldSrc;
    QString WorldDir;
    QString NIC;
    QString TMPDIR;
    QString JailDir;
    dialogConfig *configDialog;
    dialogTemplates *tempDialog;
    QString currentStatusWorkingJail;
    QString currentDetailsWorkingJail;
    QString currentUpdateWorkingJail;
    bool checkingStatus;
    bool checkingUpdates;
    bool checkingDetails;
    QTimer *statusTimer;
    QTimer *detailsTimer;
    QProcess *statusProc;
    QProcess *updateProc;
    QProcess *detailsProc;
    QString popupip;
    QMenu *popup;
    dialogImport *importDialog;
    dialogWorking *workingDialog;
    QProcess *getDetailsProc;
    QProcess *stopJailProc;
    QProcess *startJailProc;
    QProcess *exportJailProc;
    dialogDisplayOutput *dialogOutput;
    QProcess *listPackagesProc;
    QProcess *deleteJailProc;
    wizardNewJail *newJailWizard;
    QProcess *createJailProc;
    QString newHost;
    bool newSrc;
    bool newPorts;
    bool newAutostart;
    QString newRootPW;
    QString newUserPW;
    QString newUser;
    QProcess *userAddProc;
    QString importFile;
    QProcess *importJailProc;
    QProcess *toggleAutoProc;
    metaWidget *pkgWidget;
    QList<QStringList> jailDetails;
    QStringList snapshotList;
    QStringList snapshotComments;
    QStringList cloneList;
    dialogEditIP *dIP;
    int currentSnapshot;

signals:
} ;
#endif // DIALOGWARDEN_H
