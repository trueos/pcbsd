#ifndef _LUMINA_CONFIG_MAIN_UI_H
#define _LUMINA_CONFIG_MAIN_UI_H

#include <QMainWindow>

//namespace for using the *.ui file
namespace Ui{
	class MainUI;
};

class MainUI : public QMainWindow{
	Q_OBJECT
public:
	MainUI();
	~MainUI();

private:
	Ui::MainUI *ui; //the *.ui file access

private slots:
	void slotSingleInstance();

};

#endif