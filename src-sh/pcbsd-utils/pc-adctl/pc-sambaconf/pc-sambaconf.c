/*
 *	pc-sambaconf.c
 *
 *	Configure samba
 */

#include "pc-sambaconf.h"
#include "samba-parser.tab.h"

#define	SMB_OP_ADD		1
#define	SMB_OP_MODIFY	2
#define	SMB_OP_REMOVE	3

struct smb_modification {
	TAILQ_ENTRY(smb_modification) entries;
	char *section;
	char *name;
	char *value;
	int overwrite;
	int op;
};
TAILQ_HEAD(smb_modification_list, smb_modification) modifications = 
	TAILQ_HEAD_INITIALIZER(modifications);

struct smb_entry_list fentries = TAILQ_HEAD_INITIALIZER(fentries);

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

static void
usage(void)
{
	fprintf(stderr,
		"usage: sambaconf [options]\n"
		"Where option in:\n\n"
		"\t-f <input file>\n"
		"\t-o <output file>\n"
		"\t-m <(+|^|-)section:name=value)>\n"
		"\t-w\n\n"
	);

	exit(1);
}

static void
samba_config_free(void)
{
	struct smb_entry *se, *setmp;

	TAILQ_FOREACH_SAFE(se, &fentries, entries, setmp) {
		switch (se->type) {
			case SMB_ENTRY_SECTION:
				xfree(&se->ses_name);
				break;

			case SMB_ENTRY_PARAMETERS:
				xfree(&se->sep_name);
				xfree(&se->sep_value);
				break;

			case SMB_ENTRY_COMMENT:
				xfree(&se->sec_text);
				break;
		}

		TAILQ_REMOVE(&fentries, se, entries);
		xfree(&se);
	}
}

