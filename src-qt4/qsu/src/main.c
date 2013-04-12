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

#include <sys/param.h>
#include <sys/wait.h>

#include <security/pam_appl.h>

#include <err.h>
#include <getopt.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "conversation.h"
#include "database.h"
#include "session.h"
#include "strings.h"

#include "global.h"

/******************************************************** local declarations */

static void main_usage(void);

static int  main_initialize(qsu_session *session, char *user, char *desc);
static void main_cleanup(qsu_session *session);

static int  main_database_authenticate(qsu_session *session);

static int  main_pam_initialize(qsu_session *session);
static int  main_pam_authenticate(qsu_session *session);
static int  main_pam_set_items(qsu_session *session);

static void main_pam_environment(qsu_session *session);

/*********************************************************** local variables */

static const char *s_command;

/*************************************************************** definitions */

int
main(int argc, char *argv[])
{
	qsu_session session;

	int   opt;
	int   status = 1;      /* exit status */
	char *user   = "root"; /* default user */
	char *description = 0; /* description override */
	
	int   fstatus;         /* fork status */
	pid_t fpid;            /* fork pid */

	s_command = argv[0];

	/* check for X11 display */
	if (!getenv("DISPLAY")) {
		fprintf(stderr, gs_error_no_display, "");
		fprintf(stderr, "\n");
		main_usage();
		return(1);
	}

	/* parse arguments */
	while ((opt = getopt(argc, argv, "hu:d:")) != FAILURE)
		switch (opt) {
		case 'u':
			user = optarg;
			break;

		case 'd':
			description = optarg;
			break;

		default:
			main_usage();
			return(1);
		}
	argc -= optind;
	argv += optind;

	/* check for command */
	if (argc <= 0) {
		main_usage();
		return(1);
	}

	/*
	 * ** Initialization **
	 *
	 * Initialize local session check for previous authenticated session.
	 */

	if (main_initialize(&session, user, description) == FAILURE) {
		main_cleanup(&session);
		return(1);
	}

	if (main_database_authenticate(&session) == SUCCESS) {
		fprintf(stderr, gs_database_authenticated, "");
		fprintf(stderr, "\n");
	}

	/*
	 * ** PAM authentication **
	 *
	 * UI frontend will be initiated, pam will start, we will authenticate
	 * and a new session will be started.
	 */

	else if (main_pam_initialize(&session)   == FAILURE ||
	         main_pam_set_items(&session)    == FAILURE ||
	         main_pam_authenticate(&session) == FAILURE) {
		main_cleanup(&session);
		return(1);
	}

	/* get target user account information */
	if ((session.pwd = getpwnam(session.user)) == NULL) {
		perror("getpwnam");
		return(1);
	}

	/* reset stored session if authenticated via pam */
	if ((session.cleanup & (qsu_scleanup_db_started|qsu_scleanup_pam_session)) ==
	    (qsu_scleanup_db_started|qsu_scleanup_pam_session))
		qsu_database_reset(&session);

	/*
	 * ** Perform fork **
	 *
	 * If we got this far it means we may proceed.
	 */

	switch ((fpid = fork())) {

	case -1:
		/* fork failed - abort */
		warn("fork()");
		break;

	case 0:
		/* set uid and groups */
		if (initgroups(session.pwd->pw_name,
		               session.pwd->pw_gid) == FAILURE) {
			warn("initgroups()");
			break;
		}
		if (setgid(session.pwd->pw_gid) == FAILURE) {
			warn("setgid()");
			break;
		}
		if (setuid(session.pwd->pw_uid) == FAILURE) {
			warn("setuid()");
			break;
		}

		execvp(*argv, argv);
		warn("execvp()");
		break;

	default:

		waitpid(fpid, &fstatus, 0);
		status = WEXITSTATUS(fstatus);
	}

	if (status != 0) {
		fprintf(stderr, gs_error_failed, "");
		fprintf(stderr, "\n");
	}

	main_cleanup(&session);
	return(status);
}

void
main_usage(void)
{
	fprintf(stderr, gs_usage, s_command);
	fprintf(stderr, "\n");
}

/*****************************************************************************/

