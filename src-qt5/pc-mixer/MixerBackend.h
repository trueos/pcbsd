#ifndef _PC_MIXER_BACKEND_H
#define _PC_MIXER_BACKEND_H

#include <QProcess>
#include <QStringList>
#include <QString>

class Mixer{

public:
	//Process Run
	static QString quickRun(QString cmd){ //"mixer -S" always returns a single line
	  QProcess p;
		p.start(cmd);
		p.waitForFinished(-1);
	  QString out = p.readAllStandardOutput();
	  return out;
	}

	//Mixer Interaction functions
	static QStringList getDevices(){ //Returns: QStringList[device:Lval:Rval]
	  QStringList out = Mixer::quickRun("mixer -S").split(" ").filter(":");
	  return out;
	}
	
	static void setValues(QString device, int Lpercent, int Rpercent=-1){
	  if(Lpercent < 0){ Lpercent = 0; }
	  else if(Lpercent > 100){ Lpercent = 100; }
	  if(Rpercent < 0){ Rpercent = Lpercent; }
	  else if(Rpercent > 100){ Rpercent = 100; }
	  Mixer::quickRun("mixer "+device+" "+QString::number(Lpercent)+":"+QString::number(Rpercent));
	}
	
	static QString getValues(QString device){  //Returns: "Lval:Rval"
	  QString out = Mixer::quickRun("mixer -S "+device).section(":",1,2); //trim off the device
	  return out;
	}
	
	static QString getRecDevice(){
	  return Mixer::quickRun("mixer -S").section("=rec ",-1).simplified();
	}
	
	static void setRecDevice(QString device){
	  Mixer::quickRun("mixer =rec "+device);
	}

};

#endif