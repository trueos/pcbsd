/*
 *	pc-nssconf.c
 *
 *	Configure NSS
 *
 *	This is kinda messy n' stuff.
 */

#include "pc-nssconf.h"
#include "nss-parser.tab.h"

#define	NSS_DATABASE_ADD		0x1000
#define	NSS_DATABASE_MODIFY		0x2000
#define	NSS_DATABASE_REMOVE		0x4000
#define	NSS_DATABASE_RESET		0x8000

#define	NSS_SOURCE_ADD			0x0001
#define	NSS_SOURCE_REMOVE		0x0002

#define	NSS_CRITERIA_ADD		0x0010
#define	NSS_CRITERIA_REMOVE		0x0020

struct nss_info_modification {
	unsigned int type;
	int index;
	union {
		struct nss_source ns;
		struct nss_criteria nc;
	} nim;
	unsigned int op;

#define	nimc	nim.nc
#define	nims	nim.ns

#define	nimc_status		nimc.status
#define	nimc_action		nimc.action
#define	nims_source		nims.source

	TAILQ_ENTRY(nss_info_modification) entries;

};

struct nss_modification {
	unsigned int op;
	struct nss_database nd;

#define	nm_database		nd.database

	TAILQ_HEAD(, nss_info_modification) info;
	TAILQ_ENTRY(nss_modification) entries;
};
TAILQ_HEAD(nss_modification_list, nss_modification) modifications =
	TAILQ_HEAD_INITIALIZER(modifications);

struct nss_entry_list fentries = TAILQ_HEAD_INITIALIZER(fentries);

extern FILE *yyin, *yyout;
extern int yyparse();

static void
usage(void)
{
	fprintf(stderr,
		"usage: nssconf [options]\n"
		"Where option in:\n\n"
		"\t-f <input file>\n"
		"\t-o <output file>\n"
		"\t-d <(+|^|-)database>\n"
		"\t-c <(+|-)(index)?criteria>\n"
		"\t-s <(+|-)(index)?source>\n"
		"\t-r\n\n"
	);

	exit(1);
}

const char *
database2str(unsigned int db)
{
	const char *str = NULL;

	switch (db) {
		case GROUP:
			str = "group";
			break;

		case GROUP_COMPAT:
			str = "group_compat";
			break;

		case HOSTS:
			str = "hosts";
			break;

		case NETWORKS:
			str = "networks";
			break;

		case PASSWD:
			str = "passwd";
			break;

		case PASSWD_COMPAT:
			str = "passwd_compat";
			break;

		case SHELLS:
			str = "shells";
			break;

		case SERVICES:
			str = "services";
			break;

		case SERVICES_COMPAT:
			str = "services_compat";
			break;

		case RPC:
			str = "rpc";
			break;

		case PROTOCOLS:
			str = "protocols";
			break;

		case NETGROUP:
			str = "netgroup";
			break;
	}

	return (str);
}

unsigned int
str2database(const char *str)
{
	unsigned int db = 0;

	if (strcmp(str, "group") == 0)
		db = GROUP;
	else if (strcmp(str, "group_compat") == 0)
		db = GROUP_COMPAT;
	else if (strcmp(str, "hosts") == 0)
		db = HOSTS;
	else if (strcmp(str, "networks") == 0)
		db = NETWORKS;
	else if (strcmp(str, "passwd") == 0)
		db = PASSWD;
	else if (strcmp(str, "passwd_compat") == 0)
		db = PASSWD_COMPAT;
	else if (strcmp(str, "shells") == 0)
		db = SHELLS;
	else if (strcmp(str, "services") == 0)
		db = SERVICES;
	else if (strcmp(str, "services_compat") == 0)
		db = SERVICES_COMPAT;
	else if (strcmp(str, "rpc") == 0)
		db = RPC;
	else if (strcmp(str, "protocols") == 0)
		db = PROTOCOLS;
	else if (strcmp(str, "netgroup") == 0)
		db = NETGROUP;

	return (db);
}


const char *
source2str(unsigned int src)
{
	const char *str = NULL;

	switch (src) {
		case FILES:
			str = "files";
			break;

		case DB:
			str = "db";
			break;

		case DNS:
			str = "dns";
			break;

		case NIS:
			str = "nis";
			break;

		case COMPAT:
			str = "compat";
			break;

		case CACHE:
			str = "cache";
			break;
	}

	return (str);
}

