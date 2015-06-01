/***************************************************************************
 *   Copyright (C) 2008 by Tim McCormick   *
 *   tim@pcbsd.org   *
 *                                                                         *
 *   Permission is hereby granted, free of charge, to any person obtaining *
 *   a copy of this software and associated documentation files (the       *
 *   "Software"), to deal in the Software without restriction, including   *
 *   without limitation the rights to use, copy, modify, merge, publish,   *
 *   distribute, sublicense, and/or sell copies of the Software, and to    *
 *   permit persons to whom the Software is furnished to do so, subject to *
 *   the following conditions:                                             *
 *                                                                         *
 *   The above copyright notice and this permission notice shall be        *
 *   included in all copies or substantial portions of the Software.       *
 *                                                                         *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       *
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    *
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. *
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR     *
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, *
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR *
 *   OTHER DEALINGS IN THE SOFTWARE.                                       *
 ***************************************************************************/
#include <QCoreApplication>
#include <QDebug>
#include <QDialog>
#include <QFile>
#include <QLabel>
#include <QPushButton>
#include <QProcess>
#include <QProgressDialog>
#include <QMenu>
#include <QMessageBox>
#include <QTranslator>
#include <QTreeWidget>
#include <QWidget>

 
#ifndef _PCBSD_UTILS_H_
#define _PCBSD_UTILS_H_

namespace pcbsd
{

typedef struct _DesktopEnvironmentInfo
{
public:
    QString Name;          ///< De name 'Gnome', 'KDE', etc
    QString SudoCommand;   ///< native sudo command 'gtksu %s' for example
    bool    isInstalled;   ///< True if DE installed
    bool    isActive;      ///< True if DE is active now
    bool    isXDG;         ///< True if DE is XDG compatible
    QString ConfigurationApplication;
    QString TerminalCommand;
    QString TerminalTitleKey;
    QString TerminalCommandKey;
    bool    isTerminalNeedParamsSplit;
    QString FileManager;

    _DesktopEnvironmentInfo():isInstalled(false), isActive(false), isXDG(false), isTerminalNeedParamsSplit(false)
    {;}

}DesktopEnvironmentInfo;

class Utils
{
public:
   /** Get sysctl value as string
    *
    * \param sysctl - name of sysctl variable
    *
    *\return variable value
    */
   static QString sysctl(QString sysctl);

   /** Get sysctl value as integer
    *
    * \param sysctl - name of sysctl variable
    *
    *\return variable value
    */
   static long long sysctlAsInt(QString sysctl);

   /** Get human readable value of some storage space ( xx Mb, xx Gb, etc)
    *
    * \param bytes - amount of bytes
    *
    * \return string in human readable form
    */
   static QString bytesToHumanReadable(long long bytes);

   /** Function to get a value from a specific key in the pbi.conf file
    *
    *\param key- key name from pbi.conf
    *
    *\return Value of parameter
    */
   static QString getValFromPBIConf(QString key);

   /**  Function to get a value from a specific key in the pcbsd.conf file
    *
    *\param key- key name from pcbsd.conf
    *
    *\return Value of parameter
    */
   static QString getValFromPCBSDConf(QString key);

   /** Get value for from a specific key in the system config "ini style" file
    *
    *\param conf - config file name
    *\param key - key name in file 'conf'
    *
    *\return Value of 'key'
    **/
   static QString getValFromPCConf(QString conf, QString key);

   /** Get system proxy url
    *
    *\return System proxy URL
    */
   static QString getProxyURL();

   /** Get username for proxy access
    *
    *\return User name
    */
   static QString getProxyUser();
   static QString getProxyPass();
   static QString getProxyType();
   static QString getProxyPort();
   static QString getMasterMirror();
   static bool setValPBIConf(QString key, QString val);
   static bool setValSHFile(QString, QString, QString);
   static bool setValSHFile(QString, QString, QString, QString);
   static bool setValCSHFile(QString, QString, QString);
   static bool setValPCBSDConf(QString key, QString val);
   static bool setValPCConf(QString conf, QString key, QString val);
   static bool setProxyAddress(QString val);
   static bool setProxyAuth(QString val);
   static bool setMasterMirror(QString val);
   static bool setConfFileValue( QString oFile, QString oldKey, QString newKey );
   static bool setConfFileValue( QString oFile, QString oldKey, QString newKey, int occur );
   static QString getConfFileValue( QString oFile, QString Key );
   static QString getConfFileValue( QString oFile, QString Key, int occur );
   static QString getConfFileValue( QString oFile, QString Key, QString ValRx, int occur );
   static QStringList runShellCommand( QString command );
   static QStringList runShellCommand( QString command , bool &success);
   static QString runShellCommandSearch( QString command, QString sch );
   static QStringList listShellCommandSearch( QString command, QString sch );
   static QString getLineFromCommandOutput( QString cmd );
   static QString getValFromSHFile(QString envFile, QString envVal);
   static QString getValFromCSHFile(QString envFile, QString envVal);
   static QString readTextFile(QString filepath);
   static bool writeTextFile(QString filepath, QString contents, bool replace = true);
   static void restartNetworking();
   static void runInTerminal(QString command, QString windowTitle=QString());
   static void openInFileManager(QString location);
   static bool validateIPV6(QString);
   static bool validateIPV4(QString);

   static QVector<DesktopEnvironmentInfo> installedDesktops();
   static DesktopEnvironmentInfo currentDesktop();

   /** Check ability to desktop environment session quit
    *
    *\return TRUE is application is able to quit session
    */
   static bool canLogout();

   /** Performs correct desktop environment session quit
    *
    * Performs correct desktop environment session quit. It is not working for all of desktop environments.
    * \see canLogout()
    */
   bool logout();
};

} //namespace

#endif
