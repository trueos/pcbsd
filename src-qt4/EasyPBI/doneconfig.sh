#!/bin/sh
#
# Called by AppCafe after changing config values

# See if we need to restart bitlbee
grep -q '^CHANGEME_enable="YES"' /etc/rc.conf
if [ $? -eq 0 ] ; then
   # Restart bitlbee
   service CHANGEME restart
fi