unsigned int
str2source(const char *str)
{
	unsigned int src = 0;

	if (strcmp(str, "files") == 0)
		src = FILES;
	else if (strcmp(str, "db") == 0)
		src = DB;
	else if (strcmp(str, "dns") == 0)
		src = DNS;
	else if (strcmp(str, "nis") == 0)
		src = NIS;
	else if (strcmp(str, "compat") == 0)
		src = COMPAT;
	else if (strcmp(str, "cache") == 0)
		src = CACHE;

	return (src);
}

const char *
status2str(unsigned int status)
{
	const char *str = NULL;

	switch (status) {
		case SUCCESS:
			str = "success";
			break;

		case NOTFOUND:
			str = "notfound";
			break;

		case UNAVAIL:
			str = "unavail";
			break;

		case TRYAGAIN:
			str = "tryagain";
			break;
	}

	return (str);
}

unsigned int
str2status(const char *str)
{
	unsigned int status = 0;

	if (strcmp(str, "success") == 0)
		status = SUCCESS;
	else if (strcmp(str, "notfound") == 0)
		status = NOTFOUND;
	else if (strcmp(str, "unavail") == 0)
		status = UNAVAIL;
	else if (strcmp(str, "tryagain") == 0)
		status = TRYAGAIN;

	return (status);
}

const char *
action2str(unsigned int action)
{
	const char *str = NULL;

	switch (action) {
		case RETURN:
			str = "return";
			break;

		case CONTINUE:
			str = "continue";
			break;
	}

	return (str);
}

unsigned int
str2action(const char *str)
{
	unsigned int action = 0;

	if (strcmp(str, "return") == 0)
		action = RETURN;
	else if (strcmp(str, "continue") == 0)
		action = CONTINUE;

	return (action);
}

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
nsswitch_conf_free(void)
{
	struct nss_entry *ne, *netmp;

	TAILQ_FOREACH_SAFE(ne, &fentries, entries, netmp) {
		switch (ne->type) {
			case NSS_ENTRY_NULL:
				TAILQ_REMOVE(&fentries, ne, entries);
				xfree(&ne);
				break;

			case NSS_ENTRY_DATABASE: {
				struct nss_entry_info *nei, *neitmp;

				TAILQ_FOREACH_SAFE(nei, &ne->info, entries, neitmp) {
					switch (nei->type) {
						case NSS_ENTRY_INFO_SOURCE:
							TAILQ_REMOVE(&ne->info, nei, entries);
							xfree(&nei->neis_source);
							xfree(&nei);
							break;

						case NSS_ENTRY_INFO_CRITERIA:
							TAILQ_REMOVE(&ne->info, nei, entries);
							xfree(&nei);
							break;
					}
				}

				TAILQ_REMOVE(&fentries, ne, entries);
				xfree(&ne->ned_database);
				xfree(&ne);
				break;
			}

			case NSS_ENTRY_COMMENT:
				TAILQ_REMOVE(&fentries, ne, entries);
				xfree(&ne->nec_comment);
				xfree(&ne);
				break;
		}
	}
}

static struct nss_entry *
nss_entry_by_database(const char *database)
{
	struct nss_entry *ne, *node;

	if (database == NULL)
		return (NULL);

	node = NULL;	
	TAILQ_FOREACH(ne, &fentries, entries) {
		if (ne->type == NSS_ENTRY_DATABASE &&
			strcmp(ne->ned_database, database) == 0) {
			node = ne;
			break;
		}
	}

	return (node);
}

static int
reset_database_info(struct nss_modification *nm)
{
	struct nss_entry *node;
	struct nss_entry_info *nei, *neitmp;

	if (nm == NULL)
		return (-1);

	if ((node = nss_entry_by_database(nm->nm_database)) == NULL)
		return (-1);

	TAILQ_FOREACH_SAFE(nei, &node->info, entries, neitmp) {
		TAILQ_REMOVE(&node->info, nei, entries);

		switch (nei->type) {
			case NSS_ENTRY_INFO_SOURCE:
				xfree(&nei->neis_source);
				break;

			case NSS_ENTRY_INFO_NULL:
			case NSS_ENTRY_INFO_CRITERIA:
				break;
		}

		xfree(&nei);
	}

	node->ninfo = 0;
	return (0);
}

