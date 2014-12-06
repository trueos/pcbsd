#ifndef _FS_DIALOG_H
#define _FS_DIALOG_H

#include <QDialog>
#include <QString>
#include <QStringList>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QBrush>
#include <QColor>
#include <QIcon>

#include "fsWatcher.h"

class FSDialog : public QDialog{
	Q_OBJECT
	
public:
	FSDialog();
	~FSDialog();
	
private slots:
	void generateUI();	
	void closeDialog();

};

#endif
