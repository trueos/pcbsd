#ifndef _LUMINA_CONFIG_MAIN_UI_H
#define _LUMINA_CONFIG_MAIN_UI_H

// Qt includes
#include <QMainWindow>
#include <QDir>
#include <QDesktopWidget>
#include <QSettings>
#include <QStringList>
#include <QString>
#include <QIcon>
#include <QTimer>
#include <QFileDialog>

// libLumina includes
#include <LuminaXDG.h>

// local includes
#include "LPlugins.h"

#define DEFAULTBG QString("/usr/local/share/Lumina-DE/desktop-background.jpg")

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
	QSettings *settings;
	QDesktopWidget *desktop;
	LPlugins *PINFO;
	QMenu *ppmenu;

	//General purpose functions (not connected to buttons)
	void setupIcons(); //called during initialization
	void setupMenus(); //called during initialization
	void checkForChanges(); //to see whether to enable the save button
	int currentDesktop(); //the number for the current desktop
	int currentPanel(); //the number for the currently selected panel
	void addNewBackgroundFile(QString); //simplification for showing the BG preview

private slots:
	void slotSingleInstance();

	//General Utility Functions
	void loadCurrentSettings();
	void saveCurrentSettings();

	//Background Tab Functions
	void addBackground();
	void removeBackground();
	void enableBGRotateTime(bool);

	//Toolbar Tab Functions
	void loadPanelSettings();
	void savePanelSettings();
	void colorChanged();
	void addPPlugin(QAction*);
	void rmPPlugin();
	void mvLPPlugin();
	void mvRPPlugin();

};

#endif