#!/bin/sh
# Do the cron snapshot
######################################################################

# Set our vars
PROGDIR="/usr/local/share/lpreserver"

# Source our functions
. /usr/local/share/pcbsd/scripts/functions.sh
. ${PROGDIR}/backend/functions.sh

DATASET="${1}"
KEEP="${2}"

if [ -z "${DATASET}" ]; then
  exit_err "No dataset specified!"
fi

# Create the snapshot now with the "auto-" tag
echo_log "Creating snapshot on ${DATASET}"
mkZFSSnap "${DATASET}" "auto-"
if [ $? -ne 0 ] ; then
  echo_log "ERROR: Failed creating snapshot on ${DATASET}"
  email_msg "Snapshot ERROR" "ERROR: Failed creating snapshot on ${DATASET} @ `date`\n\r`cat $CMDLOG`"
else
  queue_msg "Success creating snapshot on ${DATASET} @ `date`\n\r`cat $CMDLOG`"
fi

# Get our list of snaps
snaps=$(snaplist "${DATASET}")

# Reverse the list
for tmp in $snaps
do
   rSnaps="$tmp $rSnaps"
done

# Do any pruning
num=0
for snap in $rSnaps
do
   # Only remove snapshots which are auto-created, so we don't delete one the user
   # made specifically
   cur="`echo $snap | cut -d '-' -f 1`" 
   if [ "$cur" != "auto" ] ; then
     continue;
   fi

   num=`expr $num + 1`
   if [ $num -gt $KEEP ] ; then
      echo_log "Pruning old snapshot: $snap"
      rmZFSSnap "${DATASET}" "$snap"
      if [ $? -ne 0 ] ; then
        echo_log "ERROR: Failed pruning snapshot $snap on ${DATASET}"
        email_msg "Snapshot ERROR" "ERROR: Failed pruning snapshot $snap on ${DATASET} @ `date`\n\r`cat $CMDLOG`"
      else
        queue_msg "Success pruning snapshot $snap on ${DATASET} @ `date`\n\r`cat $CMDLOG`"
      fi
    fi
done

if [ "$EMAILMODE" = "ALL" ] ; then
   email_msg "Automated Snapshot" "`echo_queue_msg`"
fi
