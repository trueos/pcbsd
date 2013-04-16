%{

/*
 *	ldap lexer
 */

#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "pc-ldapconf.h"
#include "ldap-parser.tab.h"

%}

%%


[Uu][Rr][Ii] {
	return (URI);
}
[Bb][Aa][Ss][Ee] {
	return (BASE);
}
[Bb][Ii][Nn][Dd][Dd][Nn] {
	return (BINDDN);
}
[Dd][Ee][Rr][Ee][Ff] {
	return (DEREF);
}

[Nn][Ee][Vv][Ee][Rr] {
	return (NEVER);
}
[Ss][Ee][Aa][Rr][Cc][Hh][Ii][Nn][Gg] {
	return (SEARCHING);
}
[Ff][Ii][Nn][Dd][Ii][Nn][Gg] {
	return (FINDING);
}
[Aa][Ll][Ww][Aa][Yy][Ss] {
	return (ALWAYS);
}

[Hh][Oo][Ss][Tt] {
	return (HOST);
}
[Nn][Ee][Tt][Ww][Oo][Rr][Kk]_[Tt][Ii][Mm][Ee][Oo][Uu][Tt] {
	return (NETWORK_TIMEOUT);
}
[Pp][Oo][Rr][Tt] {
	return (PORT);
}
[Rr][Ee][Ff][Ee][Rr][Rr][Aa][Ll][Ss] {
	return (REFERRALS);
}

[Oo][Nn] {
	return (ON);
}
[Tt][Rr][Uu][Ee] {
	return (TRUE);
}
[Yy][Ee][Ss] {
	return (YES);
}
[Oo][Ff][Ff] {
	return (OFF);
}
[Ff][Aa][Ll][Ss][Ee] {
	return (FALSE);
}
[Nn][Oo] {
	return (NO);
}

[Ss][Ii][Zz][Ee][Ll][Ii][Mm][Ii][Tt] {
	return (SIZELIMIT);
}
[Tt][Ii][Mm][Ee][Ll][Ii][Mm][Ii][Tt] {
	return (TIMELIMIT);
}
[Tt][Ii][Mm][Ee][Oo][Uu][Tt] {
	return (TIMEOUT);
}

[Ss][Aa][Ss][Ll]_[Mm][Ee][Cc][Hh] {
	return (SASL_MECH);
}
[Ss][Aa][Ss][Ll]_[Rr][Ee][Aa][Ll][Mm] {
	return (SASL_REALM);
}
[Ss][Aa][Ss][Ll]_[Aa][Uu][Tt][Hh][Cc][Ii][Dd] {
	return (SASL_AUTHCID);
}
[Ss][Aa][Ss][Ll]_[Aa][Uu][Tt][Hh][Za][Ii][Dd] {
	return (SASL_AUTHZID);
}
[Ss][Aa][Ss][Ll]_[Ss][Ee][Cc][Pp][Rr][Oo][Pp][Ss] {
	return (SASL_SECPROPS);
}

[Nn][Oo][Nn][Ee] {
	return (NONE);
}
[Nn][Oo][Pp][Ll][Aa][Ii][Nn] {
	return (NOPLAIN);
}
[Nn][Oo][Aa][Cc][Tt][Ii][Vv][Ee] {
	return (NOACTIVE);
}
[Nn][Oo][Dd][Ii][Cc][Tt] {
	return (NODICT);
}
[Nn][Oo][Aa][Nn][Oo][Nn][Yy][Mm][Oo][Uu][Ss] {
	return (NOANONYMOUS);
}
[Ff][Oo][Rr][Ww][Aa][Rr][Dd][Ss][Ee][Cc] {
	return (FORWARDSEC);
}
[Pp][Aa][Ss][Ss][Cc][Rr][Ee][Dd] {
	return (PASSCRED);
}
[Mm][Ii][Nn][Ss][Ss][Ff] {
	return (MINSSF);
}
[Mm][Aa][Xx][Ss][Ss][Ff] {
	return (MAXSSF);
}
[Mm][Aa][Xx][Bb][Uu][Ff][Ss][Ii][Zz][Ee] {
	return (MAXBUFSIZE);
}

[Gg][Ss][Ss][Aa][Pp][Ii]_[Ss][Ii][Gg][Nn] {
	return (GSSAPI_SIGN);
}
[Gg][Ss][Ss][Aa][Pp][Ii]_[Ee][Nn][Cc][Rr][Yy][Pp][Tt] {
	return (GSSAPI_ENCRYPT);
}
[Gg][Ss][Ss][Aa][Pp][Ii]_[Aa][Ll][Ll][Oo][Ww]_[Rr][Ee][Mm][Oo][Tt][Ee]_[Pp][Rr][Ii][Nn][Cc][Ii][Pp][Aa][Ll]	{
	return (GSSAPI_ALLOW_REMOTE_PRINCIPAL);
}

[Tt][Ll][Ss]_[Cc][Aa][Cc][Ee][Rr][Tt] {
	return (TLS_CACERT);
} 
[Tt][Ll][Ss]_[Cc][Aa][Cc][Ee][Rr][Tt][Dd][Ii][Rr] {
	return (TLS_CACERTDIR);
}
[Tt][Ll][Ss]_[Cc][Ee][Rr][Tt] {
	return (TLS_CERT);
}
[Tt][Ll][Ss]_[Kk][Ee][Yy] {
	return (TLS_KEY);
}
[Tt][Ll][Ss]_[Cc][Ii][Pp][Hh][Ee][Rr]_[Ss][Uu][Ii][Tt][Ee] {
	return (TLS_CIPHER_SUITE);
}
[Tt][Ll][Ss]_[Rr][Aa][Nn][Dd][Ff][Ii][Ll][Ee] {
	return (TLS_RANDFILE);
}
[Tt][Ll][Ss]_[Rr][Ee][Qq][Cc][Ee][Rr][Tt] {
	return (TLS_REQCERT);
}

[Aa][Ll][Ll][Oo][Ww] {
	return (ALLOW);
}
[Tt][Rr][Yy] {
	return (TRY);
}
[Dd][Ee][Mm][Aa][Nn][Dd] {
	return (DEMAND);
}
[Hh][Aa][Rr][Dd] {
	return (HARD);
}

[Tt][Ll][Ss]_[Cc][Rr][Ll][Cc][Hh][Ee][Cc][Kk] {
	return (TLS_CRLCHECK);
}

[Pp][Ee][Ee][Rr] {
	return (PEER);
}
[Aa][Ll][Ll] {
	return (ALL);
}

[Tt][Ll][Ss]_[Cc][Rr][Ll][Ff][Ii][Ll][Ee] {
	return (TLS_CRLFILE);
}

=	{ return (EQUAL); }
,	{ return (COMMA); }
\n	{ return (NEWLINE); }

[[:space:]\t]+$	{
	return (EMPTY);
}

([ \t]+)?(;|#)(.+)?$	{
	yylval.str = strdup(clean(yytext));
	return (COMMENT);
}

[0-9]+	{
	yylval.num = strtol(yytext, 0, 10);
	return (INTEGER);
}

ldap([si])?:\/\/[^#[:space:]]+	{
	yylval.str = strdup(clean(yytext));
	return (LDAP_SERVER);
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
