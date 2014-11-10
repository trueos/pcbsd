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
    
private:
    Ui::MainWindow *ui;
    pcbsd::keyboard::KeyboardSettings ks;
    QStringList allKbModels;

    void fillKbModels();
};

#endif // MAINWINDOW_H
