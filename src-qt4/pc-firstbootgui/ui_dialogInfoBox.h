/********************************************************************************
** Form generated from reading UI file 'dialogInfoBox.ui'
**
** Created: Fri Apr 5 17:27:08 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGINFOBOX_H
#define UI_DIALOGINFOBOX_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_dialogInfoBox
{
public:
    QGridLayout *gridLayout;
    QTextEdit *textDisplay;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushClose;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QDialog *dialogInfoBox)
    {
        if (dialogInfoBox->objectName().isEmpty())
            dialogInfoBox->setObjectName(QString::fromUtf8("dialogInfoBox"));
        dialogInfoBox->setWindowModality(Qt::ApplicationModal);
        dialogInfoBox->resize(400, 300);
        dialogInfoBox->setWindowTitle(QString::fromUtf8("PC-BSD"));
        gridLayout = new QGridLayout(dialogInfoBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        textDisplay = new QTextEdit(dialogInfoBox);
        textDisplay->setObjectName(QString::fromUtf8("textDisplay"));
        textDisplay->setUndoRedoEnabled(false);
        textDisplay->setReadOnly(true);

        gridLayout->addWidget(textDisplay, 0, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(98, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushClose = new QPushButton(dialogInfoBox);
        pushClose->setObjectName(QString::fromUtf8("pushClose"));

        horizontalLayout->addWidget(pushClose);

        horizontalSpacer_2 = new QSpacerItem(138, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        gridLayout->addLayout(horizontalLayout, 1, 0, 1, 1);


        retranslateUi(dialogInfoBox);

        QMetaObject::connectSlotsByName(dialogInfoBox);
    } // setupUi

    void retranslateUi(QDialog *dialogInfoBox)
    {
        pushClose->setText(QApplication::translate("dialogInfoBox", "&Close", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(dialogInfoBox);
    } // retranslateUi

};

namespace Ui {
    class dialogInfoBox: public Ui_dialogInfoBox {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGINFOBOX_H
