#ifndef DIALOGFSPROP_H
#define DIALOGFSPROP_H
#include "zmanagerwindow.h"
#include <QDialog>
#include <QAbstractButton>
#include <QDialogButtonBox>

namespace Ui {
class DialogFSProp;
}


class DialogFSProp : public QDialog
{
    Q_OBJECT
    QList<zproperty> AllProperties;
    zfs_t *fsprops;

public:

    QString getFSProperty(QString Property);
    zproperty *getFSBaseProperty(QString Property);
    bool FSPropertyExists(QString Property);
    QString getFSPropertySource(QString Property);


    void InitAllProperties();
    void refreshList();
    void setDataset(zfs_t *props);

    void itemRevertChanges(QTreeWidgetItem *item,zproperty *pr);
    void itemInherit(QTreeWidgetItem *item,zproperty *prop);
    void itemNewProperty();
    void itemRemoveProperty();



    QStringList getAllChangedProps();   // RETURN A LIST OF ALL PROPERTIES THAT WERE CHANGED BY THE USER
    QStringList getAllChangedValues();  // RETURN A LIST WITH THE VALUES OF ALL PROPERTIES CHANGED BY THE USER
    QList<int>  getAllChangedFlags();   // RETURN A LIST WITH THE FLAGS OF ALL PROPERTIES CHANGED BY THE USER
    bool        applyRecursively();

    explicit DialogFSProp(QWidget *parent = 0);
    ~DialogFSProp();

private slots:
    void on_propList_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void on_propList_comboChanged(int index);
    void on_propList_itemChanged(QTreeWidgetItem *item, int column);
    void on_propList_customContextMenuRequested(const QPoint &pos);
    void on_propList_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void scrollToCurrentItemAndEdit();
    void scrollToCurrentItem();

    void on_buttonAdd_clicked();

    void on_buttonRemove_clicked();

    void on_buttonRevert_clicked();

    void on_buttonInherit_clicked();

    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::DialogFSProp *ui;
};

#endif // DIALOGFSPROP_H
