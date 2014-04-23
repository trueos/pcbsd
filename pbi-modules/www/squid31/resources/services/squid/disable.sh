#!/bin/sh
# This script disables the service at bootup
# Return 0 on success, or 1 on failure. 

cat /etc/rc.conf | grep -v "squid_enable=" >/tmp/rc.new
echo "squid_enable=\"NO\"" >> /tmp/rc.new
mv /tmp/rc.new /etc/rc.conf

return 0
