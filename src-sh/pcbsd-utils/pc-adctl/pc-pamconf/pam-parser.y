%{

/*
 *	pam parser
 */

#include "pc-pamconf.h"

extern	int yylex(void);
extern	int yyparse();

extern	FILE *yyin, *yyout;
extern	unsigned int lineno;

extern	struct pam_entry_list pam_fentries;
extern	struct pam_config p_config;
extern	struct pam_entry *cur;

extern	void add_argument(struct pam_entry *pe, const char *arg);

%}

%union {
	char *str;
}

%token	AUTH
%token	ACCOUNT
%token	SESSION
%token	PASSWORD
%token	REQUIRED
%token	REQUISITE
%token	SUFFICIENT
%token	BINDING
%token	OPTIONAL
%token	INCLUDE
%token	NEWLINE

%token	<str> STRING
%token	<str> COMMENT

%%

file:
	|
	services

services:
	service services |
	service


service:
	comment |
	newline |
	service_no_args |
	service_with_args |
	service_with_include

newline:
	NEWLINE
	{
		if (cur != NULL && cur != last) {
			TAILQ_INSERT_TAIL(&pam_fentries, cur, fentries);
			last = cur;

		} else if (last == cur) {
			struct pam_entry *pe = new_pam_entry(-1);
			TAILQ_INSERT_TAIL(&pam_fentries, pe, fentries);
			cur = pe;
			last = cur;
		}

		lineno++;
	}

comment:
	COMMENT
	{
		struct pam_entry *pe = new_pam_entry(COMMENT);
		pe->type = PAM_ENTRY_COMMENT;
		pe->pec_text = xstrdup($1);
		cur = pe;
	}

service_no_args:
	facility control module
	{
		cur->type = PAM_ENTRY_SERVICE;
	}

service_with_args:
	facility control module arguments
	{
		cur->type = PAM_ENTRY_SERVICE;
	}

service_with_include:
	facility include service_name
	{
		cur->type = PAM_ENTRY_INCLUDE;
	}

facility:
	AUTH
	{
		struct pam_entry *pe = new_pam_entry(AUTH);
		cur = pe;
	}

	| ACCOUNT
	{
		struct pam_entry *pe = new_pam_entry(ACCOUNT);
		cur = pe;
	}

	| SESSION
	{
		struct pam_entry *pe = new_pam_entry(SESSION);
		cur = pe;
	}

	| PASSWORD
	{
		struct pam_entry *pe = new_pam_entry(PASSWORD);
		cur = pe;	
	}

control:
	REQUIRED
	{
		cur->pes_control = REQUIRED;
	}

	| REQUISITE
	{
		cur->pes_control = REQUISITE;
	}

	| SUFFICIENT
	{
		cur->pes_control = SUFFICIENT;
	}

	| BINDING
	{
		cur->pes_control = BINDING;
	}

	| OPTIONAL
	{
		cur->pes_control = OPTIONAL;
	}

module:
	STRING
	{
		cur->pes_module = xstrdup($1);
	}

arguments:
	arguments STRING
	{
		add_argument(cur, $2);
	}

	| STRING
	{
		add_argument(cur, $1);
	}

include:
	INCLUDE

service_name:
	STRING
	{
		cur->pei_service = xstrdup($1);
	}

%%

struct pam_entry *cur, *last;
unsigned int lineno = 0;

void
add_argument(struct pam_entry *pe, const char *arg)
{
	if (pe == NULL)
		return;

	if (pe->pes_arguments == NULL) {
		int i;

		pe->pes_narguments = 0;
		pe->pes_arguments = xalloc(sizeof(pe->pes_arguments) * PAM_ENTRY_ARGMAX);
		for (i = 0;i < PAM_ENTRY_ARGMAX;i++) {
			pe->pes_arguments[i] = NULL;
		}

		pe->pes_arguments[0] = xstrdup(arg);
		pe->pes_narguments++;

	} else {
		pe->pes_arguments[pe->pes_narguments++] = xstrdup(arg);
	}
}

void
yyerror(const char *str)
{
	fprintf(stderr, "%s:%d: error: %s\n", __FILE__, lineno, str);
}
