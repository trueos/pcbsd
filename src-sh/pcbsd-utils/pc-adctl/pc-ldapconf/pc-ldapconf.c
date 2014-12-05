/*
 *	pc-ldapconf.c
 *
 *	Configure ldap
 */

#include "pc-ldapconf.h"
#include "ldap-parser.tab.h"

#define	LDAP_OP_ADD		1
#define	LDAP_OP_MODIFY	2
#define	LDAP_OP_REMOVE	3

struct ldap_modification {
	TAILQ_ENTRY(ldap_modification) entries;
	char *name;
	char *value;
	int create;
	int op;
};
TAILQ_HEAD(ldap_modification_list, ldap_modification) modifications= 
	TAILQ_HEAD_INITIALIZER(modifications);

struct ldap_entry_list ldapconf = TAILQ_HEAD_INITIALIZER(ldapconf);

extern FILE *yyin, *yyout;
extern int yyparse();

void *
xalloc(size_t size)
{
	char *ptr = malloc(size);
	if (ptr == NULL)
		err(EX_OSERR, NULL);

	return (ptr);
}

void
_xfree(char **ptr)
{
	if (ptr != NULL) {
		free(*ptr);
		*ptr = NULL;
	}
}

char *
xstrdup(const char *str)
{
	char *ptr;

	if (str == NULL)
		return (NULL);

	ptr = strdup(str);
	if (ptr == NULL)
		err(EX_OSERR, NULL);

	return (ptr);
}

char *
clean(char *str)
{
	char *ptr;
 
	if (str == NULL)
		return (NULL);

	while (isspace(*str))
		str++;
	ptr = &str[strlen(str) - 1];
	while (isspace(*ptr)) {
		*ptr = 0;
		ptr--;
	}

	return (str);
}

static int
ldap_entry_free(struct ldap_entry **le_pptr)
{
	struct ldap_entry *le;

	if (le_pptr == NULL)
		return (-1);

	le = *le_pptr;
	switch (le->type) {

		case LDAP_ENTRY_URI_LIST: {
			struct ldap_uri *lu, *lutmp;
			struct ldap_uri_list *lul = le->entry;

			TAILQ_FOREACH_SAFE(lu, lul, entries, lutmp) {
				TAILQ_REMOVE(lul, lu, entries);
				xfree(&lu->uri);
				xfree(&lu);
			}

			break;
		}

		case LDAP_ENTRY_HOST_LIST: {
			struct ldap_host *lh, *lhtmp;
			struct ldap_host_list *lhl = le->entry;

			TAILQ_FOREACH_SAFE(lh, lhl, entries, lhtmp) {
				TAILQ_REMOVE(lhl, lh, entries);
				xfree(&lh->host);
				xfree(&lh);
			}

			break;
		}

		case LDAP_ENTRY_SASL_SECPROPS_LIST: {
			struct sasl_secprops *ss, *sstmp;
			struct sasl_secprops_list *lsl = le->entry;

			TAILQ_FOREACH_SAFE(ss, lsl, entries, sstmp) {
				TAILQ_REMOVE(lsl, ss, entries);
				xfree(&ss);
			}

			break;
		}

		case LDAP_ENTRY_COMMENT:
			xfree(&((struct ldap_comment *)le->entry)->text);
			break;

		case BASE:
			xfree(&((struct ldap_base *)le->entry)->base);
			break;

		case BINDDN:
			xfree(&((struct ldap_binddn *)le->entry)->dn);
			break;

		case SASL_MECH:
			xfree(&((struct sasl_mech *)le->entry)->mechanism);
			break;

		case SASL_REALM:
			xfree(&((struct sasl_realm *)le->entry)->realm);
			break;

		case SASL_AUTHCID:
			xfree(&((struct sasl_authcid *)le->entry)->authcid);
			break;

		case SASL_AUTHZID:
			xfree(&((struct sasl_authzid *)le->entry)->authzid);
			break;

		case SASL_SECPROPS: {
			/* XXX get back to this later XXX */
			break;
		}

		case TLS_CACERT:
			xfree(&((struct tls_cacert *)le->entry)->filename);
			break;

		case TLS_CACERTDIR:
			xfree(&((struct tls_cacertdir *)le->entry)->path);
			break;

		case TLS_CERT:
			xfree(&((struct tls_cert *)le->entry)->filename);
			break;

		case TLS_KEY:
			xfree(&((struct tls_key *)le->entry)->filename);
			break;

		case TLS_CIPHER_SUITE:
			xfree(&((struct tls_cipher_suite *)le->entry)->cipher_suite_spec);
			break;

		case TLS_RANDFILE:
			xfree(&((struct tls_randfile *)le->entry)->filename);
			break;

		case TLS_CRLFILE:
			xfree(&((struct tls_crlfile *)le->entry)->filename);
			break;
	}

	xfree(&le->entry);
	xfree(&le);
	*le_pptr = NULL;

	return (0);
}

