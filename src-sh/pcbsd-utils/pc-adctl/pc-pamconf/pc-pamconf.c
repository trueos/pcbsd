/*
 *	pc-pamconf.c
 *
 *	Configure pam
 */

#include "pc-pamconf.h"
#include "pam-parser.tab.h"

#define	PAM_OP_ADD		1
#define	PAM_OP_MODIFY	2
#define	PAM_OP_REMOVE	3	

struct pam_modification {
	TAILQ_ENTRY(pam_modification) entries;
	struct pam_entry *pe;
	int index;
	int op;
};

/*
 *	List of modifications as specified on the command line with -m.
 */
TAILQ_HEAD(pam_modification_list, pam_modification) pam_modifications = 
	TAILQ_HEAD_INITIALIZER(pam_modifications);

/*
 *	Global pam configuration for this file, contains lists for all the
 *	facility types as well as comments and empty entries.
 */
struct pam_config p_config;

/*
 *	Pam file list, this is just a list with sequential entries for every
 *	line in a pam file.
 */
struct pam_entry_list pam_fentries =
	TAILQ_HEAD_INITIALIZER(pam_fentries);

extern FILE *yyin, *yyout;
extern int yyparse();
extern void add_argument(struct pam_entry *, const char *);


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

struct pam_entry *
new_pam_entry(int facility)
{
	struct pam_entry *pe = xalloc(sizeof(*pe));

	pe->pes_arguments = NULL;
	pe->pes_narguments = 0;
	pe->facility = facility;
	pe->type = PAM_ENTRY_NULL;
	bzero(&pe->pe, sizeof(pe->pe));

	switch (facility) {
		case AUTH:
			TAILQ_INSERT_TAIL(&p_config.pc_auth, pe, entries);
			pe->index = p_config.pc_auth_nentries;
			p_config.pc_auth_nentries++;
			break;

		case ACCOUNT:
			TAILQ_INSERT_TAIL(&p_config.pc_account, pe, entries);
			pe->index = p_config.pc_account_nentries;
			p_config.pc_account_nentries++;
			break;

		case SESSION:
			TAILQ_INSERT_TAIL(&p_config.pc_session, pe, entries);
			pe->index = p_config.pc_session_nentries;
			p_config.pc_session_nentries++;
			break;

		case PASSWORD:
			TAILQ_INSERT_TAIL(&p_config.pc_password, pe, entries);
			pe->index = p_config.pc_password_nentries;
			p_config.pc_password_nentries++;
			break;

		case COMMENT:
			TAILQ_INSERT_TAIL(&p_config.pc_comment, pe, entries);
			pe->index = p_config.pc_comment_nentries;
			p_config.pc_comment_nentries++;
			break;
	}

	return (pe);
}

void
free_arguments(struct pam_entry *pe)
{
	if (pe != NULL) {
		int i;

		for (i = 0;i < pe->pes_narguments;i++) {
			if (pe->pes_arguments[i] != NULL)
				xfree(&pe->pes_arguments[i]);
			pe->pes_arguments[i] = 0;
		}

		xfree(&pe->pes_arguments);
		pe->pes_arguments = NULL;
		pe->pes_narguments = 0;
	}
}

void
free_pam_entry(struct pam_entry *pe)
{
	switch (pe->type) {
		case PAM_ENTRY_SERVICE:
			free_arguments(pe);
			xfree(&pe->pes_module);
			break;

		case PAM_ENTRY_INCLUDE:
			xfree(&pe->pei_service);
			break;

		case PAM_ENTRY_COMMENT:
			xfree(&pe->pec_text);
			break;
	}

	xfree(&pe);
}

void
free_pam_entries(struct pam_entry_list *pe_list)
{
	while (!TAILQ_EMPTY(pe_list)) {
		struct pam_entry *pe = TAILQ_FIRST(pe_list);
		TAILQ_REMOVE(pe_list, pe, entries);
		free_pam_entry(pe);
	}
}

