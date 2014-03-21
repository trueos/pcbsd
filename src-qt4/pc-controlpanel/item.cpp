/**************************************************************************
*   Copyright (C) 2011 by Yuri Momotyuk                                   *
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

#include "item.h"

#include <QSettings>
#include <QLocale>
#include <QFile>
#include <QTextStream>
#include <QProcess>
#include <QMessageBox>
#include <QDebug>
#include <QDir>
#include <QTemporaryFile>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "../config.h"

#include <iostream>

const char* const PBI_NAME_FIELD = "PC-PBIName";
const char* const TRY_MESSAGE_FIELD = "PC-TryMessage";
const char* const TRY_COMMAND_FIELD = "PC-TryCommand";
const char* const REQUIRED_DE = "PC-RequiredDE";

const char* const PBI_PREFIX_MACRO = "%PBI_PREFIX%";

const QString DEFAULT_ICON_LOCATION = PREFIX + "/share/pcbsd/pc-controlpanel/icons/";

#define OXYGEN_THEME_PATH "/usr/local/share/icons/oxygen/"
#define OXYGEN_DIMS "64x64"
#define OXYGEN OXYGEN_THEME_PATH OXYGEN_DIMS

const QString ICON_SEARCH_PATH[] =
{
    QString(PREFIX) + "/share/pcbsd/pc-controlpanel/icons/",
    OXYGEN"/apps/",
    OXYGEN"/actions/",
    OXYGEN"/categories/",
    OXYGEN"/places/",
    OXYGEN"/status/"
};

const int ICON_SEARCH_PASS_SIZE = sizeof(ICON_SEARCH_PATH) / sizeof(QString);

const char* const DEFAULT_ICON = "preferences-other.png";

const QString SU_NAMES[] = {QString("pc-su "), QString("kdesu "), QString("gtksu ")};
const QString SUDO_COMMAND("sudo ");

QCPItem::QCPItem():misValid(false), misMsgBox(false), isRequireRoot(false),isSudo(false)
{
}

///////////////////////////////////////////////////////////////////////////////
bool QCPItem::readDE(QString FileName, const QVector<QString>& vEnabledDE)
{
    QString Str;

    bool isPBISoftware = false;
    misValid= false;
    misSubstFound = false;
    misUseXDG = false;
    if (InstalledDEList.active())
        misUseXDG = InstalledDEList.active()->isXDG;

    misXDGLaunch = false;

    SPBIInfo PBIInfo;
    mvEnabledDE = vEnabledDE;

    //Try to open file
    QSettings Reader(FileName, QSettings::IniFormat);
    if (Reader.status() != QSettings::NoError)
        return false;
    Reader.setIniCodec("UTF-8");

    Reader.beginGroup("Desktop Entry");

    /**
          If type of desktop shortcut is 'application' or 'service' we will try
	  to launch application using own launch() realization.
          For another type (url for example) we use xdg for launch if XDGLaunch
	  is true.
	*/

    // If type is not application and we do not use xdg-launch - return false
    QString Type=Reader.value("Type").toString();
    if (Type.compare(QString("application"), Qt::CaseInsensitive))
    {
        //if type of shortcut is not application and we can use xdg...
        if (misUseXDG)
            misXDGLaunch = true;
        else
            return false;
    }

    Str= Reader.value(PBI_NAME_FIELD).toString();
    if (Str.length())
    {        
        if (!InstalledPBIs.findByName(Str, PBIInfo, Qt::CaseInsensitive))
            return false;
        /*if (!getPBIPath(Str, mPBIPath))
                        return false;*/
        mPBIPath = PBIInfo.mPrefix;
        isPBISoftware = true;
    }

    //Check TryExec field
    Str=  Reader.value("TryExec").toString();
    if (Str.length())
    {
        if (Str.contains(PBI_PREFIX_MACRO))
            misSubstFound = true;
        Str.replace(PBI_PREFIX_MACRO, PBIInfo.mPrefix);
        struct stat sts;
        if ((0 != stat(qPrintable(Str),&sts))
            &&(0 != access(qPrintable(Str), X_OK)))
            return false;
    }

    // Check PC-RequiredDE field (vmEnabledDE is a vector of names enabled DEs)
    Str = Reader.value(REQUIRED_DE).toString();
    if (Str.length())
    {
        bool found = false;
        for(int i = 0; i< mvEnabledDE.size(); i++)
        {
            Str=Str.trimmed();
            if (!Str.compare(mvEnabledDE[i].trimmed(), Qt::CaseInsensitive))
            {
                found = true;
                mRequiredDE= Str;
                break;
            }//if found
        }//for all enabled DE
        if (!found)
            return false;
    }

    //Check TryCommand extention field
    Str= Reader.value(TRY_COMMAND_FIELD).toString();
    if (Str.length())
    {
        QTemporaryFile tmpFile;
        if (!tmpFile.open())
        {
            return false;
        }
        QTextStream out(&tmpFile);
        out<<Str<<"\n";
        out.flush();
        tmpFile.flush();

        QProcess shell_proc;
        shell_proc.start("/bin/sh",QStringList()<<tmpFile.fileName());
        shell_proc.waitForFinished(7000);
        if (shell_proc.exitCode())
            return false;
    }
    /*if (Str.length())
    {
        if (system(Str.toLatin1()) != 0)
            return false;
    }*/


    mOriginalName = Reader.value("Name").toString();
    if (!mOriginalName.length())
    {
        mOriginalName = Reader.value("GenericName").toString();
        if (!mOriginalName.length())
            return false;
    }

    //Get Name or GenericName field
    if (!getLocalizedField(Reader, "Name",  Str))
    {        
        if (!getLocalizedField(Reader, "GenericName",  Str))
            return false;
    }
    setText(Str);
    mName=Str;

    //Get and check Exec field
    mExecCommand= Reader.value("Exec").toString();
    if (!mExecCommand.length() && !misXDGLaunch)
        return false;
    if (Str.contains(PBI_PREFIX_MACRO))
        misSubstFound = true;
    mExecCommand.replace(PBI_PREFIX_MACRO, PBIInfo.mPrefix);

    //Get Path field
    mPath= Reader.value("Path").toString();
    if (mPath.contains(PBI_PREFIX_MACRO))
           misSubstFound = true;
    mPath.replace(PBI_PREFIX_MACRO, PBIInfo.mPrefix);

    //Get Comment field
    if (getLocalizedField(Reader, "Comment",  Str))
    {
        setToolTip(Str);
    }

    //Get Icon field, try to load icon
    mIconFile=  Reader.value("Icon").toString();
    QIcon anIcon;
    //If .desktop file is not contain Icon field try to get icon from pbi info
    if (!mIconFile.length() && isPBISoftware)
        mIconFile = PBIInfo.mIcon;

    if (mIconFile.length())
    {
        if (Str.contains(PBI_PREFIX_MACRO))
            misSubstFound = true;

        mIconFile.replace(PBI_PREFIX_MACRO, PBIInfo.mPrefix);
        //mIconFile.replace("//","/");
        anIcon = QIcon(mIconFile);
        if (!anIcon.availableSizes().size())
        {
            //if icon loading failed (from absolute path) try to load icon
            // from theme
            anIcon = QIcon::fromTheme(mIconFile);
        }
        if (!anIcon.availableSizes().size())
        {
            QString FileName = QString(DEFAULT_ICON_LOCATION) + mIconFile;

            anIcon = QIcon(FileName);
        }        
        // try to find icons in some custom icon search paths
        if (!anIcon.availableSizes().size() && (mIconFile.indexOf("/") == -1))
        {
            QString icon_name = (mIconFile.indexOf(".")>0)?mIconFile:mIconFile + ".png";
            for (int i=0; i<ICON_SEARCH_PASS_SIZE; i++)
            {
                anIcon = QIcon(ICON_SEARCH_PATH[i] + icon_name);
                if (anIcon.availableSizes().size())
                    break;
            }
        }
        if (!anIcon.availableSizes().size())
        {
            anIcon = QIcon(QString(DEFAULT_ICON_LOCATION) + DEFAULT_ICON);
        }
    }//if got Icon field
    else
    {
        anIcon = QIcon(QString(DEFAULT_ICON_LOCATION) + DEFAULT_ICON);
    }
    if (anIcon.availableSizes().size())
        setIcon(anIcon);



    //Get TryMessage extended field
    if (getLocalizedField(Reader, TRY_MESSAGE_FIELD, mMsgBoxText))
    {
        misMsgBox= true;
    }
    else
    {
        misMsgBox= false;
    }

    //Get Tags
    mTags.clear();

    QString Tags= Reader.value("PC-Tags").toString();
    mTags=Tags.split(",",QString::SkipEmptyParts);
    if (getLocalizedField(Reader, "PC-Tags", Tags))
    {
        mTags<<Tags.split(",",QString::SkipEmptyParts);
    }
    mTags.removeDuplicates();

    // Check if item require root access
    isRequireRoot = false;
    QString tmp = mExecCommand.trimmed();

    for (unsigned int i=0; i<sizeof(SU_NAMES)/sizeof(QString); i++ )
    {
        if (tmp.left(SU_NAMES[i].size()) == SU_NAMES[i])
        {
            isRequireRoot = true;
            break;
        }
    }

    isSudo = false;
    if (tmp.left(SUDO_COMMAND.size()) == SUDO_COMMAND)
        isSudo = true;

    misValid= true;
    mFileName= FileName;

    return true;
}

