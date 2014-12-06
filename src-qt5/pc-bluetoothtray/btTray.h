// QT Includes
#include <qpair.h>
#include <QProcess>
#include <QDialog>
#include <QFileSystemWatcher>
#include <QSystemTrayIcon>
#include <QMenu>


class QString;
class QPixmap;

class BluetoothTray : public QDialog {
   Q_OBJECT
   
public:
   BluetoothTray() : QDialog()
   {

   }
   void programInit();
   
   virtual ~BluetoothTray() {};
   
private slots:
  void startBluetoothManager();
  void refreshTray();
  void restartBluetooth();
  void closeTray();
  void slotSingleInstance();
  void slotTrayActivated(QSystemTrayIcon::ActivationReason);
  
private:
  bool checkForBtDevices();
  bool isBTDevice, isFirstRun, starting;
  QSystemTrayIcon* trayIcon;
  QMenu* trayIconMenu;
  
};
