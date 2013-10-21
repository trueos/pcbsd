#ifndef _LP_WATCHER_H
#define _LP_WATCHER_H

#include <QCoreApplication>
#include <QDir>
#include <QObject>
#include <QFileSystemWatcher>
#include <QStringList>
#include <QString>
#include <QHash>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QProcess>

class LPWatcher : public QObject{
	Q_OBJECT
public:
	LPWatcher();
	~LPWatcher();

	QStringList getMessages(QString type, QStringList msgList);
	QStringList getAllCurrentMessages();
	bool isRunning();
	bool hasError();

public slots:
	void start();
	void stop();
	void refresh();

private:
	//Internal paths for the lpreserver output files
	QString FILE_LOG, FILE_ERROR, FILE_REPLICATION;
	//Internal message Logs
	QHash<unsigned int,QString> LOGS;
	//File system watcher
	QFileSystemWatcher *watcher;
	QTimer *timer;
	int sysCheckTime;
	QFile *logfile, *repfile;
	QTextStream *LFSTREAM, *RFSTREAM;
	//Replication size variables
	QString repTotK, lastSize;

	void readLogFile(bool quiet = false);
	void readReplicationFile(); //always sends quiet signals

	bool startRepFileWatcher();
	void stopRepFileWatcher();

	double displayToDoubleK(QString); // string->double conversion
	bool isReplicationRunning(); //check for replication PID file
	
	QStringList listReplicatedPools();
	QStringList getCmdOutput(QString cmd);
	
private slots:
	void fileChanged(QString); //file system watcher saw a change
	void checkPoolStatus(); //check for serious system errors

signals:
	void MessageAvailable(QString type);
	void StatusUpdated();
};

#endif
