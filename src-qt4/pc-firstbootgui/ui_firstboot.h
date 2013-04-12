/********************************************************************************
** Form generated from reading UI file 'firstboot.ui'
**
** Created: Fri Apr 5 17:27:08 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FIRSTBOOT_H
#define UI_FIRSTBOOT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QStackedWidget>
#include <QtGui/QStatusBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Installer
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QStackedWidget *installStackWidget;
    QWidget *Page_Welcome;
    QGridLayout *gridLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *horizontalSpacer_3;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QComboBox *comboLanguage;
    QLabel *label_2;
    QSpacerItem *verticalSpacer;
    QWidget *Page_Time;
    QGridLayout *gridLayout_4;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *verticalSpacer_4;
    QSpacerItem *horizontalSpacer_5;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_6;
    QLabel *label_7;
    QSpacerItem *horizontalSpacer_7;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_4;
    QComboBox *comboBoxTimezone;
    QLabel *label_5;
    QSpacerItem *verticalSpacer_11;
    QSpacerItem *verticalSpacer_3;
    QWidget *Page_Root;
    QGridLayout *gridLayout_5;
    QSpacerItem *horizontalSpacer_12;
    QSpacerItem *verticalSpacer_5;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_10;
    QLabel *label_9;
    QSpacerItem *horizontalSpacer_11;
    QLabel *label_10;
    QGridLayout *gridLayout_3;
    QLineEdit *lineRootPW;
    QLabel *label_6;
    QSpacerItem *horizontalSpacer_8;
    QLineEdit *lineRootPW2;
    QLabel *label_8;
    QSpacerItem *horizontalSpacer_9;
    QLabel *label_11;
    QSpacerItem *verticalSpacer_12;
    QSpacerItem *horizontalSpacer_13;
    QSpacerItem *verticalSpacer_6;
    QWidget *Page_User;
    QGridLayout *gridLayout_7;
    QSpacerItem *horizontalSpacer_14;
    QSpacerItem *verticalSpacer_7;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_18;
    QLabel *label_17;
    QSpacerItem *horizontalSpacer_19;
    QLabel *label_16;
    QGridLayout *gridLayout_6;
    QLineEdit *lineName;
    QLabel *label_12;
    QLineEdit *lineUsername;
    QLabel *label_13;
    QLineEdit *linePW;
    QLabel *label_14;
    QLineEdit *linePW2;
    QLabel *label_15;
    QLabel *label_18;
    QSpacerItem *verticalSpacer_8;
    QSpacerItem *horizontalSpacer_15;
    QSpacerItem *verticalSpacer_13;
    QWidget *Page_Wifi;
    QGridLayout *gridLayout_8;
    QSpacerItem *verticalSpacer_14;
    QSpacerItem *horizontalSpacer_24;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_20;
    QLabel *label_19;
    QSpacerItem *horizontalSpacer_21;
    QLabel *label_20;
    QListWidget *listWidgetWifi;
    QHBoxLayout *horizontalLayout_6;
    QSpacerItem *horizontalSpacer_22;
    QPushButton *pushButtonRescan;
    QLabel *label_21;
    QSpacerItem *verticalSpacer_15;
    QSpacerItem *horizontalSpacer_23;
    QSpacerItem *verticalSpacer_16;
    QWidget *Page_Finished;
    QGridLayout *gridLayout_9;
    QSpacerItem *verticalSpacer_10;
    QLabel *label_22;
    QSpacerItem *horizontalSpacer_16;
    QLabel *label_3;
    QSpacerItem *horizontalSpacer_17;
    QSpacerItem *verticalSpacer_9;
    QWidget *footerWidget;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *helpButton;
    QPushButton *pushTouchKeyboard;
    QPushButton *pushChangeKeyLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *backButton;
    QPushButton *nextButton;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *Installer)
    {
        if (Installer->objectName().isEmpty())
            Installer->setObjectName(QString::fromUtf8("Installer"));
        Installer->resize(619, 455);
        Installer->setWindowTitle(QString::fromUtf8("PC-BSD"));
        centralwidget = new QWidget(Installer);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setSpacing(0);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        installStackWidget = new QStackedWidget(centralwidget);
        installStackWidget->setObjectName(QString::fromUtf8("installStackWidget"));
        installStackWidget->setAutoFillBackground(false);
        installStackWidget->setStyleSheet(QString::fromUtf8("background-image: url(:/modules/images/backgroundimage.jpg);"));
        Page_Welcome = new QWidget();
        Page_Welcome->setObjectName(QString::fromUtf8("Page_Welcome"));
        gridLayout_2 = new QGridLayout(Page_Welcome);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        horizontalSpacer_2 = new QSpacerItem(170, 308, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_2, 0, 0, 3, 1);

        verticalSpacer_2 = new QSpacerItem(18, 50, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_2->addItem(verticalSpacer_2, 0, 1, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(169, 278, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_3, 0, 3, 3, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(Page_Welcome);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAutoFillBackground(false);
        label->setStyleSheet(QString::fromUtf8("border: transparent;\n"
"background: transparent;"));
        label->setPixmap(QPixmap(QString::fromUtf8(":/PCBSD/images/pcbsd-banner.png")));
        label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label);

        comboLanguage = new QComboBox(Page_Welcome);
        comboLanguage->setObjectName(QString::fromUtf8("comboLanguage"));

        verticalLayout->addWidget(comboLanguage);

        label_2 = new QLabel(Page_Welcome);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMinimumSize(QSize(0, 75));
        label_2->setAutoFillBackground(false);
        label_2->setStyleSheet(QString::fromUtf8("background: transparent;"));
        label_2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        label_2->setWordWrap(true);

        verticalLayout->addWidget(label_2);


        gridLayout_2->addLayout(verticalLayout, 1, 1, 1, 2);

        verticalSpacer = new QSpacerItem(20, 92, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer, 2, 2, 1, 1);

        installStackWidget->addWidget(Page_Welcome);
        Page_Time = new QWidget();
        Page_Time->setObjectName(QString::fromUtf8("Page_Time"));
        gridLayout_4 = new QGridLayout(Page_Time);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        horizontalSpacer_4 = new QSpacerItem(115, 278, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_4, 0, 0, 4, 1);

        verticalSpacer_4 = new QSpacerItem(20, 50, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_4->addItem(verticalSpacer_4, 0, 1, 1, 1);

        horizontalSpacer_5 = new QSpacerItem(115, 298, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_5, 0, 2, 3, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_6);

        label_7 = new QLabel(Page_Time);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_7->sizePolicy().hasHeightForWidth());
        label_7->setSizePolicy(sizePolicy);
        label_7->setMinimumSize(QSize(55, 55));
        label_7->setMaximumSize(QSize(55, 55));
        label_7->setStyleSheet(QString::fromUtf8("border: transparent;\n"
"background: transparent;"));
        label_7->setPixmap(QPixmap(QString::fromUtf8(":/modules/images/date.png")));
        label_7->setScaledContents(true);

        horizontalLayout->addWidget(label_7);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_7);


        gridLayout_4->addLayout(horizontalLayout, 1, 1, 1, 1);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        label_4 = new QLabel(Page_Time);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label_4->setFont(font);
        label_4->setMouseTracking(false);
        label_4->setStyleSheet(QString::fromUtf8("background: transparent;"));
        label_4->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(label_4);

        comboBoxTimezone = new QComboBox(Page_Time);
        comboBoxTimezone->setObjectName(QString::fromUtf8("comboBoxTimezone"));
        comboBoxTimezone->setMinimumSize(QSize(300, 0));

        verticalLayout_3->addWidget(comboBoxTimezone);

        label_5 = new QLabel(Page_Time);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setStyleSheet(QString::fromUtf8("background: transparent;"));
        label_5->setWordWrap(true);

        verticalLayout_3->addWidget(label_5);

        verticalSpacer_11 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_11);


        gridLayout_4->addLayout(verticalLayout_3, 2, 1, 1, 1);

        verticalSpacer_3 = new QSpacerItem(248, 18, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);

        gridLayout_4->addItem(verticalSpacer_3, 3, 1, 1, 1);

        installStackWidget->addWidget(Page_Time);
        Page_Root = new QWidget();
        Page_Root->setObjectName(QString::fromUtf8("Page_Root"));
        gridLayout_5 = new QGridLayout(Page_Root);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        horizontalSpacer_12 = new QSpacerItem(150, 341, QSizePolicy::Preferred, QSizePolicy::Minimum);

        gridLayout_5->addItem(horizontalSpacer_12, 0, 0, 3, 1);

        verticalSpacer_5 = new QSpacerItem(20, 50, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_5->addItem(verticalSpacer_5, 0, 1, 1, 1);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_10);

        label_9 = new QLabel(Page_Root);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        sizePolicy.setHeightForWidth(label_9->sizePolicy().hasHeightForWidth());
        label_9->setSizePolicy(sizePolicy);
        label_9->setMinimumSize(QSize(55, 55));
        label_9->setMaximumSize(QSize(55, 55));
        label_9->setStyleSheet(QString::fromUtf8("border: transparent;\n"
"background: transparent;"));
        label_9->setPixmap(QPixmap(QString::fromUtf8(":/modules/images/encrypted.png")));
        label_9->setScaledContents(true);

        horizontalLayout_3->addWidget(label_9);

        horizontalSpacer_11 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_11);


        verticalLayout_2->addLayout(horizontalLayout_3);

        label_10 = new QLabel(Page_Root);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setFont(font);
        label_10->setStyleSheet(QString::fromUtf8("background: transparent;"));

        verticalLayout_2->addWidget(label_10);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        lineRootPW = new QLineEdit(Page_Root);
        lineRootPW->setObjectName(QString::fromUtf8("lineRootPW"));
        lineRootPW->setEchoMode(QLineEdit::Password);

        gridLayout_3->addWidget(lineRootPW, 0, 0, 1, 1);

        label_6 = new QLabel(Page_Root);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setStyleSheet(QString::fromUtf8("background: transparent;"));

        gridLayout_3->addWidget(label_6, 0, 1, 1, 1);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_8, 0, 2, 1, 1);

        lineRootPW2 = new QLineEdit(Page_Root);
        lineRootPW2->setObjectName(QString::fromUtf8("lineRootPW2"));
        lineRootPW2->setEchoMode(QLineEdit::Password);

        gridLayout_3->addWidget(lineRootPW2, 1, 0, 1, 1);

        label_8 = new QLabel(Page_Root);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setStyleSheet(QString::fromUtf8("background: transparent;"));

        gridLayout_3->addWidget(label_8, 1, 1, 1, 2);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_9, 1, 3, 1, 1);


        verticalLayout_2->addLayout(gridLayout_3);

        label_11 = new QLabel(Page_Root);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setStyleSheet(QString::fromUtf8("background: transparent;"));
        label_11->setWordWrap(true);

        verticalLayout_2->addWidget(label_11);

        verticalSpacer_12 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_12);


        gridLayout_5->addLayout(verticalLayout_2, 1, 1, 1, 1);

        horizontalSpacer_13 = new QSpacerItem(150, 278, QSizePolicy::Preferred, QSizePolicy::Minimum);

        gridLayout_5->addItem(horizontalSpacer_13, 1, 2, 2, 1);

        verticalSpacer_6 = new QSpacerItem(20, 90, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_5->addItem(verticalSpacer_6, 2, 1, 1, 1);

        installStackWidget->addWidget(Page_Root);
        Page_User = new QWidget();
        Page_User->setObjectName(QString::fromUtf8("Page_User"));
        gridLayout_7 = new QGridLayout(Page_User);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        horizontalSpacer_14 = new QSpacerItem(150, 318, QSizePolicy::Preferred, QSizePolicy::Minimum);

        gridLayout_7->addItem(horizontalSpacer_14, 0, 0, 2, 1);

        verticalSpacer_7 = new QSpacerItem(20, 50, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_7->addItem(verticalSpacer_7, 0, 1, 1, 1);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalSpacer_18 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_18);

        label_17 = new QLabel(Page_User);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        sizePolicy.setHeightForWidth(label_17->sizePolicy().hasHeightForWidth());
        label_17->setSizePolicy(sizePolicy);
        label_17->setMinimumSize(QSize(55, 55));
        label_17->setMaximumSize(QSize(55, 55));
        label_17->setStyleSheet(QString::fromUtf8("border: transparent;\n"
"background: transparent;"));
        label_17->setPixmap(QPixmap(QString::fromUtf8(":/modules/images/add_user.png")));
        label_17->setScaledContents(true);

        horizontalLayout_4->addWidget(label_17);

        horizontalSpacer_19 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_19);


        verticalLayout_4->addLayout(horizontalLayout_4);

        label_16 = new QLabel(Page_User);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setFont(font);
        label_16->setStyleSheet(QString::fromUtf8("background: transparent;"));

        verticalLayout_4->addWidget(label_16);

        gridLayout_6 = new QGridLayout();
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        lineName = new QLineEdit(Page_User);
        lineName->setObjectName(QString::fromUtf8("lineName"));
        lineName->setStyleSheet(QString::fromUtf8(""));

        gridLayout_6->addWidget(lineName, 0, 0, 1, 1);

        label_12 = new QLabel(Page_User);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setStyleSheet(QString::fromUtf8("background: transparent;"));

        gridLayout_6->addWidget(label_12, 0, 1, 1, 1);

        lineUsername = new QLineEdit(Page_User);
        lineUsername->setObjectName(QString::fromUtf8("lineUsername"));

        gridLayout_6->addWidget(lineUsername, 1, 0, 1, 1);

        label_13 = new QLabel(Page_User);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setStyleSheet(QString::fromUtf8("background: transparent;"));

        gridLayout_6->addWidget(label_13, 1, 1, 1, 1);

        linePW = new QLineEdit(Page_User);
        linePW->setObjectName(QString::fromUtf8("linePW"));
        linePW->setEchoMode(QLineEdit::Password);

        gridLayout_6->addWidget(linePW, 2, 0, 1, 1);

        label_14 = new QLabel(Page_User);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setStyleSheet(QString::fromUtf8("background: transparent;"));

        gridLayout_6->addWidget(label_14, 2, 1, 1, 1);

        linePW2 = new QLineEdit(Page_User);
        linePW2->setObjectName(QString::fromUtf8("linePW2"));
        linePW2->setEchoMode(QLineEdit::Password);

        gridLayout_6->addWidget(linePW2, 3, 0, 1, 1);

        label_15 = new QLabel(Page_User);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setStyleSheet(QString::fromUtf8("background: transparent;"));

        gridLayout_6->addWidget(label_15, 3, 1, 1, 1);


        verticalLayout_4->addLayout(gridLayout_6);

        label_18 = new QLabel(Page_User);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setStyleSheet(QString::fromUtf8("background: transparent;"));
        label_18->setWordWrap(true);

        verticalLayout_4->addWidget(label_18);

        verticalSpacer_8 = new QSpacerItem(20, 29, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_8);


        gridLayout_7->addLayout(verticalLayout_4, 1, 1, 1, 1);

        horizontalSpacer_15 = new QSpacerItem(150, 20, QSizePolicy::Preferred, QSizePolicy::Minimum);

        gridLayout_7->addItem(horizontalSpacer_15, 1, 2, 1, 1);

        verticalSpacer_13 = new QSpacerItem(20, 52, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_7->addItem(verticalSpacer_13, 2, 1, 1, 1);

        installStackWidget->addWidget(Page_User);
        Page_Wifi = new QWidget();
        Page_Wifi->setObjectName(QString::fromUtf8("Page_Wifi"));
        gridLayout_8 = new QGridLayout(Page_Wifi);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        verticalSpacer_14 = new QSpacerItem(20, 50, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_8->addItem(verticalSpacer_14, 0, 1, 1, 1);

        horizontalSpacer_24 = new QSpacerItem(158, 288, QSizePolicy::Preferred, QSizePolicy::Minimum);

        gridLayout_8->addItem(horizontalSpacer_24, 1, 0, 2, 1);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalSpacer_20 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_20);

        label_19 = new QLabel(Page_Wifi);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        sizePolicy.setHeightForWidth(label_19->sizePolicy().hasHeightForWidth());
        label_19->setSizePolicy(sizePolicy);
        label_19->setMinimumSize(QSize(55, 55));
        label_19->setMaximumSize(QSize(55, 55));
        label_19->setStyleSheet(QString::fromUtf8("border: transparent;\n"
"background: transparent;"));
        label_19->setPixmap(QPixmap(QString::fromUtf8(":/modules/images/network.png")));
        label_19->setScaledContents(true);

        horizontalLayout_5->addWidget(label_19);

        horizontalSpacer_21 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_21);


        verticalLayout_5->addLayout(horizontalLayout_5);

        label_20 = new QLabel(Page_Wifi);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        label_20->setFont(font);
        label_20->setStyleSheet(QString::fromUtf8("background: transparent;"));

        verticalLayout_5->addWidget(label_20);

        listWidgetWifi = new QListWidget(Page_Wifi);
        listWidgetWifi->setObjectName(QString::fromUtf8("listWidgetWifi"));
        listWidgetWifi->setMaximumSize(QSize(16777215, 150));

        verticalLayout_5->addWidget(listWidgetWifi);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalSpacer_22 = new QSpacerItem(198, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_22);

        pushButtonRescan = new QPushButton(Page_Wifi);
        pushButtonRescan->setObjectName(QString::fromUtf8("pushButtonRescan"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/modules/images/view-refresh.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButtonRescan->setIcon(icon);

        horizontalLayout_6->addWidget(pushButtonRescan);


        verticalLayout_5->addLayout(horizontalLayout_6);

        label_21 = new QLabel(Page_Wifi);
        label_21->setObjectName(QString::fromUtf8("label_21"));
        label_21->setStyleSheet(QString::fromUtf8("background: transparent;"));
        label_21->setWordWrap(true);

        verticalLayout_5->addWidget(label_21);

        verticalSpacer_15 = new QSpacerItem(20, 13, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer_15);


        gridLayout_8->addLayout(verticalLayout_5, 1, 1, 1, 1);

        horizontalSpacer_23 = new QSpacerItem(158, 228, QSizePolicy::Preferred, QSizePolicy::Minimum);

        gridLayout_8->addItem(horizontalSpacer_23, 1, 2, 1, 1);

        verticalSpacer_16 = new QSpacerItem(20, 27, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_8->addItem(verticalSpacer_16, 2, 1, 1, 1);

        installStackWidget->addWidget(Page_Wifi);
        Page_Finished = new QWidget();
        Page_Finished->setObjectName(QString::fromUtf8("Page_Finished"));
        gridLayout_9 = new QGridLayout(Page_Finished);
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        verticalSpacer_10 = new QSpacerItem(20, 178, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_9->addItem(verticalSpacer_10, 0, 1, 1, 1);

        label_22 = new QLabel(Page_Finished);
        label_22->setObjectName(QString::fromUtf8("label_22"));
        label_22->setAutoFillBackground(false);
        label_22->setStyleSheet(QString::fromUtf8("border: transparent;\n"
"background: transparent;"));
        label_22->setPixmap(QPixmap(QString::fromUtf8(":/PCBSD/images/pcbsd-banner.png")));
        label_22->setAlignment(Qt::AlignCenter);

        gridLayout_9->addWidget(label_22, 1, 1, 1, 1);

        horizontalSpacer_16 = new QSpacerItem(194, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_9->addItem(horizontalSpacer_16, 2, 0, 1, 1);

        label_3 = new QLabel(Page_Finished);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setStyleSheet(QString::fromUtf8("background: transparent;"));
        label_3->setWordWrap(true);

        gridLayout_9->addWidget(label_3, 2, 1, 1, 1);

        horizontalSpacer_17 = new QSpacerItem(193, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_9->addItem(horizontalSpacer_17, 2, 2, 1, 1);

        verticalSpacer_9 = new QSpacerItem(20, 177, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_9->addItem(verticalSpacer_9, 3, 1, 1, 1);

        installStackWidget->addWidget(Page_Finished);

        gridLayout->addWidget(installStackWidget, 0, 0, 1, 1);

        footerWidget = new QWidget(centralwidget);
        footerWidget->setObjectName(QString::fromUtf8("footerWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(footerWidget->sizePolicy().hasHeightForWidth());
        footerWidget->setSizePolicy(sizePolicy1);
        footerWidget->setAutoFillBackground(false);
        footerWidget->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:1, y1:1, x2:1, y2:0, stop:0 rgba(0, 70, 175, 255), stop:1 rgba(255, 240, 240, 240));"));
        horizontalLayout_2 = new QHBoxLayout(footerWidget);
        horizontalLayout_2->setSpacing(10);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(5, 10, 10, 5);
        helpButton = new QPushButton(footerWidget);
        helpButton->setObjectName(QString::fromUtf8("helpButton"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/modules/images/help-hint.png"), QSize(), QIcon::Normal, QIcon::Off);
        helpButton->setIcon(icon1);
        helpButton->setIconSize(QSize(32, 32));
        helpButton->setFlat(true);

        horizontalLayout_2->addWidget(helpButton);

        pushTouchKeyboard = new QPushButton(footerWidget);
        pushTouchKeyboard->setObjectName(QString::fromUtf8("pushTouchKeyboard"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/PCBSD/images/input-keyboard.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushTouchKeyboard->setIcon(icon2);
        pushTouchKeyboard->setIconSize(QSize(32, 32));
        pushTouchKeyboard->setFlat(true);

        horizontalLayout_2->addWidget(pushTouchKeyboard);

        pushChangeKeyLayout = new QPushButton(footerWidget);
        pushChangeKeyLayout->setObjectName(QString::fromUtf8("pushChangeKeyLayout"));
        pushChangeKeyLayout->setAutoFillBackground(false);
        pushChangeKeyLayout->setStyleSheet(QString::fromUtf8(""));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/modules/images/keyboard.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushChangeKeyLayout->setIcon(icon3);
        pushChangeKeyLayout->setIconSize(QSize(32, 32));
        pushChangeKeyLayout->setAutoDefault(false);
        pushChangeKeyLayout->setDefault(false);
        pushChangeKeyLayout->setFlat(true);

        horizontalLayout_2->addWidget(pushChangeKeyLayout);

        horizontalSpacer = new QSpacerItem(532, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

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

        installStackWidget->setCurrentIndex(4);


        QMetaObject::connectSlotsByName(Installer);
    } // setupUi

    void retranslateUi(QMainWindow *Installer)
    {
        label->setText(QString());
#ifndef QT_NO_STATUSTIP
        comboLanguage->setStatusTip(QApplication::translate("Installer", "Change the installation language", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        label_2->setText(QApplication::translate("Installer", "Welcome to your new PC-BSD system! You may change your language above, and click next to begin the setup process.", 0, QApplication::UnicodeUTF8));
        label_7->setText(QString());
        label_4->setText(QApplication::translate("Installer", "System Timezone", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        comboBoxTimezone->setStatusTip(QApplication::translate("Installer", "Set the system timezone", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        label_5->setText(QApplication::translate("Installer", "Where are you located? Please select the timezone for your location.", 0, QApplication::UnicodeUTF8));
        label_9->setText(QString());
        label_10->setText(QApplication::translate("Installer", "Root Password", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("Installer", "Password", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("Installer", "Password (Repeat)", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("Installer", "The system or \"root\" password allows you to make changes to your operating system, and should be kept from non-trusted users. ", 0, QApplication::UnicodeUTF8));
        label_17->setText(QString());
        label_16->setText(QApplication::translate("Installer", "Create a User", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("Installer", "Name", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("Installer", "Username", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("Installer", "Password", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("Installer", "Password (Repeat)", 0, QApplication::UnicodeUTF8));
        label_18->setText(QApplication::translate("Installer", "Now you may create a desktop user for this system. After you login, additional users can easily be created in the Control Panel. ", 0, QApplication::UnicodeUTF8));
        label_19->setText(QString());
        label_20->setText(QApplication::translate("Installer", "Connect to a Wireless Network", 0, QApplication::UnicodeUTF8));
        pushButtonRescan->setText(QApplication::translate("Installer", "&Rescan", 0, QApplication::UnicodeUTF8));
        label_21->setText(QApplication::translate("Installer", "The following wireless networks were found. Click the one you wish to connect to. If you wish to skip, click \"Next\" below. ", 0, QApplication::UnicodeUTF8));
        label_22->setText(QString());
        label_3->setText(QApplication::translate("Installer", "Setup is now complete! Press Finish to login to your new desktop.", 0, QApplication::UnicodeUTF8));
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
        backButton->setText(QApplication::translate("Installer", "&Back", 0, QApplication::UnicodeUTF8));
        nextButton->setText(QApplication::translate("Installer", "&Next", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(Installer);
    } // retranslateUi

};

namespace Ui {
    class Installer: public Ui_Installer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FIRSTBOOT_H
