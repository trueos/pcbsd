/********************************************************************************
** Form generated from reading UI file 'nfsmanager.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NFSMANAGER_H
#define UI_NFSMANAGER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFormLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSplitter>
#include <QtGui/QStatusBar>
#include <QtGui/QTextBrowser>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_NFSManager
{
public:
    QWidget *centralWidget;
    QWidget *layoutWidget;
    QFormLayout *formLayout;
    QLabel *label;
    QLineEdit *lineEdit;
    QLabel *label_2;
    QLineEdit *lineEdit_2;
    QLabel *label_3;
    QLineEdit *lineEdit_3;
    QSplitter *splitter;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QWidget *layoutWidget1;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QTextBrowser *textBrowser;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QMenuBar *menuBar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *NFSManager)
    {
        if (NFSManager->objectName().isEmpty())
            NFSManager->setObjectName(QString::fromUtf8("NFSManager"));
        NFSManager->resize(968, 550);
        centralWidget = new QWidget(NFSManager);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(350, 420, 611, 74));
        formLayout = new QFormLayout(layoutWidget);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        lineEdit = new QLineEdit(layoutWidget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        formLayout->setWidget(0, QFormLayout::FieldRole, lineEdit);

        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        lineEdit_2 = new QLineEdit(layoutWidget);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));

        formLayout->setWidget(1, QFormLayout::FieldRole, lineEdit_2);

        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_3);

        lineEdit_3 = new QLineEdit(layoutWidget);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));

        formLayout->setWidget(2, QFormLayout::FieldRole, lineEdit_3);

        splitter = new QSplitter(centralWidget);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setGeometry(QRect(20, 0, 941, 16));
        splitter->setOrientation(Qt::Horizontal);
        label_4 = new QLabel(splitter);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        splitter->addWidget(label_4);
        label_5 = new QLabel(splitter);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        splitter->addWidget(label_5);
        label_6 = new QLabel(splitter);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        splitter->addWidget(label_6);
        layoutWidget1 = new QWidget(centralWidget);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(30, 420, 267, 25));
        horizontalLayout = new QHBoxLayout(layoutWidget1);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        pushButton = new QPushButton(layoutWidget1);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        horizontalLayout->addWidget(pushButton);

        pushButton_2 = new QPushButton(layoutWidget1);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        horizontalLayout->addWidget(pushButton_2);

        pushButton_3 = new QPushButton(layoutWidget1);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));

        horizontalLayout->addWidget(pushButton_3);

        textBrowser = new QTextBrowser(centralWidget);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setGeometry(QRect(20, 20, 941, 381));
        NFSManager->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(NFSManager);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        NFSManager->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(NFSManager);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        NFSManager->setStatusBar(statusBar);
        menuBar = new QMenuBar(NFSManager);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 968, 21));
        NFSManager->setMenuBar(menuBar);
        toolBar = new QToolBar(NFSManager);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        NFSManager->addToolBar(Qt::TopToolBarArea, toolBar);

        retranslateUi(NFSManager);

        QMetaObject::connectSlotsByName(NFSManager);
    } // setupUi

    void retranslateUi(QMainWindow *NFSManager)
    {
        NFSManager->setWindowTitle(QApplication::translate("NFSManager", "NFSManager", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("NFSManager", "Server Hostname", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("NFSManager", "Remote file system", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("NFSManager", "Mount Point", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("NFSManager", "Server Hostname", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("NFSManager", "Remote Filesystem", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("NFSManager", "Moint Point", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("NFSManager", "Add", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("NFSManager", "Remove", 0, QApplication::UnicodeUTF8));
        pushButton_3->setText(QApplication::translate("NFSManager", "Refresh", 0, QApplication::UnicodeUTF8));
        toolBar->setWindowTitle(QApplication::translate("NFSManager", "toolBar", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class NFSManager: public Ui_NFSManager {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NFSMANAGER_H