///////////////////////////////////////////////////////////////////////////////
bool QCPItem::getLocalizedField(const QSettings& Reader,
                                const QString& FieldName,
                                QString& RetVal)
{
    QString Str;
    QString LocaleCountry = QLocale::system().name();
    QString Locale= QString("[")+LocaleCountry.left(2)+QString("]");
    LocaleCountry= QString("[")+LocaleCountry+QString("]");

    RetVal= Reader.value(FieldName).toString();

    Str = Reader.value(FieldName + Locale).toString();
    if (Str.length())
        RetVal= Str;

    Str= Reader.value(FieldName+LocaleCountry).toString();
    if (Str.length())
        RetVal= Str;

    return (bool)RetVal.length();
}

///////////////////////////////////////////////////////////////////////////////
bool QCPItem::isValid()
{
    return misValid;
}

///////////////////////////////////////////////////////////////////////////////
bool QCPItem::launch()
{
    if (!isValid())
        return false;

    //If shortcut contain 'TryMessage' field
    if (misMsgBox)
    {
        QMessageBox msgBox;
        msgBox.setText(mMsgBoxText);
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);
        if (QMessageBox::Yes != msgBox.exec())
            return true;
    }

    if (misXDGLaunch)
        return launchXDG();
    else
        return launchOwn();
}

