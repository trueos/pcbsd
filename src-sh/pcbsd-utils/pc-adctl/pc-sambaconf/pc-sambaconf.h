/*
 *	pc-sambaconf.h
 *
 *	samba macros and stuff
 */

#ifndef	__PC_SAMBACONF_H
#define	__PC_SAMBACONF_H

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

#define	SMB_ENTRY_NULL			0x00
#define	SMB_ENTRY_SECTION		0x01
#define	SMB_ENTRY_PARAMETERS	0x02
#define	SMB_ENTRY_COMMENT		0x04

struct smb_parameters {
	char *name;
	char *value;
	TAILQ_ENTRY(smb_parameters) entries;
};
TAILQ_HEAD(smb_parameters_list, smb_parameters);

struct smb_section {
	char *name;
	struct smb_parameters_list parameters;
};
TAILQ_HEAD(smb_sections_list, smb_section);

struct smb_comment {
	char *text;
};
TAILQ_HEAD(smb_comments_list, smb_comment);

struct smb_entry {
	unsigned int type;
	union {
		struct smb_parameters se_parameters;
		struct smb_section se_section;
		struct smb_comment se_comment;
	} se;

#define	sep	se.se_parameters	
#define	ses	se.se_section
#define	sec	se.se_comment

#define	sep_name		sep.name
#define	sep_value		sep.value

#define	ses_name		ses.name
#define	ses_parameters	ses.parameters

#define	sec_text		sec.text	

	TAILQ_ENTRY(smb_entry) entries;
};
TAILQ_HEAD(smb_entry_list, smb_entry);

#define	smb_null(x)	(x == NULL || x[0] == 0)

extern	void *xalloc(size_t);
extern	void _xfree(char **);
extern	char *xstrdup(const char *);

#define	xfree(p) _xfree((char **)p)

#endif	/* __PC_SAMBACONF_ */
