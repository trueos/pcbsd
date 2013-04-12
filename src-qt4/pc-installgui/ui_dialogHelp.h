/********************************************************************************
** Form generated from reading UI file 'dialogHelp.ui'
**
** Created: Wed Mar 20 02:45:43 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGHELP_H
#define UI_DIALOGHELP_H

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

class Ui_dialogHelp
{
public:
    QGridLayout *gridLayout;
    QTextEdit *textHelp;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushClose;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QDialog *dialogHelp)
    {
        if (dialogHelp->objectName().isEmpty())
            dialogHelp->setObjectName(QString::fromUtf8("dialogHelp"));
        dialogHelp->setWindowModality(Qt::ApplicationModal);
        dialogHelp->resize(478, 353);
        dialogHelp->setWindowTitle(QString::fromUtf8("PC-BSD"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/modules/images/systemsettings.png"), QSize(), QIcon::Normal, QIcon::Off);
        dialogHelp->setWindowIcon(icon);
        dialogHelp->setSizeGripEnabled(true);
        dialogHelp->setModal(true);
        gridLayout = new QGridLayout(dialogHelp);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        textHelp = new QTextEdit(dialogHelp);
        textHelp->setObjectName(QString::fromUtf8("textHelp"));
        textHelp->setReadOnly(true);

        gridLayout->addWidget(textHelp, 0, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushClose = new QPushButton(dialogHelp);
        pushClose->setObjectName(QString::fromUtf8("pushClose"));

        horizontalLayout->addWidget(pushClose);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        gridLayout->addLayout(horizontalLayout, 1, 0, 1, 1);


        retranslateUi(dialogHelp);

        QMetaObject::connectSlotsByName(dialogHelp);
    } // setupUi

    void retranslateUi(QDialog *dialogHelp)
    {
        pushClose->setText(QApplication::translate("dialogHelp", "&Close", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(dialogHelp);
    } // retranslateUi

};

namespace Ui {
    class dialogHelp: public Ui_dialogHelp {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGHELP_H
