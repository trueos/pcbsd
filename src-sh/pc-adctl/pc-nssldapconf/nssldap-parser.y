%{

/*
 *	nss_ldap parser
 */

#include "pc-nssldapconf.h"

extern	int yylex(void);
extern	int yyparse();

extern	FILE *yyin, *yyout;
extern	unsigned int lineno;

extern	struct nssldap_entry_list nssldapconf;

%}

%union {
	char *str;
}


%token	NEWLINE
%token	SPACE
%token	EMPTY
%token	STRING
%token	WORD
%token	PAIR

%token	<str> WORD
%token	<str> PAIR
%token	<str> STRING
%token	<str> COMMENT

%%

file:
	|
	lines

lines:
	lines line |
	line 

line:
	newline |
	comment |
	pair |
	triplet |
	empty |
	space

space:
	SPACE

empty:
	EMPTY {
		struct nssldap_entry *ne = xalloc(sizeof(*ne));
		ne->type = NSSLDAP_ENTRY_NULL;
		TAILQ_INSERT_TAIL(&nssldapconf, ne, entries);
	}

pair:
	WORD SPACE WORD {
		struct nssldap_entry *ne = xalloc(sizeof(*ne));

		ne->type = NSSLDAP_ENTRY_PAIR;
		ne->nep_name = xstrdup(clean($1));
		ne->nep_value = xstrdup(clean($3));

		TAILQ_INSERT_TAIL(&nssldapconf, ne, entries);
	}

triplet:
	WORD SPACE WORD SPACE WORD {
		struct nssldap_entry *ne = xalloc(sizeof(*ne));

		ne->type = NSSLDAP_ENTRY_TRIPLET;
		ne->net_name = xstrdup(clean($1));
		ne->net_attr = xstrdup(clean($3));
		ne->net_value = xstrdup(clean($5));

		TAILQ_INSERT_TAIL(&nssldapconf, ne, entries);
	}

newline:
	NEWLINE {
		lineno++;
	}

comment:
	COMMENT {
		struct nssldap_entry *ne = xalloc(sizeof(*ne));

		ne->type = NSSLDAP_ENTRY_COMMENT;
		ne->nec_text = xstrdup(clean($1));

		TAILQ_INSERT_TAIL(&nssldapconf, ne, entries);
	}

%%

unsigned int lineno = 0;

void
yyerror(const char *str)
{
	fprintf(stderr, "%s:%d: error: %s\n", __FILE__, lineno, str);
}
