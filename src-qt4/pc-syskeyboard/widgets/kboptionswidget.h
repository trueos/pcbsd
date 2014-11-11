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
    ~KbOptionsWidget();
    
private:
    Ui::KbOptionsWidget *ui;
    void fillTree();
};

#endif // KBOPTIONSWIDGET_H
