/*
 *  AUTHOR: Kris Moore <kris@pcbsd.org>
 * LICENSE: BSD
 * COMMENT: FUSE based filesystem for creating virtual PBI container chroot environments
 *
 */

#define FUSE_USE_VERSION 26

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <fuse.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mount.h>
#include <sys/param.h>
#include <sys/types.h>
#include <unistd.h>


#define NOT_FOUND -1

// Set some globals
char hintsdir[MAXPATHLEN];
char pbidir[MAXPATHLEN];
char linuxdir[MAXPATHLEN];

int strpos(const char *haystack, char *needle)
{
   char *p = strstr(haystack, needle);
   if (p) {
      return p - haystack;
   }
   return NOT_FOUND;
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
*  OK, this is where all the magic happens.
*  We are going to be taking the current filesystem path, and checking it against
*  a list of ones we want to "virtualize" so that it appears we have a different
*  /usr/local namespace
***********************************************************************************/
int get_modified_path(char *npath, const char *opath)
{
	// First, lets look for calls to /var/run/ld-elf*.so.hints
	// These need to be replaced with calls to our new hints file in hintsdir
	if ( strpos(opath, "/var/run/") == 0 )
	{
		if ( strcmp(opath, "/var/run/ld-elf.so.hints") == 0 )
		{
			strcpy(npath, hintsdir);
			strcat(npath, "/ld-elf.so.hints");
			return 0;
		}
		if ( strcmp(opath, "/var/run/ld-elf32.so.hints") == 0 )
		{
			strcpy(npath, hintsdir);
			strcat(npath, "/ld-elf32.so.hints");
			return 0;
		}
	}

	if ( strcmp(opath, "/etc/rc.d/ldconfig") == 0 )
	{
		strcpy(npath, "/usr/pbi/.ldconfig");
		return 0;
	}

	// Check if we have a virtual linux /compat directory to fake as well
	if ( strpos(opath, "/compat/linux") == 0 )
	{
		// If this is /compat/linux/proc, lets use the systems
		if ( strpos(opath, "/compat/linux/proc") == 0 ) {
			strcpy(npath, opath);
			return 0;
		}

		if (0 == access(linuxdir, F_OK))
		{
			strcpy(npath, replace_str(opath, "/compat/linux", linuxdir));
			return 0;
		}
		strcpy(npath, opath);
		return 0;
	}

	// Lastly, lets do all the parsing for /usr/local matching
	if ( strpos(opath, "/usr/local") == 0 )
	{
		// Use the systems copies of some font / icon directories
		if ( strpos(opath, "/usr/local/etc/fonts") == 0 ) {
			strcpy(npath, opath);
			return 0;
		}
		if ( strpos(opath, "/usr/local/lib/X11/fonts") == 0 ) {
			strcpy(npath, opath);
			return 0;
		}
		if ( strpos(opath, "/usr/local/lib/X11/icons") == 0 ) {
			strcpy(npath, opath);
			return 0;
		}
		if ( strpos(opath, "/usr/local/share/icons") == 0 )
		{
			strcpy(npath, opath);
			return 0;
		}
		if ( strpos(opath, "/usr/local/share/font-") == 0 ) {
			strcpy(npath, opath);
			return 0;
		}

		// Look for some of our callback utils
		if ( strpos(opath, "/usr/local/bin/pbisyscmd") == 0 ) {
			strcpy(npath, "/usr/pbi/.pbisyscmd");
			return 0;
		}
		if ( strpos(opath, "/usr/local/bin/openwith") == 0 ) {
			strcpy(npath, "/usr/pbi/.pbisyscmd");
			return 0;
		}
		if ( strpos(opath, "/usr/local/bin/xdg-open") == 0 ) {
			strcpy(npath, "/usr/pbi/.pbisyscmd");
			return 0;
		}
	
		strcpy(npath, replace_str(opath, "/usr/local", pbidir));
		return 0;
	}

	strcpy(npath, opath);
	return 0;
}

int newfile_chown(const char *path) {
	struct fuse_context *ctx = fuse_get_context();
	if (ctx->uid != 0 && ctx->gid != 0) {
		int res = lchown(path, ctx->uid, ctx->gid);
		if (res)
			return -errno;
	}
        return 0;
}

int cp(const char *to, const char *from)
{
	int fd_to, fd_from;
	char buf[4096];
	ssize_t nread;
	int saved_errno;

	fd_from = open(from, O_RDONLY);
	if (fd_from < 0)
		return -1;

	fd_to = open(to, O_WRONLY | O_CREAT | O_EXCL, 0666);
	if (fd_to < 0)
		goto out_error;

	while (nread = read(fd_from, buf, sizeof buf), nread > 0)
	{
		char *out_ptr = buf;
		ssize_t nwritten;

		do {
			nwritten = write(fd_to, out_ptr, nread);

			if (nwritten >= 0)
			{
                		nread -= nwritten;
                		out_ptr += nwritten;
			}
			else if (errno != EINTR)
			{
				goto out_error;
			}
		} while (nread > 0);
	}

	if (nread == 0)
	{
       	 	if (close(fd_to) < 0)
        	{
            		fd_to = -1;
            		goto out_error;
	  	}
		close(fd_from);

		/* Success! */
		return 0;
	}

	out_error:
    		saved_errno = errno;

		close(fd_from);
		if (fd_to >= 0)
			close(fd_to);

		errno = saved_errno;
		return -1;
}

static int pbi_access(const char *path, int mask)
{
	char newpath[MAXPATHLEN];
	get_modified_path(newpath, path);

	int res = access(newpath, mask);
	if (res == -1)
		return -errno;
	return 0;
}

static int pbi_getattr(const char *path, struct stat *stbuf)
{
	char newpath[MAXPATHLEN];
	get_modified_path(newpath, path);

	/*
	char testcmd[MAXPATHLEN];
	strcpy(testcmd, "echo \"getattr: ");
	strcat(testcmd, newpath);
	strcat(testcmd, "\" >> /tmp/newpath");
	system(testcmd);
	*/

	int res = lstat(newpath, stbuf);
        if (res == -1)
		return -errno;

	return res;
}

static int pbi_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
			 off_t offset, struct fuse_file_info *fi)
{
	(void) offset;
	(void) fi;

	char newpath[MAXPATHLEN];
	get_modified_path(newpath, path);

	// Avoid a recursive directory tree
	if ( strpos(newpath, "/usr/pbi/.mounts") == 0)
           return 0;

	DIR *openDir = opendir(newpath);

	struct dirent *de;
	while ((de = readdir(openDir)) != NULL) {
                
		struct stat st;
		memset(&st, 0, sizeof(st));
		st.st_ino = de->d_ino;
		st.st_mode = de->d_type << 12;

		if (filler(buf, de->d_name, &st, 0)) break;
	}

	closedir(openDir);

	return 0;
}

