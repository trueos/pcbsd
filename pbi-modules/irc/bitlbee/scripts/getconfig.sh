#!/bin/sh
# Script which will set values for bitlbee
#
# Called by AppCafe with:
# $1 = Key we are looking for value to

KEY="$1"
CFGFILE="/usr/local/etc/bitlbee/bitlbee.conf"

# Get value from config file
oldvalue=`grep "^${KEY} = " $CFGFILE 2>/dev/null | head -n 1 | sed "s|${KEY} = ||g" | cut -d ' ' -f 1`

if [ -n "$oldvalue" ] ; then
  echo "$oldvalue"
fi
