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

#ifndef CPITEM_H
#define CPITEM_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QIcon>
#include <QImage>
#include <QSettings>

class CControlPanelItem
{
public:

    CControlPanelItem();

    bool read(QString file, bool skipRootAccess= false);
    void launch(bool useInternalLaunch=false);

    QString file()       {return mFile;};
    QString name()       {return mName;};
    QString displayName(){return mDisplayName;};
    QString comment()    {return mComment;};
    QString displayComment(){return mDisplayComment;};
    QStringList keywords(){return mKeywords;};
    QString iconFile()   {return mIconFile;};
    QStringList showIn() {return mShowIn;};
    QStringList notShowIn(){return mNotShowIn;};
    QIcon   icon();
    QIcon   displayIcon(/*QSize sizeToDisplay*/);
    QString type();    

    QString execString();
    bool    isRootRequired() {return misRootRequired;};
    bool    isSudo()         {return misSudo;};
    bool    isMessageBox();
    QString messageBoxText();

    bool    matchWithFilter(QString filter);

private:
    bool    misValid;
    QString mType;
    QString mFile;
    QString mName;
    QString mDisplayName;
    QString mComment;
    QString mDisplayComment;
    QString mExecCommand;
    QString mExecPath;
    bool    misRootRequired;
    bool    misSudo;
    QString mIconFile;
    QString mMsgBoxText;

    QStringList mKeywords;
    QStringList mShowIn;
    QStringList mNotShowIn;

    QImage mIconImage;
    QIcon  mIcon;
    QIcon  mDisplayIcon;

protected:
    QString getLocalizedField(const QSettings& Reader,
                              const QString& FieldName);

};

#endif // CPITEM_H
