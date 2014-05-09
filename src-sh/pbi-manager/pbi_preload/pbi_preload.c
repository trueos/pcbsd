/*
 *  AUTHOR: Kris Moore <kris@pcbsd.org>
 * LICENSE: BSD
 * COMMENT: Pre-Loaded library to intercept PBI file-system calls to /usr/local
 * COMPILE: cc -fPIC -c -o pbi_preload.o pbi_preload.c
 *          cc -shared -o pbi_preload.so pbi_preload.o
 */

#include "dlfcn.h"
#include <dirent.h>
#include <errno.h>
#include <libgen.h>
#include <machine/stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/acl.h>
#include <sys/param.h>
#include <sys/mac.h>
#include <sys/types.h>

// Don't grab readlink declare
#undef __POSIX_VISIBLE
#undef __XSI_VISIBLE
#undef __BSD_VISIBLE
#include <unistd.h>

#define _KERNEL
#include <fcntl.h>

// No haystack match
#define NOT_FOUND -1

// Enable this to get debug of all system calls
//#define DO_DEBUG 1

// Some dummy struct we need to compile
struct acl;
struct stat;
struct statfs;
struct timeval;
typedef struct Struct_Obj_Entry {
} Obj_Entry;

// Set some globals
char hintsfile[MAXPATHLEN];
char hints32[MAXPATHLEN];
char pbidir[MAXPATHLEN];
char username[MAXPATHLEN];
char hashdir[MAXPATHLEN];
char linuxdir[MAXPATHLEN];

// Have we done our init yet?
int do_init = 0;
int first_dlopen=0;

// Export errorno
extern int errorno;

// Define this
size_t readlink(const char *restrict path, char *restrict buf, size_t bufsiz);
int chdir(const char *path);

void dbug(const char *funcmsg, const char *oldstr, const char *newstr)
{
#ifdef DO_DEBUG
	if ( do_init != 1 )
		return;
	printf("%s: %s -> %s\n", funcmsg, oldstr, newstr);	
#endif
}

void first_init()
{
   // We need to init which PBI we are using
   char upbi[MAXPATHLEN];
   char pbiname[MAXPATHLEN];
   strcpy(upbi, getenv("PBI_RUNDIR"));
   if ( upbi == NULL ) {
      exit(1);
   }

   // Set the local / linux variables
   strcpy(pbidir, upbi);
   strcat(pbidir, "/local");
   strcpy(linuxdir, upbi);
   strcat(linuxdir, "/linux");

   // Get the PBI name
   strcpy(pbiname, basename(upbi));

   // Now setup the hintsfiles for this runtime
   strcpy(hintsfile, "/var/run/ld-elf.so.hints.");
   strcat(hintsfile, pbiname);
   strcpy(hints32, "/var/run/ld-elf32.so.hints.");
   strcat(hints32, pbiname);

   // Get the process username
   strcpy(username, getlogin());
   strcpy(hashdir, "/usr/pbi/.hashdir-");
   strcat(hashdir, username);

   // Yay, init is all done!
   do_init=1;
}

int strpos(const char *haystack, char *needle)
{
   char *p = strstr(haystack, needle);
   if (p) {
      return p - haystack;
   }
   return NOT_FOUND;
}

static char *rstrstr(const char *haystack, const char *needle)
{
    if (*needle == '\0')
        return (char *) haystack;

    char *result = NULL;
    for (;;) {
        char *p = strstr(haystack, needle);
        if (p == NULL)
            break;
        result = p;
        haystack = p + 1;
    }

    return result;
}

char *replace_str_all(const char *str, const char *old, const char *new)
{
	char *ret, *r;
	const char *p, *q;
	size_t oldlen = strlen(old);
	size_t count, retlen, newlen = strlen(new);

	if (oldlen != newlen) {
		for (count = 0, p = str; (q = strstr(p, old)) != NULL; p = q + oldlen)
			count++;
		/* this is undefined if p - str > PTRDIFF_MAX */
		retlen = p - str + strlen(p) + count * (newlen - oldlen);
	} else
		retlen = strlen(str);

	if ((ret = malloc(retlen + 1)) == NULL)
		return NULL;

	for (r = ret, p = str; (q = strstr(p, old)) != NULL; p = q + oldlen) {
		/* this is undefined if q - p > PTRDIFF_MAX */
		ptrdiff_t l = q - p;
		memcpy(r, p, l);
		r += l;
		memcpy(r, new, newlen);
		r += newlen;
	}
	strcpy(r, p);

	return ret;
}

const char *replace_str(const char *str, char *orig, char *rep)
{
	static char buffer[4096];
	char *p;

	if(!(p = strstr(str, orig)))  // Is 'orig' even in 'str'?
		return str;

	strncpy(buffer, str, p-str); // Copy characters from 'str' start to 'orig' st$
	buffer[p-str] = '\0';

	sprintf(buffer+(p-str), "%s%s", rep, p+strlen(orig));

	return buffer;
}

