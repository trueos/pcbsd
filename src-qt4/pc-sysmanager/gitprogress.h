#ifndef _GIT_PROGRESS_H
#define _GIT_PROGRESS_H

#include <QObject>
#include <QProcess>
#include <QFile>
#include <QString>
#include <QStringList>
#include <QCoreApplication>
#include <QDebug>

class gitProc : public QObject{
	Q_OBJECT
public:
	
	gitProc();
	~gitProc();
	
	//Quick-start functions for specific tasks
	bool startInitPorts(); //initialize the ports tree
	bool startPorts(); //fetch the ports tree
	bool startInitSource(); //initialize the source fetching
	bool startSource(); //fetch the source tree

	//Information functions
	bool isRunning(){ return running; } //so you can double check whether it is still running
	
	//Quick function to run a command and return success/failure
	static bool quickCMD(QString dir, QString cmd, QStringList args = QStringList()); 

private:
	QProcess *process;
	bool running;
	gitProc *longProcess;

private slots:
	void parseUpdate(); //New process message from internal worker
	void procDone(); //internal worker finished

signals:
	void ProcMessage(QString); //a new message while it is running
	void ProcFinished(bool); //the process result (good/bad)
};
#endif
