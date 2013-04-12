/********************************************************************************
** Form generated from reading UI file 'dialogFSSize.ui'
**
** Created: Wed Mar 20 02:45:43 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGFSSIZE_H
#define UI_DIALOGFSSIZE_H

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
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_dialogFSSize
{
public:
    QGridLayout *gridLayout;
    QLabel *labelDesc;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_8;
    QSlider *horizontalSizeSlider;
    QSpinBox *spinSize;
    QLabel *label_9;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushSave;
    QPushButton *pushClose;

    void setupUi(QDialog *dialogFSSize)
    {
        if (dialogFSSize->objectName().isEmpty())
            dialogFSSize->setObjectName(QString::fromUtf8("dialogFSSize"));
        dialogFSSize->resize(295, 114);
        dialogFSSize->setWindowTitle(QString::fromUtf8("PC-BSD"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/PCBSD/images/pbidiricon.png"), QSize(), QIcon::Normal, QIcon::Off);
        dialogFSSize->setWindowIcon(icon);
        gridLayout = new QGridLayout(dialogFSSize);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        labelDesc = new QLabel(dialogFSSize);
        labelDesc->setObjectName(QString::fromUtf8("labelDesc"));
        labelDesc->setWordWrap(true);

        gridLayout->addWidget(labelDesc, 0, 0, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_8 = new QLabel(dialogFSSize);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_8->sizePolicy().hasHeightForWidth());
        label_8->setSizePolicy(sizePolicy);
        label_8->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_4->addWidget(label_8);

        horizontalSizeSlider = new QSlider(dialogFSSize);
        horizontalSizeSlider->setObjectName(QString::fromUtf8("horizontalSizeSlider"));
        horizontalSizeSlider->setOrientation(Qt::Horizontal);

        horizontalLayout_4->addWidget(horizontalSizeSlider);

        spinSize = new QSpinBox(dialogFSSize);
        spinSize->setObjectName(QString::fromUtf8("spinSize"));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(spinSize->sizePolicy().hasHeightForWidth());
        spinSize->setSizePolicy(sizePolicy1);

        horizontalLayout_4->addWidget(spinSize);

        label_9 = new QLabel(dialogFSSize);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label_9->sizePolicy().hasHeightForWidth());
        label_9->setSizePolicy(sizePolicy2);

        horizontalLayout_4->addWidget(label_9);


        gridLayout->addLayout(horizontalLayout_4, 1, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(98, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushSave = new QPushButton(dialogFSSize);
        pushSave->setObjectName(QString::fromUtf8("pushSave"));

        horizontalLayout->addWidget(pushSave);

        pushClose = new QPushButton(dialogFSSize);
        pushClose->setObjectName(QString::fromUtf8("pushClose"));

        horizontalLayout->addWidget(pushClose);


        gridLayout->addLayout(horizontalLayout, 2, 0, 1, 1);


        retranslateUi(dialogFSSize);

        QMetaObject::connectSlotsByName(dialogFSSize);
    } // setupUi

    void retranslateUi(QDialog *dialogFSSize)
    {
        labelDesc->setText(QString());
        label_8->setText(QApplication::translate("dialogFSSize", "Size:", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("dialogFSSize", "MB", 0, QApplication::UnicodeUTF8));
        pushSave->setText(QApplication::translate("dialogFSSize", "&Ok", 0, QApplication::UnicodeUTF8));
        pushClose->setText(QApplication::translate("dialogFSSize", "&Cancel", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(dialogFSSize);
    } // retranslateUi

};

namespace Ui {
    class dialogFSSize: public Ui_dialogFSSize {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGFSSIZE_H
