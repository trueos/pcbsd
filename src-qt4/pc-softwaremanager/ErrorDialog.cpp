/***************************************************************************
 *   Copyright (C) 2011 - iXsystems				 	  *
 *   kris@pcbsd.org  *
 *   tim@pcbsd.org   *
 *   ken@pcbsd.org   *
 *                                                                         *
 *   Permission is hereby granted, free of charge, to any person obtaining *
 *   a copy of this software and associated documentation files (the       *
 *   "Software"), to deal in the Software without restriction, including   *
 *   without limitation the rights to use, copy, modify, merge, publish,   *
 *   distribute, sublicense, and/or sell copies of the Software, and to    *
 *   permit persons to whom the Software is furnished to do so, subject to *
 *   the following conditions:                                             *
 *                                                                         *
 *   The above copyright notice and this permission notice shall be        *
 *   included in all copies or substantial portions of the Software.       *
 *                                                                         *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       *
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    *
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. *
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR     *
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, *
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR *
 *   OTHER DEALINGS IN THE SOFTWARE.                                       *
 ***************************************************************************/
#include "ErrorDialog.h"
#include "ui_ErrorDialog.h"

ErrorDialog::ErrorDialog(QWidget* parent) : QDialog(parent), ui(new Ui::ErrorDialog){
  ui->setupUi(this);
  connect(ui->checkBox, SIGNAL(stateChanged(int)), this, SLOT(updateUI()) );
}

ErrorDialog::~ErrorDialog(){
  delete ui;
}

void ErrorDialog::setDLGTitle(QString title){
  this->setWindowTitle(title);
  updateUI();
}

void ErrorDialog::setDLGMessage(QString message){
  ui->label->setText(message);
  updateUI();
}

void ErrorDialog::setDLGLog(QStringList log){
  ui->textEdit->clear();
  ui->textEdit->setText(log.join("\n"));
  updateUI();
}


void ErrorDialog::updateUI(){
  if(ui->textEdit->toPlainText().isEmpty()){
    ui->checkBox->setVisible(FALSE);
    ui->textEdit->setVisible(FALSE);
  }else{
    ui->checkBox->setVisible(TRUE);
    if(ui->checkBox->isChecked()){ui->textEdit->setVisible(TRUE);}
    else{ui->textEdit->setVisible(FALSE);}
  }
}

void ErrorDialog::on_buttonBox_accepted(){
  this->close();  
}

void ErrorDialog::on_buttonBox_rejected(){
  this->close();  
}
