#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCheckBox>
#include <QVector>

#include "backend/cp-itemgroup.h"
#include "controls/qautoexpandlist.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    //! Structure that bind items group backed with widgets
    typedef struct _SUIItemsGroup
    {
        CItemGroup*      mItemGroup;
        QAutoExpandList* mListWidget;
        QCheckBox*       mGroupNameWidget;
        QVector<CControlPanelItem> mItems;
        _SUIItemsGroup(CItemGroup* pbackend=NULL, QAutoExpandList* lw=NULL,QCheckBox* capt=NULL)
        {
            mItemGroup = pbackend; mListWidget= lw; mGroupNameWidget= capt;
        }
    }SUIItemsGroup;
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;

    SUIItemsGroup mItemGropus [6];
    QStringList   mEnabledDEs;

    void setupDEChooser();
    void setupGroups();

    void fillGroupWidget(SUIItemsGroup* itemsGroup);
    void repaintGroupWidget(SUIItemsGroup* itemsGroup);

private slots:
    void slotItemsReady();
    void slotDEChooserActionTriggered ();
    void slotGropTextStateChanged(int state);
    void slotIconSizeActionTriggered();
    void slotViewModeActionTriggered();
    void on_refreshButton_clicked();
    void on_filterEdit_textChanged(const QString &arg1);
    void slotItemActivated ( QListWidgetItem* item );
    void on_actionSystem_only_triggered();
    void on_actionAll_desktops_triggered();
};

#endif // MAINWINDOW_H
