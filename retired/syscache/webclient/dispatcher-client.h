#ifndef _WEB_SERVER_DISPATCHER_CLIENT_MAIN_H
#define _WEB_SERVER_DISPATCHER_CLIENT_MAIN_H

#include <QString>
#include <QStringList>
#include <QProcess>
#include <QDebug>

#include "AuthorizationManager.h"

class DispatcherClient : public QProcess{
	Q_OBJECT
public:
	DispatcherClient(AuthorizationManager *auth, QObject *parent=0);
	~DispatcherClient();

	bool setupProcAuth();
	QString GetProcOutput(QString args);

	//Static function to run a request and wait for it to finish before returning
	static QStringList parseInputs(QStringList inputs, AuthorizationManager *auth);

private:
	AuthorizationManager *AUTH;

	QString ReadKey();
	bool WriteKey(QString key);
};

#endif