/**********************************************************************************
*  OK, this is where all the path re-mapping happens.
*  We are going to be taking the current filesystem path, and checking it against
*  a list of ones we want to "virtualize" so that it appears we have a different
*  /usr/local namespace
***********************************************************************************/
int get_modified_path(char *npath, const char *opath, int resolvpath)
{ 
	// Just break out now if path == NULL
	if ( opath == NULL || opath[0] == '\0') {
		return -1;
	}

	// If the process is requesting to breakout of the container
	if ( getenv("PBI_BREAKOUT") != NULL )
	{
		strcpy(npath, opath);
		return 0;
	}

	// Get the real pathname we are using now
	char rPath[MAXPATHLEN];
	// Check if this is a relative path, or absolute
	if ( strpos(opath, "/") == 0 ) {
		strcpy(rPath, opath);
	} else {
		// Check if we want pathname resolution
		if ( resolvpath == 0 )
		{
			getcwd(rPath, MAXPATHLEN);
			// If the cwd is just '/' then don't add anything to string
			if ( strcmp(rPath, "/") == 0 ) {
				strcpy(rPath, opath);
			} else {
				strcat(rPath, "/");
				strcat(rPath, opath);
			}
		} else {
			strcpy(rPath, opath);
		}
	}

	// First, lets look for calls to /var/run/ld-elf*.so.hints
	// These need to be replaced with calls to our new hints file in hintsfile
	if ( strpos(rPath, "/var/run") == 0 )
	{
		if ( strcmp(rPath, "/var/run/ld-elf.so.hints") == 0 )
		{
			// Check if we need to do our first init
			if ( do_init == 0 )
	   			first_init();

			strcpy(npath, hintsfile);
			return 0;
		}
		if ( strcmp(rPath, "/var/run/ld-elf32.so.hints") == 0 )
		{
			// Check if we need to do our first init
			if ( do_init == 0 )
	   			first_init();

			strcpy(npath, hints32);
			return 0;
		}
	}

	if ( strcmp(rPath, "/etc/rc.d/ldconfig") == 0 )
	{
		strcpy(npath, "/usr/pbi/.ldconfig");
		return 0;
	}

	// Check if we have a virtual linux /compat directory to fake as well
	if ( strpos(rPath, "/compat/linux") == 0 )
	{
		// If this is /compat/linux/proc, lets use the systems
		if ( strpos(rPath, "/compat/linux/proc") == 0 ) {
			strcpy(npath, rPath);
			return 0;
		}
		// If this is /compat/linux/sys, lets use the systems
		if ( strpos(rPath, "/compat/linux/sys") == 0 ) {
			strcpy(npath, rPath);
			return 0;
		}

		// Check if we need to do our first init
		if ( do_init == 0 )
	   		first_init();

		if (0 == access(linuxdir, F_OK))
		{
			strcpy(npath, replace_str(rPath, "/compat/linux", linuxdir));
			return 0;
		}
		strcpy(npath, rPath);
		return 0;
	}

	// Lastly, lets do all the parsing for /usr/local matching
	if ( strpos(rPath, "/usr/local") == 0 )
	{
		// Check for some of the breakout commands
		if ( strpos(rPath, "/usr/local/bin/openwith") == 0 ) {
			strcpy(npath, "/usr/bin/openwith");
			return 0;
		}
		if ( strpos(rPath, "/usr/local/bin/xdg-open") == 0 ) {
			strcpy(npath, "/usr/bin/openwith");
			return 0;
		}
		if ( strpos(rPath, "/usr/local/bin/pbisyscmd") == 0 ) {
			strcpy(npath, "/usr/bin/pbisyscmd");
			return 0;
		}

		// Use the systems copies of some font / icon directories
		if ( strpos(rPath, "/usr/local/etc/fonts") == 0 ) {
			strcpy(npath, rPath);
			return 0;
		}
		if ( strpos(rPath, "/usr/local/lib/X11/fonts") == 0 ) {
			strcpy(npath, rPath);
			return 0;
		}
		if ( strpos(rPath, "/usr/local/lib/X11/icons") == 0 ) {
			strcpy(npath, rPath);
			return 0;
		}
		if ( strpos(rPath, "/usr/local/share/mime") == 0 )
		{
			strcpy(npath, rPath);
			return 0;
		}
		if ( strpos(rPath, "/usr/local/share/icons") == 0 )
		{
			strcpy(npath, rPath);
			return 0;
		}
		if ( strpos(rPath, "/usr/local/share/font-") == 0 ) {
			strcpy(npath, rPath);
			return 0;
		}

		// Check if we need to do our first init
		if ( do_init == 0 )
	   		first_init();

		strcpy(npath, replace_str(rPath, "/usr/local", pbidir));
		return 0;
	}

	// Any special matching for /usr/pbi
	if ( strpos(rPath, "/usr/pbi") == 0 )
	{

		// Check if we need to do our first init
		if ( do_init == 0 )
			first_init();

		// The PBI shouldn't be mucking about in our hard-linked hashdir
		if ( strpos(rPath, hashdir) == 0 ) {
			strcpy(npath, replace_str(rPath, hashdir, pbidir));
			int hashloc = strpos(npath, ":::");
			if ( hashloc != -1 )
				npath[hashloc] = '\0';
			return 0;
		}

	}

	strcpy(npath, rPath);
	return 0;
}

int get_link_path(char *npath, const char *opath)
{ 
        char linkpath[MAXPATHLEN];
        char tmppath[MAXPATHLEN];
        char tmp[MAXPATHLEN];
        char origpath[MAXPATHLEN];
	int didchdir=0;

	if ( opath == NULL )
		return -1;
	
	// Dont need to resolve links that aren't absolute or in /usr/local && /usr/pbi
	if ( ( strpos(opath, "/usr/local") != 0 ) && (strpos(opath, "/usr/pbi") != 0) )
		return -1;
	
	typeof(chdir) *sys_chdir;
        sys_chdir = dlsym(RTLD_NEXT, "chdir");
	typeof(readlink) *sys_readlink;
        sys_readlink = dlsym(RTLD_NEXT, "readlink");
        char curdir[MAXPATHLEN];
        char *newdir;
	int size;

	// See if we can remove a trailing /
	strcpy(origpath, opath);
	size_t len = strlen(origpath);
	if((len > 0) && (origpath[len-1] == '/'))
		origpath[len-1] = '\0';


	// Setup our intial check on the requested path
	strcpy(tmppath, origpath);
	getcwd(curdir, MAXPATHLEN);

	for(;;) {
		newdir = dirname(tmppath);
		if ( strpos(tmppath, "/usr/pbi") == 0 && strcmp(curdir, newdir) != 0) {
			didchdir=1;
			(*sys_chdir)(newdir);
		}

		// See if this is a symlink we are path'ing
		size = sys_readlink(tmppath, linkpath, sizeof(linkpath) );
		if ( size != -1 )
		{
			// Add the null term
			linkpath[size]='\0';

			// Get the modified path for this link
			dbug("link_redirect()", tmppath, linkpath);
			get_modified_path(npath, linkpath, 0);

			// Get the string after the sym-link resolution
			strcpy(tmp, replace_str(origpath, tmppath, ""));
			dbug("Fragment:", tmp, tmp);

			// Add the fragment back to the new path
			strcat(npath, tmp);
			dbug("link_redirect2()", opath, npath);

			// Change dir back to orig
			if ( didchdir == 1 )
				(*sys_chdir)(curdir);
			return 0;
		}

		if ( didchdir == 1 )
			(*sys_chdir)(curdir);

		// If we have reached the end of the path we can break out now
		strcpy(tmp, dirname(tmppath));
		strcpy(tmppath, tmp);
		if ( strcmp(tmppath, "/usr/pbi") == 0 || \
			 strcmp(tmppath, "/usr/local") == 0 || \
			 strcmp(tmppath, "/") == 0 )
		{
			break;
		}
	}

	return -1;
}

