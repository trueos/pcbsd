#!/bin/sh

${PBI_RCDIR}/transmission stop

rm ${PBI_RCDIR}/transmission

sed -i.bak '/^transmission_*/d' /etc/rc.conf
