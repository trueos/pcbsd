/*
 * Copyright (c) 2012, Guillermo A. Amaral B <g@maral.me>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL GUILLERMO ANTONIO AMARAL BASTIDAS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "passwordpromptdialog.h"
#include "ui_passwordpromptdialog.h"

PasswordPromptDialog::PasswordPromptDialog(QWidget *parent)
    : QDialog(parent)
    , m_ui(new Ui_PasswordPromptDialog)
{
	setupUi();
}

PasswordPromptDialog::~PasswordPromptDialog(void)
{
	delete m_ui;
}

void
PasswordPromptDialog::setupUi(void)
{
	m_ui->setupUi(this);
	m_ui->errorLabel->setVisible(false);
	adjustSize();
}

QString
PasswordPromptDialog::password(void) const
{
	return(m_ui->passwordLineEdit->text());
}

void
PasswordPromptDialog::setErrorMessage(const QString &msg)
{
	m_ui->errorLabel->setText(msg);
	m_ui->errorLabel->setVisible(true);
	adjustSize();
}

void
PasswordPromptDialog::setInformationMessage(const QString &msg)
{
	m_ui->infoLabel->setText(msg);
	adjustSize();
}

void
PasswordPromptDialog::setPasswordEcho(bool echo)
{
	m_ui->passwordLineEdit->setEchoMode(echo ? QLineEdit::PasswordEchoOnEdit : QLineEdit::Password);
}

void
PasswordPromptDialog::setPasswordPrompt(const QString &prompt)
{
	m_ui->promptLabel->setText(prompt);
	adjustSize();
}
void
PasswordPromptDialog::setUserCaption(const QString &caption)
{
	m_ui->userLabel->setText(caption);
}

void
PasswordPromptDialog::setUserName(const QString &name)
{
	m_ui->userLineEdit->setText(name);
	m_ui->userLineEdit->setEnabled(false);
}

