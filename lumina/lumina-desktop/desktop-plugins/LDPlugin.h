//===========================================
//  Lumina-DE source code
//  Copyright (c) 2014, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
//  This class is the generic container layout for all desktop plugins
//  Simply subclass this when creating a new plugin to enable correct
//    visibility and usage within the desktop window
//===========================================
#ifndef _LUMINA_DESKTOP_DESKTOP_PLUGIN_H
#define _LUMINA_DESKTOP_DESKTOP_PLUGIN_H

#include <QObject>
#include <QWidget>
#include <QString>

class LDPlugin : public QWidget{
	Q_OBJECT
	
private:
	QString plugintype;

public:
	LDPlugin(QWidget *parent = 0, QString ptype="unknown") : QWidget(parent){
	  plugintype=ptype;
	  this->setStyleSheet( "LDPlugin{ background-color: transparent; }" );
	}
	
	~LDPlugin(){
	}
	
	QString type(){
	  return plugintype;
	}
	
public slots:
	void LocaleChange(){
	  //This needs to be re-implemented in the subclassed plugin
	    //This is where all text is set/translated
	}
	void ThemeChange(){
	  //This needs to be re-implemented in the subclasses plugin
	    //This is where all the visuals are set if using Theme-dependant icons.
	}
	
};

#endif