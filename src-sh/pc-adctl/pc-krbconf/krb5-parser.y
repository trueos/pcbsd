%{

/*
 *	krb5.conf parser
 */

#include "pc-krbconf.h"

extern	int yylex(void);
extern	int yyparse();

extern	FILE *yyin, *yyout;

extern	struct krb_entry_list fentries;
extern	struct krb_entry_list krbconf;

extern	int lineno;
extern	int tindex;
extern	int	*tstack;

#define	STOKENMAX	32768
#define	SECTION		1234
#define	NAME		1235
#define	VALUE		1236

struct _krbinfo {
	int type;

#define	TYPE_SECTION		1
#define	TYPE_BINDING		2

	struct krb_binding *kb;

#define	kb_bindings		kb->bindings
#define	kb_nbindings	kb->nbindings

	struct krb_section *ks;

#define	ks_bindings		ks->bindings
#define	ks_nbindings	ks->nbindings

};

extern	struct	_krbinfo bl;
extern	struct	_krbinfo ll;

%}

%union {
	char *str;
}

%token	S_OPEN
%token	S_CLOSE
%token	B_OPEN
%token	B_CLOSE
%token	EQ

%token	<str> STRING
%token	<str> COMMENT

%%

file:
	|
	lines |

lines:
	lines line |
	line

line:
	comments |
	sections

sections:
	section sections |
	section

comments:
	comments comment |
	comment

section:
	sopen section_name sclose bindings

sopen:
	S_OPEN

sclose:
	S_CLOSE

section_name:
	STRING
	{
		struct krb_entry *ke = xalloc(sizeof(*ke));
		struct krb_section *ks;

		ke->type = KRB_ENTRY_SECTION;
		ke->kes_name = xstrdup($1);
		ke->kes_nbindings = 0;

		TAILQ_INIT(&ke->kes_bindings);
		TAILQ_INSERT_TAIL(&krbconf, ke, entries);
		TAILQ_INSERT_TAIL(&fentries, ke, fentries);

		ks = &ke->kes;

		xfree(&tstack);
		tstack = xalloc(STOKENMAX);
		tindex = 0;
		tstack[tindex++] = SECTION;

		bl.type = TYPE_SECTION;
		bl.ks = ks;
		cs = ks;
		ce = ke;
	}

bindings:
	binding bindings |
	binding

binding:
	|
	comments |
	name eq value |
	name eq bopen bindings bclose

eq:
	EQ {
		tstack[tindex++] = EQ;
	}

bopen:
	B_OPEN {
		tstack[tindex++] = B_OPEN;
	}

bclose:
	B_CLOSE {
		struct krb_entry *ke = xalloc(sizeof(*ke));

		ke->type = KRB_ENTRY_BINDING_END;
		TAILQ_INSERT_TAIL(&fentries, ke, fentries);

		/*
		 *	Empty binding.
		 */
		if (tstack[tindex - 1] == B_OPEN)
			ce->type = KRB_ENTRY_BINDING_START;

		tstack[tindex++] = B_CLOSE;
	}

name:
	STRING
	{
		struct krb_entry *ke = xalloc(sizeof(*ke));

		ke->type = KRB_ENTRY_BINDING;
		ke->keb_name = xstrdup($1);
		ke->keb_value = NULL;
		ke->keb_nbindings = 0;
		TAILQ_INIT(&ke->keb_bindings);

		/*
		 *	Start of a section, set the binding list to a sectionk
		 *	binding list.
		 */
		if (tstack[tindex - 1] == SECTION) {
			bl.type = TYPE_SECTION;
			bl.ks = cs;

		/*
		 *	Don't change anything, insert into the current binding list.
		 */
		} else if (tstack[tindex - 1] == VALUE) {


		/*
		 *	Opening brace into a new binding. Save the current binding
		 *	list for now and set the binding list to the current binding
		 *	list.
		 */
		} else if (tstack[tindex - 1] == B_OPEN) {
			ce->type = KRB_ENTRY_BINDING_START;

			ll = bl;
			bl.type = TYPE_BINDING;
			bl.kb = cb;

		/*
		 *	Closing brace of a binding. Set the binding list to the list
		 *	to the binding list set when the brace opened.
		 */
		} else if (tstack[tindex - 1] == B_CLOSE) {
			bl = ll;

		} else {
			/* shouldn't get here */
		}

		if (bl.type == TYPE_SECTION) {
			TAILQ_INSERT_TAIL(&bl.ks_bindings, ke, entries);
			bl.ks_nbindings++;

		} else {
			TAILQ_INSERT_TAIL(&bl.kb_bindings, ke, entries);
			bl.kb_nbindings++;
		}

		TAILQ_INSERT_TAIL(&fentries, ke, fentries);

		cb = &ke->keb;
		ce = ke;
		tstack[tindex++] = NAME;
	}

value:
	STRING
	{
		cb->value = xstrdup($1);
		tstack[tindex++] = VALUE;
	}

comment:
	COMMENT
	{
		struct krb_entry *ke = xalloc(sizeof(*ke));

		ke->type = KRB_ENTRY_COMMENT;
		ke->kec_text = xstrdup($1);

		TAILQ_INSERT_TAIL(&fentries, ke, fentries);
	}

%%

int tindex;
int *tstack = NULL;

struct	krb_section *cs = NULL;
struct	krb_binding *cb = NULL;
struct	krb_entry *ce = NULL;

struct	_krbinfo bl;
struct	_krbinfo ll;

void
yyerror(const char *str)
{
	fprintf(stderr,"%s:%d error: %s\n", __FILE__, lineno, str);
}
