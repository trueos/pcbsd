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

public slots:
    void pkgStateChanged(CAbstractUpdateController::EUpdateControllerState new_state);
    void pbiStateChanged(CAbstractUpdateController::EUpdateControllerState new_state);
};

#endif // MAINWINDOW_H
