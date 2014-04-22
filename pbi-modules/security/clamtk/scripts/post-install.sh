#!/bin/sh
#fix the perl header
sed -i.bak "s,/usr/bin/perl,${PBI_PROGDIRPATH}/bin/perl,g" ${PBI_PROGDIRPATH}/bin/clamtk

#Now setup the clamav directories
mkdir /var/log/clamav
mkdir /var/db/clamav

chown clamav:clamav /var/log/clamav
chown clamav:clamav /var/db/clamav

