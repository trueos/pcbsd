#ifndef _CMD_DIALOG_H
#define _CMD_DIALOG_H

#include <QDialog>
#include <QString>
#include <QWidget>

#include "gitprogress.h"

namespace Ui{
	class CMDDialog;
};

class CMDDialog : public QDialog{
	Q_OBJECT
public:
	CMDDialog(QWidget *parent = 0);
	~CMDDialog();

	void start(QString cmdtype);

private:
	Ui::CMDDialog *ui;
	gitProc *PROC;
	bool initPorts, initSource; //to keep track if only the first step was running

private slots:
	void NewMessage(QString);
	void Finished(bool);
	void CloseUI();
};
#endif
