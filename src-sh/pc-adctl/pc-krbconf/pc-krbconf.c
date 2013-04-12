/*
 *	pc-krbconf.c
 *
 *	Configure kerberos
 *
 *	This can be better, but it works.
 */

#include "pc-krbconf.h"
#include "krb5-parser.tab.h"

#define	KRB_OP_ADD		1
#define	KRB_OP_MODIFY	2
#define	KRB_OP_REMOVE	3

struct krb_modification {
	TAILQ_ENTRY(krb_modification) entries;	
	char *modstr;
	int create;
	int op;
};
TAILQ_HEAD(krb_modification_list, krb_modification) modifications = 
	TAILQ_HEAD_INITIALIZER(modifications);

struct krb_entry_list fentries = TAILQ_HEAD_INITIALIZER(fentries);
struct krb_entry_list krbconf = TAILQ_HEAD_INITIALIZER(krbconf);

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

static void
usage(void)
{
	fprintf(stderr,
		"Usage: krbconf [options]\n"
		"Where options in:\n\n"
 		"\t-f <input file>\n"
		"\t-o <output file>\n"
 		"\t-m <(+|-|^)section.binding.name=value>\n\n"
	);

	exit(1);
}

static int
add_modification(const char *m, int create)
{
	struct krb_modification *km;

	if (m == NULL)
		return (-1);

	km = xalloc(sizeof(*km));
	km->create = create;

	switch (m[0]) {
		case '+':
			km->op = KRB_OP_ADD;
			km->create = 1;
			m += 1;
			break;

		case '-':
			km->op = KRB_OP_REMOVE;
			km->create = 0;
			m += 1;
			break;

		case '^':
			km->op = KRB_OP_MODIFY;
			m += 1;
			break;

		default:
			xfree(&km);
			return (-1);
	}

	km->modstr = xstrdup(m);
	TAILQ_INSERT_TAIL(&modifications, km, entries);

	return (0);
}


static void
do_indent(FILE *fp, int indent)
{
	int i = 0;

	for (i = 0;i < indent;i++)
		fprintf(fp, "\t");
}

static void
write_krb5_conf(void)
{
	int indent;
	int section;
	struct krb_entry *ke;

	indent = section = 0;
	TAILQ_FOREACH(ke, &fentries, fentries) {

		if (ke->type == KRB_ENTRY_SECTION) {
			fprintf(yyout, "\n");
			do_indent(yyout, 0);
			fprintf(yyout, "[%s]\n", ke->kes_name);
			section = 1;
			indent = 1;

		} else if (ke->type == KRB_ENTRY_BINDING_START) {
			if (section == 0)
				fprintf(yyout, "\n");

			do_indent(yyout, indent);
			fprintf(yyout, "%s = {\n", ke->keb_name);
			section = 0;
			indent++;

		} else if (ke->type == KRB_ENTRY_BINDING) {
			do_indent(yyout, indent);
			fprintf(yyout, "%s = %s\n", ke->keb_name, ke->keb_value);

		} else if (ke->type == KRB_ENTRY_BINDING_END) {
			indent--;
			do_indent(yyout, indent);
			fprintf(yyout, "}\n");

		} else if (ke->type == KRB_ENTRY_COMMENT) {
			do_indent(yyout, 0);
			fprintf(yyout, "%s\n", ke->kec_text);

		} else if (ke->type == KRB_ENTRY_NULL) {
			fprintf(yyout, "\n");
		}
	}

}


static void
krb5_bindings_unlink(struct krb_entry_list *el)
{
	struct krb_entry *ke, *ketmp;

	TAILQ_FOREACH_SAFE(ke, el, entries, ketmp) {
		if (ke->type == KRB_ENTRY_BINDING && ke->keb_nbindings > 0)
			krb5_bindings_unlink(&ke->keb_bindings);

		TAILQ_REMOVE(el, ke, entries);
	}
}

