#include "dialogfscreate.h"
#include "ui_dialogfscreate.h"
#include <QLineEdit>

DialogfsCreate::DialogfsCreate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogfsCreate)
{
    ui->setupUi(this);


    ui->fsName->setFocus();

}

DialogfsCreate::~DialogfsCreate()
{
    delete ui;
}

void DialogfsCreate::setRootPath(QString path,QString text)
{
    this->fsRoot=path;
    if(text.isEmpty()) ui->fsName->setText(fsRoot);
    else ui->fsName->setText(text);
    checkValidPath();

}


void DialogfsCreate::on_fsName_textEdited(const QString &arg1)
{

    if(arg1.startsWith(fsRoot)) { checkValidPath(); return; }
    else {
        if(arg1==fsRoot.left(fsRoot.size()-1)) ui->fsName->setText(fsRoot);
        else ui->fsName->setText(fsRoot+arg1);
        checkValidPath();
    }
return;
}

void DialogfsCreate::on_checkMountpt_stateChanged(int arg1)
{
    if(arg1) {
        ui->comboMountpt->setEnabled(true);
        if(ui->comboMountpt->currentText()==tr("[path]")) ui->mountPtEdit->setEnabled(true);
        else ui->mountPtEdit->setEnabled(false);
    }
    else {
        ui->comboMountpt->setEnabled(false);
        ui->mountPtEdit->setEnabled(false);
    }
}

void DialogfsCreate::on_comboMountpt_currentIndexChanged(int index)
{
    Q_UNUSED(index);

    if(ui->comboMountpt->currentText()==tr("[path]")) ui->mountPtEdit->setEnabled(true);
    else ui->mountPtEdit->setEnabled(false);

}

void DialogfsCreate::on_checkNormalization_stateChanged(int arg1)
{
    ui->comboNormalization->setEnabled(arg1);
}

void DialogfsCreate::on_checkCopies_stateChanged(int arg1)
{
    ui->spinCopies->setEnabled(arg1);
}


void DialogfsCreate::on_checkCompression_stateChanged(int arg1)
{
    ui->comboCompression->setEnabled(arg1);
}


QString DialogfsCreate::getPath()
{
    return ui->fsName->text();
}

QStringList DialogfsCreate::getOptions()
{
    QStringList opt;

    if(ui->checkCanMount->checkState()==Qt::Checked) opt.append("canmount=noauto");
    if(ui->checkMountpt->checkState()==Qt::Checked) {
        QString tmp;
        tmp="mountpoint=";
        if(ui->comboMountpt->currentText()=="[path]") tmp+="\""+ui->mountPtEdit->text()+"\"";
        else tmp+=ui->comboMountpt->currentText();
        opt.append(tmp);
    }
    if(ui->checkUtf8Only->checkState()==Qt::Checked) opt.append("utf8only=on");

    if(ui->checkNormalization->checkState()==Qt::Checked) {
        opt.append("normalization="+ui->comboNormalization->currentText());
    }


    if(ui->checkDedup->checkState()==Qt::Checked) opt.append("dedup=on");

    if(ui->checkCompression->checkState()==Qt::Checked) opt.append("compression="+ui->comboCompression->currentText());


    return opt;
}

void DialogfsCreate::checkValidPath()
{
    isValid=true;
    QString str=ui->fsName->text();

    if(str==fsRoot) isValid=false;
    if(str.size()<=fsRoot.size()) isValid=false;
    if(str.size()>255) isValid=false;

    int f;

    for(f=fsRoot.size();f<str.size();++f)
    {
        // CHECK FOR VALID CHARACTERS (EXCLUDING THE GIVEN ROOT
        if((str.at(f)>=QChar('0')) && (str.at(f)<=QChar('9'))) continue;
        if((str.at(f)>=QChar('a')) && (str.at(f)<=QChar('z'))) continue;
        if((str.at(f)>=QChar('A')) && (str.at(f)<=QChar('Z'))) continue;
        if(str.at(f)==QChar(' ')) continue;
        if(str.at(f)==QChar('-')) continue;
        if(str.at(f)==QChar('_')) continue;
        if(str.at(f)==QChar(':')) continue;
        // '@' SHOULD BE INCLUDED IN THE ROOT, SO IT'S NOT ALLOWED HERE
        // '/' IS NOT ALLOWED
        isValid=false;
        break;
    }

    if(isValid) ui->fsName->setStyleSheet("");
    else ui->fsName->setStyleSheet("background-color: rgb(255, 188, 183);");
}

bool DialogfsCreate::hasValidPath()
{
    return isValid;
}

void DialogfsCreate::done(int result)
{
    if(result==QDialog::Accepted && !isValid) return;
        QDialog::done(result);
}

void DialogfsCreate::setPropsVisible(bool p)
{
    Q_UNUSED(p);
    ui->frameProps->setVisible(false);
}


void DialogfsCreate::nameOnlyMode()
{
    setPropsVisible(false);
    resize(width(),height()-ui->frameProps->height());
    adjustSize();
}

void DialogfsCreate::changeTitle(QString title)
{
    setWindowTitle(title);
}
