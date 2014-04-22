#!/bin/sh

${PBI_RCDIR}/musicpd stop
killall -9 mpd

cat /etc/rc.conf | grep -v "musicpd_enable" >/etc/.rc.conf.tmp.$$
mv /etc/.rc.conf.tmp.$$ /etc/rc.conf

