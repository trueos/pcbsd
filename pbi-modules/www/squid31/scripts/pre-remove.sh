#!/bin/sh

${PBI_RCDIR}/squid stop
killall -9 squid

rm -f ${PBI_RCDIR}/squid

cat /etc/rc.conf | grep -v "squid_enable=" > /tmp/.rc.conf
mv /tmp/.rc.conf /etc/rc.conf
