#ifndef _APPCAFE_PBI_DATABASE_ACCESS_H
#define _APPCAFE_PBI_DATABASE_ACCESS_H

#include <QString>
#include <QStringList>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QDebug>
#include <QProcess>
#include <QProcessEnvironment>

#include "extras.h"

class PBIDBAccess{
public:

	PBIDBAccess(){
	  DBDir = new QDir();
	  proc = new QProcess;
	  proc->setProcessEnvironment( QProcessEnvironment::systemEnvironment() );
	  proc->setProcessChannelMode(QProcess::MergedChannels);
	}
	~PBIDBAccess(){}
	bool setDBPath(QString); //must be set before anything else!!
	void setRootCMDPrefix(QString); //required to run any of the DB modification functions
	//Repository Management
	bool setRepo(QString repoNum);
	bool currentRepoInvalid();
	QString currentRepo(){ return currentRepoNumber; }
	void reloadRepoList();
	QStringList availableRepos(); //returns list of repoNumbers 
	QStringList repoInfo(QString repoNum);
	QStringList repoMirrors(QString repoNum);
	//Main access functions
	QStringList installed(); // return list of ID strings for installed PBI's
	QStringList installedPbiInfo(QString pbiID);
	bool installedPbiNeedsRoot(QString pbiID);
	bool installedPbiAutoUpdate(QString pbiID);
	bool installedPbiHasXdgDesktop(QString installPath);
	bool installedPbiHasXdgMenu(QString installPath);
	QString indexFilePath();
	QString metaFilePath();
	QStringList parseIndexLine(QString line);
	QStringList parseAppMetaLine(QString line);
	QStringList parseCatMetaLine(QString line);
	QString remoteToLocalIcon(QString name, QString remoteIconPath);
	// Database Modification functions
	bool addRepoFile(QString rpofilepath);
	bool removeRepo(QString repoNum);
	bool moveRepoUp(QString repoNum);
	bool moveRepoDown(QString repoNum);
	bool setRepoMirrors(QString repoNum, QStringList mirrors);
	
private:
	QString currentRepoNumber, currentRepoID, DBPath, cmdPrefix;
	QStringList repoList;
	QDir *DBDir;
	QProcess *proc;
	QString readOneLineFile(QString);
	QString getIDFromNum(QString);
	QString runCMD(QString);
	
};

#endif
