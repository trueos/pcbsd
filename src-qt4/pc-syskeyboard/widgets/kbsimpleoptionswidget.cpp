#include "kbsimpleoptionswidget.h"
#include "ui_kbsimpleoptionswidget.h"

using namespace pcbsd::keyboard;

KbSimpleOptionsWidget::KbSimpleOptionsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KbSimpleOptionsWidget)
{
    ui->setupUi(this);
    setupUI(currentSettings());
}

KbSimpleOptionsWidget::~KbSimpleOptionsWidget()
{
    delete ui;
}

void KbSimpleOptionsWidget::setSettings(KeyboardSettings ks)
{
    setupUI(ks);
}

void KbSimpleOptionsWidget::mergeSettings(KeyboardSettings &ks)
{
    ks.removeOption(OPTION_SWITCH_ALT_SHIFT);
    ks.removeOption(OPTION_SWITCH_CTRL_SHIFT);
    ks.removeOption(OPTION_SWITCH_ALT_SPACE);
    ks.removeOption(OPTION_ALLOW_TERMINATE_X);

    if ( ui->klsAltShift->isChecked())
    {
        ks.addOption(OPTION_SWITCH_ALT_SHIFT);
    }
    else if(ui->klsCtrlShift->isChecked())
    {
        ks.addOption(OPTION_SWITCH_CTRL_SHIFT);
    }
    else if(ui->klsAltSpace->isChecked())
    {
        ks.addOption(OPTION_SWITCH_ALT_SPACE);
    }

    if(ui->ktermXorg->isChecked())
    {
        ks.addOption(OPTION_ALLOW_TERMINATE_X);
    }
}

void KbSimpleOptionsWidget::setupUI(KeyboardSettings ks)
{    
    bool isAltShift = ks.hasOption(OPTION_SWITCH_ALT_SHIFT);
    bool isCtrlShift =ks.hasOption(OPTION_SWITCH_CTRL_SHIFT);
    bool isAltSpace = ks.hasOption(OPTION_SWITCH_ALT_SPACE);
    bool isTerminate =ks.hasOption(OPTION_ALLOW_TERMINATE_X);

    if (isAltShift || isCtrlShift || isAltSpace)
    {
        ui->klsAltShift->setChecked(isAltShift);
        ui->klsCtrlShift->setChecked(isCtrlShift);
        ui->klsAltSpace->setChecked(isAltSpace);
    }
    ui->ktermXorg->setChecked(isTerminate);
}
