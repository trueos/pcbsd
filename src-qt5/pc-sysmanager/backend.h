#ifndef _SYSMAN_BACKEND_H
#define _SYSMAN_BACKEND_H

#include <QString>
#include <QStringList>
#include <QProcess>
#include <QFile>
#include <QCoreApplication>
#include <QDir>
#include <QDebug>

class Backend{

public:
	//General CMD utilities
	// - Run a command and return any output text
	static QStringList shortCMD(QString dir, QString cmd, QStringList args = QStringList());
	// - Run a command and return success/failure
	static bool quickCMD(QString dir, QString cmd, QStringList args = QStringList());
	// - Delete all the contents of a directory
	static void emptyDir(QString dir);

	//GIT-specific functions
	static void generatePortsUpdateCMDS(QStringList &cmds, QStringList &dirs, QStringList &info);
	static void generateSourceUpdateCMDS(QString branch, QStringList &cmds, QStringList &dirs, QStringList &info);

};

#endif
