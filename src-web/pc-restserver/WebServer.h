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

private slots:
	//Main connection comminucations procedure
	void EvaluateConnection(); //needs a current socket (csock), unsets it when done

	// Overall Server signals
	void ServerClosed(); 						//closed() signal
	void ServerError(QWebSocketProtocol::CloseCode);	//serverError() signal
        // New Connection Signals
	void NewSocketConnection(); 					//newConnection() signal
	void NewConnectError(QAbstractSocket::SocketError);	//acceptError() signal
	// SSL/Authentication Signals
	void OriginAuthRequired(QWebSocketCorsAuthenticator*);	//originAuthenticationRequired() signal
	void PeerVerifyError(const QSslError&);			//peerVerifyError() signal
	void SslErrors(const QList<QSslError>&);			//sslErrors() signal
};

#endif