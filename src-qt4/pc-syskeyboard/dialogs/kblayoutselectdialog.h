#ifndef KBLAYOUTSELECTDIALOG_H
#define KBLAYOUTSELECTDIALOG_H

#include <QDialog>
#include <QTreeWidgetItem>
#include "../keyboardsettings.h"

namespace Ui {
class KbLayoutSelectDialog;
}

class KbLayoutSelectDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit KbLayoutSelectDialog(QWidget *parent = 0);
    ~KbLayoutSelectDialog();

    void disableLayouts(QStringList lv);
    void setCurrent(QString layout_id);
    pcbsd::keyboard::SLayout selectedLayout();
    
private slots:
    void on_layoutsTW_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

private:
    Ui::KbLayoutSelectDialog *ui;

    void fillList();    
    QStringList disabledLayouts;
};

#endif // KBLAYOUTSELECTDIALOG_H
