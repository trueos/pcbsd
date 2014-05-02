#ifndef _APPCAFE_PBI_DATABASE_ACCESS_H
#define _APPCAFE_PBI_DATABASE_ACCESS_H

#include <QString>
#include <QStringList>
#include <QHash>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QProcess>
#include <QProcessEnvironment>
#include <QDateTime>
#include <QDir>
#include <QFile>

#define PBI_DBDIR QString("/var/db/pbi/index/")

// Info Container for PbiNg categories
class NGCat{
  public:
	QString name, icon, description, portcat;
  
	NGCat(){}
	~NGCat(){}
};

// Info Container for PbiNg applications
class NGApp{
  public:
	QString origin, pbiorigin, name, version, icon, description, shortdescription, portcat, maintainer, website, license, type, category, size, author, arch, rating;
	QString installedversion, installedsize, installedwhen, installedarch;
	bool isInstalled, isLocked, isOrphan, isRecommended;
	QStringList similarApps, needsPkgs, tags, screenshots, possiblePlugins, installedPlugins, buildOptions, rdependancy, dependency;
	bool hasDE, hasME, hasMT; //desktop/menu entries, mimetypes, path links
  
	NGApp(){
	  isInstalled=false; isLocked=false; isOrphan=false; isRecommended=false;
	  hasDE=false; hasME=false; hasMT=false;
	}
	~NGApp(){}
};

// Interface class for the system databases (pkg and PBI)
class PBIDBAccess{
public:

	PBIDBAccess();
	~PBIDBAccess();
	
	//Important - be sure to run this command before calling the info functions
	//  to ensure the internal data is correct for the jail needed (even if no jail);
	void syncDBInfo(QString jailID = "", bool localreload = false, bool allreload = false); //setup the internal variables to associate with the given jail
	
	//Main access functions
	QHash<QString, NGCat> Categories();  //All categories for ports/pbi's (unified)
	QHash<QString, NGApp> DetailedAppList(); //PBI-apps that can/are installed
	QHash<QString, NGApp> DetailedPkgList(); //Pkg-apps that can/are installed (Warning - takes a while!)
	
	//Individual access functions
	NGApp getLocalPkgDetails(NGApp);
	
	//Package Lists
	QStringList getRawPkgList(); //All possible pkgs
	QStringList getRawInstalledPackages(); //all installed packages
	QHash<QString, NGApp> getRawAppList(); //PBI-apps that can be installed
	
	//General item update/info
	NGApp updateAppStatus(NGApp); //re-update installed info
	QStringList AppMenuEntries(NGApp); //get available menu *.desktop files
	
	void getAppCafeHomeInfo(QStringList *NEW, QStringList *HIGHLIGHT, QStringList *RECOMMEND);
		
	QStringList listJailPackages(QString jailID);
	QStringList basePackageList();
	
private:
	QProcess *proc;
	QString jailLoaded;
	QHash<QString, NGApp> PKGINSTALLED; //Detailed list of packages (installed)
	QHash<QString, NGApp> PBIAVAIL; //raw list of PBI's from the index file
	QHash<QString, NGCat> CATAVAIL; //raw list of categories from the index file
	QHash<QString, NGApp> PKGAVAIL; //raw list of packages from the repo


	//Internal list sync-ers
	void syncLargePkgRepoList(bool reload = false); //can take a while
	bool syncPkgInstallList(QString jailID = "", bool reload = false);
	void syncPbiRepoLists(bool reload = false);

	//Parsers
	NGApp parseNgIndexLine(QString line);
	NGCat parseNgCatLine(QString line);
	void parseNgPkgLine(QString line);

	//General utilities
	QString readOneLineFile(QString);
	QString runCMD(QString cmd, QStringList args = QStringList() );
	QStringList cmdOutput(QString cmd, QStringList args = QStringList() );
	QString cleanupDescription(QStringList tmp);
	QStringList readIndexFile();
	QStringList readAppCafeFile();
};

#endif