/*
 *************************************************
 * Capture the following filesystem calls
 * Any call which could potentially try to access
 * /usr/local, /var/run/ldhints* and the like need
 * to be captured and re-directed below 
 *************************************************
 */

int access(const char *path, int mode)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);
        get_link_path(newpath, newpath);

	dbug("access()", path, newpath);
	typeof(access) *sys_access;
        sys_access = dlsym(RTLD_NEXT, "access");
	return (*sys_access)(newpath, mode);
}

int _access(const char *path, int mode)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);
        get_link_path(newpath, newpath);

	dbug("access()", path, newpath);
	typeof(_access) *sys_access;
        sys_access = dlsym(RTLD_NEXT, "_access");
	return (*sys_access)(newpath, mode);
}

int __acl_aclcheck_file(const char *_path, acl_type_t _type, struct acl *_aclp)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, _path, 0);

	dbug("__acl_aclcheck_file()", _path, newpath);
	typeof(__acl_aclcheck_file) *sys_acl;
        sys_acl = dlsym(RTLD_NEXT, "__acl_aclcheck_file");
	return (*sys_acl)(newpath, _type, _aclp);
}

acl_t acl_get_file(const char *path_p, acl_type_t type)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path_p, 0);

	dbug("acl_get_file()", path_p, newpath);
	typeof(acl_get_file) *sys_acl;
        sys_acl = dlsym(RTLD_NEXT, "acl_get_file");
	return (*sys_acl)(newpath, type);
}

acl_t _acl_get_file(const char *path_p, acl_type_t type)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path_p, 0);

	dbug("_acl_get_file()", path_p, newpath);
	typeof(_acl_get_file) *sys_acl;
        sys_acl = dlsym(RTLD_NEXT, "_acl_get_file");
        return (*sys_acl)(newpath, type);
}

int __acl_get_file(const char *_path, acl_type_t _type, struct acl *_aclp)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, _path, 0);

	dbug("__acl_get_file()", _path, newpath);
	typeof(__acl_get_file) *sys_acl;
        sys_acl = dlsym(RTLD_NEXT, "__acl_get_file");
        return (*sys_acl)(newpath, _type, _aclp);
}

int __acl_get_link(const char *_path, acl_type_t _type, struct acl *_aclp)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, _path, 0);

	dbug("__acl_get_link()", _path, newpath);
	typeof(__acl_get_link) *sys_acl;
        sys_acl = dlsym(RTLD_NEXT, "__acl_get_link");
        return (*sys_acl)(newpath, _type, _aclp);
}

acl_t acl_get_link_np(const char *path_p, acl_type_t type)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path_p, 0);

	dbug("acl_get_link_np()", path_p, newpath);
	typeof(acl_get_link_np) *sys_acl;
        sys_acl = dlsym(RTLD_NEXT, "acl_get_link_np");
        return (*sys_acl)(newpath, type);
}

acl_t _acl_get_link_np(const char *path_p, acl_type_t type)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path_p, 0);

	dbug("_acl_get_link_np()", path_p, newpath);
	typeof(_acl_get_link_np) *sys_acl;
        sys_acl = dlsym(RTLD_NEXT, "_acl_get_link_np");
        return (*sys_acl)(newpath, type);
}

int chdir(const char *path)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);
        get_link_path(newpath, newpath);

	dbug("chdir()", path, newpath);
	typeof(chdir) *sys_chdir;
        sys_chdir = dlsym(RTLD_NEXT, "chdir");
        return (*sys_chdir)(newpath);
}

int _chdir(const char *path)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);
        get_link_path(newpath, newpath);

	dbug("_chdir()", path, newpath);
	typeof(_chdir) *sys_chdir;
        sys_chdir = dlsym(RTLD_NEXT, "chdir");
        return (*sys_chdir)(newpath);
}

int chmod(const char *path, mode_t mode)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);
        get_link_path(newpath, newpath);

	dbug("chmod()", path, newpath);
	typeof(chmod) *sys_chmod;
        sys_chmod = dlsym(RTLD_NEXT, "chmod");
        return (*sys_chmod)(newpath, mode);
}

int _chmod(const char *path, mode_t mode)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);
        get_link_path(newpath, newpath);

	dbug("_chmod()", path, newpath);
	typeof(_chmod) *sys_chmod;
        sys_chmod = dlsym(RTLD_NEXT, "_chmod");
        return (*sys_chmod)(newpath, mode);
}

int chown(const char *path, uid_t owner, gid_t group)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);
        get_link_path(newpath, newpath);

	dbug("chown()", path, newpath);
	typeof(chown) *sys_chown;
        sys_chown = dlsym(RTLD_NEXT, "chown");
        return (*sys_chown)(newpath, owner, group);
}

int _chown(const char *path, uid_t owner, gid_t group)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);
        get_link_path(newpath, newpath);

	dbug("_chown()", path, newpath);
	typeof(_chown) *sys_chown;
        sys_chown = dlsym(RTLD_NEXT, "_chown");
        return (*sys_chown)(newpath, owner, group);
}

