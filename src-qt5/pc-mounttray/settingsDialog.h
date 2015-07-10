#ifndef _SETTINGS_DIALOG_H
#define _SETTINGS_DIALOG_H

#include <QDialog>
#include <QString>
#include <QStringList>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QGroupBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QLabel>
#include <QIcon>
#include <QDebug>

class SettingsDialog : public QDialog{
	Q_OBJECT
	
public:
	SettingsDialog();
	~SettingsDialog();
	
	bool SettingsChanged; //if settings should be applied (I.E. changed)
	bool useDiskWatcher; //use system disk space monitor
	  bool useDiskAutoTimer; //use devd prompting to check system
	  int diskRefreshMS; //max time between checks (milliseconds)
	bool useAutoPlay; //Automatically start playing audio/video CD/DVDs
	  
	  
	void showDialog();
	
private:
	//Disk Watcher settings
	QGroupBox *groupDiskWatch;
	QCheckBox *checkDiskAutoTimer, *checkAutoPlay;
	QSpinBox *spinDiskRefreshMin;
	//save/close buttons
	QPushButton *closeButton,  *applyButton;
	
private slots:
	void slotApply();
	void slotCloseDialog();
	
	void slotUpdateUI();

};

#endif
