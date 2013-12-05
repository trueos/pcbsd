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

#ifndef JAILSBACKEND_H
#define JAILSBACKEND_H

#include <QString>
#include <QVector>

class CJailsBackend
{
public:
    typedef struct _SJailDescription
    {
        QString mName;
        QString mType;
        bool misStarted;
        bool misEnabled;

        _SJailDescription()
        {
            misStarted = false;
            misEnabled = false;
        }
    }SJailDescription;

    CJailsBackend();

    static QVector<SJailDescription> jailsList();

    bool setJail(QString name);
    void setJailEnabled(bool isEnabled);

    bool jailEnabled();
    QString jailName();
    QString jailPrefix();

private:
    bool   misEnabled;
    QString mName;
};

#endif // JAILSBACKEND_H
