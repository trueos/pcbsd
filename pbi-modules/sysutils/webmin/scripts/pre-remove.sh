#!/bin/sh

${PBI_RCDIR}/webmin stop
killall -9 webmin
rm ${PBI_RCDIR}/webmin
rm -Rf /var/log/webmin

cat /etc/rc.conf | grep -v "webmin_enable=" > /tmp/.rc.conf
mv /tmp/.rc.conf /etc/rc.conf
