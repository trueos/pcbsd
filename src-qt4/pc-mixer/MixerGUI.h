#ifndef _PC_MIXER_GUI_H
#define _PC_MIXER_GUI_H

#include <QMainWindow>
#include <QSettings>
#include <QCloseEvent>
#include <QCoreApplication>

#include "MixerBackend.h"

namespace Ui{
	class MixerGUI;
};

class MixerGUI : public QMainWindow{
	Q_OBJECT
public:
	MixerGUI(QSettings*);
	~MixerGUI();

	void updateGUI(); //For the tray to call before the GUI becomes active

private:
	Ui::MixerGUI *ui;
	QSettings *settings;
	bool closing;

private slots:
	void hideGUI(){
	  this->hide();
	}
	void closeApplication(){
	  closing = true;
	  this->close();
	}
	
	void changeDefaultTrayDevice(QString device);
	void itemChanged(QString device); //for individual device adjustments 

protected:
	void closeEvent(QCloseEvent *event){
	  if(!closing){
	    event->ignore();
	    hideGUI();
	  }
	}
	
signals:
	void updateTray();
	
};

#endif
