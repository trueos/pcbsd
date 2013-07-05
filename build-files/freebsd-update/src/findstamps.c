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
__FBSDID("$FreeBSD: user/cperciva/freebsd-update-build/src/findstamps.c 247826 2013-03-05 00:52:14Z cperciva $");

#include <sys/types.h>

#include <err.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void usage(void);
static void findstamps_text(FILE * F0, FILE * F1);
static void findstamps_binary(FILE * F0, FILE * F1);

static void
usage(void)
{
	fprintf(stderr, "usage: findstamps [-t] file0 file1\n");
	exit(1);
	/* NOTREACHED */
}

/*
 * A text buildstamp consists of a line of line of text (immediately
 * preceded by a '\n' character, and ending with a '\n' character)
 * which differs between the two files.  It is notated by the number
 * of '\n' characters preceding it (i.e., files start at line zero).
 */
static void
findstamps_text(FILE * F0, FILE * F1)
{
	int ch0, ch1;			/* Characters read from F0, F1 */
	off_t stampendpos;		/* Offset in F0 of end of stamp */
	off_t linestartpos = 0;		/* Offset in F0 of start of line */
	int linenum = 0;		/* Line number being examined */

	do {
		/* Read a character from each file */
		ch0 = getc(F0);
		ch1 = getc(F1);

		/* Check for errors */
		if (ferror(F0) || ferror(F1))
			err(1, "getc");

		/* Have we reached the EOF of both files? */
		if (feof(F0) && feof(F1))
			return;

		/* Have we reached EOL in both files? */
		if ((ch0 == '\n') && (ch1 == '\n')) {
			linestartpos = ftello(F0);
			linenum++;
		}

		/* If the characters are equal, keep on going */
		if (ch0 == ch1)
			continue;

		/*
		 * We've found a difference between the two files.  Scan
		 * forward in each line until we find a '\n'.  If we run
		 * into EOF, exit with an error, since this means that
		 * either the files have a different number of lines, or
		 * the last line isn't EOL-terminated.
		 */
		printf("%d\n", linenum);
		while (ch0 != '\n') {
			if (feof(F0))
				errx(1, "EOF encountered within stamp!");
			ch0 = getc(F0);
			if (ferror(F0))
				err(1, "getc");
		}
		while (ch1 != '\n') {
			if (feof(F1))
				errx(1, "EOF encountered within stamp!");
			ch1 = getc(F1);
			if (ferror(F1))
				err(1, "getc");
		}

		/* Go back and print out the stamp value */
		stampendpos = ftello(F0);
		if (fseeko(F0, linestartpos, SEEK_SET))
			err(1, "fseeko");
		do {
			ch0 = getc(F0);
			if (ferror(F0))
				err(1, "getc");
			fprintf(stderr, "%c", ch0);
		} while (ch0 != '\n');

		/* We have reached EOL in both files */
		linestartpos = ftello(F0);
		linenum++;
	} while (1);
}

/*
 * A binary buildstamp consists of a series of "string" characters
 * characters (immediately preceded by a non-string character),
 * plus zero or more following NUL characters, which is different
 * between the two files; a "string" character is something which
 * isprint(3) agrees with, or an EOL character after a difference
 * has already been seen.  Buildstamps are notated by the offset
 * of the first character and the number of characters.
 */
/*-
 * We find the stamps using a finite state machine; states 0, 1, and 2
 * mean "between stamps", "inside a stamp", and "inside the trailing
 * NUL characters of a stamp" respectively.
 *
 * State  Characters  New state  startpos  Output
 * 0      non-string  0	         pos + 1
 * 1      non-string  0          pos + 1   yes
 * 2      non-string  0          pos + 1   yes
 * 0      string      0
 * 1      string      1
 * 2      string      0          pos       yes
 * 0      mismatch    1
 * 1      mismatch    1
 * 2      mismatch    1          pos       yes
 * 0      NUL         0	         pos + 1
 * 1      NUL         2
 * 2      NUL         2
 * 0      EOF         -1
 * 1      EOF         -1                   yes
 * 2      EOF         -1                   yes
 */

static void
findstamps_binary(FILE * F0, FILE * F1)
{
	int ch0, ch1;			/* Characters read from F0, F1 */
	int state = 0;			/* FSM state */
	off_t pos = 0, spos = 0, newspos = 0;
	int output = 0;

	for (pos = 0; state != -1; pos++) {
		/* Read a character from each file */
		ch0 = getc(F0);
		ch1 = getc(F1);

		/* Check for errors */
		if (ferror(F0) || ferror(F1))
			err(1, "getc");

		if (ch0 != ch1) {		/* Non-matching characters */
			/* Check for EOF of either file */
			if (feof(F0) || feof(F1))
				errx(1, "Files have different lengths!");

			if (state == 2) {
				newspos = pos;
				output = 1;
			}
			state = 1;
		} else if (ch0 == 0) {		/* Matching NUL characters */
			if (state == 0)
				newspos = pos + 1;
			else
				state = 2;
		} else if (ch0 == EOF) {	/* EOF of both files */
			if (state != 0)
				output = 1;
			state = -1;
		} else if (isprint(ch0) || (ch0 == '\n')) {
					/* Matching string characters */
			if ((state == 0) && (ch0 == '\n')) {
				/* ok, actually not a string character */
				newspos = pos + 1;
			} else if (state == 2) {
				state = 0;
				newspos = pos;
				output = 1;
			}
		} else {		/* Matching non-string characters */
			if (state != 0)
				output = 1;
			state = 0;
			newspos = pos + 1;
		}

		/* Output, if needed */
		if (output) {
			printf("%ju|%ju\n", (uintmax_t)(spos),
			    (uintmax_t)(pos - spos));

			/* Seek to start of buildstamp */
			if (fseeko(F0, spos, SEEK_SET))
				err(1, "fseeko");

			/* Print out buildstamp */
			for (; spos < pos; spos++) {
				/* Read a character */
				ch0 = getc(F0);
				if (ferror(F0))
					err(1, "getc");

				/* Stop printing the stamp if we hit a NUL */
				if (ch0 == 0)
					break;

				/* Print the character */
				fprintf(stderr, "%c", ch0);
				ch1 = ch0;
			}

			/* If we didn't end with a \n, print one now */
			if (ch1 != '\n')
				fprintf(stderr, "\n");

			/* Seek back to where we were before */
			if (fseeko(F0, pos + 1, SEEK_SET))
				err(1, "fseeko");
			output = 0;
		}

		/* Update spos */
		spos = newspos;
	}
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
	if ((F1 = fopen(argv[2], "r")) == NULL)
		err(1, "fopen(%s)", argv[2]);

	/* Find the stamps */
	if (tflag)
		findstamps_text(F0, F1);
	else
		findstamps_binary(F0, F1);

	/* Close the files */
	if (fclose(F0))
		err(1, "fclose(%s)", argv[1]);
	if (fclose(F1))
		err(1, "fclose(%s)", argv[2]);

	return 0;
}
