/**************************************************************************
*   Copyright (C) 2014- by Yuri Momotyuk                                   *
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

#ifndef UPDATEHISTORYDIALOG_H
#define UPDATEHISTORYDIALOG_H

#include <QDialog>
#include <QStringList>

namespace Ui {
class UpdateHistoryDialog;
}

class UpdateHistoryDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit UpdateHistoryDialog(QWidget *parent = 0);
    ~UpdateHistoryDialog();

    void execDialog();

    typedef struct _SFbsdUpdatesDescription
    {
        QString mRelease;
        int     mUpdateNo;
        QString mDescription;

        _SFbsdUpdatesDescription():mUpdateNo(-1){;}

    }SFbsdUpdatesDescription;
    
private slots:
    void slotSelector_currentIndexChanged(int index);

private:
    Ui::UpdateHistoryDialog *ui;

    QVector<SFbsdUpdatesDescription> mvFbsdUpdateDescriptions;

    QStringList mReleases;
    QString     mInstalledRelease;
    int         mInstalledPatch;
    int         mCurrentPatch;

    void fillHistoryList(QString release);
    QVector<SFbsdUpdatesDescription> updateDescriptions(QString RelName = QString(), bool isForse = false);
};

#endif // UPDATEHISTORYDIALOG_H
