#!/bin/sh
# This script stops a started service
# Return 0 on success, or "1" on failure 

ENABLED="YES"

DIR=`dirname $0`
# Check if this service is in /etc/rc.conf
grep '^bitlbee_enable="YES"' /etc/rc.conf 
if [ "$?" != "0" ]
then
  ENABLED="NO"
  ${DIR}/enable.sh
fi

/Programs/rc.d/bitlbee stop
ERR=$?

if [ "${ENABLED}" = "NO" ]
then
  ${DIR}/disable.sh
fi


if [ "$ERR" = "0" ]
then
 return 0
else
 return 1
fi
