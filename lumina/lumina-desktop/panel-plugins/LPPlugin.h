//===========================================
//  Lumina-DE source code
//  Copyright (c) 2014, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
//  This class is the generic container layout for all panel plugins
//  Simply subclass this when creating a new plugin to enable correct
//    visibility and usage within a panel
//===========================================
#ifndef _LUMINA_DESKTOP_PANEL_PLUGIN_H
#define _LUMINA_DESKTOP_PANEL_PLUGIN_H

#include <QObject>
#include <QWidget>
#include <QString>
#include <QHBoxLayout>

class LPPlugin : public QWidget{
	Q_OBJECT
	
private:
	QHBoxLayout *LY;
	QString plugintype;

public:
	LPPlugin(QWidget *parent = 0, QString ptype="unknown") : QWidget(parent){
	  plugintype=ptype;
	  this->setContentsMargins(0,0,0,0);
	  this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	  LY = new QHBoxLayout(this);
	  LY->setContentsMargins(0,0,0,0);
	  LY->setSpacing(1);
	  this->setLayout(LY);
	}
	
	~LPPlugin(){
	}
	
	QHBoxLayout* layout(){
	  return LY;
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