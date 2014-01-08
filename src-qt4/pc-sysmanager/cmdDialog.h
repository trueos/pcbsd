#ifndef _CMD_DIALOG_H
#define _CMD_DIALOG_H

#include <QDialog>
#include <QString>
#include <QWidget>
#include <QMessageBox>

#include "longProc.h"
#include "backend.h"

namespace Ui{
	class CMDDialog;
};

class CMDDialog : public QDialog{
	Q_OBJECT
public:
	CMDDialog(QWidget *parent = 0);
	~CMDDialog();

	void start(QString cmdtype, QString branch = "");
	bool isRunning();

private:
	Ui::CMDDialog *ui;
	LongProc *PROC;

private slots:
	void NewMessage(QString);
	void Finished();
	void CloseUI();
	void stopProcess();

protected:
	void closeEvent(QCloseEvent *event);
};
#endif
