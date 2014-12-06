//===========================================
//  PC-BSD source code
//  Ken Moore  <ken@pcbsd.org
//  Copied from the Lumina Library Source code: 12/5/14
//===========================================
// This is the general class for a single-instance application
//===========================================
//EXAMPLE USAGE in main.cpp:
//
// LSingleApplication app(argc, argv);
// if( !app.isPrimaryProcess() ){
//    return 0;
//  }
//  QMainWindow w; //or whatever the main window class is
//  connect(app, SIGNAL(InputsAvailable(QStringList)), w, SLOT(<some slot>)); //for interactive apps - optional
//  app.exec();
//===========================================
#ifndef _PCBSD_LIBRARY_SINGLE_APPLICATION_H
#define _PCBSD_LIBRARY_SINGLE_APPLICATION_H

#include <QApplication>
#include <QString>
#include <QStringList>
#include <QLocalServer>
#include <QLockFile>


class PCSingleApplication : public QApplication{
  Q_OBJECT
public:
	PCSingleApplication(int &argc, char **argv);
	~PCSingleApplication();

	bool isPrimaryProcess();

private:
	bool isActive;
	QLockFile *lockfile;
	QLocalServer *lserver;
	QString cfile;
	QStringList inputlist;

	void PerformLockChecks();

private slots:
	void newInputsAvailable(); //internally used to detect a message from an alternate instance

signals:
	void InputsAvailable(QStringList);

};

#endif
