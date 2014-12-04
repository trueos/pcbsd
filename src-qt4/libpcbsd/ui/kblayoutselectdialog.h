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

#ifndef KBLAYOUTSELECTDIALOG_H
#define KBLAYOUTSELECTDIALOG_H

#include <QDialog>
#include <QTreeWidgetItem>
#include "keyboardsettings.h"

namespace Ui {
class KbLayoutSelectDialog;
}

class KbLayoutSelectDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit KbLayoutSelectDialog(QWidget *parent = 0);
    ~KbLayoutSelectDialog();

    //! Disable somre layouts in list
    void disableLayouts(QStringList lv);
    //! Set current selected layout in list
    void setCurrent(QString layout_id);
    //! Get layout selected by user
    pcbsd::keyboard::SLayout selectedLayout();
    
private slots:
    void on_layoutsTW_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    void on_searchLE_textChanged(const QString &arg1);

private:
    Ui::KbLayoutSelectDialog *ui;

    void fillList();    
    QStringList disabledLayouts;
};

#endif // KBLAYOUTSELECTDIALOG_H
