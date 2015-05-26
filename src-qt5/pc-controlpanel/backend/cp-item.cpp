/**************************************************************************
*   Copyright (C) 2014 by Yuri Momotyuk                                   *
*   yurkis@pcbsd.org                                                      *
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

#include "cp-item.h"
#include "../config.h"
#include "pcbsd-utils.h"
#include "misc.h"

#include <unistd.h>

#include <QSettings>
#include <QLocale>
#include <QFile>
#include <QTextStream>
#include <QProcess>
#include <QTemporaryFile>
#include <QPainter>
#include <QDebug>
#include <QMessageBox>

#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>

using namespace pcbsd;

__string_constant TRY_MESSAGE_FIELD = "PC-TryMessage";
__string_constant TRY_COMMAND_FIELD = "PC-TryCommand";
__string_constant DE_FIELD_SHOW = "OnlyShowIn";
__string_constant DE_FIELD_HIDE = "NotShowIn";
__string_constant KEYWORDS_FIELD= "Keywords";

const QStringList SU_NAMES = QStringList()<<QString("pc-su ")<<QString("kdesu ")<<QString("gtksu ")<<QString("gksu ");

const QString DEFAULT_ICON_LOCATION = "/usr/local/share/pcbsd/pc-controlpanel/icons/";

__string_constant ROOT_PICTURE = ":/images/security-medium.png";

#define OXYGEN_THEME_PATH "/usr/local/share/icons/oxygen/"
#define OXYGEN_DIMS "64x64"
#define OXYGEN OXYGEN_THEME_PATH OXYGEN_DIMS

const QString ICON_SEARCH_PATH[] =
{
    QString(PREFIX) + "/share/pcbsd/pc-controlpanel/icons/",
    "/usr/local/share/pixmaps/",
    OXYGEN"/apps/",
    OXYGEN"/actions/",
    OXYGEN"/categories/",
    OXYGEN"/places/",
    OXYGEN"/status/"
};
const int ICON_SEARCH_PASS_SIZE = sizeof(ICON_SEARCH_PATH) / sizeof(QString);

__string_constant DEFAULT_ICON = "preferences-other.png";


///////////////////////////////////////////////////////////////////////////////
CControlPanelItem::CControlPanelItem()
{
    misRootRequired= false;
    misSudo= false;
}

///////////////////////////////////////////////////////////////////////////////
bool CControlPanelItem::read(QString file, bool skipRootAccess)
{
    QString Str;

    misValid= false;
    mShowIn.clear();
    mNotShowIn.clear();
    mKeywords.clear();

    //---------------Try to open file
    QSettings Reader(file, QSettings::IniFormat);
    if (Reader.status() != QSettings::NoError)
        return false;
    Reader.setIniCodec("UTF-8");

    Reader.beginGroup("Desktop Entry");

    //---------------Check TryExec field
    Str=  Reader.value("TryExec").toString();
    if (Str.length())
    {
        struct stat sts;
        if (  (0 != stat(qPrintable(Str),&sts))
            &&(0 != access(qPrintable(Str), X_OK)))
                return false;
    }

    //---------------Check TryCommand extention field
    Str= Reader.value(TRY_COMMAND_FIELD).toString();
    if (Str.length())
    {
        QTemporaryFile tmpFile;
        if (!tmpFile.open())
        {
            return false;
        }
        QTextStream out(&tmpFile);
        out<<Str.toLatin1()<<"\n";
        out.flush();
        tmpFile.flush();

        QProcess shell_proc;
        shell_proc.start("/bin/sh",QStringList()<<tmpFile.fileName());
        shell_proc.waitForFinished(7000);
        if (shell_proc.exitCode())
        {
            return false;
        }
    }//TryCommand is present

    //----------------Get Desktop environment to show (onlyShowIn)
    Str = Reader.value(DE_FIELD_SHOW).toString();
    if (Str.length())
    {
        mShowIn = Str.split(";", QString::SkipEmptyParts);
        for (int i=0; i<mShowIn.size(); i++)
        {
            mShowIn[i]= mShowIn[i].toLower().trimmed();
        }
    }// if DE_FIELD_SHOW is present

    //------------Get Desktop environment to hide (NotShowIn)
    Str = Reader.value(DE_FIELD_HIDE).toString();
    if (Str.length())
    {
        mNotShowIn = Str.split(";", QString::SkipEmptyParts);
        for (int i=0; i<mShowIn.size(); i++)
        {
            mNotShowIn[i]= mNotShowIn[i].toLower().trimmed();
        }
    }// if DE_FIELD_HIDE is present

    //--------------- Get type
    mType=Reader.value("Type").toString();

    //--------------- Get name and displayName;
    mName = Reader.value("Name").toString();
    if (!mName.length())
    {
        mName = Reader.value("GenericName").toString();
        if (!mName.length())
            return false;
    }

    mDisplayName = getLocalizedField(Reader, "Name");
    if (!mDisplayName.length())
    {
        mDisplayName = getLocalizedField(Reader, "GenericName");
    }

    //--------------- Get Exec command and check for super user rights
    mExecCommand= Reader.value("Exec").toString();
    if (mExecCommand.length())
    {
        misRootRequired = false;
        QString tmp = mExecCommand.trimmed();

        for (int i=0; i<SU_NAMES.size(); i++ )
        {
            if (tmp.left(SU_NAMES[i].size()) == SU_NAMES[i])
            {
                misRootRequired = true;
                break;
            }
        }
        if (tmp.indexOf("sudo") == 0)
            misSudo= true;
    }//Exec field & SU rights check 

    if (skipRootAccess && (misSudo || misRootRequired))
        return false;

    mExecPath= Reader.value("Path").toString();

    //---------------- Get icon
    mIconFile=  Reader.value("Icon").toString();
    if (!mIconImage.load(mIconFile))
    {
        QString FileName = QString(DEFAULT_ICON_LOCATION) + mIconFile;
        mIconImage.load(FileName);
    }

    //----------------- Get TryMessage extended field
    mMsgBoxText = getLocalizedField(Reader, TRY_MESSAGE_FIELD);

    //----------------- Get keywords field
    Str = Reader.value(KEYWORDS_FIELD).toString();
    QString LocStr = getLocalizedField(Reader, KEYWORDS_FIELD);
    mKeywords = Str.split(";", QString::SkipEmptyParts);
    if (LocStr != Str)
    {
        //Append localized keywords if present
        mKeywords+= LocStr.split(";", QString::SkipEmptyParts);
    }

    //---------------- Get comment field
    mComment= Reader.value("Comment").toString();
    mDisplayComment= getLocalizedField(Reader,"Comment");

    mFile= file;
    misValid = true;
    return true;
}

///////////////////////////////////////////////////////////////////////////////
QIcon CControlPanelItem::icon()
{
    static const QIcon UnknownIcon = QIcon(QString(DEFAULT_ICON_LOCATION));

    if (mIcon.availableSizes().size())
    {
        return mIcon;
    }

    if (!mIconImage.isNull())
    {
        mIcon = QIcon(QPixmap::fromImage(mIconImage));
    }

    if (mIconFile.length())
    {
        if (!mIcon.availableSizes().size())
        {
            //if icon loading failed (from absolute path) try to load icon
            // from theme
            mIcon = QIcon::fromTheme(mIconFile);
        }
        if (!mIcon.availableSizes().size())
        {
            //if icon loading failed try to load one of default icons
            QString FileName = QString(DEFAULT_ICON_LOCATION) + mIconFile;
            mIcon = QIcon(FileName);
        }
        // try to find icons in some custom icon search paths
        if (!mIcon.availableSizes().size() && (mIconFile.indexOf("/") == -1))
        {
            QString icon_name = (mIconFile.indexOf(".")>0)?mIconFile:mIconFile + ".png";
            for (int i=0; i<ICON_SEARCH_PASS_SIZE; i++)
            {
                mIcon = QIcon(ICON_SEARCH_PATH[i] + icon_name);
                if (mIcon.availableSizes().size())
                    break;
            }
        }
        if (!mIcon.availableSizes().size())
        {
            // And finally set default icon
            mIcon = QIcon(QString(DEFAULT_ICON_LOCATION) + DEFAULT_ICON);
        }
    }//if got Icon field
    else
    {
        mIcon= UnknownIcon;
    }//If Icon field is empty

    return mIcon;
}

///////////////////////////////////////////////////////////////////////////////
QIcon CControlPanelItem::displayIcon()
{
    if (mDisplayIcon.availableSizes().size())
    {
        return mDisplayIcon;
    }

    QSize orig_size = icon().availableSizes()[0]; //It should be loaded in read() and should have one size;
    int orig_h = orig_size.height();
    int orig_w = orig_size.width();

    if (!misRootRequired)
    {
        mDisplayIcon= icon();
        return mDisplayIcon;
    }

    QPixmap orig_pixmap =icon().pixmap(orig_size);
    QPainter painter(&orig_pixmap);
    QPixmap mark;
    mark.load(ROOT_PICTURE);
    QRect draw_rect=QRect(orig_w - orig_w/2, 0, orig_w/2, orig_h/2);
    painter.drawPixmap(draw_rect, mark);

    mDisplayIcon = QIcon(orig_pixmap);

    return mDisplayIcon;
}

///////////////////////////////////////////////////////////////////////////////
bool CControlPanelItem::matchWithFilter(QString filter)
{
    filter= filter.toLower().trimmed();
    if (!filter.length())
        return true;
    if (mDisplayName.toLower().indexOf(filter)>=0)
        return true;
    if (mName.toLower().indexOf(filter)>=0)
        return true;
    for (int i=0; i<mKeywords.size(); i++)
    {
        if (mKeywords[i].toLower().indexOf(filter)>=0)
            return true;
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////////
void CControlPanelItem::launch(bool useInternalLaunch)
{
    if (mMsgBoxText.length())
    {
        QMessageBox msgBox;
        msgBox.setText(mMsgBoxText);
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);
        if (QMessageBox::Yes != msgBox.exec())
            return;
    }
    

    if (!useInternalLaunch)
    {
        QProcess::startDetached("xdg-open",QStringList()<<mFile);
    }
    else
    {
        /*pid_t RetVal = fork();
        if (!RetVal)
        {*/
	    QProcess proc;
            if (mExecPath.length())
            {
                //chdir(qPrintable(mExecPath));
		proc.setWorkingPath(mExecPath);
            }
            QString Str = mExecCommand;
            Str.replace("%i", QString(" --icon ") + mIconFile);
            Str.replace("%c", QString("\"") + displayName().toLocal8Bit() + QString("\""));
            Str.replace("%k", mFile);
            Str.replace("%f","");
            Str.replace("%F","");
            Str.replace("%u","");
            Str.replace("%U","");
            //exit (system(Str.toLatin1()));
	    proc.startDetached(Str);
         }
    }//if internal launch

    
}

///////////////////////////////////////////////////////////////////////////////
QString CControlPanelItem::getLocalizedField(const QSettings &Reader, const QString &FieldName)
{
    QString Str;
    QString LocaleCountry = QLocale::system().name();
    QString Locale= QString("[")+LocaleCountry.left(2)+QString("]");
    LocaleCountry= QString("[")+LocaleCountry+QString("]");

    Str = Reader.value(FieldName + Locale).toString();
    if (Str.length())
        return Str;

    Str= Reader.value(FieldName+LocaleCountry).toString();
    if (Str.length())
        return Str;

    return Reader.value(FieldName).toString();
}