static void
krb5_conf_free(void)
{
	struct krb_entry *ke, *ketmp;

	/*
	 *	Unlink from krbconf list.
	 */
	TAILQ_FOREACH_SAFE(ke, &krbconf, entries, ketmp) {
		if (ke->type == KRB_ENTRY_SECTION && ke->kes_nbindings > 0)
			krb5_bindings_unlink(&ke->kes_bindings);
		TAILQ_REMOVE(&krbconf, ke, entries);
	}

	/*
	 *	Free everything from the fentries list.
	 *	This is cheating.
	 */
	TAILQ_FOREACH_SAFE(ke, &fentries, fentries, ketmp) {
		switch (ke->type) {
			case KRB_ENTRY_SECTION:
				xfree(&ke->kes_name);
				break;

			case KRB_ENTRY_BINDING:
				xfree(&ke->keb_name);
				xfree(&ke->keb_value);
				break;

			case KRB_ENTRY_COMMENT:
				xfree(&ke->kec_text);
				break;
		}

		TAILQ_REMOVE(&fentries, ke, fentries);
		xfree(&ke);
	}
}

struct krb_entry *
section2entry(const char *section)
{
	int index;
	int count;
	char *ptr, *name;
	struct krb_entry *ke, *ketmp, *entry = NULL;

	if (section == NULL || section[0] == 0)
		return (NULL);

	index = -1;
	name = xstrdup(section);
	ptr = &name[strlen(name) - 1];
	if (isdigit(*ptr)) {
		while (isdigit(*(ptr - 1)))
			ptr--;
		index = strtol(ptr, NULL, 10);
		*ptr = 0;
	}

	count = 0;
	TAILQ_FOREACH_SAFE(ke, &krbconf, entries, ketmp) {
		if (ke->type == KRB_ENTRY_SECTION &&
			strcmp(ke->kes_name, name) == 0) {
			if (index < 0 || index == count) {
				entry = ke;
				break;
			}

			count++;
		}
	}

	xfree(&name);
	return (entry);
}

struct krb_entry *
binding2entry(struct krb_entry_list *el, const char *binding)
{
	int index;
	int count;
	char *ptr, *name;
	struct krb_entry *ke, *ketmp, *entry = NULL;

	if (el == NULL || binding == NULL || binding[0] == 0)
		return (NULL);

	index = -1;
	name = xstrdup(binding);
	ptr = &name[strlen(name) - 1];
	if (isdigit(*ptr)) {
		while (isdigit(*(ptr - 1)))
			ptr--;
		index = strtol(ptr, NULL, 10);
		*ptr = 0;
	}

	count = 0;
	TAILQ_FOREACH_SAFE(ke, el, entries, ketmp) {
		if ((ke->type == KRB_ENTRY_BINDING ||
			ke->type == KRB_ENTRY_BINDING_START) &&
			strcmp(ke->keb_name, name) == 0) {
			if (index < 0 || index == count) {
				entry = ke;
				break;
			}
		}

		count++;
	}

	return (entry);
}

struct krb_entry *
get_binding_parent(struct krb_entry *kp, struct krb_entry *kc)
{
	struct krb_entry *ke, *ketmp, *parent = NULL;
	struct krb_entry_list *el;

	if (kp != NULL && kp->type == KRB_ENTRY_SECTION) {
		el = &kp->kes_bindings;
	} else {
		el = &kp->keb_bindings;
	}

	if (kp != NULL) {
		TAILQ_FOREACH_SAFE(ke, el, entries, ketmp) {
			if (ke == kc) {
				parent = kp;
				break;
			}

			if (ke->keb_nbindings > 0) {
				parent = get_binding_parent(ke, kc);
				if (parent != NULL)
					 break;
			}
		}
	}

	return (parent);
}

struct krb_entry *
get_entry_parent(struct krb_entry *ep, struct krb_entry *kc)
{
	struct krb_entry *ke, *ketmp, *parent = NULL;

	TAILQ_FOREACH_SAFE(ke, &krbconf, entries, ketmp) {
		if (ke == kc)
			break;

		if (ke->kes_nbindings > 0) {
			parent = get_binding_parent(ke, kc);
			if (parent != NULL)
				break;
		}
	}

	return (parent);
}

