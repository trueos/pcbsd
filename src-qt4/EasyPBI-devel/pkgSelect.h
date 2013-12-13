#ifndef _EASYPBI_PKG_SELECT_H
#define _EASYPBI_PKG_SELECT_H

#include <QString>
#include <QStringList>
#include <QDialog>
#include <QTreeWidget>
#include <QTreeWidgetItem>

#include "backend.h"

namespace Ui {
    class pkgSelect;
}

class pkgSelect : public QDialog{
	Q_OBJECT
	
public:
	pkgSelect();
	~pkgSelect();
	//outputs
	QString portSelected
	bool selected;

private:
	Ui::pkgSelect *ui;
	QTimer *timer;

	void loadPackageList();

private slots:
	void slotSearch();
	void slotAccept();
	void slotCancel();
};

#endif