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

#include "menuItem.h"
#include "devCheck.h"

extern bool DEBUG_MODE;
extern QString DEVICEDIR;
extern QString MOUNTDIR;

//class QString;
//class QPixmap;

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
  void removeDevice(QString);
  void slotRescan();
  
private:
  DevCheck *DCheck;
  QString USERNAME, FILEMAN;
  QLocalSocket* devdProc;
  QTimer *devdTimer;  
  int numMount, numAvail;
  QSystemTrayIcon* trayIcon;
  QMenu* trayIconMenu;
  QList<MenuItem*> deviceList;
  QStringList oldsysdev;


  void updateMenu();
  void scanInitialDevices();
  int findDeviceInList(QString);
  void addDevice(QString,QString,QString,QString);
  void startupDevdProc();
  void getInitialUsername();
  void getDefaultFileManager();
  
};

#endif
