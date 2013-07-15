#ifndef MAINWIN_H
#define MAINWIN_H

#include "ui_mainwindow.h"
#include "../config.h"

class MainWindow : public QMainWindow, private Ui::MainWindow
{
        Q_OBJECT

public:
        MainWindow() : QMainWindow()
        {
          setupUi(this);
        }

public slots:
    void ProgramInit();

private slots:
    void slotButtonClicked(QAbstractButton *myBut);
    void slotReturnPressed();

private:

signals:

} ;
#endif // MAINWIN_H

