%{

/*
 *	krb5.conf lexer
 */

#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "pc-krbconf.h"
#include "krb5-parser.tab.h"

int lineno = 0;

%}

%%

\[	{ return (S_OPEN); }
\]	{ return (S_CLOSE); }
\{	{ return (B_OPEN); }
\}	{ return (B_CLOSE); }
=	{ return (EQ); }
\n	{ lineno++; }

[[:space:]\t]+

([:space:]+)?#(.+)?	{
	yylval.str = xstrdup(clean(yytext));
	return (COMMENT);
}

[a-zA-Z0-9_\-\.:/]+	{
	yylval.str = xstrdup(clean(yytext));
	return (STRING);
}

%%

int
yywrap(void)
{
	return (1);
}
