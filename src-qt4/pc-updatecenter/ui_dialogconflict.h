/********************************************************************************
** Form generated from reading UI file 'dialogconflict.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGCONFLICT_H
#define UI_DIALOGCONFLICT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DialogConflict
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *errorPict;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QTextEdit *conflictList;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *DialogConflict)
    {
        if (DialogConflict->objectName().isEmpty())
            DialogConflict->setObjectName(QString::fromUtf8("DialogConflict"));
        DialogConflict->resize(328, 251);
        DialogConflict->setModal(true);
        verticalLayout = new QVBoxLayout(DialogConflict);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        errorPict = new QLabel(DialogConflict);
        errorPict->setObjectName(QString::fromUtf8("errorPict"));
        errorPict->setPixmap(QPixmap(QString::fromUtf8(":/images/pkgerror.png")));

        horizontalLayout->addWidget(errorPict);

        label = new QLabel(DialogConflict);
        label->setObjectName(QString::fromUtf8("label"));
        label->setWordWrap(true);

        horizontalLayout->addWidget(label);


        verticalLayout->addLayout(horizontalLayout);

        label_2 = new QLabel(DialogConflict);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setWordWrap(true);

        verticalLayout->addWidget(label_2);

        label_3 = new QLabel(DialogConflict);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setWordWrap(true);

        verticalLayout->addWidget(label_3);

        label_4 = new QLabel(DialogConflict);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        verticalLayout->addWidget(label_4);

        conflictList = new QTextEdit(DialogConflict);
        conflictList->setObjectName(QString::fromUtf8("conflictList"));
        conflictList->setReadOnly(true);

        verticalLayout->addWidget(conflictList);

        buttonBox = new QDialogButtonBox(DialogConflict);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(DialogConflict);
        QObject::connect(buttonBox, SIGNAL(accepted()), DialogConflict, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), DialogConflict, SLOT(reject()));

        QMetaObject::connectSlotsByName(DialogConflict);
    } // setupUi

    void retranslateUi(QDialog *DialogConflict)
    {
        DialogConflict->setWindowTitle(QApplication::translate("DialogConflict", "Package conflict", 0, QApplication::UnicodeUTF8));
        errorPict->setText(QString());
        label->setText(QApplication::translate("DialogConflict", "Package update was paused due to packages conflict.", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("DialogConflict", "Press 'Ok' to allow automatic conflict resolving. But correct result of resolving is not is not guaranteed", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("DialogConflict", "Or press 'cancel' to stop update process. You will need to resolve conflict manually.", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("DialogConflict", "Conflicted packages:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DialogConflict: public Ui_DialogConflict {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGCONFLICT_H
