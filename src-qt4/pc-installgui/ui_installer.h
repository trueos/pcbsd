/********************************************************************************
** Form generated from reading UI file 'installer.ui'
**
** Created: Fri Mar 22 12:18:07 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INSTALLER_H
#define UI_INSTALLER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGraphicsView>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QStackedWidget>
#include <QtGui/QStatusBar>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Installer
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QStackedWidget *installStackWidget;
    QWidget *page;
    QGridLayout *gridLayout_3;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *horizontalSpacer_3;
    QLabel *label;
    QComboBox *comboLanguage;
    QSpacerItem *verticalSpacer;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_2;
    QLabel *label_2;
    QWidget *page_2;
    QGridLayout *gridLayout_5;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushDeskLeft;
    QGraphicsView *graphicsViewOS;
    QPushButton *pushDeskRight;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_6;
    QPushButton *pushDeskCustomize;
    QSpacerItem *horizontalSpacer_7;
    QSpacerItem *verticalSpacer_3;
    QLabel *label_4;
    QSpacerItem *horizontalSpacer_4;
    QGroupBox *groupDeskSummary;
    QGridLayout *gridLayout_4;
    QTextEdit *textDeskSummary;
    QSpacerItem *horizontalSpacer_5;
    QSpacerItem *verticalSpacer_4;
    QLabel *label_7;
    QWidget *page_3;
    QGridLayout *gridLayout_7;
    QSpacerItem *horizontalSpacer_12;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer_6;
    QLabel *label_5;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_10;
    QLabel *label_6;
    QSpacerItem *horizontalSpacer_11;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_8;
    QPushButton *pushDiskCustomize;
    QSpacerItem *horizontalSpacer_9;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_6;
    QTextEdit *textEditDiskSummary;
    QSpacerItem *verticalSpacer_5;
    QSpacerItem *horizontalSpacer_13;
    QWidget *page_4;
    QGridLayout *gridLayout_8;
    QSpacerItem *horizontalSpacer_14;
    QSpacerItem *verticalSpacer_7;
    QGroupBox *groupBox_5;
    QGridLayout *gridLayout_21;
    QLabel *labelInstallHeader;
    QSpacerItem *verticalSpacer_19;
    QLabel *labelInstallStatus;
    QProgressBar *progressBarInstall;
    QProgressBar *progressBarInstall2;
    QLabel *labelInstallStatus2;
    QSpacerItem *verticalSpacer_32;
    QSpacerItem *horizontalSpacer_15;
    QSpacerItem *verticalSpacer_8;
    QWidget *page_5;
    QGridLayout *gridLayout_9;
    QSpacerItem *verticalSpacer_10;
    QSpacerItem *horizontalSpacer_16;
    QLabel *label_3;
    QSpacerItem *horizontalSpacer_17;
    QSpacerItem *verticalSpacer_9;
    QWidget *footerWidget;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushHardware;
    QPushButton *helpButton;
    QPushButton *pushTouchKeyboard;
    QPushButton *pushChangeKeyLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *abortButton;
    QPushButton *backButton;
    QPushButton *nextButton;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *Installer)
    {
        if (Installer->objectName().isEmpty())
            Installer->setObjectName(QString::fromUtf8("Installer"));
        Installer->resize(650, 538);
        Installer->setWindowTitle(QString::fromUtf8("PC-BSD"));
        Installer->setWindowOpacity(1);
        Installer->setStyleSheet(QString::fromUtf8(""));
        centralwidget = new QWidget(Installer);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        centralwidget->setStyleSheet(QString::fromUtf8(""));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setSpacing(0);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        installStackWidget = new QStackedWidget(centralwidget);
        installStackWidget->setObjectName(QString::fromUtf8("installStackWidget"));
        installStackWidget->setAutoFillBackground(false);
        installStackWidget->setStyleSheet(QString::fromUtf8("background-image: url(:/modules/images/backgroundimage.jpg);"));
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        gridLayout_3 = new QGridLayout(page);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_3->setContentsMargins(-1, -1, -1, 6);
        verticalSpacer_2 = new QSpacerItem(18, 93, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer_2, 0, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(170, 308, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_2, 0, 0, 5, 1);

        horizontalSpacer_3 = new QSpacerItem(169, 278, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_3, 0, 2, 5, 1);

        label = new QLabel(page);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAutoFillBackground(false);
        label->setStyleSheet(QString::fromUtf8("border: transparent;\n"
"background: transparent;"));
        label->setPixmap(QPixmap(QString::fromUtf8(":/PCBSD/images/pcbsd-banner.png")));
        label->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(label, 1, 1, 1, 1);

        comboLanguage = new QComboBox(page);
        comboLanguage->setObjectName(QString::fromUtf8("comboLanguage"));

        gridLayout_3->addWidget(comboLanguage, 2, 1, 1, 1);

        verticalSpacer = new QSpacerItem(20, 92, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer, 4, 1, 1, 1);

        groupBox = new QGroupBox(page);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setStyleSheet(QString::fromUtf8("border: transparent;\n"
"background: transparent;"));
        groupBox->setFlat(false);
        gridLayout_2 = new QGridLayout(groupBox);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAutoFillBackground(false);
        label_2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        label_2->setWordWrap(true);

        gridLayout_2->addWidget(label_2, 0, 0, 1, 1);


        gridLayout_3->addWidget(groupBox, 3, 1, 1, 1);

        installStackWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        gridLayout_5 = new QGridLayout(page_2);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pushDeskLeft = new QPushButton(page_2);
        pushDeskLeft->setObjectName(QString::fromUtf8("pushDeskLeft"));
        pushDeskLeft->setStyleSheet(QString::fromUtf8("background: transparent;"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/PCBSD/images/previous.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushDeskLeft->setIcon(icon);
        pushDeskLeft->setIconSize(QSize(48, 48));
        pushDeskLeft->setFlat(false);

        horizontalLayout->addWidget(pushDeskLeft);

        graphicsViewOS = new QGraphicsView(page_2);
        graphicsViewOS->setObjectName(QString::fromUtf8("graphicsViewOS"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(graphicsViewOS->sizePolicy().hasHeightForWidth());
        graphicsViewOS->setSizePolicy(sizePolicy);
        graphicsViewOS->setMaximumSize(QSize(220, 65));
        graphicsViewOS->setStyleSheet(QString::fromUtf8("background: transparent;"));
        graphicsViewOS->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        graphicsViewOS->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        graphicsViewOS->setInteractive(false);

        horizontalLayout->addWidget(graphicsViewOS);

        pushDeskRight = new QPushButton(page_2);
        pushDeskRight->setObjectName(QString::fromUtf8("pushDeskRight"));
        pushDeskRight->setStyleSheet(QString::fromUtf8("background: transparent;"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/PCBSD/images/next.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushDeskRight->setIcon(icon1);
        pushDeskRight->setIconSize(QSize(48, 48));
        pushDeskRight->setFlat(false);

        horizontalLayout->addWidget(pushDeskRight);


        gridLayout_5->addLayout(horizontalLayout, 2, 1, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_6);

        pushDeskCustomize = new QPushButton(page_2);
        pushDeskCustomize->setObjectName(QString::fromUtf8("pushDeskCustomize"));
        pushDeskCustomize->setAutoFillBackground(false);
        pushDeskCustomize->setStyleSheet(QString::fromUtf8("background-color: rgb(238, 238, 238);"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/PCBSD/images/edit.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushDeskCustomize->setIcon(icon2);
        pushDeskCustomize->setAutoDefault(false);
        pushDeskCustomize->setDefault(false);
        pushDeskCustomize->setFlat(false);

        horizontalLayout_3->addWidget(pushDeskCustomize);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_7);


        gridLayout_5->addLayout(horizontalLayout_3, 4, 1, 1, 1);

        verticalSpacer_3 = new QSpacerItem(248, 18, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_5->addItem(verticalSpacer_3, 6, 1, 1, 1);

        label_4 = new QLabel(page_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy1);
        label_4->setMaximumSize(QSize(16777215, 28));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label_4->setFont(font);
        label_4->setAutoFillBackground(false);
        label_4->setStyleSheet(QString::fromUtf8("background: transparent;"));
        label_4->setAlignment(Qt::AlignCenter);

        gridLayout_5->addWidget(label_4, 1, 1, 1, 1);

        horizontalSpacer_4 = new QSpacerItem(115, 278, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_5->addItem(horizontalSpacer_4, 1, 0, 5, 1);

        groupDeskSummary = new QGroupBox(page_2);
        groupDeskSummary->setObjectName(QString::fromUtf8("groupDeskSummary"));
        groupDeskSummary->setAutoFillBackground(false);
        groupDeskSummary->setStyleSheet(QString::fromUtf8("background: transparent;"));
        groupDeskSummary->setAlignment(Qt::AlignCenter);
        gridLayout_4 = new QGridLayout(groupDeskSummary);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        textDeskSummary = new QTextEdit(groupDeskSummary);
        textDeskSummary->setObjectName(QString::fromUtf8("textDeskSummary"));
        textDeskSummary->setAutoFillBackground(false);
        textDeskSummary->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        textDeskSummary->setReadOnly(true);

        gridLayout_4->addWidget(textDeskSummary, 1, 0, 1, 1);


        gridLayout_5->addWidget(groupDeskSummary, 5, 1, 1, 1);

        horizontalSpacer_5 = new QSpacerItem(115, 298, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_5->addItem(horizontalSpacer_5, 1, 2, 6, 1);

        verticalSpacer_4 = new QSpacerItem(20, 27, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_5->addItem(verticalSpacer_4, 0, 1, 1, 1);

        label_7 = new QLabel(page_2);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setStyleSheet(QString::fromUtf8("background: transparent;"));
        label_7->setAlignment(Qt::AlignCenter);

        gridLayout_5->addWidget(label_7, 3, 1, 1, 1);

        installStackWidget->addWidget(page_2);
        page_3 = new QWidget();
        page_3->setObjectName(QString::fromUtf8("page_3"));
        gridLayout_7 = new QGridLayout(page_3);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        horizontalSpacer_12 = new QSpacerItem(100, 341, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout_7->addItem(horizontalSpacer_12, 0, 0, 4, 1);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalSpacer_6 = new QSpacerItem(228, 40, QSizePolicy::Minimum, QSizePolicy::Minimum);

        verticalLayout_2->addItem(verticalSpacer_6);

        label_5 = new QLabel(page_3);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setFont(font);
        label_5->setStyleSheet(QString::fromUtf8("background: transparent;"));
        label_5->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(label_5);


        gridLayout_7->addLayout(verticalLayout_2, 0, 1, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_10);

        label_6 = new QLabel(page_3);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setMaximumSize(QSize(75, 75));
        label_6->setStyleSheet(QString::fromUtf8("background: transparent;"));
        label_6->setPixmap(QPixmap(QString::fromUtf8(":/modules/images/disk.png")));
        label_6->setAlignment(Qt::AlignCenter);

        horizontalLayout_5->addWidget(label_6);

        horizontalSpacer_11 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_11);


        gridLayout_7->addLayout(horizontalLayout_5, 1, 1, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_8);

        pushDiskCustomize = new QPushButton(page_3);
        pushDiskCustomize->setObjectName(QString::fromUtf8("pushDiskCustomize"));
        pushDiskCustomize->setStyleSheet(QString::fromUtf8("background-color: rgb(238, 238, 238);"));
        pushDiskCustomize->setIcon(icon2);
        pushDiskCustomize->setFlat(false);

        horizontalLayout_4->addWidget(pushDiskCustomize);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_9);


        gridLayout_7->addLayout(horizontalLayout_4, 2, 1, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox_3 = new QGroupBox(page_3);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(groupBox_3->sizePolicy().hasHeightForWidth());
        groupBox_3->setSizePolicy(sizePolicy2);
        groupBox_3->setStyleSheet(QString::fromUtf8("background: transparent;"));
        gridLayout_6 = new QGridLayout(groupBox_3);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        textEditDiskSummary = new QTextEdit(groupBox_3);
        textEditDiskSummary->setObjectName(QString::fromUtf8("textEditDiskSummary"));
        textEditDiskSummary->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        textEditDiskSummary->setReadOnly(true);

        gridLayout_6->addWidget(textEditDiskSummary, 1, 0, 1, 1);


        verticalLayout->addWidget(groupBox_3);

        verticalSpacer_5 = new QSpacerItem(268, 20, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer_5);


        gridLayout_7->addLayout(verticalLayout, 3, 1, 1, 1);

        horizontalSpacer_13 = new QSpacerItem(100, 278, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout_7->addItem(horizontalSpacer_13, 0, 2, 4, 1);

        installStackWidget->addWidget(page_3);
        page_4 = new QWidget();
        page_4->setObjectName(QString::fromUtf8("page_4"));
        gridLayout_8 = new QGridLayout(page_4);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        horizontalSpacer_14 = new QSpacerItem(50, 318, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout_8->addItem(horizontalSpacer_14, 0, 0, 3, 1);

        verticalSpacer_7 = new QSpacerItem(20, 29, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_8->addItem(verticalSpacer_7, 0, 1, 1, 1);

        groupBox_5 = new QGroupBox(page_4);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        QFont font1;
        font1.setBold(false);
        font1.setWeight(50);
        groupBox_5->setFont(font1);
        groupBox_5->setStyleSheet(QString::fromUtf8("background: transparent;"));
        gridLayout_21 = new QGridLayout(groupBox_5);
        gridLayout_21->setObjectName(QString::fromUtf8("gridLayout_21"));
        labelInstallHeader = new QLabel(groupBox_5);
        labelInstallHeader->setObjectName(QString::fromUtf8("labelInstallHeader"));
        labelInstallHeader->setFrameShape(QFrame::NoFrame);
        labelInstallHeader->setFrameShadow(QFrame::Plain);
        labelInstallHeader->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        labelInstallHeader->setWordWrap(true);

        gridLayout_21->addWidget(labelInstallHeader, 0, 0, 1, 1);

        verticalSpacer_19 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_21->addItem(verticalSpacer_19, 1, 0, 1, 1);

        labelInstallStatus = new QLabel(groupBox_5);
        labelInstallStatus->setObjectName(QString::fromUtf8("labelInstallStatus"));

        gridLayout_21->addWidget(labelInstallStatus, 2, 0, 1, 1);

        progressBarInstall = new QProgressBar(groupBox_5);
        progressBarInstall->setObjectName(QString::fromUtf8("progressBarInstall"));
        progressBarInstall->setValue(24);

        gridLayout_21->addWidget(progressBarInstall, 3, 0, 1, 1);

        progressBarInstall2 = new QProgressBar(groupBox_5);
        progressBarInstall2->setObjectName(QString::fromUtf8("progressBarInstall2"));
        progressBarInstall2->setValue(24);

        gridLayout_21->addWidget(progressBarInstall2, 6, 0, 1, 1);

        labelInstallStatus2 = new QLabel(groupBox_5);
        labelInstallStatus2->setObjectName(QString::fromUtf8("labelInstallStatus2"));

        gridLayout_21->addWidget(labelInstallStatus2, 5, 0, 1, 1);

        verticalSpacer_32 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_21->addItem(verticalSpacer_32, 4, 0, 1, 1);


        gridLayout_8->addWidget(groupBox_5, 1, 1, 1, 1);

        horizontalSpacer_15 = new QSpacerItem(50, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout_8->addItem(horizontalSpacer_15, 1, 2, 1, 1);

        verticalSpacer_8 = new QSpacerItem(20, 29, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_8->addItem(verticalSpacer_8, 2, 1, 1, 1);

        installStackWidget->addWidget(page_4);
        page_5 = new QWidget();
        page_5->setObjectName(QString::fromUtf8("page_5"));
        gridLayout_9 = new QGridLayout(page_5);
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        verticalSpacer_10 = new QSpacerItem(20, 178, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_9->addItem(verticalSpacer_10, 0, 1, 1, 1);

        horizontalSpacer_16 = new QSpacerItem(194, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_9->addItem(horizontalSpacer_16, 1, 0, 1, 1);

        label_3 = new QLabel(page_5);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setStyleSheet(QString::fromUtf8("background: transparent;"));
        label_3->setAlignment(Qt::AlignCenter);
        label_3->setWordWrap(true);

        gridLayout_9->addWidget(label_3, 1, 1, 1, 1);

        horizontalSpacer_17 = new QSpacerItem(193, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_9->addItem(horizontalSpacer_17, 1, 2, 1, 1);

        verticalSpacer_9 = new QSpacerItem(20, 177, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_9->addItem(verticalSpacer_9, 2, 1, 1, 1);

        installStackWidget->addWidget(page_5);

        gridLayout->addWidget(installStackWidget, 0, 0, 1, 1);

        footerWidget = new QWidget(centralwidget);
        footerWidget->setObjectName(QString::fromUtf8("footerWidget"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(footerWidget->sizePolicy().hasHeightForWidth());
        footerWidget->setSizePolicy(sizePolicy3);
        footerWidget->setAutoFillBackground(false);
        footerWidget->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:1, y1:1, x2:1, y2:0, stop:0 rgba(0, 70, 175, 255), stop:1 rgba(255, 240, 240, 240));"));
        horizontalLayout_2 = new QHBoxLayout(footerWidget);
        horizontalLayout_2->setSpacing(10);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(5, 10, 10, 5);
        pushHardware = new QPushButton(footerWidget);
        pushHardware->setObjectName(QString::fromUtf8("pushHardware"));
        pushHardware->setMinimumSize(QSize(32, 32));
        pushHardware->setMaximumSize(QSize(32, 32));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/modules/images/systemsettings.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushHardware->setIcon(icon3);
        pushHardware->setIconSize(QSize(32, 32));
        pushHardware->setFlat(true);

        horizontalLayout_2->addWidget(pushHardware);

        helpButton = new QPushButton(footerWidget);
        helpButton->setObjectName(QString::fromUtf8("helpButton"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/modules/images/help-hint.png"), QSize(), QIcon::Normal, QIcon::Off);
        helpButton->setIcon(icon4);
        helpButton->setIconSize(QSize(32, 32));
        helpButton->setFlat(true);

        horizontalLayout_2->addWidget(helpButton);

        pushTouchKeyboard = new QPushButton(footerWidget);
        pushTouchKeyboard->setObjectName(QString::fromUtf8("pushTouchKeyboard"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/PCBSD/images/input-keyboard.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushTouchKeyboard->setIcon(icon5);
        pushTouchKeyboard->setIconSize(QSize(32, 32));
        pushTouchKeyboard->setFlat(true);

        horizontalLayout_2->addWidget(pushTouchKeyboard);

        pushChangeKeyLayout = new QPushButton(footerWidget);
        pushChangeKeyLayout->setObjectName(QString::fromUtf8("pushChangeKeyLayout"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/modules/images/keyboard.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushChangeKeyLayout->setIcon(icon6);
        pushChangeKeyLayout->setIconSize(QSize(32, 32));
        pushChangeKeyLayout->setAutoDefault(false);
        pushChangeKeyLayout->setDefault(false);
        pushChangeKeyLayout->setFlat(true);

        horizontalLayout_2->addWidget(pushChangeKeyLayout);

        horizontalSpacer = new QSpacerItem(532, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        abortButton = new QPushButton(footerWidget);
        abortButton->setObjectName(QString::fromUtf8("abortButton"));
        abortButton->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        abortButton->setIconSize(QSize(20, 20));

        horizontalLayout_2->addWidget(abortButton);

        backButton = new QPushButton(footerWidget);
        backButton->setObjectName(QString::fromUtf8("backButton"));
        backButton->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        backButton->setIconSize(QSize(20, 20));

        horizontalLayout_2->addWidget(backButton);

        nextButton = new QPushButton(footerWidget);
        nextButton->setObjectName(QString::fromUtf8("nextButton"));
        nextButton->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        nextButton->setIconSize(QSize(20, 20));

        horizontalLayout_2->addWidget(nextButton);


        gridLayout->addWidget(footerWidget, 1, 0, 1, 1);

        Installer->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(Installer);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        statusbar->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 70, 175);\n"
"font: 75 9pt \"Sans Serif\";\n"
"color: rgb(0, 0, 0);"));
        Installer->setStatusBar(statusbar);

        retranslateUi(Installer);

        installStackWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(Installer);
    } // setupUi

    void retranslateUi(QMainWindow *Installer)
    {
        label->setText(QString());
#ifndef QT_NO_STATUSTIP
        comboLanguage->setStatusTip(QApplication::translate("Installer", "Change the installation language", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        groupBox->setTitle(QString());
        label_2->setText(QApplication::translate("Installer", "Welcome to PC-BSD! Please select your language and click next to continue.", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        pushDeskLeft->setStatusTip(QApplication::translate("Installer", "Cycle through system choices", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        pushDeskLeft->setText(QString());
#ifndef QT_NO_STATUSTIP
        pushDeskRight->setStatusTip(QApplication::translate("Installer", "Cycle through system choices", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        pushDeskRight->setText(QString());
#ifndef QT_NO_STATUSTIP
        pushDeskCustomize->setStatusTip(QApplication::translate("Installer", "Customize the selected system", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        pushDeskCustomize->setText(QApplication::translate("Installer", "&Customize", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("Installer", "System Selection", 0, QApplication::UnicodeUTF8));
        groupDeskSummary->setTitle(QApplication::translate("Installer", "Summary", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("Installer", "Click arrows to change system selection", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("Installer", "Disk Selection", 0, QApplication::UnicodeUTF8));
        label_6->setText(QString());
#ifndef QT_NO_STATUSTIP
        pushDiskCustomize->setStatusTip(QApplication::translate("Installer", "Customize the disk selection or layout", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        pushDiskCustomize->setText(QApplication::translate("Installer", "&Customize", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("Installer", "Summary", 0, QApplication::UnicodeUTF8));
        groupBox_5->setTitle(QApplication::translate("Installer", "Installation", 0, QApplication::UnicodeUTF8));
        labelInstallHeader->setText(QApplication::translate("Installer", "Your system is now being installed! This process may take a while, depending upon the installation options chosen, and system speed. You will be notified when the installation is finished.", 0, QApplication::UnicodeUTF8));
        labelInstallStatus->setText(QString());
        labelInstallStatus2->setText(QString());
        label_3->setText(QApplication::translate("Installer", "Your system is now installed!\n"
" Click Finish to reboot. After rebooting you may eject the install media.", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        pushHardware->setToolTip(QString());
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        pushHardware->setStatusTip(QApplication::translate("Installer", "Display hardware compatibility diagnostic", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        pushHardware->setText(QString());
#ifndef QT_NO_STATUSTIP
        helpButton->setStatusTip(QApplication::translate("Installer", "Display help for the current screen", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        helpButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        pushTouchKeyboard->setToolTip(QString());
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        pushTouchKeyboard->setStatusTip(QApplication::translate("Installer", "Launch the on-screen keyboard", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        pushTouchKeyboard->setText(QString());
#ifndef QT_NO_TOOLTIP
        pushChangeKeyLayout->setToolTip(QString());
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        pushChangeKeyLayout->setStatusTip(QApplication::translate("Installer", "Switch between US layout and user selected layout", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        pushChangeKeyLayout->setText(QString());
#ifndef QT_NO_STATUSTIP
        abortButton->setStatusTip(QApplication::translate("Installer", "Cancel the installation", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        abortButton->setText(QApplication::translate("Installer", "Ab&ort", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        backButton->setStatusTip(QString());
#endif // QT_NO_STATUSTIP
        backButton->setText(QApplication::translate("Installer", "&Back", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        nextButton->setStatusTip(QString());
#endif // QT_NO_STATUSTIP
        nextButton->setText(QApplication::translate("Installer", "&Next", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(Installer);
    } // retranslateUi

};

namespace Ui {
    class Installer: public Ui_Installer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INSTALLER_H
