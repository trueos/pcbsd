#ifndef DIALOGCONFLICT_H
#define DIALOGCONFLICT_H

#include <QDialog>
#include "pkgcontroller.h"


namespace Ui {
class DialogConflict;
}

class DialogConflict : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogConflict(QWidget *parent = 0);
    ~DialogConflict();

public slots:

    int exec(QString conflictList, CPkgController* controller= 0);
    
private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::DialogConflict *ui;
    CPkgController* mController;
};

#endif // DIALOGCONFLICT_H
