#include "fstabmanager.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FstabManager w;
    w.show();
    
    return a.exec();
}
