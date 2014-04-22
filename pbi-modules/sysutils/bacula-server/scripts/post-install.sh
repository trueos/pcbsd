#!/bin/sh

#Make the /var/db/bacula directory
mkdir /var/db/bacula

#Copy the *.conf.sample files as *.conf
cd ${PBI_PROGDIRPATH}/etc
for i in `ls *.conf.sample | cut -f 1 -d .`
do
   cp ${i}.conf.sample ${i}.conf
done
