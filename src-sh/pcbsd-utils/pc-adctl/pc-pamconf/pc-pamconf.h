/*
 *	pc-pamconf.h
 *
 *	pam macros and stuff
 */

#ifndef	__PC_PAMCONF_H
#define	__PC_PAMCONF_H

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

#define	PAM_ENTRY_NULL		0x00
#define	PAM_ENTRY_SERVICE	0x01
#define	PAM_ENTRY_INCLUDE	0x02
#define	PAM_ENTRY_COMMENT	0x04
#define	PAM_ENTRY_ARGMAX	64

struct pam_service {
	unsigned int control;
	char *module;
	char **arguments;
	unsigned int narguments;
};

struct pam_include {
	char *service;
};

struct pam_comment {
	char *text;
};

struct pam_entry {
	unsigned int index;
	unsigned int type;
	unsigned int lineno;
	unsigned int facility;
	union {
		struct pam_service p_service;
		struct pam_include p_include;
		struct pam_comment p_comment;
	} pe;

#define	pes	pe.p_service
#define	pei	pe.p_include
#define	pec	pe.p_comment

#define	pes_control		pes.control
#define	pes_module		pes.module
#define	pes_arguments	pes.arguments
#define pes_narguments	pes.narguments

#define	pei_service		pei.service

#define	pec_text		pec.text

	TAILQ_ENTRY(pam_entry) entries;
	TAILQ_ENTRY(pam_entry) fentries;
};

TAILQ_HEAD(pam_entry_list, pam_entry);

struct pam_config {
	unsigned int pc_auth_nentries;
	struct pam_entry_list pc_auth;

	unsigned int pc_account_nentries;
	struct pam_entry_list pc_account;

	unsigned int pc_session_nentries;
	struct pam_entry_list pc_session;

	unsigned int pc_password_nentries;
	struct pam_entry_list pc_password;

	unsigned int pc_comment_nentries;
	struct pam_entry_list pc_comment;
};

#define	pe_null(pe)		(pe->type == PAM_ENTRY_NULL)
#define	pe_service(pe)	(pe->type == PAM_ENTRY_SERVICE)
#define	pe_include(pe)	(pe->type == PAM_ENTRY_INCLUDE)
#define	pe_comment(pe)	(pe->type == PAM_ENTRY_COMMENT)

#define	facility_auth(pe)		(pe->facility == AUTH)
#define	facility_account(pe)	(pe->facility == ACCOUNT)	
#define	facility_session(pe)	(pe->facility == SESSION)
#define	facility_password(pe)	(pe->facility == PASSWORD)

#define	control_required(pe)	(pe_service(pe) && pe->pes_control == REQUIRED)
#define	control_requisite(pe)	(pe_service(pe) && pe->pes_control == REQUISITE)
#define	control_sufficient(pe)	(pe_service(pe) && pe->pes_control == SUFFICIENT)
#define	control_binding(pe)		(pe_service(pe) && pe->pes_control == BINDING)
#define	control_optional(pe)	(pe_service(pe) && pe->pes_control == OPTIONAL)

extern	void *xalloc(size_t);
extern	void _xfree(char **);
extern	char *xstrdup(const char *);

#define	xfree(p) _xfree((char **)p)

extern	struct pam_entry *new_pam_entry(int facility);
extern	void free_arguments(struct pam_entry *pe);
extern	void free_pam_entry(struct pam_entry *pe);
extern	void free_pam_entries(struct pam_entry_list *pe_list);


#endif	/* __PC_PAMCONF_H */
