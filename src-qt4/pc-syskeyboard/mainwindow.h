#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "keyboardsettings.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_advancedViewBtn_clicked();

    void on_simpleViewBtn_clicked();

    void on_applyBtn_clicked();

    void on_saveUserBtn_clicked();

private:
    Ui::MainWindow *ui;
    pcbsd::keyboard::KeyboardSettings ks;
    QStringList allKbModels;

    void fillKbModels();
    void updateSettings();
};

#endif // MAINWINDOW_H
