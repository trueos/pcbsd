/**************************************************************************
*   Copyright (C) 2013- by Yuri Momotyuk                                   *
*   yurkis@gmail.com                                                      *
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

#ifndef SYSCONTROLLER_H
#define SYSCONTROLLER_H

#include "updatecontroller.h"
#include "utils.h"

#include <QProcess>
#include <QString>
#include <QVector>
#include <QDate>

class CSysController : public CAbstractUpdateController
{
    Q_OBJECT

    USES_SYS_FLAG("SYSUPDATE")

public:
    CSysController();

    typedef enum{
        eSYSUPDATE,
        ePATCH,
        eFBSDUPDATE
    }ESysUpdate;

    typedef struct _SSystemUpdate
    {
        ESysUpdate mType;
        QString mName;
        QString mTag;
        QString mDetails; // For patch only
        QString mSize;    // For patch only
        QString mVersion; // For sysupdates only
        QDate   mDate;    // For patch only        
        bool   misStandalone;
        bool   misRequiresReboot;

        _SSystemUpdate()
        {
            misStandalone= false;
            misRequiresReboot= false;
        }
    }SSystemUpdate;

    typedef struct _SFbsdUpdatesDescription
    {
        QString mRelease;
        int     mUpdateNo;
        QString mDescription;

        _SFbsdUpdatesDescription():mUpdateNo(-1){;}

    }SFbsdUpdatesDescription;

    typedef enum{
        eUndefined,
        eFilesModifyedLocally,
        eFilesToDelete,
        eFilesToUpdate
    }ECheckState;
    
    QVector<SSystemUpdate> updates() {return mvUpdates;}
    QStringList filesToUpdate()      {return mFilesToUpdate;}
    QStringList filesToDelete()      {return mFilesToRemove;}
    QStringList fileslocalyModifyed(){return mFilesLocallyModifyed;}
    bool        rebootRequired();
    QVector<SFbsdUpdatesDescription> updateDescriptions(QString RelName = QString(), bool isForse = false);

    void updateSelected(QVector<SSystemUpdate> selectedUpdates);

protected:
    virtual void onCheckUpdates();
    virtual void onUpdateAll();
    virtual void checkShellCommand(QString& cmd, QStringList& args);
    virtual void updateShellCommand(QString& cmd, QStringList& args);

    void onReadCheckLine(QString line);
    void onReadUpdateLine(QString line);
    virtual void onReadProcessChar(char character);

    virtual void onCheckProcessfinished(int exitCode);
    virtual void onUpdateProcessfinished(int exitCode);
    virtual void onCancel();

private:
    bool misFREEBSDCheck;
    QVector<SSystemUpdate> mvUpdates;
    QStringList mFilesLocallyModifyed;
    QStringList mFilesToRemove;
    QStringList mFilesToUpdate;
    bool        misFBSDRebootRequired;
    bool        misRebootRequired;
    ECheckState mCurrCheckState;

    QVector<SSystemUpdate> mvUpdatesToApply;
    int                    mCurrentUpdate;

    QVector<SFbsdUpdatesDescription> mvFbsdUpdateDescriptions;
    QString mCurrentFbsdDescription;

    void parseCheckPCBSDLine(QString line);
    void parseCheckFREEBSDLine(QString line);

    void parsePatchUpdateLine(QString line);
    void parseUpgradeLine(QString line);
    void parseFreeBSDUpdateLine(QString line);

    QString fbsdUpdateDescription(QString line);


};

#endif // SYSCONTROLLER_H
