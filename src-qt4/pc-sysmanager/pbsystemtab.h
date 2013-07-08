#ifndef PBSYSTEMTAB_H
#define PBSYSTEMTAB_H

#include <QDialog>
#include <QFile>
#include <QMenu>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QProcess>
#include <QProgressDialog>
#include <QTextStream>
#include <QTimer>

#include <pcbsd-utils.h>
#include <pcbsd-ui.h>
#include "updaterDialog.h"
#include "ui_pbsystemtab.h"
#include "portsnapprogress.h"
#include "../config.h"

/* Change this to switch the default patch tmpdir */
const QString PATCHTMPDIR_DEFAULT( PREFIX + "/tmp" );
#define SYSTRIGGER "/tmp/.sysupdatetraytrigger"

/* The Update STATUS Flags */
#define SYSTEM_UP2DATE 0
#define SYSTEM_UPDATE_AVAIL 1
#define SYSTEM_CHECKING4UPDATES 4
#define SYSTEM_UPDATING 5
#define CHECK_FAILED 6

// Set our defines for the type of mirror the user is running
#define AUTOMIRROR      0
#define SELECTMIRROR    1
#define CUSTOMMIRROR    2



class PBSystemTab : public QDialog, private Ui::PBSystemTab
{
        Q_OBJECT

public:
        PBSystemTab() : QDialog()
        {
          setupUi(this);
        }

    QMap <QString, QString> codeMap;
    bool checkValue( QString File, QString Key, QString Value );

public slots:
    void ProgramInit();
    void saveKernScreen();
    void miscSavePressed();
    void changeOpenTab(int tab);

private slots:
    void ReadUname();
    void startGenerateSheet();
    void finishedSheet();
    void fetchSourcePressed();
    void fetchPortsPressed();
    void customSplashPressed();
    void showSplashChecked( int newState );
    void loadBootData();
    void slotMiscSave();
    void slotClose();
    
private:
    QString KernDescr[50];
    QString KernDefaultTag;
    QString KernTag[50];
    QString KernFileName[50];
    QProcess *ViewSheet;
    QProcess *SheetGenScript;
    QString SheetFileName;
    QProcess *GetPBVer;
    QProcess *GetUname;
    PortsnapProgress *portsnapUI;
    QString username;
    bool miscChanged;
    bool kernelChanged;
    void CheckUname();
    void CheckPBVer();
    void CreateSheetFile();
    void showRebootRequired();
    void LoadKernSettings();
    QString getLineFromCommandOutput(QString cmd);
    bool sanityCheckSettings();

    QString mirrorNames[900];
    QString mirrorURLs[900];
    QString mirrorJobBuf;
    QNetworkAccessManager *mirrorJob;
    QNetworkReply *mirrorReply;
    void checkProxy();

    updaterStatus *UpdaterStatusDialog;
    
    QMenu *popup;
    QNetworkAccessManager *sysFetchJob;
    QNetworkReply *sysFetchReply;
    QNetworkAccessManager *sysCopyJob;
    QNetworkReply *sysCopyReply;

    int mirrorType;
    QString mirrorURL;
    QString customTmpDir;
    QString Version;
    QString Arch;
    QString Lang;
    int programstatus;
    int currentSysWorkingItem;
    QStringList workingSysUpdates;
    int workingPopupItem;
    long totalSeconds;
    long downloadSize;
    int totalSteps;
    int attemptedRedownload;
    int firstInstall;
    int firstDownload;
    int requiresSysReboot;
    QString sysPatchsetTmpFile;
    QProcess *getUpdatesDir;
    QProcess *readSysUpdates;
    QProcess *listingProc;
    QProcess *checksumProc;
    QProcess *extractProc;
    QProcess *installProc;
    QProcess *getFreeSpaceProc;
    QString SysUpdateName[500];
    QString SysUpdateIgnore[500];
    QString SysUpdatePatchFile[500];
    QString SysUpdateDate[500];
    QString SysUpdateMD5[500];
    QString SysUpdateSize[500];
    QString SysUpdateURL[500];
    QString SysUpdateAlone[500];
    QString SysUpdateReboot[500];
    QString SysUpdateDetailsURL[500];
    QString SysUpdatesShown[501];
    metaWidget *pkgWidget;


signals:
  void changed();

} ;
#endif // PBSYSTEMTAB_H

