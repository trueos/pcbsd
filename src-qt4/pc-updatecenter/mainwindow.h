#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>

#include "syscontroller.h"
#include "pkgcontroller.h"
#include "pbicontroller.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;

    void init();

    CSysController  mSysController;
    CPkgController  mPkgController;
    CPBIController  mPBIController;

private slots:
    void sysStateChanged(CAbstractUpdateController::EUpdateControllerState new_state);
    void pkgStateChanged(CAbstractUpdateController::EUpdateControllerState new_state);
    void pbiStateChanged(CAbstractUpdateController::EUpdateControllerState new_state);

    void pbiProgress(CAbstractUpdateController::SProgress progress);
    void pkgProgress(CAbstractUpdateController::SProgress progress);

    void pkgConflict(QString conflictList);

private slots:
    void on_updateSelectedPBIBtn_clicked();
    void on_sysUpdatesList_itemChanged(QTreeWidgetItem *item, int column);
    void on_sysUpdatesList_itemActivated(QTreeWidgetItem *item, int column);
    void on_sysUpdatesList_itemSelectionChanged();
};

#endif // MAINWINDOW_H
