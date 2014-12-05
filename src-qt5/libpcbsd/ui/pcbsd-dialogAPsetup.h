#ifndef _PCBSD_WIRELESS_ACCESS_POINT_CONFIG_DIALOG_H
#define _PCBSD_WIRELESS_ACCESS_POINT_CONFIG_DIALOG_H

#include <QDialog>
#include <QString>
#include <QObject>
#include <QMessageBox>

namespace Ui{
	class APSetupDialog;
};

class APSetupDialog : public QDialog{
	Q_OBJECT
public:
	APSetupDialog(QString devicenode, QWidget *parent=0);
	~APSetupDialog();

	bool cancelled;

private:
	Ui::APSetupDialog *ui;
	QString wdev;

private slots:
	void cancelDialog(){
	  cancelled=true;
	  this->close();
	}
	void applyDialog();
	

};

#endif