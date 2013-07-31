#!/bin/sh
# ZFS functionality
# Args $1 = jail-name
# Args $2 = zfs directive
#######################################################################

# Source our functions
PROGDIR="/usr/local/share/lpreserver"

# Source our variables
. /usr/local/share/pcbsd/scripts/functions.sh
. ${PROGDIR}/backend/functions.sh

echo "Datasets scheduled for snapshots:"
echo "---------------------------------"
for i in `grep "${PROGDIR}/backend/runsnap.sh" /etc/crontab | awk '{print $8}'`
do
   min=`grep "${PROGDIR}/backend/runsnap.sh" /etc/crontab | awk '{print $1}'`
   hour=`grep "${PROGDIR}/backend/runsnap.sh" /etc/crontab | awk '{print $2}'`
   time="Min: $min Hour: $hour";
   if [ "$min" = "0" -a "$hour" = "01" ] ; then time="daily" ; fi
   if [ "$min" = "0" -a "$hour" = '*' ] ; then time="hourly" ; fi
   if [ "$min" = "0,30" ] ; then time="30min" ; fi
   if [ "$min" = '*/10' ] ; then time="10min" ; fi
   if [ "$min" = '*/5' ] ; then time="5min" ; fi
   echo "$i - $time"
done
