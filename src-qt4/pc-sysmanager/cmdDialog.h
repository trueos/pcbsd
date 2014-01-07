#ifndef _CMD_DIALOG_H
#define _CMD_DIALOG_H

#include <QDialog>
#include <QString>
#include <QWidget>

#include "longProc.h"

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
	LongProc *PROC;

private slots:
	void NewMessage(QString);
	void Finished();
	void CloseUI();
};
#endif
