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
	static QString user(){ return QString(getlogin()); }
	//Current Hostname
	static QString hostname(){ 
	  char name[50];
	  gethostname(name,sizeof(name));
	  return QString(name);
	}
	//Shutdown the system
	static void shutdown(){ system("(shutdown -p now) &"); }
	//Restart the system
	static void restart(){ system("(shutdown -r now) &"); }
};

#endif