static int
add_modification(const char *m, int create)
{
	char *tmp, *save, *ptr;
	struct ldap_modification *lm;

	if (m == NULL)
		return (-1);

	lm = xalloc(sizeof(*lm));
	switch (m[0]) {
		case '+':
			lm->op = LDAP_OP_ADD;
			lm->create = 1;
			m += 1;
			break;

		case '-':
			lm->op = LDAP_OP_REMOVE;
			lm->create = 0;
			m += 1;
			break;

		case '^':
			lm->op = LDAP_OP_MODIFY;
			lm->create = create;
			m += 1;
			break;

		default:
			xfree(&lm);
			return (-1);
	}

	ptr = xstrdup(m);
	save = ptr;

	tmp = strsep(&ptr, "=");
	if (tmp == NULL || tmp[0] == 0) {
		xfree(&lm);
		return (-1);
	}

	if (tmp != NULL)
		lm->name = xstrdup(clean(tmp));
	if (ptr != NULL)
		lm->value = xstrdup(clean(ptr));

	TAILQ_INSERT_TAIL(&modifications, lm, entries);

	xfree(&save);
	return (0);
}

static struct ldap_entry *
option2entry(unsigned long type, const char *value)
{
	struct ldap_entry *le = xalloc(sizeof(*le));

	le->type = type;

	switch (type) {
		case URI: {
			struct ldap_uri *lu = xalloc(sizeof(*lu));

			lu->uri = xstrdup(value);
			le->entry = lu;

			break;
		}

		case BASE: {
			struct ldap_base *lb = xalloc(sizeof(*lb));

			lb->base = xstrdup(value);
			le->entry = lb;

			break;
		}

		case BINDDN: {
			struct ldap_binddn *lb = xalloc(sizeof(*lb));

			lb->dn = xstrdup(value);
			le->entry = lb;

			break;
		}

		case DEREF: {
			struct ldap_deref *ld = xalloc(sizeof(*ld));

			ld->when = str2deref(value, NEVER);
			le->entry = ld;

			break;
		}

		case HOST: {
			struct ldap_host *lh = xalloc(sizeof(*lh));

			lh->host = xstrdup(value);
			le->entry = lh;

			break;
		}

		case NETWORK_TIMEOUT: {
			struct ldap_network_timeout *lnt = xalloc(sizeof(*lnt));

			lnt->timeout = strtol(value, 0, 10);
			le->entry = lnt;

			break;
		}

		case PORT: {
			struct ldap_port *lp = xalloc(sizeof(*lp));

			lp->port = strtol(value, 0, 10);
			le->entry = lp;

			break;
		}

		case REFERRALS: {
			struct ldap_referrals *lr = xalloc(sizeof(*lr));

			lr->option = str2bool(value, FALSE);
			le->entry = lr;

			break;
		}

		case SIZELIMIT: {
			struct ldap_sizelimit *ls = xalloc(sizeof(*ls));

			ls->sizelimit = strtol(value, 0, 10);
			le->entry = ls;

			break;
		}

		case TIMELIMIT: {
			struct ldap_timelimit *lt = xalloc(sizeof(*lt));

			lt->timelimit = strtol(value, 0, 10);
			le->entry = lt;

			break;
		}

		case TIMEOUT: {
			struct ldap_timeout *lt = xalloc(sizeof(*lt));

			lt->timeout = strtol(value, 0, 10);
			le->entry = lt;

			break;
		}

		case SASL_MECH: {
			struct sasl_mech *sm = xalloc(sizeof(*sm));

			sm->mechanism = xstrdup(value);
			le->entry = sm;

			break;
		}

		case SASL_REALM: {
			struct sasl_realm *sr = xalloc(sizeof(*sr));

			sr->realm = xstrdup(value);
			le->entry = sr;

			break;
		}

		case SASL_AUTHCID: {
			struct sasl_authcid *sa = xalloc(sizeof(*sa));

			sa->authcid = xstrdup(value);
			le->entry = sa;

			break;
		}

		case SASL_AUTHZID: {
			struct sasl_authzid *sa = xalloc(sizeof(*sa));

			sa->authzid = xstrdup(value);
			le->entry = sa;

			break;
		}

		case SASL_SECPROPS: {
			char *ptr, *str, *tmp, *save;
			struct sasl_secprops *ss = xalloc(sizeof(*ss));

			ptr = xstrdup(value);
			save = ptr;

			str = strsep(&tmp, "=");
			ss->properties = str2properties(tmp, NONE);
			ss->factor = -1;

			if (tmp != NULL)
				ss->factor = strtol(tmp, 0, 10);

			le->entry = ss;
			xfree(&save);

			break;
		}

		case GSSAPI_SIGN: {
			struct gssapi_sign *gs = xalloc(sizeof(*gs));

			gs->option = str2bool(value, FALSE);
			le->entry = gs;

			break;
		}

		case GSSAPI_ENCRYPT: {
			struct gssapi_encrypt *ge = xalloc(sizeof(*ge));

			ge->option = str2bool(value, FALSE);
			le->entry = ge;

			break;
		}

		case GSSAPI_ALLOW_REMOTE_PRINCIPAL: {
			struct gssapi_allow_remote_principal *garp = xalloc(sizeof(*garp));

			garp->option = str2bool(value, FALSE);
			le->entry = garp;

			break;
		}

		case TLS_CACERT: {
			struct tls_cacert *tc = xalloc(sizeof(*tc));

			tc->filename = xstrdup(value);
			le->entry = tc;

			break;
		}

		case TLS_CACERTDIR: {
			struct tls_cacertdir *tc = xalloc(sizeof(*tc));

			tc->path = xstrdup(value);
			le->entry = tc;

			break;
		}

		case TLS_CERT: {
			struct tls_cert *tc = xalloc(sizeof(*tc));

			tc->filename = xstrdup(value);
			le->entry = tc;

			break;
		}

		case TLS_KEY: {
			struct tls_key *tk = xalloc(sizeof(*tk));

			tk->filename = xstrdup(value);
			le->entry = tk;

			break;
		}

		case TLS_CIPHER_SUITE: {
			struct tls_cipher_suite *tcs = xalloc(sizeof(*tcs));

			tcs->cipher_suite_spec = xstrdup(value);
			le->entry = tcs;

			break;
		}

		case TLS_RANDFILE: {
			struct tls_randfile *tr = xalloc(sizeof(*tr));

			tr->filename = xstrdup(value);
			le->entry = tr;

			break;
		}

		case TLS_REQCERT: {
			struct tls_reqcert *tr = xalloc(sizeof(*tr));

			tr->level = str2level(value, NEVER);
			le->entry = tr;

			break;
		}

		case TLS_CRLCHECK: {
			struct tls_crlcheck *tc = xalloc(sizeof(*tc));

			tc->level = str2level(value, NEVER);
			le->entry = tc;

			break;
		}

		case TLS_CRLFILE: {
			struct tls_crlfile *tc = xalloc(sizeof(*tc));

			tc->filename = xstrdup(value);
			le->entry = tc;

			break;
		}
	}

	return (le);
}

