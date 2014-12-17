#ifndef _PCBSD_SYSTRAY_REMINDER_DIALOG_H
#define _PCBSD_SYSTRAY_REMINDER_DIALOG_H

#include <QDialog>
#include <QString>
#include <QComboBox>

#include "ui_dialogReminder.h"

namespace Ui{
	class DialogReminder;
};

class DialogReminder : public QDialog{
	Q_OBJECT
	
private:
	Ui::DialogReminder *ui;
	int minutes;

private slots:
	void minutesChanged(int index){
	  //Convert from a list index to a minutes value (list set in the *.ui form)
	  switch (index){
	    case 0:
		minutes = 0; // never
	        break;
	    case 1:
		minutes = 30; //30 minutes
	        break;
	    case 2:
		minutes = 60; //1 hour
	        break;
	    case 3:
		minutes = 300; //5 hours
	        break;
	    case 4:
		minutes = 720; //12 hours
	        break;
	    case 5:
		minutes = 1440; //24 hours
	        break;
	    default:
		minutes = 0; //should never happen, but just in case
	  }
	}
	
public:


	DialogReminder() : QDialog(), ui(new Ui::DialogReminder()){
	  ui->setupUi(this);
	  minutes = 30; //default value if the user just closes the window
	  //Center on the middle of the screen (TO DO)
	  	
	  //Connect the signals/slots
	  connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(minutesChanged(int)) );
	  connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(close()) );
	}

	~DialogReminder(){}
		
	int delayMinutes(){
	  return minutes;
	}
};

#endif
