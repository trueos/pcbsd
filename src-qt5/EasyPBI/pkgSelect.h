#ifndef _EASYPBI_PKG_SELECT_H
#define _EASYPBI_PKG_SELECT_H

#include <QString>
#include <QStringList>
#include <QDialog>
#include <QTimer>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QMessageBox>

#include "backend.h"

namespace Ui {
    class pkgSelect;
}

class pkgSelect : public QDialog{
	Q_OBJECT
	
public:
	pkgSelect(QWidget *parent = 0, bool single = true);
	~pkgSelect();
	//outputs
	QString portSelected; //if single selection only allowed
	QStringList portsSelected; //if multiple selections allowed
	bool selected;

private:
	Ui::pkgSelect *ui;
	QTimer *timer;
	bool singleSelection;

	void loadPackageList();

	bool performSearch(QString, QTreeWidget*, QTreeWidgetItem*);
	bool searchChildren(QString, QTreeWidget*, QTreeWidgetItem*, bool&, QTreeWidgetItem*);
	
private slots:
	void slotCheckPkg();
	void slotSearch();
	void slotAccept();
	void slotCancel();
};

#endif
