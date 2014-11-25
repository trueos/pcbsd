%{

/*
 *	ldap parser
 */

#include "pc-ldapconf.h"

extern	int yylex(void);
extern	int yyparse();

extern	FILE *yyin, *yyout;
extern	unsigned int lineno;

extern	struct ldap_entry_list ldapconf;

static	void add_deref(unsigned long);
static	void add_referrals(unsigned long);

static	void sasl_secprops_parse(const char *);

static	void add_gssapi_sign(unsigned long);
static	void add_gssapi_encrypt(unsigned long);
static	void add_gssapi_allow_remote_principal(unsigned long);

static	void add_tls_reqcert(unsigned long);
static	void add_tls_crlcheck(unsigned long);

%}

%union {
	char *str;
	unsigned long num;
}

%token	URI
%token	BASE
%token	BINDDN
%token	DEREF

%token	NEVER
%token	SEARCHING
%token	FINDING
%token	ALWAYS

%token	HOST
%token	NETWORK_TIMEOUT
%token	PORT
%token	REFERRALS

%token	ON	
%token	TRUE	
%token	YES	
%token	OFF	
%token	FALSE
%token	NO	

%token	SIZELIMIT
%token	TIMELIMIT
%token	TIMEOUT

%token	SASL_MECH
%token	SASL_REALM
%token	SASL_AUTHCID
%token	SASL_AUTHZID
%token	SASL_SECPROPS

%token	NONE	
%token	NOPLAIN
%token	NOACTIVE
%token	NODICT
%token	NOANONYMOUS
%token	FORWARDSEC
%token	PASSCRED
%token	MINSSF
%token	MAXSSF
%token	MAXBUFSIZE

%token	GSSAPI_SIGN
%token	GSSAPI_ENCRYPT
%token	GSSAPI_ALLOW_REMOTE_PRINCIPAL

%token	TLS_CACERT
%token	TLS_CACERTDIR
%token	TLS_CERT
%token	TLS_KEY
%token	TLS_CIPHER_SUITE
%token	TLS_RANDFILE
%token	TLS_REQCERT

%token	ALLOW
%token	TRY
%token	DEMAND
%token	HARD

%token	TLS_CRLCHECK

%token	NONE
%token	PEER
%token	ALL

%token	TLS_CRLFILE

%token	EQUAL
%token	COMMA

%token	NEWLINE
%token	SPACE
%token	EMPTY
%token	WORD

%token	<str> WORD
%token	<str> COMMENT
%token	<str> LDAP_SERVER

%token	<num> INTEGER

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
	empty |
	space |
	options |
	sasl_options |
	gssapi_options |
	tls_options 

options:
	uri |
	base |
	binddn |
	deref |
	host |
	network_timeout |
	port |
	referrals |
	sizelimit |
	timelimit |
	timeout

sasl_options:
	sasl_mech |
	sasl_realm |
	sasl_authcid |
	sasl_authzid |
	sasl_secprops

gssapi_options:
	gssapi_sign |
	gssapi_encrypt |
	gssapi_allow_remote_principal

tls_options:
	tls_cacert |
	tls_cacertdir |
	tls_cert |
	tls_key |
	tls_cipher_suite |
	tls_randfile |
	tls_reqcert |
	tls_crlcheck |
	tls_crlfile

ldap_server:
	LDAP_SERVER
	{
		struct ldap_uri *lu = xalloc(sizeof(*lu));

		lu->uri = xstrdup(clean($1));
		if (lul == NULL) {
			struct ldap_entry *le = xalloc(sizeof(*le));

			lul = xalloc(sizeof(*lul));
			TAILQ_INIT(lul);

			le->type = LDAP_ENTRY_URI_LIST;
			le->entry = lul;

			TAILQ_INSERT_TAIL(lul, lu, entries);
			TAILQ_INSERT_TAIL(&ldapconf, le, entries);

		} else {
			TAILQ_INSERT_TAIL(lul, lu, entries);
		}
	}

ldap_servers:
	ldap_servers ldap_server |
	ldap_server

uri:
	URI ldap_servers

