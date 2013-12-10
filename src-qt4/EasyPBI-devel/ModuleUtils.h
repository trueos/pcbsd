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
	static void compressModule(QString modulePath);
	static PBIModule newModule(QString moduleDir, QString port, QString iconFile);

};

#endif