static struct ldap_entry *
ldap_create_uri_list(struct ldap_modification *lm)
{
	struct ldap_uri *lu;
	struct ldap_entry *le;
	struct ldap_uri_list *lul;

	if (lm == NULL)
		return (NULL);

	lu = xalloc(sizeof(*lu));
	lu->uri = xstrdup(lm->value);

	le = xalloc(sizeof(*le));

	lul = xalloc(sizeof(*lul));
	TAILQ_INIT(lul);

	le->type = LDAP_ENTRY_URI_LIST;
	le->entry = lul;

	TAILQ_INSERT_TAIL(lul, lu, entries);
	return (le);
}

static struct ldap_entry *
ldap_create_host_list(struct ldap_modification *lm)
{
	struct ldap_host *lh;
	struct ldap_entry *le;
	struct ldap_host_list *lhl;

	if (lm == NULL)
		return (NULL);

	lh = xalloc(sizeof(*lh));
	lh->host = xstrdup(lm->value);

	le = xalloc(sizeof(*le));

	lhl = xalloc(sizeof(*lhl));
	TAILQ_INIT(lhl);

	le->type = LDAP_ENTRY_HOST_LIST;
	le->entry = lhl;

	TAILQ_INSERT_TAIL(lhl, lh, entries);
	return (le);
}

static struct ldap_entry *
ldap_create_sasl_secprops_list(struct ldap_modification *lm)
{
	struct ldap_entry *le;
	struct sasl_secprops_list *lsl;
	char *ptr, *str, *tmp, *save;

	if (lm == NULL)
		return (NULL);

	lsl = xalloc(sizeof(*lsl));
	TAILQ_INIT(lsl);

	le->type = LDAP_ENTRY_SASL_SECPROPS_LIST;
	le->entry = lsl;

	ptr = xstrdup(lm->value);
	save = ptr;

	while ((tmp = strsep(&ptr, ",")) != NULL) {
		struct sasl_secprops *ss = xalloc(sizeof(*ss));

		str = strsep(&tmp, "=");
		ss->properties = str2properties(str, NONE);
		ss->factor = -1;

		if (tmp != NULL)
			ss->factor = strtol(tmp, 0, 10);

		TAILQ_INSERT_TAIL(lsl, ss, entries);
	}

	xfree(&save);
	return (le);
}

static int
ldap_op_add(struct ldap_modification *lm)
{
	int exists = 0;
	unsigned long type;
	struct ldap_entry *le, *letmp;

	if (lm == NULL)
		return (-1);

	type = str2type(lm->name);
	TAILQ_FOREACH_SAFE(le, &ldapconf, entries, letmp) {
		switch (le->type) {
			case LDAP_ENTRY_URI_LIST: {
				if (type == URI) {
					struct ldap_uri_list *lul = le->entry;
					struct ldap_entry *new = option2entry(URI, lm->value);
					struct ldap_uri *lu = new->entry;
					TAILQ_INSERT_TAIL(lul, lu, entries);
					exists = 1;
				}

				break;
			}

			case LDAP_ENTRY_HOST_LIST: {
				if (type == HOST) {
					struct ldap_host_list *lhl = le->entry;
					struct ldap_entry *new = option2entry(HOST, lm->value);
					struct ldap_host *lh = new->entry;
					TAILQ_INSERT_TAIL(lhl, lh, entries);
					exists = 1;
				}

				break;
			}

			case LDAP_ENTRY_SASL_SECPROPS_LIST: {
				if (type == SASL_SECPROPS) {
					struct sasl_secprops_list *lsl = le->entry;
					struct ldap_entry *new = option2entry(SASL_SECPROPS, lm->value);
					struct sasl_secprops *ss = new->entry;
					TAILQ_INSERT_TAIL(lsl, ss, entries);
					exists = 1;
				}

				break;
			}

			default:
				if (le->type == type)
					exists = 1;

				break;
		}
	}

	if (exists == 0) {
		switch (type) {
			case URI: {
				struct ldap_entry *new = ldap_create_uri_list(lm);
				TAILQ_INSERT_TAIL(&ldapconf, new, entries);
				break;
			}
			
			case HOST: {
				struct ldap_entry *new = ldap_create_host_list(lm);
				TAILQ_INSERT_TAIL(&ldapconf, new, entries);
				break;
			}

			case SASL_SECPROPS: {
				struct ldap_entry *new = ldap_create_sasl_secprops_list(lm);
				TAILQ_INSERT_TAIL(&ldapconf, new, entries);
				break;
			}

			default: {
				struct ldap_entry *new = option2entry(type, lm->value);
				TAILQ_INSERT_TAIL(&ldapconf, new, entries);
				break;
			}
		}
	}

	return (0);
}

