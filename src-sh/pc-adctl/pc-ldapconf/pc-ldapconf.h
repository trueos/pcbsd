/*
 *	pc-ldapconf.h
 *
 *	ldap macros and stuff
 */

#ifndef	__PC_LDAPCONF_H
#define	__PC_LDAPCONF_H

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

#define	LDAP_ENTRY_NULL					0x00
#define	LDAP_ENTRY_URI_LIST				0x02
#define	LDAP_ENTRY_HOST_LIST			0x04
#define	LDAP_ENTRY_SASL_SECPROPS_LIST	0x08
#define	LDAP_ENTRY_COMMENT				0x10

/*
 *	OPTIONS
 */
struct ldap_uri {
	char *uri;
	TAILQ_ENTRY(ldap_uri) entries;
};
TAILQ_HEAD(ldap_uri_list, ldap_uri);
struct ldap_base {
	char *base;
};
struct ldap_binddn {
	char *dn;
};
struct ldap_deref {
	unsigned long when;
};
struct ldap_host {
	char *host;
	TAILQ_ENTRY(ldap_host) entries;
};
TAILQ_HEAD(ldap_host_list, ldap_host);
struct ldap_network_timeout {
	unsigned long timeout;
};
struct ldap_port {
	unsigned long port;
};
struct ldap_referrals {
	unsigned long option;
};
struct ldap_sizelimit {
	unsigned long sizelimit;
};
struct ldap_timelimit {
	unsigned long timelimit;
};
struct ldap_timeout {
	unsigned long timeout;
};


/*
 *	SASL OPTIONS
 */
struct sasl_mech {
	char *mechanism;
};
struct sasl_realm {
	char *realm;
};
struct sasl_authcid {
	char *authcid;
};
struct sasl_authzid {
	char *authzid;
};
struct sasl_secprops {
	unsigned long properties;
	long factor;
	TAILQ_ENTRY(sasl_secprops) entries;
};
TAILQ_HEAD(sasl_secprops_list, sasl_secprops);


/*
 *	GSSAPI OPTIONS
 */
struct gssapi_sign {
	unsigned long option;
};
struct gssapi_encrypt {
	unsigned long option;
};
struct gssapi_allow_remote_principal {
	unsigned long option;
};


/*
 *	TLS OPTIONS
 */
struct tls_cacert {
	char *filename;
};
struct tls_cacertdir {
	char *path;
};
struct tls_cert {
	char *filename;
};
struct tls_key {
	char *filename;
};
struct tls_cipher_suite {
	char *cipher_suite_spec;
};
struct tls_randfile {
	char *filename;
};
struct tls_reqcert {
	unsigned long level;
};
struct tls_crlcheck {
	unsigned long level;
};
struct tls_crlfile {
	char *filename;
};


struct ldap_comment {
	char *text;
};

struct ldap_pair {
	char *name;
	char *value;
};

struct ldap_entry {
	unsigned int type;
	void *entry;
	TAILQ_ENTRY(ldap_entry) entries;
};

TAILQ_HEAD(ldap_entry_list, ldap_entry);

extern  void *xalloc(size_t);
extern  void _xfree(char **);
extern  char *xstrdup(const char *);

#define xfree(p) _xfree((char **)p)

extern	char *clean(char *);

extern	const char *deref2str(unsigned long, const char *);
extern	unsigned long str2deref(const char *, unsigned long);

extern	const char *bool2str(unsigned long, const char *);
extern	unsigned long str2bool(const char *, unsigned long);

extern	const char *level2str(unsigned long, const char *);
extern	unsigned long str2level(const char *, unsigned long);

extern	const char *properties2str(unsigned long, const char *);
extern	unsigned long str2properties(const char *, unsigned long);

extern	unsigned long str2type(const char *);
extern	const char *type2str(unsigned long);

#endif	/* __PC_LDAPCONF_H */
