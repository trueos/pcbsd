#!/bin/sh
# This script checks if a service is enabled to run at bootup
# Return a "0" if service is enabled or a "1" if it is disabled

grep 'bitlbee_enable="YES"' /etc/rc.conf >/dev/null
if [ "$?" = "0" ]
then
 return 0
else 
 return 1
fi

