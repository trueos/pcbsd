/*-
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
	char *newargv[1024];
        int argoffset;

	// Tickle the ldconfig file
	system("/usr/pbi/.pbild >/dev/null 2>/dev/null");

	newargv[0] = argv[1];
	newargv[1] = '\0';
	// Setup the newargv with right stack
	for ( int i = 2; i<argc; i++)
	{
		//printf("argv %d %s\n", i, argv[i]);
		newargv[i-1] = argv[i];
		if ( i > 1020 ) {
			err(1, "Too many args...");
		}
		newargv[i] = '\0';
	}

	// Execute the PBI now
	//printf("Running %s\n", argv[1]);
	return execvp(argv[1], newargv);
}

static void
usage(void)
{

	fprintf(stderr, "usage: pbirun command\n");
	exit(1); 
}
