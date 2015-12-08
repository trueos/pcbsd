//===========================================
//  PC-BSD source code
//  Copyright (c) 2015, PC-BSD Software/iXsystems
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#ifndef __PCBSD_LIB_UTILS_GENERAL_H
#define __PCBSD_LIB_UTILS_GENERAL_H

#include "global.h"

namespace pcUtil{

class General{
public:
	//Non-event-blocking versions of QProcess::execute() or system()
	//Note: environment changes should be listed as such: [<variable>=<value>]
	// - Both success/log of output
	static QString RunCommand(bool &success, QString command, QStringList arguments = QStringList(), QString workdir = "", QStringList env = QStringList() ); 
	// - Log output only
	static QString RunCommand(QString command, QStringList arguments = QStringList(), QString workdir = "", QStringList env = QStringList() ); 
	// - success output only
	static bool RunQuickCommand(QString command, QStringList arguments = QStringList(), QString workdir = "", QStringList env = QStringList() ); 
	
	//File Access Functions
	static QStringList readTextFile(QString filename);
	static bool writeTextFile(QString filename, QStringList contents, bool overwrite = true);
	
	//Retrieve a text-based sysctl
	static QString sysctl(QString var);
	//Retrieve a number-based sysctl
	static long long sysctlAsInt(QString var);
};
	
} //end of pcbsd namespace

#endif