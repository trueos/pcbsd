#!/bin/sh

if [ -z "$1" ] ; then
   STAGEDIR="/usr/local"
else
   STAGEDIR="$1"
fi

# lpreserver install script
PROGDIR="${STAGEDIR}/share/lpreserver"

mkdir -p ${PROGDIR} >/dev/null 2>/dev/null

DIR=`dirname $0`
cd ${DIR}

cp lpreserver ${STAGEDIR}/bin/lpreserver
chmod 755 ${STAGEDIR}/bin/lpreserver

cp lpreserver-host-iscsi ${STAGEDIR}/bin/lpreserver-host-iscsi
chmod 755 ${STAGEDIR}/bin/lpreserver-host-iscsi

if [ -d "${PROGDIR}/backend" ] ; then
  rm -rf ${PROGDIR}/backend
fi

cp -r backend ${PROGDIR}
chmod 755 ${PROGDIR}/backend/*.sh

#Also install the man page
gzip -c lpreserver.8 > lpreserver.8.gz
mv -f lpreserver.8.gz ${STAGEDIR}/man/man8/lpreserver.8.gz

exit 0
