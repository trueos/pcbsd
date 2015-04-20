#include "pkgVulDialog.h"
#include "ui_pkgVulDialog.h"

#include "pcbsd-utils.h"

#include <QDebug>
#include <QDesktopServices>
#include <QUrl>

using namespace pcbsd;

static bool wasFetch = false;

PkgVulDialog::PkgVulDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PkgVulDialog)
{
    ui->setupUi(this);
    ui->mainStack->setCurrentIndex(0);
}

PkgVulDialog::~PkgVulDialog()
{
    delete ui;
}

void PkgVulDialog::beginAudit()
{
    mVulVector.clear();
    qDebug()<<"Begin";
    QString cmd = "pkg audit";
    if (!wasFetch)
    {
        cmd+=" -F";
    }

    QStringList out = Utils::runShellCommand(cmd);

    SVulInfo entry;
    QString line;
    bool isMessage = false;

    for (int i=0; i<out.size(); i++)
    {
        line = out[i];
        //Empty line - end of package entry
        if (!line.length())
        {
            if (entry.mPkgGenericName.length())
            {
                qDebug()<<"Found vulnerable package ===========";
                qDebug()<<entry.mPkgGenericName;
                qDebug()<<entry.mPkgName;
                qDebug()<<entry.mPkgVersion;
                qDebug()<<entry.mCVEList;
                qDebug()<<entry.mWWW;
                qDebug()<<entry.mMessage;
                qDebug()<<".....................";

                mVulVector.push_back(entry);
            }
            entry = SVulInfo();
            continue;
        }//if empty line

        /*Example:
         *  chromium-40.0.2214.93 is vulnerable:
         */
        if (line.indexOf("is vulnerable:") >= 0)
        {
            entry.mPkgGenericName = line.split(" ")[0];
            int idx= entry.mPkgGenericName.lastIndexOf("-");
            entry.mPkgName= entry.mPkgGenericName.left(idx);
            entry.mPkgVersion = entry.mPkgGenericName.mid(idx + 1);
            isMessage = true;
            continue;
        }//package name

        /* Example:
         * CVE: CVE-2015-1212
         */
        if (line.startsWith("CVE: "))
        {
            isMessage = false;
            entry.mCVEList.append(line.replace("CVE: ",""));

            continue;
        }

        /* Example:
         * WWW: http://vuxml.FreeBSD.org/freebsd/a6eb239f-adbe-11e4-9fce-080027593b9a.html
         */
        if (line.startsWith("WWW: "))
        {
            isMessage = false;
            entry.mWWW = line.replace("WWW: ","");
            continue;
        }

        if (isMessage)
        {
            if (entry.mMessage.length())
                entry.mMessage+="\n";
            entry.mMessage+=line;
        }

    }//for all lines
}

void PkgVulDialog::fillUI()
{
    ui->vulList->clear();
    for(int i=0; i<mVulVector.size(); i++ )
    {
        QStringList strs;
        strs<<mVulVector[i].mPkgName<<mVulVector[i].mPkgVersion<<mVulVector[i].mMessage;
        QTreeWidgetItem* item = new QTreeWidgetItem(ui->vulList,strs);
        item->setData(0, Qt::UserRole, QVariant(i));
        ui->vulList->addTopLevelItem(item);
    }
}

int PkgVulDialog::execDialog()
{
    this->show();
    beginAudit();
    if (mVulVector.size())
    {
        fillUI();
        ui->mainStack->setCurrentIndex(1);
    }
    else
    {
        ui->mainStack->setCurrentIndex(2);
    }

    return exec();
}

void PkgVulDialog::on_vulList_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    Q_UNUSED(previous);
    qDebug()<<"changed";
    ui->cveList->clear();
    if (!current) return;
    int idx = current->data(0, Qt::UserRole).toInt();
    if ((idx<0)||(idx>=mVulVector.size())) return;
    for(int i=0; i<mVulVector[idx].mCVEList.size(); i++)
    {
        ui->cveList->addItem(mVulVector[idx].mCVEList[i]);
    }
}

void PkgVulDialog::on_moreInfoBtn_clicked()
{
    QTreeWidgetItem* current = ui->vulList->currentItem();
    if (!current)
        return;
    int idx = current->data(0, Qt::UserRole).toInt();
    if ((idx<0)||(idx>=mVulVector.size())) return;
    QString url = mVulVector[idx].mWWW;
    QDesktopServices::openUrl(QUrl(url));
}
