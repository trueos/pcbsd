#ifndef _APPCAFE_PROCESS_MANAGER_H
#define _APPCAFE_PROCESS_MANAGER_H

#include <QString>
#include <QStringList>
#include <QObject>
#include <QDebug>

#include <pcbsd-DLProcess.h>

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
	QStringList getProcessLog(ProcessID);
	
signals:
	void ProcessFinished(int ID);
	void ProcessMessage(int ID,QString);
	void ProcessError(int ID,QStringList);
	
private:
	DLProcess *upProc, *remProc, *dlProc, *inProc, *otProc;
	QStringList upLog, remLog, dlLog, inLog;
        bool remStrictErrChecking;

private slots:
	//QString parseDlLine(QString);
	void slotUpProcMessage(QString);
	void slotUpProcStats(QString,QString, QString);
	void slotUpProcFinished();

	void slotRemProcMessage(QString);
	void slotRemProcFinished();

	void slotDlProcMessage(QString);
	void slotDlProcStats(QString,QString, QString);
	void slotDlProcFinished();

	void slotInProcMessage(QString);
	void slotInProcFinished();

	void slotOtProcMessage(QString);
	void slotOtProcFinished();

};

#endif
