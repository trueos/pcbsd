#ifndef _PCBSD_MOUNT_TRAY_DEVICE_WIDGET_H
#define _PCBSD_MOUNT_TRAY_DEVICE_WIDGET_H

#include <QWidget>
#include <QString>
#include <QObject>
#include <QWidgetAction>

namespace Ui{
  class DeviceWidget;
};

class DeviceWidget : public QWidget{
	Q_OBJECT
public:
	DeviceWidget(QString devicenode);
	~DeviceWidget();

	QString node(); //Get the device ID that is managed by this widget
	QString type(); //Get the device type 
	QWidgetAction* action(); //Simplification for adding a widget to a menu (QMenu->addAction(DeviceWidget->action())

	void UpdateDevice(bool ismounted); //Full update of the widget
	void QuickUpdate(bool ismounted); //Just update status & item visibility (fast)

private:
	Ui::DeviceWidget *ui; //Designer form
	bool isMounted, quickupdates, canmount;
	QWidgetAction *WAct;

private slots:
	void doUpdate();
	void changeAutoMount(); //auto-mount option changed
	void mountButtonClicked(); //mount/unmount the device (based on current status)
	void runButtonClicked(); //Run the device (audio/video CD, browse filesystem)
	void OpenTrayClicked();  //Open the CD tray

signals:
	void closeMenu();
	void ShowMessage(QString title, QString contents);
	void OpenDevice(QString nodedir, QString type); //device/directory to open, and the type of device it is
	void RefreshDeviceList(); //this is usually if the device was modified by the gui directly (such as opening the CD tray)
	
};

#endif