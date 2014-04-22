#!/bin/sh
# This script disables the service at bootup
# Return 0 on success, or 1 on failure. 

cat /etc/rc.conf | grep -v "bitlbee_enable=" >/tmp/rc.new
echo "bitlbee_enable=\"NO\"" >> /tmp/rc.new
mv /tmp/rc.new /etc/rc.conf

return 0