int
main_initialize(qsu_session *session, char *user, char *desc)
{
	struct passwd *l_pw;
	
	/* get actual username */
	if ((l_pw = getpwuid(getuid())) == NULL)
	    return(FAILURE);

	memset(session, 0, sizeof(*session));
	session->euser = l_pw->pw_name;
	session->user = user;
	session->conv = 0;
	session->status = 0;
	session->cleanup = 0;

	/* select appropriate description */

	if (!desc) {
		if (session->user && strcmp(session->user, gs_default_user) == 0)
			session->description = gs_default_desc_default;
		else
			session->description = gs_default_desc_other;
	} else session->description = desc;

	return(SUCCESS);
}

void
main_cleanup(qsu_session *session)
{
	if (session->cleanup & qsu_scleanup_pam_session)
		session->status = pam_close_session(session->handle, 0);

	if (session->cleanup & qsu_scleanup_pam_started)
		pam_end(session->handle, session->status);

	free(session->conv), session->conv = 0;

	if (session->cleanup & qsu_scleanup_db_started)
		qsu_database_finalize(session);

	session->handle = 0;
	session->cleanup = 0;

	ui_finalize();
}

int
main_database_authenticate(qsu_session *session)
{
	if (qsu_database_initialize(session) == FAILURE) {
		fprintf(stderr, gs_error_database_init, QSU_DATABASE_PATH);
		fprintf(stderr, "\n");
		return(FAILURE);
	}

	session->cleanup |= qsu_scleanup_db_started;

	/* authenticate if possible */
	if (qsu_database_authenticate(session) == FAILURE)
		return(FAILURE);

	return(SUCCESS);
}

int
main_pam_initialize(qsu_session *session)
{
	session->conv = malloc(sizeof(*session->conv));
	session->conv->conv = ui_conversation;
	session->conv->appdata_ptr = (void *)session;

	/* start pam */

	if ((session->status = pam_start("su", session->user, session->conv, &session->handle)) != PAM_SUCCESS)
		return(FAILURE);

	session->cleanup |= qsu_scleanup_pam_started;

	ui_initialize();

	return(SUCCESS);
}

int
main_pam_authenticate(qsu_session *session)
{
	if ((session->status = pam_authenticate(session->handle, 0)) != PAM_SUCCESS) {
		ui_error_message(gs_error_auth_failed);
		return(FAILURE);
	}

	if ((session->status = pam_acct_mgmt(session->handle, 0)) == PAM_NEW_AUTHTOK_REQD &&
	    (session->status = pam_chauthtok(session->handle, PAM_CHANGE_EXPIRED_AUTHTOK)) != PAM_SUCCESS) {
		ui_error_message(gs_error_auth_expired);
		return(FAILURE);
	}

	if ((session->status = pam_setcred(session->handle, PAM_ESTABLISH_CRED)) != PAM_SUCCESS ||
	    (session->status = pam_open_session(session->handle, 0)) != PAM_SUCCESS) {
		ui_error_message(gs_error_auth_failed);
		return(FAILURE);
	}

	session->cleanup |= qsu_scleanup_pam_session;

	if ((session->status = pam_get_item(session->handle, PAM_USER, (const void **)&session->user)) != PAM_SUCCESS ||
	    (session->pwd = getpwnam(session->user)) == NULL) {
		ui_error_message(gs_error_auth_failed);
		return(FAILURE);
	}

	main_pam_environment(session);

	return(SUCCESS);
}

int
main_pam_set_items(qsu_session *session)
{
	const char *l_user, *l_display;

	l_user    = getlogin();
	l_display = getenv("DISPLAY");

	if ((session->status = pam_set_item(session->handle, PAM_RUSER, l_user))  != PAM_SUCCESS ||
	    (session->status = pam_set_item(session->handle, PAM_TTY, l_display)) != PAM_SUCCESS)
		return(FAILURE);

	return(SUCCESS);
}

void
main_pam_environment(qsu_session *session)
{
	char **l_envlist, **l_env;
	char  *l_name, *l_value;

	if ((l_envlist = pam_getenvlist(session->handle)) != 0) {
		for (l_env = l_envlist; *l_env != NULL; ++l_env) {
			l_value = *l_env;
			l_name = strsep(&l_value, "=");

			if (!l_name || !l_value
			    || strlen(l_name) == 0 || strlen(l_value) == 0)
				continue;

			setenv(l_name, l_value, 1);
			free(*l_env);
		}
		free(l_envlist);
	}

        // New user can't connect to it
        unsetenv("DBUS_SESSION_BUS_ADDRESS");
}