static void
usage(void)
{
	fprintf(stderr,
		"usage: pamconf [options]\n"
		"Where option in:\n\n"
		"\t-f <input file>\n"
		"\t-o <output file>\n"
		"\t-m <(+|^|-)(index)?facility:(control:module:arg:arg:...|include:service)>\n\n"
	);

	exit(1);
}


static int
facility2int(const char *f)
{
	if (strcmp(f, "auth") == 0)
		return (AUTH);
	else if (strcmp(f, "account") == 0)
		return (ACCOUNT);
	else if (strcmp(f, "session") == 0)
		return (SESSION);
	else if (strcmp(f, "password") == 0)
		return (PASSWORD);	

	return (-1);
}

static const char *
int2facility(int f)
{
	switch (f) {
		case AUTH:
			return ("auth");
		case ACCOUNT:
			return ("account");
		case SESSION:
			return ("session");
		case PASSWORD:
			return ("password");
		default:
			return (NULL);
	}
}

static int
control2int(const char *c)
{
	if (strcmp(c, "required") == 0)
		return (REQUIRED);
	else if (strcmp(c, "requisite") == 0)
		return (REQUISITE);
	else if (strcmp(c, "sufficient") == 0)
		return (SUFFICIENT);
	else if (strcmp(c, "binding") == 0)
		return (BINDING);
	else if (strcmp(c, "optional") == 0)
		return (OPTIONAL);
	else if (strcmp(c, "include") == 0)
		return (INCLUDE);

	return (-1);
}

static const char *
int2control(int c)
{
	switch (c) {
		case REQUIRED:
			return ("required");
		case REQUISITE:
			return ("requisite");
		case SUFFICIENT:
			return ("sufficient");
		case BINDING:
			return ("binding");
		case OPTIONAL:
			return ("optional");
		case INCLUDE:
			return ("include");
		default:
			return (NULL);
	}

	return NULL;
}

static char *
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
add_modification(const char *m)
{
	int i, c, f;
	char *s, *ptr, *eptr, *save;
	struct pam_modification *pm;
	struct pam_entry *pe;

	if (m == NULL)
		return (-1);

	pm = xalloc(sizeof(*pm));
	switch (m[0]) {
		case '+':
			pm->op = PAM_OP_ADD;
			break;

		case '^':
			pm->op = PAM_OP_MODIFY;
			break;

		case '-':
			pm->op = PAM_OP_REMOVE;
			break;

		default:
			xfree(&pm);
			return (-1);
	}

	pe = new_pam_entry(-1);
	pm->pe = pe;

	pm->index = strtol(&m[1], &eptr, 10);
	if (&m[1] == eptr)
		pm->index = -1;

	i = c = f = 0;
	save = ptr = xstrdup(eptr);
	while ((s = strsep(&ptr, ":")) != NULL) {
		s = clean(s);
		switch (i) {

			/* facility */
			case 0:
				if ((f = facility2int(s)) < 0) {
					xfree(&save);
					return (-1);
				}
				break;

			/* include | control */
			case 1:
				if ((c = control2int(s)) < 0) {
					if (pm->index < 0) {
						xfree(&save);
						return (-1);

					/*
					 *	An index is specified, with a facility and a *	trailing :
					 */
					} else {
						pe->facility = f;
						break;
					}
				}

				if (c == INCLUDE) {
	 				pe->type = PAM_ENTRY_INCLUDE;
					pe->facility = f;

				} else {
					pe->type = PAM_ENTRY_SERVICE;
					pe->facility = f;
					pe->pes_control = c;
				}
				break;

			/* module | service */
			case 2:
				if (c == INCLUDE)
					pe->pei_service = xstrdup(s);
				else
					pe->pes_module = xstrdup(s);

				break;

			/* args */
			default:
				if (c != INCLUDE)
					add_argument(pe, s);
				break;
		}

		i++;
	}

	/*
	 *	This happens when only an index and facility is specified.
	 */
	if (f != 0 && c == 0)
		pe->facility = f;

	TAILQ_INSERT_TAIL(&pam_modifications, pm, entries);
	xfree(&save);

	return (0);
}

