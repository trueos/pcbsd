#!/bin/sh

${PBI_RCDIR}/apache22 stop
killall -9 httpd

rm -Rf ${PBI_RCDIR}/apache22
rm -Rf ${PBI_RCDIR}/htcacheclean

cat /etc/rc.conf | grep -v "apache22_enable=" > /tmp/.rc.conf
mv /tmp/.rc.conf /etc/rc.conf
