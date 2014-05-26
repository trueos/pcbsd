#include "mainwindow.h"
#include <QApplication>

#include "pcbsd-ui.h"

#include "backend/cp-itemgroup.h"

int main(int argc, char *argv[])
{        
    PCSingleApplication a(argc, argv);

    if ( a.isRunning() )
              return !(a.sendMessage("show"));

    MainWindow w;       

    w.show();

    QObject::connect(&a, SIGNAL(messageReceived(const QString&)), &w, SLOT(slotSingleInstance()) );
    
    return a.exec();
}
