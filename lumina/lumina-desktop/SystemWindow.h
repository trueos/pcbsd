#ifndef _LUMINA_DESKTOP_SYSTEM_WINDOW_H
#define _LUMINA_DESKTOP_SYSTEM_WINDOW_H

#include <QDialog>
#include <QCoreApplication>
#include <QDesktopWidget>
#include <QList>

#include "ui_SystemWindow.h"
#include "Globals.h"

#include <LuminaXDG.h>
#include <LuminaX11.h>



namespace Ui{
	class SystemWindow;
};

class SystemWindow : public QDialog{
	Q_OBJECT
public:
	SystemWindow();
	~SystemWindow();

private:
	Ui::SystemWindow *ui;

	void closeAllWindows(){
	  QList<WId> WL = LX11::WindowList();
	  for(int i=0; i<WL.length(); i++){
	    LX11::CloseWindow(WL[i]);
	  }
	}
	
private slots:
	void sysLogout(){
	  closeAllWindows();
	  QCoreApplication::exit(0);
	}
	
	void sysRestart(){
	  closeAllWindows();
	  SYSTEM::restart();
	  QCoreApplication::exit(0);		
	}
	
	void sysShutdown(){
	  closeAllWindows();
	  SYSTEM::shutdown();
	  QCoreApplication::exit(0);
	}
	
	void sysCancel(){
	  this->close();
	}
};

#endif