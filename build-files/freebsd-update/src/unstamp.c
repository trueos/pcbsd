/*-
 * Copyright 2006 Colin Percival
 * All rights reserved
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted providing that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <sys/cdefs.h>
__FBSDID("$FreeBSD: user/cperciva/freebsd-update-build/src/unstamp.c 247826 2013-03-05 00:52:14Z cperciva $");

#include <err.h>
#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

static void usage(void);
static void unstamp_text(FILE * F0, FILE * F1);
static void unstamp_binary(FILE * F0, FILE * F1);

static void
usage(void)
{
	fprintf(stderr, "usage: unstamp [-t] infile outfile < locations\n");
	exit(1);
	/* NOTREACHED */
}

/*
 * Read a sequence of increasing <line> values from stdin and
 * delete those lines from F0, writing the result to F1.
 */
static void
unstamp_text(FILE * F0, FILE * F1)
{
	int ch;				/* Character read from F0 */
	int linenum = 0;		/* Current line number */
	char * line;			/* Line of text from stdin */
	size_t linelen;			/* Length of line from stdin */
	int nextdelete;			/* Next line to delete */

	do {
		/* Read a line from stdin */
		if ((line = fgetln(stdin, &linelen)) == NULL) {
			if (feof(stdin))
				nextdelete = INT_MAX;
			else
				err(1, "fgetln");
		} else {
			/* Convert to integer */
			nextdelete = strtol(line, NULL, 0);
		}

		/* Sanity check */
		if (nextdelete < linenum)
			errx(1, "Stamp positions went backwards!");

		/* Read lines */
		do {
			/* Handle one line */
			do {
				/* Read a character */
				ch = getc(F0);

				/* Handle EOF / error */
				if (ch == EOF) {
					if (feof(F0))
						return;
					else
						err(1, "fgetc");
				}

				/* Write it, if we're not deleting the line */
				if (linenum != nextdelete)
					putc(ch, F1);
			} while (ch != '\n');

			linenum++;
		} while (linenum <= nextdelete);
	} while (1);
}

/*
 * Read a sequence of <start>|<len> pairs from stdin, where the <start>
 * values are all >= than the previous <start>+<len> values, and copy
 * bytes from F0 to F1, replacing the indicated bytes with zeros.
 */
static void
findstamps_binary(FILE * F0, FILE * F1)
{
	int ch;
	int pos = 0, spos = 0, slen = 0;
	char * line;
	size_t linelen;

	do {
		/* Read a line from stdin */
		if ((line = fgetln(stdin, &linelen)) == NULL) {
			if (feof(stdin)) {
				spos = INT_MAX;
				slen = 0;
			} else
				err(1, "fgetln");
		} else {
			/* Convert to integer */
			spos = strtol(line, &line, 0);
			line++;
			slen = strtol(line, NULL, 0);
		}

		/* Sanity check */
		if (spos < pos)
			errx(1, "Stamp positions went backwards!");

		/* Copy bytes */
		do {
			/* Read a byte */
			ch = getc(F0);

			/* Handle EOF / error */
			if (ch == EOF) {
				if (feof(F0))
					return;
				else
					err(1, "fgetc");
			}

			/* Write the byte */
			if (pos < spos)
				putc(ch, F1);
			else
				putc(0, F1);

			pos++;
		} while (pos < spos + slen);
	} while (1);
}

int
main(int argc, char *argv[])
{
	FILE * F0, * F1;
	int tflag = 0;

	/* Are we examining text files? */
	if (argc < 2)
		usage();
	if (strcmp(argv[1], "-t") == 0) {
		tflag = 1;
		argc--;
		argv++;
	}

	/* Open the files */
	if (argc != 3)
		usage();
	if ((F0 = fopen(argv[1], "r")) == NULL)
		err(1, "fopen(%s)", argv[1]);
	if ((F1 = fopen(argv[2], "w")) == NULL)
		err(1, "fopen(%s)", argv[2]);

	/* Remove stamps */
	if (tflag)
		unstamp_text(F0, F1);
	else
		findstamps_binary(F0, F1);

	/* Close the files */
	if (fclose(F0))
		err(1, "fclose(%s)", argv[1]);
	if (fclose(F1))
		err(1, "fclose(%s)", argv[2]);

	return 0;
}