static int
ldap_op_modify(struct ldap_modification *lm)
{
	int exists = 0;
	unsigned long type;
	struct ldap_entry *le, *letmp;

	if (lm == NULL)
		return (-1);

	type = str2type(lm->name);
	TAILQ_FOREACH_SAFE(le, &ldapconf, entries, letmp) {
		switch (le->type) {
			case LDAP_ENTRY_URI_LIST: {
				if (type == URI) {
					struct ldap_uri *lu, *lutmp;
					struct ldap_uri_list *lul = le->entry;
					struct ldap_entry *new = option2entry(type, lm->value);

					TAILQ_FOREACH_SAFE(lu, lul, entries, lutmp) {
						TAILQ_REMOVE(lul, lu, entries);
						xfree(&lu->uri);
						xfree(&lu);
					}

					lu = new->entry;
					xfree(&new);

					TAILQ_INSERT_TAIL(lul, lu, entries);
					exists = 1;
				}
				break;
			}

			case LDAP_ENTRY_HOST_LIST: {
				if (type == HOST) {
					struct ldap_host *lh, *lhtmp;
					struct ldap_host_list *lhl = le->entry;
					struct ldap_entry *new = option2entry(type, lm->value);

					TAILQ_FOREACH_SAFE(lh, lhl, entries, lhtmp) {
						TAILQ_REMOVE(lhl, lh, entries);
						xfree(&lh->host);
						xfree(&lh);
					}

					lh = new->entry;
					xfree(&new);

					TAILQ_INSERT_TAIL(lhl, lh, entries);
					exists = 1;
				}
				break;
			}

			case LDAP_ENTRY_SASL_SECPROPS_LIST: {
				if (type == SASL_SECPROPS) {
					struct sasl_secprops *ss, *sstmp;
					struct sasl_secprops_list *lsl = le->entry;
					struct ldap_entry *new = option2entry(type, lm->value);

					TAILQ_FOREACH_SAFE(ss, lsl, entries, sstmp) {
						TAILQ_REMOVE(lsl, ss, entries);
						xfree(&ss);
					}

					ss = new->entry;
					xfree(&new);

					TAILQ_INSERT_TAIL(lsl, ss, entries);
					exists = 1;
				}
				break;
			}

			default:
				if (le->type == type) {
					struct ldap_entry *new = option2entry(le->type, lm->value);

					TAILQ_INSERT_AFTER(&ldapconf, le, new, entries);
					TAILQ_REMOVE(&ldapconf, le, entries);
					ldap_entry_free(&le);

					exists = 1;
				}

				break;
		}
	}

	if (exists == 0 && lm->create > 0)
		return (ldap_op_add(lm));

	return (0);
}

static int
ldap_op_remove(struct ldap_modification *lm)
{
	unsigned long type;
	struct ldap_entry *le, *letmp;

	if (lm == NULL)
		return (-1);

	type = str2type(lm->name);
	TAILQ_FOREACH_SAFE(le, &ldapconf, entries, letmp) {
		switch (le->type) {
			case LDAP_ENTRY_URI_LIST: {
				if (type == URI) {
					long count = 0;
					struct ldap_uri *lu, *lutmp;
					struct ldap_uri_list *lul = le->entry;

					TAILQ_FOREACH(lu, lul, entries)
						count++;

					if (lm->value == NULL) {
						TAILQ_FOREACH_SAFE(lu, lul, entries, lutmp) {
							TAILQ_REMOVE(lul, lu, entries);
							xfree(&lu->uri);
							xfree(&lu);
							count--;
						}

					} else {
						TAILQ_FOREACH_SAFE(lu, lul, entries, lutmp) {
							if (strcasecmp(lu->uri, lm->value) == 0) {
								TAILQ_REMOVE(lul, lu, entries);
								xfree(&lu->uri);
								xfree(&lu);
								count--;
								break;
							}
						}
					}

					if (count <= 0) {
						TAILQ_REMOVE(&ldapconf, le, entries);
						xfree(&le->entry);
						xfree(&le);
					}
				}

				break;
			}

			case LDAP_ENTRY_HOST_LIST: {
				if (type == HOST) {
					long count = 0;
					struct ldap_host *lh, *lhtmp;
					struct ldap_host_list *lhl = le->entry;

					TAILQ_FOREACH(lh, lhl, entries)
						count++;

					if (lm->value == NULL) {
						TAILQ_FOREACH_SAFE(lh, lhl, entries, lhtmp) {
							TAILQ_REMOVE(lhl, lh, entries);
							xfree(&lh->host);
							xfree(&lh);
							count--;
						}

					} else {
						TAILQ_FOREACH_SAFE(lh, lhl, entries, lhtmp) {
							if (strcasecmp(lh->host, lm->value) == 0) {
								TAILQ_REMOVE(lhl, lh, entries);
								xfree(&lh->host);
								xfree(&lh);
								count--;
								break;
							}
						}
					}

					if (count <= 0) {
						TAILQ_REMOVE(&ldapconf, le, entries);
						xfree(&le->entry);
						xfree(&le);
					}
				}

				break;
			}

			case LDAP_ENTRY_SASL_SECPROPS_LIST: {
				if (type == SASL_SECPROPS) {
					long count = 0;
					struct sasl_secprops *ss , *sstmp;
					struct sasl_secprops_list *lsl = le->entry;

					TAILQ_FOREACH(ss, lsl, entries)
						count++;

					if (lm->value == NULL) {
						TAILQ_FOREACH_SAFE(ss, lsl, entries, sstmp) {
							TAILQ_REMOVE(lsl, ss, entries);
							xfree(&ss);
							count--;
						}

					} else {
						unsigned long properties = str2properties(lm->value, NONE);

						TAILQ_FOREACH_SAFE(ss, lsl, entries, sstmp) {
							if (ss->properties == properties) {
								TAILQ_REMOVE(lsl, ss, entries);
								xfree(&ss);
								count--;
								break;
							}
						}
					}

					if (count <= 0) {
						TAILQ_REMOVE(&ldapconf, le, entries);
						xfree(&le->entry);
						xfree(&le);
					}
				}

				break;
			}

			default: {
				if (le->type == type) {
					TAILQ_REMOVE(&ldapconf, le, entries);
					ldap_entry_free(&le);
				}

				break;
			}
		}
	}

	return (0);
}

