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

#ifndef INDICATORWIDGET_H
#define INDICATORWIDGET_H

#include <QWidget>
#include <QIcon>

#include "updatecontroller.h"

namespace Ui {
class IndicatorWidget;
}

class IndicatorWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit IndicatorWidget(QWidget *parent = 0);
    ~IndicatorWidget();

    bool init(QString check_img, QString ok_img, QString avail_img,
              QString download_img, QString install_img, QString error_img,
              CAbstractUpdateController* upd_controller);    
    
public slots:
    void stateChanged(CAbstractUpdateController::EUpdateControllerState new_state);
    void progress(CAbstractUpdateController::SProgress progress);
    void updateError(QString message);

protected:
    void onCheck();
    void onUpdateAvail();
    void onFullyUpdated();
    void onUpdateProgress();
    void onUpdateError();

protected:

    QString mDownloadImage;
    QString mInstallImage;
    CAbstractUpdateController* mpUC;

    QString mStateImages[CAbstractUpdateController::eMAX];

private:
    Ui::IndicatorWidget *ui;
};

#endif // INDICATORWIDGET_H
