/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QStackedWidget>
#include <QtGui/QTabWidget>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <indicatorwidget.h>
#include <pbidetailswidget.h>
#include <pkgdetailswidget.h>
#include <sysdetailswidget.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionLast_system_update_log;
    QAction *actionLast_package_update_log;
    QAction *actionLast_software_update_log;
    QAction *actionUpdate_set;
    QAction *actionSystem_branches;
    QAction *actionIgnored_updates;
    QAction *actionExit;
    QAction *actionJail;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox_4;
    QHBoxLayout *horizontalLayout_13;
    QLabel *label_13;
    QLabel *label_14;
    QSpacerItem *horizontalSpacer_10;
    QPushButton *pushButton;
    QTabWidget *mainTab;
    QWidget *Main;
    QVBoxLayout *verticalLayout_2;
    QStackedWidget *mainStatesStack;
    QWidget *page_5;
    QVBoxLayout *verticalLayout_3;
    QSpacerItem *verticalSpacer;
    QGridLayout *gridLayout;
    QLabel *label;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *checkAllButton;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *verticalSpacer_2;
    QWidget *page_6;
    QVBoxLayout *verticalLayout_7;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_3;
    IndicatorWidget *sysIndicator;
    QFrame *line;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_4;
    IndicatorWidget *pkgIndicator;
    QFrame *line_2;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_5;
    IndicatorWidget *pbiIndicator;
    QFrame *line_3;
    QPushButton *updateAllButton;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_8;
    SysDetailsWidget *sysDetailsW;
    QWidget *tab;
    QVBoxLayout *verticalLayout_6;
    PkgDetailsWidget *pkgDetailsW;
    QWidget *tab_3;
    QVBoxLayout *verticalLayout_18;
    PBIDetailsWidget *pbiDetailsW;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuLogs;
    QMenu *menuControl;
    QMenu *menuRemote;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(631, 445);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/update-center.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        actionLast_system_update_log = new QAction(MainWindow);
        actionLast_system_update_log->setObjectName(QString::fromUtf8("actionLast_system_update_log"));
        actionLast_system_update_log->setEnabled(false);
        actionLast_package_update_log = new QAction(MainWindow);
        actionLast_package_update_log->setObjectName(QString::fromUtf8("actionLast_package_update_log"));
        actionLast_package_update_log->setEnabled(false);
        actionLast_software_update_log = new QAction(MainWindow);
        actionLast_software_update_log->setObjectName(QString::fromUtf8("actionLast_software_update_log"));
        actionLast_software_update_log->setEnabled(false);
        actionUpdate_set = new QAction(MainWindow);
        actionUpdate_set->setObjectName(QString::fromUtf8("actionUpdate_set"));
        actionUpdate_set->setEnabled(false);
        actionSystem_branches = new QAction(MainWindow);
        actionSystem_branches->setObjectName(QString::fromUtf8("actionSystem_branches"));
        actionSystem_branches->setEnabled(false);
        actionIgnored_updates = new QAction(MainWindow);
        actionIgnored_updates->setObjectName(QString::fromUtf8("actionIgnored_updates"));
        actionIgnored_updates->setEnabled(false);
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionExit->setEnabled(false);
        actionJail = new QAction(MainWindow);
        actionJail->setObjectName(QString::fromUtf8("actionJail"));
        actionJail->setEnabled(false);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox_4 = new QGroupBox(centralWidget);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        horizontalLayout_13 = new QHBoxLayout(groupBox_4);
        horizontalLayout_13->setSpacing(6);
        horizontalLayout_13->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        label_13 = new QLabel(groupBox_4);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setPixmap(QPixmap(QString::fromUtf8(":/images/beta.png")));

        horizontalLayout_13->addWidget(label_13);

        label_14 = new QLabel(groupBox_4);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        horizontalLayout_13->addWidget(label_14);

        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_13->addItem(horizontalSpacer_10);

        pushButton = new QPushButton(groupBox_4);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        horizontalLayout_13->addWidget(pushButton);


        verticalLayout->addWidget(groupBox_4);

        mainTab = new QTabWidget(centralWidget);
        mainTab->setObjectName(QString::fromUtf8("mainTab"));
        mainTab->setStyleSheet(QString::fromUtf8(""));
        mainTab->setDocumentMode(false);
        Main = new QWidget();
        Main->setObjectName(QString::fromUtf8("Main"));
        verticalLayout_2 = new QVBoxLayout(Main);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        mainStatesStack = new QStackedWidget(Main);
        mainStatesStack->setObjectName(QString::fromUtf8("mainStatesStack"));
        page_5 = new QWidget();
        page_5->setObjectName(QString::fromUtf8("page_5"));
        verticalLayout_3 = new QVBoxLayout(page_5);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalSpacer = new QSpacerItem(20, 82, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);

        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(page_5);
        label->setObjectName(QString::fromUtf8("label"));
        label->setPixmap(QPixmap(QString::fromUtf8(":/images/sysupdater.png")));

        gridLayout->addWidget(label, 0, 0, 2, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        label_2 = new QLabel(page_5);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout->addWidget(label_2);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        gridLayout->addLayout(horizontalLayout, 0, 1, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        checkAllButton = new QPushButton(page_5);
        checkAllButton->setObjectName(QString::fromUtf8("checkAllButton"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/view-refresh.png"), QSize(), QIcon::Normal, QIcon::Off);
        checkAllButton->setIcon(icon1);

        horizontalLayout_2->addWidget(checkAllButton);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);


        gridLayout->addLayout(horizontalLayout_2, 1, 1, 1, 1);


        verticalLayout_3->addLayout(gridLayout);

        verticalSpacer_2 = new QSpacerItem(20, 49, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_2);

        mainStatesStack->addWidget(page_5);
        page_6 = new QWidget();
        page_6->setObjectName(QString::fromUtf8("page_6"));
        verticalLayout_7 = new QVBoxLayout(page_6);
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setContentsMargins(11, 11, 11, 11);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        groupBox = new QGroupBox(page_6);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        horizontalLayout_3 = new QHBoxLayout(groupBox);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        sysIndicator = new IndicatorWidget(groupBox);
        sysIndicator->setObjectName(QString::fromUtf8("sysIndicator"));

        horizontalLayout_3->addWidget(sysIndicator);


        verticalLayout_7->addWidget(groupBox);

        line = new QFrame(page_6);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout_7->addWidget(line);

        groupBox_2 = new QGroupBox(page_6);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        verticalLayout_4 = new QVBoxLayout(groupBox_2);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        pkgIndicator = new IndicatorWidget(groupBox_2);
        pkgIndicator->setObjectName(QString::fromUtf8("pkgIndicator"));

        verticalLayout_4->addWidget(pkgIndicator);


        verticalLayout_7->addWidget(groupBox_2);

        line_2 = new QFrame(page_6);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        verticalLayout_7->addWidget(line_2);

        groupBox_3 = new QGroupBox(page_6);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        verticalLayout_5 = new QVBoxLayout(groupBox_3);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        pbiIndicator = new IndicatorWidget(groupBox_3);
        pbiIndicator->setObjectName(QString::fromUtf8("pbiIndicator"));

        verticalLayout_5->addWidget(pbiIndicator);


        verticalLayout_7->addWidget(groupBox_3);

        line_3 = new QFrame(page_6);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);

        verticalLayout_7->addWidget(line_3);

        updateAllButton = new QPushButton(page_6);
        updateAllButton->setObjectName(QString::fromUtf8("updateAllButton"));
        updateAllButton->setEnabled(false);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/install.png"), QSize(), QIcon::Normal, QIcon::Off);
        updateAllButton->setIcon(icon2);

        verticalLayout_7->addWidget(updateAllButton);

        mainStatesStack->addWidget(page_6);

        verticalLayout_2->addWidget(mainStatesStack);

        mainTab->addTab(Main, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        verticalLayout_8 = new QVBoxLayout(tab_2);
        verticalLayout_8->setSpacing(6);
        verticalLayout_8->setContentsMargins(11, 11, 11, 11);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        sysDetailsW = new SysDetailsWidget(tab_2);
        sysDetailsW->setObjectName(QString::fromUtf8("sysDetailsW"));

        verticalLayout_8->addWidget(sysDetailsW);

        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/sysupdates.png"), QSize(), QIcon::Normal, QIcon::Off);
        mainTab->addTab(tab_2, icon3, QString());
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        verticalLayout_6 = new QVBoxLayout(tab);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        pkgDetailsW = new PkgDetailsWidget(tab);
        pkgDetailsW->setObjectName(QString::fromUtf8("pkgDetailsW"));

        verticalLayout_6->addWidget(pkgDetailsW);

        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/images/pkgupdates.png"), QSize(), QIcon::Normal, QIcon::Off);
        mainTab->addTab(tab, icon4, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        verticalLayout_18 = new QVBoxLayout(tab_3);
        verticalLayout_18->setSpacing(6);
        verticalLayout_18->setContentsMargins(11, 11, 11, 11);
        verticalLayout_18->setObjectName(QString::fromUtf8("verticalLayout_18"));
        pbiDetailsW = new PBIDetailsWidget(tab_3);
        pbiDetailsW->setObjectName(QString::fromUtf8("pbiDetailsW"));

        verticalLayout_18->addWidget(pbiDetailsW);

        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/images/pbiupdates.png"), QSize(), QIcon::Normal, QIcon::Off);
        mainTab->addTab(tab_3, icon5, QString());

        verticalLayout->addWidget(mainTab);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 631, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuLogs = new QMenu(menuBar);
        menuLogs->setObjectName(QString::fromUtf8("menuLogs"));
        menuControl = new QMenu(menuBar);
        menuControl->setObjectName(QString::fromUtf8("menuControl"));
        menuRemote = new QMenu(menuBar);
        menuRemote->setObjectName(QString::fromUtf8("menuRemote"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuRemote->menuAction());
        menuBar->addAction(menuControl->menuAction());
        menuBar->addAction(menuLogs->menuAction());
        menuFile->addAction(actionExit);
        menuLogs->addAction(actionLast_system_update_log);
        menuLogs->addAction(actionLast_package_update_log);
        menuLogs->addAction(actionLast_software_update_log);
        menuControl->addAction(actionIgnored_updates);
        menuControl->addAction(actionUpdate_set);
        menuControl->addAction(actionSystem_branches);
        menuRemote->addAction(actionJail);

        retranslateUi(MainWindow);

        mainTab->setCurrentIndex(0);
        mainStatesStack->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "PC-BSD Update center", 0, QApplication::UnicodeUTF8));
        actionLast_system_update_log->setText(QApplication::translate("MainWindow", "Last system update log", 0, QApplication::UnicodeUTF8));
        actionLast_package_update_log->setText(QApplication::translate("MainWindow", "Last package update log", 0, QApplication::UnicodeUTF8));
        actionLast_software_update_log->setText(QApplication::translate("MainWindow", "Last software update log", 0, QApplication::UnicodeUTF8));
        actionUpdate_set->setText(QApplication::translate("MainWindow", "Update set", 0, QApplication::UnicodeUTF8));
        actionSystem_branches->setText(QApplication::translate("MainWindow", "System branches", 0, QApplication::UnicodeUTF8));
        actionIgnored_updates->setText(QApplication::translate("MainWindow", "Ignored updates", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", 0, QApplication::UnicodeUTF8));
        actionJail->setText(QApplication::translate("MainWindow", "Jail", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QString());
        label_13->setText(QString());
        label_14->setText(QApplication::translate("MainWindow", "This is early beta not full featured software for testing only!", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("MainWindow", "More details...", 0, QApplication::UnicodeUTF8));
        label->setText(QString());
        label_2->setText(QApplication::translate("MainWindow", "Your system is fully updated", 0, QApplication::UnicodeUTF8));
        checkAllButton->setText(QApplication::translate("MainWindow", "Check for updates", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("MainWindow", "System", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "Packages", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "Software", 0, QApplication::UnicodeUTF8));
        updateAllButton->setText(QApplication::translate("MainWindow", "Update all", 0, QApplication::UnicodeUTF8));
        mainTab->setTabText(mainTab->indexOf(Main), QApplication::translate("MainWindow", "Main", 0, QApplication::UnicodeUTF8));
        mainTab->setTabText(mainTab->indexOf(tab_2), QApplication::translate("MainWindow", "System update details", 0, QApplication::UnicodeUTF8));
        mainTab->setTabText(mainTab->indexOf(tab), QApplication::translate("MainWindow", "Package update details", 0, QApplication::UnicodeUTF8));
        mainTab->setTabText(mainTab->indexOf(tab_3), QApplication::translate("MainWindow", "Software update details", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
        menuLogs->setTitle(QApplication::translate("MainWindow", "Logs", 0, QApplication::UnicodeUTF8));
        menuControl->setTitle(QApplication::translate("MainWindow", "Control", 0, QApplication::UnicodeUTF8));
        menuRemote->setTitle(QApplication::translate("MainWindow", "Remote", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
