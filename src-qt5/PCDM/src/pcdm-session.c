/*
     Name: pcdm-session
     Desc: Small executable to drop priv and execute the desktop
  License: BSD (2 Clause)
   Author: Kris Moore
*/
#include <sys/param.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char ** argv)
{

  // Set our marker that X started properly
  system("touch /tmp/.pcdm-x-started");

  // Load the CLI args
  uid_t my_gid;
  uid_t my_uid;
  char username[MAXPATHLEN];
  char cmd[MAXPATHLEN];
  char xcmd[MAXPATHLEN];
  char xlog[MAXPATHLEN];
  char *xauth;
  if ( argc != 6 ) {
     exit(1);
  }

  char output[MAXPATHLEN];

  // Set our values
  strcpy(username, argv[1]);
  my_uid = atoi(argv[2]);
  my_gid = atoi(argv[3]);
  strcpy(xcmd, argv[4]);
  strcpy(xlog, argv[5]);

  // Change owner / group on XAUTHORITY file
  xauth = getenv("XAUTHORITY");
  if ( chown(xauth, my_uid, my_gid) != 0)
     printf("WARNING: Failed to chown XAUTHORITY");

  // Set the GID
  if (setgid(my_gid) < 0) {
     exit(1);
  }

  // Setup our other groups
  if (initgroups(username, my_gid) < 0) {
      exit(1);
  }

  // Lets drop to user privs
  if (setuid(my_uid) < 0) {
      exit(1);
  }

  // Run the app now
  strcpy(cmd, "sh ");
  strcat(cmd, xcmd);
  strcat(cmd, " >");
  strcat(cmd, xlog);
  strcat(cmd, " 2>");
  strcat(cmd, xlog);
  int ret = system(cmd);
  exit(ret);

}
