/**************************************************************************
*   Copyright (C) 2011-2014 by Yuri Momotyuk                              *
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

#include "qautoexpandlist.h"

QAutoExpandList::QAutoExpandList(QWidget *parent) :
	QListWidget(parent), mIsExpandEnabled(true)
{
	setResizeMode(QListView::Adjust);
        //setGridSize(QSize(100,100));
}

///////////////////////////////////////////////////////////////////////////////
void QAutoExpandList::setIsAutoExpand(bool isEnabled)
{
	mIsExpandEnabled= isEnabled;
}

///////////////////////////////////////////////////////////////////////////////
bool QAutoExpandList::isAutoExpand()
{
	return mIsExpandEnabled;
}

///////////////////////////////////////////////////////////////////////////////
void QAutoExpandList::resizeEvent ( QResizeEvent * event )
{	
    fitSize();
    QListWidget::resizeEvent(event);
}

/*void QAutoExpandList::dataChanged ( const QModelIndex & topLeft,
                                   const QModelIndex & bottomRight, const QVector<int> & roles)
{
    QListWidget::dataChanged(topLeft, bottomRight);
    //fitSize();
}*/

///////////////////////////////////////////////////////////////////////////////
void QAutoExpandList::focusOutEvent(QFocusEvent *event)
{
    QListWidget::focusOutEvent( event);
    setCurrentRow(-1);
}

///////////////////////////////////////////////////////////////////////////////
void QAutoExpandList::fitSize()
{
    if (mIsExpandEnabled)
    {

        QSize Size = contentsSize();
        QMargins Mar = contentsMargins();
        Size.setHeight(Size.height() + Mar.top() + Mar.bottom());
        if ((Size.height()) >=0 && (Size.width()>=0))
        {
                setMinimumSize(Size);
        }
    }
}


