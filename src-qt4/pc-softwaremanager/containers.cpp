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
#include "containers.h"

//Initialization functions for the container classes

// --- INSTALLEDPBI ---
InstalledPBI::InstalledPBI(){
      rootInstall=FALSE; autoUpdate=FALSE; desktopIcons=FALSE; menuIcons=FALSE;
      status = NONE;
}

void InstalledPBI::setStatus(PBISTATUS stat, QString addinfo){
  //addinfo: additional info (download statistics or update version available)
  status = stat;
  if(stat == DOWNLOADING){ 
    if(addinfo.isEmpty()){ statusString = QObject::tr("Downloading..."); }
    else{ statusString = QString(QObject::tr("Downloading: %1")).arg(addinfo); }
  }
  else if(stat == INSTALLING){ statusString = QObject::tr("Installing..."); }
  else if(stat == REMOVING){ statusString = QObject::tr("Removing..."); }
  else if(stat == UPDATING){ 
    if(addinfo.isEmpty()){ statusString = QObject::tr("Updating..."); }
    else{ statusString = QString(QObject::tr("Updating Downloading: %1 ")).arg(addinfo); }
  }
  else if(stat == PENDINGDOWNLOAD){ statusString = QObject::tr("Pending Download"); }
  else if(stat == PENDINGINSTALL){ statusString = QObject::tr("Pending Install"); }
  else if(stat == PENDINGREMOVAL){ statusString = QObject::tr("Pending Removal"); }
  else if(stat == PENDINGUPDATE){ statusString = QObject::tr("Pending Update"); }
  else if(stat == UPDATEAVAILABLE){ 
    if(addinfo.isEmpty()){ statusString = QObject::tr("Update Available!"); }
    else{ statusString = QString(QObject::tr("Version %1 Available")).arg(addinfo); }
  }
  else{ statusString = ""; }
  //qDebug() << "Status Update:" << name << status << statusString;
}

