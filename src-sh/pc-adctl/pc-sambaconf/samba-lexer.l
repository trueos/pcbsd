%{

/*
 *	samba lexer
 */

#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "pc-sambaconf.h"
#include "samba-parser.tab.h"

static	char *clean(char *str);

%}

%%

\[		{ return (SOPEN); }
\]		{ return (SCLOSE); }
=		{ return (EQUAL); }
\n		{ return (NEWLINE); }

[ \t]+

([ \t]+)?(;|#)(.+)?$	{
	yylval.str = strdup(clean(yytext));
	return (COMMENT);
}

[^\[\]\n\t=]+	{
	yylval.str = strdup(clean(yytext));
	return (STRING);
}


%%

static char * 
clean(char *str)
{
	char *ptr;

	while (isspace(*str))
		str++;
	ptr = &str[strlen(str) - 1];
	while (isspace(*ptr)) {
		*ptr = 0;
		ptr--;
	}

	return (str);
}

int
yywrap(void)
{
	return (1);
}
