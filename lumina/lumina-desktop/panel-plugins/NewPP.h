//===========================================
//  Lumina-DE source code
//  Copyright (c) 2014, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
//  This class is the interface to load all the different panel plugins
//===========================================
#ifndef _LUMINA_DESKTOP_NEW_PANEL_PLUGIN_H
#define _LUMINA_DESKTOP_NEW_PANEL_PLUGIN_H

#include <QDebug>

//List all the individual plugin includes here
#include "LPPlugin.h"
#include "userbutton/LUserButton.h"
#include "desktopbar/LDeskBar.h"
#include "spacer/LSpacer.h"
#include "clock/LClock.h"
#include "taskmanager/LTaskManagerPlugin.h"
#include "systemtray/LSysTray.h" //must be last due to X11 compile issues

class NewPP{
public:
	static LPPlugin* createPlugin(QString plugin, QWidget* parent = 0){
	  LPPlugin *plug = 0;
	  if(plugin=="userbutton"){
	    plug = new LUserButtonPlugin(parent);
	  }else if(plugin=="desktopbar"){
	    plug = new LDeskBarPlugin(parent);
	  }else if(plugin=="spacer"){
	    plug = new LSpacerPlugin(parent);
	  }else if(plugin=="taskmanager"){
	    plug = new LTaskManagerPlugin(parent);
	  }else if(plugin=="systemtray"){
	    plug = new LSysTray(parent);
	  }else if(plugin=="clock"){
	    plug = new LClock(parent);
	  }else{
	    qWarning() << "Invalid Panel Plugin:"<<plugin << " -- Ignored";
	  }
	  return plug;
	}

};

#endif