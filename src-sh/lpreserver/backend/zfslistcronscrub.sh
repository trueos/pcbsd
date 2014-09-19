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

echo "Scrub scheduled for pools:"
echo "---------------------------------"
for i in `grep "${PROGDIR}/backend/runscrub.sh" /etc/crontab | awk '{print $8}'`
do
   hour=`grep "${PROGDIR}/backend/runscrub.sh ${i}" /etc/crontab | awk '{print $2}'`
   day_week=`grep "${PROGDIR}/backend/runscrub.sh ${i}" /etc/crontab | awk '{print $5}'`
   day_month=`grep "${PROGDIR}/backend/runscrub.sh ${i}" /etc/crontab | awk '{print $3}'`
   if [ "$housr" != '*' -a "$day_week" = '*' -a "$day_month" = '*' ] ; then time="daily @ hour $hour" ; fi
   if [ "$day_week" != '*' ] ; then time="weekly @ day $day_week" ; fi
   if [ "$month" != '*' ] ; then time="monthly @ day $day_month @ hour $hour" ; fi
   echo "$i - $time"
done
