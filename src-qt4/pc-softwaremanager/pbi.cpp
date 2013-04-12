/***************************************************************************
 *   Copyright (C) 2007 by Tim McCormick   *
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
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR     *
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, *
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR *
 *   OTHER DEALINGS IN THE SOFTWARE.                                       *
 ***************************************************************************/

#include "pbi.h"
#include <qimage.h>
#include <qtranslator.h>
#include <qobject.h>
#include <QFile>
#include <QDir>

PBI::PBI(QTreeWidget *listbox) : QTreeWidgetItem(listbox)
{
   name = "SOMENAME";
   version = "0.0.1";
   setIcon(0, QIcon());
   author = "";
   website = "";
   installBy = "";
   savedIconPath = "";
   setText(0, name + " " + version);
   update = false;
   workingStatus = PBIINACTIVE;
   working = false;
   dirty = false;
   dodelete = false;
   newdownload = false;
   downloadFailed = false;
   autoup = false;
}

PBI::PBI(QTreeWidget *listbox, QString name, QString version, QString iconPath, QString author, QString website) : QTreeWidgetItem(listbox)
{
   this->name = name;
   this->version = version;
   setIcon(0, QIcon(iconPath));
   this->author = author;
   this->website = website;
   savedIconPath = iconPath;
   setText(0,name + " " + version);
   update = false;
   workingStatus = PBIINACTIVE;
   working = false;
   installBy = "";
   dirty = false;
   dodelete = false;
   newdownload = false;
   downloadFailed = false;
   autoup = false;
}

PBI::PBI(QIcon icon, QTreeWidget *listbox) : QTreeWidgetItem(listbox)
{
   setIcon(0, icon);
   update = false;
   workingStatus = PBIINACTIVE;
   working = false;
   dirty = false;
   dodelete = false;
   newdownload = false;
   downloadFailed = false;
   autoup = false;
   installBy = "";
}

PBI::~PBI()
{

}

bool PBI::isNewDownload()
{
   return newdownload;
}

bool PBI::getDownloadFailed()
{
  return downloadFailed;
}

void PBI::setDownloadFailed( bool status)
{
   this->downloadFailed = status;
}

void PBI::setNewDownload( bool status)
{
   this->newdownload = status;
}

void PBI::setProgIndexName( QString name) 
{
   this->progIndexName = name;
}

QString PBI::getProgIndexName()
{
   return progIndexName;
}

bool PBI::doDelete()
{
   return dodelete;
}

void PBI::setDelete( bool status)
{
   this->dodelete = status;
}

bool PBI::isDirty()
{
   return dirty;
}

void PBI::setDirty( bool status)
{
   this->dirty = status;
}


bool PBI::getUpdateAvail()
{
   return update;
}

int PBI::getDownloadAttempts()
{
   return downloadAttempts;
}

void PBI::setDownloadAttempts(int num)
{
  this->downloadAttempts = num;
}

QString PBI::getName()
{
     return name;
}

QString PBI::getVersion()
{
   return version;
}

QString PBI::getAuthor()
{
   return author;
}

QString PBI::getWebsite()
{
   return website;
}

int PBI::getWorkingStatus()
{
  return workingStatus;
}

void PBI::setDownloadVersion(QString newver)
{
  this->downloadVersion = newver;
}

QString PBI::getDownloadVersion()
{
  return downloadVersion;
}

void PBI::setDownloadArch(QString arch)
{
  this->downloadArch = arch;
}

void PBI::setDownloadRepo(QString repo)
{
  this->downloadRepo = repo;
}

QString PBI::getProgDir()
{
   return progDir;
}

void PBI::setProgDir(QString path)
{
   this->progDir = path;
}

QString PBI::getDownloadArch()
{
  return downloadArch;
}

QString PBI::getDownloadRepo()
{
  return downloadRepo;
}

void PBI::setWorkingStatus(int status)
{
  this->workingStatus = status;
}

