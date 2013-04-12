/********************************************************************************
** Form generated from reading UI file 'dialogPreferences.ui'
**
** Created: Tue Apr 2 18:15:11 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGPREFERENCES_H
#define UI_DIALOGPREFERENCES_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_dialogPreferences
{
public:
    QGridLayout *gridLayout_2;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QCheckBox *checkMenuIcon;
    QCheckBox *checkDesktopIcon;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushOK;
    QPushButton *pushCancel;

    void setupUi(QDialog *dialogPreferences)
    {
        if (dialogPreferences->objectName().isEmpty())
            dialogPreferences->setObjectName(QString::fromUtf8("dialogPreferences"));
        dialogPreferences->resize(400, 300);
        gridLayout_2 = new QGridLayout(dialogPreferences);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        groupBox = new QGroupBox(dialogPreferences);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        checkMenuIcon = new QCheckBox(groupBox);
        checkMenuIcon->setObjectName(QString::fromUtf8("checkMenuIcon"));

        gridLayout->addWidget(checkMenuIcon, 0, 0, 1, 1);

        checkDesktopIcon = new QCheckBox(groupBox);
        checkDesktopIcon->setObjectName(QString::fromUtf8("checkDesktopIcon"));

        gridLayout->addWidget(checkDesktopIcon, 1, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 2, 0, 1, 1);


        gridLayout_2->addWidget(groupBox, 0, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushOK = new QPushButton(dialogPreferences);
        pushOK->setObjectName(QString::fromUtf8("pushOK"));

        horizontalLayout->addWidget(pushOK);

        pushCancel = new QPushButton(dialogPreferences);
        pushCancel->setObjectName(QString::fromUtf8("pushCancel"));

        horizontalLayout->addWidget(pushCancel);


        gridLayout_2->addLayout(horizontalLayout, 1, 0, 1, 1);


        retranslateUi(dialogPreferences);

        QMetaObject::connectSlotsByName(dialogPreferences);
    } // setupUi

    void retranslateUi(QDialog *dialogPreferences)
    {
        dialogPreferences->setWindowTitle(QApplication::translate("dialogPreferences", "Preferences", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("dialogPreferences", "AppCafe Preferences", 0, QApplication::UnicodeUTF8));
        checkMenuIcon->setText(QApplication::translate("dialogPreferences", "Create menu icons at install", 0, QApplication::UnicodeUTF8));
        checkDesktopIcon->setText(QApplication::translate("dialogPreferences", "Create desktop icons at install", 0, QApplication::UnicodeUTF8));
        pushOK->setText(QApplication::translate("dialogPreferences", "&Ok", 0, QApplication::UnicodeUTF8));
        pushCancel->setText(QApplication::translate("dialogPreferences", "&Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class dialogPreferences: public Ui_dialogPreferences {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGPREFERENCES_H
