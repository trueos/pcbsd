#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCheckBox>
#include <QVector>
#include <QLabel>

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
        QIcon*           mGroupIcon;
        QLabel*          mLoadingIcon;
        QVector<CControlPanelItem> mItems;
        bool             mStoredNameState;        
        _SUIItemsGroup(CItemGroup* pbackend=NULL, QAutoExpandList* lw=NULL,QCheckBox* capt=NULL, QIcon* icon = NULL, QLabel* loading_label= NULL)
        {
            mItemGroup = pbackend; mListWidget= lw; mGroupNameWidget= capt;
            mGroupIcon = icon;
            mLoadingIcon = loading_label;
            mStoredNameState= true;
        }
    }SUIItemsGroup;
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
protected:
    virtual void closeEvent(QCloseEvent *event);
private:
    Ui::MainWindow *ui;

    SUIItemsGroup mItemGropus [6];
    QStringList   mEnabledDEs;

    int mLastFilterLength;

    bool misSettingsSystemOnly;
    bool misSettingsFixedLayout;
    int  mGroupsLoaded;

    void setupLoadingScreen();
    void setupDEChooser();
    void setupGroups();
    void loadSettings();
    void saveSettings();
    bool checkUserGroup(QString groupName);

    void fillGroupWidget(SUIItemsGroup* itemsGroup);
    void repaintGroupWidget(SUIItemsGroup* itemsGroup);
    void setBigIcons(bool isBig);
    void setListMode(bool isListMode);
    void setFixedItemsLayout(bool isFixedLayout);

    void onStartupFinished();

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

    void slotSingleInstance();
    void on_actionFixed_item_width_triggered();
};

#endif // MAINWINDOW_H
