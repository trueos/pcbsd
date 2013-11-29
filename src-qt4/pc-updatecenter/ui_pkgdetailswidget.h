/********************************************************************************
** Form generated from reading UI file 'pkgdetailswidget.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PKGDETAILSWIDGET_H
#define UI_PKGDETAILSWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QStackedWidget>
#include <QtGui/QTabWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <indicatorwidget.h>

QT_BEGIN_NAMESPACE

class Ui_PkgDetailsWidget
{
public:
    QVBoxLayout *verticalLayout;
    IndicatorWidget *pkgDetailsIndicator;
    QStackedWidget *pkgDetailsStack;
    QWidget *page_3;
    QVBoxLayout *verticalLayout_16;
    QHBoxLayout *horizontalLayout_6;
    QLabel *pkgDlSizeLabel_2;
    QLabel *pkgSpaceReqLabel_2;
    QSpacerItem *horizontalSpacer_7;
    QTabWidget *pkgListsTab;
    QWidget *pkgUpgradeTab;
    QVBoxLayout *verticalLayout_10;
    QTreeWidget *pkgUpgradeList;
    QWidget *tab_9;
    QVBoxLayout *verticalLayout_13;
    QTreeWidget *pkgInstallList;
    QWidget *tab_10;
    QVBoxLayout *verticalLayout_14;
    QTreeWidget *pkgReinstallList;
    QWidget *page_4;
    QVBoxLayout *verticalLayout_15;
    QLabel *label_3;
    QTextEdit *pkgUpdateLog;

    void setupUi(QWidget *PkgDetailsWidget)
    {
        if (PkgDetailsWidget->objectName().isEmpty())
            PkgDetailsWidget->setObjectName(QString::fromUtf8("PkgDetailsWidget"));
        PkgDetailsWidget->resize(444, 295);
        verticalLayout = new QVBoxLayout(PkgDetailsWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        pkgDetailsIndicator = new IndicatorWidget(PkgDetailsWidget);
        pkgDetailsIndicator->setObjectName(QString::fromUtf8("pkgDetailsIndicator"));

        verticalLayout->addWidget(pkgDetailsIndicator);

        pkgDetailsStack = new QStackedWidget(PkgDetailsWidget);
        pkgDetailsStack->setObjectName(QString::fromUtf8("pkgDetailsStack"));
        page_3 = new QWidget();
        page_3->setObjectName(QString::fromUtf8("page_3"));
        verticalLayout_16 = new QVBoxLayout(page_3);
        verticalLayout_16->setObjectName(QString::fromUtf8("verticalLayout_16"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        pkgDlSizeLabel_2 = new QLabel(page_3);
        pkgDlSizeLabel_2->setObjectName(QString::fromUtf8("pkgDlSizeLabel_2"));

        horizontalLayout_6->addWidget(pkgDlSizeLabel_2);

        pkgSpaceReqLabel_2 = new QLabel(page_3);
        pkgSpaceReqLabel_2->setObjectName(QString::fromUtf8("pkgSpaceReqLabel_2"));

        horizontalLayout_6->addWidget(pkgSpaceReqLabel_2);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_7);


        verticalLayout_16->addLayout(horizontalLayout_6);

        pkgListsTab = new QTabWidget(page_3);
        pkgListsTab->setObjectName(QString::fromUtf8("pkgListsTab"));
        pkgUpgradeTab = new QWidget();
        pkgUpgradeTab->setObjectName(QString::fromUtf8("pkgUpgradeTab"));
        verticalLayout_10 = new QVBoxLayout(pkgUpgradeTab);
        verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
        pkgUpgradeList = new QTreeWidget(pkgUpgradeTab);
        pkgUpgradeList->setObjectName(QString::fromUtf8("pkgUpgradeList"));

        verticalLayout_10->addWidget(pkgUpgradeList);

        pkgListsTab->addTab(pkgUpgradeTab, QString());
        tab_9 = new QWidget();
        tab_9->setObjectName(QString::fromUtf8("tab_9"));
        verticalLayout_13 = new QVBoxLayout(tab_9);
        verticalLayout_13->setObjectName(QString::fromUtf8("verticalLayout_13"));
        pkgInstallList = new QTreeWidget(tab_9);
        pkgInstallList->setObjectName(QString::fromUtf8("pkgInstallList"));

        verticalLayout_13->addWidget(pkgInstallList);

        pkgListsTab->addTab(tab_9, QString());
        tab_10 = new QWidget();
        tab_10->setObjectName(QString::fromUtf8("tab_10"));
        verticalLayout_14 = new QVBoxLayout(tab_10);
        verticalLayout_14->setObjectName(QString::fromUtf8("verticalLayout_14"));
        pkgReinstallList = new QTreeWidget(tab_10);
        pkgReinstallList->setObjectName(QString::fromUtf8("pkgReinstallList"));

        verticalLayout_14->addWidget(pkgReinstallList);

        pkgListsTab->addTab(tab_10, QString());

        verticalLayout_16->addWidget(pkgListsTab);

        pkgDetailsStack->addWidget(page_3);
        page_4 = new QWidget();
        page_4->setObjectName(QString::fromUtf8("page_4"));
        verticalLayout_15 = new QVBoxLayout(page_4);
        verticalLayout_15->setObjectName(QString::fromUtf8("verticalLayout_15"));
        label_3 = new QLabel(page_4);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        verticalLayout_15->addWidget(label_3);

        pkgUpdateLog = new QTextEdit(page_4);
        pkgUpdateLog->setObjectName(QString::fromUtf8("pkgUpdateLog"));
        pkgUpdateLog->setReadOnly(true);

        verticalLayout_15->addWidget(pkgUpdateLog);

        pkgDetailsStack->addWidget(page_4);

        verticalLayout->addWidget(pkgDetailsStack);


        retranslateUi(PkgDetailsWidget);

        pkgDetailsStack->setCurrentIndex(1);
        pkgListsTab->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(PkgDetailsWidget);
    } // setupUi

    void retranslateUi(QWidget *PkgDetailsWidget)
    {
        PkgDetailsWidget->setWindowTitle(QApplication::translate("PkgDetailsWidget", "Form", 0, QApplication::UnicodeUTF8));
        pkgDlSizeLabel_2->setText(QApplication::translate("PkgDetailsWidget", "TextLabel", 0, QApplication::UnicodeUTF8));
        pkgSpaceReqLabel_2->setText(QString());
        QTreeWidgetItem *___qtreewidgetitem = pkgUpgradeList->headerItem();
        ___qtreewidgetitem->setText(2, QApplication::translate("PkgDetailsWidget", "New version", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(1, QApplication::translate("PkgDetailsWidget", "Installed version", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(0, QApplication::translate("PkgDetailsWidget", "Package name", 0, QApplication::UnicodeUTF8));
        pkgListsTab->setTabText(pkgListsTab->indexOf(pkgUpgradeTab), QApplication::translate("PkgDetailsWidget", "Packages to upgrade", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem1 = pkgInstallList->headerItem();
        ___qtreewidgetitem1->setText(1, QApplication::translate("PkgDetailsWidget", "Version", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem1->setText(0, QApplication::translate("PkgDetailsWidget", "Package name", 0, QApplication::UnicodeUTF8));
        pkgListsTab->setTabText(pkgListsTab->indexOf(tab_9), QApplication::translate("PkgDetailsWidget", "Packages to install", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem2 = pkgReinstallList->headerItem();
        ___qtreewidgetitem2->setText(2, QApplication::translate("PkgDetailsWidget", "Reason", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem2->setText(1, QApplication::translate("PkgDetailsWidget", "Version", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem2->setText(0, QApplication::translate("PkgDetailsWidget", "Package name", 0, QApplication::UnicodeUTF8));
        pkgListsTab->setTabText(pkgListsTab->indexOf(tab_10), QApplication::translate("PkgDetailsWidget", "Packages to reinstall", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("PkgDetailsWidget", "Update log:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class PkgDetailsWidget: public Ui_PkgDetailsWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PKGDETAILSWIDGET_H
