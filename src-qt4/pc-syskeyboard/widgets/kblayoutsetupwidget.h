#ifndef KBLAYOUTSETUPWIDGET_H
#define KBLAYOUTSETUPWIDGET_H

#include <QWidget>

#include"../keyboardsettings.h"

#include <QTreeWidgetItem>

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

    void on_editBtn_clicked();

    void on_removeBtn_clicked();

    void on_layoutsTW_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    void on_upBtn_clicked();

    void on_downBtn_clicked();

private:
    Ui::KbLayoutSetupWidget *ui;
    void fillList(pcbsd::keyboard::KeyboardSettings cs);
    QStringList active();
    void fillItem(QTreeWidgetItem* item, pcbsd::keyboard::Layout layout);
    void moveItem(int idx, int off);
};

#endif // KBLAYOUTSETUPWIDGET_H
