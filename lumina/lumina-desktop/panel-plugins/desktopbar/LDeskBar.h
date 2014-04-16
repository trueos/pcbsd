//===========================================
//  Lumina-DE source code
//  Copyright (c) 2012, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
//  This plugin displays the contents of the user's home directory 
//    as organized within a couple buttons on the panel (apps, dirs, files)
//===========================================
#ifndef _LUMINA_DESKTOP_DESKBAR_H
#define _LUMINA_DESKTOP_DESKBAR_H

// Qt includes
#include <QWidget>
#include <QString>
#include <QAction>
#include <QMenu>
#include <QProcess>
#include <QTimer>
#include <QFileSystemWatcher>
#include <QHBoxLayout>
#include <QIcon>
#include <QToolBar>
#include <QDebug>

// libLumina includes
#include <LuminaXDG.h>

// local includes
#include "../LTBWidget.h"
#include "../LPPlugin.h"

class LDeskBarPlugin : public LPPlugin{
	Q_OBJECT
public:
	LDeskBarPlugin(QWidget* parent=0);
	~LDeskBarPlugin();
	
private:
	//QHBoxLayout *layout;
	QString desktopPath;
	QFileSystemWatcher *watcher;
	//Special toolbuttons and menus
	LTBWidget *appB, *fileB, *dirB;
	QMenu *appM, *dirM, *audioM, *videoM, *pictureM, *fileM, *otherM;
	QStringList audioFilter, videoFilter, pictureFilter;
	QFileInfoList totals;
	//QList<QToolButton*> exeList;
	
	void initializeDesktop();
	//bool readDesktopFile(QString path, QString &name, QString &iconpath);
	
	QAction* newAction(QString filepath, QString name, QString iconpath);
	QAction* newAction(QString filepath, QString name, QIcon icon);
	//QToolButton* newLauncher(QString filepath, QString name, QString iconpath);
	void updateMenu(QMenu* menu, QFileInfoList files, bool trim = true);
	//bool searchForExe(QString filename);
	
private slots:
	void ActionTriggered(QAction* act);
	void desktopChanged();
	
};


#endif

