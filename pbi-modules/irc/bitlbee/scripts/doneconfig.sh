#!/bin/sh
#
# Called by AppCafe after changing config values

# See if we need to restart bitlbee
grep -q '^bitlbee_enable="YES"' /etc/rc.conf
if [ $? -eq 0 ] ; then
   # Restart bitlbee
   service bitlbee restart
fi
