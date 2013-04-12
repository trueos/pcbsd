#ifndef XGUI_H
#define XGUI_H

#include <QFile>
#include <QMainWindow>
#include <QMessageBox>
#include "ui_xgui.h"

class XGUI : public QMainWindow, private Ui::XGUI
{
        Q_OBJECT

public:
        XGUI() : QMainWindow()
        {
          setupUi(this);
        }

    void ProgramInit();

public slots:

private slots:
    void applySlot();
    void closeSlot();
    void dualChangedSlot();
    void monitorChangedSlot();

private:
    void checkUnprobedMonitor();
    QString getLineFromCommandOutput(QString command);
    bool isDriverAvailable(QString driver);
    void loadDepth();
    void loadDeviceInformation();
    void loadDrivers();
    void loadResolutions();
    void suggestDriver(QString driver);

signals:

} ;
#endif // XGUI_H

