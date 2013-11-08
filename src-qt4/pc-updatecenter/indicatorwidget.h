#ifndef INDICATORWIDGET_H
#define INDICATORWIDGET_H

#include <QWidget>

namespace Ui {
class IndicatorWidget;
}

class IndicatorWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit IndicatorWidget(QWidget *parent = 0);
    ~IndicatorWidget();
    
private:
    Ui::IndicatorWidget *ui;
};

#endif // INDICATORWIDGET_H