static int pbi_open(const char *path, struct fuse_file_info *fi)
{

	char newpath[MAXPATHLEN];
	get_modified_path(newpath, path);

	int fd = open(newpath, fi->flags);
        if (fd == -1)
		return -errno;
	fi->fh = (unsigned long)fd;

	return 0;
}

static int pbi_read(const char *path, char *buf, size_t size, off_t offset,
		      struct fuse_file_info *fi)
{
	(void) fi;

	int result = pread(fi->fh, buf, size, offset);
	if (result == -1)
		return -errno;

	return result;
}

static int pbi_statfs(const char *path, struct statvfs *stbuf) 
{
	char newpath[MAXPATHLEN];
	get_modified_path(newpath, path);


	return statvfs(newpath, stbuf);
}

static int pbi_symlink(const char *from, const char *to)
{
	char newpath[MAXPATHLEN];
	get_modified_path(newpath, from);

	return symlink(newpath, to);
}

static int pbi_readlink(const char *path, char *buf, size_t size)
{
	char newpath[MAXPATHLEN];
	get_modified_path(newpath, path);

	int result= readlink(newpath, buf, size - 1);
	if (result == -1)
		return -errno;
	buf[result] = '\0';
	return 0;
}

static int pbi_chmod(const char *path, mode_t mode)
{
	char newpath[MAXPATHLEN];
	get_modified_path(newpath, path);

	int result = lchmod(newpath, mode);
        if (result == -1) 
		return -errno;
        return 0;
}

