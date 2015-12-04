#ifndef _DEVICE_DAEMON_MAIN_H
#define _DEVICE_DAEMON_MAIN_H

#include <QString>
#include <QStringList>
#include <QObject>
#include <QTimer>
#include <QTextStream>
#include <QFile>
#include <QLocalServer>
#include <QLocalSocket>
#include <QCoreApplication>

#include "Backend.h"

class MainDaemon : public QObject{
	Q_OBJECT
public:
	MainDaemon(QObject *parent=0);
	~MainDaemon();

	void startServer();

private:
	QLocalServer *server;
	QLocalSocket *curSock;
	Backend *SYSTEM;

private slots:
	void stopServer();

	//Server/Client connections
	void checkForConnections();
	void answerRequest();
	void requestFinished();

	

};

#endif
