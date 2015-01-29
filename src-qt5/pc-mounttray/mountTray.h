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
#include <QFileDialog>

//libpcbsd includes
#include <pcbsd-xdgfile.h>
#include <pcbsd-xdgutils.h>

//#include "menuItem.h"
#include "devCheck.h"
#include "fsWatcher.h"
#include "fsDialog.h"
#include "settingsDialog.h"
#include "DeviceWidget.h"

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
  void slotDoneWithInit();
  void newDevdMessage();
  //void slotDevChanges(bool showPopup = true);
  void slotTrayActivated();
  //void slotOpenMediaDir();
  //void openMediaDir(QString); 
  void slotDisplayPopup(QString,QString, QString device = "");
  void slotDisplayWarning(QString,QString);
  void slotPopupClicked();
  //void removeDevice(QString);
  void slotRescan();
  void slotOpenFSDialog();
  void slotOpenSettings();
  void slotOpenISO();
  void slotCloseMenu();
  //void slotOpenAVDisk(QString dev);

  //New Functions
  void UpdateDeviceMenu(bool fast = false, bool refresh=false);
  
private:
  QList<DeviceWidget*> DEVLIST;	
  QAction *menuline;

  DevCheck *DCheck;
  //QString USERNAME;
  QLocalSocket* devdProc;
  QTimer *devdTimer;  
  int numMount, numAvail;
  QSystemTrayIcon* trayIcon;
  QMenu *trayIconMenu, *sysMenu;
  //QList<MenuItem*> deviceList;
  //QStringList oldsysdev;
  FSWatcher *diskWatcher;
  FSDialog *diskDisplay;
  //Saved Settings
  bool useDiskWatcher, useDiskTimerDevd;
  int diskTimerMaxMS; //milliseconds
  QString popupSave; //for saving the device for the latest pop-up message
  bool MTINIT;

  //CMD to use for opening the file manager
  //QString FMCMD;

  //void updateMenu();
  //void scanInitialDevices();
  //int findDeviceInList(QString);
  //bool addDevice(QString,QString,QString,QString);
  void startupDevdProc();
  //void getInitialUsername();
  //void getFileManager();
  void loadSavedSettings();
  void saveCurrentSettings();
  
};

#endif
