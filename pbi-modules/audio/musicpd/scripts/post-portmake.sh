#!/bin/sh

sed -i.bak "s,/bin/musicpd,/${PBI_FAKEBINDIR}/musicpd,g"${PBI_PROGDIRPATH}/etc/rc.d/musicpd
chmod 755 ${PBI_PROGDIRPATH}/bin/musicpd

sed -i.bak "s,LOGIN,LOGIN DAEMON snddetect,g" ${PBI_PROGDIRPATH}/etc/rc.d/musicpd
