/* PCDM Login Manager:
*  Written by Ken Moore (ken@pcbsd.org) 2012/2013
*  Copyright(c) 2013 by the PC-BSD Project
*  Available under the 3-clause BSD license
*/

#ifndef PCDM_CONFIG_H
#define PCDM_CONFIG_H

#include <QString>
#include <QStringList>
#include <QFile>
#include <QDebug>

class Config{

private:
  static void loadDefaults();
  static void readConfigFile(QString);

public:
  //Load the Configuration File
  static void loadConfigFile(QString);
  //General Configuration Settings Output
  static QString themeFile();
  static QString xSessionsDir();
  static QString xSessionsImageDir();
  static QString loginScriptsDir();
  static bool useAutoLogin(); 		//returns whether auto-login is enabled
  static QString autoLoginUsername();	//returns username to log into automatically
  static QString autoLoginDesktop();	//return *.desktop file to startup automatically
  static QString autoLoginPassword();	//returns password to log in automatically
  static int autoLoginDelay(); //Returns the number of seconds to delay auto-login
  static QString splashscreen();	//return the image file for the splashscreen
  static bool allowPasswordView();	//returns whether password view button is enabled
  static bool allowUserSelection(); 	//returns whether to display system users for selection

};

#endif
