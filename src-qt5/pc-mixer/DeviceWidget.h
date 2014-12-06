#ifndef _MIXER_DEVICE_WIDGET_H
#define _MIXER_DEVICE_WIDGET_H

#include <QWidget>
#include <QString>

#include "MixerBackend.h"

namespace Ui{
	class DeviceWidget;
};

class DeviceWidget : public QWidget{
	Q_OBJECT
public:
	DeviceWidget(QWidget *parent = 0);
	~DeviceWidget();

	void setupDevice(QString device, int Lvol, int Rvol);
	
	QString device();
	int LVolume();
	int RVolume();

private:
	Ui::DeviceWidget *ui;
	int CRV, CLV; //Current R/L volume (in case it was muted)
	bool changing, ismuted, islinked; // internal status flags

	void updateVolumes(int, int);
	void updateButtons();

private slots:
	void muteClicked();
	void linkClicked();
	void LSliderChanged(int);
	void RSliderChanged(int);
	
signals:
	void deviceChanged(QString);
	
};

#endif
