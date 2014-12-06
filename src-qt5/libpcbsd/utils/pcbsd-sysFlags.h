#ifndef _PCBSD_SYSTEM_FLAGS_H
#define _PCBSD_SYSTEM_FLAGS_H

#include <QObject>
#include <QFileSystemWatcher>
#include <QString>
#include <QDateTime>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QTimer>
#include <QTextStream>

#include <unistd.h>

#include "pcbsd-utils.h"

// DEFINE THE FLAG FILES
#define FLAGDIR QString("/tmp/.pcbsdflags")
#define NETWORKRESTARTED QString("network-restarted")
#define PKGUPDATEAVAILABLE QString("pkg-update-available")
#define SYSUPDATEAVAILABLE QString("sys-update-available")
#define WARDENUPDATEAVAILABLE QString("warden-update-available")

// DEFINE THE SIMPLE MESSAGES
#define MWORKING QString("working")
#define MERROR QString("error")
#define MSUCCESS QString("success")
#define MUPDATE QString("updateavailable")
#define MUPDATING QString("updating")

class SystemFlags{
public:
	enum SYSFLAG{ NetRestart, PkgUpdate, SysUpdate, WardenUpdate};
	enum SYSMESSAGE{ Working, Error, Success, UpdateAvailable, Updating};
};

class SystemFlagWatcher : public QObject{
	Q_OBJECT
public:
	SystemFlagWatcher(QObject *parent = 0);
	~SystemFlagWatcher();

	void checkForRecent(int minutes);

public slots:
	void checkFlags(); //force re-checking for flag updates at the current time

private:
	QDateTime CDT;  //current date time
	QFileSystemWatcher *watcher;
	QTimer *chktime;

private slots:
	void watchFlagDir();
	void watcherNotification();
	QString quickRead(QString filepath);

signals:
	void FlagChanged(SystemFlags::SYSFLAG, SystemFlags::SYSMESSAGE);
};

#endif
