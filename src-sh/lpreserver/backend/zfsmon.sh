#!/bin/sh
# Do the cron snapshot
######################################################################

# Set our vars
PROGDIR="/usr/local/share/lpreserver"

# Source our functions
. /usr/local/share/pcbsd/scripts/functions.sh
. ${PROGDIR}/backend/functions.sh

# Lets do a health-check on the zpool
ZSTATUS="`zpool status -x`"
if [ "$ZSTATUS" != "all pools are healthy" ] ; then
  email_msg "zpool issue" "The zpool command reports an issue on the system:\n\r $ZSTATUS"
  echo "$ZSTATUS" > $DBDIR/zpool-alert
else
  # Cleanup any old alert files
  if [ -d "${DBDIR}/zpool-alert" ] ; then rm ${DBDIR}/zpool-alert; fi
fi

if [ "$EMAILMODE" = "ERROR" ] ; then exit 0 ; fi

haveMsg=0

# Remove the old du alert flag
if [ -e "${DBDIR}/zpool-alert-du" ] ; then rm ${DBDIR}/zpool-alert-du ; fi

zpool list  | grep -v "^NAME" > /tmp/.lPreserver-df.$$
while read line
do
   cap=`echo $line | awk '{print $5}' | cut -d '%' -f 1`
   zpool=`echo $line | awk '{print $1}'`
   if [ $(is_num "$cap") ] ; then
      if [ $cap -gt $DUWARN ] ; then
 	 queue_msg "Warning! ${zpool} is currently at ${cap}% capacity!"	
         echo "$cap" > $DBDIR/zpool-alert-du
         haveMsg=1
      fi
   fi
done< /tmp/.lPreserver-df.$$
rm /tmp/.lPreserver-df.$$

# Lets check free disk space on the pools
if [ $haveMsg -eq 1 ] ; then
   email_msg "zpool warning" "`echo_queue_msg`"
fi