struct krb_entry_list *
get_entry_list(struct krb_entry_list *el, struct krb_entry *kc)
{
	struct krb_entry *ke, *ketmp;
	struct krb_entry_list *fel = NULL;

	if (el == NULL) {
		return (get_entry_list(&krbconf, kc));

	} else {

		TAILQ_FOREACH_SAFE(ke, el, entries, ketmp) {
			switch (ke->type) {
				case KRB_ENTRY_SECTION:
					if (ke->kes_nbindings > 0) {
						if ((fel = get_entry_list(&ke->kes_bindings, kc)) != NULL)
							return (fel);
						fel = NULL;
					}
					break;

				case KRB_ENTRY_BINDING:
				case KRB_ENTRY_BINDING_START:
					if (ke->keb_nbindings > 0) {
						if ((fel = get_entry_list(&ke->keb_bindings, kc)) != NULL)
							return (fel);

						fel = NULL;
					}
					break;
			}

			if (ke == kc) {
				fel = el;
				break;
			}
		}
	}

	return (fel);
}

static int
parse_modstr(const char *modstr, char **section_pptr,
	int *nbindingsptr, char *bindings[], int bmax, char **value)
{
	struct krb_entry *ke = NULL;
	char *tmp, *ptr, *str, *save, *last, *buf, *bptr;
	int i, nbindings, bsize;

	save = xstrdup(modstr);
	ptr = save;

	tmp = strsep(&ptr, "=");	
	if (value != NULL && ptr != NULL && ptr[0] != 0)
		*value = xstrdup(ptr);
	last = str = tmp;

	bsize = 8192;
	buf = xalloc(bsize);
	bptr = buf;

	i = nbindings = 0;
	while (*tmp != 0) {
		if (*tmp == '.' && *last != '\\' || *(tmp + 1) == 0) {
			if (*tmp == '.' && *last != '\\') {
				*bptr = 0;
				bptr = buf;

			} else if (*(tmp + 1) == 0) {
				*bptr++ = *tmp;
				*bptr = 0;
				bptr = buf;
			}

			switch (i) {
				case 0:
					*section_pptr = xstrdup(buf);
					break;

				default:
					bindings[nbindings++] = xstrdup(buf);
					if (nbindings >= bmax)
						return (-1);
					break;
			}

			i++;

		} else if (*tmp == '.' && *last == '\\') {
			*(bptr - 1) = *tmp;

		} else {
			*bptr++ = *tmp;
		}

		last = tmp;
		tmp++;
	}

	*nbindingsptr = nbindings;
	xfree(&save);
	return (0);
}

