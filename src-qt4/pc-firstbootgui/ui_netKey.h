/********************************************************************************
** Form generated from reading UI file 'netKey.ui'
**
** Created: Fri Apr 5 17:27:08 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NETKEY_H
#define UI_NETKEY_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_netKey
{
public:
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer_2;
    QLabel *labelssid;
    QLabel *label;
    QLineEdit *lineKey2;
    QSpacerItem *verticalSpacer_4;
    QLabel *label_2;
    QLineEdit *lineKey;
    QSpacerItem *verticalSpacer;
    QCheckBox *checkShowKey;
    QSpacerItem *verticalSpacer_3;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushApply;
    QPushButton *pushCancel;

    void setupUi(QDialog *netKey)
    {
        if (netKey->objectName().isEmpty())
            netKey->setObjectName(QString::fromUtf8("netKey"));
        netKey->resize(255, 239);
        netKey->setWindowTitle(QString::fromUtf8("PC-BSD"));
        verticalLayout_2 = new QVBoxLayout(netKey);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_2);

        labelssid = new QLabel(netKey);
        labelssid->setObjectName(QString::fromUtf8("labelssid"));
        labelssid->setLayoutDirection(Qt::LeftToRight);
        labelssid->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(labelssid);

        label = new QLabel(netKey);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout_2->addWidget(label);

        lineKey2 = new QLineEdit(netKey);
        lineKey2->setObjectName(QString::fromUtf8("lineKey2"));

        verticalLayout_2->addWidget(lineKey2);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_4);

        label_2 = new QLabel(netKey);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout_2->addWidget(label_2);

        lineKey = new QLineEdit(netKey);
        lineKey->setObjectName(QString::fromUtf8("lineKey"));

        verticalLayout_2->addWidget(lineKey);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        checkShowKey = new QCheckBox(netKey);
        checkShowKey->setObjectName(QString::fromUtf8("checkShowKey"));

        verticalLayout_2->addWidget(checkShowKey);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushApply = new QPushButton(netKey);
        pushApply->setObjectName(QString::fromUtf8("pushApply"));

        horizontalLayout->addWidget(pushApply);

        pushCancel = new QPushButton(netKey);
        pushCancel->setObjectName(QString::fromUtf8("pushCancel"));

        horizontalLayout->addWidget(pushCancel);


        verticalLayout_2->addLayout(horizontalLayout);


        retranslateUi(netKey);

        QMetaObject::connectSlotsByName(netKey);
    } // setupUi

    void retranslateUi(QDialog *netKey)
    {
        labelssid->setText(QApplication::translate("netKey", "SSID", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("netKey", "Network Key", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("netKey", "Network Key (Repeat)", 0, QApplication::UnicodeUTF8));
        checkShowKey->setText(QApplication::translate("netKey", "Show network key as text", 0, QApplication::UnicodeUTF8));
        pushApply->setText(QApplication::translate("netKey", "Apply", 0, QApplication::UnicodeUTF8));
        pushCancel->setText(QApplication::translate("netKey", "Cancel", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(netKey);
    } // retranslateUi

};

namespace Ui {
    class netKey: public Ui_netKey {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NETKEY_H
