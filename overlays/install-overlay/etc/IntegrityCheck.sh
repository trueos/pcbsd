#!/bin/sh
#
# Copyright (c) 2012 PC-BSD Project
# Authors: tim@pcbsd.org, kris@pcbsd.org
#
# This script checks the md5 of the usr.uzip filesystem

echo -e "Checking integrity of uzip filesystem...\c"

/sbin/md5 -q /uzip/usr.uzip | /bin/cmp -s /uzip/usr.uzip.md5 -

if [ "$?" != "0" ]
then
 echo "FAILED"
 exit 1
else
 echo "OK"
fi
