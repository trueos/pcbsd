//===========================================
//  Lumina-DE source code
//  Copyright (c) 2014, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
//  This class is a generic invisible spacer for the panel
//===========================================
#ifndef _LUMINA_DESKTOP_PANEL_PLUGIN_SPACER_H
#define _LUMINA_DESKTOP_PANEL_PLUGIN_SPACER_H

#include "../LPPlugin.h"

class LSpacerPlugin : public LPPlugin{
	Q_OBJECT
public:
	LSpacerPlugin(QWidget *parent=0) : LPPlugin(parent, "spacer"){
	  this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	}
	~LSpacerPlugin(){}
};


#endif