//#include <QtGui/QApplication>
#include "pcbsd-ui.h"
#include "mainwindow.h"


int main(int argc, char *argv[])
{
    PCApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}