static int
fentries_insert(struct krb_entry *kp, struct krb_entry *ke)
{
	struct krb_entry *last, *end = NULL, *entry = NULL;

	if (kp == NULL || ke == NULL)
		return (-1);

	if (ke->type == KRB_ENTRY_BINDING_START) {
		end = xalloc(sizeof(*end));
		end->type = KRB_ENTRY_BINDING_END;
		end->keb_name = end->keb_value = NULL;
		end->keb_nbindings = 0;
		TAILQ_INIT(&end->keb_bindings);
	}

	switch (kp->type) {
		case KRB_ENTRY_SECTION:
			if (kp->kes_nbindings > 1 && ke->type != KRB_ENTRY_BINDING_START) {
				struct krb_entry *fe, *fetmp, *last = NULL;

				TAILQ_FOREACH_SAFE(fe, &kp->kes_bindings, entries, fetmp) {
					if (fe == ke)
						break;
					last = fe;
				}

				if (last != NULL && last->type == KRB_ENTRY_BINDING_START) {
					int bs = 0, found = 0;
					struct krb_entry *node = last;

					TAILQ_FOREACH_SAFE(fe, &fentries, fentries, fetmp) {
						if (fe == node)
							found = 1;
						if (fe->type == KRB_ENTRY_BINDING_START && found == 1)
							bs++;
						if (fe->type == KRB_ENTRY_BINDING_END && found == 1)
							bs--;

						last = fe;

						if (found == 1 && bs == 0)
							break;
					}
				}

				if (last != NULL) {
					TAILQ_INSERT_AFTER(&fentries, last, ke, fentries);
					if (end != NULL)
						TAILQ_INSERT_AFTER(&fentries, ke, end, fentries);
				}

			} else if (kp->kes_nbindings > 1 && ke->type == KRB_ENTRY_BINDING_START) {
				struct krb_entry *fe, *fetmp, *last = NULL;
				int section = 0;

				TAILQ_FOREACH_SAFE(fe, &fentries, fentries, fetmp) {
					if (fe == kp) {
						section = 1;
						continue;
					}

					if (fe->type == KRB_ENTRY_SECTION && section > 0)
						break;

					last = fe;
				}

				if (section > 0) {
					TAILQ_INSERT_AFTER(&fentries, last, ke, fentries);
					if (end != NULL)
						TAILQ_INSERT_AFTER(&fentries, ke, end, fentries);
				}

			} else {
				TAILQ_INSERT_AFTER(&fentries, kp, ke, fentries);
				if (end != NULL)
					TAILQ_INSERT_AFTER(&fentries, ke, end, fentries);
			}

			break;

		case KRB_ENTRY_BINDING:
		case KRB_ENTRY_BINDING_START: 
			if (kp->keb_nbindings > 1 && ke->type != KRB_ENTRY_BINDING_START) {
				struct krb_entry *fe, *fetmp, *last = NULL;

				TAILQ_FOREACH_SAFE(fe, &kp->keb_bindings, entries, fetmp) {
					if (fe == ke)
						break;
					last = fe;
				}

				if (last != NULL && last->type == KRB_ENTRY_BINDING_START) {
					int bs = 0, found = 0;
					struct krb_entry *node = last;

					TAILQ_FOREACH_SAFE(fe, &fentries, fentries, fetmp) {
						if (fe == node)
							found = 1;
						if (fe->type == KRB_ENTRY_BINDING_START && found == 1)
							bs++;
						if (fe->type == KRB_ENTRY_BINDING_END && found == 1)
							bs--;

						last = fe;

						if (found == 1 && bs == 0)
							break;
					}
				}

				if (last != NULL) {
					TAILQ_INSERT_AFTER(&fentries, last, ke, fentries);
					if (end != NULL)
						TAILQ_INSERT_AFTER(&fentries, ke, end, fentries);
				}

			} else if (kp->keb_nbindings > 1 && ke->type == KRB_ENTRY_BINDING_START) {
				struct krb_entry *fe, *fetmp;

				TAILQ_FOREACH_SAFE(fe, &fentries, fentries, fetmp) {
					if (fe == kp) {
						TAILQ_INSERT_AFTER(&fentries, fe, ke, fentries);
						if (end != NULL)
							TAILQ_INSERT_AFTER(&fentries, ke, end, fentries);
						break;
					}
				}

			} else {
				TAILQ_INSERT_AFTER(&fentries, kp, ke, fentries);
				if (end != NULL)
					TAILQ_INSERT_AFTER(&fentries, ke, end, fentries);
			}

			break;
	}

	return (0);
}

static struct krb_entry *
create_bindings(struct krb_entry *ke, int nbindings,
	char *bindings[], int bmax, const char *value)
{
	int i;
	struct krb_entry *ep = ke;
	struct krb_entry *re = NULL;
	struct krb_entry_list *el = &ke->kes_bindings;

	for (i = 0;i < nbindings;i++) {
		struct krb_entry *kb = binding2entry(el, bindings[i]);

		if (kb != NULL) {
			el = &kb->keb_bindings;
			ep = kb;
			re = kb;

		} else {
			struct krb_entry *new = xalloc(sizeof(*new));
			struct krb_entry *end = NULL;

			re = new;
			new->keb_name = xstrdup(bindings[i]);

			if (i + 1 == nbindings && value == NULL)
				new->type = KRB_ENTRY_BINDING_START;
			else if (i + 1 == nbindings && value != NULL)
				new->type = KRB_ENTRY_BINDING;
			else
				new->type = KRB_ENTRY_BINDING_START;

			new->keb_value = NULL;
			TAILQ_INIT(&new->keb_bindings);
			new->keb_nbindings = 0;

			if (new->type == KRB_ENTRY_BINDING && value != NULL)
				new->keb_value = xstrdup(value);

			switch (ep->type) {
				case KRB_ENTRY_SECTION:
					TAILQ_INSERT_TAIL(&ep->kes_bindings, new, entries);
					ep->kes_nbindings++;
					break;
				case KRB_ENTRY_BINDING:
				case KRB_ENTRY_BINDING_START:
					TAILQ_INSERT_TAIL(&ep->keb_bindings, new, entries);
					ep->keb_nbindings++;
					break;
			}

			fentries_insert(ep, new);
			ep = new;
			re = new;
		}
	}

	return (re);
}

