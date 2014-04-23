#!/bin/sh

sed -i.bak "s,/usr/bin/perl,${PBI_PROGDIRPATH}/bin/perl,g" ${PBI_PROGDIRPATH}/sbin/munin-node

#Make the log file directory
if [ ! -d /var/log/munin ]; then
   echo "Creating directory: /var/log/munin/" 
   mkdir /var/log/munin/
fi
