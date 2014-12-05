#ifndef REBOOTWIDGET_H
#define REBOOTWIDGET_H

#include <QWidget>

#include "syscontroller.h"
#include "pkgcontroller.h"
#include "pbicontroller.h"

namespace Ui {
class RebootWidget;
}

class RebootWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit RebootWidget(QWidget *parent = 0);
    ~RebootWidget();

    void init(CSysController* sys_controller, CPkgController* pkg_controller, CPBIController* pbi_controller);
    
private slots:
    void on_restartButton_clicked();

    void globalStateChanged(CAbstractUpdateController::EUpdateControllerState new_state);

private:
    Ui::RebootWidget *ui;

    CSysController* mSysController;
    CPkgController* mPkgController;
    CPBIController* mPBIController;

    bool            misReboot;
    bool            misLogoff;
};

#endif // REBOOTWIDGET_H
