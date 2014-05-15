#!/bin/sh
# This script restarts a currently running service
# Return a "0" on success, or a "1" on failure

killall vsftpd
/usr/local/etc/rc.d/vsftpd start
if [ "$?" = "0" ]
then
  exit 0
else
  exit 1
fi
