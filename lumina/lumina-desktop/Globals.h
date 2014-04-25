//===========================================
//  Lumina-DE source code
//  Copyright (c) 2012, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#ifndef _LUMINA_DESKTOP_GLOBALS_H
#define _LUMINA_DESKTOP_GLOBALS_H

#include <unistd.h>

class Lumina{
public:
  enum STATES {NONE, VISIBLE, INVISIBLE, ACTIVE, NOTIFICATION, NOSHOW};

};

class SYSTEM{
public:
	//Current Username
	static QString user(){ return QString::fromLocal8Bit(getlogin()); }
	//Current Hostname
	static QString hostname(){ 
	  char name[64];
	  int count = gethostname(name,sizeof(name));
	  if (count < 0) {
	    return QString::null;
	  }
	  return QString::fromLocal8Bit(name,count);
	}
	//Shutdown the system
	static void shutdown(){ system("(shutdown -p now) &"); }
	//Restart the system
	static void restart(){ system("(shutdown -r now) &"); }
};

#endif
