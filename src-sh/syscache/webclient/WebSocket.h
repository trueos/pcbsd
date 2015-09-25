// ===============================
//  PC-BSD REST/JSON API Server
// Available under the 3-clause BSD License
// Written by: Ken Moore <ken@pcbsd.org> July 2015
// =================================
#ifndef _PCBSD_REST_WEB_SOCKET_H
#define _PCBSD_REST_WEB_SOCKET_H

#include <QWebSocket>
#include <QList>
#include <QObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QTimer>

#include "syscache-client.h"
#include "RestStructs.h"
#include "AuthorizationManager.h"

class WebSocket : public QObject{
	Q_OBJECT
public:
	WebSocket(QWebSocket*, QString ID, AuthorizationManager *auth);
	~WebSocket();

	QString ID();
	
private:
	QTimer *idletimer;
	QWebSocket *SOCKET;
	QString SockID, SockAuthToken;
	AuthorizationManager *AUTHSYSTEM;

	//Main connection comminucations procedure
	void EvaluateREST(QString);
	void EvaluateRequest(const RestInputStruct&); //This is where all the magic happens
	void EvaluateSysCacheRequest(const QJsonValue in_args, QJsonObject *out);


	//Simplification functions
	QString JsonValueToString(QJsonValue);
	QStringList JsonArrayToStringList(QJsonArray);
	void SetOutputError(QJsonObject *ret, QString id, int err, QString msg);

private slots:
	void checkIdle(); //see if the currently-connected client is idle
	void SocketClosing();

	//Currently connected socket signal/slot connections
	void EvaluateMessage(const QByteArray&); 
	void EvaluateMessage(const QString&);


signals:
	void SocketClosed(QString); //ID

};

#endif
