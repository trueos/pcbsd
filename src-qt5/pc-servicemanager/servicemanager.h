#ifndef SERVICEMANAGER_H
#define SERVICEMANAGER_H

#include <qfile.h>
#include <qmessagebox.h>
#include <qdialog.h>
#include "ui_servicemanager.h"
#include "ui_progress.h"
#include "progress.h"

class ServiceManager : public QDialog, private Ui::ServiceManager
{
        Q_OBJECT

public:
        ServiceManager() : QDialog()
        {
          setupUi(this);
        }


public slots:
    void ProgramInit(QString, QString);

private slots:
    void startSlot();
    void stopSlot();
    void restartSlot();
    void enableSlot();
    void disableSlot();
    void checkRunningFinishedSlot();
    void listSelectionChanged();
    void checkRunning();

private:
    QString wDir, wIP;
    void setButtonsAllEnabled(bool enabled);
    QProcess *ServiceEnable;
    QProcess *ServiceDisable;
    QProcess *CheckServiceRunning;
    QProcess *CheckServiceEnabled;
    void populateList();
    void checkEnabled();
    QTreeWidgetItem *workingTreeWidgetItem;
    QTreeWidgetItemIterator *currentCheckRunningItem;
    QTreeWidgetItemIterator *currentCheckEnabledItem;
    progressUI *servAction;

signals:

} ;
#endif // SERVICEMANAGER_H

