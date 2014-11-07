#ifndef _APPCAFE_UPDATE_DIALOG_H
#define _APPCAFE_UPDATE_DIALOG_H

#include <QDialog>
#include <QString>
#include <QCloseEvent>

#include <pcbsd-DLProcess.h>

namespace Ui{
	class UpdateDialog;
};

class UpdateDialog : public QDialog{
	Q_OBJECT
public:
	UpdateDialog(QWidget *parent = 0, QString jailID = "");
	~UpdateDialog();

	bool rebooting;

private:
	Ui::UpdateDialog *ui;
	DLProcess *proc;
	bool installphase;

private slots:
	void procMessage(QString);
	void procUpdate(QString, QString, QString);
	void procFinished(int, QProcess::ExitStatus);
	//void logview();
	void rebootsystem();
	void closeDialog();

protected:
	void closeEvent(QCloseEvent *event){
	  if(proc->isRunning()){
	    event->ignore(); //do not close
	     this->showMinimized(); //just minimize this
	  }else{
	    event->accept(); //continue closing
	  }
	}
};
#endif