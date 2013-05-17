#ifndef _APPCAFE_PROCESS_MANAGER_H
#define _APPCAFE_PROCESS_MANAGER_H

#include <QProcess>
#include <QProcessEnvironment>
#include <QString>
#include <QObject>
#include <QDebug>

#include "extras.h"
	
class ProcessManager : public QObject
{
	Q_OBJECT
public:
	enum ProcessID{ ALL, UPDATE, REMOVE, DOWNLOAD, INSTALL, OTHER };
	
	ProcessManager();
	~ProcessManager();
	
	void goToDirectory(ProcessID, QString);
	
	void startProcess(ProcessID, QString);
	void stopProcess(ProcessID);
	
signals:
	void ProcessFinished(int ID);
	void ProcessMessage(int ID,QString);
	void ProcessError(int ID,QString);
	
private:
	QProcess *upProc, *remProc, *dlProc, *inProc, *otProc;
	
private slots:
	QString parseDlLine(QString);
	void slotUpProcMessage();
	void slotUpProcFinished();
	void slotRemProcMessage();
	void slotRemProcFinished();
	void slotDlProcMessage();
	void slotDlProcFinished();
	void slotInProcMessage();
	void slotInProcFinished();
	void slotOtProcMessage();
	void slotOtProcFinished();

};

#endif
