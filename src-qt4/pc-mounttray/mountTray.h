#ifndef MOUNT_TRAY_H
#define MOUNT_TRAY_H

// QT Includes
#include <QDialog>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QLocalSocket>
#include <QString>
#include <QPixmap>
#include <QList>
#include <QTimer>
#include <QDir>
#include <QFile>
#include <QTextStream>

#include "menuItem.h"
#include "devCheck.h"
#include "fsWatcher.h"
#include "fsDialog.h"
#include "settingsDialog.h"

extern bool DEBUG_MODE;
extern QString DEVICEDIR;
extern QString MOUNTDIR;

class MountTray : public QDialog {
   Q_OBJECT
   
public:
   MountTray() : QDialog()
   {

   }
   void programInit();
   
   ~MountTray();
   
public slots:
  void closeTray();
  
private slots:
  void slotSingleInstance();
  void newDevdMessage();
  void slotDevChanges(bool showPopup = TRUE);
  void slotTrayActivated(QSystemTrayIcon::ActivationReason);
  void slotOpenMediaDir();
  void openMediaDir(QString); 
  void slotDisplayPopup(QString,QString);
  void slotDisplayWarning(QString,QString);
  void removeDevice(QString);
  void slotRescan();
  void slotOpenFSDialog();
  void slotOpenSettings();
  
private:
  DevCheck *DCheck;
  QString USERNAME, FILEMAN;
  QLocalSocket* devdProc;
  QTimer *devdTimer;  
  int numMount, numAvail;
  QSystemTrayIcon* trayIcon;
  QMenu *trayIconMenu, *sysMenu;
  QList<MenuItem*> deviceList;
  QStringList oldsysdev;
  FSWatcher *diskWatcher;
  FSDialog *diskDisplay;
  //Saved Settings
  bool useDiskWatcher, useDiskTimerDevd;
  int diskTimerMaxMS; //milliseconds


  void updateMenu();
  void scanInitialDevices();
  int findDeviceInList(QString);
  bool addDevice(QString,QString,QString,QString);
  void startupDevdProc();
  void getInitialUsername();
  void getDefaultFileManager();
  void loadSavedSettings();
  void saveCurrentSettings();
  
};

#endif
