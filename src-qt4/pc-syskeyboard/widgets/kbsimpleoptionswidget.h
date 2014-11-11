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

    void mergeSettings(pcbsd::keyboard::KeyboardSettings& ks);
    
private:
    Ui::KbSimpleOptionsWidget *ui;
    void setupUI();
};

#endif // KBSIMPLEOPTIONSWIDGET_H