static void
do_modifications(void)
{
	struct ldap_modification *lm, *lmtmp;

	TAILQ_FOREACH_SAFE(lm, &modifications, entries, lmtmp) {
		switch (lm->op) {
			case LDAP_OP_ADD:
				if (ldap_op_add(lm) < 0)
					warnx("ldap_op_add: returned -1\n");
				break;

			case LDAP_OP_MODIFY:
				if (ldap_op_modify(lm) < 0)
					warnx("ldap_op_modify: returned -1\n");
				break;

			case LDAP_OP_REMOVE:
				if (ldap_op_remove(lm) < 0)
					warnx("ldap_op_remove: returned -1\n");
				break;
		}

		TAILQ_REMOVE(&modifications, lm, entries);
		xfree(&lm->name);
		xfree(&lm->value);
		xfree(&lm);
	}
}

const char *
deref2str(unsigned long deref, const char *def)
{
	const char *str = def;

	switch (deref) {
		case NEVER:
			str = "never";
			break;

		case SEARCHING:
			str = "searching";
			break;

		case FINDING:
			str = "finding";
			break;

		case ALWAYS:
			str = "always";
			break;
	}

	return (str);
}

unsigned long
str2deref(const char *str, unsigned long def)
{
	unsigned long deref = def;

	if (str == NULL || str[0] == 0)
		return (def);

	if (strcasecmp(str, "never") == 0) {
		deref = NEVER;

	} else if (strcasecmp(str, "searching") == 0) {
		deref = SEARCHING;

	} else if (strcasecmp(str, "finding") == 0) {
		deref = FINDING;

	} else if (strcasecmp(str, "always") == 0) {
		deref = ALWAYS;
	}

	return (deref);
}

const char *
bool2str(unsigned long b, const char *def)
{
	const char *str = def;

	switch (b) {
		case ON:
			str = "on";
			break;

		case TRUE:
			str = "true";
			break;

		case YES:
			str = "yes";
			break;

		case OFF:
			str = "off";
			break;

		case FALSE:
			str = "false";
			break;

		case NO:
			str = "no";
			break;
	}

	return (str);
}

unsigned long
str2bool(const char *str, unsigned long def)
{
	unsigned long b = def;

	if (str == NULL || str[0] == 0)
		return (def);

	if (strcasecmp(str, "on") == 0) {
		b = ON;

	} else if (strcasecmp(str, "true") == 0) {
		b = TRUE;

	} else if (strcasecmp(str, "yes") == 0) {
		b = YES;

	} else if (strcasecmp(str, "off") == 0) {
		b = OFF;

	} else if (strcasecmp(str, "false") == 0) {
		b = FALSE;

	} else if (strcasecmp(str, "no") == 0) {
		b = NO;
	}

	return (b);
}


const char *
level2str(unsigned long level, const char *def)
{
	const char *str = def;

	switch (level) {
		case NEVER:
			str = "never";
			break;

		case ALLOW:
			str = "allow";
			break;

		case TRY:
			str = "try";
			break;

		case DEMAND:
			str = "demand";
			break;

		case HARD:
			str = "hard";
			break;


		case NONE:
			str = "none";
			break;

		case PEER:
			str = "peer";
			break;

		case ALL:
			str = "all";
			break;
	}

	return (str);
}

unsigned long
str2level(const char *str, unsigned long def)
{
	unsigned long level = def;

	if (str == NULL || str[0] == 0)
		return (def);

	if (strcasecmp(str, "never") == 0) {
		level = NEVER;

	} else if (strcasecmp(str, "allow") == 0) {
		level = ALLOW;

	} else if (strcasecmp(str, "try") == 0) {
		level = TRY;

	} else if (strcasecmp(str, "demand") == 0) {
		level = DEMAND;

	} else if (strcasecmp(str, "hard") == 0) {
		level = HARD;

	} else if (strcasecmp(str, "none") == 0) {
		level = NONE;

	} else if (strcasecmp(str, "peer") == 0) {
		level = PEER;

	} else if (strcasecmp(str, "all") == 0) {
		level = ALL;
	}

	return (level);
}