int creat(const char *path, mode_t mode)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);

	dbug("creat()", path, newpath);
	typeof(creat) *sys_creat;
        sys_creat = dlsym(RTLD_NEXT, "creat");
        return (*sys_creat)(newpath, mode);
}

int _creat(const char *path, mode_t mode)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);

	dbug("_creat()", path, newpath);
	typeof(_creat) *sys_creat;
        sys_creat = dlsym(RTLD_NEXT, "_creat");
        return (*sys_creat)(newpath, mode);
}

//extern void *libc_dlopen(const char *path, int mode);
void * dlopen(const char *path, int mode)
{
       	char newpath[MAXPATHLEN];
       	get_modified_path(newpath, path, 1);
        get_link_path(newpath, newpath);

	dbug("dlopen()", path, newpath);

	typeof(dlopen) *sys_dlopen;
       	sys_dlopen = dlsym(RTLD_NEXT, "dlopen");

	if ( path == NULL )
      		return (*sys_dlopen)(path, mode);
	else
      		return (*sys_dlopen)(newpath, mode);
}

int eaccess(const char *path, int mode)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);

	dbug("eaccess()", path, newpath);
	typeof(eaccess) *sys_eaccess;
        sys_eaccess = dlsym(RTLD_NEXT, "eaccess");
        return (*sys_eaccess)(newpath, mode);
}

int _eaccess(const char *path, int mode)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);

	dbug("_eaccess()", path, newpath);
	typeof(_eaccess) *sys_eaccess;
        sys_eaccess = dlsym(RTLD_NEXT, "_eaccess");
        return (*sys_eaccess)(newpath, mode);
}

int exect(const char *path, char *const argv[], char *const envp[])
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 1);
        get_link_path(newpath, newpath);

	dbug("exect()", path, newpath);
	typeof(exect) *sys_exect;
        sys_exect = dlsym(RTLD_NEXT, "exect");
        return (*sys_exect)(newpath, argv, envp);
}

int _exect(const char *path, char *const argv[], char *const envp[])
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 1);
        get_link_path(newpath, newpath);

	dbug("_exect()", path, newpath);
	typeof(_exect) *sys_exect;
        sys_exect = dlsym(RTLD_NEXT, "_exect");
        return (*sys_exect)(newpath, argv, envp);
}

int execv(const char *path, char *const argv[])
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 1);
        get_link_path(newpath, newpath);

	dbug("execv()", path, newpath);
	typeof(execv) *sys_execv;
        sys_execv = dlsym(RTLD_NEXT, "execv");
        return (*sys_execv)(newpath, argv);
}

int _execv(const char *path, char *const argv[])
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 1);
        get_link_path(newpath, newpath);

	dbug("_execv()", path, newpath);
	typeof(_execv) *sys_execv;
        sys_execv = dlsym(RTLD_NEXT, "_execv");
        return (*sys_execv)(newpath, argv);
}

int execve(const char *path, char *const argv[], char *const envp[])
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 1);
        get_link_path(newpath, newpath);

	dbug("execve()", path, newpath);
	typeof(execve) *sys_execve;
        sys_execve = dlsym(RTLD_NEXT, "execve");
        return (*sys_execve)(newpath, argv, envp);
}

int _execve(const char *path, char *const argv[], char *const envp[])
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 1);
        get_link_path(newpath, newpath);

	dbug("_execve()", path, newpath);
	typeof(_execve) *sys_execve;
        sys_execve = dlsym(RTLD_NEXT, "_execve");
        return (*sys_execve)(newpath, argv, envp);
}

int execvp(const char *path, char *const argv[])
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 1);
        get_link_path(newpath, newpath);

	dbug("execvp()", path, newpath);
	typeof(execvp) *sys_execvp;
        sys_execvp = dlsym(RTLD_NEXT, "execvp");
        return (*sys_execvp)(newpath, argv);
}

int _execvp(const char *path, char *const argv[])
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 1);
        get_link_path(newpath, newpath);

	dbug("_execvp()", path, newpath);
	typeof(_execvp) *sys_execvp;
        sys_execvp = dlsym(RTLD_NEXT, "_execvp");
        return (*sys_execvp)(newpath, argv);
}

int flopen(const char *path, int flags, mode_t mode)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);
        get_link_path(newpath, newpath);

	dbug("flopen()", path, newpath);
	typeof(flopen) *sys_flopen;
	sys_flopen = dlsym(RTLD_NEXT, "flopen");
	return (*sys_flopen)(newpath, flags, mode);
}

int fchmodat(int fd, const char *path, mode_t mode, int flag)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);
        get_link_path(newpath, newpath);

	dbug("fchmodat()", path, newpath);
	typeof(fchmodat) *sys_fchmodat;
	sys_fchmodat = dlsym(RTLD_NEXT, "fchmodat");
	return (*sys_fchmodat)(fd, newpath, mode, flag);
}

int _fchmodat(int fd, const char *path, mode_t mode, int flag)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);
        get_link_path(newpath, newpath);

	dbug("_fchmodat()", path, newpath);
	typeof(_fchmodat) *sys_fchmodat;
	sys_fchmodat = dlsym(RTLD_NEXT, "_fchmodat");
	return (*sys_fchmodat)(fd, newpath, mode, flag);
}

int fchownat(int fd, const char *path, uid_t owner, gid_t group, int flag)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);
        get_link_path(newpath, newpath);

	dbug("fchownat()", path, newpath);
	typeof(fchownat) *sys_fchownat;
	sys_fchownat = dlsym(RTLD_NEXT, "fchownat");
	return (*sys_fchownat)(fd, newpath, owner, group, flag);
}

int _fchownat(int fd, const char *path, uid_t owner, gid_t group, int flag)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);
        get_link_path(newpath, newpath);

	dbug("_fchownat()", path, newpath);
	typeof(_fchownat) *sys_fchownat;
	sys_fchownat = dlsym(RTLD_NEXT, "_fchownat");
	return (*sys_fchownat)(fd, newpath, owner, group, flag);
}

