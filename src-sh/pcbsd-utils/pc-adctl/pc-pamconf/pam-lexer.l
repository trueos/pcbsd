%{

/*
 *	pam lexer
 */

#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "pc-pamconf.h"
#include "pam-parser.tab.h"

%}

%%


\n				{ return (NEWLINE); }
^#(.+)?$		{ yylval.str = xstrdup(yytext); return (COMMENT); }

auth			{ return (AUTH); }
account			{ return (ACCOUNT); }
session			{ return (SESSION); }
password		{ return (PASSWORD); }
required		{ return (REQUIRED); }
requisite		{ return (REQUISITE); }
sufficient		{ return (SUFFICIENT); }
binding			{ return (BINDING); }
optional		{ return (OPTIONAL); }
include			{ return (INCLUDE); }

[a-zA-Z0-9_\-\./=]+	{ yylval.str = xstrdup(yytext); return (STRING); }
[ \t]+

%%

int
yywrap(void)
{
	return (1);
}
