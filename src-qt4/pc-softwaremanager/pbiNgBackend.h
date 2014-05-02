#ifndef _APPCAFE_PBI_BACKEND_H
#define _APPCAFE_PBI_BACKEND_H

/*  TERMINOLOGY NOTE:
	"PBI" refers to locally installed applications
	"APP" refers to available applications in the repo database
	"CAT" refers to available categories in the repo database
*/
// Qt includes
//#include <QFileSystemWatcher>
#include <QString>
#include <QStringList>
#include <QHash>
#include <QTimer>
#include <QFile>
#include <QDir>
#include <QDebug>
//#include <QDate>
#include <QMessageBox>
#include <QProcess>

// libPCBSD includes
#include <pcbsd-DLProcess.h>
#include <pcbsd-xdgfile.h>

// Local includes
#include "extras.h"
#include "pbiDBAccess.h"

class PBIBackend : public QObject{
	Q_OBJECT

public:
	//Initializations
	PBIBackend(QWidget *parent = 0);
	~PBIBackend(){}
	//General Setup or restart functions
	void setAutoInstallDesktopEntries(bool);
	void setRawPkgMode(bool);
		
	int appAvailable, pkgAvailable;
	//Search variables for public slot inputs
	QString searchTerm;
	QString searchSimilar;
	// Main Listing functions
	QStringList installedList(); //return origin of all installed PBI's
	QStringList pendingInstallList(); //return origin of all apps pending install
	QStringList pendingRemoveList(); //return origin of all apps pending removal
	QStringList browserCategories(); //return list of available browser categories
	QStringList browserApps( QString catID ); //list all apps in the given category
	QStringList getRecommendedApps(); //list all PC-BSD recommended applications
	QStringList getHighlightedApps(); //list highlighted apps
	QStringList getNewApps(); //list new applications
	
	// Local/Repo Interaction (and simplifications)
	bool safeToQuit(); //return FALSE if there are process running/waiting
	bool isInstalled(QString appID);
	QString upgradeAvailable(QString pbiID); //returns version available
	
	// PBI Actions
	void cancelActions(QStringList appID); //cancel any pending/current operations
	void removePBI(QStringList appID, QString injail=""); //start the removal process
	void installApp(QStringList appID, QString injail=""); //install application from the repo
	void lockApp(QStringList appID, QString injail="");    //Lock current version of application
	void unlockApp(QStringList appID, QString injail=""); //unlock an application
	void addDesktopIcons(QStringList pbiID, bool allusers); // add XDG desktop icons
	void rmDesktopIcons(QStringList pbiID, bool allusers); // remove XDG desktop icons

	
	// Information Retrieval
	NGApp singleAppInfo(QString app);
	QList<NGApp> AppInfo( QStringList apps );
	NGCat singleCatInfo(QString cat);
	QList<NGCat> CatInfo( QStringList cats );
	QList<NGApp> SimilarApps( QString app );
	
	QString currentAppStatus( QString appID );
	bool isWorking(QString pbiID);
	QStringList appBinList(QString appID); //<name>::::<*.desktop file path> list

	//Import/Export PBI lists
	bool exportPbiListToFile(QString filepath);
	bool importPbiListFromFile(QString filepath);
	
	//Jail Availability
	QStringList runningJails();
	QStringList jailsWithoutPkg(QString appID);
	
	//General purpose
	void runCmdAsUser(QString cmd);
	
public slots:
	void startAppSearch(); //get list of apps containing the search string (SIGNAL OUTPUT ONLY)
	void startSimilarSearch(); //get list of apps that are similar to the input app
	
private:
	QWidget *parentWidget;
	//variables - database
	PBIDBAccess *sysDB;
	QHash<QString, NGCat> CATHASH;
	QHash<QString, NGApp> APPHASH;
	QHash<QString, NGApp> PKGHASH;
	QStringList RECLIST, HIGHLIST, NEWLIST;

	//General values
	QString sysArch; //system architecture
	QString sysUser; //Current user running the AppCafe
	bool autoDE;	//automatically create desktop entries after an install
	
	//All the Process queing/interaction
	DLProcess *pkgProc;
	QStringList PENDING;
	QString PKGRUN; //origin for the currently-running process
	QString PKGCMD; //Currently running command
	QString PKGJAIL; //
	QString PKGRUNSTAT; //string for the running process stat
	QStringList PROCLOG;
	int PROCTYPE; //The type of process that is running [0=install, 1=remove, -1=other]
	bool PROCCANCELLED; //the process was already cancelled
	
	void queueProcess(QString origin, bool install, QString injail="");
	
	//Jail interaction/translation
	QHash<QString, QString> RUNNINGJAILS; // <name, ID>
	QHash<QString, QStringList> JAILPKGS; // <name, list of installed pkgs>
	void checkForJails(QString jailID=""); //parses the "jls" command to get name/JID combinations
	
private slots:
	//Process functions
	void checkProcesses();
	void procMessage(QString);
	void procPercent(QString,QString,QString); //percent, file size, filename
	void procFinished(int, QProcess::ExitStatus);

	// Database sync
	void slotSyncToDatabase(bool localChanges=false);
	void updateStatistics(); //number available/installed
	
signals:
	void RepositoryInfoReady();
	void NoRepoAvailable();
	void LocalPBIChanges();
	void PBIStatusChange(QString pbiID);
	//Process Signals
	void Error(QString title, QString message, QStringList log);
	//Search results
	void SearchComplete(QStringList, QStringList);// "best" and "rest" results lists
	void SimilarFound(QStringList);
	//Process Messages (developer mode)
	void devMessage(QString);

};

#endif
