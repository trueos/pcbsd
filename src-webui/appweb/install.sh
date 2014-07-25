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

# Copy dispatcher script
cp dispatcher ${PROGDIR}/dispatcher
chmod 755 ${PROGDIR}/dispatcher

# Copy sudoers file
if [ ! -d "$STAGEDIR/etc/sudoers.d" ] ; then
   mkdir -p ${STAGEDIR}/etc/sudoers.d
fi
cp appweb-sudoers ${STAGEDIR}/etc/sudoers.d/
chmod 644 ${STAGEDIR}/etc/sudoers.d/appweb-sudoers

cp -r share/* ${PROGDIR}/

if [ ! -d "${STAGEDIR}/etc/rc.d" ] ; then
   mkdir ${STAEGDIR}/etc/rc.d
fi
cp rc.d/* ${STAGEDIR}/etc/rc.d/

exit 0
