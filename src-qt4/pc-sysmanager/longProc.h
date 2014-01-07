#ifndef _LONG_PROGRESS_H
#define _LONG_PROGRESS_H

#include <QObject>
#include <QProcess>
#include <QFile>
#include <QString>
#include <QStringList>
#include <QCoreApplication>
#include <QDebug>

class LongProc : public QObject{
	Q_OBJECT
public:
	
	LongProc();
	~LongProc();
	
	//Quick-start functions for specific tasks
	bool startCMDs(QStringList cmds, QStringList dirs, QStringList info);
	
	//Information functions
	bool isRunning(){ return running; } //so you can double check whether it is still running
	
	//Quick function to run a command and return success/failure
	static bool quickCMD(QString dir, QString cmd, QStringList args = QStringList()); 

private:
	QProcess *process;
	bool running;
	QStringList cmdList, infoList, dirList;
	int currentItem;

private slots:
	void parseUpdate(); //New process message from internal worker
	void procDone(); //internal worker finished

signals:
	void ProcMessage(QString); //a new message while it is running
	void ProcFinished();
};
#endif
