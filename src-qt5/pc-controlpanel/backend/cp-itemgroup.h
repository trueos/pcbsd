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

#ifndef CPITEMGROUP_H
#define CPITEMGROUP_H

#include "cp-item.h"
#include <QVector>
#include <QThread>
#include <QMutex>

class CItemGroup: public QThread
{
    Q_OBJECT
public:
    CItemGroup(QString path, QString name);

    bool read();
    void readAssync();

    void setSkipRootRequiredItems(bool isSkip);

    QVector<CControlPanelItem> items(QStringList enabled_de, QString filter=QString());
    int itemsCount(QStringList enabled_de, QString filter=QString());

    QString path();
    QString name();

signals:
    void itemsReady();
    void itemsReadyAssync();

private:
    QVector<CControlPanelItem> mItems;
    QString mName;
    QString mPath;
    QMutex  mGlobalMutex;

    bool checkItemDE(CControlPanelItem &item, const QStringList& enabled_de);    

private:
    void run();
    bool mSkipRootAccess;
};

#endif // CPITEMGROUP_H