static void
update_indexes(struct nss_entry *node)
{
	int index = 0;
	struct nss_entry_info *nei, *neitmp;

	TAILQ_FOREACH_SAFE(nei, &node->info, entries, neitmp) {
		nei->index = index;
		index++;
	}
}

static void
nss_source_add(struct nss_entry *node,
	struct nss_modification *nm, struct nss_info_modification *nim)
{
	struct nss_entry_info *new = xalloc(sizeof(*new));

	new->type = NSS_ENTRY_INFO_SOURCE;
	new->neis_source = xstrdup(nim->nims_source);

	if (nim->index < 0) {
		TAILQ_INSERT_TAIL(&node->info, new, entries);
		TAILQ_REMOVE(&nm->info, nim, entries);
		node->ninfo++;

	} else {
		struct nss_entry_info *nei, *neitmp;
		int inserted = 0;

		TAILQ_FOREACH_SAFE(nei, &node->info, entries, neitmp) {
			if (nim->index == nei->index) {
				TAILQ_INSERT_BEFORE(nei, new, entries);
				TAILQ_REMOVE(&nm->info, nim, entries);
				node->ninfo++;
				inserted = 1;
				break;
			}

		}

		if (inserted != 1) {
			TAILQ_INSERT_TAIL(&node->info, new, entries);
			TAILQ_REMOVE(&nm->info, nim, entries);
			node->ninfo++;
		}

		update_indexes(node);
	}

	xfree(&nim->nims_source);
	xfree(&nim);
}

static void
nss_source_remove(struct nss_entry *node,
	struct nss_modification *nm, struct nss_info_modification *nim)
{
	struct nss_entry_info *nei, *neitmp;

	TAILQ_FOREACH_SAFE(nei, &node->info, entries, neitmp) {
		if (nei->type == NSS_ENTRY_INFO_SOURCE &&
			strcmp(nei->neis_source, nim->nims_source) == 0) {
			TAILQ_REMOVE(&node->info, nei, entries);
			xfree(&nei->neis_source);
			xfree(&nei);
			node->ninfo--;
			break;
		}
	}
}

static void
nss_criteria_add(struct nss_entry *node,
	struct nss_modification *nm, struct nss_info_modification *nim)
{
	struct nss_entry_info *new = xalloc(sizeof(*new));

	new->type = NSS_ENTRY_INFO_CRITERIA;
	new->neic_status = nim->nimc_status;
	new->neic_action = nim->nimc_action;

	if (nim->index < 0) {
		TAILQ_INSERT_TAIL(&node->info, new, entries);
		TAILQ_REMOVE(&nm->info, nim, entries);
		node->ninfo++;

	} else {
		struct nss_entry_info *nei, *neitmp;
		int inserted = 0;

		TAILQ_FOREACH_SAFE(nei, &node->info, entries, neitmp) {
			if (nim->index == nei->index) {
				TAILQ_INSERT_BEFORE(nei, new, entries);
				TAILQ_REMOVE(&nm->info, nim, entries);
				node->ninfo++;
				inserted = 1;
				break;
			}
		}

		if (inserted != 1) {
			TAILQ_INSERT_TAIL(&node->info, new, entries);
			TAILQ_REMOVE(&nm->info, nim, entries);
			node->ninfo++;
		}

		update_indexes(node);
	}

	xfree(&nim);
}

static void
nss_criteria_remove(struct nss_entry *node,
	struct nss_modification *nm, struct nss_info_modification *nim)
{
	struct nss_entry_info *nei, *neitmp;

	TAILQ_FOREACH_SAFE(nei, &node->info, entries, neitmp) {
		if (nei->type == NSS_ENTRY_INFO_CRITERIA &&
			nei->neic_status == nim->nimc_status) {
			TAILQ_REMOVE(&node->info, nei, entries);
			node->ninfo--;
			xfree(&nei);
			break;
		}
	}
}

