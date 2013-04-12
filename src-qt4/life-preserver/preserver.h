/***************************************************************************
 *   Copyright (C) 2009 by Kris Moore   *
 *   kris@pcbsd.org   *
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
#include <QVariant>


#ifndef INC_PRESERVER_H
#define INC_PRESERVER_H

class PRESERVER : public QObject {

public:
    //Constructors
    PRESERVER(QString name);
    ~PRESERVER();
    
    //Getters
    virtual QString getHost();
    virtual int getPort();
    virtual int getNumBackups();
    virtual QString getLastBackup();
    virtual QString getFrequency();
    virtual QString getCronSettings();
    virtual int getPID();
    virtual bool getFailedCleanup();
    virtual QString getPreserverString();
    virtual QString getRemoteDir();
    virtual QStringList getUserExcludes();
    virtual QStringList getUserIncludes();
    
    //Setters
    virtual void setHost(QString userhost);
    virtual void setPort(int port);
    virtual void setNumBackups(int num);
    virtual void setType(QString type);
    virtual void setCron(QString);
    virtual void setFailedCleanup(bool enabled);
    virtual void setRemoteDir(QString remoteDir);
    virtual void setUserExcludes(QStringList excludes);
    virtual void setUserIncludes(QStringList includes);

    // Other Functions which take actions
    virtual void addCronTab();
    virtual void remove();
    virtual void removeCronTab();
    virtual void startBackup();
    virtual void stopBackup();
    virtual QString listServerBackups();

private:
    void setConfValue(QString key, QVariant value);
    QVariant getConfValue(QString key);
    QString connectHost;

};

#endif // INC_PRESERVER_H
