#ifndef _LP_GUTILS_H
#define _LP_GUTILS_H

#include <QMessageBox>
#include <QDialog>
#include <QStringList>
#include <QString>
#include <QFile>
#include <QDir>

#include "LPBackend.h"
#include "LPContainers.h"

class LPGUtils{
public:
	static LPDataset loadPoolData(QString zpool); //Load backend data into container
	static void loadSnapshotInfo(LPDataset*); //Load the backend snapshot info into container
	static QString generateReversionFileName(QString filename, QString destDir);
	static bool revertFile(QString oldPath, QString newPath); //copy a file out of a snapshot
	static QStringList revertDir(QString oldPath, QString newPath); //copy a dir out of a snapshot
	//Functions for packaging up a user's home directory and extracting it later
	static QString packageHomeDir(QString username, QString packageName);
	static bool checkPackageUserPath(QString packagePath, QString *user);
	static bool extractHomeDirPackage(QString packagePath);
	//Function to scan the system for available harddisks/devices
	static QStringList listAvailableHardDisks();
	//Function to scan the network for available replication targets (SSH open)
	static QStringList scanNetworkSSH();
};

#endif
