#ifndef _PCBSD_APPCAFE_MIGRATE_UI_H
#define _PCBSD_APPCAFE_MIGRATE_UI_H

#include <QMainWindow>
#include <QProcess>
#include <QString>
#include <QCloseEvent>

#include <pcbsd-DLProcess.h>

#include "ui_migrateUI.h"

namespace Ui{
	class MigrateUI;
};

class MigrateUI : public QMainWindow{
	Q_OBJECT
public:
	MigrateUI();
	~MigrateUI(){}

private:
	Ui::MigrateUI *ui;
	DLProcess *proc;
        QString ConflictList;

private slots:
	void slotSingleInstance();
	void closeWindow();
	void startMigration();
	void procFinished();
	void updateProgress(QString);
	void updatePercent(QString,QString,QString);
	void restartSystem();
	QString getConflictDetailText();

protected:
	void closeEvent(QCloseEvent *event){
	  if(proc->isRunning()){
	    //Dangerous to stop the process while it is running - just minimize it
	    event->ignore();
	    this->showMinimized();
	  }else{
	    event->accept();
	    this->close();
	  }
	}

};

#endif