///////////////////////////////////////////////////////////////////////////////
bool QCPItem::launchOwn()
{
    pid_t RetVal = fork();
    if (!RetVal)
    {
        if (mPath.length())
        {
            chdir(qPrintable(mPath));
        }
        QString Str = mExecCommand;
        Str.replace("%i", QString(" --icon ") + mIconFile);
        Str.replace("%c", QString("\"") + text().toLocal8Bit() + QString("\""));
        Str.replace("%k", mFileName);
        Str.replace("%f","");
        Str.replace("%F","");
        Str.replace("%u","");
        Str.replace("%U","");
        exit (system(Str.toLatin1()));
    }
    return RetVal > 0;
}

///////////////////////////////////////////////////////////////////////////////
bool QCPItem::launchXDG()
{
    QProcess proc;
    QString FileToOpen = mFileName;

    if (misSubstFound)
    {
        /* File is contain %PBI_PREFIX% macro, we must replace macro and save to
		   temporary file */
        QTemporaryFile outfile(QDir::tempPath()+"/XXXXXXX.desktop");
        if (!outfile.open())
            return false;
        outfile.setAutoRemove(false);

        QFile infile (mFileName);
        if (!infile.open(QIODevice::ReadOnly | QIODevice::Text))
            return false;

        QTextStream out(&outfile);
        QString Str;
        while (!infile.atEnd())
        {
            Str = infile.readLine();
            Str.replace(PBI_PREFIX_MACRO, mPBIPath);
            out<<Str;
        }
        FileToOpen = outfile.fileName();
        out.flush();
        outfile.flush();
    }
    //TODO: Remove temp file!
    proc.startDetached("xdg-open",QStringList()<<FileToOpen);
    return true;
}

///////////////////////////////////////////////////////////////////////////////
void QCPItem::setDisplayDEName(bool isDisplay)
{
    QString NewText = mName;
    if (isDisplay && mRequiredDE.length())
        NewText = QString("%1 - %2"). arg(mRequiredDE).arg(mName);
    setText(NewText);
}
