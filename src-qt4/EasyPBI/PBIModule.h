#ifndef _EASYPBI_PBI_MODULE_H
#define _EASYPBI_PBI_MODULE_H

#include <QHash>
#include <QString>
#include <QStringList>
#include <QVariant>
#include <QIcon>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QDebug>

#include "SConfigDialog.h"

class PBIModule{
	
public:
	PBIModule();
	~PBIModule();

	//Initial read
	bool loadModule(QString); //must give it the absolute path to a "pbi.conf" file

	//Module directory path
	QString basepath();

	//pbi.conf Values
	QString stringVal(QString);
	void setStringVal(QString, QString);
	QStringList listVal(QString);
	void setListVal(QString, QStringList);

	QIcon appIcon();
	bool setAppIcon(QString icopath);

	void loadConfig();
	bool saveConfig();
	
	//Scripts
	QStringList readScript(QString);
	bool removeScript(QString);
	bool writeScript(QString,QStringList);
	QStringList validScripts();
	QStringList existingScripts();
	
	//XDG files
	QStringList validXdgText();
	QString xdgText(QString);
	QStringList xdgTextL(QStringList);
	void setXdgText(QString,QString);
	void setXdgTextL(QStringList, QStringList);
	
	QStringList validXdgEnables();
	bool xdgEnabled(QString);
	void setXdgEnabled(QString, bool);
	
	QStringList listXdgDesktopFiles();
	QStringList listXdgMenuFiles();
	bool saveXdgDesktop(QString fileName);
	bool saveXdgMenu(QString fileName);
	bool removeXdgDesktop(QString fileName);
	bool removeXdgMenu(QString fileName);
	bool loadXdgDesktop(QString fileName);
	bool loadXdgMenu(QString filename);
	
	QStringList listMimeFiles();
	QStringList validMimeValues();
	QString mimeText(QString);
	QStringList mimeTextL(QStringList);
	void setMimeText(QString, QString);
	void setMimeTextL(QStringList, QStringList);
	bool saveMimeFile(QString fileName);
	bool removeMimeFile(QString fileName);
	bool loadMimeFile(QString fileName);
	
	//Service configuration file
	bool saveServiceConfig();
	void loadServiceConfig();
	QList<ServiceOption>  ServiceOptions;
	
	//General Utilities
	static bool createFile(QString,QStringList);
	static QStringList readFile(QString);
	static QStringList filesInDir(QString);
	void clearXdgData();
	void clearMimeData();
	
private:
	QHash<QString, QVariant> HASH;
	QStringList OldValues, CTextValues, CListValues, scriptValues, mimeValues, serviceScripts;
	QStringList xdgTextValues, xdgBoolValues;
	QString basePath, version;

};

#endif