const char *
properties2str(unsigned long properties, const char *def)
{
	const char *str = def;

	switch (properties) {
		case NONE:
			str = "none";
			break;

		case NOPLAIN:
			str = "noplain";
			break;

		case NOACTIVE:
			str = "noactive";
			break;

		case NODICT:
			str = "nodict";
			break;

		case NOANONYMOUS:
			str = "noanonymous";
			break;

		case FORWARDSEC:
			str = "forwardsec";
			break;

		case PASSCRED:
			str = "passcred";
			break;

		case MINSSF:
			str = "minssf";
			break;

		case MAXSSF:
			str = "maxssf";
			break;

		case MAXBUFSIZE:
			str = "maxbufsize";
			break;
	}

	return (str);
}

unsigned long
str2properties(const char *str, unsigned long def)
{
	unsigned long properties = def;

	if (str == NULL || str[0] == 0)
		return (def);

	if (strcasecmp(str, "none") == 0) {
		properties = NONE;

	} else if (strcasecmp(str, "noplain") == 0) {
		properties = NOPLAIN;

	} else if (strcasecmp(str, "noactive") == 0) {
		properties = NOACTIVE;

	} else if (strcasecmp(str, "nodict") == 0) {
		properties = NODICT;

	} else if (strcasecmp(str, "noanonymous") == 0) {
		properties = NOANONYMOUS;

	} else if (strcasecmp(str, "forwardsec") == 0) {
		properties = FORWARDSEC;

	} else if (strcasecmp(str, "passcred") == 0) {
		properties = PASSCRED;

	} else if (strcasecmp(str, "minssf") == 0) {
		properties = MINSSF;

	} else if (strcasecmp(str, "maxssf") == 0) {
		properties = MAXSSF;

	} else if (strcasecmp(str, "maxbufsize") == 0) {
		properties = MAXBUFSIZE;
	}

	return (properties);
}

unsigned long
str2type(const char *name)
{
	unsigned long type = 0;

	if (name == NULL || name[0] == 0)
		return (0);

	if (strcasecmp(name, "URI") == 0) {
		type = URI;

	} else if (strcasecmp(name, "BASE") == 0) {
		type = BASE;

	} else if (strcasecmp(name, "BINDDN") == 0) {
		type = BINDDN;

	} else if (strcasecmp(name, "DEREF") == 0) {
		type = DEREF;

	} else if (strcasecmp(name, "HOST") == 0) {
		type = HOST;

	} else if (strcasecmp(name, "NETWORK_TIMEOUT") == 0) {
		type = NETWORK_TIMEOUT;

	} else if (strcasecmp(name, "PORT") == 0) {
		type = PORT;

	} else if (strcasecmp(name, "REFERRALS") == 0) {
		type = REFERRALS;

	} else if (strcasecmp(name, "SIZELIMIT") == 0) {
		type = SIZELIMIT;

	} else if (strcasecmp(name, "TIMELIMIT") == 0) {
		type = TIMELIMIT;

	} else if (strcasecmp(name, "TIMEOUT") == 0) {
		type = TIMEOUT;

	} else if (strcasecmp(name, "SASL_MECH") == 0) {
		type = SASL_MECH;

	} else if (strcasecmp(name, "SASL_REALM") == 0) {
		type = SASL_REALM;

	} else if (strcasecmp(name, "SASL_AUTHCID") == 0) {
		type = SASL_AUTHCID;

	} else if (strcasecmp(name, "SASL_AUTHZID") == 0) {
		type = SASL_AUTHZID;

	} else if (strcasecmp(name, "SASL_SECPROPS") == 0) {
		type = SASL_SECPROPS;

	} else if (strcasecmp(name, "GSSAPI_SIGN") == 0) {
		type = GSSAPI_SIGN;

	} else if (strcasecmp(name, "GSSAPI_ENCRYPT") == 0) {
		type = GSSAPI_ENCRYPT;

	} else if (strcasecmp(name, "GSSAPI_ALLOW_REMOTE_PRINCIPAL") == 0) {
		type = GSSAPI_ALLOW_REMOTE_PRINCIPAL;

	} else if (strcasecmp(name, "TLS_CACERT") == 0) {
		type = TLS_CACERT;

	} else if (strcasecmp(name, "TLS_CACERTDIR") == 0) {
		type = TLS_CACERTDIR;

	} else if (strcasecmp(name, "TLS_CERT") == 0) {
		type = TLS_CERT;

	} else if (strcasecmp(name, "TLS_KEY") == 0) {
		type = TLS_KEY;

	} else if (strcasecmp(name, "TLS_CIPHER_SUITE") == 0) {
		type = TLS_CIPHER_SUITE;

	} else if (strcasecmp(name, "TLS_RANDFILE") == 0) {
		type = TLS_RANDFILE;

	} else if (strcasecmp(name, "TLS_REQCERT") == 0) {
		type = TLS_REQCERT;

	} else if (strcasecmp(name, "TLS_CRLCHECK") == 0) {
		type = TLS_CRLCHECK;

	} else if (strcasecmp(name, "TLS_CRLFILE") == 0) {
		type = TLS_CRLFILE;
	}

	return (type);
}