FILE * fopen(const char * restrict path, const char * restrict mode)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);
        get_link_path(newpath, newpath);

	dbug("fopen()", path, newpath);
	typeof(fopen) *sys_fopen;
	sys_fopen = dlsym(RTLD_NEXT, "fopen");
	return (*sys_fopen)(newpath, mode);
}

FILE * _fopen(const char * restrict path, const char * restrict mode)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);
        get_link_path(newpath, newpath);

	dbug("_fopen()", path, newpath);
	typeof(_fopen) *sys_fopen;
	sys_fopen = dlsym(RTLD_NEXT, "_fopen");
	return (*sys_fopen)(newpath, mode);
}

int fstatat(int fd, const char *path, struct stat *stbuf, int flag)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);

	dbug("fstatat()", path, newpath);
	typeof(fstatat) *sys_fstatat;
        sys_fstatat = dlsym(RTLD_NEXT, "fstatat");
                         
        return (*sys_fstatat)(fd, newpath, stbuf, flag);
}

int _fstatat(int fd, const char *path, struct stat *stbuf, int flag)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);

	dbug("_fstatat()", path, newpath);
	typeof(_fstatat) *sys_fstatat;
        sys_fstatat = dlsym(RTLD_NEXT, "_fstatat");
                         
        return (*sys_fstatat)(fd, newpath, stbuf, flag);
}
int futimesat(int fd, const char *path, const struct timeval times[2])
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);

	dbug("futimesat()", path, newpath);
	typeof(futimesat) *sys_futimesat;
	sys_futimesat = dlsym(RTLD_NEXT, "futimesat");
	return (*sys_futimesat)(fd, newpath, times);
}

int _futimesat(int fd, const char *path, const struct timeval times[2])
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);

	dbug("_futimesat()", path, newpath);
	typeof(_futimesat) *sys_futimesat;
	sys_futimesat = dlsym(RTLD_NEXT, "_futimesat");
	return (*sys_futimesat)(fd, newpath, times);
}

int lchmod(const char *path, mode_t mode)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);
        get_link_path(newpath, newpath);

	dbug("lchmod()", path, newpath);
	typeof(lchmod) *sys_lchmod;
        sys_lchmod = dlsym(RTLD_NEXT, "lchmod");
        return (*sys_lchmod)(newpath, mode);
}

int _lchmod(const char *path, mode_t mode)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);
        get_link_path(newpath, newpath);

	dbug("_lchmod()", path, newpath);
	typeof(_lchmod) *sys_lchmod;
        sys_lchmod = dlsym(RTLD_NEXT, "_lchmod");
        return (*sys_lchmod)(newpath, mode);
}

int lchown(const char *path, uid_t owner, gid_t group)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);
        get_link_path(newpath, newpath);

	dbug("lchown()", path, newpath);
	typeof(lchown) *sys_lchown;
        sys_lchown = dlsym(RTLD_NEXT, "lchown");
        return (*sys_lchown)(newpath, owner, group);
}

int _lchown(const char *path, uid_t owner, gid_t group)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);
        get_link_path(newpath, newpath);

	dbug("_lchown()", path, newpath);
	typeof(_lchown) *sys_lchown;
        sys_lchown = dlsym(RTLD_NEXT, "_lchown");
        return (*sys_lchown)(newpath, owner, group);
}

int link(const char *name1, const char *name2)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, name2, 0);

	dbug("link()", name2, newpath);
	typeof(link) *sys_link;
        sys_link = dlsym(RTLD_NEXT, "link");
        return (*sys_link)(name1, newpath);
}

int _link(const char *name1, const char *name2)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, name2, 0);

	dbug("_link()", name2, newpath);
	typeof(_link) *sys_link;
        sys_link = dlsym(RTLD_NEXT, "_link");
        return (*sys_link)(name1, newpath);
}

int linkat(int fd1, const char *name1, int fd2, const char *name2, int flag)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, name2, 0);

	dbug("linkat()", name2, newpath);
	typeof(linkat) *sys_linkat;
        sys_linkat = dlsym(RTLD_NEXT, "linkat");
        return (*sys_linkat)(fd1, name1, fd2, newpath, flag);
}

int _linkat(int fd1, const char *name1, int fd2, const char *name2, int flag)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, name2, 0);

	dbug("linkat()", name2, newpath);
	typeof(_linkat) *sys_linkat;
        sys_linkat = dlsym(RTLD_NEXT, "_linkat");
        return (*sys_linkat)(fd1, name1, fd2, newpath, flag);
}

long lpathconf(const char *path, int name)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);

	dbug("lpathconf()", path, newpath);
	typeof(lpathconf) *sys_lpath;
        sys_lpath = dlsym(RTLD_NEXT, "lpathconf");
        return (*sys_lpath)(newpath, name);
}

long _lpathconf(const char *path, int name)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);

	dbug("_lpathconf()", path, newpath);
	typeof(_lpathconf) *sys_lpath;
        sys_lpath = dlsym(RTLD_NEXT, "_lpathconf");
        return (*sys_lpath)(newpath, name);
}

int lstat(const char *path, struct stat *sb)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);

	dbug("lstat()", path, newpath);
	typeof(lstat) *sys_lstat;
        sys_lstat = dlsym(RTLD_NEXT, "lstat");
        return (*sys_lstat)(newpath, sb);
}

int _lstat(const char *path, struct stat *sb)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);

	dbug("_lstat()", path, newpath);
	typeof(_lstat) *sys_lstat;
        sys_lstat = dlsym(RTLD_NEXT, "_lstat");
        return (*sys_lstat)(newpath, sb);
}

int lutimes(const char *path, const struct timeval *times)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);

	dbug("lutimes()", path, newpath);
	typeof(lutimes) *sys_lutimes;
	sys_lutimes = dlsym(RTLD_NEXT, "lutimes");
	return (*sys_lutimes)(newpath, times);
}