/*
 *	Facility to list in global config.
 */
static struct pam_entry_list *
f2pel(int f)
{
	struct pam_entry_list *pel;

	switch (f) {
		case AUTH:
			pel = &p_config.pc_auth;
			break;
		case ACCOUNT:
			pel = &p_config.pc_account;
			break;
		case SESSION:
			pel = &p_config.pc_session;
			break;
		case PASSWORD:
			pel = &p_config.pc_password;
			break;
	}

	return (pel);
}

static unsigned int
get_list_count(struct pam_entry_list *pel)
{
	unsigned int count = 0;
	struct pam_entry *pe;

	TAILQ_FOREACH(pe, pel, entries) {
		count++;
	}

	return (count);
}

static void
update_count(struct pam_entry_list *pel)
{
	if (pel == &p_config.pc_auth)
		p_config.pc_auth_nentries = get_list_count(pel);
	else if (pel == &p_config.pc_account)
		p_config.pc_account_nentries = get_list_count(pel);
	else if (pel == &p_config.pc_session)
		p_config.pc_session_nentries = get_list_count(pel);
	else if (pel == &p_config.pc_password)
		p_config.pc_password_nentries = get_list_count(pel);
	else if (pel == &p_config.pc_comment)
		p_config.pc_comment_nentries = get_list_count(pel);
}

/*
 *	Re-order pam entry indexes. This is so that we can specify multiple
 *	-m options in a row and it just work "as expected" if using indexes.
 */
static void
update_indexes(struct pam_entry_list *pel)
{
	int index = 0;
	struct pam_entry *pe, *petmp;

	TAILQ_FOREACH_SAFE(pe, pel, entries, petmp) {
		pe->index = index;
		index++;
	}
}

static unsigned int
get_list_last_index(struct pam_entry_list *pel)
{
	unsigned int index = 0;
	struct pam_entry *pe;

	TAILQ_FOREACH(pe, pel, entries) {
		index = pe->index;
	}

	return (index);
}

static unsigned int
get_list_first_index(struct pam_entry_list *pel)
{
	unsigned int count = 0;
	struct pam_entry *pe;

	pe = TAILQ_FIRST(pel);

	return (pe->index);
}

/*
 *	Get the first facility in a pam file, if it exists, based on count.
 */
static int
get_first_facility(void)
{
	int facility = -1;

	if (p_config.pc_password_nentries > 0)
		facility = PASSWORD;
	if (p_config.pc_session_nentries > 0)
		facility = SESSION;
	if (p_config.pc_account_nentries > 0)
		facility = ACCOUNT;
	if (p_config.pc_auth_nentries > 0)
		facility = AUTH;

	return (facility);
}

static int
get_next_facility(int facility)
{
	int count;

	switch (facility) {
		case AUTH:
			if ((count = get_list_count(&p_config.pc_account)) > 0)
				return (ACCOUNT);
			if ((count = get_list_count(&p_config.pc_session)) > 0)
				return (SESSION);
			if ((count = get_list_count(&p_config.pc_password)) > 0)
				return (PASSWORD);

		case ACCOUNT:
			if ((count = get_list_count(&p_config.pc_session)) > 0)
				return (SESSION);
			if ((count = get_list_count(&p_config.pc_password)) > 0)
				return (PASSWORD);

		case SESSION:
			if ((count = get_list_count(&p_config.pc_password)) > 0)
				return (PASSWORD);

		case PASSWORD:
		default:
			if ((count = get_list_count(&p_config.pc_password)) > 0)
				return (PASSWORD);
	}

	return (-1);
}

static int
get_last_facility(void)
{
	int facility = -1;

	if (p_config.pc_auth_nentries > 0)
		facility = AUTH;
	if (p_config.pc_account_nentries > 0)
		facility = ACCOUNT;
	if (p_config.pc_session_nentries > 0)
		facility = SESSION;
	if (p_config.pc_password_nentries > 0)
		facility = PASSWORD;

	return (facility);
}