static struct krb_entry *
km2entry(struct krb_modification *km, char **value)
{
	struct krb_entry *ke = NULL;
	int i, nbindings = 0, bmax = 1024;
	char *section = NULL;
	char *bindings[bmax];

	if (km == NULL)
		return (NULL);

	if (parse_modstr(km->modstr, &section, &nbindings, bindings, bmax, value) < 0)
		return (NULL);

	if (section != NULL) {
		ke = section2entry(section);

		if (ke != NULL && nbindings > 0 && km->create == 0) {
			struct krb_entry_list *el = &ke->kes_bindings;

			for (i = 0;i < nbindings;i++) {
				ke = binding2entry(el, bindings[i]);
				if (ke == NULL)
					break;

				el = &ke->keb_bindings;
			}

		} else if (ke != NULL && nbindings > 0 && km->create == 1) {
			ke = create_bindings(ke, nbindings, bindings, bmax,
				value == NULL ? NULL : *value);

		} else if (km->create == 1) {
			struct krb_entry *ns = xalloc(sizeof(*ns));

			ns->type = KRB_ENTRY_SECTION;
			ns->kes_name = xstrdup(section);
			TAILQ_INIT(&ns->kes_bindings);
			ns->kes_nbindings = 0;

			TAILQ_INSERT_TAIL(&krbconf, ns, entries);
			TAILQ_INSERT_TAIL(&fentries, ns, fentries);

			if (nbindings > 0)
				ke = create_bindings(ns, nbindings, bindings, bmax,
					value == NULL ? NULL : *value);
		}
	}

	return (ke);
}

static int
krb_op_add(struct krb_entry *ke)
{
	return (0);
}

static int
krb_fentries_remove_section(struct krb_entry *ke)
{
	int delete = 0;
	struct krb_entry *ks, *kstmp;

	TAILQ_FOREACH_SAFE(ks, &fentries, fentries, kstmp) {
		if (ks == ke) {
			TAILQ_REMOVE(&fentries, ks, fentries);
			delete = 1;
			continue;
		}

		if (ks->type == KRB_ENTRY_SECTION && delete == 1)
			break;

		if (delete == 1) {
			TAILQ_REMOVE(&fentries, ks, fentries);
			switch (ks->type) {
				case KRB_ENTRY_COMMENT:
					xfree(&ks->kec_text);
					xfree(&ks);
					break;

				case KRB_ENTRY_NULL:
				case KRB_ENTRY_BINDING_END:
					xfree(&ks);
					break;
			}
		}
	}

	return (0);
}

static int
krb_fentries_remove_binding(struct krb_entry *ke)
{
	int bs, delete;
	struct krb_entry *kb, *kbtmp;

	if (ke == NULL)
		return (-1);

	bs = delete = 0;
	TAILQ_FOREACH_SAFE(kb, &fentries, fentries, kbtmp) {
		if (kb == ke && ke->type == KRB_ENTRY_BINDING) {
			TAILQ_REMOVE(&fentries, kb, fentries);
			break;
		}

		if (ke == kb)
			delete = 1;

		if (kb->type == KRB_ENTRY_BINDING_START) {
			if (delete == 1)
				bs++;
		}

		if (delete == 1) {
			TAILQ_REMOVE(&fentries, kb, fentries);

			switch (kb->type) {
				case KRB_ENTRY_NULL:
					xfree(&kb);
					break;

				case KRB_ENTRY_COMMENT:
					xfree(&kb->kec_text);
					xfree(&kb);
					break;

				case KRB_ENTRY_BINDING:
					break;
			}
		}

		if (kb->type == KRB_ENTRY_BINDING_END) {
			if (delete == 1) {
				xfree(&kb);
				bs--;
			}
		}

		if (delete == 1 && bs == 0)
			break;
	}

	return (0);
}

