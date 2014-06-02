#ifndef _BACKEND_H
#define _BACKEND_H

#include <QString>
#include <QDebug>
#include <QIcon>
#include <QProcess>
#include <QCoreApplication>
#include <QFile>
#include <QDir>


class Backend{
public:
  static QIcon icon(QString);
  static QStringList getCmdOutput(QString cmd, QString dir = ""); //Run a command and return the output
  static bool writeFile(QString filepath, QStringList contents);
  //Package database query functions
  static QStringList getPkgList();   //output format: <category>/<pkgname>
  static QStringList getPkgInfo(QString pkgorigin); //output format: [<name>, <port>, <maintainer>, <website>]
  static QStringList getPkgOpts(QString pkgorigin); //output format: <option>=<off/on>
  static QStringList getPkgPList(QString pkgorigin); //output format: one file per entry (full paths)
  static QStringList findPkgPlugins(QString pkgorigin); //output format: one pkgorigin per entry
  //Port query functions
  static QStringList getPortOpts(QString portPath);
};

#endif

