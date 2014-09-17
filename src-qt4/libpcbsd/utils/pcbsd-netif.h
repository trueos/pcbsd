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
 
#ifndef _PCBSD_NETIF_H_
#define _PCBSD_NETIF_H_

#include <qstringlist.h>
#include <qstring.h>

class NetworkInterface
{
public:
   NetworkInterface(QString devName);
   QString ipAsString();
   QString ipv6AsString();
   QString netmaskAsString();
   QString desc();
   QString macAsString();
   QString mediaTypeAsString();
   QString mediaStatusAsString();
   QString devName();
   bool isWireless();
   bool usesDHCP();
   bool isUp();
   long packetsRx();
   long packetsTx();
   long errorsRx();
   long errorsTx();
   uint devNum();
   
   static void restart();
   static QStringList getInterfaces();
   static QStringList parseWifiScanLine(QString linein, bool isverbose);
   static QString getWifiSecurity(QString SSID,QString deviceName);
   static QString parseWifiSecurity( QString caps, QString etc );
   static void wifiQuickConnect(QString SSID, QString netKey, QString DeviceName, bool WEPHex = true);
   static QString getFirstWiredDevice();
   static QString getWifiParent(QString dev);
   static void enableLagg(QString dev);
   
   static int enableWirelessAccessPoint(QString wdev, QString name, QString password, bool persist = true, QString ip = "192.168.0.1", QString mode = "11g", int channel = 1, QString netmask = "255.255.255.0");
   static void disableWirelessAccessPoint(QString wdev);

private:
   QString name;
};

#endif
