/********************************************************************************
** Form generated from reading UI file 'softconfigwidget.ui'
**
** Created: Tue Apr 2 18:15:11 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SOFTCONFIGWIDGET_H
#define UI_SOFTCONFIGWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_softwareConfigWidget
{
public:
    QGridLayout *gridLayout_2;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QListWidget *listRepos;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer_2;
    QPushButton *pushRepoUp;
    QPushButton *pushRepoDown;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushAddRepo;
    QPushButton *pushDelRepo;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushSave;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushCancel;

    void setupUi(QDialog *softwareConfigWidget)
    {
        if (softwareConfigWidget->objectName().isEmpty())
            softwareConfigWidget->setObjectName(QString::fromUtf8("softwareConfigWidget"));
        softwareConfigWidget->resize(413, 327);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/preferences-system.png"), QSize(), QIcon::Normal, QIcon::Off);
        softwareConfigWidget->setWindowIcon(icon);
        gridLayout_2 = new QGridLayout(softwareConfigWidget);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        groupBox = new QGroupBox(softwareConfigWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        listRepos = new QListWidget(groupBox);
        listRepos->setObjectName(QString::fromUtf8("listRepos"));

        gridLayout->addWidget(listRepos, 0, 0, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        pushRepoUp = new QPushButton(groupBox);
        pushRepoUp->setObjectName(QString::fromUtf8("pushRepoUp"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/arrow-up.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushRepoUp->setIcon(icon1);

        verticalLayout->addWidget(pushRepoUp);

        pushRepoDown = new QPushButton(groupBox);
        pushRepoDown->setObjectName(QString::fromUtf8("pushRepoDown"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/arrow-down.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushRepoDown->setIcon(icon2);

        verticalLayout->addWidget(pushRepoDown);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        gridLayout->addLayout(verticalLayout, 0, 1, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        pushAddRepo = new QPushButton(groupBox);
        pushAddRepo->setObjectName(QString::fromUtf8("pushAddRepo"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/add.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushAddRepo->setIcon(icon3);

        horizontalLayout_2->addWidget(pushAddRepo);

        pushDelRepo = new QPushButton(groupBox);
        pushDelRepo->setObjectName(QString::fromUtf8("pushDelRepo"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/remove.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushDelRepo->setIcon(icon4);

        horizontalLayout_2->addWidget(pushDelRepo);


        gridLayout->addLayout(horizontalLayout_2, 1, 0, 1, 2);


        gridLayout_2->addWidget(groupBox, 0, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pushSave = new QPushButton(softwareConfigWidget);
        pushSave->setObjectName(QString::fromUtf8("pushSave"));
        pushSave->setIcon(icon);

        horizontalLayout->addWidget(pushSave);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushCancel = new QPushButton(softwareConfigWidget);
        pushCancel->setObjectName(QString::fromUtf8("pushCancel"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/dialog-cancel.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushCancel->setIcon(icon5);

        horizontalLayout->addWidget(pushCancel);


        gridLayout_2->addLayout(horizontalLayout, 1, 0, 1, 1);


        retranslateUi(softwareConfigWidget);

        QMetaObject::connectSlotsByName(softwareConfigWidget);
    } // setupUi

    void retranslateUi(QDialog *softwareConfigWidget)
    {
        softwareConfigWidget->setWindowTitle(QApplication::translate("softwareConfigWidget", "Repository Configuration", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("softwareConfigWidget", "Repository Mirrors", 0, QApplication::UnicodeUTF8));
        pushRepoUp->setText(QString());
        pushRepoDown->setText(QString());
        pushAddRepo->setText(QApplication::translate("softwareConfigWidget", "&Add", 0, QApplication::UnicodeUTF8));
        pushDelRepo->setText(QApplication::translate("softwareConfigWidget", "&Remove", 0, QApplication::UnicodeUTF8));
        pushSave->setText(QApplication::translate("softwareConfigWidget", "&Save", 0, QApplication::UnicodeUTF8));
        pushCancel->setText(QApplication::translate("softwareConfigWidget", "&Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class softwareConfigWidget: public Ui_softwareConfigWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SOFTCONFIGWIDGET_H
