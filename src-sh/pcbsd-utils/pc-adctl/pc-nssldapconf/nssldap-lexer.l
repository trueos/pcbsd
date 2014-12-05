%{

/*
 *	nss_ldap lexer
 */

#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "pc-nssldapconf.h"
#include "nssldap-parser.tab.h"

%}

%%

\n				{ return (NEWLINE); }
[[:space:]\t]+$	{ return (EMPTY); }
[[:space:]\t]+	{ return (SPACE); }

([ \t]+)?(;|#)(.+)?$	{
	yylval.str = strdup(clean(yytext));
	return (COMMENT);
}

[^#[:space:]]+ {
	yylval.str = strdup(clean(yytext));
	return (WORD);
}

%%

int
yywrap(void)
{
	return (1);
}
