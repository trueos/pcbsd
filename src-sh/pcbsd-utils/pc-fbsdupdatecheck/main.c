#include <sys/types.h>
#include <sys/fcntl.h>
#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char ** argv)
{
  if ( argc == 2 ) {
    if ( strcmp(argv[1], "update") != 0 )
    {
       printf("Invalid option!");
       return 1;
    }
    // Fetch to the standard working-dir
    system("touch /tmp/.fbsdup-lock");
    system("cat /usr/sbin/freebsd-update | sed 's|! -t 0|-z '1'|g' | /bin/sh -s 'fetch'");
    system("rm /tmp/.fbsdup-lock");
    return 0;
  } else {
    system("touch /tmp/.fbsdup-lock");
    system("mkdir /var/db/freebsd-update-check 2>/dev/null");
    system("cat /usr/sbin/freebsd-update | sed 's|! -t 0|-z '1'|g' | /bin/sh -s 'fetch' '-d' '/var/db/freebsd-update-check'");
    system("rm /tmp/.fbsdup-lock");
    return 0;
  }
}
