%{

/*
 *	NSS lexer
 */

#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "pc-nssconf.h"
#include "nss-parser.tab.h"


%}

%%

files				{ return (FILES); }
db					{ return (DB); }
dns					{ return (DNS); }
nis					{ return (NIS); }	
compat				{ return (COMPAT); }
cache				{ return (CACHE); }

group				{ return (GROUP); }
group_compat		{ return (GROUP_COMPAT); }
hosts				{ return (HOSTS); }
networks			{ return (NETWORKS); }
passwd				{ return (PASSWD); }
passwd_compat		{ return (PASSWD_COMPAT); }
shells				{ return (SHELLS); }
services			{ return (SERVICES); }
services_compat		{ return (SERVICES_COMPAT); }
rpc					{ return (RPC); }
protocols			{ return (PROTOCOLS); }
netgroup			{ return (NETGROUP); }

success				{ return (SUCCESS); }
notfound			{ return (NOTFOUND); }
tryagain			{ return (TRYAGAIN); }
unavail				{ return (UNAVAIL); }

continue			{ return (CONTINUE); }
return				{ return (RETURN); }

\[					{ return (BOPEN); }
\]					{ return (BCLOSE); }
:					{ return (COLON); }
=					{ return (EQUAL); }
\n					{ return (NEWLINE); }

[[:space:]\t]+


([:space:]+)?#(.+)?	{
	yylval.str = strdup(clean(yytext));
	return (COMMENT);
}


[a-zA-Z_][a-zA-Z0-9_]+	{
	yylval.str = strdup(clean(yytext));
	return (STRING);
}


%%

int
yywrap(void)
{
	return (1);
}
