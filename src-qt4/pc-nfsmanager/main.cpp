#include "nfsmanager.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    NFSManager w;
    w.show();

    return a.exec();
}
