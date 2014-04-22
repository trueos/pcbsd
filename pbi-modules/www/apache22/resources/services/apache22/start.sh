#!/bin/sh
# This script starts a stopped service
# Return 0 on success, or "1" on failure 

/usr/local/etc/rc.d/apache22 start
if [ "$?" = "0" ]
then
 return 0
else
 return 1
fi

