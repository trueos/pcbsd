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
	//Scan for mounted external devices
	static QStringList ExternalDevicePaths(); //Returns: QStringList[<type>::::<filesystem>::::<path>]
	  //Note: <type> = [USB, HDRIVE, DVD, SDCARD, UNKNOWN]

	//Read screen brightness information
	static int ScreenBrightness(); //Returns: Screen Brightness as a percentage (0-100, with -1 for errors)
	//Set screen brightness
	static void setScreenBrightness(int percent);

	//Read the current volume
	static int audioVolume(); //Returns: audio volume as a percentage (0-100, with -1 for errors)
	//Set the current volume
	static void setAudioVolume(int percent);
	//Modify the current volume by a set amount (+ or -)
	static void changeAudioVolume(int percentdiff);
};

#endif
