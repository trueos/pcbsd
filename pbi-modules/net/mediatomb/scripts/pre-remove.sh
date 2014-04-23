#!/bin/sh

${PBI_RCDIR}/mediatomb stop
killall -9 mediatomb

rm -Rf ${PBI_RCDIR}/mediatomb

cat /etc/rc.conf | grep -v "mediatomb_enable=" > /tmp/.rc.conf
mv /tmp/.rc.conf /etc/rc.conf

rm -fr /var/mediatomb

/usr/sbin/pw userdel mediatomb