const char *
type2str(unsigned long type)
{
	const char *str = NULL;

	switch (type) {
		case URI:
			str = "URI";
			break;

		case BASE:
			str = "BASE";
			break;

		case BINDDN:
			str = "BINDDN";
			break;

		case DEREF:
			str = "DEREF";
			break;

		case HOST:
			str = "HOST";
			break;

		case NETWORK_TIMEOUT:
			str = "NETWORK_TIMEOUT";
			break;

		case PORT:
			str = "PORT";
			break;

		case REFERRALS:
			str = "REFERRALS";
			break;

		case SIZELIMIT:
			str = "SIZELIMIT";
			break;

		case TIMELIMIT:
			str = "TIMELIMIT";
			break;

		case TIMEOUT:
			str = "TIMEOUT";
			break;
		
		case SASL_MECH:
			str = "SASL_MECH";
			break;

		case SASL_REALM:
			str = "SASL_REALM";
			break;

		case SASL_AUTHCID:
			str = "SASL_AUTHCID";
			break;

		case SASL_AUTHZID:
			str = "SASL_AUTHZID";
			break;

		case SASL_SECPROPS:
			str = "SASL_SECPROPS";
			break;

		case GSSAPI_SIGN:
			str = "GSSAPI_SIGN";
			break;

		case GSSAPI_ENCRYPT:
			str = "GSSAPI_ENCRYPT";
			break;

		case GSSAPI_ALLOW_REMOTE_PRINCIPAL:
			str = "GSSAPI_ALLOW_REMOTE_PRINCIPAL";
			break;

		case TLS_CACERT:
			str = "TLS_CACERT";
			break;

		case TLS_CACERTDIR:
			str = "TLS_CACERTDIR";
			break;

		case TLS_CERT:
			str = "TLS_CERT";
			break;

		case TLS_KEY:
			str = "TLS_KEY";
			break;

		case TLS_CIPHER_SUITE:
			str = "TLS_CIPHER_SUITE";
			break;

		case TLS_RANDFILE:
			str = "TLS_RANDFILE";
			break;

		case TLS_REQCERT:
			str = "TLS_REQCERT";
			break;

		case TLS_CRLCHECK:
			str = "TLS_CRLCHECK";
			break;

		case TLS_CRLFILE:
			str = "TLS_CRLFILE";
			break;
	} 

	return (str);
}

