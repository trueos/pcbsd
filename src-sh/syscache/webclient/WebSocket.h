// ===============================
//  PC-BSD REST API Server
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
#include <QTimer>

#include "syscache-client.h"
#include "RestStructs.h"

class WebSocket : public QObject{
	Q_OBJECT
public:
	WebSocket(QWebSocket*, QString ID);
	~WebSocket();

	QString ID();
	
private:
	QTimer *idletimer;
	QWebSocket *SOCKET;
	QString SockID;

	//Main connection comminucations procedure
	void EvaluateREST(QString);
	void EvaluateRequest(const RestInputStruct&); //This is where all the magic happens (needs csock)

	//Simplification functions
	QString JsonValueToString(QJsonValue);
	QStringList JsonArrayToStringList(QJsonArray);

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
