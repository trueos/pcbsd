#ifndef _PC_MIXER_GUI_H
#define _PC_MIXER_GUI_H

#include <QMainWindow>
#include <QSettings>
#include <QCloseEvent>
#include <QCoreApplication>
#include <QHBoxLayout>
#include <QScrollBar>
#include <QProcess>
#include <QAction>

#include "DeviceWidget.h"
#include "MixerBackend.h"

namespace Ui{
	class MixerGUI;
};

class MixerGUI : public QMainWindow{
	Q_OBJECT
public:
	MixerGUI(QSettings* set = 0);
	~MixerGUI();

	void updateGUI(); //For the tray to call before the GUI becomes active

private:
	Ui::MixerGUI *ui;
	QSettings *settings;
	bool closing;

private slots:
	void hideGUI(){
	  if(settings==0){ this->close(); } //no tray
	  else{ this->hide(); } //tray
	}
	void closeApplication(){
	  closing = true;
	  this->close();
	}
	
	void startExternalApp(QAction *act){
	  QProcess::startDetached(act->whatsThis());
	}
	
	void changeDefaultTrayDevice(QString device);
	void itemChanged(QString device); //for individual device adjustments 

	void slotSingleInstance(){
	  updateGUI();
	  this->show();
	}
protected:
	void closeEvent(QCloseEvent *event){
	  if(!closing && settings!=0){
	    //tray running - just hide it
	    event->ignore();
	    hideGUI();
	  }
	}
	
signals:
	void updateTray();
	
};

#endif
