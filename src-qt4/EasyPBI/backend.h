#ifndef _BACKEND_H
#define _BACKEND_H

#include <QString>
#include <QDebug>
#include <QIcon>
#include <QProcess>
#include <QCoreApplication>
#include <QFile>


class Backend{
public:
  static QIcon icon(QString);
  static QStringList getCmdOutput(QString cmd, QString dir = ""); //Run a command and return the output
  //Package database query functions
  static QStringList getPkgList();   //output format: <category>/<pkgname>
  static QStringList getPkgInfo(QString pkgname); //output format: [<name>, <port>, <maintainer>, <website>]
  static QStringList getPkgOpts(QString pkgname); //output format: <option>=<off/on>
  //Port query functions
  static QStringList getPortOpts(QString portPath);
};

#endif

