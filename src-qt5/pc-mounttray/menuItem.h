/*
 Sub-classed widget for a fancy item selector
*/

#ifndef MENU_ITEM_H
#define MENU_ITEM_H

#include <QWidgetAction>
#include <QDir>
#include <QFile>
#include <QPushButton>
#include <QProgressBar>
#include <QGridLayout>
#include <QCheckBox>
#include <QDebug>
#include <QLabel>
#include <QTimer>
#include <QPainter>
#include <QPixmap>
#include <QInputDialog>

#include "devCheck.h"

extern bool DEBUG_MODE;
extern QString DEVICEDIR;
extern QString MOUNTDIR;
//extern QString USERNAME;

class MenuItem : public QWidgetAction
{
	Q_OBJECT

  public:
	MenuItem(QWidget* parent = 0, DevCheck *chk = 0, QString newdevice="", QString newlabel="", QString newtype="", QString newfs="");
	~MenuItem();
	
	QString device;
  	QString devType;
  	QString filesystem;
        QString mountpoint;
        QString maxSize;  //number in KB saved as a QString
        QString currentSize; //number in KB saved as a QString
        QString AMFILE;
  	//QString currentUser; //username of current user (for setting mount dir ownership)
	
  	//Setters
	void updateItem();
	void mountItem(); //be careful calling this on it's own
  
	//Getters
	QString getDeviceName();
	
	//Device information
	bool isConnected();
	bool isMounted();
	
	//Device Cleanup
	void cleanup();
	
  private:
	QWidget *frame;
  	QLabel* devLabel;
  	QLabel* devIcon;
  	QProgressBar* currentSpace;
  	QPushButton* pushMount;
  	QCheckBox* checkAutomount;
	QPixmap baseicon;
	DevCheck *DEVCHECK;
	bool mountedHere; //Whether this utility is the one that mounted the device
	bool rootRequired, oldroot;
  
  	void unmountItem(bool force = false);
        void updateSizes();
        QString getSizeDisplay(int);
        bool checkSavedAutoMount();
        QStringList systemCMD(QString);
	QString createRunScript(QString mntpoint, QString mntcmd);
	QString createRemoveScript(QString mntpoint, bool force);
  
	bool umount(bool force, QString mntpoint);
  	
  private slots:
  	void slotMountClicked();
	void slotAutoMount();
        void slotAutoMountToggled(bool);
	
  signals:
	//Emits these signals whenever needed
	void itemMounted(QString);  	//device node (/dev/da*)
	void itemUnmounted(QString); 	//device node
	void itemRemoved(QString);  	//device node
        void newMessage(QString, QString);	//message to be displayed
	void itemWorking();
	void openAVDisk(QString); //device type (AUDIO, VIDEO)

};
#endif
