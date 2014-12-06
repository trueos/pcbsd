#ifndef _FS_WATCHER_H
#define _FS_WATCHER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QTimer>
#include <QProcess>
#include <QProcessEnvironment>
#include <QCoreApplication>
#include <QDebug>
#include <QTime>

#include <math.h>

class FSWatcher : public QObject
{
	Q_OBJECT
	
public:
  FSWatcher();
  ~FSWatcher();
  
  void start(int); //input in milliseconds
  void stop();
  
  static QStringList getFSmountpoints();
  static QString doubleToDisplay(double);
  static int displayToDouble(QString); 
  
private:
  QTimer *timer;
  QStringList oldBadDevs;
  
  static QStringList runCMD(QString);
  static int calculatePercentage(double,double);
  
public slots:
  void checkFS(); //function in a timer loop

signals:
  void FSWarning(QString, QString);
  
};


#endif
