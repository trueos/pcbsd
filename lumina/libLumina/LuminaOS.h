//===========================================
//  Lumina-DE source code
//  Copyright (c) 2014, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#ifndef _LUMINA_LIBRARY_OS_H
#define _LUMINA_LIBRARY_OS_H

#include <QString>
#include <QStringList>

#include "LuminaUtils.h"

class LOS{
public:
	//Scan for external devices
	QStringList ExternalDevicePaths(); //Returns: QStringList[<type>::::<filesystem>::::<path>]
	  //Note: <type> = [USB, HDRIVE, DVD, SDCARD, UNKNOWN]
};

#endif
