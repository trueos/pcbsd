/*-
 * Copyright (c) 2003 Mike Barcroft <mike@FreeBSD.org>
 * Copyright (c) 2007 Bill Moran/Collaborative Fusion
 * Copyright (c) 2013 Kris Moore/PC-BSD Software <kris@pcbsd.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

#include <sys/param.h>
#include <sys/jail.h>
#include <sys/types.h>

#include <err.h>
#include <errno.h>
#include <login_cap.h>
#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <unistd.h>
#include <string.h>

static void	usage(void);

int
main(int argc, char *argv[])
{
	char newchroot[2048];
	char mountscript[4096];
	int jid, ngroups;
        int argoffset;
	uid_t huid;
	struct passwd *husername, *jusername;
	gid_t groups[NGROUPS];
	login_cap_t *lcap;

	/* Is this a request to unmount? */
	if ( argc == 3 ) {
		if ( strcmp(argv[1], "umount") == 0 ) {
			strcpy(mountscript, "/usr/pbi/.pbimount umount ");
			strcat(mountscript, argv[2]);
			return system(mountscript);
		}
	}

	if (argc < 4)
		usage();

	/* Get the current user ID and user name in the host system */
	huid = getuid();
	husername = getpwuid(huid);

	/* Run the mount script */
	if ( (strlen("/usr/pbi/.pbimount") + strlen(argv[1]) + strlen(argv[2])) > 4090)
                err(1, "Max length exceeded for pbidir / mntdir");

	if ( strlen("argv[2]") + strlen("/virtbase") > 2040)
                err(1, "Max length exceeded for virtbase");
	strcpy(newchroot, argv[1]);
	strcat(newchroot, "/virtbase");
	
	strcpy(mountscript, "/usr/pbi/.pbimount ");
	strcat(mountscript, argv[1]);
	strcat(mountscript, " ");
	strcat(mountscript, argv[2]);
	//printf( "mountscript: %s \n", mountscript);
	if ( system(mountscript) != 0 )
                err(1, "Failed mounting PBI");

	if (chdir(newchroot) == -1 || chroot(".") == -1)
                err(1, "Could not chroot to: %s", newchroot);

        argoffset=4;
	if (chdir(argv[4]) == -1 ) {
                // Running with old pbi wrapper
                argoffset=3;
        }

	/* Get the user name in the jail */
	jusername = getpwuid(huid);
	if (jusername == NULL || strcmp(husername->pw_name, jusername->pw_name) != 0)
		err(1, "Username mapping failed");
	//if (chdir("/") == -1)
	//	err(1, "chdir(): /");
	lcap = login_getpwclass(jusername);
	if (lcap == NULL) {
	  err(1, "getpwclass: %s", jusername->pw_name);
        }
	ngroups = NGROUPS;
	if (getgrouplist(jusername->pw_name, jusername->pw_gid, groups, &ngroups) != 0)	
		err(1, "getgrouplist: %s", jusername->pw_name);
	if (setgroups(ngroups, groups) != 0)
		err(1, "setgroups");
	if (setgid(jusername->pw_gid) != 0)
		err(1, "setgid");
	if (setusercontext(lcap, jusername, jusername->pw_uid,
	    LOGIN_SETALL & ~LOGIN_SETGROUP & ~LOGIN_SETLOGIN) != 0)
		err(1, "setusercontext");
	login_close(lcap);
	if (execvp(argv[3], argv + argoffset) == -1)
		err(1, "execvp(): %s", argv[3]);
	exit(0);
}

static void
usage(void)
{

	fprintf(stderr, "usage: pbime mntdir pbidir command [...]\n");
	exit(1); 
}
