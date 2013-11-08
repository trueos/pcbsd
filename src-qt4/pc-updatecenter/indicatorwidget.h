#ifndef INDICATORWIDGET_H
#define INDICATORWIDGET_H

#include <QWidget>

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

    bool init(QString check_img, QString ok_img, QString avail_img, QString process_img,
              CAbstractUpdateController* upd_controller);
    
public slots:
    void stateChanged(CAbstractUpdateController::EUpdateControllerState new_state);
    void progress(CAbstractUpdateController::SProgress progress);

private:
    Ui::IndicatorWidget *ui;
};

#endif // INDICATORWIDGET_H
