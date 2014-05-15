#!/bin/sh
# This script stops a started service
# Return 0 on success, or "1" on failure 

/usr/local/etc/rc.d/webmin stop
if [ "$?" = "0" ]
then
 return 0
else
 return 1
fi

