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

#ifndef QAUTOEXPANDLIST_H
#define QAUTOEXPANDLIST_H

#include <QListWidget>

class QAutoExpandList : public QListWidget
{
    Q_OBJECT
public:
    explicit QAutoExpandList(QWidget *parent = 0);

	/**
	 *  Function for getting size of contents outside class
	 *  ( contentsSize() is protected member in Qt )
	*/
	const QSize pubContentsSize()
	{
			return contentsSize();
	}

	/**
	  * Enable or disable auto expand
	  *
	  * @param isEnabled - true to enable auto expand
	  */
	void setIsAutoExpand(bool isEnabled);

	/**
	  * Get 'AutoExpand' state
	  *
	  * @return TRUE if auto expand enabled
	  */
	bool isAutoExpand();

        void fitSize();

protected:
	virtual void resizeEvent ( QResizeEvent * event );
        virtual void dataChanged ( const QModelIndex & topLeft,
                                   const QModelIndex & bottomRight );

private:
	//! Is auto expand enabled (TRUE by default)
	bool mIsExpandEnabled;

signals:

protected:
    virtual void focusOutEvent ( QFocusEvent * event );

};

#endif // QAUTOEXPANDLIST_H
