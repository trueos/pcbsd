/*
 *	pc-nssldapconf.c
 *
 *	Configure nss_ldap
 */

#include "pc-nssldapconf.h"
#include "nssldap-parser.tab.h"

#define	NSSLDAP_OP_ADD		1
#define	NSSLDAP_OP_MODIFY	2
#define	NSSLDAP_OP_REMOVE	3

struct nssldap_modification {
	TAILQ_ENTRY(nssldap_modification) entries;
	char *name;
	char *attr;
	char *value;
	int create;
	int op;
};
TAILQ_HEAD(nssldap_modification_list, nssldap_modification) modifications= 
	TAILQ_HEAD_INITIALIZER(modifications);

struct nssldap_entry_list nssldapconf = TAILQ_HEAD_INITIALIZER(nssldapconf);

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

/*
 * Swiped directly from FreeBSD and modified to fit my needs.
 */
char *
xstrsep(char **stringp, const char *delim)
{
	char *s;
	const char *spanp;
	int c, sc;
	char *tok;

	if ((s = *stringp) == NULL)
		return (NULL);

	for (tok = s;;) {
		c = *s++;
		spanp = delim;

		do {
			if ((sc = *spanp++) == c && *(s - 2) != '\\') {
				if (c == 0)
					s = NULL;
				else
					s[-1] = 0;

				*stringp = s;
				return (tok);
			}

		} while (sc != 0);
	}
}