static int
do_pam_op_add(struct pam_entry_list *pel, int index, struct pam_entry *pmpe)
{
	int firstf = get_first_facility();
	int lastf = get_last_facility();
	int nextf = get_next_facility(pmpe->facility);

	/*
	 *	Facility list is empty, insert first entry.
	 */
	if (TAILQ_EMPTY(pel)) {
		TAILQ_INSERT_TAIL(pel, pmpe, entries);

		/*
		 *	File list is empty, insert first entry.
		 */
		if (TAILQ_EMPTY(&pam_fentries)) {
			TAILQ_INSERT_TAIL(&pam_fentries, pmpe, fentries);

		/*
		 *	File is not empty and no facilities exist in the file, so it
		 *	must be just comments or empty entries, insert at the end.
		 */
		} else if (firstf < 0) {
			TAILQ_INSERT_TAIL(&pam_fentries, pmpe, fentries);

		/*
		 *	File is not empty, a facility exists, but this entry is
		 *	after it, so we just add this entry to the end of the file.
		 */
		} else if (nextf < 0) {
			TAILQ_INSERT_TAIL(&pam_fentries, pmpe, fentries);

		/*
		 *	This entry belongs somewhere in the middle since a next
		 *	facility exists, so we find it and insert this entry before
		 *	it.
		 */
		} else {
			struct pam_entry *pe, *petmp, *prev = NULL;

			TAILQ_FOREACH_SAFE(pe, &pam_fentries, fentries, petmp) {
				if (pe->facility == nextf) {
					TAILQ_INSERT_BEFORE(pe, pmpe, fentries);
					break;
				}
			}
		}

	/*
	 *	An index was specified, try to insert at it.
	 */
	} else if (index >= 0) {
		struct pam_entry *pe, *petmp;

		/*
		 *	Insert into the facility list at specified index. If the
		 *	index can't be found, we insert after the last index.
		 */
		int lindex = get_list_last_index(pel);
		TAILQ_FOREACH_SAFE(pe, pel, entries, petmp) {
			if (index == pe->index) {
				TAILQ_INSERT_BEFORE(pe, pmpe, entries);
				TAILQ_INSERT_BEFORE(pe, pmpe, fentries);
				break;

			} else if (pe->index == lindex) {
				TAILQ_INSERT_AFTER(pel, pe, pmpe, entries);
				TAILQ_INSERT_AFTER(pel, pe, pmpe, fentries);
				break;
			}
		}

	/*
	 *	The list isn't empty, an index wasn't specified, so just insert
	 *	at the end of the list then find the previous entry in the file
	 *	list and insert after it.
	 */
	} else {
		struct pam_entry *pe1, *pe2;

		TAILQ_INSERT_TAIL(pel, pmpe, entries);

		pe1 = TAILQ_FIRST(pel);
		while (pe1 != NULL) {
			pe2 = TAILQ_NEXT(pe1, entries);
			if (pe2 == pmpe) {
				TAILQ_INSERT_AFTER(&pam_fentries, pe1, pmpe, fentries);
				break;
			}

			pe1 = pe2;
		}
	}

	update_count(pel);
	update_indexes(pel);
	return (0);
}

static int
do_pam_op_modify(struct pam_entry_list *pel, int index, struct pam_entry *pmpe)
{
	if ((pe_service(pmpe) && pmpe->pes_module == NULL) ||
		(pe_include(pmpe) && pmpe->pei_service == NULL) || pe_null(pmpe))
		return (-1);

	/*
	 *	Only allow modify operations if an index is used.
	 */
	if (index >= 0) {
		struct pam_entry *pe, *petmp;

		TAILQ_FOREACH_SAFE(pe, pel, entries, petmp) {

			/*
			 *	We found the index, insert entry after it, then remove it.
			 */
			if (index == pe->index) {
				TAILQ_INSERT_AFTER(pel, pe, pmpe, entries);
				TAILQ_INSERT_AFTER(&pam_fentries, pe, pmpe, fentries);

				TAILQ_REMOVE(pel, pe, entries);
				TAILQ_REMOVE(&pam_fentries, pe, fentries);

				free_pam_entry(pe);
				break;
			}
		}
	}

	update_count(pel);
	update_indexes(pel);
	return (0);
}

