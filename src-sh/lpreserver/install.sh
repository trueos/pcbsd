#!/bin/sh
# lpreserver install script
PROGDIR="/usr/local/share/lpreserver"

mkdir -p ${PROGDIR} >/dev/null 2>/dev/null

DIR=`dirname $0`
cd ${DIR}

cp lpreserver /usr/local/bin/lpreserver
chmod 755 /usr/local/bin/lpreserver

if [ -d "${PROGDIR}/backend" ] ; then
  rm -rf ${PROGDIR}/backend
fi

cp -r backend ${PROGDIR}
chmod 755 ${PROGDIR}/backend/*.sh

exit 0
