/********************************************************************************
** Form generated from reading UI file 'dialogCheckHardware.ui'
**
** Created: Wed Mar 20 02:45:43 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGCHECKHARDWARE_H
#define UI_DIALOGCHECKHARDWARE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_dialogCheckHardware
{
public:
    QGridLayout *gridLayout_2;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QLabel *labelVideoDriver;
    QLabel *labelVideoDriverIcon;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    QLabel *labelVideoResolution;
    QLabel *labelVideoResolutionIcon;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_3;
    QLabel *labelEthernet;
    QLabel *labelEthernetIcon;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_4;
    QLabel *labelWifi;
    QLabel *labelWifiIcon;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_5;
    QLabel *labelSound;
    QLabel *labelSoundIcon;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushClose;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QDialog *dialogCheckHardware)
    {
        if (dialogCheckHardware->objectName().isEmpty())
            dialogCheckHardware->setObjectName(QString::fromUtf8("dialogCheckHardware"));
        dialogCheckHardware->resize(409, 317);
        dialogCheckHardware->setWindowTitle(QString::fromUtf8("PC-BSD"));
        gridLayout_2 = new QGridLayout(dialogCheckHardware);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        groupBox = new QGroupBox(dialogCheckHardware);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setMinimumSize(QSize(32, 32));
        label->setMaximumSize(QSize(32, 32));
        label->setPixmap(QPixmap(QString::fromUtf8(":/modules/images/display.png")));
        label->setScaledContents(true);

        horizontalLayout_2->addWidget(label);

        labelVideoDriver = new QLabel(groupBox);
        labelVideoDriver->setObjectName(QString::fromUtf8("labelVideoDriver"));

        horizontalLayout_2->addWidget(labelVideoDriver);

        labelVideoDriverIcon = new QLabel(groupBox);
        labelVideoDriverIcon->setObjectName(QString::fromUtf8("labelVideoDriverIcon"));
        sizePolicy.setHeightForWidth(labelVideoDriverIcon->sizePolicy().hasHeightForWidth());
        labelVideoDriverIcon->setSizePolicy(sizePolicy);
        labelVideoDriverIcon->setMinimumSize(QSize(32, 32));
        labelVideoDriverIcon->setMaximumSize(QSize(32, 32));
        labelVideoDriverIcon->setScaledContents(true);

        horizontalLayout_2->addWidget(labelVideoDriverIcon);


        gridLayout->addLayout(horizontalLayout_2, 0, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);
        label_2->setMinimumSize(QSize(32, 32));
        label_2->setMaximumSize(QSize(32, 32));
        label_2->setPixmap(QPixmap(QString::fromUtf8(":/modules/images/randr.png")));
        label_2->setScaledContents(true);

        horizontalLayout_3->addWidget(label_2);

        labelVideoResolution = new QLabel(groupBox);
        labelVideoResolution->setObjectName(QString::fromUtf8("labelVideoResolution"));

        horizontalLayout_3->addWidget(labelVideoResolution);

        labelVideoResolutionIcon = new QLabel(groupBox);
        labelVideoResolutionIcon->setObjectName(QString::fromUtf8("labelVideoResolutionIcon"));
        sizePolicy.setHeightForWidth(labelVideoResolutionIcon->sizePolicy().hasHeightForWidth());
        labelVideoResolutionIcon->setSizePolicy(sizePolicy);
        labelVideoResolutionIcon->setMinimumSize(QSize(32, 32));
        labelVideoResolutionIcon->setMaximumSize(QSize(32, 32));
        labelVideoResolutionIcon->setScaledContents(true);

        horizontalLayout_3->addWidget(labelVideoResolutionIcon);


        gridLayout->addLayout(horizontalLayout_3, 1, 0, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        sizePolicy.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy);
        label_3->setMinimumSize(QSize(32, 32));
        label_3->setMaximumSize(QSize(32, 32));
        label_3->setPixmap(QPixmap(QString::fromUtf8(":/modules/images/networkwired.png")));
        label_3->setScaledContents(true);

        horizontalLayout_4->addWidget(label_3);

        labelEthernet = new QLabel(groupBox);
        labelEthernet->setObjectName(QString::fromUtf8("labelEthernet"));

        horizontalLayout_4->addWidget(labelEthernet);

        labelEthernetIcon = new QLabel(groupBox);
        labelEthernetIcon->setObjectName(QString::fromUtf8("labelEthernetIcon"));
        sizePolicy.setHeightForWidth(labelEthernetIcon->sizePolicy().hasHeightForWidth());
        labelEthernetIcon->setSizePolicy(sizePolicy);
        labelEthernetIcon->setMinimumSize(QSize(32, 32));
        labelEthernetIcon->setMaximumSize(QSize(32, 32));
        labelEthernetIcon->setScaledContents(true);

        horizontalLayout_4->addWidget(labelEthernetIcon);


        gridLayout->addLayout(horizontalLayout_4, 2, 0, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        sizePolicy.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy);
        label_4->setMinimumSize(QSize(32, 32));
        label_4->setMaximumSize(QSize(32, 32));
        label_4->setPixmap(QPixmap(QString::fromUtf8(":/modules/images/network_wifi.png")));
        label_4->setScaledContents(true);

        horizontalLayout_5->addWidget(label_4);

        labelWifi = new QLabel(groupBox);
        labelWifi->setObjectName(QString::fromUtf8("labelWifi"));

        horizontalLayout_5->addWidget(labelWifi);

        labelWifiIcon = new QLabel(groupBox);
        labelWifiIcon->setObjectName(QString::fromUtf8("labelWifiIcon"));
        sizePolicy.setHeightForWidth(labelWifiIcon->sizePolicy().hasHeightForWidth());
        labelWifiIcon->setSizePolicy(sizePolicy);
        labelWifiIcon->setMinimumSize(QSize(32, 32));
        labelWifiIcon->setMaximumSize(QSize(32, 32));
        labelWifiIcon->setScaledContents(true);

        horizontalLayout_5->addWidget(labelWifiIcon);


        gridLayout->addLayout(horizontalLayout_5, 3, 0, 1, 1);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        sizePolicy.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
        label_5->setSizePolicy(sizePolicy);
        label_5->setMinimumSize(QSize(32, 32));
        label_5->setMaximumSize(QSize(32, 32));
        label_5->setPixmap(QPixmap(QString::fromUtf8(":/modules/images/system-config-sound.png")));
        label_5->setScaledContents(true);

        horizontalLayout_6->addWidget(label_5);

        labelSound = new QLabel(groupBox);
        labelSound->setObjectName(QString::fromUtf8("labelSound"));

        horizontalLayout_6->addWidget(labelSound);

        labelSoundIcon = new QLabel(groupBox);
        labelSoundIcon->setObjectName(QString::fromUtf8("labelSoundIcon"));
        sizePolicy.setHeightForWidth(labelSoundIcon->sizePolicy().hasHeightForWidth());
        labelSoundIcon->setSizePolicy(sizePolicy);
        labelSoundIcon->setMinimumSize(QSize(32, 32));
        labelSoundIcon->setMaximumSize(QSize(32, 32));
        labelSoundIcon->setScaledContents(true);

        horizontalLayout_6->addWidget(labelSoundIcon);


        gridLayout->addLayout(horizontalLayout_6, 4, 0, 1, 1);


        gridLayout_2->addWidget(groupBox, 0, 0, 1, 1);

        verticalSpacer = new QSpacerItem(58, 33, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer, 1, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushClose = new QPushButton(dialogCheckHardware);
        pushClose->setObjectName(QString::fromUtf8("pushClose"));

        horizontalLayout->addWidget(pushClose);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        gridLayout_2->addLayout(horizontalLayout, 2, 0, 1, 1);


        retranslateUi(dialogCheckHardware);

        QMetaObject::connectSlotsByName(dialogCheckHardware);
    } // setupUi

    void retranslateUi(QDialog *dialogCheckHardware)
    {
        groupBox->setTitle(QApplication::translate("dialogCheckHardware", "Detected hardware devices", 0, QApplication::UnicodeUTF8));
        label->setText(QString());
        labelVideoDriver->setText(QString());
        labelVideoDriverIcon->setText(QString());
        label_2->setText(QString());
        labelVideoResolution->setText(QString());
        labelVideoResolutionIcon->setText(QString());
        label_3->setText(QString());
        labelEthernet->setText(QString());
        labelEthernetIcon->setText(QString());
        label_4->setText(QString());
        labelWifi->setText(QString());
        labelWifiIcon->setText(QString());
        label_5->setText(QString());
        labelSound->setText(QString());
        labelSoundIcon->setText(QString());
        pushClose->setText(QApplication::translate("dialogCheckHardware", "&Close", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(dialogCheckHardware);
    } // retranslateUi

};

namespace Ui {
    class dialogCheckHardware: public Ui_dialogCheckHardware {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGCHECKHARDWARE_H
