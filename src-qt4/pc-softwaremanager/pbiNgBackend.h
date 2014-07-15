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
#include <QSplashScreen>
#include <QMessageBox>
#include <QProcess>
#include <QCoreApplication>

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
	PBIBackend(QWidget *parent = 0, QSplashScreen *splash = 0);
	~PBIBackend(){}
	//Total Shutdown 
	void shutdown();
		
	//General Setup or restart functions
	void setAutoInstallDesktopEntries(bool);
	void syncLocalPackages();
		
	int appAvailable, pkgAvailable;
	//Search variables for public slot inputs
	QString searchTerm;
	bool searchAll; //whether to include raw packages or not
	QString searchSimilar, searchSize, searchJail;
	// Main Listing functions
	QStringList installedList(QString injail = "", bool raw = false, bool orphan = false); //return origin of all installed PBI's
	QStringList pendingInstallList(); //return origin of all apps pending install
	QStringList pendingRemoveList(); //return origin of all apps pending removal
	QStringList browserCategories(); //return list of available browser categories
	QStringList browserApps( QString catID ); //list all apps in the given category
	QStringList getRecommendedApps(); //list all PC-BSD recommended applications
	QStringList getHighlightedApps(); //list highlighted apps
	QStringList getNewApps(); //list new applications
	
	// Local/Repo Interaction (and simplifications)
	bool safeToQuit(); //return FALSE if there are process running/waiting
	bool isInstalled(QString appID, QString injail = "");
	QString upgradeAvailable(QString pbiID); //returns version available
	
	// PBI Actions
	void cancelActions(QStringList appID); //cancel any pending/current operations
	void removePBI(QStringList appID, QString injail=""); //start the removal process
	void installApp(QStringList appID, QString injail=""); //install application from the repo
	void installAppIntoJail(QString appID);			//install application into a new jail
	void lockApp(QStringList appID, QString injail="");    //Lock current version of application
	void unlockApp(QStringList appID, QString injail=""); //unlock an application
	void addDesktopIcons(QStringList pbiID, bool allusers); // add XDG desktop icons
	void rmDesktopIcons(QStringList pbiID, bool allusers); // remove XDG desktop icons

	
	// Information Retrieval
	NGApp singleAppInfo(QString app, QString injail = "");
	QList<NGApp> AppInfo( QStringList apps, QString injail = "");
	NGCat singleCatInfo(QString cat);
	QList<NGCat> CatInfo( QStringList cats );
	QList<NGApp> SimilarApps( QString app );
	QString getMetaPkgSize(QString appID, QString injail = "");
	QString currentAppStatus(QString appID , QString injail = "");
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
	bool checkForUpdates(QString injail="");
	bool safeToRemove(QString appID);
	QString updateDetails(QString injail="");
	QStringList filterBasePkgs(QStringList);
	QString JailID(QString jailname);
	
	//General functions
	QStringList listDependencies(QString);
	QStringList listRDependencies(QString);
	
public slots:
	void startAppSearch(); //get list of apps containing the search string (SIGNAL OUTPUT ONLY)
	void startSimilarSearch(); //get list of apps that are similar to the input app
	void startSizeSearch(); //Determine the size of the application (useful for non-installed apps)

	void UpdateIndexFiles(bool force = true); //Force update the PBI index from remote

private:
	QWidget *parentWidget;
	QSplashScreen *Splash; //only used during initial sync
	//variables - database
	PBIDBAccess *sysDB;
	QHash<QString, NGCat> CATHASH;
	QHash<QString, NGApp> APPHASH;
	QHash<QString, NGApp> PKGHASH;
	QStringList RECLIST, HIGHLIST, NEWLIST, BASELIST;

	//General values
	QString sysArch; //system architecture
	QString sysUser; //Current user running the AppCafe
	bool autoDE;	//automatically create desktop entries after an install
	bool updavail; //updates available
	
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
	QHash<QString, QHash<QString, NGApp> > JAILPKGS; // <name, hash of pkg details>
	QHash<QString, bool> JAILUPD; // <name, updates available>
	void checkForJails(QString jailID=""); //parses the "jls" command to get name/JID combinations
	
	//General Pkg Conversions
	double pkgSizeToBytes(QString size);
	QString bytesToPkgSize(double bytes);
	
private slots:
	//Process functions
	void checkProcesses();
	void procMessage(QString);
	void procPercent(QString,QString,QString); //percent, file size, filename
	void procFinished(int, QProcess::ExitStatus);

	// Database sync
	void slotSyncToDatabase(bool localChanges=false, bool all = false);
	void updateStatistics(); //number available/installed
	bool checkForPkgUpdates(QString jailID = "");
	void updateSplashScreen(QString);
	
signals:
	void RepositoryInfoReady();
	void NoRepoAvailable();
	void RepositoryInfoUpdated();
	void LocalPBIChanges();
	void JailListChanged();
	void PBIStatusChange(QString pbiID);
	//Process Signals
	void Error(QString title, QString message, QStringList log);
	//Search results
	void SearchComplete(QStringList, QStringList);// "best" and "rest" results lists
	void SimilarFound(QStringList);
	void SizeFound(QString); //Size of the app for display
	//Process Messages (developer mode)
	void devMessage(QString);

};

#endif
