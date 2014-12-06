// QT Includes
#include <qpair.h>
#include <QProcess>
#include <QDialog>
#include <QFileSystemWatcher>
#include <QSystemTrayIcon>
#include <QMenu>

#include "netKey.h"

#define PROGPATH QString("/usr/local/share/pcbsd/pc-netmanager")
#define IFCONFIG QString("/sbin/ifconfig")

class QString;
class QPixmap;

class NetworkTray : public QDialog {
   Q_OBJECT
   
public:
   NetworkTray() : QDialog()
   {

   }
   void programInit(QString);
   
   virtual ~NetworkTray() {};
   
private slots:
   void openNetManager();
   void openDeviceInfo();
   void openConfigDlg();
   void monitorStatus(bool noloop=FALSE);
   void slotRestartNetwork();
   void slotTrayActivated(QSystemTrayIcon::ActivationReason);
   void slotQuit();
   void slotTriggerFileChanged();
   void slotGetNetKey(QAction* act);
   void slotQuickConnect(QString key, QString SSID, bool hexkey);
   void slotCheckWifiAvailability();
   void slotUpdateIfStatus();
   
private:
   void displayTooltip();
   void confirmDevice( QString device );
   void loadIcons();
   void updateWifiNetworks();   
   QFileSystemWatcher *fileWatcherClosed;
   QString getLineFromCommandOutput( QString command );
   QString getNameForIdent( QString ident );
   QString getIpForIdent();
   QString getMacForIdent( QString ident );
   QString getSSIDForIdent();
   QString getNetmaskForIdent();
   QString getSignalStrengthForIdent( QString ident );
   QString getWirelessSpeedForIdent( QString ident );
   QString getUpStatusForIdent();
   QString getStatusForIdent();
   QString getMediaForIdent();
   QString getIPv6ForIdent();
   QString getWifiParent( QString dev );
   QString getTypeForIdent( QString ident );
   QString ifconfigOutput;
   QProcess *runCommandProc;
   QProcess *getIfProc;
   QIcon iconWiredConnect;
   QIcon iconWiredDisconnect;
   QIcon iconWifiConnect;
   QIcon iconWifiDisconnect;
   QIcon iconWifiConnect30;
   QIcon iconWifiConnect60;
   QIcon iconWifiConnect85;
   QIcon iconLocked;
   QIcon iconUnlocked;
   QSystemTrayIcon *trayIcon;
   QMenu *trayIconMenu;
   QActionGroup *trayActionGroup;
   bool usingLagg;
   
   netKey *dialogNetKey;
};
