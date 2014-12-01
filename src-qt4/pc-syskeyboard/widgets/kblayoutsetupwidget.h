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

#ifndef KBLAYOUTSETUPWIDGET_H
#define KBLAYOUTSETUPWIDGET_H

#include <QWidget>

#include"../keyboardsettings.h"

#include <QTreeWidgetItem>

namespace Ui {
class KbLayoutSetupWidget;
}

class KbLayoutSetupWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit KbLayoutSetupWidget(QWidget *parent = 0);
    ~KbLayoutSetupWidget();

    //! Set user selected layouts to the keyboard settings
    void mergeSettings(pcbsd::keyboard::KeyboardSettings& ks);
    
private slots:
    void on_addBtn_clicked();

    void on_editBtn_clicked();

    void on_removeBtn_clicked();

    void on_layoutsTW_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    void on_upBtn_clicked();

    void on_downBtn_clicked();

private:
    Ui::KbLayoutSetupWidget *ui;
    void fillList(pcbsd::keyboard::KeyboardSettings cs);
    QStringList active();
    void fillItem(QTreeWidgetItem* item, pcbsd::keyboard::Layout layout);
    void moveItem(int idx, int off);
};

#endif // KBLAYOUTSETUPWIDGET_H
