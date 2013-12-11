#ifndef _EASYPBI_MODULE_UTILS_H
#define _EASYPBI_MODULE_UTILS_H

#include <QString>
#include <QStringList>
#include <QFile>
#include <QDir>

#include "PBIModule.h"

class ModuleUtils{

public:
	static QStringList validXdgCategories();
	static QString recommendedXdgCategory(QString portCategory);
	static bool xdgExecUsesRoot(QString exec);
	static QString pruneXdgLine(QString exec);
	static QString generateXdgExec(QString shortExec, bool useRoot);
	static QString generateXdgPath(QString shortline);
	static void compressModule(QString modulePath);
	static PBIModule newModule(QString moduleDir, QString port, QString iconFile);

};

#endif
