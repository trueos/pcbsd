/********************************************************************************
** Form generated from reading UI file 'mainUI.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINUI_H
#define UI_MAINUI_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainUI
{
public:
    QAction *actionSave_and_Quit;
    QAction *actionClose;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QSpinBox *spinBox;
    QSpacerItem *horizontalSpacer_2;
    QTabWidget *tabWidget;
    QWidget *tab_background;
    QGridLayout *gridLayout;
    QListWidget *list_backgrounds;
    QLabel *label_BGImage;
    QHBoxLayout *horizontalLayout_3;
    QToolButton *tool_addbackground;
    QToolButton *tool_rmbackground;
    QSpacerItem *horizontalSpacer_3;
    QRadioButton *radio_singleBG;
    QRadioButton *radio_rotateBG;
    QSpinBox *spinBox_2;
    QWidget *tab_deskplugins;
    QWidget *tab;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_2;
    QPushButton *push_save;
    QMenuBar *menubar;
    QMenu *menuFile;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainUI)
    {
        if (MainUI->objectName().isEmpty())
            MainUI->setObjectName(QString::fromUtf8("MainUI"));
        MainUI->resize(481, 376);
        MainUI->setUnifiedTitleAndToolBarOnMac(false);
        actionSave_and_Quit = new QAction(MainUI);
        actionSave_and_Quit->setObjectName(QString::fromUtf8("actionSave_and_Quit"));
        actionClose = new QAction(MainUI);
        actionClose->setObjectName(QString::fromUtf8("actionClose"));
        centralwidget = new QWidget(MainUI);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);

        horizontalLayout_2->addWidget(label);

        spinBox = new QSpinBox(centralwidget);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));

        horizontalLayout_2->addWidget(spinBox);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout_2);

        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setEnabled(true);
        tab_background = new QWidget();
        tab_background->setObjectName(QString::fromUtf8("tab_background"));
        gridLayout = new QGridLayout(tab_background);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        list_backgrounds = new QListWidget(tab_background);
        list_backgrounds->setObjectName(QString::fromUtf8("list_backgrounds"));

        gridLayout->addWidget(list_backgrounds, 1, 0, 1, 1);

        label_BGImage = new QLabel(tab_background);
        label_BGImage->setObjectName(QString::fromUtf8("label_BGImage"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_BGImage->sizePolicy().hasHeightForWidth());
        label_BGImage->setSizePolicy(sizePolicy);
        label_BGImage->setFrameShape(QFrame::StyledPanel);
        label_BGImage->setFrameShadow(QFrame::Raised);
        label_BGImage->setScaledContents(true);

        gridLayout->addWidget(label_BGImage, 1, 1, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        tool_addbackground = new QToolButton(tab_background);
        tool_addbackground->setObjectName(QString::fromUtf8("tool_addbackground"));

        horizontalLayout_3->addWidget(tool_addbackground);

        tool_rmbackground = new QToolButton(tab_background);
        tool_rmbackground->setObjectName(QString::fromUtf8("tool_rmbackground"));

        horizontalLayout_3->addWidget(tool_rmbackground);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        radio_singleBG = new QRadioButton(tab_background);
        radio_singleBG->setObjectName(QString::fromUtf8("radio_singleBG"));

        horizontalLayout_3->addWidget(radio_singleBG);

        radio_rotateBG = new QRadioButton(tab_background);
        radio_rotateBG->setObjectName(QString::fromUtf8("radio_rotateBG"));

        horizontalLayout_3->addWidget(radio_rotateBG);

        spinBox_2 = new QSpinBox(tab_background);
        spinBox_2->setObjectName(QString::fromUtf8("spinBox_2"));
        spinBox_2->setMinimum(2);
        spinBox_2->setValue(10);

        horizontalLayout_3->addWidget(spinBox_2);


        gridLayout->addLayout(horizontalLayout_3, 2, 0, 1, 2);

        tabWidget->addTab(tab_background, QString());
        tab_deskplugins = new QWidget();
        tab_deskplugins->setObjectName(QString::fromUtf8("tab_deskplugins"));
        tabWidget->addTab(tab_deskplugins, QString());
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        tabWidget->addTab(tab, QString());

        verticalLayout->addWidget(tabWidget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton_2 = new QPushButton(centralwidget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        horizontalLayout->addWidget(pushButton_2);

        push_save = new QPushButton(centralwidget);
        push_save->setObjectName(QString::fromUtf8("push_save"));

        horizontalLayout->addWidget(push_save);


        verticalLayout->addLayout(horizontalLayout);

        MainUI->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainUI);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 481, 20));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        MainUI->setMenuBar(menubar);
        statusbar = new QStatusBar(MainUI);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainUI->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menuFile->addAction(actionSave_and_Quit);
        menuFile->addAction(actionClose);

        retranslateUi(MainUI);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainUI);
    } // setupUi

    void retranslateUi(QMainWindow *MainUI)
    {
        MainUI->setWindowTitle(QApplication::translate("MainUI", "Desktop Configuration", 0, QApplication::UnicodeUTF8));
        actionSave_and_Quit->setText(QApplication::translate("MainUI", "Save and &Quit", 0, QApplication::UnicodeUTF8));
        actionClose->setText(QApplication::translate("MainUI", "&Close", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainUI", "Screen Number:", 0, QApplication::UnicodeUTF8));
        label_BGImage->setText(QString());
        tool_addbackground->setText(QApplication::translate("MainUI", "Add", 0, QApplication::UnicodeUTF8));
        tool_rmbackground->setText(QApplication::translate("MainUI", "Rem", 0, QApplication::UnicodeUTF8));
        radio_singleBG->setText(QApplication::translate("MainUI", "Single Image", 0, QApplication::UnicodeUTF8));
        radio_rotateBG->setText(QApplication::translate("MainUI", "Rotate Images", 0, QApplication::UnicodeUTF8));
        spinBox_2->setSuffix(QApplication::translate("MainUI", "  Minutes", 0, QApplication::UnicodeUTF8));
        spinBox_2->setPrefix(QApplication::translate("MainUI", "Every  ", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_background), QApplication::translate("MainUI", "Background", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_deskplugins), QApplication::translate("MainUI", "Desktop Plugins", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainUI", "Panels", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("MainUI", "&Cancel", 0, QApplication::UnicodeUTF8));
        push_save->setText(QApplication::translate("MainUI", "&Save Changes", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MainUI", "File", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainUI: public Ui_MainUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINUI_H
