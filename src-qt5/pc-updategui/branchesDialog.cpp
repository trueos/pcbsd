#include "branchesDialog.h"
#include "ui_branchesDialog.h"

#include <pcbsd-utils.h>

const char* const GET_BRANCHES_CMD = "pc-updatemanager branches";

const char* const START_BRANCHES_TAG = "-----";
const char* const END_BRANCHES_TAG = "To change branches run:";
const char* const CURRENT_BRANCH_MARK = "*";

BranchesDialog::BranchesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BranchesDialog)
{
    ui->setupUi(this);
    //Ensure the dialog is centered on the parent
    if(parent!=0){
       QPoint ctr = parent->geometry().center();
       this->move( ctr.x()-(this->width()/2), ctr.y()-(this->height()/2) );
    }
}

BranchesDialog::~BranchesDialog()
{
    delete ui;
}

QString BranchesDialog::selectedBranch()
{
    QListWidgetItem* item = ui->branchesList->currentItem();
    if (!item)
        return QString();
    return item->text();
}

void BranchesDialog::setupDialog()
{
    this->setEnabled(false);
    //Run pc-updatemanager and parse output for branch names
    QStringList out = pcbsd::Utils::runShellCommand(GET_BRANCHES_CMD);
    bool is_branches = false;
    mBranches.clear();
    for(int i=0; i<out.size(); i++)
    {
        out[i] = out[i].trimmed();
        if (!out[i].length()) continue;
        if (out[i].startsWith(START_BRANCHES_TAG))
        {
            is_branches=true;
            continue;
        }
        if (out[i].startsWith(END_BRANCHES_TAG))
        {
            break;
        }
        if (is_branches)
        {
            QString name = out[i];
            //check current branch
            if (name.endsWith(CURRENT_BRANCH_MARK))
            {
                name = name.left(name.length()-1).trimmed();
                mCurrentBranch = name;
                continue;
            }
            mBranches.append(name);

        }
    }

    //Now fill list in UI
    ui->currentBranch->setText(mCurrentBranch);
    ui->branchesList->clear();
    ui->branchesList->addItems(mBranches);
    if (!mBranches.size())
    {
        ui->acceptBtn->setEnabled(false);
    }
    else
    {
        ui->branchesList->setCurrentRow(0);
    }

    this->setEnabled(true);
}
