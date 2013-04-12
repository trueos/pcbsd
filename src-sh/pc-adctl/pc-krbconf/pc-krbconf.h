/*
 *	pc-krbconf.h
 *
 *	krb5.conf macros and stuff
 */

#ifndef	__PC_KRBCONF_H
#define	__PC_KRBCONF_H

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

#define	KRB_ENTRY_NULL			0x0000
#define	KRB_ENTRY_SECTION		0x0001
#define	KRB_ENTRY_BINDING		0x0002
#define	KRB_ENTRY_BINDING_START	0x0004
#define	KRB_ENTRY_BINDING_END	0x0008
#define	KRB_ENTRY_COMMENT		0x0010

TAILQ_HEAD(krb_entry_list, krb_entry);

struct krb_comment {
	char *text;
};

struct krb_binding {
	char *name;
	char *value;
	unsigned int nbindings;
	struct krb_entry_list bindings;
};

struct krb_section {
	char *name;  
	unsigned int nbindings;
	struct krb_entry_list bindings;
};

struct krb_entry {
	unsigned int type;
	union {
		struct krb_section ks;
		struct krb_binding kb;
		struct krb_comment kc;
	} ke;

#define	kes	ke.ks
#define	keb	ke.kb
#define	kec	ke.kc

#define	kes_name		kes.name
#define	kes_nbindings	kes.nbindings
#define	kes_bindings	kes.bindings
#define	kes_entries		kes.entries

#define	keb_name		keb.name
#define	keb_value		keb.value
#define	keb_nbindings	keb.nbindings
#define	keb_bindings	keb.bindings
#define	keb_entries		keb.entries

#define	kec_text		kec.text

	TAILQ_ENTRY(krb_entry) entries;
	TAILQ_ENTRY(krb_entry) fentries;
};

extern	void *xalloc(size_t);
extern	void _xfree(char **);
    
#define	xfree(p) _xfree((char **)p)

extern	char *xstrdup(const char *);
extern	char *clean(char *);

#endif	/* __PC_KRBCONF_H */
