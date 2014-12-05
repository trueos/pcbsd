/***************************************************************************
 *   Copyright (C) 2011 by Ken Moore	   				   *
 *   ken@pcbsd.org   							   *
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
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR     *
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, *
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR *
 *   OTHER DEALINGS IN THE SOFTWARE.                                       *
 ***************************************************************************/
 
#ifndef _PCBSD_HARDWARE_H_
#define _PCBSD_HARDWARE_H_

#include <QStringList>
#include <QString>

class Hardware
{
public:
   Hardware(QString devName);
   //---BLUETOOTH FUNCTIONS---
   static QStringList getBTdevNames();
   static void setBTdevName(QString newName, QString devName);
   static void addBTdevice(QString bdaddr, QString name, QString key, QString pin);
   static void rmBTdevice(QString bdaddr, bool willRestart);
   static void restartBT();
   static QStringList findBTdevices();
   static QString getBTRemoteName(QString bdaddr, bool tryagain = TRUE);
   static QStringList readAllSavedBTDevices();
   static QStringList readSavedBTDevice(QString bdaddr);
   static QString pairBTDevice(QString bdaddr);
   static void disconnectBTDevice(QString bdaddr);
   static QString getBTConnectionHandle(QString bdaddr);

private:
   static QString getHcsecdDeviceValue(QString rawline); //for bluetooth functions
};

#endif
