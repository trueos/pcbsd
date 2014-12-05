#ifndef MAINWIN_H
#define MAINWIN_H

#include "ui_mainwindow.h"
#include <QProcess>
#include <QSettings>
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
    void slotProcDone();
    void slotPrintStdErr();
    void slotPrintStdOut();
    void slotPwdTextChanged ( const QString & text );
    void slotExpandCommandClicked(bool isChecked);


private:
    void startSudo();
    void testPass();
    bool checkUserGroup();
    QProcess *sudoProc;
    int tries;
    QSettings *settings;

   QStringList runQuickCmd(QString cmd);

signals:

} ;
#endif // MAINWIN_H

