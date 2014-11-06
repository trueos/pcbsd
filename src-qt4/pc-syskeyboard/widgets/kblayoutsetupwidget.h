#ifndef KBLAYOUTSETUPWIDGET_H
#define KBLAYOUTSETUPWIDGET_H

#include <QWidget>

#include"../keyboardsettings.h"

namespace Ui {
class KbLayoutSetupWidget;
}

class KbLayoutSetupWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit KbLayoutSetupWidget(QWidget *parent = 0);
    ~KbLayoutSetupWidget();
    
private slots:
    void on_addBtn_clicked();

private:
    Ui::KbLayoutSetupWidget *ui;
    pcbsd::keyboard::KeyboardSettings cs;
    void fillList();
};

#endif // KBLAYOUTSETUPWIDGET_H
