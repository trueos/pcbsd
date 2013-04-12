#!/bin/sh
# Check if we have an unprobed monitor so that XGUI can warn the user
############################################################################

rm /tmp/.unprobedMonitor >/dev/null 2>/dev/null

grep "Unprobed Monitor" /root/xorg.conf.new >/dev/null 2>/dev/null
if [ "$?" = "0" ]
then
   touch /tmp/.unprobedMonitor
fi
