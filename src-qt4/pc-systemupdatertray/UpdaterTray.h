#include <QApplication>
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QImage>
#include <QMenu>
#include <QMessageBox>
#include <QProcess>
#include <QSettings>
#include <QTimer>
#include <QToolTip>
#include <QTranslator>
#include <QTextStream>
#include <QAction>
#include <QDialog>
#include <QFileSystemWatcher>
#include <QPair>
#include <QString>
#include <QSystemTrayIcon>

#include <unistd.h>
#include <sys/types.h>

/* libPCBSD includes */
#include <pcbsd-sysFlags.h>

//Local includes
#include "../config.h"

#define SYSTRIGGER "/tmp/.sysupdatetraytrigger"

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
   void slotQuitTray();
   void slotSaveUpdaterPrefs();
   void slotScheduledSystemCheck();
   void slotSetTimerReadAutoStatus();
   void slotStartUpdateCheck();
   void slotSysUpdateTimer();
   void slotTrayActivated(QSystemTrayIcon::ActivationReason reason);
   void slotShowSysUpdatePopup();
   void slotShowJailUpdatePopup();
   void slotChangePopup();
   void slotShowPkgUpdatePopup();
   
protected:
   
private:
   void displayTooltip();
   QString getLineFromCommandOutput( QString command );
   void loadUpdaterPrefs();
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
   SystemFlagWatcher *watcher;
};
