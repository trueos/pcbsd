#!/bin/sh

if [ -z "$1" ] ; then
   STAGEDIR="/usr/local"
else
   STAGEDIR="$1"
fi

PROGDIR="${STAGEDIR}/share/appcafe"
rm -rf ${PROGDIR} >/dev/null 2>/dev/null
mkdir -p ${PROGDIR} >/dev/null 2>/dev/null

DIR=`dirname $0`
cd ${DIR}

# Copy dispatcher script
cp dispatcher ${PROGDIR}/dispatcher
chmod 755 ${PROGDIR}/dispatcher
cp dispatcher ${PROGDIR}/dispatcher-auth
chmod 755 ${PROGDIR}/dispatcher-auth

# Copy sudoers file
if [ ! -d "$STAGEDIR/etc/sudoers.d" ] ; then
   mkdir -p ${STAGEDIR}/etc/sudoers.d
fi
cp appcafe-sudoers ${STAGEDIR}/etc/sudoers.d/
chmod 644 ${STAGEDIR}/etc/sudoers.d/appcafe-sudoers

cp -r share/* ${PROGDIR}/

if [ ! -d "${STAGEDIR}/etc/rc.d" ] ; then
   mkdir ${STAEGDIR}/etc/rc.d
fi
cp rc.d/* ${STAGEDIR}/etc/rc.d/

# Copy over the conf file
cp etc/appcafe.conf.dist ${STAGEDIR}/etc/appcafe.conf.dist

# Copy over the setpass util
if [ ! -d "${STAGEDIR}/bin" ] ; then
   mkdir ${STAEGDIR}/bin
fi
cp appcafe-setpass ${STAGEDIR}/bin/
chmod 755 ${STAGEDIR}/bin/appcafe-setpass

exit 0
