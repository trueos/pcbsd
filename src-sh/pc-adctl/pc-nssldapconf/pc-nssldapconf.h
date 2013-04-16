/*
 *	pc-nssldapconf.h
 *
 *	nssldap macros and stuff
 */

#ifndef	__PC_NSSLDAPCONF_H
#define	__PC_NSSLDAPCONF_H

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

#define	NSSLDAP_ENTRY_NULL		0x00
#define	NSSLDAP_ENTRY_PAIR		0x01
#define	NSSLDAP_ENTRY_TRIPLET	0x02
#define	NSSLDAP_ENTRY_COMMENT	0x04

struct nssldap_comment {
	char *text;
};

struct nssldap_triplet {
	char *name;
	char *attr;
	char *value;
};

struct nssldap_pair {
	char *name;
	char *value;
};

struct nssldap_entry {
	unsigned int type;
	union {
		struct nssldap_pair ne_pair;
		struct nssldap_triplet ne_triplet;
		struct nssldap_comment ne_comment;
	} ne;

#define	nep	ne.ne_pair
#define	net	ne.ne_triplet
#define	nec	ne.ne_comment

#define	nep_name	nep.name
#define	nep_value	nep.value

#define	net_name	net.name
#define	net_attr	net.attr
#define	net_value	net.value

#define	nec_text	nec.text

	TAILQ_ENTRY(nssldap_entry) entries;
};

TAILQ_HEAD(nssldap_entry_list, nssldap_entry);

extern  void *xalloc(size_t);
extern  void _xfree(char **);
extern  char *xstrdup(const char *);

#define xfree(p) _xfree((char **)p)

extern	char *clean(char *);

#endif	/* __PC_NSSLDAPCONF_H */
