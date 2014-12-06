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

#ifndef PBIDETAILSWIDGET_H
#define PBIDETAILSWIDGET_H

#include "pbicontroller.h"
#include <QWidget>
#include <QTreeWidgetItem>

namespace Ui {
class PBIDetailsWidget;
}

class PBIDetailsWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit PBIDetailsWidget(QWidget *parent = 0);
    ~PBIDetailsWidget();
    bool init(QString check_img, QString ok_img, QString avail_img,
              QString download_img, QString install_img, QString error_img,
              CPBIController* upd_controller);

private slots:
    void slotControllerStateChanged(CAbstractUpdateController::EUpdateControllerState new_state);
    void slotControllerLogLine(QString line);
    
    void on_updateSelectedPBIBtn_clicked();

    void on_pbiUpdateList_itemChanged(QTreeWidgetItem *item, int column);

private:
    Ui::PBIDetailsWidget *ui;
    CPBIController       *mUpdateController;
};

#endif // PBIDETAILSWIDGET_H
