#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void pkgStateChanged(CAbstractUpdateController::EUpdateControllerState new_state);
    void pbiStateChanged(CAbstractUpdateController::EUpdateControllerState new_state);

    void pbiProgress(CAbstractUpdateController::SProgress progress);
    void pkgProgress(CAbstractUpdateController::SProgress progress);

    void pkgConflict(QString conflictList);

private slots:
    void on_updateSelectedPBIBtn_clicked();
};

#endif // MAINWINDOW_H
