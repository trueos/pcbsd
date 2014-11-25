%{

/*
 *	NSS parser
 */

#include "pc-nssconf.h"

extern	int yylex(void);
extern	int yyparse();

extern	FILE *yyin, *yyout;
extern	unsigned int lineno;

extern	struct nss_entry_list fentries;

%}

%union {
	char *str;
}

%token	NEWLINE
%token	BOPEN
%token	BCLOSE
%token	EQUAL
%token	COLON

%token	SUCCESS
%token	NOTFOUND
%token	UNAVAIL
%token	TRYAGAIN

%token	RETURN
%token	CONTINUE

%token	GROUP
%token	GROUP_COMPAT
%token	HOSTS
%token	NETWORKS
%token	PASSWD
%token	PASSWD_COMPAT
%token	SHELLS
%token	SERVICES
%token	SERVICES_COMPAT
%token	RPC
%token	PROTOCOLS
%token	NETGROUP

%token	FILES
%token	DB
%token	DNS
%token	NIS
%token	COMPAT
%token	CACHE

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
	entries

entries:
	entries entry |
	entry

entry:
	{
		struct nss_entry *ne = xalloc(sizeof(*ne));
		ne->type = NSS_ENTRY_NULL;
		ne->ninfo = 0;
		TAILQ_INIT(&ne->info);
		TAILQ_INSERT_TAIL(&fentries, ne, entries);
		cindex = 0;
		cur = ne;
	}
	database colon sources_and_criteria

database:
	GROUP 			{ set_database(GROUP, NULL); }				|
	GROUP_COMPAT	{ set_database(GROUP_COMPAT, NULL); }		|
	HOSTS			{ set_database(HOSTS, NULL); }				|
	NETWORKS		{ set_database(NETWORKS, NULL); }			|
	PASSWD			{ set_database(PASSWD, NULL); }				|
	PASSWD_COMPAT	{ set_database(PASSWD_COMPAT, NULL); }		|
	SHELLS			{ set_database(SHELLS, NULL); }				|
	SERVICES		{ set_database(SERVICES, NULL); }			|
	SERVICES_COMPAT	{ set_database(SERVICES_COMPAT, NULL); }	|
	RPC				{ set_database(RPC, NULL); }				|
	PROTOCOLS		{ set_database(PROTOCOLS, NULL); }			|
	NETGROUP		{ set_database(NETGROUP, NULL); }			|
	STRING			{ set_database(STRING, $1); }

sources_and_criteria:
	sources_and_criteria sources |
	sources_and_criteria criteria_section |
	criteria_section |
	sources

sources:
	sources source |
	source

source:
	FILES 	{ add_source(FILES, NULL);	}	|
	DB 		{ add_source(DB, NULL);		}	|
	DNS 	{ add_source(DNS, NULL);	}	|
	NIS 	{ add_source(NIS, NULL);	}	|
	COMPAT	{ add_source(COMPAT, NULL); }	|
	CACHE 	{ add_source(CACHE, NULL);	}	|
	STRING	{ add_source(STRING, $1);	}

criteria_section:
	bopen criteria bclose

criteria:
	criteria criterion |
	criterion 

criterion:
	{
		struct nss_entry_info *nei = xalloc(sizeof(*nei));
		nei->type = NSS_ENTRY_INFO_CRITERIA;
		bzero(&nei->neic, sizeof(nei->neic));
		TAILQ_INSERT_TAIL(&cur->info, nei, entries);
		nei->index = cindex;
		cur->ninfo++;
		cindex++;
		curei = nei;
	}
	status equal action

bopen:
	BOPEN

bclose:
	 BCLOSE

status:
	SUCCESS		{ curei->neic_status = SUCCESS; }	|
	NOTFOUND	{ curei->neic_status = NOTFOUND; }	|
	UNAVAIL		{ curei->neic_status = UNAVAIL; }	|
	TRYAGAIN	{ curei->neic_status = TRYAGAIN; }

action:
	RETURN		{ curei->neic_action = RETURN; }	|
	CONTINUE	{ curei->neic_action = CONTINUE; }

equal:
	EQUAL

colon:
	COLON

database:
	STRING

newline:
	NEWLINE
	{
		lineno++;
	}

comment:
	COMMENT
	{
		struct nss_entry *ne = xalloc(sizeof(*ne));
		ne->type = NSS_ENTRY_COMMENT;
		ne->nec_comment = xstrdup($1);
		TAILQ_INSERT_TAIL(&fentries, ne, entries);
		cur = ne;
	}

%%

unsigned int lineno = 0;
unsigned int cindex = 0;
struct nss_entry *cur = NULL;
struct nss_entry_info *curei = NULL;

void
set_database(int type, const char *db)
{
	const char *str = NULL;

	cur->type = NSS_ENTRY_DATABASE;
	if (type == STRING && db != NULL) {
		cur->ned_database = xstrdup(db);

	}  else {
		str = database2str(type);
		cur->ned_database = xstrdup(str);
	}
}

void
add_source(int source, const char *str)
{
	const char *s = NULL;
	struct nss_entry_info *nei = xalloc(sizeof(*nei));

	nei->type = NSS_ENTRY_INFO_SOURCE;
	if (source == STRING && str != NULL) {
		nei->neis_source = xstrdup(str);

	} else {
		s = source2str(source);
		nei->neis_source = xstrdup(s);
	}

	nei->index = cindex;
	cur->ninfo++;
	cindex++;

	TAILQ_INSERT_TAIL(&cur->info, nei, entries);
}

void
yyerror(const char *str)
{
	fprintf(stderr, "%s:%d: error: %s\n", __FILE__, lineno, str);
}
