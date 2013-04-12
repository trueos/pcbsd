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
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. *
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR     *
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, *
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR *
 *   OTHER DEALINGS IN THE SOFTWARE.                                       *
 ***************************************************************************/

#include <qapplication.h>
#include <qpainter.h>
#include <QTreeWidget>

#ifndef INC_PBI_H
#define INC_PBI_H

#define PBIINACTIVE     0
#define PBINEWDOWNLOAD 	1
#define PBIUPDATING   	2

#define PBIPREPARING 0
#define PBIFETCH   1
#define PBIINSTALL 2

class PBI : public QTreeWidgetItem {

public:
    //Constructors
    PBI(QTreeWidget *listbox = 0);
    PBI(QTreeWidget *listbox, QString name, QString version, QString iconPath, QString author, QString website);
    PBI(QIcon icon, QTreeWidget *listbox = 0);
    ~PBI();
    
    //Getters
    virtual bool doDelete();
    virtual bool getAutoUpdateStatus();
    virtual bool getDownloadFailed();
    virtual bool getUpdateAvail();
    virtual bool isDirty();
    virtual bool isNewDownload();
    virtual bool isWorking();
    virtual bool hasDesktopEntries();
    virtual bool hasMenuEntries();
    virtual int getDownloadAttempts();
    virtual int getWorkingStatus();
    virtual QString getArch();
    virtual QString getAuthor();
    virtual QString getDownloadArch();
    virtual QString getDownloadRepo();
    virtual QString getDownloadVersion();
    virtual QString getIconPath();
    virtual QString getInstallBy();
    virtual QString getName();
    virtual QString getNameCLI();
    virtual QString getProgDir();
    virtual QString getProgIndexName();
    virtual QString getRepoID();
    virtual QString getVersion();
    virtual QString getWebsite();
    virtual QString getStatusText();

    virtual int getProgressStatus();
    virtual int getProgressCurrent();
    virtual int getProgressMax();
    virtual QString getProgressText();
    virtual QString getProgressCaption();

    
    //Setters
    virtual void setArch(QString type);
    virtual void setAuthor(QString author);
    virtual void setAutoUpdateStatus(bool status);
    virtual void setDelete(bool status);
    virtual void setDirty(bool status);
    virtual void setDownloadArch(QString arch);
    virtual void setDownloadAttempts(int num);
    virtual void setDownloadFailed(bool status);
    virtual void setDownloadRepo(QString repo);
    virtual void setDownloadVersion(QString newver);
    virtual void setIconPath(QString iconPath);
    virtual void setInstallBy(QString name);
    virtual void setIsWorking(bool status);
    virtual void setProgDir(QString path);
    virtual void setProgIndexName(QString name);
    virtual void setName(QString name);
    virtual void setNewDownload(bool status);
    virtual void setRepoID(QString id);
    virtual void setStatusText(QString text);
    virtual void setUpdateAvail(bool status);
    virtual void setVersion(QString version);
    virtual void setWebsite(QString website);
    virtual void setWorkingStatus(int status);

    virtual void setActiveProgress(int current, int max, QString ProgressCaption, QString ProgressText=QString());
    
    //Overrides
    virtual QString text();
    virtual void paint(QPainter *p);
    
    virtual QString toString();

private:
    bool autoup;
    bool dirty;
    bool dodelete;
    bool downloadFailed;
    bool newdownload;
    bool update;
    bool working;
    int downloadAttempts;
    int workingStatus;
    QString arch;
    QString author;
    QString downloadArch;
    QString downloadRepo;
    QString downloadVersion;
    QString installBy;
    QString localMdate;
    QString name;
    QString progDir;
    QString progIndexName;
    QString savedIconPath;
    QString repoID;
    QString version;
    QString website;
    QString status_text;

    int progressCurrent;
    int progressMax;
    QString progressText;
    QString progressCaption;
    int progressStatus;
};

#endif // INC_PBI_H
