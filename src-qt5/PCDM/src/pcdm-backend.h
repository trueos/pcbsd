/* PCDM Login Manager:
*  Written by Ken Moore (ken@pcbsd.org) 2012/2013
*  Copyright(c) 2013 by the TrueOS Project
*  Available under the 3-clause BSD license
*/

#ifndef PCDM_BACKEND_H
#define PCDM_BACKEND_H

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileSystemWatcher>
#include <QProcess>
#include <QString>
#include <QStringList>
#include <QTextStream>

#include "pcdm-config.h"
#include "pcbsd-utils.h"

#define PCSYSINSTALL    QString("/usr/sbin/pc-sysinstall")
#define DBDIR QString("/var/db/pcdm/")

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
    static QString getNLDesktopName(QString);
    static QString getDesktopIcon(QString);
    static QString getDesktopBinary(QString);
    static void allowUidUnder1K(bool allow, QStringList excludes = QStringList() );
    static QStringList getSystemUsers(bool realnames = true);
    static QString getUsernameFromDisplayname(QString);
    static QString getDisplayNameFromUsername(QString);
    static QStringList keyModels();
    static QStringList keyLayouts();
    static QStringList keyVariants(const QString &layout, QStringList &savedKeyVariants);
    static bool changeKbMap(QString model, QString layout, QString variant);
    static QStringList languages();
    static void openLogFile(QString);
    static void log(QString); 
    static QString getUserHomeDir(QString);
    static QString getUserShell(QString);
    static void checkLocalDirs();

    //Auto-login usage
    static QString getALUsername();
    static QString getALDesktopCmd();
    static QString getALPassword();
    
    //Saved/Prior Settings
    static QString getLastUser();
    static QString getLastDE(QString);
    static void saveLoginInfo(QString, QString);
    static void readDefaultSysEnvironment(QString &lang, QString &keymodel, QString &keylayout, QString &keyvariant);
    static void saveDefaultSysEnvironment(QString lang, QString keymodel, QString keylayout, QString keyvariant);
    
    //Personacrypt usage
    static QStringList getRegisteredPersonaCryptUsers();
    static QStringList getAvailablePersonaCryptUsers();
    static bool MountPersonaCryptUser(QString user, QString pass);
    static bool UnmountPersonaCryptUser(QString user);
    
    static bool writeFile(QString fileName, QStringList contents);
    
private:	
    static void loadXSessionsData();
    static QStringList readXSessionsFile(QString, QString);
    static void readSystemUsers(bool directfile = false);
    static void readSystemLastLogin();
    static void writeSystemLastLogin(QString, QString);
    static QString readUserLastDesktop(QString);
    static void writeUserLastDesktop(QString, QString);
};


#endif // PCDM_BACKEND_H
