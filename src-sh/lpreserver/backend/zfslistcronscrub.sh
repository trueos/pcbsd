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
   day=`grep "${PROGDIR}/backend/runscrub.sh ${i}" /etc/crontab | awk '{print $5}'`
   month=`grep "${PROGDIR}/backend/runscrub.sh ${i}" /etc/crontab | awk '{print $3}'`
   if [ "$day" = '*' -a "$month" = '*' ] ; then time="daily @ hour $hour" ; fi
   if [ "$day" != '*' ] ; then time="weekly @ day $day" ; fi
   if [ "$month" != '*' ] ; then time="monthly @ day $day @ hour $hour" ; fi
   echo "$i - $time"
done

#monthly @ day 21 @ hour 13
#0       22      *       *       *       root    /usr/local/share/lpreserver/backend/runscrub.sh tank
#0       22      *       *       7       root    /usr/local/share/lpreserver/backend/runscrub.sh tank
#0       13      21       *       *      root    /usr/local/share/lpreserver/backend/runscrub.sh tank

