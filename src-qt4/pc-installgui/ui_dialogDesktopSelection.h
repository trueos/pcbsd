/********************************************************************************
** Form generated from reading UI file 'dialogDesktopSelection.ui'
**
** Created: Wed Mar 20 02:45:43 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGDESKTOPSELECTION_H
#define UI_DIALOGDESKTOPSELECTION_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTreeWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_desktopSelection
{
public:
    QGridLayout *gridLayout;
    QGroupBox *groupBox_7;
    QGridLayout *gridLayout_16;
    QTreeWidget *treeWidgetDesktopPackages;
    QLabel *label_25;
    QLabel *labelPkgDesc;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushSave;
    QPushButton *pushClose;

    void setupUi(QWidget *desktopSelection)
    {
        if (desktopSelection->objectName().isEmpty())
            desktopSelection->setObjectName(QString::fromUtf8("desktopSelection"));
        desktopSelection->resize(615, 442);
        desktopSelection->setWindowTitle(QString::fromUtf8("PC-BSD"));
        gridLayout = new QGridLayout(desktopSelection);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        groupBox_7 = new QGroupBox(desktopSelection);
        groupBox_7->setObjectName(QString::fromUtf8("groupBox_7"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox_7->sizePolicy().hasHeightForWidth());
        groupBox_7->setSizePolicy(sizePolicy);
        QFont font;
        font.setBold(false);
        font.setWeight(50);
        groupBox_7->setFont(font);
        gridLayout_16 = new QGridLayout(groupBox_7);
        gridLayout_16->setObjectName(QString::fromUtf8("gridLayout_16"));
        treeWidgetDesktopPackages = new QTreeWidget(groupBox_7);
        treeWidgetDesktopPackages->setObjectName(QString::fromUtf8("treeWidgetDesktopPackages"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(treeWidgetDesktopPackages->sizePolicy().hasHeightForWidth());
        treeWidgetDesktopPackages->setSizePolicy(sizePolicy1);
        treeWidgetDesktopPackages->setIconSize(QSize(42, 42));

        gridLayout_16->addWidget(treeWidgetDesktopPackages, 0, 0, 1, 1);

        label_25 = new QLabel(groupBox_7);
        label_25->setObjectName(QString::fromUtf8("label_25"));
        label_25->setWordWrap(true);

        gridLayout_16->addWidget(label_25, 2, 0, 1, 1);

        labelPkgDesc = new QLabel(groupBox_7);
        labelPkgDesc->setObjectName(QString::fromUtf8("labelPkgDesc"));
        labelPkgDesc->setFrameShape(QFrame::StyledPanel);
        labelPkgDesc->setFrameShadow(QFrame::Plain);

        gridLayout_16->addWidget(labelPkgDesc, 1, 0, 1, 1);


        gridLayout->addWidget(groupBox_7, 0, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushSave = new QPushButton(desktopSelection);
        pushSave->setObjectName(QString::fromUtf8("pushSave"));

        horizontalLayout->addWidget(pushSave);

        pushClose = new QPushButton(desktopSelection);
        pushClose->setObjectName(QString::fromUtf8("pushClose"));

        horizontalLayout->addWidget(pushClose);


        gridLayout->addLayout(horizontalLayout, 1, 0, 1, 1);


        retranslateUi(desktopSelection);

        QMetaObject::connectSlotsByName(desktopSelection);
    } // setupUi

    void retranslateUi(QWidget *desktopSelection)
    {
        groupBox_7->setTitle(QApplication::translate("desktopSelection", "System Package Configuration", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem = treeWidgetDesktopPackages->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("desktopSelection", "Available System Packages", 0, QApplication::UnicodeUTF8));
        label_25->setText(QApplication::translate("desktopSelection", "Please select the software you wish to have installed. At least one desktop must be selected, and changes may be made after installation.", 0, QApplication::UnicodeUTF8));
        labelPkgDesc->setText(QString());
        pushSave->setText(QApplication::translate("desktopSelection", "&Save", 0, QApplication::UnicodeUTF8));
        pushClose->setText(QApplication::translate("desktopSelection", "&Cancel", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(desktopSelection);
    } // retranslateUi

};

namespace Ui {
    class desktopSelection: public Ui_desktopSelection {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGDESKTOPSELECTION_H
