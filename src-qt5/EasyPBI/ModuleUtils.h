#ifndef _EASYPBI_MODULE_UTILS_H
#define _EASYPBI_MODULE_UTILS_H

#include <QString>
#include <QStringList>
#include <QFile>
#include <QDir>

#include "PBIModule.h"
#include "backend.h"

class ModuleUtils{

public:
	//Valid/Recommended values
	static QStringList validXdgCategories();
	static QString recommendedXdgCategory(QString portCategory);
	static QStringList validExternalLinkTypes();
	static QString addExternalLinkType(QString current, QString newtype);
	static QStringList validRepoCategories();
	static QString recommendedRepoCategory(QString portCategory);
	static QStringList validRepoTypes();
	
	//XDG Simplifications
	static bool xdgExecUsesRoot(QString exec);
	static QString pruneXdgLine(QString exec);
	static QString generateXdgExec(QString shortExec, bool useRoot);
	static QString generateXdgPath(QString shortline);

	//General Module Utilities
	static void compressModule(QString modulePath);
	static PBIModule newModule(QString moduleDir, QString port, QString iconFile, QStringList *plist, bool useplist = false);
	static QString generatePbiBuildCmd(QString confDir, QString outDir, QString sigFile, bool packageBuild = true);
	static QStringList generateWrapperScriptTemplate();

};

#endif
