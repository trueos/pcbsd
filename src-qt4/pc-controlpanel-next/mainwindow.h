#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCheckBox>

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

    void fillGroupWidget(SUIItemsGroup* itemsGroup );

private slots:
    void slotItemsReady();
    void slotDEChooserActionTriggered ( bool checked );
    void slotGropTextStateChanged(int state);
    void slotIconSizeActionTriggered();
    void slotViewModeActionTriggered();
    void on_refreshButton_clicked();
};

#endif // MAINWINDOW_H