static int
do_pam_op_remove(struct pam_entry_list *pel, int index, struct pam_entry *pmpe)
{
	struct pam_entry *pe, *petmp;

	/*
	 *	Remove by index.
	 */
	if (index >= 0) {
		struct pam_entry *pe, *petmp;

		TAILQ_FOREACH_SAFE(pe, pel, entries, petmp) {
			if (index == pe->index) {
				TAILQ_REMOVE(pel, pe, entries);
				TAILQ_REMOVE(&pam_fentries, pe, fentries);
				free_pam_entry(pe);
				break;
			}
		}

	/*
	 *	Remove based on what information we have, this allows for
	 *	wildcard like removals.
	 */
	} else {
		struct pam_entry *pe, *petmp;

		TAILQ_FOREACH_SAFE(pe, pel, entries, petmp) {

			/*
			 *	We only know the facility, remove all entries of this
			 *	facility type.
			 */
			if (pe_null(pmpe)) {
				TAILQ_REMOVE(pel, pe, entries);
				TAILQ_REMOVE(&pam_fentries, pe, fentries);
				free_pam_entry(pe);

			/*
			 *	This is a service entry, if no module is specified,
			 *	remove all entries of specified facility and control. If
			 *	a module was specified, remove only that entry.
			 */
			} else if (pe_service(pmpe) && pe_service(pe) &&
				pmpe->facility == pe->facility &&
				pmpe->pes_control == pe->pes_control) {
				TAILQ_REMOVE(pel, pe, entries);
				if (pmpe->pes_module == NULL ||
					pmpe->pes_module[0] == 0 ||
					strcmp(pmpe->pes_module, pe->pes_module) == 0) {
					TAILQ_REMOVE(pel, pe, entries);
					TAILQ_REMOVE(&pam_fentries, pe, fentries);
					free_pam_entry(pe);
				}

			/*
			 *	this is a include entry. If no service was specified,
			 *	remove all entries of specified facility. If a service
			 *	was specified, only remove that entry.
			 */
			} else if (pe_include(pmpe) && pe_include(pe) &&
				pmpe->facility == pe->facility) {
				if (pmpe->pei_service == NULL ||
					pmpe->pei_service[0] == 0 ||
					strcmp(pmpe->pei_service, pe->pei_service) == 0) {
					TAILQ_REMOVE(pel, pe, entries);
					TAILQ_REMOVE(&pam_fentries, pe, fentries);
					free_pam_entry(pe);
				}
			}
		}
	}

	update_count(pel);
	update_indexes(pel);
	return (0);
}


static void
do_modifications(void)
{
	struct pam_modification *pm, *pmtmp;

	TAILQ_FOREACH_SAFE(pm, &pam_modifications, entries, pmtmp) {
		struct pam_entry_list *pel;
		struct pam_entry *pmpe;

		pmpe = pm->pe;
		pel = f2pel(pmpe->facility);

		switch (pm->op) {
			case PAM_OP_ADD:
				do_pam_op_add(pel, pm->index, pmpe);
				break;

			case PAM_OP_MODIFY:
				do_pam_op_modify(pel, pm->index, pmpe);
				break;

			case PAM_OP_REMOVE:
				do_pam_op_remove(pel, pm->index, pmpe);
				break;
		}

		TAILQ_REMOVE(&pam_modifications, pm, entries);
		pm->pe = NULL;
		xfree(&pm);
	}
}

