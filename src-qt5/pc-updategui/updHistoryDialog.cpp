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

#include "updHistoryDialog.h"
#include "ui_updHistoryDialog.h"

#include "pcbsd-utils.h"
#include <QDebug>
#include <QTreeWidgetItem>
#include <QFont>

#define __string_constant static const char* const

__string_constant UPDATE_DESCRIPTIONS_FETCH_COMMAND = "fetch -q -o-";
__string_constant UPDATE_DESCRIPTIONS_URL = "http://fbsd-update.pcbsd.org/updates.desc";
__string_constant UPDATE_DESCRIPTIONS_FIELDS_SEPARATOR = ":::";

static bool isRefresh= true;

///////////////////////////////////////////////////////////////////////////////
UpdateHistoryDialog::UpdateHistoryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateHistoryDialog)
{
    ui->setupUi(this);
    mCurrentPatch = 0;
    ui->historyListTW->header()->resizeSection(0, 64);
}

///////////////////////////////////////////////////////////////////////////////
UpdateHistoryDialog::~UpdateHistoryDialog()
{
    delete ui;
}

///////////////////////////////////////////////////////////////////////////////
void UpdateHistoryDialog::execDialog()
{    
    //get current release and patchset
    QStringList uname_out = pcbsd::Utils::runShellCommand("uname -r");
    mInstalledRelease = uname_out[0].left(uname_out[0].lastIndexOf("-"));
    QString rel_patch = uname_out[0].right(uname_out[0].length() - uname_out[0].lastIndexOf("-"));
    mInstalledPatch = rel_patch.replace("-p","").toInt();

    descriptions(mInstalledRelease);

    //get all releases and fill release selector.
    ui->releaseSelector->clear();
    for(int i=0; i<mUpdates.size(); i++)
    {
        //fill releases
        if(!mReleases.contains(mUpdates[i].mRelease, Qt::CaseInsensitive))
        {
            mReleases.append(mUpdates[i].mRelease);
            ui->releaseSelector->addItem(mUpdates[i].mRelease);
            if (mUpdates[i].mRelease.trimmed().toLower() == mInstalledRelease.trimmed().toLower())
            {
                ui->releaseSelector->setCurrentIndex(ui->releaseSelector->count()-1);
                ui->releaseSelector->setItemText(ui->releaseSelector->currentIndex(), mUpdates[i].mRelease + tr(" (Current)"));
            }
        }

        //calculate max patch number for curren release
        if (mInstalledRelease.trimmed().toLower() == mUpdates[i].mRelease.trimmed().toLower())
        {
            mCurrentPatch= (mCurrentPatch>=mUpdates[i].mUpdateNo)?mCurrentPatch:mUpdates[i].mUpdateNo;
        }
    }

    if (mInstalledPatch<mCurrentPatch)
    {
        ui->updateAvailLabel->setVisible(true);
    }
    else
    {
        ui->updateAvailLabel->setVisible(false);
    }

    fillHistoryList(mInstalledRelease);

    connect(ui->releaseSelector, SIGNAL(currentIndexChanged(int)),
            this, SLOT(slotSelector_currentIndexChanged(int)));

    exec();
}

///////////////////////////////////////////////////////////////////////////////
void UpdateHistoryDialog::fillHistoryList(QString release)
{
    QVector<SFbsdUpdatesDescription> list = descriptions(release);
    ui->historyListTW->clear();

    for(int i=0; i<list.size(); i++)
    {
        QTreeWidgetItem* item = new QTreeWidgetItem(QStringList()<<QString::number(list[i].mUpdateNo)<<list[i].mDescription);\

        ui->historyListTW->addTopLevelItem(item);

        if ((list[i].mUpdateNo == mInstalledPatch) && (release == mInstalledRelease))
        {
            QFont font = item->font(1);
            font.setBold(true);
            item->setFont(1, font);
            ui->historyListTW->setCurrentItem(item);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void UpdateHistoryDialog::slotSelector_currentIndexChanged(int index)
{
    fillHistoryList(mReleases[index]);
}

///////////////////////////////////////////////////////////////////////////////
QVector<UpdateHistoryDialog::SFbsdUpdatesDescription> UpdateHistoryDialog::descriptions(QString release)
{
    QVector<SFbsdUpdatesDescription> out;

    if (!mUpdates.size() || isRefresh)
    {
        QStringList fetch_out = pcbsd::Utils::runShellCommand(QString(UPDATE_DESCRIPTIONS_FETCH_COMMAND) + " " + UPDATE_DESCRIPTIONS_URL);
        mUpdates.clear();
        for (int i=0; i<fetch_out.size(); i++)
        {
                SFbsdUpdatesDescription entry;
                QStringList line_split = fetch_out[i].split(UPDATE_DESCRIPTIONS_FIELDS_SEPARATOR);
                if (line_split.size() < 3)
                    continue;
                entry.mRelease = line_split[0];
                entry.mDescription = line_split[2];
                entry.mUpdateNo = line_split[1].toInt();
                mUpdates.push_back(entry);
        }
        isRefresh = mUpdates.size()>0;
     }

     //filter by RelName
     for (int i=0; i<mUpdates.size(); i++)
     {
         if ((!release.length()) || release.trimmed().toUpper() == mUpdates[i].mRelease.trimmed().toUpper() )
         {
             out.push_back(mUpdates[i]);
         }
     }

     return out;
}
