#ifndef KBSIMPLEOPTIONSWIDGET_H
#define KBSIMPLEOPTIONSWIDGET_H

#include <QWidget>

namespace Ui {
class KbSimpleOptionsWidget;
}

class KbSimpleOptionsWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit KbSimpleOptionsWidget(QWidget *parent = 0);
    ~KbSimpleOptionsWidget();
    
private:
    Ui::KbSimpleOptionsWidget *ui;
};

#endif // KBSIMPLEOPTIONSWIDGET_H