void
unlink_pam_fentries(void)
{
	struct pam_entry *pe, *petmp;

	TAILQ_FOREACH_SAFE(pe, &pam_fentries, fentries, petmp) {
		TAILQ_REMOVE(&pam_fentries, pe, fentries);
	}
}


static void
pam_config_init()
{
	p_config.pc_auth_nentries = 0;
	TAILQ_INIT(&p_config.pc_auth);

	p_config.pc_account_nentries = 0;
	TAILQ_INIT(&p_config.pc_account);

	p_config.pc_session_nentries = 0;
	TAILQ_INIT(&p_config.pc_session);

	p_config.pc_password_nentries = 0;
	TAILQ_INIT(&p_config.pc_password);

	p_config.pc_comment_nentries = 0;
	TAILQ_INIT(&p_config.pc_comment);

	TAILQ_INIT(&pam_fentries);
}

static void
pam_config_free(void)
{
	unlink_pam_fentries();

	free_pam_entries(&p_config.pc_auth);
	p_config.pc_auth_nentries = 0;

	free_pam_entries(&p_config.pc_account);
	p_config.pc_account_nentries = 0;

	free_pam_entries(&p_config.pc_session);
	p_config.pc_session_nentries = 0;

	free_pam_entries(&p_config.pc_password);
	p_config.pc_password_nentries = 0;

	free_pam_entries(&p_config.pc_comment);
	p_config.pc_comment_nentries = 0;
}

static int
make_service_line(struct pam_entry *pe, char **line)
{
	if (pe == NULL || line == NULL)
		return (-1);

	if (pe->pes_narguments > 0) {
		int i, j;
		char buf[8192];

		j = 0;
		bzero(&buf, sizeof(buf));
		for (i = 0;i < pe->pes_narguments;i++)
			j += snprintf(&buf[j], sizeof(buf) - j, "\t%s", pe->pes_arguments[i]);

		asprintf(line, "%s\t%s\t%s%s",
			int2facility(pe->facility),
			int2control(pe->pes_control),
			pe->pes_module,
			buf	
		);

	} else {
		asprintf(line, "%s\t%s\t%s",
			int2facility(pe->facility),
			int2control(pe->pes_control),
			pe->pes_module
		);
	}

	return (0);
}

static int
make_include_line(struct pam_entry *pe, char **line)
{
	if (pe == NULL || line == NULL)
		return (-1);

	asprintf(line, "%s\tinclude\t%s",
		int2facility(pe->facility),
		pe->pei_service
	);

	return (0);
}

static int
make_comment_line(struct pam_entry *pe, char **line)
{
	if (pe == NULL || line == NULL)
		return (-1);

	asprintf(line, "%s", pe->pec_text);

	return (0);
}

static int
make_line(struct pam_entry *pe, char **line)
{
	if (pe_service(pe))
		return (make_service_line(pe, line));

	else if (pe_include(pe))
		return (make_include_line(pe, line));

	else if (pe_comment(pe))
		return (make_comment_line(pe, line));

	return (-1);
}

static void
write_pam_file(void)
{
	struct pam_entry *pe;

	TAILQ_FOREACH(pe, &pam_fentries, fentries) {
		char *line = NULL;

		if (make_line(pe, &line) >= 0)
			fprintf(yyout, "%s\n", line);
		else
			fprintf(yyout, "\n");

		xfree(&line);
	}
}

int
main(int argc, char **argv)
{
	int ch;
	char *infile, *outfile;

	if (argc <= 1)
		usage();

	pam_config_init();

	infile = outfile = NULL;
	while ((ch = getopt(argc, argv, "f:m:o:")) != -1) {
		switch (ch) {
			case 'f':
				xfree(&infile);
				infile = xstrdup(optarg);
				break;

			case 'm':
				if (add_modification(optarg) < 0)
					errx(EX_DATAERR, "syntax error");
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
	write_pam_file();

	pam_config_free();
	xfree(&outfile);
	xfree(&infile);

	if (yyout != NULL)
		fclose(yyout);
	if (yyin != NULL)
		fclose(yyin);

	return (0);
}
