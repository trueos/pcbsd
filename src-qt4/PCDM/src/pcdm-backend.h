/* PCDM Login Manager:
*  Written by Ken Moore (ken@pcbsd.org) 2012/2013
*  Copyright(c) 2013 by the PC-BSD Project
*  Available under the 3-clause BSD license
*/

#ifndef PCDM_BACKEND_H
#define PCDM_BACKEND_H

#include <QStringList>
#include <QString>
#include <QDebug>
#include <QDir>
#include <QProcess>

#include "pcdm-config.h"
#include "pcbsd-utils.h"

#define PCSYSINSTALL    QString("/usr/sbin/pc-sysinstall")

class Process : public QProcess {
public:
    Process(const QStringList &args) {
        setReadChannel(QProcess::StandardOutput);
        start(PCSYSINSTALL, args);
    }
};

class Backend {
public:
    static QStringList getAvailableDesktops();
    static QString getDesktopComment(QString);
    static QString getDesktopIcon(QString);
    static QString getDesktopBinary(QString);
    static QStringList getSystemUsers();
    static QString getUsernameFromDisplayname(QString);
    static QStringList keyModels();
    static QStringList keyLayouts();
    static QStringList keyVariants(const QString &layout, QStringList &savedKeyVariants);
    static void changeKbMap(QString model, QString layout, QString variant);
    static QStringList languages();
    static void openLogFile(QString);
    static void log(QString); 
    static QString getUserHomeDir(QString);
    static void checkLocalDirs();

    static QString getALUsername();
    static QString getALDesktopCmd();
    static QString getALPassword();
    
private:	
    static void loadXSessionsData();
    static QStringList readXSessionsFile(QString, QString);
    static void readSystemUsers();
 
    
};


#endif // PCDM_BACKEND_H
