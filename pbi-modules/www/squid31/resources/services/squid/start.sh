#!/bin/sh
# This script starts a stopped service
# Return 0 on success, or "1" on failure 

/Programs/rc.d/squid start
if [ "$?" = "0" ]
then
 return 0
else
 return 1
fi