base:
	BASE WORD
	{
		struct ldap_entry *le = xalloc(sizeof(*le));
		struct ldap_base *lb = xalloc(sizeof(*lb));

		lb->base = xstrdup(clean($2));

		le->type = BASE;
		le->entry = lb;

		TAILQ_INSERT_TAIL(&ldapconf, le, entries);
	}

binddn:
	BINDDN WORD
	{
		struct ldap_entry *le = xalloc(sizeof(*le));
		struct ldap_binddn *lb = xalloc(sizeof(*lb));

		lb->dn = xstrdup(clean($2));

		le->type = BINDDN;
		le->entry = lb;

		TAILQ_INSERT_TAIL(&ldapconf, le, entries);
	}

deref:
	DEREF NEVER 	{ add_deref(NEVER);		} |
	DEREF SEARCHING { add_deref(SEARCHING);	} |
	DEREF FINDING 	{ add_deref(FINDING);	} |
	DEREF ALWAYS	{ add_deref(ALWAYS);	}

ldap_host:
	WORD
	{
		struct ldap_host *lh = xalloc(sizeof(*lh));

		lh->host = xstrdup(clean($1));

		if (lhl == NULL) {
			struct ldap_entry *le = xalloc(sizeof(*le));

			lhl = xalloc(sizeof(*lhl));
			TAILQ_INIT(lhl);

			le->type = LDAP_ENTRY_HOST_LIST;
			le->entry = lhl;

			TAILQ_INSERT_TAIL(lhl, lh, entries);
			TAILQ_INSERT_TAIL(&ldapconf, le, entries);

		} else {
			TAILQ_INSERT_TAIL(lhl, lh, entries);
		}
	}

ldap_hosts:
	ldap_hosts ldap_host |
	ldap_host

host:
	HOST ldap_hosts

network_timeout:
	NETWORK_TIMEOUT INTEGER
	{
		struct ldap_entry *le = xalloc(sizeof(*le));
		struct ldap_network_timeout *lnt = xalloc(sizeof(*lnt));

		lnt->timeout = $2;

		le->type = NETWORK_TIMEOUT;
		le->entry = lnt;

		TAILQ_INSERT_TAIL(&ldapconf, le, entries);
	}

port:
	PORT INTEGER
	{
		struct ldap_entry *le = xalloc(sizeof(*le));
		struct ldap_port *lp = xalloc(sizeof(*lp));

		lp->port = $2;

		le->type = PORT;
		le->entry = lp;

		TAILQ_INSERT_TAIL(&ldapconf, le, entries);
	}

referrals:
	REFERRALS ON 	{ add_referrals(ON);	} |
	REFERRALS TRUE	{ add_referrals(TRUE);	} |
	REFERRALS YES 	{ add_referrals(YES);	} |
	REFERRALS OFF 	{ add_referrals(OFF);	} |
	REFERRALS FALSE	{ add_referrals(FALSE);	} |
	REFERRALS NO	{ add_referrals(NO);	}

sizelimit:
	SIZELIMIT INTEGER
	{
		struct ldap_entry *le = xalloc(sizeof(*le));
		struct ldap_sizelimit *ls = xalloc(sizeof(*ls));

		ls->sizelimit = $2;

		le->type = SIZELIMIT;
		le->entry = ls;

		TAILQ_INSERT_TAIL(&ldapconf, le, entries);
	}

timelimit:
	TIMELIMIT INTEGER
	{
		struct ldap_entry *le = xalloc(sizeof(*le));
		struct ldap_timelimit *lt = xalloc(sizeof(*lt));

		lt->timelimit = $2;

		le->type = TIMELIMIT;
		le->entry = lt;

		TAILQ_INSERT_TAIL(&ldapconf, le, entries);
	}

timeout:
	TIMEOUT INTEGER
	{
		struct ldap_entry *le = xalloc(sizeof(*le));
		struct ldap_timeout *lt = xalloc(sizeof(*lt));

		lt->timeout = $2;

		le->type = TIMEOUT;
		le->entry = lt;

		TAILQ_INSERT_TAIL(&ldapconf, le, entries);
	}

