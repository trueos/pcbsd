#ifndef _LONG_PROGRESS_H
#define _LONG_PROGRESS_H

#include <QObject>
#include <QProcess>
#include <QFile>
#include <QString>
#include <QStringList>
#include <QDebug>
#include <QTimer>

class LongProc : public QObject{
	Q_OBJECT
public:
	
	LongProc();
	~LongProc();
	
	//Quick-start functions for specific tasks
	bool startCMDs(QStringList cmds, QStringList dirs, QStringList info);
	void stopProc();

	//Information functions
	bool isRunning(){ return running; } //so you can double check whether it is still running

private:
	QProcess *process;
	bool running, stopped;
	QStringList cmdList, infoList, dirList;
	int currentItem;
	QTimer *timer;

private slots:
	void parseUpdate(); //New process message from internal worker
	void procDone(); //internal worker finished
	void procTimeout(); //internal timeout signal

signals:
	void ProcMessage(QString); //a new message while it is running
	void ProcFinished();
};
#endif
