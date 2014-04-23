#!/bin/sh

rm ${PBI_RCDIR}/bitlbee
cat /etc/rc.conf | grep -v "bitlbee_enable" > /etc/rc.conf.new
mv /etc/rc.conf.new /etc/rc.conf