static int pbi_chown(const char *path, uid_t uid, gid_t gid)
{
	char newpath[MAXPATHLEN];
	get_modified_path(newpath, path);

	int result = lchown(newpath, uid, gid);
	if (result == -1)
		return -errno;
	return 0;        
}

static int pbi_create(const char *path, mode_t mode, struct fuse_file_info *fi)
{
	char newpath[MAXPATHLEN];
	get_modified_path(newpath, path);

	int res = open(newpath, fi->flags, 0);
	if (res == -1)
		return -errno;

	newfile_chown(newpath);
	fchmod(res, mode);

	fi->fh = res;
	return 0;
}

static int pbi_flush(const char *path, struct fuse_file_info *fi)
{
	int fd = dup(fi->fh);
	if (fd == -1) {
		if (fsync(fi->fh) == -1) 
			return -EIO;
		return -errno;
        }

	int res = close(fd);
	if (res == -1)
		return -errno;

	return 0;
}

static int pbi_fsync(const char *path, int isdatasync, struct fuse_file_info *fi)
{
	(void) isdatasync;
	int res = fsync(fi->fh);
        if (res == -1)
		return -errno;
	return 0;
}

static int pbi_link(const char *from, const char *to)
{
	char newpath[MAXPATHLEN];
	get_modified_path(newpath, from);

	int res = link(newpath, to);
	if (res == -1)
		return -errno;
	return 0;
}

static int pbi_mkdir(const char *path, mode_t mode)
{
	char newpath[MAXPATHLEN];
	get_modified_path(newpath, path);

	int res = mkdir(newpath, 0);
	if (res == -1)
		return -errno;

	newfile_chown(newpath);
	chmod(newpath, mode);

	return 0;
}

static int pbi_mknod(const char *path, mode_t mode, dev_t rdev)
{
	int ftype = mode & S_IFMT;
	int fpath = mode & (S_ISUID| S_ISGID | S_ISVTX | S_IRWXU | S_IRWXG | S_IRWXO);

	char newpath[MAXPATHLEN];
	get_modified_path(newpath, path);

	int res = mknod(newpath, ftype, rdev);
	if (res == -1)
		return -errno;

        newfile_chown(newpath);
	chmod(newpath, fpath);

	return 0;
}

static int pbi_release(const char *path, struct fuse_file_info *fi)
{
	(void) path;

	int res = close(fi->fh);
	if (res == -1)
		return -errno;

	return 0;
}

static int pbi_rename(const char *from, const char *to)
{
	char newfrom[MAXPATHLEN];
	char newto[MAXPATHLEN];
	get_modified_path(newfrom, from);
	get_modified_path(newto, to);

	if ( strcmp(from, newfrom) != 0 || strcmp(to, newto) != 0 )
	{
		// Unlink the target first
		if (0 == access(newto, F_OK)) {
			if ( unlink(newto) == -1 )
				return -errno;
		}

		// Doing a across file-system move, need to use copy instead of rename 
		if ( cp(newto, newfrom) != 0)
			return -errno;

		// Set owner / permissions now
		struct stat fst;
		stat(newfrom, &fst);
		chown(newto, fst.st_uid,fst.st_gid);
		chmod(newto, fst.st_mode);

		if ( unlink(newfrom) == -1 )
			return -errno;
		
	} else {
		int res = rename(newfrom, newto);
		if (res == -1)
			return -errno;
	}
	return 0;
}

int pbi_rmdir(const char *path)
{
	char newpath[MAXPATHLEN];
	get_modified_path(newpath, path);

	int res = rmdir(newpath);
	if (res == -1)
		return -errno;
	return 0;
}

static int pbi_truncate(const char *path, off_t size)
{
	char newpath[MAXPATHLEN];
	get_modified_path(newpath, path);

	int res = truncate(newpath, size);
	if (res == -1)
		return -errno;
	return 0;
}

int pbi_unlink(const char *path)
{
	char newpath[MAXPATHLEN];
	get_modified_path(newpath, path);

	int res = unlink(newpath);
	if (res == -1)
		return -errno;
	return 0;
}

