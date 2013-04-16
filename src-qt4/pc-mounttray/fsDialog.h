#ifndef _FS_DIALOG_H
#define _FS_DIALOG_H

#include <QDialog>
#include <QString>
#include <QStringList>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QLabel>
#include <QProgressBar>
#include <QWidget>

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