char *
unescape(char *str)
{
	char *src, *dst;

	if (str == NULL)
		return (NULL);

	src = dst = str;
	while (*src != 0) {
		if (*src == '\\' && *(src + 1) != 0 && *(src + 1) == '=')
			src++;
		*dst++ = *src++;
	}

	*dst = 0;
	return (str);
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


#define	add_pair(m,c)		add_modification(m, 'p', c)
#define	add_triplet(m,c)	add_modification(m, 't', c)	

static int
add_modification(const char *m, int mt, int create)
{
	char *tmp, *save, *ptr;
	struct nssldap_modification *nm;

	if (m == NULL)
		return (-1);

	nm = xalloc(sizeof(*nm));
	nm->name = nm->attr = nm->value = NULL;

	switch (m[0]) {
		case '+':
			nm->op = NSSLDAP_OP_ADD;
			nm->create = 1;
			m += 1;
			break;

		case '-':
			nm->op = NSSLDAP_OP_REMOVE;
			nm->create = 0;
			m += 1;
			break;

		case '^':
			nm->op = NSSLDAP_OP_MODIFY;
			nm->create = create;
			m += 1;
			break;

		default:
			xfree(&nm);
			return (-1);
	}

	ptr = xstrdup(m);
	save = ptr;

	tmp = strsep(&ptr, "=");
	if (tmp == NULL || tmp[0] == 0) {
		xfree(&nm);
		return (-1);
	}

	if (tmp != NULL)
		nm->name = xstrdup(unescape(clean(tmp)));
	if (ptr != NULL) {
		switch (mt) {
			case 't': {
				tmp = xstrsep(&ptr, "=");
				if (ptr != NULL) {
					nm->attr = xstrdup(unescape(clean(tmp)));
					nm->value = xstrdup(unescape(clean(ptr)));
				}
			}

			case 'p':
			default:
				nm->value = xstrdup(unescape(clean(ptr)));
		}
	}

	TAILQ_INSERT_TAIL(&modifications, nm, entries);

	xfree(&save);
	return (0);
}

static int
nssldap_op_add(struct nssldap_modification *nm)
{
	int type;
	int exists = 0;
	struct nssldap_entry *ne;

	if (nm == NULL)
		return (-1);

	type = NSSLDAP_ENTRY_PAIR;	
	if (nm->attr != NULL)
		type = NSSLDAP_ENTRY_TRIPLET;

	TAILQ_FOREACH(ne, &nssldapconf, entries) {
		if (ne->type == type && type == NSSLDAP_ENTRY_PAIR &&
			strcasecmp(ne->nep_name, nm->name) == 0 &&
			strcasecmp(ne->nep_value, nm->value) == 0) {
			exists = 1;
			break;

		} else if (ne->type == type && type == NSSLDAP_ENTRY_TRIPLET &&
			strcasecmp(ne->net_name, nm->name) == 0 &&
			strcasecmp(ne->net_attr, nm->attr) == 0 &&
			strcasecmp(ne->net_value, nm->value) == 0) {
			exists = 1;
			break;
		}
	}

	if (exists == 0) {
		ne = xalloc(sizeof(*ne));

		ne->type = type;
		switch (type) {
			case NSSLDAP_ENTRY_PAIR: {
				ne->nep_name = xstrdup(nm->name);
				ne->nep_value = xstrdup(nm->value);
				break;
			}
				
			case NSSLDAP_ENTRY_TRIPLET: {
				ne->net_name = xstrdup(nm->name);
				ne->net_attr = xstrdup(nm->attr);
				ne->net_value = xstrdup(nm->value);
				break;
			}
		}

		TAILQ_INSERT_TAIL(&nssldapconf, ne, entries);
	}

	return (0);
}

static int
nssldap_op_modify(struct nssldap_modification *nm)
{
	int type;
	int exists = 0;
	struct nssldap_entry *ne, *netmp;

	if (nm == NULL)
		return (-1);

	type = NSSLDAP_ENTRY_PAIR;	
	if (nm->attr != NULL)
		type = NSSLDAP_ENTRY_TRIPLET;

	TAILQ_FOREACH_SAFE(ne, &nssldapconf, entries, netmp) {
		if (ne->type == type && type == NSSLDAP_ENTRY_PAIR &&
			strcasecmp(ne->nep_name, nm->name) == 0) {
			xfree(&ne->nep_value);
			ne->nep_value = xstrdup(nm->value);
			exists = 1;
			break;

		} else if (ne->type == type && type == NSSLDAP_ENTRY_TRIPLET &&
			strcasecmp(ne->net_name, nm->name) == 0 &&
			strcasecmp(ne->net_attr, nm->attr) == 0) {
			xfree(&ne->net_attr);
			xfree(&ne->net_value);
			ne->net_attr = xstrdup(nm->attr);
			ne->net_value = xstrdup(nm->value);
			exists = 1;
			break;
		}
	}

	if (exists == 0 && nm->create > 0)
		return (nssldap_op_add(nm));

	return (0);
}

static int
nssldap_op_remove(struct nssldap_modification *nm)
{
	int type;
	struct nssldap_entry *ne, *netmp;

	if (nm == NULL)
		return (-1);

	type = NSSLDAP_ENTRY_PAIR;	
	if (nm->attr != NULL)
		type = NSSLDAP_ENTRY_TRIPLET;

	TAILQ_FOREACH_SAFE(ne, &nssldapconf, entries, netmp) {
		if (ne->type == type && type == NSSLDAP_ENTRY_PAIR &&
			strcasecmp(ne->nep_name, nm->name) == 0) {
			TAILQ_REMOVE(&nssldapconf, ne, entries);
			xfree(&ne->nep_name);
			xfree(&ne->nep_value);
			xfree(&ne);
			break;

		} else if (ne->type == type && type == NSSLDAP_ENTRY_TRIPLET &&
			strcasecmp(ne->net_name, nm->name) == 0) {
			TAILQ_REMOVE(&nssldapconf, ne, entries);
			xfree(&ne->net_name);
			xfree(&ne->net_attr);
			xfree(&ne->net_value);
			xfree(&ne);
			break;
		}
	}

	return (0);
}

static void
do_modifications(void)
{
	struct nssldap_modification *nm, *nmtmp;

	TAILQ_FOREACH_SAFE(nm, &modifications, entries, nmtmp) {
		switch (nm->op) {
			case NSSLDAP_OP_ADD:
				if (nssldap_op_add(nm) < 0)
					warnx("nssldap_op_add: returned -1\n");
				break;

			case NSSLDAP_OP_MODIFY:
				if (nssldap_op_modify(nm) < 0)
					warnx("nssldap_op_modify: returned -1\n");
				break;

			case NSSLDAP_OP_REMOVE:
				if (nssldap_op_remove(nm) < 0)
					warnx("nssldap_op_remove: returned -1\n");
				break;
		}

		TAILQ_REMOVE(&modifications, nm, entries);
		xfree(&nm->name);
		xfree(&nm->value);
		xfree(&nm);
	}
}

static int
write_nss_ldap_conf(void)
{
	struct nssldap_entry *ne;

	TAILQ_FOREACH(ne, &nssldapconf, entries) {
		switch (ne->type) {
			case NSSLDAP_ENTRY_NULL:
				fprintf(yyout, "\n");
				break;

			case NSSLDAP_ENTRY_PAIR:
				fprintf(yyout, "%s\t%s\n", ne->nep_name, ne->nep_value);
				break;

			case NSSLDAP_ENTRY_TRIPLET:
				fprintf(yyout, "%s\t%s\t%s\n", ne->net_name,
					ne->net_attr, ne->net_value);
				break;

			case NSSLDAP_ENTRY_COMMENT:
				fprintf(yyout, "%s\n", ne->nec_text);
				break;
		}
	}
}

static void
nss_ldap_conf_free(void)
{
	struct nssldap_entry *ne, *netmp;

	TAILQ_FOREACH_SAFE(ne, &nssldapconf, entries, netmp) {
		TAILQ_REMOVE(&nssldapconf, ne, entries);

		switch (ne->type) {
			case NSSLDAP_ENTRY_NULL:
				break;

			case NSSLDAP_ENTRY_PAIR:
				xfree(&ne->nep_name);
				xfree(&ne->nep_value);
				break;

			case NSSLDAP_ENTRY_TRIPLET:
				xfree(&ne->net_name);
				xfree(&ne->net_attr);
				xfree(&ne->net_value);
				break;

			case NSSLDAP_ENTRY_COMMENT:
				xfree(&ne->nec_text);
				break;
		}

		xfree(&ne);
	}
}

static void
usage(void)
{
	fprintf(stderr,
		"Usage: nssldapconf [options]\n"
		"Where options in:\n\n"
		"\t-f <input file>\n"
		"\t-o <output file>\n"
		"\t-p <(+|-|^)name=value>\n"
		"\t-t <(+|-|^)name=attr=value>\n\n"
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
	while ((ch = getopt(argc, argv, "f:m:p:t:co:")) != -1) {
		switch (ch) {
			case 'f':
				xfree(&infile);
				infile = xstrdup(optarg);
				break;

			case 'c':
				create++;
				break;

			case 'm':
			case 'p':
				add_pair(optarg, create);
				create = 0;
				break;

			case 't':
				add_triplet(optarg, create);
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
	write_nss_ldap_conf();
	nss_ldap_conf_free();

	xfree(&outfile);
	xfree(&infile);

	if (yyout != NULL)
		fclose(yyout);
	if (yyin != NULL)
		fclose(yyin);

	return (0);
}