sasl_mech:
	SASL_MECH WORD
	{
		struct ldap_entry *le = xalloc(sizeof(*le));
		struct sasl_mech *sm = xalloc(sizeof(*sm));

		sm->mechanism = xstrdup(clean($2));

		le->type = SASL_MECH;
		le->entry = sm;

		TAILQ_INSERT_TAIL(&ldapconf, le, entries);
	}

sasl_realm:
	SASL_REALM WORD
	{
		struct ldap_entry *le = xalloc(sizeof(*le));
		struct sasl_realm *sr = xalloc(sizeof(*sr));

		sr->realm = xstrdup(clean($2));

		le->type = SASL_REALM;
		le->entry = sr;

		TAILQ_INSERT_TAIL(&ldapconf, le, entries);
	}

sasl_authcid:
	SASL_AUTHCID WORD
	{
		struct ldap_entry *le = xalloc(sizeof(*le));
		struct sasl_authcid *sa = xalloc(sizeof(*sa));

		sa->authcid = xstrdup(clean($2));

		le->type = SASL_AUTHCID;
		le->entry = sa;

		TAILQ_INSERT_TAIL(&ldapconf, le, entries);
	}

sasl_authzid:
	SASL_AUTHZID WORD
	{
		struct ldap_entry *le = xalloc(sizeof(*le));
		struct sasl_authzid *sa = xalloc(sizeof(*sa));

		sa->authzid = xstrdup(clean($2));

		le->type = SASL_AUTHZID;
		le->entry = sa;

		TAILQ_INSERT_TAIL(&ldapconf, le, entries);
	}

sasl_secprops:
	SASL_SECPROPS WORD
	{
		sasl_secprops_parse($2);
	}

gssapi_sign:
	GSSAPI_SIGN ON		{ add_gssapi_sign(ON);		} |
	GSSAPI_SIGN TRUE	{ add_gssapi_sign(TRUE);	} |
	GSSAPI_SIGN YES		{ add_gssapi_sign(YES);		} |
	GSSAPI_SIGN OFF		{ add_gssapi_sign(OFF);		} |
	GSSAPI_SIGN FALSE	{ add_gssapi_sign(FALSE);	} |
	GSSAPI_SIGN NO		{ add_gssapi_sign(NO);		}

gssapi_encrypt:
	GSSAPI_ENCRYPT ON		{ add_gssapi_encrypt(ON);		} |
	GSSAPI_ENCRYPT TRUE		{ add_gssapi_encrypt(TRUE);		} |
	GSSAPI_ENCRYPT YES		{ add_gssapi_encrypt(YES);		} |
	GSSAPI_ENCRYPT OFF 		{ add_gssapi_encrypt(OFF);		} |
	GSSAPI_ENCRYPT FALSE 	{ add_gssapi_encrypt(FALSE);	} |
	GSSAPI_ENCRYPT NO		{ add_gssapi_encrypt(NO);		}

gssapi_allow_remote_principal:
	GSSAPI_ALLOW_REMOTE_PRINCIPAL ON 	{ add_gssapi_allow_remote_principal(ON); } |
	GSSAPI_ALLOW_REMOTE_PRINCIPAL TRUE 	{ add_gssapi_allow_remote_principal(TRUE); } |
	GSSAPI_ALLOW_REMOTE_PRINCIPAL YES 	{ add_gssapi_allow_remote_principal(YES); } |
	GSSAPI_ALLOW_REMOTE_PRINCIPAL OFF 	{ add_gssapi_allow_remote_principal(OFF); } |
	GSSAPI_ALLOW_REMOTE_PRINCIPAL FALSE	{ add_gssapi_allow_remote_principal(FALSE); } |
	GSSAPI_ALLOW_REMOTE_PRINCIPAL NO	{ add_gssapi_allow_remote_principal(NO); }

tls_cacert:
	TLS_CACERT WORD
	{
		struct ldap_entry *le = xalloc(sizeof(*le));
		struct tls_cacert *tc = xalloc(sizeof(*tc));

		tc->filename = xstrdup(clean($2));

		le->type = TLS_CACERT;
		le->entry = tc;

		TAILQ_INSERT_TAIL(&ldapconf, le, entries);
	}

