#ifndef _PCBSD_DOWNLOAD_PROCESS_H
#define _PCBSD_DOWNLOAD_PROCESS_H

#include <QProcess>
#include <QProcessEnvironment>
#include <QObject>
#include <QString>
#include <QDebug>
#include <QFile>

class DLProcess : public QProcess{
	Q_OBJECT
public:
	DLProcess(QObject* parent = 0);
	~DLProcess();

	void setWardenDir(QString wardendir); //If the PKG type is running in a jail (before setting type)
	void setDLType(QString type); // [PBI, PKG, CDN] - (***ONLY RUN ONCE PER INSTANCE***)
	bool isRunning();

private:
	int DLTYPE;
	QProcess *watcher;
	QString pipeFile;

	void calculateStats(QString current, QString total, QString speed, QString filename);
	void parsePBILine(QString line);
	void parsePKGLine(QString line);

	QString kbToString(double);

private slots:
	void ProcFinished();
	void newMessage();

signals:
	void UpdateMessage(QString); //Any messages not related to download status
	void UpdatePercent(QString, QString, QString); //percent (XX.x), file size, Other info (speed or filename)
};

#endif
