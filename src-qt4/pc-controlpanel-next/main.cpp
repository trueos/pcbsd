#include "mainwindow.h"
#include <QApplication>

#include "backend/cp-itemgroup.h"

int main(int argc, char *argv[])
{        
    QApplication a(argc, argv);
    MainWindow w;

    CItemGroup system("/usr/local/share/pcbsd/pc-controlpanel/items/system", "system");
    system.read();

    w.show();
    
    return a.exec();
}