static int
do_nss_op_modify(struct nss_modification *nm, int reset)
{
	struct nss_entry *node;
	struct nss_info_modification *nim, *nimtmp;

	if (nm == NULL)
		return (-1);

	if (reset == 1)
		reset_database_info(nm);

	if ((node = nss_entry_by_database(nm->nm_database)) == NULL)
		return (-1);

	TAILQ_FOREACH_SAFE(nim, &nm->info, entries, nimtmp) {
		switch (nim->op) {
			case NSS_SOURCE_ADD:
				nss_source_add(node, nm, nim);
				break;

			case NSS_SOURCE_REMOVE:
				nss_source_remove(node, nm, nim);
				break;

			case NSS_CRITERIA_ADD:
				nss_criteria_add(node, nm, nim);
				break;

			case NSS_CRITERIA_REMOVE:
				nss_criteria_remove(node, nm, nim);
				break;
		}
	}

	return (0);
}

static int
do_nss_op_add(struct nss_modification *nm)
{
	struct nss_entry *ne = xalloc(sizeof(*ne));

	ne->type = NSS_ENTRY_DATABASE;
	ne->ned_database = xstrdup(nm->nm_database);

	TAILQ_INIT(&ne->info);
	TAILQ_INSERT_TAIL(&fentries, ne, entries);

	return (do_nss_op_modify(nm, 0));
}

static int
do_nss_op_remove(struct nss_modification *nm)
{
	struct nss_entry *node;
	struct nss_entry_info *nei, *neitmp;

	if ((node = nss_entry_by_database(nm->nm_database)) == NULL)
		return (-1);

	TAILQ_FOREACH_SAFE(nei, &node->info, entries, neitmp) {
		switch (nei->type) {
			case NSS_ENTRY_INFO_SOURCE:
				TAILQ_REMOVE(&node->info, nei, entries);
				xfree(&nei->neis_source);
				break;
		}

		TAILQ_REMOVE(&node->info, nei, entries);
		xfree(&nei);
	}

	TAILQ_REMOVE(&fentries, node, entries);
	xfree(&node->ned_database);
	xfree(&node);
}

static int
do_modifications(void)
{
	struct nss_modification *nm, *nmtmp;

	TAILQ_FOREACH_SAFE(nm, &modifications, entries, nmtmp) {
		struct nss_entry *ne, *netmp;
		int reset = 0;

		switch (nm->op) {
			case NSS_DATABASE_ADD:
				if (do_nss_op_add(nm) < 0) {
					warnx("do_nss_op_add: failed\n");
					return (-1);
				}
				break;

			case NSS_DATABASE_RESET:
				reset = 1;

			case NSS_DATABASE_MODIFY:
				if (do_nss_op_modify(nm, reset) < 0) {
					warnx("do_nss_op_modify: failed\n");
					return (-1);
				}
				break;

			case NSS_DATABASE_REMOVE:
				do_nss_op_remove(nm);
				break;
		}


		TAILQ_REMOVE(&modifications, nm, entries);
		xfree(&nm->nm_database);
		xfree(&nm);
	}

	return (0);
}

static void
write_nsswitch_conf(void)
{
	struct nss_entry *ne;

	TAILQ_FOREACH(ne, &fentries, entries) {

		switch (ne->type) {
			case NSS_ENTRY_NULL:
				fprintf(yyout, "\n");
				break;

			case NSS_ENTRY_DATABASE: {
				struct nss_entry_info *nei;
				int c = 0;

				if (ne->ninfo <= 0)
					break;

				fprintf(yyout, "%s:", ne->ned_database);

				TAILQ_FOREACH(nei, &ne->info, entries) {
					switch (nei->type) {
						case NSS_ENTRY_INFO_SOURCE:
							if (c == 1) {
								c = 0;
								fprintf(yyout, " ]");
							}

							fprintf(yyout, " %s", nei->neis_source);
							break;

						case NSS_ENTRY_INFO_CRITERIA:
							if (c == 0) {
								c = 1;
								fprintf(yyout, " [");
							}

							fprintf(yyout, " %s=%s",
								status2str(nei->neic_status),
								action2str(nei->neic_action)
							);

							break;
					}
				}

				if (c == 1) {
					c = 0;
					fprintf(yyout, " ]");
				}

				fprintf(yyout, "\n");

				break;
			}

			case NSS_ENTRY_COMMENT:
				fprintf(yyout, "%s\n", ne->nec_comment);
				break;
		}

	}
}

