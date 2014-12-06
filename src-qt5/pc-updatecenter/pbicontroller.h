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

#ifndef PBICONTROLLER_H
#define PBICONTROLLER_H

#include "updatecontroller.h"

#include <QVector>

class CPBIController : public CAbstractUpdateController
{
    Q_OBJECT
    USES_CHECK_SHELL_COMMAND("pbi_update", QString("--check-all"))

public:
    typedef struct _SPBIUpdate
    {
        QString mName;
        QString mGenericName;
        QString mOldVer;
        QString mNewVer;
    }SPBIUpdate;

public:
    CPBIController();

    QVector<SPBIUpdate> pbiUpdates()
        { return mvUpdates;}

    void updateSelected(QStringList generic_names_list);

protected:
    virtual void onCheckUpdates();
    virtual void onUpdateAll();
    virtual void onCancel();

    void onReadCheckLine(QString line);
    void onReadUpdateLine(QString line);

    virtual void onCheckProcessfinished(int exitCode);
    virtual void onUpdateProcessfinished(int exitCode);

    virtual void updateShellCommand(QString& cmd, QStringList& args);

private:
    QVector<SPBIUpdate> mvUpdates;

    QStringList mAppsToUpdate;
    int         mCurrentUpdate;
    bool        misWasFETCHDONE;

};

#endif // PBICONTROLLER_H
