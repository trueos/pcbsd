/********************************************************************************
** Form generated from reading UI file 'indicatorwidget.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INDICATORWIDGET_H
#define UI_INDICATORWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_IndicatorWidget
{
public:
    QHBoxLayout *horizontalLayout_2;
    QLabel *stateImage;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *msgLabel;
    QPushButton *checkButton;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *installButton;
    QProgressBar *progress;
    QPushButton *cancelButton;

    void setupUi(QWidget *IndicatorWidget)
    {
        if (IndicatorWidget->objectName().isEmpty())
            IndicatorWidget->setObjectName(QString::fromUtf8("IndicatorWidget"));
        IndicatorWidget->resize(496, 76);
        horizontalLayout_2 = new QHBoxLayout(IndicatorWidget);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        stateImage = new QLabel(IndicatorWidget);
        stateImage->setObjectName(QString::fromUtf8("stateImage"));
        stateImage->setPixmap(QPixmap(QString::fromUtf8(":/images/syscheck.png")));

        horizontalLayout_2->addWidget(stateImage);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        msgLabel = new QLabel(IndicatorWidget);
        msgLabel->setObjectName(QString::fromUtf8("msgLabel"));

        horizontalLayout->addWidget(msgLabel);

        checkButton = new QPushButton(IndicatorWidget);
        checkButton->setObjectName(QString::fromUtf8("checkButton"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/view-refresh.png"), QSize(), QIcon::Normal, QIcon::Off);
        checkButton->setIcon(icon);

        horizontalLayout->addWidget(checkButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        installButton = new QPushButton(IndicatorWidget);
        installButton->setObjectName(QString::fromUtf8("installButton"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/install.png"), QSize(), QIcon::Normal, QIcon::Off);
        installButton->setIcon(icon1);

        horizontalLayout_3->addWidget(installButton);

        progress = new QProgressBar(IndicatorWidget);
        progress->setObjectName(QString::fromUtf8("progress"));
        progress->setValue(24);

        horizontalLayout_3->addWidget(progress);

        cancelButton = new QPushButton(IndicatorWidget);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/dialog-cancel.png"), QSize(), QIcon::Normal, QIcon::Off);
        cancelButton->setIcon(icon2);

        horizontalLayout_3->addWidget(cancelButton);


        verticalLayout->addLayout(horizontalLayout_3);


        horizontalLayout_2->addLayout(verticalLayout);


        retranslateUi(IndicatorWidget);

        QMetaObject::connectSlotsByName(IndicatorWidget);
    } // setupUi

    void retranslateUi(QWidget *IndicatorWidget)
    {
        IndicatorWidget->setWindowTitle(QApplication::translate("IndicatorWidget", "Form", 0, QApplication::UnicodeUTF8));
        stateImage->setText(QString());
        msgLabel->setText(QApplication::translate("IndicatorWidget", "Checking for updates...", 0, QApplication::UnicodeUTF8));
        checkButton->setText(QApplication::translate("IndicatorWidget", "Check for updates", 0, QApplication::UnicodeUTF8));
        installButton->setText(QApplication::translate("IndicatorWidget", "Install all updates", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("IndicatorWidget", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class IndicatorWidget: public Ui_IndicatorWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INDICATORWIDGET_H
