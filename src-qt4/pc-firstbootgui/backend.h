#ifndef BACKEND_H
#define BACKEND_H

#include <QStringListIterator>
#include <QStringList>
#include <QString>
#include <QList>
#include <QDebug>
#include <QProcess>
#include <QByteArray>
#include <QWidget>

#define PCSYSINSTALL    QString("/usr/local/sbin/pc-sysinstall")
#define PCSYSINSTALLDIR QString("/usr/local/share/pc-sysinstall")
#define PCSYSINSTALLCFG QString("/tmp/sys-install.cfg")
#define TMPLANGFILE QString("/tmp/.SysInstallLang")

// Set the minimum sizes for FBSD
#define FBSD_MINSIZE 2500
#define FBSD_MINROOT 1000
#define FBSD_MINVAR 500
#define FBSD_MINUSR 1000

// Set the minimum sizes for PCBSD
#define PCBSD_MINSIZE 3000
#define PCBSD_MINROOT 1000
#define PCBSD_MINVAR 500
#define PCBSD_MINUSR 1500


namespace Scripts {

class Process : public QProcess {
public:
    Process(const QStringList &args) {
        setReadChannel(QProcess::StandardOutput);
        start(PCSYSINSTALL, args);
    }
};

class Backend {
public:
    static void enableNic(QString Nic, QString IP, QString NetMask, QString DNS, QString Gate, bool fetchMirrors, QString IPv6="", QString IPv6Gate="", QString IPv6DNS="");
    static void createErrorReport();
    static void setupSSHKeys(QString Host, QString User, QString Port);
    static QStringList languages();
    static QStringList keyLayouts();
    static QStringList keyModels();
    static QStringList keyVariants(const QString &, QStringList &savedKeyVariants);
    static QStringList bootableMedia();
    static QStringList networkDevices();
    static QStringList timezones();
    static QString guessTimezone();
    static QString detectCountryCode();
    static void changeKbMap(QString model, QString layout, QString variant);
    static QList<QStringList> hardDrives();
    static QList<QStringList> availComponents();
    static int systemMemory();
    static int deleteMBRPart(QString Slice, QString &output);
    static int addMBRPart(QString Disk, int size, QString &output);
    static QList<QStringList> getPackageData(bool &found);
    //static QList<QStringList> slices();

};


} //namespace Scripts


#endif // BACKEND_H
