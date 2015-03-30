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
#include <QFile>
#include <QTextStream>
#include <QThread>
#include <QTime>

class Syncer : public QObject{
	Q_OBJECT
public:
	Syncer(QObject *parent = 0, QHash<QString,QString> *hash = 0);
	~Syncer();

	//Subclass run(), so that we can kick off a sync by just Syncer->start();
	/*void run(){
	  performSync();
	}*/
public slots:
	void performSync(); //Overarching start function

private:
	QHash<QString,QString> *HASH;
	bool stopping;

	//System Command functions 
	QStringList sysCmd(QString cmd); // ensures only 1 running at a time (for things like pkg)
	QStringList directSysCmd(QString cmd); //run command immediately
	QStringList readFile(QString filepath); //read the contents of a text file

	//Internal Hash maintenance functions
	void clearRepo(QString repo);
	void clearJail(QString jail);
	void clearLocalPkg(QString pkgprefix);
	void clearPbi();

	//Internal sync checks
	bool needsLocalSync(QString jail);
	bool needsRemoteSync(QString jail);
	bool needsPbiSync();
	bool needsSysSync();
	
	//Simplification functions
	QString generateRepoID(QString jail);
	
private slots:

	//Individual sync functions
	void syncJailInfo();
	void syncPkgLocalJail(QString jail);
	void syncPkgLocal();
	void syncPkgRemoteJail(QString jail);
	void syncPkgRemote();
	void syncSysStatus();
	void syncPbi();

signals:
	void finishedJails();
	void finishedLocal();
	void finishedRemote();
	void finishedSystem();
	void finishedPBI();
};

//Overall class for managing the syncer and reading data from the hash
class DB : public QObject{
	Q_OBJECT
public:
	DB(QObject *parent=0);
	~DB();


	void shutDown();

	QString fetchInfo(QStringList request);
	//Request Format: [<type>, <cmd1>, <cmd2>, .... ]

	void writeToLog(QString message);

public slots:
	void startSync();

private:
	QHash<QString, QString> *HASH;
	QFileSystemWatcher *watcher;
	QTimer *chkTime, *maxTime;
	Syncer *SYNC;
	QThread *syncThread;
	bool jrun, locrun, remrun, pbirun, sysrun;

	QStringList doSearch(QString srch, QString jail = "pbi", int findmin = 10, int filter = 0);
	//Filter Note: [0=all, 1=graphical, -1=!graphical, 2=server, -2=!server, 3=text, -3=!text]

	QStringList sortByName(QStringList origins, bool haspriority = false);

	//Internal pause/syncing functions
	void validateHash(QString key);
	bool isRunning(QString key);
	void pausems(int ms);


private slots:
	void watcherChange(QString); //watcher found something change
	void kickoffSync();
	
	//Syncer status updates
	void localSyncFinished(){ locrun = false; writeToLog(" - Local Sync Finished:"+QDateTime::currentDateTime().toString(Qt::ISODate)); }
	void remoteSyncFinished(){ remrun = false; writeToLog(" - Remote Sync Finished:"+QDateTime::currentDateTime().toString(Qt::ISODate)); }
	void pbiSyncFinished(){ pbirun = false; writeToLog(" - PBI Sync Finished:"+QDateTime::currentDateTime().toString(Qt::ISODate)); }
	void jailSyncFinished();
	void systemSyncFinished(){ sysrun = false; writeToLog(" - Full Sync Complete:"+QDateTime::currentDateTime().toString(Qt::ISODate)); }

};


#endif
