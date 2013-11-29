/********************************************************************************
** Form generated from reading UI file 'pbidetailswidget.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PBIDETAILSWIDGET_H
#define UI_PBIDETAILSWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QStackedWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <indicatorwidget.h>

QT_BEGIN_NAMESPACE

class Ui_PBIDetailsWidget
{
public:
    QVBoxLayout *verticalLayout;
    IndicatorWidget *pbiDetailsIndicator;
    QStackedWidget *pbiUpdateStack;
    QWidget *page_7;
    QVBoxLayout *verticalLayout_17;
    QTreeWidget *pbiUpdateList;
    QHBoxLayout *horizontalLayout_8;
    QSpacerItem *horizontalSpacer_9;
    QPushButton *updateSelectedPBIBtn;
    QWidget *page_8;
    QVBoxLayout *verticalLayout_19;
    QLabel *label_4;
    QTextEdit *pbiUpdateLog;

    void setupUi(QWidget *PBIDetailsWidget)
    {
        if (PBIDetailsWidget->objectName().isEmpty())
            PBIDetailsWidget->setObjectName(QString::fromUtf8("PBIDetailsWidget"));
        PBIDetailsWidget->resize(452, 308);
        verticalLayout = new QVBoxLayout(PBIDetailsWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        pbiDetailsIndicator = new IndicatorWidget(PBIDetailsWidget);
        pbiDetailsIndicator->setObjectName(QString::fromUtf8("pbiDetailsIndicator"));

        verticalLayout->addWidget(pbiDetailsIndicator);

        pbiUpdateStack = new QStackedWidget(PBIDetailsWidget);
        pbiUpdateStack->setObjectName(QString::fromUtf8("pbiUpdateStack"));
        page_7 = new QWidget();
        page_7->setObjectName(QString::fromUtf8("page_7"));
        verticalLayout_17 = new QVBoxLayout(page_7);
        verticalLayout_17->setObjectName(QString::fromUtf8("verticalLayout_17"));
        pbiUpdateList = new QTreeWidget(page_7);
        pbiUpdateList->setObjectName(QString::fromUtf8("pbiUpdateList"));

        verticalLayout_17->addWidget(pbiUpdateList);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_9);

        updateSelectedPBIBtn = new QPushButton(page_7);
        updateSelectedPBIBtn->setObjectName(QString::fromUtf8("updateSelectedPBIBtn"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/install.png"), QSize(), QIcon::Normal, QIcon::Off);
        updateSelectedPBIBtn->setIcon(icon);

        horizontalLayout_8->addWidget(updateSelectedPBIBtn);


        verticalLayout_17->addLayout(horizontalLayout_8);

        pbiUpdateStack->addWidget(page_7);
        page_8 = new QWidget();
        page_8->setObjectName(QString::fromUtf8("page_8"));
        verticalLayout_19 = new QVBoxLayout(page_8);
        verticalLayout_19->setObjectName(QString::fromUtf8("verticalLayout_19"));
        label_4 = new QLabel(page_8);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        verticalLayout_19->addWidget(label_4);

        pbiUpdateLog = new QTextEdit(page_8);
        pbiUpdateLog->setObjectName(QString::fromUtf8("pbiUpdateLog"));
        pbiUpdateLog->setTextInteractionFlags(Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);

        verticalLayout_19->addWidget(pbiUpdateLog);

        pbiUpdateStack->addWidget(page_8);

        verticalLayout->addWidget(pbiUpdateStack);


        retranslateUi(PBIDetailsWidget);

        pbiUpdateStack->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(PBIDetailsWidget);
    } // setupUi

    void retranslateUi(QWidget *PBIDetailsWidget)
    {
        PBIDetailsWidget->setWindowTitle(QApplication::translate("PBIDetailsWidget", "Form", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem = pbiUpdateList->headerItem();
        ___qtreewidgetitem->setText(2, QApplication::translate("PBIDetailsWidget", "New version", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(1, QApplication::translate("PBIDetailsWidget", "Installed version", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(0, QApplication::translate("PBIDetailsWidget", "Name", 0, QApplication::UnicodeUTF8));
        updateSelectedPBIBtn->setText(QApplication::translate("PBIDetailsWidget", "Update selected only", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("PBIDetailsWidget", "Update log:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class PBIDetailsWidget: public Ui_PBIDetailsWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PBIDETAILSWIDGET_H
