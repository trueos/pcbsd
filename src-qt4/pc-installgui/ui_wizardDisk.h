/********************************************************************************
** Form generated from reading UI file 'wizardDisk.ui'
**
** Created: Wed Mar 20 02:45:43 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIZARDDISK_H
#define UI_WIZARDDISK_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTextEdit>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWizard>
#include <QtGui/QWizardPage>

QT_BEGIN_NAMESPACE

class Ui_wizardDisk
{
public:
    QWizardPage *Page_Intro;
    QGridLayout *gridLayout_2;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_2;
    QSpacerItem *verticalSpacer_10;
    QSpacerItem *verticalSpacer;
    QLabel *label;
    QSpacerItem *horizontalSpacer_2;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QRadioButton *radioBasic;
    QRadioButton *radioAdvanced;
    QRadioButton *radioExpert;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *verticalSpacer_3;
    QWizardPage *Page_BasicDisk;
    QGridLayout *gridLayout_11;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_3;
    QSpacerItem *verticalSpacer_11;
    QSpacerItem *verticalSpacer_5;
    QLabel *label_4;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *verticalSpacer_4;
    QGridLayout *gridLayout_3;
    QComboBox *comboDisk;
    QLabel *label_5;
    QComboBox *comboPartition;
    QLabel *label_6;
    QSpacerItem *verticalSpacer_17;
    QCheckBox *checkGPT;
    QSpacerItem *verticalSpacer_6;
    QWizardPage *Page_FS;
    QGridLayout *gridLayout_7;
    QVBoxLayout *verticalLayout_6;
    QLabel *label_12;
    QSpacerItem *verticalSpacer_16;
    QSpacerItem *verticalSpacer_15;
    QLabel *label_11;
    QSpacerItem *horizontalSpacer_5;
    QSpacerItem *verticalSpacer_13;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_6;
    QRadioButton *radioUFS;
    QRadioButton *radioZFS;
    QCheckBox *checkMBR;
    QSpacerItem *verticalSpacer_20;
    QSpacerItem *verticalSpacer_14;
    QWizardPage *Page_ZFS;
    QGridLayout *gridLayout_13;
    QVBoxLayout *verticalLayout_7;
    QLabel *label_20;
    QSpacerItem *verticalSpacer_31;
    QSpacerItem *verticalSpacer_32;
    QLabel *label_14;
    QSpacerItem *verticalSpacer_30;
    QGroupBox *groupZFSOpts;
    QGridLayout *gridLayout_12;
    QHBoxLayout *horizontalLayout;
    QComboBox *comboZFSMode;
    QLabel *label_21;
    QLabel *labelZFSMsg;
    QListWidget *listZFSDisks;
    QLabel *label_22;
    QWizardPage *Page_BasicEnc;
    QGridLayout *gridLayout_5;
    QVBoxLayout *verticalLayout_5;
    QLabel *label_8;
    QSpacerItem *verticalSpacer_12;
    QSpacerItem *verticalSpacer_8;
    QLabel *label_7;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *verticalSpacer_7;
    QGroupBox *groupEncryption;
    QGridLayout *gridLayout_4;
    QLineEdit *lineEncPW;
    QLabel *label_9;
    QLineEdit *lineEncPW2;
    QLabel *label_10;
    QSpacerItem *verticalSpacer_9;
    QWizardPage *Page_Mounts;
    QGridLayout *gridLayout_10;
    QVBoxLayout *verticalLayout_10;
    QLabel *label_19;
    QSpacerItem *verticalSpacer_28;
    QSpacerItem *verticalSpacer_27;
    QLabel *label_18;
    QSpacerItem *horizontalSpacer_10;
    QSpacerItem *verticalSpacer_29;
    QTreeWidget *treeMounts;
    QHBoxLayout *horizontalLayout_4;
    QLineEdit *lineFreeMB;
    QLabel *labelFreeSpace;
    QSpacerItem *horizontalSpacer_11;
    QPushButton *pushAddMount;
    QPushButton *pushSizeMount;
    QPushButton *pushRemoveMount;
    QWizardPage *Page_Expert;
    QGridLayout *gridLayout_8;
    QVBoxLayout *verticalLayout_8;
    QLabel *label_15;
    QSpacerItem *verticalSpacer_21;
    QSpacerItem *verticalSpacer_18;
    QSpacerItem *horizontalSpacer_6;
    QLabel *label_13;
    QSpacerItem *verticalSpacer_22;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushTerminal;
    QSpacerItem *horizontalSpacer_7;
    QSpacerItem *verticalSpacer_19;
    QSpacerItem *horizontalSpacer_8;
    QWizardPage *Page_Confirmation;
    QGridLayout *gridLayout_9;
    QVBoxLayout *verticalLayout_9;
    QLabel *label_17;
    QSpacerItem *verticalSpacer_25;
    QSpacerItem *verticalSpacer_23;
    QLabel *label_16;
    QSpacerItem *horizontalSpacer_9;
    QSpacerItem *verticalSpacer_26;
    QTextEdit *textConfirmation;
    QSpacerItem *verticalSpacer_24;

    void setupUi(QWizard *wizardDisk)
    {
        if (wizardDisk->objectName().isEmpty())
            wizardDisk->setObjectName(QString::fromUtf8("wizardDisk"));
        wizardDisk->resize(608, 359);
        wizardDisk->setWindowTitle(QString::fromUtf8("PC-BSD"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/modules/images/disk.png"), QSize(), QIcon::Normal, QIcon::Off);
        wizardDisk->setWindowIcon(icon);
        Page_Intro = new QWizardPage();
        Page_Intro->setObjectName(QString::fromUtf8("Page_Intro"));
        gridLayout_2 = new QGridLayout(Page_Intro);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
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
        label_2->setPixmap(QPixmap(QString::fromUtf8(":/modules/images/disk.png")));
        label_2->setScaledContents(true);
        label_2->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(label_2);

        verticalSpacer_10 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_10);


        gridLayout_2->addLayout(verticalLayout_3, 0, 0, 5, 1);

        verticalSpacer = new QSpacerItem(387, 18, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_2->addItem(verticalSpacer, 0, 1, 1, 1);

        label = new QLabel(Page_Intro);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);
        label->setWordWrap(true);

        gridLayout_2->addWidget(label, 1, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(17, 256, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_2, 1, 2, 4, 1);

        groupBox = new QGroupBox(Page_Intro);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        radioBasic = new QRadioButton(groupBox);
        radioBasic->setObjectName(QString::fromUtf8("radioBasic"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/modules/images/basic.png"), QSize(), QIcon::Normal, QIcon::Off);
        radioBasic->setIcon(icon1);
        radioBasic->setChecked(true);

        gridLayout->addWidget(radioBasic, 0, 0, 1, 1);

        radioAdvanced = new QRadioButton(groupBox);
        radioAdvanced->setObjectName(QString::fromUtf8("radioAdvanced"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/modules/images/advanced.png"), QSize(), QIcon::Normal, QIcon::Off);
        radioAdvanced->setIcon(icon2);

        gridLayout->addWidget(radioAdvanced, 1, 0, 1, 1);

        radioExpert = new QRadioButton(groupBox);
        radioExpert->setObjectName(QString::fromUtf8("radioExpert"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/modules/images/expert.png"), QSize(), QIcon::Normal, QIcon::Off);
        radioExpert->setIcon(icon3);

        gridLayout->addWidget(radioExpert, 2, 0, 1, 1);


        gridLayout_2->addWidget(groupBox, 3, 1, 1, 1);

        verticalSpacer_2 = new QSpacerItem(387, 83, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer_2, 4, 1, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 25, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_2->addItem(verticalSpacer_3, 2, 1, 1, 1);

        wizardDisk->addPage(Page_Intro);
        Page_BasicDisk = new QWizardPage();
        Page_BasicDisk->setObjectName(QString::fromUtf8("Page_BasicDisk"));
        gridLayout_11 = new QGridLayout(Page_BasicDisk);
        gridLayout_11->setObjectName(QString::fromUtf8("gridLayout_11"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        label_3 = new QLabel(Page_BasicDisk);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        sizePolicy.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy);
        label_3->setMaximumSize(QSize(55, 55));
        label_3->setPixmap(QPixmap(QString::fromUtf8(":/modules/images/disk.png")));
        label_3->setScaledContents(true);
        label_3->setAlignment(Qt::AlignCenter);

        verticalLayout_4->addWidget(label_3);

        verticalSpacer_11 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_11);


        gridLayout_11->addLayout(verticalLayout_4, 0, 0, 7, 1);

        verticalSpacer_5 = new QSpacerItem(387, 18, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_11->addItem(verticalSpacer_5, 0, 1, 1, 1);

        label_4 = new QLabel(Page_BasicDisk);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        sizePolicy1.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy1);
        label_4->setWordWrap(true);

        gridLayout_11->addWidget(label_4, 1, 1, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(17, 256, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout_11->addItem(horizontalSpacer_3, 1, 2, 6, 1);

        verticalSpacer_4 = new QSpacerItem(387, 18, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_11->addItem(verticalSpacer_4, 2, 1, 1, 1);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        comboDisk = new QComboBox(Page_BasicDisk);
        comboDisk->setObjectName(QString::fromUtf8("comboDisk"));
        QSizePolicy sizePolicy2(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(comboDisk->sizePolicy().hasHeightForWidth());
        comboDisk->setSizePolicy(sizePolicy2);

        gridLayout_3->addWidget(comboDisk, 0, 0, 1, 1);

        label_5 = new QLabel(Page_BasicDisk);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
        label_5->setSizePolicy(sizePolicy3);

        gridLayout_3->addWidget(label_5, 0, 1, 1, 1);

        comboPartition = new QComboBox(Page_BasicDisk);
        comboPartition->setObjectName(QString::fromUtf8("comboPartition"));
        sizePolicy2.setHeightForWidth(comboPartition->sizePolicy().hasHeightForWidth());
        comboPartition->setSizePolicy(sizePolicy2);

        gridLayout_3->addWidget(comboPartition, 1, 0, 1, 1);

        label_6 = new QLabel(Page_BasicDisk);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        sizePolicy3.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy3);

        gridLayout_3->addWidget(label_6, 1, 1, 1, 1);


        gridLayout_11->addLayout(gridLayout_3, 3, 1, 1, 1);

        verticalSpacer_17 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_11->addItem(verticalSpacer_17, 4, 1, 1, 1);

        checkGPT = new QCheckBox(Page_BasicDisk);
        checkGPT->setObjectName(QString::fromUtf8("checkGPT"));

        gridLayout_11->addWidget(checkGPT, 5, 1, 1, 1);

        verticalSpacer_6 = new QSpacerItem(387, 43, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_11->addItem(verticalSpacer_6, 6, 1, 1, 1);

        wizardDisk->addPage(Page_BasicDisk);
        Page_FS = new QWizardPage();
        Page_FS->setObjectName(QString::fromUtf8("Page_FS"));
        gridLayout_7 = new QGridLayout(Page_FS);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        label_12 = new QLabel(Page_FS);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        sizePolicy.setHeightForWidth(label_12->sizePolicy().hasHeightForWidth());
        label_12->setSizePolicy(sizePolicy);
        label_12->setMaximumSize(QSize(55, 55));
        label_12->setPixmap(QPixmap(QString::fromUtf8(":/modules/images/disk.png")));
        label_12->setScaledContents(true);
        label_12->setAlignment(Qt::AlignCenter);

        verticalLayout_6->addWidget(label_12);

        verticalSpacer_16 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_6->addItem(verticalSpacer_16);


        gridLayout_7->addLayout(verticalLayout_6, 0, 0, 5, 1);

        verticalSpacer_15 = new QSpacerItem(378, 18, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_7->addItem(verticalSpacer_15, 0, 1, 1, 1);

        label_11 = new QLabel(Page_FS);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        sizePolicy1.setHeightForWidth(label_11->sizePolicy().hasHeightForWidth());
        label_11->setSizePolicy(sizePolicy1);
        label_11->setWordWrap(true);

        gridLayout_7->addWidget(label_11, 1, 1, 1, 1);

        horizontalSpacer_5 = new QSpacerItem(17, 256, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout_7->addItem(horizontalSpacer_5, 1, 2, 4, 1);

        verticalSpacer_13 = new QSpacerItem(20, 18, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_7->addItem(verticalSpacer_13, 2, 1, 1, 1);

        groupBox_2 = new QGroupBox(Page_FS);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout_6 = new QGridLayout(groupBox_2);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        radioUFS = new QRadioButton(groupBox_2);
        radioUFS->setObjectName(QString::fromUtf8("radioUFS"));
        radioUFS->setChecked(true);

        gridLayout_6->addWidget(radioUFS, 0, 0, 1, 1);

        radioZFS = new QRadioButton(groupBox_2);
        radioZFS->setObjectName(QString::fromUtf8("radioZFS"));

        gridLayout_6->addWidget(radioZFS, 1, 0, 1, 1);

        checkMBR = new QCheckBox(groupBox_2);
        checkMBR->setObjectName(QString::fromUtf8("checkMBR"));
        checkMBR->setChecked(true);

        gridLayout_6->addWidget(checkMBR, 3, 0, 1, 1);

        verticalSpacer_20 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_6->addItem(verticalSpacer_20, 2, 0, 1, 1);


        gridLayout_7->addWidget(groupBox_2, 3, 1, 1, 1);

        verticalSpacer_14 = new QSpacerItem(20, 117, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_7->addItem(verticalSpacer_14, 4, 1, 1, 1);

        wizardDisk->addPage(Page_FS);
        Page_ZFS = new QWizardPage();
        Page_ZFS->setObjectName(QString::fromUtf8("Page_ZFS"));
        gridLayout_13 = new QGridLayout(Page_ZFS);
        gridLayout_13->setObjectName(QString::fromUtf8("gridLayout_13"));
        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        label_20 = new QLabel(Page_ZFS);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        sizePolicy.setHeightForWidth(label_20->sizePolicy().hasHeightForWidth());
        label_20->setSizePolicy(sizePolicy);
        label_20->setMaximumSize(QSize(55, 55));
        label_20->setPixmap(QPixmap(QString::fromUtf8(":/modules/images/disk.png")));
        label_20->setScaledContents(true);
        label_20->setAlignment(Qt::AlignCenter);

        verticalLayout_7->addWidget(label_20);

        verticalSpacer_31 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_7->addItem(verticalSpacer_31);


        gridLayout_13->addLayout(verticalLayout_7, 0, 0, 6, 1);

        verticalSpacer_32 = new QSpacerItem(378, 18, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_13->addItem(verticalSpacer_32, 0, 1, 1, 1);

        label_14 = new QLabel(Page_ZFS);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        sizePolicy1.setHeightForWidth(label_14->sizePolicy().hasHeightForWidth());
        label_14->setSizePolicy(sizePolicy1);
        label_14->setWordWrap(true);

        gridLayout_13->addWidget(label_14, 1, 1, 1, 1);

        verticalSpacer_30 = new QSpacerItem(20, 18, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_13->addItem(verticalSpacer_30, 3, 1, 1, 1);

        groupZFSOpts = new QGroupBox(Page_ZFS);
        groupZFSOpts->setObjectName(QString::fromUtf8("groupZFSOpts"));
        groupZFSOpts->setCheckable(true);
        groupZFSOpts->setChecked(false);
        gridLayout_12 = new QGridLayout(groupZFSOpts);
        gridLayout_12->setObjectName(QString::fromUtf8("gridLayout_12"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        comboZFSMode = new QComboBox(groupZFSOpts);
        comboZFSMode->insertItems(0, QStringList()
         << QString::fromUtf8("mirror")
         << QString::fromUtf8("raidz1")
         << QString::fromUtf8("raidz2")
         << QString::fromUtf8("raidz3")
        );
        comboZFSMode->setObjectName(QString::fromUtf8("comboZFSMode"));

        horizontalLayout->addWidget(comboZFSMode);

        label_21 = new QLabel(groupZFSOpts);
        label_21->setObjectName(QString::fromUtf8("label_21"));

        horizontalLayout->addWidget(label_21);


        gridLayout_12->addLayout(horizontalLayout, 0, 0, 1, 1);

        labelZFSMsg = new QLabel(groupZFSOpts);
        labelZFSMsg->setObjectName(QString::fromUtf8("labelZFSMsg"));

        gridLayout_12->addWidget(labelZFSMsg, 1, 0, 1, 1);

        listZFSDisks = new QListWidget(groupZFSOpts);
        listZFSDisks->setObjectName(QString::fromUtf8("listZFSDisks"));

        gridLayout_12->addWidget(listZFSDisks, 2, 0, 1, 1);


        gridLayout_13->addWidget(groupZFSOpts, 5, 1, 1, 1);

        label_22 = new QLabel(Page_ZFS);
        label_22->setObjectName(QString::fromUtf8("label_22"));
        QFont font;
        font.setBold(false);
        font.setWeight(50);
        label_22->setFont(font);
        label_22->setWordWrap(true);

        gridLayout_13->addWidget(label_22, 6, 1, 1, 1);

        wizardDisk->addPage(Page_ZFS);
        Page_BasicEnc = new QWizardPage();
        Page_BasicEnc->setObjectName(QString::fromUtf8("Page_BasicEnc"));
        gridLayout_5 = new QGridLayout(Page_BasicEnc);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        label_8 = new QLabel(Page_BasicEnc);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        sizePolicy.setHeightForWidth(label_8->sizePolicy().hasHeightForWidth());
        label_8->setSizePolicy(sizePolicy);
        label_8->setMaximumSize(QSize(55, 55));
        label_8->setPixmap(QPixmap(QString::fromUtf8(":/modules/images/encrypted.png")));
        label_8->setScaledContents(true);
        label_8->setAlignment(Qt::AlignCenter);

        verticalLayout_5->addWidget(label_8);

        verticalSpacer_12 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer_12);


        gridLayout_5->addLayout(verticalLayout_5, 0, 0, 5, 1);

        verticalSpacer_8 = new QSpacerItem(378, 18, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_5->addItem(verticalSpacer_8, 0, 1, 1, 1);

        label_7 = new QLabel(Page_BasicEnc);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        sizePolicy1.setHeightForWidth(label_7->sizePolicy().hasHeightForWidth());
        label_7->setSizePolicy(sizePolicy1);
        label_7->setWordWrap(true);

        gridLayout_5->addWidget(label_7, 1, 1, 1, 1);

        horizontalSpacer_4 = new QSpacerItem(17, 256, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout_5->addItem(horizontalSpacer_4, 1, 2, 4, 1);

        verticalSpacer_7 = new QSpacerItem(20, 18, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_5->addItem(verticalSpacer_7, 2, 1, 1, 1);

        groupEncryption = new QGroupBox(Page_BasicEnc);
        groupEncryption->setObjectName(QString::fromUtf8("groupEncryption"));
        groupEncryption->setCheckable(true);
        groupEncryption->setChecked(false);
        gridLayout_4 = new QGridLayout(groupEncryption);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        lineEncPW = new QLineEdit(groupEncryption);
        lineEncPW->setObjectName(QString::fromUtf8("lineEncPW"));
        lineEncPW->setEchoMode(QLineEdit::Password);

        gridLayout_4->addWidget(lineEncPW, 0, 0, 1, 1);

        label_9 = new QLabel(groupEncryption);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout_4->addWidget(label_9, 0, 1, 1, 1);

        lineEncPW2 = new QLineEdit(groupEncryption);
        lineEncPW2->setObjectName(QString::fromUtf8("lineEncPW2"));
        lineEncPW2->setEchoMode(QLineEdit::Password);

        gridLayout_4->addWidget(lineEncPW2, 1, 0, 1, 1);

        label_10 = new QLabel(groupEncryption);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout_4->addWidget(label_10, 1, 1, 1, 1);


        gridLayout_5->addWidget(groupEncryption, 3, 1, 1, 1);

        verticalSpacer_9 = new QSpacerItem(20, 113, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_5->addItem(verticalSpacer_9, 4, 1, 1, 1);

        wizardDisk->addPage(Page_BasicEnc);
        Page_Mounts = new QWizardPage();
        Page_Mounts->setObjectName(QString::fromUtf8("Page_Mounts"));
        gridLayout_10 = new QGridLayout(Page_Mounts);
        gridLayout_10->setObjectName(QString::fromUtf8("gridLayout_10"));
        verticalLayout_10 = new QVBoxLayout();
        verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
        label_19 = new QLabel(Page_Mounts);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        sizePolicy.setHeightForWidth(label_19->sizePolicy().hasHeightForWidth());
        label_19->setSizePolicy(sizePolicy);
        label_19->setMaximumSize(QSize(55, 55));
        label_19->setPixmap(QPixmap(QString::fromUtf8(":/modules/images/disk.png")));
        label_19->setScaledContents(true);
        label_19->setAlignment(Qt::AlignCenter);

        verticalLayout_10->addWidget(label_19);

        verticalSpacer_28 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_10->addItem(verticalSpacer_28);


        gridLayout_10->addLayout(verticalLayout_10, 0, 0, 5, 1);

        verticalSpacer_27 = new QSpacerItem(378, 18, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_10->addItem(verticalSpacer_27, 0, 1, 1, 1);

        label_18 = new QLabel(Page_Mounts);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        sizePolicy1.setHeightForWidth(label_18->sizePolicy().hasHeightForWidth());
        label_18->setSizePolicy(sizePolicy1);
        label_18->setWordWrap(true);

        gridLayout_10->addWidget(label_18, 1, 1, 1, 1);

        horizontalSpacer_10 = new QSpacerItem(17, 256, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout_10->addItem(horizontalSpacer_10, 1, 2, 4, 1);

        verticalSpacer_29 = new QSpacerItem(20, 18, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_10->addItem(verticalSpacer_29, 2, 1, 1, 1);

        treeMounts = new QTreeWidget(Page_Mounts);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        treeMounts->setHeaderItem(__qtreewidgetitem);
        treeMounts->setObjectName(QString::fromUtf8("treeMounts"));

        gridLayout_10->addWidget(treeMounts, 3, 1, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        lineFreeMB = new QLineEdit(Page_Mounts);
        lineFreeMB->setObjectName(QString::fromUtf8("lineFreeMB"));
        lineFreeMB->setReadOnly(true);

        horizontalLayout_4->addWidget(lineFreeMB);

        labelFreeSpace = new QLabel(Page_Mounts);
        labelFreeSpace->setObjectName(QString::fromUtf8("labelFreeSpace"));

        horizontalLayout_4->addWidget(labelFreeSpace);

        horizontalSpacer_11 = new QSpacerItem(18, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_11);

        pushAddMount = new QPushButton(Page_Mounts);
        pushAddMount->setObjectName(QString::fromUtf8("pushAddMount"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/PCBSD/images/edit_add.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushAddMount->setIcon(icon4);

        horizontalLayout_4->addWidget(pushAddMount);

        pushSizeMount = new QPushButton(Page_Mounts);
        pushSizeMount->setObjectName(QString::fromUtf8("pushSizeMount"));
        pushSizeMount->setIcon(icon);

        horizontalLayout_4->addWidget(pushSizeMount);

        pushRemoveMount = new QPushButton(Page_Mounts);
        pushRemoveMount->setObjectName(QString::fromUtf8("pushRemoveMount"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/PCBSD/images/edit_remove.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushRemoveMount->setIcon(icon5);

        horizontalLayout_4->addWidget(pushRemoveMount);


        gridLayout_10->addLayout(horizontalLayout_4, 4, 1, 1, 1);

        wizardDisk->addPage(Page_Mounts);
        Page_Expert = new QWizardPage();
        Page_Expert->setObjectName(QString::fromUtf8("Page_Expert"));
        gridLayout_8 = new QGridLayout(Page_Expert);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        label_15 = new QLabel(Page_Expert);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        sizePolicy.setHeightForWidth(label_15->sizePolicy().hasHeightForWidth());
        label_15->setSizePolicy(sizePolicy);
        label_15->setMaximumSize(QSize(55, 55));
        label_15->setPixmap(QPixmap(QString::fromUtf8(":/modules/images/systemsettings.png")));
        label_15->setScaledContents(true);
        label_15->setAlignment(Qt::AlignCenter);

        verticalLayout_8->addWidget(label_15);

        verticalSpacer_21 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_8->addItem(verticalSpacer_21);


        gridLayout_8->addLayout(verticalLayout_8, 0, 0, 6, 1);

        verticalSpacer_18 = new QSpacerItem(298, 18, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_8->addItem(verticalSpacer_18, 0, 1, 1, 1);

        horizontalSpacer_6 = new QSpacerItem(18, 278, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout_8->addItem(horizontalSpacer_6, 0, 2, 6, 1);

        label_13 = new QLabel(Page_Expert);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(label_13->sizePolicy().hasHeightForWidth());
        label_13->setSizePolicy(sizePolicy4);
        label_13->setMaximumSize(QSize(700, 16777215));
        label_13->setWordWrap(true);

        gridLayout_8->addWidget(label_13, 1, 1, 1, 1);

        verticalSpacer_22 = new QSpacerItem(248, 18, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_8->addItem(verticalSpacer_22, 2, 1, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Preferred, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        pushTerminal = new QPushButton(Page_Expert);
        pushTerminal->setObjectName(QString::fromUtf8("pushTerminal"));
        QSizePolicy sizePolicy5(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(pushTerminal->sizePolicy().hasHeightForWidth());
        pushTerminal->setSizePolicy(sizePolicy5);

        horizontalLayout_3->addWidget(pushTerminal);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Preferred, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_7);


        gridLayout_8->addLayout(horizontalLayout_3, 3, 1, 1, 1);

        verticalSpacer_19 = new QSpacerItem(20, 93, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_8->addItem(verticalSpacer_19, 4, 1, 1, 1);

        horizontalSpacer_8 = new QSpacerItem(386, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_8->addItem(horizontalSpacer_8, 5, 1, 1, 1);

        wizardDisk->addPage(Page_Expert);
        Page_Confirmation = new QWizardPage();
        Page_Confirmation->setObjectName(QString::fromUtf8("Page_Confirmation"));
        gridLayout_9 = new QGridLayout(Page_Confirmation);
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        verticalLayout_9 = new QVBoxLayout();
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        label_17 = new QLabel(Page_Confirmation);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        sizePolicy.setHeightForWidth(label_17->sizePolicy().hasHeightForWidth());
        label_17->setSizePolicy(sizePolicy);
        label_17->setMaximumSize(QSize(55, 55));
        label_17->setPixmap(QPixmap(QString::fromUtf8(":/modules/images/disk.png")));
        label_17->setScaledContents(true);
        label_17->setAlignment(Qt::AlignCenter);

        verticalLayout_9->addWidget(label_17);

        verticalSpacer_25 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_9->addItem(verticalSpacer_25);


        gridLayout_9->addLayout(verticalLayout_9, 0, 0, 5, 1);

        verticalSpacer_23 = new QSpacerItem(378, 18, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_9->addItem(verticalSpacer_23, 0, 1, 1, 1);

        label_16 = new QLabel(Page_Confirmation);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        sizePolicy1.setHeightForWidth(label_16->sizePolicy().hasHeightForWidth());
        label_16->setSizePolicy(sizePolicy1);
        label_16->setWordWrap(true);

        gridLayout_9->addWidget(label_16, 1, 1, 1, 1);

        horizontalSpacer_9 = new QSpacerItem(17, 256, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout_9->addItem(horizontalSpacer_9, 1, 2, 4, 1);

        verticalSpacer_26 = new QSpacerItem(20, 18, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_9->addItem(verticalSpacer_26, 2, 1, 1, 1);

        textConfirmation = new QTextEdit(Page_Confirmation);
        textConfirmation->setObjectName(QString::fromUtf8("textConfirmation"));
        textConfirmation->setReadOnly(true);

        gridLayout_9->addWidget(textConfirmation, 3, 1, 1, 1);

        verticalSpacer_24 = new QSpacerItem(20, 8, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_9->addItem(verticalSpacer_24, 4, 1, 1, 1);

        wizardDisk->addPage(Page_Confirmation);

        retranslateUi(wizardDisk);

        QMetaObject::connectSlotsByName(wizardDisk);
    } // setupUi

    void retranslateUi(QWizard *wizardDisk)
    {
        label_2->setText(QString());
        label->setText(QApplication::translate("wizardDisk", "This wizard will walk you through the disk setup process. Please choose the setup mode you wish to use.", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("wizardDisk", "Setup Mode", 0, QApplication::UnicodeUTF8));
        radioBasic->setText(QApplication::translate("wizardDisk", "Basic (New to BSD or disk partitioning)", 0, QApplication::UnicodeUTF8));
        radioAdvanced->setText(QApplication::translate("wizardDisk", "Advanced (Experienced with file-systems)", 0, QApplication::UnicodeUTF8));
        radioExpert->setText(QApplication::translate("wizardDisk", "FreeBSD Experts (CLI manual mode)", 0, QApplication::UnicodeUTF8));
        label_3->setText(QString());
        label_4->setText(QApplication::translate("wizardDisk", "Please select the disk or partition you wish to install onto. Please note that BSD can only be installed to an entire disk, or primary partition larger than 10GB. ", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("wizardDisk", "Selected Disk", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("wizardDisk", "Selected Partition", 0, QApplication::UnicodeUTF8));
        checkGPT->setText(QApplication::translate("wizardDisk", "Partition disk with GPT", 0, QApplication::UnicodeUTF8));
        label_12->setText(QString());
        label_11->setText(QApplication::translate("wizardDisk", "Please select the type of filesystem you want to use for this installation.", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("wizardDisk", "Filesystem", 0, QApplication::UnicodeUTF8));
        radioUFS->setText(QApplication::translate("wizardDisk", "UFS (32bit and systems with less than 2GB of RAM)", 0, QApplication::UnicodeUTF8));
        radioZFS->setText(QApplication::translate("wizardDisk", "ZFS (64bit systems with more than 2GB RAM)", 0, QApplication::UnicodeUTF8));
        checkMBR->setText(QApplication::translate("wizardDisk", "Install bootable MBR", 0, QApplication::UnicodeUTF8));
        label_20->setText(QString());
        label_14->setText(QApplication::translate("wizardDisk", "If this is a single disk ZFS install, you can continue, otherwise please select the mirror / raid mode and disks below. ", 0, QApplication::UnicodeUTF8));
        groupZFSOpts->setTitle(QApplication::translate("wizardDisk", "Enable ZFS mirror/raidz mode", 0, QApplication::UnicodeUTF8));
        label_21->setText(QApplication::translate("wizardDisk", "ZFS Virtual Device Mode", 0, QApplication::UnicodeUTF8));
        labelZFSMsg->setText(QApplication::translate("wizardDisk", "Please select at least 1 other drive for mirroring", 0, QApplication::UnicodeUTF8));
        label_22->setText(QApplication::translate("wizardDisk", "Note: Using ZFS mirror/raidz can only be enabled when doing full-disk installations", 0, QApplication::UnicodeUTF8));
        label_8->setText(QString());
        label_7->setText(QApplication::translate("wizardDisk", "Do you wish to Encrypt your private data? You will need to enter a password at each system bootup.", 0, QApplication::UnicodeUTF8));
        groupEncryption->setTitle(QApplication::translate("wizardDisk", "Encrypt user data", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("wizardDisk", "Passphrase", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("wizardDisk", "Passphrase (repeat)", 0, QApplication::UnicodeUTF8));
        label_19->setText(QString());
        label_18->setText(QApplication::translate("wizardDisk", "Below you may adjust the file-system mount points. For most users the defaults will work best. ", 0, QApplication::UnicodeUTF8));
        labelFreeSpace->setText(QApplication::translate("wizardDisk", "Free Space (MB)", 0, QApplication::UnicodeUTF8));
        pushAddMount->setText(QApplication::translate("wizardDisk", "&Add", 0, QApplication::UnicodeUTF8));
        pushSizeMount->setText(QApplication::translate("wizardDisk", "Resize", 0, QApplication::UnicodeUTF8));
        pushRemoveMount->setText(QApplication::translate("wizardDisk", "&Remove", 0, QApplication::UnicodeUTF8));
        label_15->setText(QString());
        label_13->setText(QApplication::translate("wizardDisk", "You have chosen Expert mode. In this mode you are able to launch a terminal window and use the command-line to manually partition your disk. When you are finished, make sure to mount your filesystems to /mnt to begin the installation. ", 0, QApplication::UnicodeUTF8));
        pushTerminal->setText(QApplication::translate("wizardDisk", "&Launch terminal", 0, QApplication::UnicodeUTF8));
        label_17->setText(QString());
        label_16->setText(QApplication::translate("wizardDisk", "Please check the disk summary below and click finish to save your settings. ", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(wizardDisk);
    } // retranslateUi

};

namespace Ui {
    class wizardDisk: public Ui_wizardDisk {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIZARDDISK_H