int _lutimes(const char *path, const struct timeval *times)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);

	dbug("_lutimes()", path, newpath);
	typeof(_lutimes) *sys_lutimes;
	sys_lutimes = dlsym(RTLD_NEXT, "_lutimes");
	return (*sys_lutimes)(newpath, times);
}

int mkdir(const char *path, mode_t mode)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);

	dbug("mkdir()", path, newpath);
	typeof(mkdir) *sys_mkdir;
        sys_mkdir = dlsym(RTLD_NEXT, "mkdir");
        return (*sys_mkdir)(newpath, mode);
}

int _mkdir(const char *path, mode_t mode)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);

	dbug("_mkdir()", path, newpath);
	typeof(_mkdir) *sys_mkdir;
        sys_mkdir = dlsym(RTLD_NEXT, "_mkdir");
        return (*sys_mkdir)(newpath, mode);
}

int mkdirat(int fd, const char *path, mode_t mode)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);

	dbug("mkdirat()", path, newpath);
	typeof(mkdirat) *sys_mkdirat;
        sys_mkdirat = dlsym(RTLD_NEXT, "mkdirat");
        return (*sys_mkdirat)(fd, newpath, mode);
}

int _mkdirat(int fd, const char *path, mode_t mode)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);

	dbug("_mkdirat()", path, newpath);
	typeof(_mkdirat) *sys_mkdirat;
        sys_mkdirat = dlsym(RTLD_NEXT, "_mkdirat");
        return (*sys_mkdirat)(fd, newpath, mode);
}

int mac_get_file(const char *path, mac_t label)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);

	dbug("mac_get_file()", path, newpath);
	typeof(mac_get_file) *sys_mac;
        sys_mac = dlsym(RTLD_NEXT, "mac_get_file");
                         
        return (*sys_mac)(newpath, label);
}

int _mac_get_file(const char *path, mac_t label)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);

	dbug("_mac_get_file()", path, newpath);
	typeof(_mac_get_file) *sys_mac;
        sys_mac = dlsym(RTLD_NEXT, "_mac_get_file");
                         
        return (*sys_mac)(newpath, label);
}

int mac_get_link(const char *path, mac_t label)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);

	dbug("mac_get_link()", path, newpath);
	typeof(mac_get_link) *sys_mac;
        sys_mac = dlsym(RTLD_NEXT, "mac_get_link");
                         
        return (*sys_mac)(newpath, label);
}

int _mac_get_link(const char *path, mac_t label)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);

	dbug("_mac_get_link()", path, newpath);
	typeof(_mac_get_link) *sys_mac;
        sys_mac = dlsym(RTLD_NEXT, "_mac_get_link");
                         
        return (*sys_mac)(newpath, label);
}

DIR* opendir(const char *filename)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, filename, 0);
        get_link_path(newpath, newpath);

	dbug("opendir()", filename, newpath);
	typeof(opendir) *sys_opendir;
        sys_opendir = dlsym(RTLD_NEXT, "opendir");
                         
        return (*sys_opendir)(newpath);
}

DIR* _opendir(const char *filename)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, filename, 0);
        get_link_path(newpath, newpath);

	dbug("_opendir()", filename, newpath);
	typeof(_opendir) *sys_opendir;
        sys_opendir = dlsym(RTLD_NEXT, "_opendir");
                         
        return (*sys_opendir)(newpath);
}

long pathconf(const char *path, int name)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);

	dbug("pathconf()", path, newpath);
	typeof(pathconf) *sys_path;
        sys_path = dlsym(RTLD_NEXT, "pathconf");
        return (*sys_path)(newpath, name);
}

long _pathconf(const char *path, int name)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);

	dbug("_pathconf()", path, newpath);
	typeof(_pathconf) *sys_path;
        sys_path = dlsym(RTLD_NEXT, "_pathconf");
        return (*sys_path)(newpath, name);
}

int stat(const char *path, struct stat *sb)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);
        get_link_path(newpath, newpath);

	dbug("stat()", path, newpath);
	typeof(stat) *sys_stat;
        sys_stat = dlsym(RTLD_NEXT, "stat");
                         
        return (*sys_stat)(newpath, sb);
}

int _stat(const char *path, struct stat *sb)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);

	dbug("_stat()", path, newpath);
	typeof(_stat) *sys_stat;
        sys_stat = dlsym(RTLD_NEXT, "_stat");
                         
        return (*sys_stat)(newpath, sb);
}

int open(const char *path, int flags, int mode)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);
        get_link_path(newpath, newpath);

	/*
	va_list args;
  	mode_t mode;
  
  	va_start(args, flags);
	mode = va_arg(args, int);
	va_end(args);
	*/

	dbug("open()", path, newpath);
	typeof(open) *sys_open;
	sys_open = dlsym(RTLD_NEXT, "open");
	if ( flags & O_CREAT )
		return (*sys_open)(newpath, flags, mode);
	else
		return (*sys_open)(newpath, flags, 0);
}

int _open(const char *path, int flags, int mode)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);
        get_link_path(newpath, newpath);

	/*
	va_list args;
  	mode_t mode;
  
  	va_start(args, flags);
	mode = va_arg(args, int);
	va_end(args);
	*/

	typeof(_open) *sys_open;
	dbug("_open()", path, newpath);
	sys_open = dlsym(RTLD_NEXT, "_open");
	if ( flags & O_CREAT )
		return (*sys_open)(newpath, flags, mode);
	else
		return (*sys_open)(newpath, flags, 0);
}

int openat(int fd, const char * pathname, int flags, mode_t mode)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, pathname, 0);
        get_link_path(newpath, newpath);

	dbug("openat()", pathname, newpath);
	typeof(openat) *sys_openat;
	sys_openat = dlsym(RTLD_NEXT, "openat");
	if ( flags & O_CREAT )
		return (*sys_openat)(fd, newpath, flags, mode);
	else
		return (*sys_openat)(fd, newpath, flags, 0);
}

