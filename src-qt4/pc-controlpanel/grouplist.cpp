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

#include "grouplist.h"
#include "pbiinfo.h"
#include <QDir>
#include <QSizePolicy>
#include <QLayout>
#include <QPainter>

const int ICON_DIM = 64;
const char* const ROOT_PICTURE = ":/images/images/security-medium.png";
//const char* const ROOT_IMAGE[] = ":/images/images/object-locked.png";

QGroupList::QGroupList(QWidget *parent) :
    QAutoExpandList(parent)
{
    setIconSize(QSize(ICON_DIM, ICON_DIM));
	setViewMode(QListView::IconMode);
	setWordWrap(true);
	setFrameStyle(QFrame::NoFrame);
    setSortingEnabled(true);
        //setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum));
}

///////////////////////////////////////////////////////////////////////////////
int QGroupList::read(QString Path, const QVector<QString>& vEnabledDE, bool EnableSudo, bool EnableSu)
{
	clear();
	QDir dir(Path);
	if (!dir.exists(Path))
		return 0;    

	QStringList DirList =dir.entryList(QStringList("*.desktop"));
	for(int i=0; i<DirList.size(); i++)
	{
		QCPItem* anItem = new QCPItem;

                if (anItem->readDE(dir.absoluteFilePath(DirList[i]), vEnabledDE))
                {
                    if ((anItem->isSudoRequired() && (!EnableSudo))
                      ||(anItem->isRootRequired() && (!EnableSu)))
                    {
                        delete anItem;
                    }
                    else
                    {
                        makeIcon(anItem);
                        addItem(anItem);
                    }
                }
                else
                    delete anItem;
	}// for all files in dir

    setAcceptDrops(false);
    setDragEnabled(false);

	setIconSize(QSize(32, 32));

    fitSize();



    return count();
}

///////////////////////////////////////////////////////////////////////////////
void QGroupList::makeIcon(QCPItem* anItem)
{

    QPixmap orig(ICON_DIM, ICON_DIM);

    QPixmap pix;
    orig.fill(Qt::transparent);

    QPainter painter(&orig);

    painter.drawPixmap(0, 0, ICON_DIM, ICON_DIM, anItem->icon().pixmap(ICON_DIM));

    if ( anItem->isRootRequired() && pix.load(ROOT_PICTURE) )
    {

        painter.drawPixmap(orig.width() - pix.width(), 0 , pix);
        anItem->setIcon(QIcon(orig));
    }

    anItem->setIcon(QIcon(orig));
}

///////////////////////////////////////////////////////////////////////////////
void QGroupList::focusOutEvent ( QFocusEvent * event )
{
	QListWidget::focusOutEvent( event);
	setCurrentRow(-1);
}

///////////////////////////////////////////////////////////////////////////////
bool QGroupList::setFilter(const QString& filerString)
{	
    int NumHidden=0;

	for (int i=0; i< count(); i++)
	{
                QCPItem* curr = (QCPItem*)item(i);
		if (!filerString.length())
		{
			curr->setHidden(false);
            curr->setFlags(curr->flags() | Qt::ItemIsEnabled);
		}
		else
		{
             bool IsHide = !curr->itemName().contains(filerString, Qt::CaseInsensitive);
             IsHide = IsHide && (!curr->itemOriginalName().contains(filerString, Qt::CaseInsensitive));
             //IsHide = IsHide && (!curr->tags().contains(filerString, Qt::CaseInsensitive));
             //curr->setHidden(IsHide);
             QStringList Tags = curr->tags();
             QString Tag;
             for(int i=0; i<Tags.size(); i++)
             {
                Tag = Tags.at(i).trimmed();
                if (Tag.contains(filerString, Qt::CaseInsensitive))
                {
                    IsHide = false;
                    break;
                }
             }

             curr->setFlags((IsHide)?curr->flags() & (~Qt::ItemIsEnabled)
                                    :curr->flags() | Qt::ItemIsEnabled);
             if (IsHide)
                NumHidden++;
		}
	}

        fitSize();
        this->layout()->update();

        return (NumHidden<count());
}

///////////////////////////////////////////////////////////////////////////////
void QGroupList::setDisplayDEName(bool isDisplay)
{
    for (int i=0; i< count(); i++)
    {
        QCPItem* curr = (QCPItem*)item(i);
        if (curr)
            curr->setDisplayDEName(isDisplay);
    }//for all items
}
