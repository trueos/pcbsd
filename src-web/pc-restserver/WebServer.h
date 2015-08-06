// ===============================
//  PC-BSD REST API Server
// Available under the 3-clause BSD License
// Written by: Ken Moore <ken@pcbsd.org> July 2015
// =================================
#ifndef _PCBSD_REST_WEB_SERVER_H
#define _PCBSD_REST_WEB_SERVER_H

#include <QWebSocketServer>
#include <QWebSocket>
#include <QWebSocketCorsAuthenticator>
#include <QSslError>
#include <QList>
#include <QObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include "syscache-client.h"
#include "RestStructs.h"

class WebServer : public QWebSocketServer{
	Q_OBJECT
public:
	WebServer();
	~WebServer();

	bool startServer();

public slots:
	void stopServer();

private:
	QWebSocket *csock; //current socket connection
	SysCacheClient *syscache;
	QTimer *idletimer;

	//Main connection comminucations procedure
	void EvaluateREST(QString);
	void EvaluateRequest(const RestInputStruct&); //This is where all the magic happens (needs csock)

	//Simplification functions
	QString JsonValueToString(QJsonValue);
	QStringList JsonArrayToStringList(QJsonArray);
private slots:
	// Overall Server signals
	void ServerClosed(); 						//closed() signal
	void ServerError(QWebSocketProtocol::CloseCode);	//serverError() signal
	void checkIdle(); //see if the currently-connected client is idle
        // New Connection Signals
	void NewSocketConnection(); 					//newConnection() signal
	void NewConnectError(QAbstractSocket::SocketError);	//acceptError() signal
	void SocketClosing();
	// SSL/Authentication Signals
	void OriginAuthRequired(QWebSocketCorsAuthenticator*);	//originAuthenticationRequired() signal
	void PeerVerifyError(const QSslError&);			//peerVerifyError() signal
	void SslErrors(const QList<QSslError>&);			//sslErrors() signal
	//Currently connected socket signal/slot connections
	void EvaluateMessage(const QByteArray&); //needs a current socket (csock), unsets it when done (automatic)
	void EvaluateMessage(const QString&); //needs a current socket (csock), unsets it when done (automatic)
};

#endif