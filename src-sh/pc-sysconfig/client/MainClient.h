#ifndef _PCBSD_SYSTEM_CONFIG_CLIENT_MAIN_H
#define _PCBSD_SYSTEM_CONFIG_CLIENT_MAIN_H

#include <QString>
#include <QStringList>
#include <QObject>
#include <QTimer>
#include <QTextStream>
#include <QLocalSocket>
#include <QCoreApplication>
#include <QDebug>

class MainClient : public QObject{
	Q_OBJECT
public:
	MainClient(QObject *parent=0);
	~MainClient();

	void parseInputs(QStringList inputs);

private:
	QLocalSocket *curSock;
	QStringList userRequest;
	QStringList servRequest;

private slots:
	//Server/Client connections
	void startRequest();
	void requestFinished();
	void connectionError();
	

};

#endif
