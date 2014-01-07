/********************************************************************************
** Form generated from reading UI file 'gitprogress.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GITPROGRESS_H
#define UI_GITPROGRESS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QMessageBox>
#include <sys/stat.h>
#include <sys/types.h>

QT_BEGIN_NAMESPACE

class Ui_gitProgress
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *taskProgressLbl;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *spacer40;
    QPushButton *cancelBut;

    void setupUi(QDialog *gitProgress)
    {
        if (gitProgress->objectName().isEmpty())
            gitProgress->setObjectName(QString::fromUtf8("gitProgress"));
        gitProgress->resize(508, 122);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/system.png"), QSize(), QIcon::Normal, QIcon::Off);
        gitProgress->setWindowIcon(icon);
        gitProgress->setModal(true);
        gridLayout = new QGridLayout(gitProgress);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(gitProgress);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        taskProgressLbl = new QLabel(gitProgress);
        taskProgressLbl->setObjectName(QString::fromUtf8("taskProgressLbl"));
        taskProgressLbl->setWordWrap(false);

        gridLayout->addWidget(taskProgressLbl, 1, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        spacer40 = new QSpacerItem(491, 31, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(spacer40);

        cancelBut = new QPushButton(gitProgress);
        cancelBut->setObjectName(QString::fromUtf8("cancelBut"));

        horizontalLayout->addWidget(cancelBut);


        gridLayout->addLayout(horizontalLayout, 2, 0, 1, 1);


        retranslateUi(gitProgress);

        QMetaObject::connectSlotsByName(gitProgress);
    } // setupUi

    void retranslateUi(QDialog *gitProgress)
    {
        gitProgress->setWindowTitle(QApplication::translate("gitProgress", "Git Progress", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("gitProgress", "Fetching Ports, this may take a while...", 0, QApplication::UnicodeUTF8));
        taskProgressLbl->setText(QApplication::translate("gitProgress", "Task Progress...", 0, QApplication::UnicodeUTF8));
        cancelBut->setText(QApplication::translate("gitProgress", "&Cancel", 0, QApplication::UnicodeUTF8));
        cancelBut->setShortcut(QApplication::translate("gitProgress", "Alt+C", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class gitProgress: public Ui_gitProgress {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GITPROGRESS_H
