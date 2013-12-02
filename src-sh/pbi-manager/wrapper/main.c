#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/param.h>
#include <sys/sysctl.h>
#include <err.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#include <paths.h>

#define MAX_SIZE 9012

int main(int argc, char **argv)
{
    // Setup our vars
    char *progdir = NULL;
    char *progtarget = NULL;
    char newtarget[MAX_SIZE];
    char pbimntdir[MAX_SIZE];
    char pbime[MAX_SIZE];

    // Setup working variables
    char bfile[PATH_MAX];
    char tfilepath[PATH_MAX];
    FILE *tfile; 
    char *optline;
    char *bufptr;
    char buf[1024];
    char cmpstr[1024];

    if ( strlen(argv[0]) > PATH_MAX ) {
       exit(2);
    }

    // Lets use sysctl to figure out where we are
    int mib[4];
    mib[0] = CTL_KERN;
    mib[1] = KERN_PROC;
    mib[2] = KERN_PROC_PATHNAME;
    mib[3] = -1;
    char mypath[1024];
    size_t cb = sizeof(mypath);
    sysctl(mib, 4, mypath, &cb, NULL, 0);

    if ( mypath[0] == 0 )
      strcpy(mypath, argv[0]);
    
    // Figure out where we are
    if (realpath(mypath, bfile) == NULL) {
       perror("Could not determine realpath...");
       return -1;
    }

    // Set the target file path
    strcpy(tfilepath, bfile);
    strcat(tfilepath, ".pbiopt");

    // Open target file
    tfile = fopen(tfilepath, "r");
    if ( tfile == NULL ) {
       printf("Missing pbi options file: %s\n", tfilepath);
       return -1;
    }

    // Read in .pbiopt file
    while ( (optline = fgets(buf, sizeof(buf), tfile)) != NULL ) {
       strcpy(cmpstr, "PROGDIR:"); 
       if ( ! strncmp(cmpstr, buf, strlen(cmpstr))) {
          bufptr = strdup(buf);
          progdir = strsep(&bufptr, " ");
          progdir = strsep(&bufptr, " ");
       }

       strcpy(cmpstr, "TARGET:"); 
       if ( ! strncmp(cmpstr, buf, strlen(cmpstr))) {
          bufptr = strdup(buf);
          progtarget = strsep(&bufptr, " ");
          progtarget = strsep(&bufptr, " ");
       }
    
    }
    fclose(tfile);

    if ( progdir == NULL ) {
       printf("Missing PROGDIR:");
       return -1;
    }

    // Remove any newline from progdir
    char *ptr;
    if( ( ptr = strchr(progdir, '\n')) != NULL )
	*ptr = '\0'; 

    if ( progtarget == NULL ) {
       printf("Missing TARGET:");
       return -1;
    }


    // Set the target
    strcpy(newtarget, "/usr/local" );
    strcat(newtarget, "/");
    strncat(newtarget, progtarget, strlen(progtarget) -1 );

    // Enable for debug
    //printf( "PATH: %s\n", newpath);
    //printf( "Running: %s \n", newtarget);
    //return system(newtarget);
    //strncpy(argv[0], newtarget);

    // Setup the mount-directory
    strcpy(pbimntdir, "/usr/pbi/.mounts/");
    strcat(pbimntdir, basename(progdir));

    strcpy(pbime, "/usr/pbi/.pbime");

    // Build new target argv
    char *targv[argc+4];
    targv[0] = pbime;
    targv[1] = pbimntdir;
    targv[2] = progdir;
    targv[3] = newtarget;
    int i=4;
    int j=1;
    while(j < argc){
      targv[i] = argv[j];
      //printf( "Arg: %s \n", targv[i]);
      i++;
      j++;
    }
    targv[i] = '\0';
    //printf( "Arg: %s \n", targv[0]);
    //printf( "Arg: %s \n", targv[1]);
    //printf( "Arg: %s \n", targv[2]);
    //printf( "Arg: %s \n", targv[3]);

    return execv(pbime, targv);
    //return execv(newtarget, targv);
}
