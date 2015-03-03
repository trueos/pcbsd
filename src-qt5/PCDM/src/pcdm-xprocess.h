/* PCDM Login Manager:
*  Written by Ken Moore (ken@pcbsd.org) 2012/2013
*  Copyright(c) 2013 by the PC-BSD Project
*  Available under the 3-clause BSD license
*/

/*
 Sub-classed widget for starting an XSession process
*/

#ifndef PCDM_XPROCESS_H
#define PCDM_XPROCESS_H

#include <QFile>
#include <QDebug>
#include <QProcess>
#include <QProcessEnvironment>
#include <QTemporaryFile>

#include <sys/types.h>
#include <security/pam_appl.h>
#include <security/openpam.h>

#include "pcdm-backend.h"

class XProcess : public QProcess
{
	Q_OBJECT

  public:
	XProcess();
	~XProcess();
	bool isRunning();
        void waitForSessionClosed();
	
	//void startDesktop(); 
	
  
  public slots:
        void loginToXSession(QString, QString, QString, QString, QString, bool); //user, pwd, desktop, lang, devicePass, anonymous?
  	void checkPW(QString,QString); //user, pwd
	//void setupDesktop(QString,QString,QString); //user, pwd, desktop
	
  private:
        int fstatus; //fork status
        pid_t fpid;  //fork pid
	QString xuser, xcmd, xhome, xpwd, xde, xshell, xlang, xdevpass;
        bool xanonlogin;
  	bool startXSession();
	
	// Session environment setup
	void setupSessionEnvironment();
  
	// PAM stuff and simplification functions
	pam_handle_t *pamh;  //handle for the PAM process structure
	bool pam_started, pam_session_open;
	bool pam_checkPW();
	bool pam_startSession();
	bool pam_stopSession();
	void pam_logFailure(int);
  	void pam_shutdown(); //cleanly close all the PAM stuff
  
	QTemporaryFile *tFile;

  private slots:
  	void slotCleanup();

  signals:
	void ValidLogin();
  	void InvalidLogin();

};
#endif
