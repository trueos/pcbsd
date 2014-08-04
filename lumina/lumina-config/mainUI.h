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
#include <QFile>
#include <QMessageBox>
#include <QColorDialog>
#include <QColor>
#include <QInputDialog>
#include <QListWidgetItem>

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
	QMenu *ppmenu, *mpmenu;
	QString panelcolor;
	QList<XDGDesktop> sysApps;
	bool loading;
	int panelnumber;

	//General purpose functions (not connected to buttons)
	void setupIcons(); //called during initialization
	void setupMenus(); //called during initialization
	void setupConnections(); //called during intialization

	int currentDesktop(); //the number for the current desktop

	//Panels Page simplifications
	QString getColorStyle(QString current);
	QString getNewPanelPlugin();
	
private slots:
	void slotSingleInstance();

	//General UI Behavior
	void slotChangePage(bool enabled);
	void slotChangeScreen();
	void saveAndQuit();

	//General Utility Functions
	void loadCurrentSettings();
	void saveCurrentSettings();

	//Desktop Page
	void deskplugchanged();
	void deskbgchanged();
	void desktimechanged();
	void deskbgremoved();
	void deskbgadded();
	void deskplugadded();


	//Panels Page
	void addpanel1();
	void addpanel2();
	void rmpanel1();
	void rmpanel2();
	
	void checkpanels();
	void adjustpanel1();
	void adjustpanel2();
	
	void getpanel1color();
	void getpanel2color();
	void addpanel1plugin();
	void addpanel2plugin();
	void rmpanel1plugin();
	void rmpanel2plugin();
	void uppanel1plugin();
	void uppanel2plugin();
	void dnpanel1plugin();
	void dnpanel2plugin();
	
	
	//Background Tab Functions
	/*void addBackground();
	void removeBackground();
	void enableBGRotateTime(bool);

	//Toolbar Tab Functions
	void loadPanelSettings();
	void savePanelSettings();
	void getNewColor();
	void colorChanged();
	void addPPlugin(QAction*);
	void rmPPlugin();
	void mvLPPlugin();
	void mvRPPlugin();

	//Menu Tab Functions
	void loadMenuSettings();
	void saveMenuSettings();
	void findTerminalBinary();
	void addMenuItem(QAction*);
	void rmMenuItem();
	void upMenuItem();
	void downMenuItem();*/

};

#endif