int _openat(int fd, const char * pathname, int flags, mode_t mode)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, pathname, 0);
        get_link_path(newpath, newpath);

	dbug("_openat()", pathname, newpath);
	typeof(_openat) *sys_openat;
	sys_openat = dlsym(RTLD_NEXT, "_openat");
	if ( flags & O_CREAT )
		return (*sys_openat)(fd, newpath, flags, mode);
	else
		return (*sys_openat)(fd, newpath, flags, 0);
}

size_t readlink(const char *restrict path, char *restrict buf, size_t bufsiz)
{
        char newpath[MAXPATHLEN];
        char bufcpy[MAXPATHLEN];
        char newlink[MAXPATHLEN];
	size_t newbufsiz;
        get_modified_path(newpath, path, 0);

	dbug("readlink()", path, newpath);
	typeof(readlink) *sys_readlink;
	sys_readlink = dlsym(RTLD_NEXT, "readlink");

	return (*sys_readlink)(newpath, buf, bufsiz);

	/*
	size_t ret = (*sys_readlink)(newpath, buf, bufsiz);
	if ( ret == -1 )
		return ret;

	// Good fetch, lets mangle the link we got now
	buf[ret]='\0';
	strcpy(bufcpy, buf);

	// We don't need to mangle links which are not absolute
	if ( strpos(bufcpy, "/") != 0 )
		return ret;

	//printf("readlink() %s -> %s\n", path, bufcpy);

	dbug("readlink1()", path, bufcpy);
        get_modified_path(newlink, bufcpy, 0);
	//printf("readlink2() %s -> %s\n", path, newlink);

	// Zero out the old buf
	//bzero(buf, ret);

	// Copy the new string back
	memcpy(buf, newlink, (sizeof(newlink)-1));
	newbufsiz = sizeof(newlink);

	dbug("readlink2()", path, buf);
	printf("readlink3() %s -> %s\n", path, buf);
        return newbufsiz;
	*/
}

size_t _readlink(const char *restrict path, char *restrict buf, size_t bufsiz)
{
        char newpath[MAXPATHLEN];
        char bufcpy[MAXPATHLEN];
        char newlink[MAXPATHLEN];
	size_t newbufsiz;
        get_modified_path(newpath, path, 0);

	dbug("_readlink()", path, newpath);
	typeof(_readlink) *sys_readlink;
	sys_readlink = dlsym(RTLD_NEXT, "_readlink");

	size_t ret = (*sys_readlink)(newpath, buf, bufsiz);
	if ( ret == -1 )
		return ret;

	// Good fetch, lets mangle the link we got now
	buf[ret]='\0';
	strcpy(bufcpy, buf);

        get_modified_path(newlink, bufcpy, 0);

	// Zero out the old buf
	bzero(buf, bufsiz);

	// Copy the new string back
	memcpy(buf, newlink, (sizeof(newlink)-1));
	newbufsiz = sizeof(newlink);
        return newbufsiz;
}

size_t readlinkat(int fd, const char *restrict path, char *restrict buf, size_t bufsiz)
{
        char newpath[MAXPATHLEN];
        char bufcpy[MAXPATHLEN];
        char newlink[MAXPATHLEN];
	size_t newbufsiz;
        get_modified_path(newpath, path, 0);

	dbug("readlinkat()", path, newpath);
	typeof(readlinkat) *sys_readlinkat;
	sys_readlinkat = dlsym(RTLD_NEXT, "readlinkat");
	size_t ret = (*sys_readlinkat)(fd, newpath, buf, bufsiz);
	if ( ret == -1 )
		return ret;

	// Good fetch, lets mangle the link we got now
	buf[ret]='\0';
	strcpy(bufcpy, buf);

        get_modified_path(newlink, bufcpy, 0);

	// Zero out the old buf
	bzero(buf, bufsiz);

	// Copy the new string back
	memcpy(buf, newlink, (sizeof(newlink)-1));
	newbufsiz = sizeof(newlink);
        return newbufsiz;
}

size_t _readlinkat(int fd, const char *restrict path, char *restrict buf, size_t bufsiz)
{
        char newpath[MAXPATHLEN];
        char bufcpy[MAXPATHLEN];
        char newlink[MAXPATHLEN];
	size_t newbufsiz;
        get_modified_path(newpath, path, 0);

	dbug("_readlinkat()", path, newpath);
	typeof(_readlinkat) *sys_readlinkat;
	sys_readlinkat = dlsym(RTLD_NEXT, "_readlinkat");
	size_t ret = (*sys_readlinkat)(fd, newpath, buf, bufsiz);
	if ( ret == -1 )
		return ret;

	// Good fetch, lets mangle the link we got now
	buf[ret]='\0';
	strcpy(bufcpy, buf);

        get_modified_path(newlink, bufcpy, 0);

	// Zero out the old buf
	bzero(buf, bufsiz);

	// Copy the new string back
	memcpy(buf, newlink, (sizeof(newlink)-1));
	newbufsiz = sizeof(newlink);
        return newbufsiz;
}

int rename(const char *from, const char *to)
{
        char newfrom[MAXPATHLEN];
        char newto[MAXPATHLEN];
        get_modified_path(newfrom, from, 0);
        get_modified_path(newto, to, 0);

	dbug("rename()", from, newfrom);
	dbug("rename2()", to, newto);
	typeof(rename) *sys_rename;
	sys_rename = dlsym(RTLD_NEXT, "rename");
	return (*sys_rename)(newfrom, newto);
}

int _rename(const char *from, const char *to)
{
        char newfrom[MAXPATHLEN];
        char newto[MAXPATHLEN];
        get_modified_path(newfrom, from, 0);
        get_modified_path(newto, to, 0);

	dbug("_rename()", from, newfrom);
	dbug("_rename2()", to, newto);
	typeof(_rename) *sys_rename;
	sys_rename = dlsym(RTLD_NEXT, "_rename");
	return (*sys_rename)(newfrom, newto);
}

