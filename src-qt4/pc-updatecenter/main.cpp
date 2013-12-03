#include "mainwindow.h"
#include <QApplication>
#include "pcbsd-ui.h"

int main(int argc, char *argv[])
{
    PCSingleApplication a(argc, argv);

    MainWindow w;

    QObject::connect(&a, SIGNAL(messageReceived(const QString &)), &w, SLOT(slotSingleInstance()));

    w.show();
    
    return a.exec();
}
