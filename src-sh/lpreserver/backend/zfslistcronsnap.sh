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
   min=`grep "${PROGDIR}/backend/runsnap.sh ${i}" /etc/crontab | awk '{print $1}'`
   hour=`grep "${PROGDIR}/backend/runsnap.sh ${i}" /etc/crontab | awk '{print $2}'`
   count=`grep "${PROGDIR}/backend/runsnap.sh ${i}" /etc/crontab | awk '{print $9}'`
   time="Min: $min Hour: $hour";
   if [ "$min" = "0" -a "$hour" != '*' ] ; then time="daily@$hour" ; fi
   if [ "$min" = "0" -a "$hour" = '*' ] ; then time="hourly" ; fi
   if [ "$min" = "0,30" ] ; then time="30min" ; fi
   if [ "$min" = '*/10' ] ; then time="10min" ; fi
   if [ "$min" = '*/5' ] ; then time="5min" ; fi
   if [ "$min" = '*/5' -a "$count" = "auto" ] ; then time="Automatic" ; fi
   echo "$i - $time - total: $count"
done
