#!/bin/sh

echo "TOTALSTEPS: 3"
echo "SETSETPS: 1"
echo "MSG: Configurating"

mkdir -p ${SYS_LOCALBASE}/etc/squid >/dev/null 2>/dev/null

SQUIDCONF="squid.conf cachemgr.conf mime.conf msntauth.conf"
for i in ${SQUIDCONF}
do
  if [ ! -e "${SYS_LOCALBASE}/etc/squid/${i}" ]
  then
    cp ${PBI_PROGDIRPATH}/etc/squid/${i}.default ${SYS_LOCALBASE}/etc/squid/${i}
    if [ "$i" = "squid.conf" ]
    then
      echo "visible_hostname localhost" >> ${SYS_LOCALBASE}/etc/squid/${i}
    fi
  fi
  ln -s ${SYS_LOCALBASE}/etc/squid/${i} ${PBI_PROGDIRPATH}/etc/squid/${i}
done


ln -fs ${PBI_PROGDIRPATH}/etc/rc.d/squid ${PBI_RCDIR}/squid

echo "SETSETPS: 2"
echo "squid_enable=\"YES\"" >> /etc/rc.conf

mkdir -p ${PBI_PROGDIRPATH}/squid/logs >/dev/null 2>/dev/null
mkdir -p ${PBI_PROGDIRPATH}/squid/cache >/dev/null 2>/dev/null
chown -R squid:squid ${PBI_PROGDIRPATH}/squid



echo "SETSETPS: 3"
echo "MSG: Starting squid service"
${PBI_PROGDIRPATH}/sbin/squid -z

${PBI_RCDIR}/squid start

echo "MSG: Finished!"

