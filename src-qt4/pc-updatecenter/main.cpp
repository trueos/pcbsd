#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include "pcbsd-ui.h"

int main(int argc, char *argv[])
{
    PCSingleApplication a(argc, argv);

    MainWindow w;

    QObject::connect(&a, SIGNAL(messageReceived(const QString &)), &w, SLOT(slotSingleInstance()));

    w.show();

    //Check for root
    if (0 != geteuid())
    {
        QMessageBox msg;
        msg.setText(w.tr("You should run this application as root"));
        msg.exec();
        exit(2);
    }
    
    return a.exec();
}
