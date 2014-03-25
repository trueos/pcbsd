/*
  Static class of simplification functions for XDG compatibility/searching
  Author: Ken Moore <ken@pcbsd.org> 3/25/14
*/

#ifndef _PCBSD_XDG_UTILS_H
#define _PCBSD_XDG_UTILS_H

#include <QDir>
#include <QFile>
#include <QString>
#include <QStringList>
#include <QList>
#include <QHash>

#include <unistd.h>
#include <stdlib.h>

#include "pcbsd-xdgfile.h"

class XDGUtils{
public:
	//Search the system for all application shortcuts
	static QList<XDGFile> allApplications(bool showhidden = false, bool showinvalid = false);
	//Filter a list of applications by category
	static QList<XDGFile> filterAppsByCategory(QString cat, QList<XDGFile> apps);
	//Sort a list of applications by name
	static QList<XDGFile> sortAppsByName(QList<XDGFile> apps);
};

#endif