static void
write_samba_config(void)
{
	struct smb_entry *se;

	TAILQ_FOREACH(se, &fentries, entries) {
		switch (se->type) {
			case SMB_ENTRY_SECTION:
				fprintf(yyout, "[%s]\n", se->ses_name);
				break;

			case SMB_ENTRY_PARAMETERS:
				fprintf(yyout, "\t%s = %s\n", se->sep_name, se->sep_value);
				break;

			case SMB_ENTRY_COMMENT:
				fprintf(yyout, "%s\n", se->sec_text);
				break;

			case SMB_ENTRY_NULL:
			default:
				fprintf(yyout, "\n");
				break;
		}
	}
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
add_modification(const char *m, int overwrite)
{
	int i;
	char *ptr, *eptr, *save;
	char *l, *r, *s, *section;
	struct smb_modification *sm;

	if (m == NULL)
		return (-1);

	sm = xalloc(sizeof(*sm));
	switch (m[0]) {
		case '+':
			sm->op = SMB_OP_ADD;
			break;

		case '^':
			sm->op = SMB_OP_MODIFY;
			break;

		case '-':
			sm->op = SMB_OP_REMOVE;
			break;

		default:
			xfree(&sm);
			return (-1);
	}

	sm->section = NULL;
	sm->name = NULL;
	sm->value = NULL;
	sm->overwrite = overwrite;

	save = ptr = xstrdup(&m[1]);
	section = strsep(&ptr, ":");
	if (section == NULL) {
		xfree(&save);
		return (-1);
	}
	section = clean(section);

	l = strsep(&ptr, "=");
	if (l == NULL && sm->op != SMB_OP_REMOVE) {
		xfree(&save);
		return (-1);
	}
	l = clean(l);

	r = ptr;
	if (r == NULL && sm->op != SMB_OP_REMOVE) {
		xfree(&save);
		return (-1);
	}
	r = clean(r);

	sm->section = xstrdup(section);
	sm->name = xstrdup(l);
	sm->value = xstrdup(r);
	TAILQ_INSERT_TAIL(&modifications, sm, entries);

	xfree(&save);
	return (0);
}

static int 
do_smb_op_add(struct smb_modification *sm)
{
	int section;
	struct smb_entry *se, *node;

	node = NULL;
	section = 0;
	TAILQ_FOREACH(se, &fentries, entries) {
		if (se->type == SMB_ENTRY_SECTION &&
			strcmp(se->ses_name, sm->section) == 0) {
			section = 1;
		}

		node = se;
		if (section != 0)
			break;
	}

	if (sm->overwrite) {
		struct smb_entry *save;

		save = node;
		TAILQ_FOREACH(se, &fentries, entries) {
			if (se->type == SMB_ENTRY_PARAMETERS &&
				strcmp(se->sep_name, sm->name) == 0) {
				xfree(&se->sep_value);
				se->sep_value = xstrdup(sm->value);
				return (0);
			}
		}

		node = save;
	}

	if (node != NULL) {
		struct smb_entry *new = xalloc(sizeof(*new));
		new->type = SMB_ENTRY_PARAMETERS;
		new->sep_name = xstrdup(sm->name);
		new->sep_value = xstrdup(sm->value);
		TAILQ_INSERT_AFTER(&fentries, node, new, entries);

	} else {
		struct smb_entry *news = xalloc(sizeof(*news));
		news->type = SMB_ENTRY_SECTION;
		news->ses_name = xstrdup(sm->section);
		TAILQ_INSERT_TAIL(&fentries, news, entries);

		struct smb_entry *newp = xalloc(sizeof(*newp));
		newp->type = SMB_ENTRY_PARAMETERS;
		newp->sep_name = xstrdup(sm->name);
		newp->sep_value = xstrdup(sm->value);
		TAILQ_INSERT_TAIL(&fentries, newp, entries);
	}

	return (0);
}

static int
do_smb_op_modify(struct smb_modification *sm)
{
	int section;
	struct smb_entry *se, *setmp;

	section = 0;
	TAILQ_FOREACH_SAFE(se, &fentries, entries, setmp) {
		if (se->type == SMB_ENTRY_SECTION &&
			strcmp(se->ses_name, sm->section) == 0) {
			section = 1;
		}

		if (section != 0 && se->type == SMB_ENTRY_PARAMETERS &&
			strcmp(se->sep_name, sm->name) == 0 && !smb_null(sm->value)) {
			free(se->sep_value);
			se->sep_value = xstrdup(sm->value);
			break;
		}
	}

	return (0);
}

static int
do_smb_op_remove(struct smb_modification *sm)
{
	int section;
	struct smb_entry *se, *setmp;

	section = 0;
	TAILQ_FOREACH_SAFE(se, &fentries, entries, setmp) {
		if (se->type == SMB_ENTRY_SECTION &&
			strcmp(se->ses_name, sm->section) == 0) {
			section = 1;
		}

		if (section != 0) {
			switch (se->type) {
				case SMB_ENTRY_SECTION: {
					if (smb_null(sm->name)) {
						TAILQ_REMOVE(&fentries, se, entries);
						xfree(&se->ses_name);
						xfree(&se);
					}

					break;
				}

				case SMB_ENTRY_PARAMETERS: {
					if (smb_null(sm->name)) {
						TAILQ_REMOVE(&fentries, se, entries);
						xfree(&se->sep_name);
						xfree(&se->sep_value);
						xfree(&se);

					} else if (strcmp(sm->name, se->sep_name) == 0) {
						TAILQ_REMOVE(&fentries, se, entries);
						xfree(&se->sep_name);
						xfree(&se->sep_value);
						xfree(&se);
					}

					break;
				}
			}
		}
	}

	return (0);
}

static void
do_modifications(void)
{
	struct smb_modification *sm, *smtmp;

	TAILQ_FOREACH_SAFE(sm, &modifications, entries, smtmp) {
		switch (sm->op) {
			case SMB_OP_ADD:
				do_smb_op_add(sm);
				break;

			case SMB_OP_MODIFY:
				do_smb_op_modify(sm);
				break;

			case SMB_OP_REMOVE:
				do_smb_op_remove(sm);
				break;
		}

		TAILQ_REMOVE(&modifications, sm, entries);
		xfree(&sm->section);
		xfree(&sm->name);
		xfree(&sm->value);
		xfree(&sm);
	}
}

int
main(int argc, char **argv)
{
	int ch, overwrite;
	char *infile, *outfile;

	if (argc <= 1)
		usage();

	overwrite = 0;
	infile = outfile = NULL;
	while ((ch = getopt(argc, argv, "f:m:o:w")) != -1) {
		switch (ch) {
			case 'f':
				xfree(&infile);
				infile = xstrdup(optarg);
				break;

			case 'm':
				add_modification(optarg, overwrite);
				overwrite = 0;
				break;

			case 'o':
				xfree(&outfile);
				outfile = xstrdup(optarg);
				break;
	
			case 'w':
				overwrite = 1;
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
	write_samba_config();

	samba_config_free();
	xfree(&outfile);
	xfree(&infile);

	if (yyout != NULL)
		fclose(yyout);
	if (yyin != NULL)
		fclose(yyin);

	return (0);
}