static struct nss_modification *
new_nss_modification(void)
{
	struct nss_modification *nm = xalloc(sizeof(*nm));

	nm->op = 0;
	bzero(&nm->nd, sizeof(nm->nd));
	TAILQ_INIT(&nm->info);

	return (nm);
}


static int
set_database(struct nss_modification *nm, const char *str)
{
	if (nm == NULL || str == NULL)
		return (-1);

	switch (str[0]) {
		case '-':
			nm->op = NSS_DATABASE_REMOVE;
			str += 1;
			break;

		case '^':
			nm->op = NSS_DATABASE_MODIFY;
			str += 1;
			break;

		case '+':
			nm->op = NSS_DATABASE_ADD;
			str += 1;
			break;

		default:
			nm->op =  NSS_DATABASE_MODIFY;
			break;
	}

	nm->nm_database = xstrdup(str);
	return (0);
}

static int
set_criteria(struct nss_info_modification *nim, const char *str)
{
	char *status, *action, *tmp, *ptr;

	if (nim == NULL || str == NULL)
		return (-1);

	switch (str[0]) {
		case '-':
			nim->op = NSS_CRITERIA_REMOVE;
			str += 1;
			break;

		case '+':
			str += 1;
		default:
			nim->op = NSS_CRITERIA_ADD;
			break;
	}

	ptr = NULL;
	nim->index = strtol(str, &ptr, 10);
	if (ptr == str)
		nim->index = -1;

	tmp = xstrdup(ptr);
	ptr = tmp;

	status = strsep(&tmp, "=");
	action = tmp;

	if (action == NULL || action[0] == 0) {
		xfree(&ptr);
		return (-1);
	}

	if ((nim->nimc_status = str2status(status)) == 0)
		return (-1);
	if ((nim->nimc_action = str2action(action)) == 0)
		return (-1);

	nim->type = NSS_ENTRY_INFO_CRITERIA;
	xfree(&ptr);

	return (0);
}

static int
set_source(struct nss_info_modification *nim, const char *str)
{
	char *ptr;

	if (nim == NULL || str == NULL)
		return (-1);

	switch (str[0]) {
		case '-':
			nim->op = NSS_SOURCE_REMOVE;
			str += 1;
			break;

		case '+':
			str += 1;
		default:
			nim->op = NSS_SOURCE_ADD;
			break;
	}

	ptr = NULL;
	nim->index = strtol(str, &ptr, 10);
	if (ptr == str)
		nim->index = -1;

	nim->nims_source = xstrdup(ptr);
	nim->type = NSS_ENTRY_INFO_SOURCE;
	return (0);
}


int
main(int argc, char **argv)
{
	int ch;
	char *infile, *outfile, *database;
	struct nss_modification *nm;

	if (argc <= 1)
		usage();

	nm = NULL;
	infile = outfile = NULL;
	while ((ch = getopt(argc, argv, "f:d:c:s:ro:")) != -1) {
		switch (ch) {
			case 'f':
				xfree(&infile);
				infile = xstrdup(optarg);
				break;

			case 'd':
				nm = new_nss_modification();
				set_database(nm, optarg);
				TAILQ_INSERT_TAIL(&modifications, nm, entries);
				break;

			/*
			 *	Only insert criterion nodes if a database node has been
			 *	allocated.
			 */
			case 'c': {
				if (nm != NULL) {
					struct nss_info_modification *nim = xalloc(sizeof(*nim));
					set_criteria(nim, optarg);
					TAILQ_INSERT_TAIL(&nm->info, nim, entries);
				}
				break;
			}
				
			/*
			 *	Only insert source nodes if a database node has been
			 *	allocated.
			 */
			case 's': {
				if (nm != NULL) {
					struct nss_info_modification *nim = xalloc(sizeof(*nim));
					set_source(nim, optarg);
					TAILQ_INSERT_TAIL(&nm->info, nim, entries);
				}
				break;
				
			}

			/*
			 *	"Reset" a database node.
			 */
			case 'r':
				if (nm != NULL)
					nm->op = NSS_DATABASE_RESET;
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

	if (do_modifications() < 0)
		errx(EX_DATAERR, "parameter error");

	write_nsswitch_conf();

	nsswitch_conf_free();
	xfree(&outfile);
	xfree(&infile);

	if (yyout != NULL)
		fclose(yyout);
	if (yyin != NULL)
		fclose(yyin);

	return (0);
}
