#ifndef BTMAINGUI_H
#define BTMAINGUI_H

#include <QMainWindow>
#include <pcbsd-hardware.h>
#include <pcbsd-utils.h>

namespace Ui {
    class btmaingui;
}

class btmaingui : public QMainWindow
{
    Q_OBJECT

public:
    explicit btmaingui(QWidget *parent = 0);
    ~btmaingui();
    void firstRun();
    
private:
    Ui::btmaingui *ui;
    bool rootPermissions();
    void refreshGUI();  
    
private slots:
    void scanForDevices();
    void tabChanged(int);
    void refreshGUIslot(int);
    void refreshOldDeviceList();
    void removeOldDevice();
    void addNewDevice();
    void updateCompInfo();
    void updateNewDeviceInfo();
    void updateOldDeviceInfo(int);
    void configureOldDevice();
    void disconnectDevice();
    void connectDevice();
    void restartBluetooth();
    void changeCompName();
    void slotSingleInstance();
    void startTrayApplication();
};

#endif // BTMAINGUI_H