static int
write_ldap_conf(void)
{
	struct ldap_entry *le;

	rewind(yyout);
	TAILQ_FOREACH(le, &ldapconf, entries) {
		switch (le->type) {
			case LDAP_ENTRY_NULL:
				fprintf(yyout, "\n");
				break;

			case LDAP_ENTRY_URI_LIST: {
				struct ldap_uri *lu;
				struct ldap_uri_list *lul = le->entry;

				fprintf(yyout, "%s", type2str(URI));
				TAILQ_FOREACH(lu, lul, entries) {
					fprintf(yyout, "\t%s", lu->uri);
				}
				fprintf(yyout, "\n");
				break;
			}

			case LDAP_ENTRY_HOST_LIST: {
				struct ldap_host *lh;
				struct ldap_host_list *lhl = le->entry;

				fprintf(yyout, "%s", type2str(HOST));
				TAILQ_FOREACH(lh, lhl, entries) {
					fprintf(yyout, "\t%s", lh->host);
				}
				fprintf(yyout, "\n");
				break;
			}

			case LDAP_ENTRY_SASL_SECPROPS_LIST: {
				struct sasl_secprops *ss;
				struct sasl_secprops_list *lsl = le->entry;
				char buf[1024], *ptr;

				ptr = &buf[0];
				bzero(&buf, sizeof(buf));
				TAILQ_FOREACH(ss, lsl, entries) {
					ptr += snprintf(ptr, sizeof(buf) - (ptr - buf),
						"%s", properties2str(ss->properties, "none"));
					if (ss->factor > -1)
						ptr += snprintf(ptr, sizeof(buf) - (ptr - buf), "=%d", ss->factor);

					*ptr++ = ',';
					*ptr = 0;
				}
				if (*(ptr - 1) == ',')
					*(ptr - 1) = 0;

				fprintf(yyout, "%s\t%s\n", type2str(SASL_SECPROPS), buf);
				break;
			}

			case LDAP_ENTRY_COMMENT:
				fprintf(yyout, "%s\n", ((struct ldap_comment *)le->entry)->text);
				break;

			case BASE: {
				fprintf(yyout, "%s\t%s\n", type2str(le->type),
					((struct ldap_base *)le->entry)->base);
				break;
			}
			case BINDDN:
				fprintf(yyout, "%s\t%s\n", type2str(le->type),
					((struct ldap_binddn *)le->entry)->dn);
				break;
			case DEREF:
				fprintf(yyout, "%s\t%s\n", type2str(le->type),
					deref2str(((struct ldap_deref *)le->entry)->when, "always"));
				break;

			case NETWORK_TIMEOUT:
				fprintf(yyout, "%s\t%d\n", type2str(le->type),
					((struct ldap_network_timeout *)le->entry)->timeout);
				break;
			case PORT:
				fprintf(yyout, "%s\t%d\n", type2str(le->type),
					((struct ldap_port *)le->entry)->port);
				break;
			case REFERRALS:
				fprintf(yyout, "%s\t%s\n", type2str(le->type),
					bool2str(((struct ldap_referrals *)le->entry)->option, "on"));
				break;

			case SIZELIMIT:
				fprintf(yyout, "%s\t%d\n", type2str(le->type),
					((struct ldap_sizelimit *)le->entry)->sizelimit);
				break;
			case TIMELIMIT:
				fprintf(yyout, "%s\t%d\n", type2str(le->type),
					((struct ldap_timelimit *)le->entry)->timelimit);
				break;
			case TIMEOUT:
				fprintf(yyout, "%s\t%d\n", type2str(le->type),
					((struct ldap_timeout *)le->entry)->timeout);
				break;

			case SASL_MECH:
				fprintf(yyout, "%s\t%s\n", type2str(le->type),
					((struct sasl_mech *)le->entry)->mechanism);
				break;
			case SASL_REALM:
				fprintf(yyout, "%s\t%s\n", type2str(le->type),
					((struct sasl_realm *)le->entry)->realm);
				break;
			case SASL_AUTHCID:
				fprintf(yyout, "%s\t%s\n", type2str(le->type),
					((struct sasl_authcid *)le->entry)->authcid);
				break;
			case SASL_AUTHZID:
				fprintf(yyout, "%s\t%s\n", type2str(le->type),
					((struct sasl_authzid *)le->entry)->authzid);
				break;
			case SASL_SECPROPS: {
				/* XXX get back to this later XXX */
				break;
			}

			case GSSAPI_SIGN:
				fprintf(yyout, "%s\t%s\n", type2str(le->type),
					bool2str(((struct gssapi_sign *)le->entry)->option, "off"));
				break;
			case GSSAPI_ENCRYPT:
				fprintf(yyout, "%s\t%s\n", type2str(le->type),
					bool2str(((struct gssapi_encrypt *)le->entry)->option, "off"));
				break;
			case GSSAPI_ALLOW_REMOTE_PRINCIPAL:
				fprintf(yyout, "%s\t%s\n", type2str(le->type),
					bool2str(((struct gssapi_allow_remote_principal *)le->entry)->option, "off"));
				break;

			case TLS_CACERT:
				fprintf(yyout, "%s\t%s\n", type2str(le->type),
					((struct tls_cacert *)le->entry)->filename);
				break;
			case TLS_CACERTDIR:
				fprintf(yyout, "%s\t%s\n", type2str(le->type),
					((struct tls_cacertdir *)le->entry)->path);
				break;
			case TLS_CERT:
				fprintf(yyout, "%s\t%s\n", type2str(le->type),
					((struct tls_cert *)le->entry)->filename);
				break;
			case TLS_KEY:
				fprintf(yyout, "%s\t%s\n", type2str(le->type),
					((struct tls_key *)le->entry)->filename);
				break;
			case TLS_CIPHER_SUITE:
				fprintf(yyout, "%s\t%s\n", type2str(le->type),
					((struct tls_cipher_suite *)le->entry)->cipher_suite_spec);
				break;
			case TLS_RANDFILE:
				fprintf(yyout, "%s\t%s\n", type2str(le->type),
					((struct tls_randfile *)le->entry)->filename);
				break;
			case TLS_REQCERT:
				fprintf(yyout, "%s\t%s\n", type2str(le->type),
					level2str(((struct tls_reqcert *)le->entry)->level, "never"));
				break;
			case TLS_CRLCHECK:
				fprintf(yyout, "%s\t%s\n", type2str(le->type),
					level2str(((struct tls_crlcheck *)le->entry)->level, "none"));
				break;
			case TLS_CRLFILE:
				fprintf(yyout, "%s\t%s\n", type2str(le->type),
					((struct tls_crlfile *)le->entry)->filename);
				break;
		}
	}
}

static void
ldap_conf_free(void)
{
	struct ldap_entry *le, *letmp;

	TAILQ_FOREACH_SAFE(le, &ldapconf, entries, letmp) {
		TAILQ_REMOVE(&ldapconf, le, entries);
		ldap_entry_free(&le);
	}
}

static void
usage(void)
{
	fprintf(stderr,
		"Usage: ldapconf [options]\n"
		"Where options in:\n\n"
		"\t-f <input file>\n"
		"\t-o <output file>\n"
		"\t-m <(+|-|^)name=value>\n\n"
	);

	exit (1);
}

int
main(int argc, char **argv)
{
	int ch, create;
	char *infile, *outfile;

	if (argc <= 1)
		usage();

	create = 0;
	infile = outfile = NULL;
	while ((ch = getopt(argc, argv, "f:m:co:")) != -1) {
		switch (ch) {
			case 'f':
				xfree(&infile);
				infile = xstrdup(optarg);
				break;

			case 'c':
				create++;
				break;

			case 'm':
				add_modification(optarg, create);
				create = 0;
				break;

			case 'o':
				xfree(&outfile);
				outfile = xstrdup(optarg);
				break;

			case '?':
			default:
				usage();
		}
	}

	argc -= optind;
	argv += optind;

	if (infile != NULL) {
		yyin = fopen(infile, "r");
		if (yyin == NULL)
			err(EX_NOINPUT, "%s", infile);
	}

	if (outfile != NULL) {
		yyout = fopen(outfile, "w+");
		if (yyout == NULL)
			err(EX_NOINPUT, "%s", outfile);
	}

	if (infile != NULL)
		yyparse();
	if (yyout == NULL)
		yyout = stdout;

	do_modifications();
	write_ldap_conf();
	ldap_conf_free();

	xfree(&outfile);
	xfree(&infile);

	if (yyout != NULL)
		fclose(yyout);
	if (yyin != NULL)
		fclose(yyin);

	return (0);
}
