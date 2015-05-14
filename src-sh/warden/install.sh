#!/bin/sh
# Warden install script

DEFAULT="/usr/local"

if [ -z "$1" ] ; then
        LB="${DEFAULT}"
else
        LB="${1}"
fi

PROGDIR="${LB}/share/warden"

mkdir -p ${PROGDIR} >/dev/null 2>/dev/null

DIR=`dirname $0`
cd ${DIR}

cp bin/warden ${LB}/bin/warden
chmod 755 ${LB}/bin/warden

cp conf/warden.conf ${LB}/etc/warden.conf.dist
chmod 644 ${LB}/etc/warden.conf.dist

if [ -d "${PROGDIR}/linux-installs" ] ; then
  rm -rf ${PROGDIR}/linux-installs
fi
cp -r linux-installs ${PROGDIR}

if [ -d "${PROGDIR}/scripts" ] ; then
  rm -rf ${PROGDIR}/scripts
fi

cp -r scripts ${PROGDIR}
chmod 755 ${PROGDIR}/scripts/backend/*

# Setup rc.d
cp scripts/rc.d/wardenrc ${LB}/etc/rc.d/wardenrc
chmod 755 ${LB}/etc/rc.d/wardenrc

cp agent.png ${PROGDIR}/
cp agent.png ${PROGDIR}/warden.png

exit 0
