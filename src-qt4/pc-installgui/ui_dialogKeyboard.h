/********************************************************************************
** Form generated from reading UI file 'dialogKeyboard.ui'
**
** Created: Wed Mar 20 02:45:43 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGKEYBOARD_H
#define UI_DIALOGKEYBOARD_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_widgetKeyboard
{
public:
    QGridLayout *gridLayout;
    QGroupBox *groupKeyboard;
    QGridLayout *gridLayout_6;
    QLabel *label_4;
    QFrame *line;
    QLabel *label_6;
    QListWidget *listKbLayouts;
    QListWidget *listKbVariants;
    QComboBox *comboBoxKeyboardModel;
    QLabel *label_19;
    QFrame *frame;
    QVBoxLayout *verticalLayout_7;
    QLabel *label_5;
    QLineEdit *lineKbTest;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushApply;
    QPushButton *pushClose;

    void setupUi(QWidget *widgetKeyboard)
    {
        if (widgetKeyboard->objectName().isEmpty())
            widgetKeyboard->setObjectName(QString::fromUtf8("widgetKeyboard"));
        widgetKeyboard->resize(559, 440);
        widgetKeyboard->setWindowTitle(QString::fromUtf8("PC-BSD"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/PCBSD/images/input-keyboard.png"), QSize(), QIcon::Normal, QIcon::Off);
        widgetKeyboard->setWindowIcon(icon);
        gridLayout = new QGridLayout(widgetKeyboard);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        groupKeyboard = new QGroupBox(widgetKeyboard);
        groupKeyboard->setObjectName(QString::fromUtf8("groupKeyboard"));
        QFont font;
        font.setBold(false);
        font.setWeight(50);
        groupKeyboard->setFont(font);
        gridLayout_6 = new QGridLayout(groupKeyboard);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        label_4 = new QLabel(groupKeyboard);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy);
        label_4->setMinimumSize(QSize(0, 26));
        QFont font1;
        font1.setPointSize(10);
        font1.setBold(false);
        font1.setWeight(50);
        label_4->setFont(font1);
        label_4->setMargin(5);

        gridLayout_6->addWidget(label_4, 2, 0, 1, 1);

        line = new QFrame(groupKeyboard);
        line->setObjectName(QString::fromUtf8("line"));
        line->setMaximumSize(QSize(2, 16777215));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout_6->addWidget(line, 2, 1, 2, 1);

        label_6 = new QLabel(groupKeyboard);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        sizePolicy.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy);
        label_6->setMinimumSize(QSize(0, 26));
        label_6->setFont(font1);
        label_6->setMargin(5);

        gridLayout_6->addWidget(label_6, 2, 2, 1, 1);

        listKbLayouts = new QListWidget(groupKeyboard);
        listKbLayouts->setObjectName(QString::fromUtf8("listKbLayouts"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(listKbLayouts->sizePolicy().hasHeightForWidth());
        listKbLayouts->setSizePolicy(sizePolicy1);
        QFont font2;
        font2.setPointSize(9);
        listKbLayouts->setFont(font2);
        listKbLayouts->setFrameShape(QFrame::NoFrame);
        listKbLayouts->setSpacing(2);
        listKbLayouts->setUniformItemSizes(true);

        gridLayout_6->addWidget(listKbLayouts, 3, 0, 1, 1);

        listKbVariants = new QListWidget(groupKeyboard);
        listKbVariants->setObjectName(QString::fromUtf8("listKbVariants"));
        sizePolicy1.setHeightForWidth(listKbVariants->sizePolicy().hasHeightForWidth());
        listKbVariants->setSizePolicy(sizePolicy1);
        listKbVariants->setFont(font2);
        listKbVariants->setFrameShape(QFrame::NoFrame);
        listKbVariants->setSpacing(2);
        listKbVariants->setUniformItemSizes(true);

        gridLayout_6->addWidget(listKbVariants, 3, 2, 1, 1);

        comboBoxKeyboardModel = new QComboBox(groupKeyboard);
        comboBoxKeyboardModel->setObjectName(QString::fromUtf8("comboBoxKeyboardModel"));

        gridLayout_6->addWidget(comboBoxKeyboardModel, 1, 0, 1, 3);

        label_19 = new QLabel(groupKeyboard);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        sizePolicy.setHeightForWidth(label_19->sizePolicy().hasHeightForWidth());
        label_19->setSizePolicy(sizePolicy);
        label_19->setMinimumSize(QSize(0, 26));
        label_19->setFont(font1);
        label_19->setAlignment(Qt::AlignCenter);
        label_19->setMargin(5);

        gridLayout_6->addWidget(label_19, 0, 0, 1, 3);


        gridLayout->addWidget(groupKeyboard, 0, 0, 1, 1);

        frame = new QFrame(widgetKeyboard);
        frame->setObjectName(QString::fromUtf8("frame"));
        sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy);
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Sunken);
        verticalLayout_7 = new QVBoxLayout(frame);
        verticalLayout_7->setSpacing(2);
        verticalLayout_7->setContentsMargins(0, 0, 0, 0);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        label_5 = new QLabel(frame);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setMinimumSize(QSize(0, 25));
        QFont font3;
        font3.setPointSize(8);
        font3.setBold(true);
        font3.setWeight(75);
        label_5->setFont(font3);
        label_5->setAlignment(Qt::AlignCenter);
        label_5->setMargin(9);

        verticalLayout_7->addWidget(label_5);

        lineKbTest = new QLineEdit(frame);
        lineKbTest->setObjectName(QString::fromUtf8("lineKbTest"));
        lineKbTest->setInputMask(QString::fromUtf8(""));
        lineKbTest->setFrame(false);
        lineKbTest->setAlignment(Qt::AlignCenter);

        verticalLayout_7->addWidget(lineKbTest);


        gridLayout->addWidget(frame, 1, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(348, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushApply = new QPushButton(widgetKeyboard);
        pushApply->setObjectName(QString::fromUtf8("pushApply"));

        horizontalLayout->addWidget(pushApply);

        pushClose = new QPushButton(widgetKeyboard);
        pushClose->setObjectName(QString::fromUtf8("pushClose"));

        horizontalLayout->addWidget(pushClose);


        gridLayout->addLayout(horizontalLayout, 2, 0, 1, 1);


        retranslateUi(widgetKeyboard);

        QMetaObject::connectSlotsByName(widgetKeyboard);
    } // setupUi

    void retranslateUi(QWidget *widgetKeyboard)
    {
        groupKeyboard->setTitle(QApplication::translate("widgetKeyboard", "Keyboard Settings", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("widgetKeyboard", "Key Layout", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("widgetKeyboard", "Variant", 0, QApplication::UnicodeUTF8));
        label_19->setText(QApplication::translate("widgetKeyboard", "Keyboard Model", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("widgetKeyboard", "( you may type into the space below to test your selected settings. )", 0, QApplication::UnicodeUTF8));
        pushApply->setText(QApplication::translate("widgetKeyboard", "&Apply", 0, QApplication::UnicodeUTF8));
        pushClose->setText(QApplication::translate("widgetKeyboard", "&Close", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(widgetKeyboard);
    } // retranslateUi

};

namespace Ui {
    class widgetKeyboard: public Ui_widgetKeyboard {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGKEYBOARD_H