void PBI::setUpdateAvail(bool status)
{
   this->update = status;
   if ( status ) {
     this->setText(1, downloadVersion + " " + "Available");
   } else {
     this->setText(1, "");
       setStatusText("");
   }
}

void PBI::setName(QString name)
{
   this->name = name;
   setText(0, name + " " + version);
}

void PBI::setVersion(QString version)
{
   this->version = version;
   setText(0, name + " " + version);
}


void PBI::setIconPath(QString iconPath)
{
   this->savedIconPath = iconPath;
   setIcon(0, QIcon(iconPath));
}

void PBI::setAuthor(QString author)
{
   this->author = author;
}

void PBI::setWebsite(QString website)
{
   this->website = website;
}

void PBI::setArch(QString type)
{
   this->arch = type;
}

QString PBI::getArch()
{
   return arch;
}

QString PBI::getNameCLI()
{
   return name.toLower() + "-" + version.toLower() + "-" + arch.toLower();
}

void PBI::setStatusText(QString text)
{
   this->setText(1, text);
   status_text = text;
}

QString PBI::text()
{
   return name + " " + version;
}

void PBI::paint(QPainter *p)
{
   QPixmap icon = QPixmap();
   p->drawPixmap(3, 0, icon);
   if (!text().isEmpty())
   {
      QFontMetrics fm = p->fontMetrics();
      int yPos;
      if (icon.height() < fm.height()) yPos = fm.ascent() + fm.leading()/2;
      else yPos = icon.height()/2 - fm.height()/2 + fm.ascent();
      p->drawText(icon.width() + 5, yPos, text());
   }
}

QString PBI::toString()
{
   return text() + "\n" + getAuthor();
}

QString PBI::getIconPath()
{
   return savedIconPath;
}

QString PBI::getRepoID()
{
	return repoID;
}

void PBI::setRepoID(QString id)
{
	repoID = id;
}

void PBI::setIsWorking(bool status)
{
	working=status;
}


bool PBI::getAutoUpdateStatus()
{
	return autoup;
}

void PBI::setAutoUpdateStatus(bool status)
{
	autoup=status;
}

void PBI::setInstallBy(QString name)
{
	installBy=name;
}

QString PBI::getInstallBy()
{
	return installBy;
}

bool PBI::isWorking()
{
	return working;
}

QString PBI::getStatusText()
{
    return status_text;
}

void PBI::setActiveProgress(int current, int max, QString ProgressCaption, QString ProgressText)
{
    progressCurrent = current;
    progressMax = max;
    progressCaption = ProgressCaption;
    progressText = ProgressText;
}

int PBI::getProgressStatus()
{
    return progressStatus;
}

int PBI::getProgressCurrent()
{
    return progressCurrent;
}

int PBI::getProgressMax()
{
    return progressMax;
}

QString PBI::getProgressText()
{
    return progressText;
}

QString PBI::getProgressCaption()
{
    return progressCaption;
}

bool PBI::hasDesktopEntries(){
  QString pbiDir = "/usr/pbi/"+progIndexName.section("-",0,0)+"-"+arch;
  QDir dir(pbiDir+"/.xdg-desktop");
  if(!dir.exists()){return FALSE;} //not installed yet
  QStringList entries = dir.entryList( QStringList("*.desktop"),QDir::Files,QDir::Unsorted );
  if(entries.isEmpty()){ return FALSE; }
  else{ return TRUE; }
}

bool PBI::hasMenuEntries(){
  QString pbiDir = "/usr/pbi/"+progIndexName.section("-",0,0)+"-"+arch;
  QDir dir(pbiDir+"/.xdg-menu");
  if(!dir.exists()){return FALSE;} //not installed yet
  QStringList entries = dir.entryList( QStringList("*.desktop"),QDir::Files,QDir::Unsorted );
  if(entries.isEmpty()){ return FALSE; }
  else{ return TRUE; }
}

