#ifndef _SYSCACHE_CLIENT_MAIN_H
#define _SYSCACHE_CLIENT_MAIN_H

#include <QString>
#include <QStringList>
#include <QObject>
#include <QTextStream>
#include <QLocalSocket>
#include <QCoreApplication>
#include <QDebug>

class SysCacheClient : public QLocalSocket{
	Q_OBJECT
public:
	SysCacheClient(QObject *parent=0);
	~SysCacheClient();

	//Static function to run a request and wait for it to finish before returning
	static QStringList parseInputs(QStringList inputs);

	//input/output variables
	QStringList userRequest, servRequest, ans;


private slots:
	//Server/Client connections
	void startRequest();
	void requestFinished();
	void connectionError();

};

#endif
