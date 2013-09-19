#ifndef _LP_MAIN_H
#define _LP_MAIN_H

#include <QMainWindow>
#include <QComboBox>
#include <QToolButton>
#include <QLabel>
#include <QString>
#include <QStringList>
#include <QRadioButton>

#include "LPBackend.h"
#include "LPContainers.h"

namespace Ui{
	class LPMain;
};

class LPMain : public QMainWindow{
	Q_OBJECT
public:
	LPMain(QWidget *parent = 0);
	~LPMain();
	
public slots:
	void setupUI();

private:
	Ui::LPMain *ui;
	QRadioButton *viewBasic, *viewAdvanced;
	bool poolSelected;
	LPDataset POOLDATA;

	void updateInterface(); //update interface based on selected pool
	LPDataset loadPoolData(QString pool);

private slots:
	void viewChanged();
	
protected:
	
};

#endif