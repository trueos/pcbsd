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

#include "database.h"

#include <sys/stat.h>
#include <sys/types.h>

#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "session.h"
#include "string.h"
#include "strings.h"

#include "global.h"

#define QSU_DB_MAGIC 4106420479 // great movie

/********************************************************** global variables */

extern char **environ;

/******************************************************** local declarations */

static int prepare_database_directory(const char *path);
static int prepare_database_file(const char *path);

/*********************************************************** local variables */

static char s_db_path[PATH_MAX];
static int  s_db_file;

/*************************************************************** definitions */

int
qsu_database_initialize(qsu_session *session)
{
	s_db_path[0] = '\0';
	s_db_file = 0;

	strcpy(s_db_path, QSU_DATABASE_PATH);
	if (prepare_database_directory(s_db_path) == FAILURE)
		return(FAILURE);

	strcat(s_db_path, "/");
	strcat(s_db_path, session->euser);
	if (prepare_database_directory(s_db_path) == FAILURE)
		return(FAILURE);

	strcat(s_db_path, "/");
	strcat(s_db_path, getenv("DISPLAY"));
	if (prepare_database_directory(s_db_path) == FAILURE)
		return(FAILURE);

	/* append filename - i.e. target user */
	strcat(s_db_path, "/");
	strcat(s_db_path, session->user);
	if (prepare_database_file(s_db_path) == FAILURE)
		return(FAILURE);

	return(SUCCESS);
}

void
qsu_database_finalize(qsu_session *session)
{
	QSU_UNUSED(session);

	if (s_db_file)
		close(s_db_file);

	s_db_path[0] = '\0';
	s_db_file = 0;
}

int
qsu_database_authenticate(qsu_session *session)
{
	char  *l_name, *l_value;
	int    l_delta;
	size_t l_len = 0;
	time_t l_auth;
	unsigned int l_magic;

	QSU_UNUSED(session);

	/* move cursor to beginning */
	if (lseek(s_db_file, 0, SEEK_SET) == -1) {
		perror("lseek");
		return(FAILURE);
	}

	/* check for magic */
	l_magic = 0;
	if (read(s_db_file, &l_magic, sizeof(l_magic)) == -1) {
		perror("read");
		return(FAILURE);
	}
	if (l_magic != QSU_DB_MAGIC)
		return(FAILURE);

	/* read expiration (dangerously) */
	l_auth = 0;
	if (read(s_db_file, &l_auth, sizeof(l_auth)) == -1) {
		perror("read");
		return(FAILURE);
	}

	/* validity check */
	l_delta = l_auth + QSU_AUTH_TIMEOUT - time(0);
	if ( l_delta < 0 || l_delta > QSU_AUTH_TIMEOUT)
		return(FAILURE);

	/*
	 * Environment
	 */

	do {
		l_len = 0;

		if (read(s_db_file, &l_len, sizeof(l_len)) <= 0)
			break;

		l_value = malloc(l_len);
		if (read(s_db_file, l_value, l_len) == -1) {
			free(l_value);
			perror("read");
			return(FAILURE);
		}

		l_name = strsep(&l_value, "=");
		if (!l_name || !l_value
		    || strlen(l_name) == 0 || strlen(l_value) == 0)
			continue;

		if (setenv(l_name, l_value, 1) != 0) {
			free(l_name);
			perror("setenv");
			return(FAILURE);
		}

		free(l_name);
	}
	while(1);

	return(SUCCESS);
}

int
qsu_database_reset(qsu_session *session)
{
	const unsigned int l_magic = QSU_DB_MAGIC;
	const time_t l_auth = time(0);
	char **l_env;

	QSU_UNUSED(session);

	/* move cursor to beginning */
	if (lseek(s_db_file, 0, SEEK_SET) == -1) {
		perror("lseek");
		return(FAILURE);
	}

	/* writemagic */
	if (write(s_db_file, &l_magic, sizeof(l_magic)) == -1) {
		perror("write");
		return(FAILURE);
	}

	/* write expiration */
	if (write(s_db_file, &l_auth, sizeof(l_auth)) == -1) {
		perror("write");
		return(FAILURE);
	}

	/*
	 * Environment
	 */

	l_env = environ;
	while(*l_env) {
		size_t l_len = strlen(*l_env) + 1;
		if (write(s_db_file, &l_len, sizeof(l_len)) == -1) {
			perror("write");
			return(FAILURE);
		}

		if (write(s_db_file, *l_env, l_len) == -1) {
			perror("write");
			return(FAILURE);
		}

		++l_env;
	}

	return(SUCCESS);
}

/*****************************************************************************/

int
prepare_database_directory(const char *path)
{
	struct stat l_sb;

	/* get directory stats */
	if (stat(path, &l_sb) == -1) {
		if (errno != ENOENT) {
			perror("stat");
			return(FAILURE);
		}

		/* create directory */
		if (mkdir(path, S_IRWXU) == -1) {
			perror("mkdir");
			return(FAILURE);
		}
	}

	/* found, now make sure it's a directory */
	else if ((l_sb.st_mode & S_IFMT) != S_IFDIR)
		return(FAILURE);

	/* check for valid directory permissions */
	else if ((l_sb.st_mode & 0777) != S_IRWXU) {
		/* wrong permissions, try to correct */
		if (chmod(path, S_IRWXU) == -1)
			return(FAILURE);
	}

	return(SUCCESS);
}

int
prepare_database_file(const char *path)
{
	struct stat l_sb;

	/* get file stats */
	if (stat(path, &l_sb) == -1) {
		if (errno != ENOENT) {
			perror("stat");
			return(FAILURE);
		}

		if ((s_db_file = creat(path, S_IRUSR|S_IWUSR)) == -1) {
			s_db_file = 0;
			perror("creat");
			return(FAILURE);
		}

		/* we a handle capable of both read & write */
		close(s_db_file);
	}

	/* found, now make sure it's a regular file */
	else if ((l_sb.st_mode & S_IFMT) != S_IFREG)
		return(FAILURE);

	/* check for valid permissions */
	else if ((l_sb.st_mode & 0777) != (S_IRUSR|S_IWUSR)) {
		/* wrong permissions, try to correct */
		if (chmod(path, S_IRUSR|S_IWUSR) == -1)
			return(FAILURE);
	}

	/* open file */
	if ((s_db_file = open(path, O_RDWR)) == -1) {
		s_db_file = 0;
		perror("open");
		return(FAILURE);
	}

	return(SUCCESS);
}

