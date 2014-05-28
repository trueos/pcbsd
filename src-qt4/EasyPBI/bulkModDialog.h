#ifndef _EASYPBI_BULK_MODULE_DIALOG_H
#define _EASYPBI_BULK_MODULE_DIALOG_H

#include <QDialog>
#include <QTimer>
#include <QMenu>
#include <QAction>
#include <QString>
#include <QStringList>
#include <QDir>
#include <QFileDialog>

#include "backend.h"
#include "ModuleUtils.h"

namespace Ui{
	class BulkModuleDialog;
};

class BulkModuleDialog : public QDialog{
	Q_OBJECT
public:
	BulkModuleDialog(QWidget *parent = 0);
	~BulkModuleDialog();

private:
	Ui::BulkModuleDialog *ui;
	QMenu *catMenu;
	QStringList pkgList;

	//Module Creation variables
	int skipped, invalid, gnew, tnew, snew, onew;
	bool isWorking, stopProc;

private slots:
	void updateUI(); 	//Update UI buttons
	void startWorking(); 	//Should be started with a QTimer::singleShot() or signal/slot
	void stopWorking(); 	//Set the flag to stop running
	void closeWindow();	//Close this dialog

	void selectBaseDir();
	void selectPkgCategory(QAction*);
};

#endif