#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include "pcbsd-ui.h"
#include "jailsbackend.h"

#include <unistd.h>
#include <sys/types.h>

bool checkUserGroup(QString groupName)
{
   QString loginName = getlogin();
   QStringList gNames;
   if ( loginName == "root" )
     return true;

   QString tmp;
   QFile iFile("/etc/group");
   if ( ! iFile.open(QIODevice::ReadOnly | QIODevice::Text))
     return true; //or FALSE?

   while ( !iFile.atEnd() ) {
     tmp = iFile.readLine().simplified();
     if ( tmp.indexOf(groupName) == 0 ) {
    gNames = tmp.section(":", 3, 3).split(",");
    break;
     }
   }
   iFile.close();

   for ( int i = 0; i < gNames.size(); ++i )
      if ( gNames.at(i).indexOf(loginName) == 0 )
            return true;

   return false;
}

int main(int argc, char *argv[])
{        
    PCSingleApplication a(argc, argv);

    CJailsBackend jail;
    CJailsBackend* pJail = NULL;

    bool is_warden_found= false;
    QString jail_name;
    for (int i=1; i<argc; i++)
    {
        QString arg = argv[i];
        if (arg.trimmed() == "-jail")
        {
            is_warden_found= true;
            continue;
        }
        if (is_warden_found)
        {
            jail_name= arg;
            break;
        }
    }//for all args
    if (jail_name.length())
    {        
        jail.setJail(jail_name);
        jail.setJailEnabled(true);        
        pJail = &jail;
    }

    if (a.isRunning() && (!jail.jailEnabled()))
    {
        return !(a.sendMessage("show"));
    }

    MainWindow w(pJail);

    w.show();

    QObject::connect(&a, SIGNAL(messageReceived(const QString &)), &w, SLOT(slotSingleInstance()));

    //Check for root

    bool isNotPriv = (0 != geteuid()) && jail.jailEnabled();
    isNotPriv |= !(checkUserGroup("wheel") || checkUserGroup("operator"));

    if (isNotPriv)
    {
        QMessageBox msg;
        msg.setText(w.tr("You should run this application as root"));
        msg.exec();
        exit(2);
    }    
    
    return a.exec();
}
