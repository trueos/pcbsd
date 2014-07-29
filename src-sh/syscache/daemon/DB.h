#ifndef _SYSCACHE_DATABASE_CLASS_H
#define _SYSCACHE_DATABASE_CLASS_H

#include <QObject>
#include <QHash>
#include <QString>
#include <QStringList>
#include <QProcess>
#include <QStringList>
#include <QThread>
#include <QCoreApplication>
#include <QTimer>
#include <QDebug>
#include <QFileSystemWatcher>
#include <QFileInfo>
#include <QDateTime>
#include <QDir>

class DB : public QObject{
	Q_OBJECT
public:
	DB(QObject *parent=0);
	~DB();

	void startSync();
	void shutDown();

	QString fetchInfo(QStringList request);
	//Request Format: [<type>, <cmd1>, <cmd2>, .... ]

private:
	QHash<QString, QString> *HASH;
	QFileSystemWatcher *watcher;
	QTimer *chkTime;
	bool cmdRunning, stopping, syncing;

	//System Command functions 
	QStringList sysCmd(QString cmd); // ensures only 1 running at a time (for things like pkg)
	QStringList directSysCmd(QString cmd); //run command immediately

	//Internal Hash maintenance functions
	void clearRepo(QString repo);
	void clearJail(QString jail);
	void clearLocalPkg(QString pkgprefix);

	//Internal sync checks
	bool needsLocalSync(QString jail);
	bool needsRemoteSync(QString jail);

	//Simplification functions
	QString generateRepoID(QString jail);

private slots:
	void watcherChange(); //watcher found something change
	//General Sync Functions
	void initialSync();
	void syncJailInfo();
	void syncPkgLocalJail(QString jail);
	void syncPkgLocal();
	void syncPkgRemoteJail(QString jail);
	void syncPkgRemote();
	void syncSysStatus();
	void syncPbi();
	


};
#endif
