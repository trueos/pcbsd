#!/bin/sh
# Do the cron scrub
######################################################################

# Set our vars
PROGDIR="/usr/local/share/lpreserver"

# Source our functions
. /usr/local/share/pcbsd/scripts/functions.sh
. ${PROGDIR}/backend/functions.sh

do_scrub() {
  echo "Starting scrub on $1"
  echo_log "Starting scrub on $1"
  zpool scrub "$1"
  if [ $? -ne 0 ] ; then
     echo_log "ERROR: Failed starting scrub on $1"
     queue_msg "ERROR: Failed starting scrub on $1 @ `date`\n\r`cat $CMDLOG`"
     scrubStat=1
  else
     queue_msg "Success starting scrub on $1 @ `date`\n\r`cat $CMDLOG`"
  fi
}

# Now run the main script
POOL="${1}"
scrubStat=0

if [ -z "${POOL}" ]; then
  exit_err "No pool specified!"
fi

# Make sure this is a valid POOL
zpool list ${POOL} >/dev/null 2>/dev/null
if [ $? -ne 0 ] ; then
   exit_err "Invalid pool specified ${POOL}"
fi

# Start the scrub
do_scrub "$POOL"

# If we failed at any point, sent out a notice
if [ $scrubStat -ne 0 ] ; then
   email_msg "FAILED - Automated Scrub" "`echo_queue_msg`"
fi

# If we are successful and user wants all notifications, send out a message
if [ $scrubStat -eq 0 -a "$EMAILMODE" = "ALL" ] ; then
   email_msg "Success - Automated Scrub" "`echo_queue_msg`"
else
   rm ${MSGQUEUE} 2>/dev/null
fi
