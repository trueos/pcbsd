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

    //void pbiStateChanged(CAbstractUpdateController::EUpdateControllerState new_state);

    void globalStateChanged(CAbstractUpdateController::EUpdateControllerState new_state);

    //void pbiProgress(CAbstractUpdateController::SProgress progress);


private slots:
    //void on_updateSelectedPBIBtn_clicked();
    void on_updateAllButton_clicked();
    void on_pushButton_clicked();
};

#endif // MAINWINDOW_H
