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
	bool cmdRunning, stopping;

	//System Command functions 
	QStringList sysCmd(QString cmd); // ensures only 1 running at a time (for things like pkg)
	QStringList directSysCmd(QString cmd); //run command immediately

	//Internal Hash maintenance functions
	void clearRepo(QString repo);
	void clearJail(QString jail);
	void clearRemotePkg(QString pkgprefix);
	void clearLocalPkg(QString pkgprefix);

private slots:
	//General Sync Functions
	void initialSync();
	void syncJailInfo();
	void syncPkgLocal();
	void syncPkgRemote();
	void syncSysStatus();
	void syncPbi();
	


};
#endif
