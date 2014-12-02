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

#ifndef KBSIMPLEOPTIONSWIDGET_H
#define KBSIMPLEOPTIONSWIDGET_H

#include <QWidget>

#include "keyboardsettings.h"

namespace Ui {
class KbSimpleOptionsWidget;
}

class KbSimpleOptionsWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit KbSimpleOptionsWidget(QWidget *parent = 0);
    ~KbSimpleOptionsWidget();

    //! Set keyboard settings to display
    void setSettings(pcbsd::keyboard::KeyboardSettings ks);

    //! Set user selected options to the keyboard settings
    void mergeSettings(pcbsd::keyboard::KeyboardSettings& ks);
    
private:
    Ui::KbSimpleOptionsWidget *ui;
    void setupUI(pcbsd::keyboard::KeyboardSettings ks);
};

#endif // KBSIMPLEOPTIONSWIDGET_H
