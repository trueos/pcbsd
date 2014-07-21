#ifndef _SYSCACHE_DAEMON_MAIN_H
#define _SYSCACHE_DAEMON_MAIN_H

#include <QString>
#include <QStringList>
#include <QObject>
#include <QTimer>
#include <QTextStream>
#include <QLocalServer>
#include <QLocalSocket>
#include <QCoreApplication>

class SysCacheDaemon : public QObject{
	Q_OBJECT
public:
	SysCacheDaemon(QObject *parent=0);
	~SysCacheDaemon();

	void startServer();

private:
	QLocalServer *server;
	QLocalSocket *curSock;

private slots:
	void stopServer();

	//Server/Client connections
	void checkForConnections();
	void answerRequest();
	void requestFinished();

	

};

#endif
