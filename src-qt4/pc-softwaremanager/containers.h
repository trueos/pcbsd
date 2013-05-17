/***************************************************************************
 *   Copyright (C) 2011 - iXsystems				 	  *
 *   kris@pcbsd.org  *
 *   tim@pcbsd.org   *
 *   ken@pcbsd.org   *
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
#ifndef _APPCAFE_CONTAINERS_H
#define _APPCAFE_CONTAINERS_H

#include <QString>
#include <QProcess>
#include <QObject>
#include <QDebug>

// Info Container for Installed PBI's (pbi_info <pbi-name> data)
class InstalledPBI{
	
  public:
    enum PBISTATUS{ NONE, DOWNLOADING, INSTALLING, REMOVING, UPDATING, PENDINGDOWNLOAD, PENDINGINSTALL, PENDINGREMOVAL, PENDINGUPDATE, UPDATEAVAILABLE, WORKING };
    QString name, version, author, website, arch, path, icon, license, mdate, metaID, statusString, downloadfile;
    PBISTATUS status;
    bool rootInstall, autoUpdate, desktopIcons, menuIcons;
    
    InstalledPBI();
    ~InstalledPBI(){}
    void setStatus(PBISTATUS stat, QString addInfo="");
  
};

// Info Container for PBI Meta information on a repo
class MetaPBI{
	
  public:
    QString name, category, remoteIcon, localIcon, author, website, license, appType, description;
    QStringList tags;
    QString latestVersion, latestDatetime, latestArch, latestSizeK, latestFilename; //newest version in the index
    QString backupVersion, backupDatetime, backupArch, backupSizeK, backupFilename; //next-to-newest version in the index
    bool requiresroot;
    
    MetaPBI(){ requiresroot=FALSE; }
    ~MetaPBI(){}
		
};

// Info Container for Category Meta information on a repo
class MetaCategory{
	
  public:
    QString name, remoteIcon, localIcon, description;
    
    MetaCategory(){}
    ~MetaCategory(){}
    
};

#endif