static int pbi_utimens(const char *path, const struct timespec ts[2])
{
	char newpath[MAXPATHLEN];
	get_modified_path(newpath, path);

	struct timeval tv[2];
	tv[0].tv_sec = ts[0].tv_sec;
	tv[0].tv_usec = ts[0].tv_nsec / 1000;
	tv[1].tv_sec = ts[0].tv_sec;
	tv[1].tv_usec = ts[0].tv_nsec / 1000;
	int res = lutimes(newpath, tv);
	if (res == -1)
		return -errno;
	return 0;
}

static int pbi_write(const char *path, const char *buf, size_t size, off_t offset,
	struct fuse_file_info *fi)
{
	(void) path;
	int written = pwrite(fi->fh, buf, size, offset);
	if (written == -1)
		return -errno;
	return written;
}

static struct fuse_operations pbi_oper = {
	.access		= pbi_access,
	.chmod		= pbi_chmod,
	.chown		= pbi_chown,
	.create		= pbi_create,
	.flush		= pbi_flush,
	.fsync		= pbi_fsync,
	.getattr	= pbi_getattr,
	.link		= pbi_link,
	.mkdir		= pbi_mkdir,
	.mknod		= pbi_mknod,
	.open		= pbi_open,
	.read		= pbi_read,
	.readlink	= pbi_readlink,
	.readdir	= pbi_readdir,
	.release	= pbi_release,
	.rename		= pbi_rename,
	.rmdir		= pbi_rmdir,
	.statfs		= pbi_statfs,
	.symlink	= pbi_symlink,
	.truncate	= pbi_truncate,
	.unlink		= pbi_unlink,
	.utimens	= pbi_utimens,
	.write		= pbi_write,
};

int main(int argc, char *argv[])
{

	if ( argc != 4 ) 
	{
		printf("Usage: %s <pbidir> <hintsdir> <mountpoint>\n", argv[0]);
		printf("Where: ");
		printf("       <pbidir>  = PBI directory with a new /local directory\n");
		printf("                   and optional /linux directory\n");
		printf("     <hintsdir>  = Directory with new ld-elf*.so.hints files\n");
		printf("   <mountpoint>  = Directory to mount PBI container\n");
		return 1;
	}

	if ( strlen(argv[1]) >= MAXPATHLEN || strlen(argv[2]) >= MAXPATHLEN ) {
		printf("MAXPATHLEN violation!\n");
		return 1;
	}

	// Verify the pbidir exists
	strcpy(pbidir, argv[1]);
	strcat(pbidir, "/local");
	if (0 != access(pbidir, F_OK)) {
  		if (ENOENT == errno) {
     			// does not exist
			printf("ERROR: The directory %s does not exist!", pbidir);
			return 1;
  		}
  		if (ENOTDIR == errno) {
     			// not a directory
			printf("ERROR: %s is not a directory!", pbidir);
			return 1;
  		}
	}


	// Add a linux dir
	strcpy(linuxdir, argv[1]);
	strcat(linuxdir, "/linux");

	// Verify that hintsdir exists
	strcpy(hintsdir, argv[2]);
	if (0 != access(hintsdir, F_OK)) {
  		if (ENOENT == errno) {
     			// does not exist
			printf("ERROR: The directory %s does not exist!", pbidir);
			return 1;
  		}
  		if (ENOTDIR == errno) {
     			// not a directory
			printf("ERROR: %s is not a directory!", pbidir);
			return 1;
  		}
	}


	// Set the mountpoint before starting fuse
	char *newargv[1024];
	newargv[0] = argv[0];
	newargv[1] = argv[3];

	// Set some fuse options
	struct fuse_args args = FUSE_ARGS_INIT(0, NULL);
	for ( int i = 0; i < argc ; i++ )
	{
		if ( i == 1 || i == 2 )
			continue;
		fuse_opt_add_arg(&args, argv[i]);
	}
	fuse_opt_add_arg(&args, "-oallow_other");

	return fuse_main(args.argc, args.argv, &pbi_oper, NULL);
}