tls_cacertdir:
	TLS_CACERTDIR WORD
	{
		struct ldap_entry *le = xalloc(sizeof(*le));
		struct tls_cacertdir *tc = xalloc(sizeof(*tc));

		tc->path = xstrdup(clean($2));

		le->type = TLS_CACERTDIR;
		le->entry = tc;

		TAILQ_INSERT_TAIL(&ldapconf, le, entries);
	}

tls_cert:
	TLS_CERT WORD
	{
		struct ldap_entry *le = xalloc(sizeof(*le));
		struct tls_cert *tc = xalloc(sizeof(*tc));

		tc->filename = xstrdup(clean($2));

		le->type = TLS_CERT;
		le->entry = tc;

		TAILQ_INSERT_TAIL(&ldapconf, le, entries);
	}

tls_key:
	TLS_KEY WORD
	{
		struct ldap_entry *le = xalloc(sizeof(*le));
		struct tls_key *tk = xalloc(sizeof(*tk));

		tk->filename = xstrdup(clean($2));

		le->type = TLS_KEY;
		le->entry = tk;

		TAILQ_INSERT_TAIL(&ldapconf, le, entries);
	}

tls_cipher_suite:
	TLS_CIPHER_SUITE WORD
	{
		struct ldap_entry *le = xalloc(sizeof(*le));
		struct tls_cipher_suite *tcs = xalloc(sizeof(*tcs));

		tcs->cipher_suite_spec = xstrdup(clean($2));

		le->type = TLS_CIPHER_SUITE;
		le->entry = tcs;

		TAILQ_INSERT_TAIL(&ldapconf, le, entries);
	}

tls_randfile:
	TLS_RANDFILE WORD
	{
		struct ldap_entry *le = xalloc(sizeof(*le));
		struct tls_randfile *tr = xalloc(sizeof(*tr));

		tr->filename = xstrdup(clean($2));

		le->type = TLS_RANDFILE;
		le->entry = tr;

		TAILQ_INSERT_TAIL(&ldapconf, le, entries);
	}

tls_reqcert:
	TLS_REQCERT NEVER 	{ add_tls_reqcert(NEVER);	} |
	TLS_REQCERT ALLOW 	{ add_tls_reqcert(ALLOW);	} |
	TLS_REQCERT TRY 	{ add_tls_reqcert(TRY);		} |
	TLS_REQCERT DEMAND	{ add_tls_reqcert(DEMAND);	} |
	TLS_REQCERT HARD	{ add_tls_reqcert(HARD);	} 

tls_crlcheck:
	TLS_CRLCHECK NONE	{ add_tls_crlcheck(NONE);	} |
	TLS_CRLCHECK PEER	{ add_tls_crlcheck(PEER);	} |
	TLS_CRLCHECK ALL	{ add_tls_crlcheck(ALL);	}

tls_crlfile:
	TLS_CRLFILE WORD
	{
		struct ldap_entry *le = xalloc(sizeof(*le));
		struct tls_crlfile *tc = xalloc(sizeof(*tc));

		tc->filename = xstrdup(clean($2));

		le->type = TLS_CRLFILE;
		le->entry = tc;

		TAILQ_INSERT_TAIL(&ldapconf, le, entries);
	}

space:
	SPACE

empty:
	EMPTY {
		struct ldap_entry *le = xalloc(sizeof(*le));

		le->type = LDAP_ENTRY_NULL;
		le->entry = NULL;

		TAILQ_INSERT_TAIL(&ldapconf, le, entries);
	}

newline:
	NEWLINE {
		lineno++;
	}

comment:
	COMMENT
	{
		struct ldap_entry *le = xalloc(sizeof(*le));
		struct ldap_comment *lc = xalloc(sizeof(*lc));

		lc->text = xstrdup($1);

		le->type = LDAP_ENTRY_COMMENT;
		le->entry = lc;

		TAILQ_INSERT_TAIL(&ldapconf, le, entries);
	}

%%