static int
krbconf_remove_binding(struct krb_entry_list *el, struct krb_entry *ke)
{
	if (ke == NULL || el == NULL)
		return (-1);
	if (ke->type != KRB_ENTRY_BINDING && ke->type != KRB_ENTRY_BINDING_START)
		return (-1);

	if (ke->keb_nbindings > 0) {
		struct krb_entry *kb, *kbtmp;

		TAILQ_FOREACH_SAFE(kb, &ke->keb_bindings, entries, kbtmp)
			krbconf_remove_binding(&ke->keb_bindings, kb);
	}

	TAILQ_REMOVE(el, ke, entries);
	xfree(&ke->keb_name);
	xfree(&ke->keb_value);
	xfree(&ke);

	return (0);
}

static int
krbconf_remove_section(struct krb_entry *ke)
{
	if (ke == NULL)
		return (-1);
	if (ke->type != KRB_ENTRY_SECTION)
		return (-1);

	if (ke->kes_nbindings > 0) {
		struct krb_entry *ks, *kstmp;

		TAILQ_FOREACH_SAFE(ks, &ke->kes_bindings, entries, kstmp) {
			if (ks->keb_nbindings > 0) {
				krbconf_remove_binding(&ke->kes_bindings, ks);

			} else {
				TAILQ_REMOVE(&ke->kes_bindings, ks, entries);
				xfree(&ks->keb_name);
				xfree(&ks->keb_value);
				xfree(&ks);
			}
		}
	}

	TAILQ_REMOVE(&krbconf, ke, entries);
	xfree(&ke->kes_name);
	xfree(&ke);

	return (0);
}

static int
krb_op_remove(struct krb_entry *ke)
{
	if (ke == NULL)
		return (-1);

	switch (ke->type) {
		case KRB_ENTRY_SECTION:
			krb_fentries_remove_section(ke);
			krbconf_remove_section(ke);
			break;

		case KRB_ENTRY_BINDING:
		case KRB_ENTRY_BINDING_START: {
			struct krb_entry_list *el = get_entry_list(NULL, ke);

			krb_fentries_remove_binding(ke);
			if (el != NULL)
				krbconf_remove_binding(el, ke);
			break;
		}
	}
	
	return (0);
}

static void
do_modifications(void)
{
	struct krb_modification *km, *kmtmp;

	TAILQ_FOREACH_SAFE(km, &modifications, entries, kmtmp) {
		switch (km->op) {
			case KRB_OP_ADD: {
				char *value = NULL;
				struct krb_entry *ke = km2entry(km, &value);

				if (krb_op_add(ke) < 0)
					warnx("KRB_OP_ADD: returned -1\n");

				xfree(&value);
				break;
			}

			case KRB_OP_MODIFY: {
				char *value = NULL;
				struct krb_entry *ke = km2entry(km, &value);
				if (ke == NULL)
					warnx("KRB_OP_MODIFY: km2entry() returned NULL\n");
				if (ke != NULL)
					ke->keb_value = xstrdup(value);
			
				xfree(&value);
				break;
			}

			case KRB_OP_REMOVE: {
				struct krb_entry *ke = km2entry(km, NULL);
				if (krb_op_remove(ke) < 0)
					warnx("KRB_OP_REMOVE: returned -1\n");

				break;
			}
		}

		TAILQ_REMOVE(&modifications, km, entries);
		xfree(&km->modstr);
		xfree(&km);
	}
}

int
main(int argc, char **argv)
{
	int ch;
	int create;
	char *infile, *outfile;

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
		yyin = fopen(infile, "r+");
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

	xfree(&infile);
	xfree(&outfile);

	do_modifications();
	write_krb5_conf();
	krb5_conf_free();

	if (yyin != NULL)
		fclose(yyin);
	if (yyout != NULL)
		fclose(yyout);

	return (0);
}
