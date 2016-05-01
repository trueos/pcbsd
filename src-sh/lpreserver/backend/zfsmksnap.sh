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

DATASET="${1}"

if [ -z "${DATASET}" ]; then
  exit_err "No dataset specified!"
fi

SNAPNAME="${2}"
if [ -z "${SNAPNAME}" ]; then
  exit_err "No snapshot name specified!"
fi

COMMENT="$3"

# Make the snapshot now
snapStat=0

echo_log "Creating snapshot on ${DATASET}"
mkZFSSnap "${DATASET}" "${SNAPNAME}" "$COMMENT"
if [ $? -ne 0 ] ; then
  echo_log "ERROR: Failed creating snapshot on ${DATASET}"
  queue_msg "Snapshot ERROR" "ERROR: Failed creating snapshot on ${DATASET} @ `date`\n\r`cat $CMDLOG`"
  snapStat=1
else
  queue_msg "Success creating snapshot on ${DATASET} @ `date`\n\r`cat $CMDLOG`"
fi

# If we failed at any point, sent out a notice
if [ $snapStat -ne 0 ] ; then
   email_msg "FAILED - Manual Snapshot" "`echo_queue_msg`"
fi

# If we are successful and user wants all notifications, send out a message
if [ $snapStat -eq 0 -a "$EMAILMODE" = "ALL" ] ; then
   email_msg "Success - Manual Snapshot" "`echo_queue_msg`"
fi
