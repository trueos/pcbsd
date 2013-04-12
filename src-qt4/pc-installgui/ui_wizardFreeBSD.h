/********************************************************************************
** Form generated from reading UI file 'wizardFreeBSD.ui'
**
** Created: Wed Mar 20 02:45:43 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIZARDFREEBSD_H
#define UI_WIZARDFREEBSD_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWizard>
#include <QtGui/QWizardPage>

QT_BEGIN_NAMESPACE

class Ui_wizardFreeBSD
{
public:
    QWizardPage *Page_Intro;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_2;
    QSpacerItem *verticalSpacer_10;
    QSpacerItem *verticalSpacer;
    QLabel *label;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer_2;
    QWizardPage *Page_Root;
    QGridLayout *gridLayout_3;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_13;
    QSpacerItem *verticalSpacer_9;
    QSpacerItem *verticalSpacer_5;
    QSpacerItem *horizontalSpacer_6;
    QLabel *label_3;
    QSpacerItem *verticalSpacer_3;
    QGridLayout *gridLayout_2;
    QLineEdit *lineRootPW;
    QLabel *label_4;
    QSpacerItem *horizontalSpacer_3;
    QLineEdit *lineRootPW2;
    QLabel *label_5;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *verticalSpacer_4;
    QWizardPage *Page_User;
    QGridLayout *gridLayout_5;
    QVBoxLayout *verticalLayout;
    QLabel *label_12;
    QSpacerItem *verticalSpacer_7;
    QSpacerItem *verticalSpacer_6;
    QLabel *label_6;
    QGridLayout *gridLayout_4;
    QLineEdit *lineName;
    QLabel *label_7;
    QLineEdit *lineUsername;
    QLabel *label_8;
    QLineEdit *linePW;
    QLabel *label_9;
    QLineEdit *linePW2;
    QLabel *label_10;
    QComboBox *comboShell;
    QLabel *label_11;
    QSpacerItem *horizontalSpacer_7;
    QSpacerItem *verticalSpacer_8;
    QSpacerItem *verticalSpacer_11;
    QWizardPage *Page_Host;
    QGridLayout *gridLayout_6;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_14;
    QSpacerItem *verticalSpacer_12;
    QSpacerItem *verticalSpacer_13;
    QLabel *label_15;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer_14;
    QHBoxLayout *horizontalLayout;
    QLineEdit *lineHostname;
    QLabel *label_16;
    QSpacerItem *horizontalSpacer_9;
    QSpacerItem *verticalSpacer_15;
    QSpacerItem *verticalSpacer_16;
    QCheckBox *checkSSH;
    QWizardPage *wizardPage;
    QGridLayout *gridLayout_8;
    QVBoxLayout *verticalLayout_6;
    QLabel *label_19;
    QSpacerItem *verticalSpacer_21;
    QSpacerItem *verticalSpacer_22;
    QLabel *label_20;
    QSpacerItem *verticalSpacer_23;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_8;
    QVBoxLayout *verticalLayout_13;
    QLabel *label_21;
    QLabel *textIP;
    QLabel *textNetmask;
    QLabel *textDNS;
    QVBoxLayout *verticalLayout_11;
    QLabel *textDefaultRouter;
    QLabel *textIPv6;
    QLabel *textIPv6DefaultRouter;
    QLabel *textIPv6DNS;
    QVBoxLayout *verticalLayout_9;
    QVBoxLayout *verticalLayout_12;
    QComboBox *comboSelectNic;
    QLineEdit *lineIP;
    QLineEdit *lineNetmask;
    QLineEdit *lineDNS;
    QVBoxLayout *verticalLayout_7;
    QLineEdit *lineDefaultRouter;
    QLineEdit *lineIPv6;
    QLineEdit *lineIPv6DefaultRouter;
    QLineEdit *lineIPv6DNS;
    QSpacerItem *horizontalSpacer_8;
    QSpacerItem *verticalSpacer_25;
    QWizardPage *Page_Optional;
    QGridLayout *gridLayout_7;
    QVBoxLayout *verticalLayout_5;
    QLabel *label_17;
    QSpacerItem *verticalSpacer_17;
    QSpacerItem *verticalSpacer_18;
    QLabel *label_18;
    QSpacerItem *horizontalSpacer_5;
    QSpacerItem *verticalSpacer_19;
    QCheckBox *checkSrc;
    QCheckBox *checkPorts;
    QSpacerItem *verticalSpacer_20;

    void setupUi(QWizard *wizardFreeBSD)
    {
        if (wizardFreeBSD->objectName().isEmpty())
            wizardFreeBSD->setObjectName(QString::fromUtf8("wizardFreeBSD"));
        wizardFreeBSD->resize(527, 363);
        wizardFreeBSD->setWindowTitle(QString::fromUtf8("PC-BSD"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/modules/images/freebsd.png"), QSize(), QIcon::Normal, QIcon::Off);
        wizardFreeBSD->setWindowIcon(icon);
        wizardFreeBSD->setWizardStyle(QWizard::ClassicStyle);
        Page_Intro = new QWizardPage();
        Page_Intro->setObjectName(QString::fromUtf8("Page_Intro"));
        gridLayout = new QGridLayout(Page_Intro);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        label_2 = new QLabel(Page_Intro);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);
        label_2->setMaximumSize(QSize(55, 55));
        label_2->setPixmap(QPixmap(QString::fromUtf8(":/modules/images/pcbsd-server.png")));
        label_2->setScaledContents(true);
        label_2->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(label_2);

        verticalSpacer_10 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_10);


        gridLayout->addLayout(verticalLayout_3, 0, 0, 3, 1);

        verticalSpacer = new QSpacerItem(258, 18, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout->addItem(verticalSpacer, 0, 1, 1, 1);

        label = new QLabel(Page_Intro);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);
        label->setWordWrap(true);

        gridLayout->addWidget(label, 1, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(18, 178, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 1, 2, 2, 1);

        verticalSpacer_2 = new QSpacerItem(228, 163, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);

        gridLayout->addItem(verticalSpacer_2, 2, 1, 1, 1);

        wizardFreeBSD->addPage(Page_Intro);
        Page_Root = new QWizardPage();
        Page_Root->setObjectName(QString::fromUtf8("Page_Root"));
        gridLayout_3 = new QGridLayout(Page_Root);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label_13 = new QLabel(Page_Root);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        sizePolicy.setHeightForWidth(label_13->sizePolicy().hasHeightForWidth());
        label_13->setSizePolicy(sizePolicy);
        label_13->setMaximumSize(QSize(55, 55));
        label_13->setPixmap(QPixmap(QString::fromUtf8(":/modules/images/encrypted.png")));
        label_13->setScaledContents(true);

        verticalLayout_2->addWidget(label_13);

        verticalSpacer_9 = new QSpacerItem(20, 118, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_9);


        gridLayout_3->addLayout(verticalLayout_2, 0, 0, 5, 1);

        verticalSpacer_5 = new QSpacerItem(268, 18, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_3->addItem(verticalSpacer_5, 0, 1, 1, 1);

        horizontalSpacer_6 = new QSpacerItem(18, 198, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_6, 0, 2, 5, 1);

        label_3 = new QLabel(Page_Root);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_3->addWidget(label_3, 1, 1, 1, 1);

        verticalSpacer_3 = new QSpacerItem(288, 17, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_3->addItem(verticalSpacer_3, 2, 1, 1, 1);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        lineRootPW = new QLineEdit(Page_Root);
        lineRootPW->setObjectName(QString::fromUtf8("lineRootPW"));
        lineRootPW->setEchoMode(QLineEdit::Password);

        gridLayout_2->addWidget(lineRootPW, 0, 0, 1, 1);

        label_4 = new QLabel(Page_Root);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_2->addWidget(label_4, 0, 1, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_3, 0, 2, 1, 1);

        lineRootPW2 = new QLineEdit(Page_Root);
        lineRootPW2->setObjectName(QString::fromUtf8("lineRootPW2"));
        lineRootPW2->setEchoMode(QLineEdit::Password);

        gridLayout_2->addWidget(lineRootPW2, 1, 0, 1, 1);

        label_5 = new QLabel(Page_Root);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_2->addWidget(label_5, 1, 1, 1, 2);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_4, 1, 3, 1, 1);


        gridLayout_3->addLayout(gridLayout_2, 3, 1, 1, 1);

        verticalSpacer_4 = new QSpacerItem(218, 97, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer_4, 4, 1, 1, 1);

        wizardFreeBSD->addPage(Page_Root);
        Page_User = new QWizardPage();
        Page_User->setObjectName(QString::fromUtf8("Page_User"));
        gridLayout_5 = new QGridLayout(Page_User);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label_12 = new QLabel(Page_User);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        sizePolicy.setHeightForWidth(label_12->sizePolicy().hasHeightForWidth());
        label_12->setSizePolicy(sizePolicy);
        label_12->setMaximumSize(QSize(55, 55));
        label_12->setPixmap(QPixmap(QString::fromUtf8(":/modules/images/add_user.png")));
        label_12->setScaledContents(true);

        verticalLayout->addWidget(label_12);

        verticalSpacer_7 = new QSpacerItem(20, 78, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_7);


        gridLayout_5->addLayout(verticalLayout, 0, 0, 5, 1);

        verticalSpacer_6 = new QSpacerItem(326, 18, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_5->addItem(verticalSpacer_6, 0, 1, 1, 2);

        label_6 = new QLabel(Page_User);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_5->addWidget(label_6, 1, 1, 1, 2);

        gridLayout_4 = new QGridLayout();
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        lineName = new QLineEdit(Page_User);
        lineName->setObjectName(QString::fromUtf8("lineName"));

        gridLayout_4->addWidget(lineName, 0, 0, 1, 1);

        label_7 = new QLabel(Page_User);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_4->addWidget(label_7, 0, 1, 1, 1);

        lineUsername = new QLineEdit(Page_User);
        lineUsername->setObjectName(QString::fromUtf8("lineUsername"));

        gridLayout_4->addWidget(lineUsername, 1, 0, 1, 1);

        label_8 = new QLabel(Page_User);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout_4->addWidget(label_8, 1, 1, 1, 1);

        linePW = new QLineEdit(Page_User);
        linePW->setObjectName(QString::fromUtf8("linePW"));
        linePW->setEchoMode(QLineEdit::Password);

        gridLayout_4->addWidget(linePW, 2, 0, 1, 1);

        label_9 = new QLabel(Page_User);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout_4->addWidget(label_9, 2, 1, 1, 1);

        linePW2 = new QLineEdit(Page_User);
        linePW2->setObjectName(QString::fromUtf8("linePW2"));
        linePW2->setEchoMode(QLineEdit::Password);

        gridLayout_4->addWidget(linePW2, 3, 0, 1, 1);

        label_10 = new QLabel(Page_User);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout_4->addWidget(label_10, 3, 1, 1, 1);

        comboShell = new QComboBox(Page_User);
        comboShell->insertItems(0, QStringList()
         << QString::fromUtf8("/bin/csh")
         << QString::fromUtf8("/bin/tcsh")
         << QString::fromUtf8("/bin/sh")
        );
        comboShell->setObjectName(QString::fromUtf8("comboShell"));

        gridLayout_4->addWidget(comboShell, 4, 0, 1, 1);

        label_11 = new QLabel(Page_User);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout_4->addWidget(label_11, 4, 1, 1, 1);


        gridLayout_5->addLayout(gridLayout_4, 3, 1, 1, 1);

        horizontalSpacer_7 = new QSpacerItem(18, 128, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout_5->addItem(horizontalSpacer_7, 3, 2, 1, 1);

        verticalSpacer_8 = new QSpacerItem(299, 17, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_5->addItem(verticalSpacer_8, 4, 1, 1, 1);

        verticalSpacer_11 = new QSpacerItem(20, 17, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_5->addItem(verticalSpacer_11, 2, 1, 1, 1);

        wizardFreeBSD->addPage(Page_User);
        Page_Host = new QWizardPage();
        Page_Host->setObjectName(QString::fromUtf8("Page_Host"));
        gridLayout_6 = new QGridLayout(Page_Host);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        label_14 = new QLabel(Page_Host);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        sizePolicy.setHeightForWidth(label_14->sizePolicy().hasHeightForWidth());
        label_14->setSizePolicy(sizePolicy);
        label_14->setMaximumSize(QSize(55, 55));
        label_14->setPixmap(QPixmap(QString::fromUtf8(":/modules/images/systemsettings.png")));
        label_14->setScaledContents(true);

        verticalLayout_4->addWidget(label_14);

        verticalSpacer_12 = new QSpacerItem(20, 78, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_12);


        gridLayout_6->addLayout(verticalLayout_4, 0, 0, 8, 1);

        verticalSpacer_13 = new QSpacerItem(299, 18, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_6->addItem(verticalSpacer_13, 0, 1, 1, 1);

        label_15 = new QLabel(Page_Host);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setWordWrap(true);

        gridLayout_6->addWidget(label_15, 1, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(18, 197, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout_6->addItem(horizontalSpacer, 1, 2, 7, 1);

        verticalSpacer_14 = new QSpacerItem(299, 13, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_6->addItem(verticalSpacer_14, 2, 1, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        lineHostname = new QLineEdit(Page_Host);
        lineHostname->setObjectName(QString::fromUtf8("lineHostname"));

        horizontalLayout->addWidget(lineHostname);

        label_16 = new QLabel(Page_Host);
        label_16->setObjectName(QString::fromUtf8("label_16"));

        horizontalLayout->addWidget(label_16);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_9);


        gridLayout_6->addLayout(horizontalLayout, 3, 1, 1, 1);

        verticalSpacer_15 = new QSpacerItem(299, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_6->addItem(verticalSpacer_15, 7, 1, 1, 1);

        verticalSpacer_16 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_6->addItem(verticalSpacer_16, 5, 1, 1, 1);

        checkSSH = new QCheckBox(Page_Host);
        checkSSH->setObjectName(QString::fromUtf8("checkSSH"));

        gridLayout_6->addWidget(checkSSH, 4, 1, 1, 1);

        wizardFreeBSD->addPage(Page_Host);
        wizardPage = new QWizardPage();
        wizardPage->setObjectName(QString::fromUtf8("wizardPage"));
        gridLayout_8 = new QGridLayout(wizardPage);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        label_19 = new QLabel(wizardPage);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        sizePolicy.setHeightForWidth(label_19->sizePolicy().hasHeightForWidth());
        label_19->setSizePolicy(sizePolicy);
        label_19->setMaximumSize(QSize(55, 55));
        label_19->setPixmap(QPixmap(QString::fromUtf8(":/modules/images/network.png")));
        label_19->setScaledContents(true);

        verticalLayout_6->addWidget(label_19);

        verticalSpacer_21 = new QSpacerItem(20, 78, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_6->addItem(verticalSpacer_21);


        gridLayout_8->addLayout(verticalLayout_6, 0, 0, 5, 1);

        verticalSpacer_22 = new QSpacerItem(437, 18, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_8->addItem(verticalSpacer_22, 0, 1, 1, 2);

        label_20 = new QLabel(wizardPage);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        label_20->setWordWrap(true);

        gridLayout_8->addWidget(label_20, 1, 1, 1, 2);

        verticalSpacer_23 = new QSpacerItem(437, 13, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_8->addItem(verticalSpacer_23, 2, 1, 1, 2);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        verticalLayout_13 = new QVBoxLayout();
        verticalLayout_13->setObjectName(QString::fromUtf8("verticalLayout_13"));
        label_21 = new QLabel(wizardPage);
        label_21->setObjectName(QString::fromUtf8("label_21"));
        label_21->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        verticalLayout_13->addWidget(label_21);

        textIP = new QLabel(wizardPage);
        textIP->setObjectName(QString::fromUtf8("textIP"));
        textIP->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        verticalLayout_13->addWidget(textIP);

        textNetmask = new QLabel(wizardPage);
        textNetmask->setObjectName(QString::fromUtf8("textNetmask"));
        textNetmask->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        verticalLayout_13->addWidget(textNetmask);

        textDNS = new QLabel(wizardPage);
        textDNS->setObjectName(QString::fromUtf8("textDNS"));
        textDNS->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        verticalLayout_13->addWidget(textDNS);


        verticalLayout_8->addLayout(verticalLayout_13);

        verticalLayout_11 = new QVBoxLayout();
        verticalLayout_11->setObjectName(QString::fromUtf8("verticalLayout_11"));
        textDefaultRouter = new QLabel(wizardPage);
        textDefaultRouter->setObjectName(QString::fromUtf8("textDefaultRouter"));
        textDefaultRouter->setEnabled(true);
        textDefaultRouter->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        verticalLayout_11->addWidget(textDefaultRouter);

        textIPv6 = new QLabel(wizardPage);
        textIPv6->setObjectName(QString::fromUtf8("textIPv6"));
        textIPv6->setEnabled(true);
        textIPv6->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        verticalLayout_11->addWidget(textIPv6);

        textIPv6DefaultRouter = new QLabel(wizardPage);
        textIPv6DefaultRouter->setObjectName(QString::fromUtf8("textIPv6DefaultRouter"));
        textIPv6DefaultRouter->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        verticalLayout_11->addWidget(textIPv6DefaultRouter);

        textIPv6DNS = new QLabel(wizardPage);
        textIPv6DNS->setObjectName(QString::fromUtf8("textIPv6DNS"));
        textIPv6DNS->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        verticalLayout_11->addWidget(textIPv6DNS);


        verticalLayout_8->addLayout(verticalLayout_11);


        horizontalLayout_2->addLayout(verticalLayout_8);

        verticalLayout_9 = new QVBoxLayout();
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        verticalLayout_12 = new QVBoxLayout();
        verticalLayout_12->setObjectName(QString::fromUtf8("verticalLayout_12"));
        comboSelectNic = new QComboBox(wizardPage);
        comboSelectNic->setObjectName(QString::fromUtf8("comboSelectNic"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(comboSelectNic->sizePolicy().hasHeightForWidth());
        comboSelectNic->setSizePolicy(sizePolicy2);

        verticalLayout_12->addWidget(comboSelectNic);

        lineIP = new QLineEdit(wizardPage);
        lineIP->setObjectName(QString::fromUtf8("lineIP"));
        sizePolicy2.setHeightForWidth(lineIP->sizePolicy().hasHeightForWidth());
        lineIP->setSizePolicy(sizePolicy2);
        lineIP->setAlignment(Qt::AlignCenter);

        verticalLayout_12->addWidget(lineIP);

        lineNetmask = new QLineEdit(wizardPage);
        lineNetmask->setObjectName(QString::fromUtf8("lineNetmask"));
        sizePolicy2.setHeightForWidth(lineNetmask->sizePolicy().hasHeightForWidth());
        lineNetmask->setSizePolicy(sizePolicy2);
        lineNetmask->setAlignment(Qt::AlignCenter);

        verticalLayout_12->addWidget(lineNetmask);

        lineDNS = new QLineEdit(wizardPage);
        lineDNS->setObjectName(QString::fromUtf8("lineDNS"));
        sizePolicy2.setHeightForWidth(lineDNS->sizePolicy().hasHeightForWidth());
        lineDNS->setSizePolicy(sizePolicy2);
        lineDNS->setAlignment(Qt::AlignCenter);

        verticalLayout_12->addWidget(lineDNS);


        verticalLayout_9->addLayout(verticalLayout_12);

        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        lineDefaultRouter = new QLineEdit(wizardPage);
        lineDefaultRouter->setObjectName(QString::fromUtf8("lineDefaultRouter"));
        sizePolicy2.setHeightForWidth(lineDefaultRouter->sizePolicy().hasHeightForWidth());
        lineDefaultRouter->setSizePolicy(sizePolicy2);
        lineDefaultRouter->setAlignment(Qt::AlignCenter);

        verticalLayout_7->addWidget(lineDefaultRouter);

        lineIPv6 = new QLineEdit(wizardPage);
        lineIPv6->setObjectName(QString::fromUtf8("lineIPv6"));
        sizePolicy2.setHeightForWidth(lineIPv6->sizePolicy().hasHeightForWidth());
        lineIPv6->setSizePolicy(sizePolicy2);
        lineIPv6->setMaxLength(64);

        verticalLayout_7->addWidget(lineIPv6);

        lineIPv6DefaultRouter = new QLineEdit(wizardPage);
        lineIPv6DefaultRouter->setObjectName(QString::fromUtf8("lineIPv6DefaultRouter"));
        sizePolicy2.setHeightForWidth(lineIPv6DefaultRouter->sizePolicy().hasHeightForWidth());
        lineIPv6DefaultRouter->setSizePolicy(sizePolicy2);
        lineIPv6DefaultRouter->setMaxLength(64);

        verticalLayout_7->addWidget(lineIPv6DefaultRouter);

        lineIPv6DNS = new QLineEdit(wizardPage);
        lineIPv6DNS->setObjectName(QString::fromUtf8("lineIPv6DNS"));
        sizePolicy2.setHeightForWidth(lineIPv6DNS->sizePolicy().hasHeightForWidth());
        lineIPv6DNS->setSizePolicy(sizePolicy2);
        lineIPv6DNS->setMaxLength(64);

        verticalLayout_7->addWidget(lineIPv6DNS);


        verticalLayout_9->addLayout(verticalLayout_7);


        horizontalLayout_2->addLayout(verticalLayout_9);


        gridLayout_8->addLayout(horizontalLayout_2, 3, 1, 1, 1);

        horizontalSpacer_8 = new QSpacerItem(18, 210, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout_8->addItem(horizontalSpacer_8, 3, 2, 2, 1);

        verticalSpacer_25 = new QSpacerItem(20, 1, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_8->addItem(verticalSpacer_25, 4, 1, 1, 1);

        wizardFreeBSD->addPage(wizardPage);
        Page_Optional = new QWizardPage();
        Page_Optional->setObjectName(QString::fromUtf8("Page_Optional"));
        gridLayout_7 = new QGridLayout(Page_Optional);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        label_17 = new QLabel(Page_Optional);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        sizePolicy.setHeightForWidth(label_17->sizePolicy().hasHeightForWidth());
        label_17->setSizePolicy(sizePolicy);
        label_17->setMaximumSize(QSize(55, 55));
        label_17->setPixmap(QPixmap(QString::fromUtf8(":/modules/images/systemsettings.png")));
        label_17->setScaledContents(true);

        verticalLayout_5->addWidget(label_17);

        verticalSpacer_17 = new QSpacerItem(20, 78, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer_17);


        gridLayout_7->addLayout(verticalLayout_5, 0, 0, 6, 1);

        verticalSpacer_18 = new QSpacerItem(238, 18, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_7->addItem(verticalSpacer_18, 0, 1, 1, 1);

        label_18 = new QLabel(Page_Optional);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        sizePolicy1.setHeightForWidth(label_18->sizePolicy().hasHeightForWidth());
        label_18->setSizePolicy(sizePolicy1);
        label_18->setWordWrap(true);

        gridLayout_7->addWidget(label_18, 1, 1, 1, 1);

        horizontalSpacer_5 = new QSpacerItem(18, 197, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout_7->addItem(horizontalSpacer_5, 1, 2, 5, 1);

        verticalSpacer_19 = new QSpacerItem(248, 13, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_7->addItem(verticalSpacer_19, 2, 1, 1, 1);

        checkSrc = new QCheckBox(Page_Optional);
        checkSrc->setObjectName(QString::fromUtf8("checkSrc"));

        gridLayout_7->addWidget(checkSrc, 3, 1, 1, 1);

        checkPorts = new QCheckBox(Page_Optional);
        checkPorts->setObjectName(QString::fromUtf8("checkPorts"));

        gridLayout_7->addWidget(checkPorts, 4, 1, 1, 1);

        verticalSpacer_20 = new QSpacerItem(218, 107, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_7->addItem(verticalSpacer_20, 5, 1, 1, 1);

        wizardFreeBSD->addPage(Page_Optional);

        retranslateUi(wizardFreeBSD);

        QMetaObject::connectSlotsByName(wizardFreeBSD);
    } // setupUi

    void retranslateUi(QWizard *wizardFreeBSD)
    {
        label_2->setText(QString());
        label->setText(QApplication::translate("wizardFreeBSD", "This wizard will walk you though the setup of your new server. Click next to begin. ", 0, QApplication::UnicodeUTF8));
        label_13->setText(QString());
        label_3->setText(QApplication::translate("wizardFreeBSD", "Please enter the root password for this server", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("wizardFreeBSD", "Password", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("wizardFreeBSD", "Password (Repeat)", 0, QApplication::UnicodeUTF8));
        label_12->setText(QString());
        label_6->setText(QApplication::translate("wizardFreeBSD", "Please create a user for this server", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("wizardFreeBSD", "Name", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("wizardFreeBSD", "Username", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("wizardFreeBSD", "Password", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("wizardFreeBSD", "Password (Repeat)", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("wizardFreeBSD", "Default shell", 0, QApplication::UnicodeUTF8));
        label_14->setText(QString());
        label_15->setText(QApplication::translate("wizardFreeBSD", "Please enter a system hostname and remote login preference.", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("wizardFreeBSD", "Hostname", 0, QApplication::UnicodeUTF8));
        checkSSH->setText(QApplication::translate("wizardFreeBSD", "Enable remote SSH login", 0, QApplication::UnicodeUTF8));
        label_19->setText(QString());
        label_20->setText(QApplication::translate("wizardFreeBSD", "Please select a network device to configure for this system.", 0, QApplication::UnicodeUTF8));
        label_21->setText(QApplication::translate("wizardFreeBSD", "Network Interface", 0, QApplication::UnicodeUTF8));
        textIP->setText(QApplication::translate("wizardFreeBSD", "IP Address", 0, QApplication::UnicodeUTF8));
        textNetmask->setText(QApplication::translate("wizardFreeBSD", "Netmask", 0, QApplication::UnicodeUTF8));
        textDNS->setText(QApplication::translate("wizardFreeBSD", "Nameserver (DNS)", 0, QApplication::UnicodeUTF8));
        textDefaultRouter->setText(QApplication::translate("wizardFreeBSD", "Gateway", 0, QApplication::UnicodeUTF8));
        textIPv6->setText(QApplication::translate("wizardFreeBSD", "IPv6 Address", 0, QApplication::UnicodeUTF8));
        textIPv6DefaultRouter->setText(QApplication::translate("wizardFreeBSD", "IPv6 Gateway", 0, QApplication::UnicodeUTF8));
        textIPv6DNS->setText(QApplication::translate("wizardFreeBSD", "IPv6 Nameserver", 0, QApplication::UnicodeUTF8));
        lineIP->setInputMask(QApplication::translate("wizardFreeBSD", "000.000.000.000; ", 0, QApplication::UnicodeUTF8));
        lineNetmask->setInputMask(QApplication::translate("wizardFreeBSD", "000.000.000.000; ", 0, QApplication::UnicodeUTF8));
        lineDNS->setInputMask(QApplication::translate("wizardFreeBSD", "000.000.000.000; ", 0, QApplication::UnicodeUTF8));
        lineDefaultRouter->setInputMask(QApplication::translate("wizardFreeBSD", "000.000.000.000; ", 0, QApplication::UnicodeUTF8));
        label_17->setText(QString());
        label_18->setText(QApplication::translate("wizardFreeBSD", "Please select any optional features to install.", 0, QApplication::UnicodeUTF8));
        checkSrc->setText(QApplication::translate("wizardFreeBSD", "Install system sources (/usr/src)", 0, QApplication::UnicodeUTF8));
        checkPorts->setText(QApplication::translate("wizardFreeBSD", "Install ports tree (/usr/ports)", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(wizardFreeBSD);
    } // retranslateUi

};

namespace Ui {
    class wizardFreeBSD: public Ui_wizardFreeBSD {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIZARDFREEBSD_H
