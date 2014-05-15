//===========================================
//  Lumina-DE source code
//  Copyright (c) 2014, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
//  This class is the interface to load all the different desktop plugins
//===========================================
#ifndef _LUMINA_DESKTOP_NEW_DESKTOP_PLUGIN_H
#define _LUMINA_DESKTOP_NEW_DESKTOP_PLUGIN_H

#include <QDebug>

//List all the individual plugin includes here
#include "LDPlugin.h"
#include "desktopview/DesktopViewPlugin.h"


class NewDP{
public:
	static LDPlugin* createPlugin(QString plugin, QWidget* parent=0){
	  LDPlugin *plug = 0;
	  if(plugin=="desktopview"){
	    plug = new DesktopViewPlugin(parent);
	  }else{
	    qWarning() << "Invalid Desktop Plugin:"<<plugin << " -- Ignored";
	  }
	  return plug;
	}

};

#endif