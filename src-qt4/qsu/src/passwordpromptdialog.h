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

#ifndef QSU_PASSWORDPROMPTDIALOG_H
#define QSU_PASSWORDPROMPTDIALOG_H 1

#include <QtGui/QDialog>

class Ui_PasswordPromptDialog;

class PasswordPromptDialog : public QDialog
{
	Ui_PasswordPromptDialog *m_ui;

	Q_OBJECT
public:
	PasswordPromptDialog(QWidget *parent = 0);
	virtual ~PasswordPromptDialog(void);

	QString password(void) const;

	void setErrorMessage(const QString &msg);
	void setInformationMessage(const QString &msg);
	void setPasswordEcho(bool echo);
	void setPasswordPrompt(const QString &prompt);
	void setUserCaption(const QString &caption);
	void setUserName(const QString &name);

protected:
	void setupUi(void);
};

#endif
