#!/bin/sh
echo -e "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" > /tmp/.webmin_tmp.tmp
/usr/local/lib/webmin/setup.sh < /tmp/.webmin_tmp.tmp
rm -f /tmp/.webmin_tmp.tmp
cp -r /usr/local/etc/webmin ${PBI_PROGDIRPATH}/etc/webmin
