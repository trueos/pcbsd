/********************************************************************************
** Form generated from reading UI file 'softmanager-main.ui'
**
** Created: Tue Apr 2 18:15:11 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SOFTMANAGER_2D_MAIN_H
#define UI_SOFTMANAGER_2D_MAIN_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QStackedWidget>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QToolButton>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PBM
{
public:
    QAction *action_Quit;
    QAction *actionPreferences;
    QWidget *centralwidget;
    QGridLayout *gridLayout_2;
    QTabWidget *tabPBM;
    QWidget *tabSoftwareBrowser;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_2;
    QToolButton *toolBackButton;
    QToolButton *toolHomeButton;
    QSpacerItem *horizontalSpacer_4;
    QLabel *Searchlabel;
    QLineEdit *lineSearch;
    QPushButton *pushSearch;
    QStackedWidget *browseStack;
    QWidget *tab;
    QGridLayout *gridLayout_3;
    QHBoxLayout *horizontalLayout_4;
    QTreeWidget *SoftwareListBox;
    QHBoxLayout *horizontalLayout_10;
    QPushButton *pushUpdateAll;
    QSpacerItem *horizontalSpacer_7;
    QStackedWidget *AppInfoStack;
    QWidget *InstalledPage;
    QHBoxLayout *horizontalLayout_9;
    QLabel *AppIcon;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_5;
    QLabel *AppNameLabel;
    QSpacerItem *horizontalSpacer_5;
    QLabel *AppStateLabel;
    QHBoxLayout *horizontalLayout;
    QLabel *label_4;
    QLabel *AppVersionLabel;
    QLabel *NewVersionLabel;
    QLabel *AppUpdateVersionLabel;
    QPushButton *pushUpdatePBIs;
    QSpacerItem *horizontalSpacer_2;
    QToolButton *PBIMoreButton;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QPushButton *AppAuthorLabel;
    QLabel *label_2;
    QLabel *AppLicenseLabel;
    QSpacerItem *horizontalSpacer;
    QPushButton *RemoveButton;
    QWidget *page_2;
    QHBoxLayout *horizontalLayout_8;
    QLabel *AppIcon2;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_7;
    QLabel *AppNameLabel2;
    QSpacerItem *horizontalSpacer_6;
    QLabel *AppDownloadStateLabel;
    QProgressBar *PbiProgressBar;
    QHBoxLayout *horizontalLayout_6;
    QLabel *ProgressTextLabel;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *ProgressCancelButton;
    QMenuBar *menubar;
    QMenu *menu_File;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *PBM)
    {
        if (PBM->objectName().isEmpty())
            PBM->setObjectName(QString::fromUtf8("PBM"));
        PBM->resize(654, 543);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/appcafe.png"), QSize(), QIcon::Normal, QIcon::Off);
        PBM->setWindowIcon(icon);
        action_Quit = new QAction(PBM);
        action_Quit->setObjectName(QString::fromUtf8("action_Quit"));
        actionPreferences = new QAction(PBM);
        actionPreferences->setObjectName(QString::fromUtf8("actionPreferences"));
        centralwidget = new QWidget(PBM);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout_2 = new QGridLayout(centralwidget);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        tabPBM = new QTabWidget(centralwidget);
        tabPBM->setObjectName(QString::fromUtf8("tabPBM"));
        tabPBM->setIconSize(QSize(24, 24));
        tabSoftwareBrowser = new QWidget();
        tabSoftwareBrowser->setObjectName(QString::fromUtf8("tabSoftwareBrowser"));
        gridLayout = new QGridLayout(tabSoftwareBrowser);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        toolBackButton = new QToolButton(tabSoftwareBrowser);
        toolBackButton->setObjectName(QString::fromUtf8("toolBackButton"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/go-previous.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolBackButton->setIcon(icon1);
        toolBackButton->setIconSize(QSize(22, 22));
        toolBackButton->setAutoRaise(true);

        horizontalLayout_2->addWidget(toolBackButton);

        toolHomeButton = new QToolButton(tabSoftwareBrowser);
        toolHomeButton->setObjectName(QString::fromUtf8("toolHomeButton"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/go-home.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolHomeButton->setIcon(icon2);
        toolHomeButton->setIconSize(QSize(22, 22));
        toolHomeButton->setAutoRaise(true);

        horizontalLayout_2->addWidget(toolHomeButton);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);

        Searchlabel = new QLabel(tabSoftwareBrowser);
        Searchlabel->setObjectName(QString::fromUtf8("Searchlabel"));

        horizontalLayout_2->addWidget(Searchlabel);

        lineSearch = new QLineEdit(tabSoftwareBrowser);
        lineSearch->setObjectName(QString::fromUtf8("lineSearch"));

        horizontalLayout_2->addWidget(lineSearch);

        pushSearch = new QPushButton(tabSoftwareBrowser);
        pushSearch->setObjectName(QString::fromUtf8("pushSearch"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/edit-find.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushSearch->setIcon(icon3);
        pushSearch->setIconSize(QSize(22, 22));
        pushSearch->setAutoDefault(true);
        pushSearch->setDefault(false);
        pushSearch->setFlat(true);

        horizontalLayout_2->addWidget(pushSearch);


        gridLayout->addLayout(horizontalLayout_2, 0, 0, 1, 1);

        browseStack = new QStackedWidget(tabSoftwareBrowser);
        browseStack->setObjectName(QString::fromUtf8("browseStack"));
        browseStack->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));

        gridLayout->addWidget(browseStack, 1, 0, 1, 1);

        tabPBM->addTab(tabSoftwareBrowser, icon, QString());
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        gridLayout_3 = new QGridLayout(tab);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        SoftwareListBox = new QTreeWidget(tab);
        SoftwareListBox->setObjectName(QString::fromUtf8("SoftwareListBox"));
        SoftwareListBox->setIconSize(QSize(32, 32));
        SoftwareListBox->setTextElideMode(Qt::ElideRight);
        SoftwareListBox->setAutoExpandDelay(-1);
        SoftwareListBox->setIndentation(20);
        SoftwareListBox->setRootIsDecorated(false);
        SoftwareListBox->setUniformRowHeights(false);
        SoftwareListBox->setItemsExpandable(true);
        SoftwareListBox->setHeaderHidden(false);
        SoftwareListBox->header()->setCascadingSectionResizes(false);
        SoftwareListBox->header()->setStretchLastSection(true);

        horizontalLayout_4->addWidget(SoftwareListBox);


        gridLayout_3->addLayout(horizontalLayout_4, 0, 0, 1, 1);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        pushUpdateAll = new QPushButton(tab);
        pushUpdateAll->setObjectName(QString::fromUtf8("pushUpdateAll"));

        horizontalLayout_10->addWidget(pushUpdateAll);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_7);


        gridLayout_3->addLayout(horizontalLayout_10, 1, 0, 1, 1);

        AppInfoStack = new QStackedWidget(tab);
        AppInfoStack->setObjectName(QString::fromUtf8("AppInfoStack"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(AppInfoStack->sizePolicy().hasHeightForWidth());
        AppInfoStack->setSizePolicy(sizePolicy);
        InstalledPage = new QWidget();
        InstalledPage->setObjectName(QString::fromUtf8("InstalledPage"));
        horizontalLayout_9 = new QHBoxLayout(InstalledPage);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        AppIcon = new QLabel(InstalledPage);
        AppIcon->setObjectName(QString::fromUtf8("AppIcon"));
        AppIcon->setMinimumSize(QSize(64, 64));
        AppIcon->setMaximumSize(QSize(64, 64));
        AppIcon->setPixmap(QPixmap(QString::fromUtf8(":/application.png")));
        AppIcon->setScaledContents(true);
        AppIcon->setWordWrap(false);

        horizontalLayout_9->addWidget(AppIcon);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        AppNameLabel = new QLabel(InstalledPage);
        AppNameLabel->setObjectName(QString::fromUtf8("AppNameLabel"));
        QFont font;
        font.setBold(true);
        font.setUnderline(true);
        font.setWeight(75);
        AppNameLabel->setFont(font);

        horizontalLayout_5->addWidget(AppNameLabel);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_5);

        AppStateLabel = new QLabel(InstalledPage);
        AppStateLabel->setObjectName(QString::fromUtf8("AppStateLabel"));

        horizontalLayout_5->addWidget(AppStateLabel);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_4 = new QLabel(InstalledPage);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout->addWidget(label_4);

        AppVersionLabel = new QLabel(InstalledPage);
        AppVersionLabel->setObjectName(QString::fromUtf8("AppVersionLabel"));

        horizontalLayout->addWidget(AppVersionLabel);

        NewVersionLabel = new QLabel(InstalledPage);
        NewVersionLabel->setObjectName(QString::fromUtf8("NewVersionLabel"));
        QFont font1;
        font1.setBold(true);
        font1.setWeight(75);
        NewVersionLabel->setFont(font1);

        horizontalLayout->addWidget(NewVersionLabel);

        AppUpdateVersionLabel = new QLabel(InstalledPage);
        AppUpdateVersionLabel->setObjectName(QString::fromUtf8("AppUpdateVersionLabel"));
        AppUpdateVersionLabel->setFont(font1);

        horizontalLayout->addWidget(AppUpdateVersionLabel);

        pushUpdatePBIs = new QPushButton(InstalledPage);
        pushUpdatePBIs->setObjectName(QString::fromUtf8("pushUpdatePBIs"));

        horizontalLayout->addWidget(pushUpdatePBIs);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        PBIMoreButton = new QToolButton(InstalledPage);
        PBIMoreButton->setObjectName(QString::fromUtf8("PBIMoreButton"));
        PBIMoreButton->setPopupMode(QToolButton::MenuButtonPopup);

        horizontalLayout->addWidget(PBIMoreButton);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_3 = new QLabel(InstalledPage);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_3->addWidget(label_3);

        AppAuthorLabel = new QPushButton(InstalledPage);
        AppAuthorLabel->setObjectName(QString::fromUtf8("AppAuthorLabel"));
        QFont font2;
        font2.setUnderline(true);
        AppAuthorLabel->setFont(font2);
        AppAuthorLabel->setCursor(QCursor(Qt::PointingHandCursor));
        AppAuthorLabel->setFlat(true);

        horizontalLayout_3->addWidget(AppAuthorLabel);

        label_2 = new QLabel(InstalledPage);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_3->addWidget(label_2);

        AppLicenseLabel = new QLabel(InstalledPage);
        AppLicenseLabel->setObjectName(QString::fromUtf8("AppLicenseLabel"));

        horizontalLayout_3->addWidget(AppLicenseLabel);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        RemoveButton = new QPushButton(InstalledPage);
        RemoveButton->setObjectName(QString::fromUtf8("RemoveButton"));

        horizontalLayout_3->addWidget(RemoveButton);


        verticalLayout->addLayout(horizontalLayout_3);


        horizontalLayout_9->addLayout(verticalLayout);

        AppInfoStack->addWidget(InstalledPage);
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        horizontalLayout_8 = new QHBoxLayout(page_2);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        AppIcon2 = new QLabel(page_2);
        AppIcon2->setObjectName(QString::fromUtf8("AppIcon2"));
        AppIcon2->setMinimumSize(QSize(64, 64));
        AppIcon2->setMaximumSize(QSize(64, 64));
        AppIcon2->setPixmap(QPixmap(QString::fromUtf8(":/application.png")));
        AppIcon2->setScaledContents(true);
        AppIcon2->setWordWrap(false);

        horizontalLayout_8->addWidget(AppIcon2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        AppNameLabel2 = new QLabel(page_2);
        AppNameLabel2->setObjectName(QString::fromUtf8("AppNameLabel2"));
        AppNameLabel2->setFont(font);

        horizontalLayout_7->addWidget(AppNameLabel2);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_6);

        AppDownloadStateLabel = new QLabel(page_2);
        AppDownloadStateLabel->setObjectName(QString::fromUtf8("AppDownloadStateLabel"));

        horizontalLayout_7->addWidget(AppDownloadStateLabel);


        verticalLayout_3->addLayout(horizontalLayout_7);

        PbiProgressBar = new QProgressBar(page_2);
        PbiProgressBar->setObjectName(QString::fromUtf8("PbiProgressBar"));
        PbiProgressBar->setValue(0);
        PbiProgressBar->setInvertedAppearance(false);

        verticalLayout_3->addWidget(PbiProgressBar);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        ProgressTextLabel = new QLabel(page_2);
        ProgressTextLabel->setObjectName(QString::fromUtf8("ProgressTextLabel"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(ProgressTextLabel->sizePolicy().hasHeightForWidth());
        ProgressTextLabel->setSizePolicy(sizePolicy1);
        ProgressTextLabel->setMaximumSize(QSize(400, 16777215));

        horizontalLayout_6->addWidget(ProgressTextLabel);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_3);

        ProgressCancelButton = new QPushButton(page_2);
        ProgressCancelButton->setObjectName(QString::fromUtf8("ProgressCancelButton"));

        horizontalLayout_6->addWidget(ProgressCancelButton);


        verticalLayout_3->addLayout(horizontalLayout_6);


        horizontalLayout_8->addLayout(verticalLayout_3);

        AppInfoStack->addWidget(page_2);

        gridLayout_3->addWidget(AppInfoStack, 2, 0, 1, 1);

        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/application.png"), QSize(), QIcon::Normal, QIcon::Off);
        tabPBM->addTab(tab, icon4, QString());

        gridLayout_2->addWidget(tabPBM, 0, 0, 1, 1);

        PBM->setCentralWidget(centralwidget);
        menubar = new QMenuBar(PBM);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 654, 22));
        menu_File = new QMenu(menubar);
        menu_File->setObjectName(QString::fromUtf8("menu_File"));
        PBM->setMenuBar(menubar);
        statusbar = new QStatusBar(PBM);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        PBM->setStatusBar(statusbar);

        menubar->addAction(menu_File->menuAction());
        menu_File->addAction(actionPreferences);
        menu_File->addSeparator();
        menu_File->addAction(action_Quit);

        retranslateUi(PBM);

        tabPBM->setCurrentIndex(0);
        browseStack->setCurrentIndex(-1);
        AppInfoStack->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(PBM);
    } // setupUi

    void retranslateUi(QMainWindow *PBM)
    {
        PBM->setWindowTitle(QApplication::translate("PBM", "AppCafe", 0, QApplication::UnicodeUTF8));
        action_Quit->setText(QApplication::translate("PBM", "&Quit", 0, QApplication::UnicodeUTF8));
        actionPreferences->setText(QApplication::translate("PBM", "Preferences", 0, QApplication::UnicodeUTF8));
        toolBackButton->setText(QApplication::translate("PBM", "...", 0, QApplication::UnicodeUTF8));
        toolHomeButton->setText(QApplication::translate("PBM", "...", 0, QApplication::UnicodeUTF8));
        Searchlabel->setText(QApplication::translate("PBM", "Search", 0, QApplication::UnicodeUTF8));
        pushSearch->setText(QString());
        tabPBM->setTabText(tabPBM->indexOf(tabSoftwareBrowser), QApplication::translate("PBM", "Browse", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem = SoftwareListBox->headerItem();
        ___qtreewidgetitem->setText(1, QApplication::translate("PBM", "Status", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(0, QApplication::translate("PBM", "Program Name", 0, QApplication::UnicodeUTF8));
        pushUpdateAll->setText(QApplication::translate("PBM", "Update &All", 0, QApplication::UnicodeUTF8));
        AppNameLabel->setText(QApplication::translate("PBM", "Application name", 0, QApplication::UnicodeUTF8));
        AppStateLabel->setText(QApplication::translate("PBM", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("PBM", "Version:", 0, QApplication::UnicodeUTF8));
        AppVersionLabel->setText(QApplication::translate("PBM", "1.20.19", 0, QApplication::UnicodeUTF8));
        NewVersionLabel->setText(QApplication::translate("PBM", "New version available:", 0, QApplication::UnicodeUTF8));
        AppUpdateVersionLabel->setText(QApplication::translate("PBM", "1.20.20", 0, QApplication::UnicodeUTF8));
        pushUpdatePBIs->setText(QApplication::translate("PBM", "Update", 0, QApplication::UnicodeUTF8));
        PBIMoreButton->setText(QApplication::translate("PBM", "More...", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("PBM", "Author:", 0, QApplication::UnicodeUTF8));
        AppAuthorLabel->setText(QString());
        label_2->setText(QApplication::translate("PBM", "License:", 0, QApplication::UnicodeUTF8));
        AppLicenseLabel->setText(QApplication::translate("PBM", "BSD", 0, QApplication::UnicodeUTF8));
        RemoveButton->setText(QApplication::translate("PBM", "&Remove", 0, QApplication::UnicodeUTF8));
        AppNameLabel2->setText(QApplication::translate("PBM", "Application name", 0, QApplication::UnicodeUTF8));
        AppDownloadStateLabel->setText(QString());
        ProgressTextLabel->setText(QString());
        ProgressCancelButton->setText(QApplication::translate("PBM", "Cancel", 0, QApplication::UnicodeUTF8));
        tabPBM->setTabText(tabPBM->indexOf(tab), QApplication::translate("PBM", "&Installed", 0, QApplication::UnicodeUTF8));
        menu_File->setTitle(QApplication::translate("PBM", "&File", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class PBM: public Ui_PBM {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SOFTMANAGER_2D_MAIN_H
