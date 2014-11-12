#ifndef KBOPTIONSWIDGET_H
#define KBOPTIONSWIDGET_H

#include <QWidget>

#include "keyboardsettings.h"

namespace Ui {
class KbOptionsWidget;
}

class KbOptionsWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit KbOptionsWidget(QWidget *parent = 0);

    void setSettings(pcbsd::keyboard::KeyboardSettings ks);
    void mergeSettings(pcbsd::keyboard::KeyboardSettings& ks);

    ~KbOptionsWidget();
    
private:
    Ui::KbOptionsWidget *ui;
    void fillTree(pcbsd::keyboard::KeyboardSettings ks);
};

#endif // KBOPTIONSWIDGET_H
