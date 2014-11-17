/*
 *	pc-nssconf.h
 *
 *	NSS macros and stuff
 */

#ifndef	__PC_NSSCONF_H
#define	__PC_NSSCONF_H

#include <sys/types.h>
#include <sys/queue.h>
#include <ctype.h>
#include <err.h>
#include <search.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sysexits.h>
#include <unistd.h>


#define	NSS_ENTRY_NULL		0x00
#define	NSS_ENTRY_DATABASE	0x01
#define	NSS_ENTRY_COMMENT	0x02

#define	NSS_ENTRY_INFO_NULL		0x00
#define	NSS_ENTRY_INFO_SOURCE	0x01
#define	NSS_ENTRY_INFO_CRITERIA	0x02

struct nss_comment {
	char *text;
};

struct nss_database {
	char *database;
};

struct nss_criteria {
	unsigned int status;
	unsigned int action;
};

struct nss_source {
	char *source;
};

struct nss_entry_info {
	unsigned int type;
	unsigned int index;
	union {
		struct nss_source n_source;
		struct nss_criteria n_criteria;
	} nei;

#define	neis	nei.n_source
#define	neic	nei.n_criteria

#define	neis_source	neis.source

#define	neic_status	neic.status
#define	neic_action	neic.action

	TAILQ_ENTRY(nss_entry_info) entries;
};

struct nss_entry {
	unsigned int type;
	union {
		struct nss_database n_database;
		struct nss_comment n_comment;
	} ne;

#define	ned	ne.n_database
#define	nec	ne.n_comment

#define	ned_database	ned.database
#define	nec_comment		nec.text

	unsigned int ninfo;

	TAILQ_HEAD(nss_entry_info_list, nss_entry_info) info;
	TAILQ_ENTRY(nss_entry) entries;
};
TAILQ_HEAD(nss_entry_list, nss_entry);

extern	void *xalloc(size_t);
extern	void _xfree(char **);

#define	xfree(p) _xfree((char **)p)

extern	char *xstrdup(const char *);
extern	char *clean(char *);

extern	const char *database2str(unsigned int);
extern	unsigned int str2database(const char *);

extern	const char *source2str(unsigned int);
extern	unsigned int str2source(const char *);

extern	const char *status2str(unsigned int);
extern	unsigned int str2status(const char *);

extern	const char *action2str(unsigned int);
extern	unsigned int str2action(const char *);

#endif	/* __PC_NSSCONF_ */
