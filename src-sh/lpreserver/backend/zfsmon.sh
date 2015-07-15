#!/bin/sh
# Do the cron snapshot
# Modified: 7/14/2015 by Joshua Smith
######################################################################

# Set our vars
PROGDIR="/usr/local/share/lpreserver"

# Source our functions
. /usr/local/share/pcbsd/scripts/functions.sh
. ${PROGDIR}/backend/functions.sh

# Lets do a health-check on the zpool
ZSTATUS="`zpool status -x`"
if [ "$ZSTATUS" != "all pools are healthy" ] ; then
  echo $ZSTATUS | grep -q "Expect reduced performance."
  if [ $? -ne 0 ] ; then
    email_msg "zpool issue" "The zpool command reports an issue on the system:\n\r $ZSTATUS"
    echo "$ZSTATUS" > $DBDIR/zpool-alert
  else
    if [ -d "${DBDIR}/zpool-alert" ] ; then
       rm ${DBDIR}/zpool-alert
    fi
  fi
else
  # Cleanup any old alert files
  if [ -d "${DBDIR}/zpool-alert" ] ; then
     rm ${DBDIR}/zpool-alert
  fi
fi

if [ "$EMAILMODE" = "ERROR" ] ; then
  exit 0
fi

haveMsg=0

# Remove the old du alert flag
if [ -e "${DBDIR}/zpool-alert-du" ] ; then rm ${DBDIR}/zpool-alert-du ; fi

# Do the disk-space check
for zpool in `zpool list -H | awk '{print $1}'`
do
  poolCap="`zpool list -H -o capacity $zpool | cut -d '%' -f 1`"

  if [ $poolCap -gt $DUWARN ] ; then
     queue_msg "Warning! ${zpool} is currently at ${poolCap}% capacity!"
     echo "$poolCap" > $DBDIR/zpool-alert-du
     haveMsg=1
  fi

  # See if we can do any auto-cleanup of this pool
  if [ $poolCap -gt 75 ] ; then
     do_pool_cleanup "$zpool"
  fi

done

# Lets check free disk space on the pools
if [ $haveMsg -eq 1 ] ; then
   email_msg "zpool warning" "`echo_queue_msg`"
fi

