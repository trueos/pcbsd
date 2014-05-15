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
#include <QSettings>

class CControlPanelItem: public QObject
{
    Q_OBJECT
public:

    CControlPanelItem();

    bool read(QString file, QStringList enabledDE);
    bool launch();

    QString file();
    QString name();
    QString displayName();
    QString description();
    QString displayDescription();
    QStringList tags();
    QString iconFile();
    QIcon   icon();
    QIcon   displayIcon();

    QString execString();
    QString execPath();
    bool    isRootRequired();
    bool    isSudoRequired();
    bool    isMessageBox();
    QString messageBoxText();

private:
    QString mFile;
    QString mName;
    QString mDisplayName;
    QString mDescription;
    QString mDisplayDescription;

protected:
    QString getLocalizedField(const QSettings& Reader,
                              const QString& FieldName);

};

#endif // CPITEM_H
