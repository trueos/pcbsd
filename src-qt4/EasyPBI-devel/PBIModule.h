#ifndef _EASYPBI_PBI_MODULE_H
#define _EASYPBI_PBI_MODULE_H

#include <QHash>
#include <QString>
#include <QStringList>
#include <QVariant>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QDebug>

class PBIModule{
	
public:
	PBIModule();
	~PBIModule();

	//Initial read
	bool loadModule(QString); //must give it the absolute path to a "pbi.conf" file

	//Module directory path
	QString basepath();

	//pbi.conf Values
	QStringList textL(QStringList); //a list of individual QString values
	void setTextL(QStringList, QStringList); //set a list of QString values
	QString text(QString); //a single QString value
	void setText(QString,QString); //set a single QString value
	QStringList textValues(); //list all the possible values

	bool isEnabled(QString); 
	void setEnabled(QString, bool);
	QStringList enableValues(); //list all the possible enables

	int number(QString);
	void setNumber(QString,int);
	QStringList numberValues(); //list all possible integer values

	void loadConfig();
	bool saveConfig();
	
	//Scripts
	QStringList readScript(QString);
	bool writeScript(QString,QStringList);
	QStringList validScripts();
	QStringList existingScripts();
	
	//Resources
	QStringList existingResources();
	bool addResource(QString filePath, QString resourcePath = "");
	
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
	
	
	//External Links
	void loadExternalLinks( QStringList& bin, QStringList& link, QStringList& type);
	bool saveExternalLinks( QStringList bin, QStringList link, QStringList type);

	//General Utilities
	static bool createFile(QString,QStringList);
	static QStringList readFile(QString);
	static QStringList filesInDir(QString);
	void clearXdgData();
	void clearMimeData();
	
private:
	QHash<QString, QVariant> HASH;
	QStringList CTextValues, CBoolValues, CIntValues, scriptValues, mimeValues;
	QStringList xdgTextValues, xdgBoolValues;
	QString basePath, version;

};

#endif