unsigned int lineno = 0;

static struct ldap_uri_list *lul = NULL;
static struct ldap_host_list *lhl = NULL;
static struct sasl_secprops_list *lsl = NULL;

static void
add_deref(unsigned long when)
{
	struct ldap_entry *le = xalloc(sizeof(*le));
	struct ldap_deref *ld = xalloc(sizeof(*ld));

	ld->when = when;

	le->type = DEREF;
	le->entry = ld;

	TAILQ_INSERT_TAIL(&ldapconf, le, entries);
}

static void
add_referrals(unsigned long option)
{
	struct ldap_entry *le = xalloc(sizeof(*le));
	struct ldap_referrals *lr = xalloc(sizeof(*lr));

	lr->option = option;

	le->type = REFERRALS;
	le->entry = lr;

	TAILQ_INSERT_TAIL(&ldapconf, le, entries);
}

static void
sasl_secprops_parse(const char *word)
{
	char *ptr, *str, *tmp, *save;

	if (lsl == NULL) {
		struct ldap_entry *le = xalloc(sizeof(*le));

		lsl = xalloc(sizeof(*lsl));
		TAILQ_INIT(lsl);

		le->type = LDAP_ENTRY_SASL_SECPROPS_LIST;
		le->entry = lsl;

		TAILQ_INSERT_TAIL(&ldapconf, le, entries);
	}

	ptr = xstrdup(word);
	save = ptr;

	while ((tmp = strsep(&ptr, ",")) != NULL) {
		struct ldap_entry *le = xalloc(sizeof(*le));
		struct sasl_secprops *ss = xalloc(sizeof(*ss));

		str = strsep(&tmp, "=");
		ss->properties = str2properties(str, NONE);
		ss->factor = -1;

		if (tmp != NULL)
			ss->factor = strtol(tmp, 0, 10);

		le->type = SASL_SECPROPS;
		le->entry = ss;

		TAILQ_INSERT_TAIL(lsl, ss, entries);
	}

	xfree(&save);
}

static void
add_gssapi_sign(unsigned long option)
{
	struct ldap_entry *le = xalloc(sizeof(*le));
	struct gssapi_sign *gs = xalloc(sizeof(*gs));

	gs->option = option;

	le->type = GSSAPI_SIGN;
	le->entry = gs;

	TAILQ_INSERT_TAIL(&ldapconf, le, entries);
}

static void
add_gssapi_encrypt(unsigned long option)
{
	struct ldap_entry *le = xalloc(sizeof(*le));
	struct gssapi_encrypt *ge = xalloc(sizeof(*ge));

	ge->option = option;

	le->type = GSSAPI_ENCRYPT;
	le->entry = ge;

	TAILQ_INSERT_TAIL(&ldapconf, le, entries);
} 

static void
add_gssapi_allow_remote_principal(unsigned long option)
{
	struct ldap_entry *le = xalloc(sizeof(*le));
	struct gssapi_allow_remote_principal *garp = xalloc(sizeof(*garp));

	garp->option = option;

	le->type = GSSAPI_ALLOW_REMOTE_PRINCIPAL;
	le->entry = garp;

	TAILQ_INSERT_TAIL(&ldapconf, le, entries);
}

static void
add_tls_reqcert(unsigned long level)
{
	struct ldap_entry *le = xalloc(sizeof(*le));
	struct tls_reqcert *tr = xalloc(sizeof(*tr));

	tr->level = level;

	le->type = TLS_REQCERT;
	le->entry = tr;

	TAILQ_INSERT_TAIL(&ldapconf, le, entries);
}

static void
add_tls_crlcheck(unsigned long level)
{
	struct ldap_entry *le = xalloc(sizeof(*le));
	struct tls_crlcheck *tc = xalloc(sizeof(*tc));

	tc->level = level;

	le->type = TLS_CRLCHECK;
	le->entry = tc;

	TAILQ_INSERT_TAIL(&ldapconf, le, entries);
}

void
yyerror(const char *str)
{
	fprintf(stderr, "%s:%d: error: %s\n", __FILE__, lineno, str);
}
