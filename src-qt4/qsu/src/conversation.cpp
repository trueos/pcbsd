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

#include "conversation.h"

#include <QtGui/QApplication>
#include <QtGui/QIcon>
#include <QtGui/QMessageBox>

#include <X11/Xlib.h>

#include <security/pam_appl.h>

#include "passwordpromptdialog.h"
#include "session.h"
#include "strings.h"

/***************************************************************** anonymous */

namespace
{
	QApplication *s_qt_app = 0;
}

extern "C" {

void
ui_initialize(void)
{
	s_qt_app = new QApplication(XOpenDisplay(getenv("DISPLAY")));
	s_qt_app->setWindowIcon(QIcon(":/icons/lock.png"));
}

void
ui_finalize(void)
{
	delete s_qt_app, s_qt_app = 0;
}

int
ui_conversation(int num_msg,
                 const struct pam_message **msg,
                 struct pam_response **resp,
                 void *appdata_ptr)
{
	struct pam_response *l_resp(0);
	int l_error_code(PAM_SUCCESS);

	qDebug("Qt conversation started.");

	/* sanity check */
	if (num_msg < 0 || num_msg > PAM_MAX_NUM_MSG) {
		qDebug("Invalid amount of messages received.");
		l_error_code = PAM_CONV_ERR;
	}

	/* allocate response messages */
	if (l_error_code == PAM_SUCCESS &&
	    (l_resp = reinterpret_cast<pam_response *>
	      (calloc(num_msg, sizeof(*l_resp)))) == NULL) {
		qDebug("Failed to allocate response messages.");
		l_error_code = PAM_BUF_ERR;
	}

	PasswordPromptDialog dialog;

	qsu_session *session = reinterpret_cast<qsu_session *>(appdata_ptr);
	dialog.setWindowTitle(QString(gs_default_title).arg(session->user));
	if (session->description)
		dialog.setInformationMessage(session->description);
	dialog.setUserCaption(gs_default_user_caption);
	dialog.setUserName(session->user);

	/* process messages */
	for (int i = 0;
	     l_error_code == PAM_SUCCESS && i < num_msg;
	     ++i)
	{
		l_resp[i].resp = NULL;
		l_resp[i].resp_retcode = 0;

		switch (msg[i]->msg_style) {
		case PAM_PROMPT_ECHO_OFF:
		case PAM_PROMPT_ECHO_ON:
			dialog.setPasswordPrompt(msg[i]->msg);
			dialog.setPasswordEcho(msg[i]->msg_style == PAM_PROMPT_ECHO_ON);

			if (dialog.exec() == QDialog::Accepted)
				l_resp[i].resp = strdup(dialog.password().toAscii().constData());
			else
				exit(1);

			break;
		case PAM_ERROR_MSG:
			dialog.setErrorMessage(msg[i]->msg);
			break;
		case PAM_TEXT_INFO:
			dialog.setInformationMessage(msg[i]->msg);
			break;
		default:
			qDebug("Got unknown message style. ABORT.");
			l_error_code = PAM_CONV_ERR;
			break;
		}
	}

	/* cleanup on error */
	if (l_error_code != PAM_SUCCESS) {
		qDebug("Qt conversation error cleanup.");

		for (int i = 0; i < num_msg; ++i)
			free(l_resp[i].resp);
		free(l_resp);

		*resp = NULL;
	}
	else *resp = l_resp; // success

	qDebug("Qt conversation finished.");

	return(l_error_code);
}

int
ui_error_message(const char *message)
{
	QMessageBox::critical(0, gs_error_access_denied, message, QMessageBox::Close);
	return(0);
}

}