int renameat(int fromfd, const char *from, int tofd, const char *to)
{
        char newfrom[MAXPATHLEN];
        char newto[MAXPATHLEN];
        get_modified_path(newfrom, from, 0);
        get_modified_path(newto, to, 0);

	dbug("renameat()", from, newfrom);
	dbug("renameat2()", to, newto);
	typeof(renameat) *sys_renameat;
	sys_renameat = dlsym(RTLD_NEXT, "renameat");
	return (*sys_renameat)(fromfd, newfrom, tofd, newfrom);
}

int _renameat(int fromfd, const char *from, int tofd, const char *to)
{
        char newfrom[MAXPATHLEN];
        char newto[MAXPATHLEN];
        get_modified_path(newfrom, from, 0);
        get_modified_path(newto, to, 0);

	dbug("_renameat()", from, newfrom);
	dbug("_renameat2()", to, newto);
	typeof(_renameat) *sys_renameat;
	sys_renameat = dlsym(RTLD_NEXT, "_renameat");
	return (*sys_renameat)(fromfd, newfrom, tofd, newfrom);
}

int rmdir(const char *path)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);

	dbug("rmdir()", path, newpath);
	typeof(rmdir) *sys_rmdir;
	sys_rmdir = dlsym(RTLD_NEXT, "rmdir");
	return (*sys_rmdir)(newpath);
}

int _rmdir(const char *path)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);

	dbug("_rmdir()", path, newpath);
	typeof(_rmdir) *sys_rmdir;
	sys_rmdir = dlsym(RTLD_NEXT, "_rmdir");
	return (*sys_rmdir)(newpath);
}

int statfs(const char *path, struct statfs *buf)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);
        get_link_path(newpath, newpath);

	typeof(statfs) *sys_statfs;
	dbug("statfs()", path, newpath);
	sys_statfs = dlsym(RTLD_NEXT, "statfs");
	return (*sys_statfs)(newpath, buf);
}

int _statfs(const char *path, struct statfs *buf)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);
        get_link_path(newpath, newpath);

	dbug("_statfs()", path, newpath);
	typeof(_statfs) *sys_statfs;
	sys_statfs = dlsym(RTLD_NEXT, "_statfs");
	return (*sys_statfs)(newpath, buf);
}

int symlink(const char *name1, const char *name2)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, name2, 0);

	dbug("symlink()", name2, newpath);
	typeof(symlink) *sys_symlink;
	sys_symlink = dlsym(RTLD_NEXT, "symlink");
	return (*sys_symlink)(name1, newpath);
}

int _symlink(const char *name1, const char *name2)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, name2, 0);

	dbug("_symlink()", name2, newpath);
	typeof(_symlink) *sys_symlink;
	sys_symlink = dlsym(RTLD_NEXT, "_symlink");
	return (*sys_symlink)(name1, newpath);
}

int symlinkat(const char *name1, int fd, const char *name2)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, name2, 0);

	dbug("symlinkat()", name2, newpath);
	typeof(symlinkat) *sys_symlinkat;
	sys_symlinkat = dlsym(RTLD_NEXT, "symlinkat");
	return (*sys_symlinkat)(name1, fd, newpath);
}

int _symlinkat(const char *name1, int fd, const char *name2)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, name2, 0);

	dbug("_symlinkat()", name2, newpath);
	typeof(_symlinkat) *sys_symlinkat;
	sys_symlinkat = dlsym(RTLD_NEXT, "_symlinkat");
	return (*sys_symlinkat)(name1, fd, newpath);
}

int truncate(const char *path, off_t length)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);

	dbug("truncate()", path, newpath);
	typeof(truncate) *sys_truncate;
	sys_truncate = dlsym(RTLD_NEXT, "truncate");
	return (*sys_truncate)(newpath, length);
}

int _truncate(const char *path, off_t length)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);

	dbug("_truncate()", path, newpath);
	typeof(_truncate) *sys_truncate;
	sys_truncate = dlsym(RTLD_NEXT, "_truncate");
	return (*sys_truncate)(newpath, length);
}

int unlink(const char * path)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);

	dbug("unlink()", path, newpath);
	typeof(unlink) *sys_unlink;
	sys_unlink = dlsym(RTLD_NEXT, "unlink");
	return (*sys_unlink)(newpath);
}

int _unlink(const char * path)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);

	dbug("_unlink()", path, newpath);
	typeof(_unlink) *sys_unlink;
	sys_unlink = dlsym(RTLD_NEXT, "_unlink");
	return (*sys_unlink)(newpath);
}

int unlinkat(int fd, const char * path, int flag)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);

	dbug("unlinkat()", path, newpath);
	typeof(unlinkat) *sys_unlinkat;
	sys_unlinkat = dlsym(RTLD_NEXT, "unlinkat");
	return (*sys_unlinkat)(fd, newpath, flag);
}

int _unlinkat(int fd, const char * path, int flag)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);

	dbug("_unlinkat()", path, newpath);
	typeof(_unlinkat) *sys_unlinkat;
	sys_unlinkat = dlsym(RTLD_NEXT, "_unlinkat");
	return (*sys_unlinkat)(fd, newpath, flag);
}

int utimes(const char *path, const struct timeval *times)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);

	dbug("utimes()", path, newpath);
	typeof(utimes) *sys_utimes;
	sys_utimes = dlsym(RTLD_NEXT, "utimes");
	return (*sys_utimes)(newpath, times);
}

int _utimes(const char *path, const struct timeval *times)
{
        char newpath[MAXPATHLEN];
        get_modified_path(newpath, path, 0);

	dbug("_utimes()", path, newpath);
	typeof(_utimes) *sys_utimes;
	sys_utimes = dlsym(RTLD_NEXT, "_utimes");
	return (*sys_utimes)(newpath, times);
}
