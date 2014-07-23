#!/bin/sh

if [ -z "$1" ] ; then
   STAGEDIR="/usr/local"
else
   STAGEDIR="$1"
fi

PROGDIR="${STAGEDIR}/share/appweb"
rm -rf ${PROGDIR} >/dev/null 2>/dev/null
mkdir -p ${PROGDIR} >/dev/null 2>/dev/null

DIR=`dirname $0`
cd ${DIR}

cp -r share/* ${PROGDIR}/

if [ ! -d "${STAGEDIR}/etc/rc.d" ] ; then
   mkdir ${STAEGDIR}/etc/rc.d
fi
cp rc.d/* ${STAGEDIR}/etc/rc.d/

exit 0
