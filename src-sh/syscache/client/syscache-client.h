#ifndef _SYSCACHE_CLIENT_MAIN_H
#define _SYSCACHE_CLIENT_MAIN_H

#include <QString>
#include <QStringList>
#include <QObject>
#include <QTimer>
#include <QTextStream>
#include <QLocalSocket>
#include <QCoreApplication>
#include <QDebug>

class SysCacheClient : public QObject{
	Q_OBJECT
public:
	SysCacheClient(QObject *parent=0);
	~SysCacheClient();

	void parseInputs(QStringList inputs);

private:
	QLocalSocket *curSock;
	QStringList userRequest;
	QStringList servRequest;

private slots:
	void showUsage();

	//Server/Client connections
	void startRequest();
	void requestFinished();
	void connectionError();
	

};

#endif
