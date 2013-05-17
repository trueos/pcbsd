#ifndef _APPCAFE_PBI_BACKEND_H
#define _APPCAFE_PBI_BACKEND_H

/*  TERMINOLOGY NOTE:
	"PBI" refers to locally installed applications
	"APP" refers to available applications in the repo database
	"CAT" refers to available categories in the repo database
*/
#include <QFileSystemWatcher>
#include <QString>
#include <QStringList>
#include <QHash>
#include <QTimer>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QDate>
#include <QMessageBox>

#include "containers.h"
#include "extras.h"
#include "pbiDBAccess.h"
#include "processManager.h"
#include "configDialog.h"

class PBIBackend : public QObject{
	Q_OBJECT

public:
	//Initializations
	PBIBackend(QWidget *parent = 0);
	~PBIBackend(){}
	// Clean Start/Stop functions
	void setWardenMode(QString dir, QString ip);
	bool start();
	int numInstalled, numAvailable;
	//Search variables for public slot inputs
	QString searchTerm;
	QString searchSimilar;
	// Main Listing functions
	QStringList installedList(); //return unique ID's of all installed PBI's
	QStringList browserCategories(); //return list of available browser categories
	QStringList browserApps( QString catID ); //list all apps in the given category
	QStringList getRecentApps(); //list all recent applications/update
	bool safeToQuit(); //return FALSE if there are process running/waiting
	// Local/Repo Interaction
	QString isInstalled(QString appID); //returns pbiID that is installed (if possible);
	QString upgradeAvailable(QString pbiID); //returns appID for PBI upgrade (if available)
	// PBI Actions
	void cancelActions(QStringList pbiID); //cancel any pending/current operations
	void upgradePBI(QStringList pbiID); //start upgrade process for list of PBI's
	void removePBI(QStringList pbiID); //start the removal process
	void stopUpdate(QStringList pbiID); //stop upgrade/downgrade/removal process
	void installApp(QStringList appID); //[install/upgrade/downgrade] application from the repo (as appropriate)
	void addDesktopIcons(QStringList pbiID, bool allusers); // add XDG desktop icons
	void addMenuIcons(QStringList pbiID, bool allusers); // add XDG menu icons
	void addPaths(QStringList pbiID, bool allusers); // create path links
	void addMimeTypes(QStringList pbiID, bool allusers); //add XDG Mime type associations
	void rmDesktopIcons(QStringList pbiID, bool allusers); // remove XDG desktop icons
	void rmMenuIcons(QStringList pbiID, bool allusers); // remove XDG menu icons
	void rmPaths(QStringList pbiID, bool allusers); // remove path links
	void rmMimeTypes(QStringList pbiID, bool allusers); //remove XDG Mime type associations
	void enableAutoUpdate(QString pbiID, bool enable); //enable/disable auto-updating
	
	// Information Retrieval
	QStringList PBIInfo( QString pbiID, QStringList infoList);
	QStringList CatInfo( QString catID, QStringList infoList);
	QStringList AppInfo( QString appID, QStringList infoList);
	QString currentAppStatus( QString appID );
	
	//Configuration Management
	void openConfigurationDialog();
	
	//Import/Export PBI lists
	bool exportPbiListToFile(QString filepath);
	bool importPbiListFromFile(QString filepath);

public slots:
	void startAppSearch(); //get list of apps containing the search string (SIGNAL OUTPUT ONLY)
	void startSimilarSearch(); //get list of apps that are similar to the input app
	
private:
	QWidget *parentWidget;
	//variables - warden mode
	QString wardenDir, wardenIP;
	bool wardenMode;
	//variables - filesystem watcher;
	QFileSystemWatcher *watcher;
	QString baseDBDir; //  This is always /var/db/pbi/
	QString DBDir;
	//variables - database
	PBIDBAccess *sysDB;
	QHash<QString, InstalledPBI> PBIHASH;
	QHash<QString, MetaCategory> CATHASH;
	QHash<QString, MetaPBI> APPHASH;
	bool noRepo;
	//variables - processes
	ProcessManager *PMAN;
	QString cDownload, cInstall, cRemove, cUpdate, cDownloadFile, cOther; //currently running command/pbi
	bool sDownload, sInstall, sRemove, sUpdate; //flag that these processes were Stopped
	QStringList PENDINGDL, PENDINGINSTALL, PENDINGREMOVAL, PENDINGUPDATE, PENDINGOTHER;
	//variables - other
	QString baseDlDir, dlDir; // download/install directories
	QString sysArch; //system architecture
	//Configuration Management
	bool keepDownloads;
	QStringList autoXDG;
	QString settingsFile, currentRepoNum; //currentRepoNum only used during initial load
	bool saveSettings();
	bool loadSettings();

	//functions
	QString addRootCMD(QString cmd, bool needRoot);
	QString generateUpdateCMD(QString pbiID);
	QString generateRemoveCMD(QString pbiID);
	QString generateAutoUpdateCMD(QString pbiID, bool enable);
	QString generateXDGCMD(QString pbiID, QStringList actions, bool allusers = FALSE);
	QString generateDownloadCMD(QString appID, QString version="");
	QString generateInstallCMD(QString pbiID);
	QStringList removePbiCMD(QString pbiID, QStringList list);
	
private slots:
	void updateDlDirPath(QString);
	
	//Process functions
	void checkProcesses();
	void slotProcessFinished(int);
	void slotProcessMessage(int, QString);
	void slotProcessError(int, QString);

	// Database watcher
	void slotSyncToDatabase(bool localChanges=TRUE);
	void syncPBI(QString pbiID, bool useDB=TRUE);
	void slotUpdateAllStatus();
	void updateStatus(QString pbiID);
	void syncCurrentRepo();
	
	//Filesystem Watcher
	void startFileSystemWatcher();
	void stopFileSystemWatcher();

	
signals:
	void RepositoryInfoReady();
	void NoRepoAvailable();
	void LocalPBIChanges();
	void PBIStatusChange(QString pbiID);
	//Process Signals
	void Error(QString title, QString message);
	//Search results
	void SearchComplete(QStringList, QStringList);// "best" and "rest" results lists
	void SimilarFound(QStringList);
};

#endif
