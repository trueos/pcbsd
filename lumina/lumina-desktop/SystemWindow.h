#ifndef _LUMINA_DESKTOP_SYSTEM_WINDOW_H
#define _LUMINA_DESKTOP_SYSTEM_WINDOW_H

#include <QDialog>
#include <QCoreApplication>
#include <QDesktopWidget>

#include <LuminaXDG.h>

#include "Globals.h"

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

private slots:
	void sysLogout(){
	  QCoreApplication::exit(0);
	}
	
	void sysRestart(){
	  SYSTEM::restart();
	  QCoreApplication::exit(0);		
	}
	
	void sysShutdown(){
	  SYSTEM::shutdown();
	  QCoreApplication::exit(0);
	}
	
	void sysCancel(){
	  this->close();
	}
};

#endif