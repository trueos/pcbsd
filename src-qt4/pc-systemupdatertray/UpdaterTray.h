#include <QAction>
#include <QDialog>
#include <QFileSystemWatcher>
#include <QFile>
#include <QMenu>
#include <QPair>
#include <QProcess>
#include <QString>
#include <QSystemTrayIcon>

#define SYSTRIGGER "/tmp/.sysupdatetraytrigger"
#define PBITRIGGER "/tmp/.pbiupdatetraytrigger"

class QString;
class QPixmap;

class UpdaterTray : public QDialog {
   Q_OBJECT
   
public:
   UpdaterTray() : QDialog()
   {
   }
   void programInit();
   
   virtual ~UpdaterTray() {};

public slots:
    void slotSingleInstance();
   
private slots:
   void contextMenuRefresh();
   void slotAutoUpdateStatusChanged();
   void slotChangeRunStartup();
   void slotCheckAllUpdates();
   void slotMonitorForChanges();
   void slotOpenConfigDlg();
   void slotOpenJailManager();
   void slotOpenUpdateManager();
   void slotOpenPackageManager();
   void slotOpenSoftwareManager();
   void slotOpenSoftwareManagerInstalled();
   void slotPBICheckUpdate();
   void slotPopulatePBIList();
   void slotQuitTray();
   void slotReadPBIBuffer();
   void slotSaveUpdaterPrefs();
   void slotScheduledPBICheck();
   void slotScheduledSystemCheck();
   void slotSetTimerReadAutoStatus();
   void slotStartPBIUpdateCheck();
   void slotStartUpdateCheck();
   void slotSysUpdateTimer();
   void slotTrayActivated(QSystemTrayIcon::ActivationReason reason);
   void slotShowSysUpdatePopup();
   void slotShowJailUpdatePopup();
   void slotShowPBIUpdatePopup();
   void slotChangePopup();
   void slotShowPkgUpdatePopup();
   
protected:
   
private:
   void displayTooltip();
   QString getLineFromCommandOutput( QString command );
   void loadUpdaterPrefs();
   QProcess *checkPBIProc;
   QFileSystemWatcher *fileWatcher;
   QFileSystemWatcher *fileWatcherSys;
   QFileSystemWatcher *fileWatcherAutoUpdate;
   QFileSystemWatcher *pbiWatcherAutoUpdate;
   QString sysPatchsetTmpFile;
   bool useCustomTmpDir;
   QString customTmpDir;
   QString patchTmpDir;
   QAction *runAction;
   QAction *popAction;
   int autoStatus;
   QString autoCurrentUpdate;
   QStringList autoUpdatesFailed;
   QStringList autoUpdatesInstalled;
   QSystemTrayIcon *trayIcon;
   QMenu *trayIconMenu;
   bool shownPopup;
   bool doingCheck;
};
