%{

/*
 *	samba parser
 */

#include "pc-sambaconf.h"

extern	int yylex(void);
extern	int yyparse();

extern	FILE *yyin, *yyout;
extern	unsigned int lineno;

extern	struct smb_entry_list fentries;

%}

%union {
	char *str;
}


%token	SOPEN
%token	SCLOSE
%token	SLASH
%token	EQUAL
%token	NEWLINE
%token	SECTION
%token	STRING

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
	sections |
	parameters

sections:
	sections section |
	section

section:
	sopen section_name sclose

sopen:
	SOPEN

sclose:
	SCLOSE

section_name:
	STRING
	{
		struct smb_entry *se = xalloc(sizeof(*se));
		se->type = SMB_ENTRY_SECTION;
		se->ses_name = xstrdup($1);
		TAILQ_INIT(&se->ses_parameters);

		TAILQ_INSERT_TAIL(&fentries, se, entries);
		curs = &se->ses;
		cur = se;
	}

parameters:
	parameters parameter |
	parameter

parameter:
	name equal value

name:
	STRING
	{
		struct smb_entry *se = xalloc(sizeof(*se));
		se->type = SMB_ENTRY_PARAMETERS;
		se->sep_name = xstrdup($1);
		se->sep_value = NULL;
		TAILQ_INSERT_TAIL(&curs->parameters, &se->sep, entries);

		TAILQ_INSERT_TAIL(&fentries, se, entries);
		cur = se;
	}

equal:
	EQUAL

value:
	STRING
	{
		cur->sep_value = xstrdup($1);
	}

newline:
	NEWLINE
	{   
		if (cur != NULL && cur != last) {
			last = cur;

		} else if (last == cur) {
			struct smb_entry *se = xalloc(sizeof(*se));
			se->type = SMB_ENTRY_NULL;
			TAILQ_INSERT_TAIL(&fentries, se, entries);
			cur = se;  
			last = cur;
		}
		lineno++;
	}

comment:
	COMMENT
	{
		struct smb_entry *se = xalloc(sizeof(*se));
		se->type = SMB_ENTRY_COMMENT;
		se->sec_text = xstrdup($1);

		TAILQ_INSERT_TAIL(&fentries, se, entries);
		cur = se;
	}

%%

unsigned int lineno = 0;
static struct smb_entry *cur = NULL;
static struct smb_entry *last = NULL; 
static struct smb_section *curs = NULL;

struct smb_entry *
new_smb_entry(void)
{
	return (NULL);
}

void
yyerror(const char *str)
{
	fprintf(stderr, "%s:%d: error: %s\n", __FILE__, lineno, str);
}
