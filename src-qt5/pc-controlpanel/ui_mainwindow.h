/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <controls/qautoexpandlist.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionNormal_icons;
    QAction *actionLarge_icons;
    QAction *actionGrid_view;
    QAction *actionList_view;
    QAction *actionSystem_only;
    QAction *actionAll_desktops;
    QAction *actionFixed_item_width;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout;
    QToolButton *refreshButton;
    QToolButton *DEChooserButton;
    QToolButton *DELaunchConfigApp;
    QSpacerItem *horizontalSpacer;
    QToolButton *viewModeButton;
    QLineEdit *filterEdit;
    QStackedWidget *mainStack;
    QWidget *page;
    QVBoxLayout *verticalLayout_4;
    QSpacerItem *verticalSpacer_3;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QLabel *loadingSoftware;
    QLabel *loadingSystem;
    QLabel *loadingHardware;
    QLabel *loadingNetwork;
    QLabel *loadingDE;
    QLabel *loadingTools;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_4;
    QLabel *label_6;
    QSpacerItem *horizontalSpacer_5;
    QSpacerItem *verticalSpacer_2;
    QWidget *page_2;
    QVBoxLayout *verticalLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_2;
    QCheckBox *softwareGroupName;
    QAutoExpandList *softwareLW;
    QCheckBox *systemGroupName;
    QAutoExpandList *systemLW;
    QCheckBox *hardwareGroupName;
    QAutoExpandList *hardwareLW;
    QCheckBox *networkingGroupName;
    QAutoExpandList *networkingLW;
    QCheckBox *deGroupName;
    QAutoExpandList *deLW;
    QCheckBox *toolsGroupName;
    QAutoExpandList *toolsLW;
    QSpacerItem *verticalSpacer;
    QMenuBar *menuBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(639, 464);
        QIcon icon;
        icon.addFile(QStringLiteral(":/icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        actionNormal_icons = new QAction(MainWindow);
        actionNormal_icons->setObjectName(QStringLiteral("actionNormal_icons"));
        actionNormal_icons->setCheckable(true);
        actionNormal_icons->setChecked(true);
        actionLarge_icons = new QAction(MainWindow);
        actionLarge_icons->setObjectName(QStringLiteral("actionLarge_icons"));
        actionLarge_icons->setCheckable(true);
        actionGrid_view = new QAction(MainWindow);
        actionGrid_view->setObjectName(QStringLiteral("actionGrid_view"));
        actionGrid_view->setCheckable(true);
        actionGrid_view->setChecked(true);
        actionList_view = new QAction(MainWindow);
        actionList_view->setObjectName(QStringLiteral("actionList_view"));
        actionList_view->setCheckable(true);
        actionSystem_only = new QAction(MainWindow);
        actionSystem_only->setObjectName(QStringLiteral("actionSystem_only"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/images/system_only.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSystem_only->setIcon(icon1);
        actionAll_desktops = new QAction(MainWindow);
        actionAll_desktops->setObjectName(QStringLiteral("actionAll_desktops"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/images/all_desktops.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAll_desktops->setIcon(icon2);
        actionFixed_item_width = new QAction(MainWindow);
        actionFixed_item_width->setObjectName(QStringLiteral("actionFixed_item_width"));
        actionFixed_item_width->setCheckable(true);
        actionFixed_item_width->setEnabled(true);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout_3 = new QVBoxLayout(centralWidget);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        refreshButton = new QToolButton(centralWidget);
        refreshButton->setObjectName(QStringLiteral("refreshButton"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/images/refresh.png"), QSize(), QIcon::Normal, QIcon::Off);
        refreshButton->setIcon(icon3);
        refreshButton->setIconSize(QSize(24, 24));
        refreshButton->setAutoRaise(true);

        horizontalLayout->addWidget(refreshButton);

        DEChooserButton = new QToolButton(centralWidget);
        DEChooserButton->setObjectName(QStringLiteral("DEChooserButton"));
        DEChooserButton->setContextMenuPolicy(Qt::ActionsContextMenu);
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/images/kde.png"), QSize(), QIcon::Normal, QIcon::Off);
        DEChooserButton->setIcon(icon4);
        DEChooserButton->setIconSize(QSize(24, 24));
        DEChooserButton->setPopupMode(QToolButton::InstantPopup);
        DEChooserButton->setAutoRaise(true);
        DEChooserButton->setArrowType(Qt::NoArrow);

        horizontalLayout->addWidget(DEChooserButton);

        DELaunchConfigApp = new QToolButton(centralWidget);
        DELaunchConfigApp->setObjectName(QStringLiteral("DELaunchConfigApp"));
        DELaunchConfigApp->setIcon(icon4);
        DELaunchConfigApp->setIconSize(QSize(24, 24));
        DELaunchConfigApp->setAutoRaise(true);

        horizontalLayout->addWidget(DELaunchConfigApp);

        horizontalSpacer = new QSpacerItem(338, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        viewModeButton = new QToolButton(centralWidget);
        viewModeButton->setObjectName(QStringLiteral("viewModeButton"));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/images/view_settings.png"), QSize(), QIcon::Normal, QIcon::Off);
        viewModeButton->setIcon(icon5);
        viewModeButton->setIconSize(QSize(24, 24));
        viewModeButton->setPopupMode(QToolButton::InstantPopup);
        viewModeButton->setAutoRaise(true);
        viewModeButton->setArrowType(Qt::NoArrow);

        horizontalLayout->addWidget(viewModeButton);

        filterEdit = new QLineEdit(centralWidget);
        filterEdit->setObjectName(QStringLiteral("filterEdit"));

        horizontalLayout->addWidget(filterEdit);


        verticalLayout_3->addLayout(horizontalLayout);

        mainStack = new QStackedWidget(centralWidget);
        mainStack->setObjectName(QStringLiteral("mainStack"));
        mainStack->setLineWidth(0);
        page = new QWidget();
        page->setObjectName(QStringLiteral("page"));
        verticalLayout_4 = new QVBoxLayout(page);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalSpacer_3 = new QSpacerItem(20, 120, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        loadingSoftware = new QLabel(page);
        loadingSoftware->setObjectName(QStringLiteral("loadingSoftware"));
        loadingSoftware->setPixmap(QPixmap(QString::fromUtf8(":/images/loading-software.png")));
        loadingSoftware->setScaledContents(true);

        horizontalLayout_2->addWidget(loadingSoftware);

        loadingSystem = new QLabel(page);
        loadingSystem->setObjectName(QStringLiteral("loadingSystem"));
        loadingSystem->setPixmap(QPixmap(QString::fromUtf8(":/images/system_only.png")));
        loadingSystem->setScaledContents(true);

        horizontalLayout_2->addWidget(loadingSystem);

        loadingHardware = new QLabel(page);
        loadingHardware->setObjectName(QStringLiteral("loadingHardware"));
        loadingHardware->setPixmap(QPixmap(QString::fromUtf8(":/images/loading-hardware.png")));

        horizontalLayout_2->addWidget(loadingHardware);

        loadingNetwork = new QLabel(page);
        loadingNetwork->setObjectName(QStringLiteral("loadingNetwork"));
        loadingNetwork->setPixmap(QPixmap(QString::fromUtf8(":/images/loading-network.png")));
        loadingNetwork->setScaledContents(true);

        horizontalLayout_2->addWidget(loadingNetwork);

        loadingDE = new QLabel(page);
        loadingDE->setObjectName(QStringLiteral("loadingDE"));
        loadingDE->setPixmap(QPixmap(QString::fromUtf8(":/images/kde.png")));
        loadingDE->setScaledContents(true);

        horizontalLayout_2->addWidget(loadingDE);

        loadingTools = new QLabel(page);
        loadingTools->setObjectName(QStringLiteral("loadingTools"));
        loadingTools->setPixmap(QPixmap(QString::fromUtf8(":/images/loading-tools.png")));
        loadingTools->setScaledContents(true);

        horizontalLayout_2->addWidget(loadingTools);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);


        verticalLayout_4->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_4);

        label_6 = new QLabel(page);
        label_6->setObjectName(QStringLiteral("label_6"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label_6->setFont(font);

        horizontalLayout_3->addWidget(label_6);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_5);


        verticalLayout_4->addLayout(horizontalLayout_3);

        verticalSpacer_2 = new QSpacerItem(20, 120, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_2);

        mainStack->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QStringLiteral("page_2"));
        verticalLayout = new QVBoxLayout(page_2);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        scrollArea = new QScrollArea(page_2);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 272, 1324));
        verticalLayout_2 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        softwareGroupName = new QCheckBox(scrollAreaWidgetContents);
        softwareGroupName->setObjectName(QStringLiteral("softwareGroupName"));
        softwareGroupName->setFont(font);
        softwareGroupName->setAutoFillBackground(false);
        softwareGroupName->setStyleSheet(QLatin1String("QCheckBox{ background-color: rgb(160, 160, 160)}\n"
"QCheckBox::indicator:unchecked{\n"
"image: url(:images/arrow-down.png);\n"
"}\n"
"QCheckBox::indicator:checked{\n"
"image: url(:images/arrow-up.png);\n"
"}"));
        softwareGroupName->setChecked(true);
        softwareGroupName->setAutoExclusive(false);

        verticalLayout_2->addWidget(softwareGroupName);

        softwareLW = new QAutoExpandList(scrollAreaWidgetContents);
        softwareLW->setObjectName(QStringLiteral("softwareLW"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(softwareLW->sizePolicy().hasHeightForWidth());
        softwareLW->setSizePolicy(sizePolicy);
        softwareLW->setWordWrap(true);
        softwareLW->setSortingEnabled(true);

        verticalLayout_2->addWidget(softwareLW);

        systemGroupName = new QCheckBox(scrollAreaWidgetContents);
        systemGroupName->setObjectName(QStringLiteral("systemGroupName"));
        systemGroupName->setFont(font);
        systemGroupName->setAutoFillBackground(false);
        systemGroupName->setStyleSheet(QLatin1String("QCheckBox{ background-color: rgb(160, 160, 160)}\n"
"QCheckBox::indicator:unchecked{\n"
"image: url(:images/arrow-down.png);\n"
"}\n"
"QCheckBox::indicator:checked{\n"
"image: url(:images/arrow-up.png);\n"
"}"));
        systemGroupName->setChecked(true);

        verticalLayout_2->addWidget(systemGroupName);

        systemLW = new QAutoExpandList(scrollAreaWidgetContents);
        systemLW->setObjectName(QStringLiteral("systemLW"));
        sizePolicy.setHeightForWidth(systemLW->sizePolicy().hasHeightForWidth());
        systemLW->setSizePolicy(sizePolicy);
        systemLW->setWordWrap(true);
        systemLW->setSortingEnabled(true);

        verticalLayout_2->addWidget(systemLW);

        hardwareGroupName = new QCheckBox(scrollAreaWidgetContents);
        hardwareGroupName->setObjectName(QStringLiteral("hardwareGroupName"));
        hardwareGroupName->setFont(font);
        hardwareGroupName->setAutoFillBackground(false);
        hardwareGroupName->setStyleSheet(QLatin1String("QCheckBox{ background-color: rgb(160, 160, 160)}\n"
"QCheckBox::indicator:unchecked{\n"
"image: url(:images/arrow-down.png);\n"
"}\n"
"QCheckBox::indicator:checked{\n"
"image: url(:images/arrow-up.png);\n"
"}"));
        hardwareGroupName->setChecked(true);

        verticalLayout_2->addWidget(hardwareGroupName);

        hardwareLW = new QAutoExpandList(scrollAreaWidgetContents);
        hardwareLW->setObjectName(QStringLiteral("hardwareLW"));
        sizePolicy.setHeightForWidth(hardwareLW->sizePolicy().hasHeightForWidth());
        hardwareLW->setSizePolicy(sizePolicy);
        hardwareLW->setWordWrap(true);
        hardwareLW->setSortingEnabled(true);

        verticalLayout_2->addWidget(hardwareLW);

        networkingGroupName = new QCheckBox(scrollAreaWidgetContents);
        networkingGroupName->setObjectName(QStringLiteral("networkingGroupName"));
        networkingGroupName->setFont(font);
        networkingGroupName->setAutoFillBackground(false);
        networkingGroupName->setStyleSheet(QLatin1String("QCheckBox{ background-color: rgb(160, 160, 160)}\n"
"QCheckBox::indicator:unchecked{\n"
"image: url(:images/arrow-down.png);\n"
"}\n"
"QCheckBox::indicator:checked{\n"
"image: url(:images/arrow-up.png);\n"
"}"));
        networkingGroupName->setChecked(true);

        verticalLayout_2->addWidget(networkingGroupName);

        networkingLW = new QAutoExpandList(scrollAreaWidgetContents);
        networkingLW->setObjectName(QStringLiteral("networkingLW"));
        sizePolicy.setHeightForWidth(networkingLW->sizePolicy().hasHeightForWidth());
        networkingLW->setSizePolicy(sizePolicy);
        networkingLW->setWordWrap(true);
        networkingLW->setSortingEnabled(true);

        verticalLayout_2->addWidget(networkingLW);

        deGroupName = new QCheckBox(scrollAreaWidgetContents);
        deGroupName->setObjectName(QStringLiteral("deGroupName"));
        deGroupName->setFont(font);
        deGroupName->setAutoFillBackground(false);
        deGroupName->setStyleSheet(QLatin1String("QCheckBox{ background-color: rgb(160, 160, 160)}\n"
"QCheckBox::indicator:unchecked{\n"
"image: url(:images/arrow-down.png);\n"
"}\n"
"QCheckBox::indicator:checked{\n"
"image: url(:images/arrow-up.png);\n"
"}"));
        deGroupName->setChecked(true);

        verticalLayout_2->addWidget(deGroupName);

        deLW = new QAutoExpandList(scrollAreaWidgetContents);
        deLW->setObjectName(QStringLiteral("deLW"));
        sizePolicy.setHeightForWidth(deLW->sizePolicy().hasHeightForWidth());
        deLW->setSizePolicy(sizePolicy);
        deLW->setWordWrap(true);
        deLW->setSortingEnabled(true);

        verticalLayout_2->addWidget(deLW);

        toolsGroupName = new QCheckBox(scrollAreaWidgetContents);
        toolsGroupName->setObjectName(QStringLiteral("toolsGroupName"));
        toolsGroupName->setFont(font);
        toolsGroupName->setAutoFillBackground(false);
        toolsGroupName->setStyleSheet(QLatin1String("QCheckBox{ background-color: rgb(160, 160, 160)}\n"
"QCheckBox::indicator:unchecked{\n"
"image: url(:images/arrow-down.png);\n"
"}\n"
"QCheckBox::indicator:checked{\n"
"image: url(:images/arrow-up.png);\n"
"}"));
        toolsGroupName->setChecked(true);

        verticalLayout_2->addWidget(toolsGroupName);

        toolsLW = new QAutoExpandList(scrollAreaWidgetContents);
        toolsLW->setObjectName(QStringLiteral("toolsLW"));
        sizePolicy.setHeightForWidth(toolsLW->sizePolicy().hasHeightForWidth());
        toolsLW->setSizePolicy(sizePolicy);
        toolsLW->setWordWrap(true);
        toolsLW->setSortingEnabled(true);

        verticalLayout_2->addWidget(toolsLW);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout->addWidget(scrollArea);

        mainStack->addWidget(page_2);

        verticalLayout_3->addWidget(mainStack);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 639, 18));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        mainStack->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "PC-BSD Control Panel", 0));
        actionNormal_icons->setText(QApplication::translate("MainWindow", "Normal icons", 0));
        actionLarge_icons->setText(QApplication::translate("MainWindow", "Large icons", 0));
        actionGrid_view->setText(QApplication::translate("MainWindow", "Grid view", 0));
        actionList_view->setText(QApplication::translate("MainWindow", "List view", 0));
        actionSystem_only->setText(QApplication::translate("MainWindow", "System only", 0));
        actionAll_desktops->setText(QApplication::translate("MainWindow", "All desktops", 0));
        actionFixed_item_width->setText(QApplication::translate("MainWindow", "Fixed items layout", 0));
        refreshButton->setText(QString());
        DEChooserButton->setText(QString());
        DELaunchConfigApp->setText(QString());
        viewModeButton->setText(QString());
        filterEdit->setPlaceholderText(QApplication::translate("MainWindow", "What do you want to configure?", 0));
        loadingSoftware->setText(QString());
        loadingSystem->setText(QString());
        loadingHardware->setText(QString());
        loadingNetwork->setText(QString());
        loadingDE->setText(QString());
        loadingTools->setText(QString());
        label_6->setText(QApplication::translate("MainWindow", "Loading items...", 0));
        softwareGroupName->setText(QApplication::translate("MainWindow", "Software and updates", 0));
        systemGroupName->setText(QApplication::translate("MainWindow", "System management", 0));
        hardwareGroupName->setText(QApplication::translate("MainWindow", "Hardware", 0));
        networkingGroupName->setText(QApplication::translate("MainWindow", "Networking", 0));
        deGroupName->setText(QApplication::translate("MainWindow", "Desktop environment", 0));
        toolsGroupName->setText(QApplication::translate("MainWindow", "Tools", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
