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

#ifndef GROUPLIST_H
#define GROUPLIST_H
#include <QVector>
#include "qautoexpandlist.h"
#include "item.h"
#include "deinfo.h"

class QGroupList : public QAutoExpandList
{
    Q_OBJECT
public:
    explicit QGroupList(QWidget *parent = 0);

	//! Read category list
        int read(QString Path, const QVector<QString>& vEnabledDE,
                 bool EnableSudo=true, bool EnableSu=true);

	//! Set name filter string
        bool setFilter(const QString& filetrString);

        void setDisplayDEName(bool isDisplay);

protected:
	virtual void focusOutEvent ( QFocusEvent * event );
        void makeIcon(QCPItem* anItem);

signals:

public slots:

};

#endif // GROUPLIST_H
