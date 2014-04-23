#!/bin/sh
# This script enables the service at bootup
# Return 0 on success, or 1 on failure. 

cat /etc/rc.conf | grep -v "vsftpd_enable=" >/tmp/rc.new
echo "vsftpd_enable=\"YES\"" >> /tmp/rc.new
mv /tmp/rc.new /etc/rc.conf

return 0
