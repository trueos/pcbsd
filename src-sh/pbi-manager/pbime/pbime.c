/*-
 * Copyright (c) 2003 Mike Barcroft <mike@FreeBSD.org>
 * Copyright (c) 2007 Bill Moran/Collaborative Fusion
 * Copyright (c) 2014 Kris Moore/PC-BSD Software <kris@pcbsd.org>
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
#include <sys/wait.h>

#include <err.h>
#include <errno.h>
#include <login_cap.h>
#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <unistd.h>
#include <string.h>

static void	usage(void);

void ch_user(void);

void ch_user(void)
{
	int jid, ngroups;
	uid_t huid;
	struct passwd *husername, *jusername;
	gid_t groups[NGROUPS];
	login_cap_t *lcap;

	/* Get the current user ID and user name in the host system */
	huid = getuid();
	husername = getpwuid(huid);

	/* Get the user name in the jail */
	jusername = getpwuid(huid);
	if (jusername == NULL || strcmp(husername->pw_name, jusername->pw_name) != 0)
		err(1, "Username mapping failed");
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
}

int
main(int argc, char *argv[])
{
	char newchroot[2048];
	char mountscript[4096];
	char pipetemplate[] = "/tmp/.pbi-callback-XXXXXXXXXXXXXXXX";
        char *pipedir;
	char *newargv[1024];
	char fifoin[MAXPATHLEN]; 
	char newlibdir[MAXPATHLEN];
        int argoffset;

	pid_t listen_pid;
	pid_t app_pid;
	int listen_status;
	int app_status;

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

	// Lets get our template directory
	pipedir = mktemp(pipetemplate);

	// Set the environment variable
	setenv("PBI_SYSPIPE", pipedir, 1);
	
	// Fork off a listener for system commands
	listen_pid = fork();
	if(listen_pid == 0) {
		/* This is done by the child process. */
		// drop priv
		ch_user();

		// Create the pipe file
		strcpy(fifoin, "mkfifo ");
		strcat(fifoin, pipedir);
		if ( system(fifoin) == 0 ) {
			// Start the listen process only if mkfifo is successful
			exit(system("sh /usr/pbi/.pbisyslisten"));
		}
  	} else {

		// Do the chroot
		if (chdir(newchroot) == -1 || chroot(".") == -1)
                	err(1, "Could not chroot to: %s", newchroot);

		// drop priv
		ch_user();

		// Backwards compat check for old PBIs
		argoffset=5;
		if ( (argc > 5) && (strcmp(argv[5], "args") == 0) )
		{
			chdir(argv[4]);
		} else {
        	        // Running with old pbi wrapper
                	argoffset=3;
		}

		// Setup the newargv with right stack
		for ( int i = argoffset; i<argc; i++)
		{
			newargv[i-argoffset] = argv[i];
			if ( i > 1020 ) {
				err(1, "Too many args...");
			}
		}
		newargv[0] = argv[3];

		// Make sure 32bit compat is enabled
		strncat(newlibdir, "/usr/local", (strlen("/usr/local") ));
		strcat(newlibdir, "/lib");
		setenv("LD_32_LIBRARY_PATH", newlibdir, 1);

		// Fork off the PBI process, and have the parent wait for it to finish
		app_pid = fork();
		if(app_pid == 0) {
			// Execute the PBI now
			if (execvp(argv[3], newargv) == -1)
				err(1, "execvp(): %s", argv[3]);
			exit(0);
		} else {
     			/* This is run by the parent.  Wait for the child to terminate. */
			pid_t tpid;
			do {
       				tpid = wait(&app_status);
			} while(tpid != app_pid);

			// Stop the listener
			strcpy(fifoin, "echo 'CLOSE:' > ");
			strcat(fifoin, pipedir);
			system(fifoin);

			// Remove the old pipefile
			strcpy(fifoin, "rm ");
			strcat(fifoin, pipedir);
			system(fifoin);

     			return app_status;
		}

  	}

	exit(0);
}

static void
usage(void)
{

	fprintf(stderr, "usage: pbime mntdir pbidir command cwd [...]\n");
	exit(1); 
}
