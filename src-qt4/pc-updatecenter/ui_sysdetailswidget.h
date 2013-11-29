/********************************************************************************
** Form generated from reading UI file 'sysdetailswidget.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SYSDETAILSWIDGET_H
#define UI_SYSDETAILSWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFormLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QStackedWidget>
#include <QtGui/QTabWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <indicatorwidget.h>

QT_BEGIN_NAMESPACE

class Ui_SysDetailsWidget
{
public:
    QVBoxLayout *verticalLayout;
    IndicatorWidget *sysDetailsIndicator;
    QStackedWidget *sysUpdatesStack;
    QWidget *page;
    QVBoxLayout *verticalLayout_9;
    QTreeWidget *sysUpdatesList;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *sysInstallSelectedBtn;
    QStackedWidget *sysUpdateDetailsStack;
    QWidget *page_9;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_5;
    QVBoxLayout *verticalLayout_11;
    QLabel *label_8;
    QFormLayout *formLayout;
    QLabel *label_12;
    QLabel *sysPadthDescription;
    QLabel *label_10;
    QLabel *sysPatchDate;
    QLabel *label_11;
    QLabel *sysPatchSize;
    QLabel *sysPatchStandalone;
    QLabel *sysPatchRebootRequired;
    QSpacerItem *horizontalSpacer_8;
    QWidget *page_10;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_6;
    QVBoxLayout *verticalLayout_12;
    QLabel *label_9;
    QLabel *sysUpgradeText;
    QLabel *sysUpgradeStandalone;
    QLabel *sysUpgradehRebootRequired;
    QSpacerItem *horizontalSpacer_6;
    QWidget *page_11;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_7;
    QTabWidget *sysUpdateFilesTab;
    QWidget *tab_4;
    QHBoxLayout *horizontalLayout_10;
    QListWidget *sysFilesToUpdateList;
    QWidget *tab_5;
    QHBoxLayout *horizontalLayout_11;
    QListWidget *sysFilesToDeleteList;
    QWidget *tab_6;
    QHBoxLayout *horizontalLayout_12;
    QListWidget *sysFilesModifyedList;
    QWidget *page_2;
    QVBoxLayout *verticalLayout_2;
    QLabel *label;
    QTextEdit *sysUpdateLog;

    void setupUi(QWidget *SysDetailsWidget)
    {
        if (SysDetailsWidget->objectName().isEmpty())
            SysDetailsWidget->setObjectName(QString::fromUtf8("SysDetailsWidget"));
        SysDetailsWidget->resize(610, 311);
        verticalLayout = new QVBoxLayout(SysDetailsWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        sysDetailsIndicator = new IndicatorWidget(SysDetailsWidget);
        sysDetailsIndicator->setObjectName(QString::fromUtf8("sysDetailsIndicator"));

        verticalLayout->addWidget(sysDetailsIndicator);

        sysUpdatesStack = new QStackedWidget(SysDetailsWidget);
        sysUpdatesStack->setObjectName(QString::fromUtf8("sysUpdatesStack"));
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        verticalLayout_9 = new QVBoxLayout(page);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        sysUpdatesList = new QTreeWidget(page);
        sysUpdatesList->setObjectName(QString::fromUtf8("sysUpdatesList"));

        verticalLayout_9->addWidget(sysUpdatesList);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_5);

        sysInstallSelectedBtn = new QPushButton(page);
        sysInstallSelectedBtn->setObjectName(QString::fromUtf8("sysInstallSelectedBtn"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/install.png"), QSize(), QIcon::Normal, QIcon::Off);
        sysInstallSelectedBtn->setIcon(icon);

        horizontalLayout_4->addWidget(sysInstallSelectedBtn);


        verticalLayout_9->addLayout(horizontalLayout_4);

        sysUpdateDetailsStack = new QStackedWidget(page);
        sysUpdateDetailsStack->setObjectName(QString::fromUtf8("sysUpdateDetailsStack"));
        page_9 = new QWidget();
        page_9->setObjectName(QString::fromUtf8("page_9"));
        horizontalLayout_7 = new QHBoxLayout(page_9);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_5 = new QLabel(page_9);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setPixmap(QPixmap(QString::fromUtf8(":/images/sysupdates-patch.png")));

        horizontalLayout_7->addWidget(label_5);

        verticalLayout_11 = new QVBoxLayout();
        verticalLayout_11->setObjectName(QString::fromUtf8("verticalLayout_11"));
        label_8 = new QLabel(page_9);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label_8->setFont(font);

        verticalLayout_11->addWidget(label_8);

        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        label_12 = new QLabel(page_9);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label_12);

        sysPadthDescription = new QLabel(page_9);
        sysPadthDescription->setObjectName(QString::fromUtf8("sysPadthDescription"));

        formLayout->setWidget(0, QFormLayout::FieldRole, sysPadthDescription);

        label_10 = new QLabel(page_9);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_10);

        sysPatchDate = new QLabel(page_9);
        sysPatchDate->setObjectName(QString::fromUtf8("sysPatchDate"));

        formLayout->setWidget(1, QFormLayout::FieldRole, sysPatchDate);

        label_11 = new QLabel(page_9);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_11);

        sysPatchSize = new QLabel(page_9);
        sysPatchSize->setObjectName(QString::fromUtf8("sysPatchSize"));

        formLayout->setWidget(2, QFormLayout::FieldRole, sysPatchSize);


        verticalLayout_11->addLayout(formLayout);

        sysPatchStandalone = new QLabel(page_9);
        sysPatchStandalone->setObjectName(QString::fromUtf8("sysPatchStandalone"));

        verticalLayout_11->addWidget(sysPatchStandalone);

        sysPatchRebootRequired = new QLabel(page_9);
        sysPatchRebootRequired->setObjectName(QString::fromUtf8("sysPatchRebootRequired"));

        verticalLayout_11->addWidget(sysPatchRebootRequired);


        horizontalLayout_7->addLayout(verticalLayout_11);

        horizontalSpacer_8 = new QSpacerItem(314, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_8);

        sysUpdateDetailsStack->addWidget(page_9);
        page_10 = new QWidget();
        page_10->setObjectName(QString::fromUtf8("page_10"));
        horizontalLayout_5 = new QHBoxLayout(page_10);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_6 = new QLabel(page_10);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setPixmap(QPixmap(QString::fromUtf8(":/images/sysupdates-upgrade.png")));

        horizontalLayout_5->addWidget(label_6);

        verticalLayout_12 = new QVBoxLayout();
        verticalLayout_12->setObjectName(QString::fromUtf8("verticalLayout_12"));
        label_9 = new QLabel(page_10);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setFont(font);

        verticalLayout_12->addWidget(label_9);

        sysUpgradeText = new QLabel(page_10);
        sysUpgradeText->setObjectName(QString::fromUtf8("sysUpgradeText"));

        verticalLayout_12->addWidget(sysUpgradeText);

        sysUpgradeStandalone = new QLabel(page_10);
        sysUpgradeStandalone->setObjectName(QString::fromUtf8("sysUpgradeStandalone"));

        verticalLayout_12->addWidget(sysUpgradeStandalone);

        sysUpgradehRebootRequired = new QLabel(page_10);
        sysUpgradehRebootRequired->setObjectName(QString::fromUtf8("sysUpgradehRebootRequired"));

        verticalLayout_12->addWidget(sysUpgradehRebootRequired);


        horizontalLayout_5->addLayout(verticalLayout_12);

        horizontalSpacer_6 = new QSpacerItem(347, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_6);

        sysUpdateDetailsStack->addWidget(page_10);
        page_11 = new QWidget();
        page_11->setObjectName(QString::fromUtf8("page_11"));
        horizontalLayout_9 = new QHBoxLayout(page_11);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        label_7 = new QLabel(page_11);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setPixmap(QPixmap(QString::fromUtf8(":/images/sysupdates-freebsd.png")));

        horizontalLayout_9->addWidget(label_7);

        sysUpdateFilesTab = new QTabWidget(page_11);
        sysUpdateFilesTab->setObjectName(QString::fromUtf8("sysUpdateFilesTab"));
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        horizontalLayout_10 = new QHBoxLayout(tab_4);
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        sysFilesToUpdateList = new QListWidget(tab_4);
        sysFilesToUpdateList->setObjectName(QString::fromUtf8("sysFilesToUpdateList"));

        horizontalLayout_10->addWidget(sysFilesToUpdateList);

        sysUpdateFilesTab->addTab(tab_4, QString());
        tab_5 = new QWidget();
        tab_5->setObjectName(QString::fromUtf8("tab_5"));
        horizontalLayout_11 = new QHBoxLayout(tab_5);
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        sysFilesToDeleteList = new QListWidget(tab_5);
        sysFilesToDeleteList->setObjectName(QString::fromUtf8("sysFilesToDeleteList"));

        horizontalLayout_11->addWidget(sysFilesToDeleteList);

        sysUpdateFilesTab->addTab(tab_5, QString());
        tab_6 = new QWidget();
        tab_6->setObjectName(QString::fromUtf8("tab_6"));
        horizontalLayout_12 = new QHBoxLayout(tab_6);
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        sysFilesModifyedList = new QListWidget(tab_6);
        sysFilesModifyedList->setObjectName(QString::fromUtf8("sysFilesModifyedList"));

        horizontalLayout_12->addWidget(sysFilesModifyedList);

        sysUpdateFilesTab->addTab(tab_6, QString());

        horizontalLayout_9->addWidget(sysUpdateFilesTab);

        sysUpdateDetailsStack->addWidget(page_11);

        verticalLayout_9->addWidget(sysUpdateDetailsStack);

        sysUpdatesStack->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        verticalLayout_2 = new QVBoxLayout(page_2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label = new QLabel(page_2);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout_2->addWidget(label);

        sysUpdateLog = new QTextEdit(page_2);
        sysUpdateLog->setObjectName(QString::fromUtf8("sysUpdateLog"));
        sysUpdateLog->setReadOnly(true);

        verticalLayout_2->addWidget(sysUpdateLog);

        sysUpdatesStack->addWidget(page_2);

        verticalLayout->addWidget(sysUpdatesStack);


        retranslateUi(SysDetailsWidget);

        sysUpdatesStack->setCurrentIndex(0);
        sysUpdateDetailsStack->setCurrentIndex(2);
        sysUpdateFilesTab->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(SysDetailsWidget);
    } // setupUi

    void retranslateUi(QWidget *SysDetailsWidget)
    {
        SysDetailsWidget->setWindowTitle(QApplication::translate("SysDetailsWidget", "Form", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem = sysUpdatesList->headerItem();
        ___qtreewidgetitem->setText(1, QApplication::translate("SysDetailsWidget", "Update type", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(0, QApplication::translate("SysDetailsWidget", "Update", 0, QApplication::UnicodeUTF8));
        sysInstallSelectedBtn->setText(QApplication::translate("SysDetailsWidget", "Install selected only", 0, QApplication::UnicodeUTF8));
        label_5->setText(QString());
        label_8->setText(QApplication::translate("SysDetailsWidget", "PC-BSD update patch", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("SysDetailsWidget", "Description", 0, QApplication::UnicodeUTF8));
        sysPadthDescription->setText(QString());
        label_10->setText(QApplication::translate("SysDetailsWidget", "Date", 0, QApplication::UnicodeUTF8));
        sysPatchDate->setText(QString());
        label_11->setText(QApplication::translate("SysDetailsWidget", "Size", 0, QApplication::UnicodeUTF8));
        sysPatchSize->setText(QString());
        sysPatchStandalone->setText(QApplication::translate("SysDetailsWidget", "Update should be installed standalone. ", 0, QApplication::UnicodeUTF8));
        sysPatchRebootRequired->setText(QApplication::translate("SysDetailsWidget", "Update requires system reboot", 0, QApplication::UnicodeUTF8));
        label_6->setText(QString());
        label_9->setText(QApplication::translate("SysDetailsWidget", "PC-BSD upgrade", 0, QApplication::UnicodeUTF8));
        sysUpgradeText->setText(QString());
        sysUpgradeStandalone->setText(QApplication::translate("SysDetailsWidget", "Update should be installed standalone. ", 0, QApplication::UnicodeUTF8));
        sysUpgradehRebootRequired->setText(QApplication::translate("SysDetailsWidget", "Update requires system reboot", 0, QApplication::UnicodeUTF8));
        label_7->setText(QString());
        sysUpdateFilesTab->setTabText(sysUpdateFilesTab->indexOf(tab_4), QApplication::translate("SysDetailsWidget", "Files to update", 0, QApplication::UnicodeUTF8));
        sysUpdateFilesTab->setTabText(sysUpdateFilesTab->indexOf(tab_5), QApplication::translate("SysDetailsWidget", "Files to delete", 0, QApplication::UnicodeUTF8));
        sysUpdateFilesTab->setTabText(sysUpdateFilesTab->indexOf(tab_6), QApplication::translate("SysDetailsWidget", "Locally modifyed files", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("SysDetailsWidget", "System update log:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SysDetailsWidget: public Ui_SysDetailsWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SYSDETAILSWIDGET_H
