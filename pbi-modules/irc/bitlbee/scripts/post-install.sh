#!/bin/sh

if [ ! -d "${SYS_LOCALBASE}/etc" ]
then
   mkdir ${SYS_LOCALBASE}/etc
fi

if [ ! -d "${SYS_LOCALBASE}/etc/bitlbee" ]
then
  mkdir ${SYS_LOCALBASE}/etc/bitlbee
fi

rm -rf ${PBI_PROGDIRPATH}/etc/bitlbee
ln -s ${SYS_LOCALBASE}/etc/bitlbee ${PBI_PROGDIRPATH}/etc/bitlbee

if [ ! -e "${PBI_PROGDIRPATH}/etc/bitlbee/bitlbee.conf" ]
then
  cp ${PBI_PROGDIRPATH}/bitlbee.conf ${PBI_PROGDIRPATH}/etc/bitlbee/
  cp ${PBI_PROGDIRPATH}/motd.txt ${PBI_PROGDIRPATH}/etc/bitlbee/
fi

ln -fs ${PBI_PROGDIRPATH}/etc/rc.d/bitlbee ${PBI_RCDIR}/bitlbee

grep 'bitlbee_enable="YES"' /etc/rc.conf
if [ "$?" != "0" ]
then
  echo 'bitlbee_enable="YES"' >>/etc/rc.conf
fi

mkdir -p /var/db/bitlbee
chown -R bitlbee:bitlbee /var/db/bitlbee
chown -R bitlbee:bitlbee ${SYS_LOCALBASE}/etc/bitlbee

${PBI_RCDIR}/bitlbee start
