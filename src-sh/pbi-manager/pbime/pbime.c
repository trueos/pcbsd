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
#include <sys/types.h>

#include <err.h>
#include <errno.h>
#include <libgen.h>
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
	char initscript[4096];
	char hintsfile[4096];
	char pbirun[500];
	char *newargv[1024];
	char newlibdir[MAXPATHLEN];
        int argoffset;

	/* Is this a request to cleanup? */
	if ( argc == 3 )
		return 0;

	if (argc < 4)
		usage();

	if ( (strlen("/usr/pbi/.pbi_preload.so") + strlen(argv[1]) + strlen(argv[2])) > 4090)
                err(1, "Max length exceeded for pbidir");

	// Set the environment variables
	setenv("LD_32_PBIPRELOAD", "/usr/pbi/.pbi_preload32.so", 1);
	setenv("LD_PBIPRELOAD", "/usr/pbi/.pbi_preload.so", 1);

	setenv("PBI_RUNDIR", argv[2], 1);
	unsetenv("PBI_BREAKOUT");

	// Set the hints-file location
	strcpy(hintsfile, "/var/run/ld-elf.so.hints.");
        strcat(hintsfile, basename(argv[2]));
	setenv("LD_ELF_HINTS_PATH", hintsfile, 1);
	strcpy(hintsfile, "/var/run/ld-elf32.so.hints.");
        strcat(hintsfile, basename(argv[2]));
	setenv("LD_32_ELF_HINTS_PATH", hintsfile, 1);
	
	// Do the init of the environment
	strcpy(initscript, "/usr/pbi/.pbiinit ");
	strcat(initscript, argv[2]);
	//printf( "initscript: %s \n", initscript);
	//if ( system(initscript) != 0 )
	//	err(1, "Failed PBI init!");
	
	// Backwards compat check for old PBIs
	if ( (argc > 5) && (strcmp(argv[5], "args") == 0) )
	{
		argoffset=6;
	} else {
       	        // Running with old pbi wrapper
               	argoffset=4;
	}

	// Setup the newargv with right stack
	//if (strstr(argv[2], "-i386"))
	//	strcpy(pbirun, "/usr/pbi/.pbirun32");
	//else

	strcpy(pbirun, "/usr/pbi/.pbirun");

	newargv[0] = pbirun;
	newargv[1] = argv[3];

	int j = 2;
	for ( int i = argoffset; i<argc; i++)
	{
		newargv[j] = argv[i];
		if ( i > 1020 ) {
			err(1, "Too many args...");
		}
		j++;
	}
	//sleep(5);

	// Make sure 32bit compat is enabled
	strncat(newlibdir, "/usr/local", (strlen("/usr/local") ));
	strcat(newlibdir, "/lib");
	setenv("LD_32_LIBRARY_PATH", newlibdir, 1);

	//printf("Running %s - %s\n", newargv[0], newargv[1]);

	// Execute the PBI now
	return execvp("/usr/pbi/.pbirun", newargv);
}

static void
usage(void)
{

	fprintf(stderr, "usage: pbime mntdir pbidir command cwd [...]\n");
	exit(1); 
}
