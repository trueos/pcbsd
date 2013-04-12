#include <sys/types.h>
#include <sys/fcntl.h>
#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main()
{
	system("cat /usr/sbin/freebsd-update | sed 's|! -t 0|-z '1'|g' | /bin/sh -s 'fetch'");
	return 0;
}
