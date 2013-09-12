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
snapStat=0

if [ -z "${DATASET}" ]; then
  exit_err "No dataset specified!"
fi

# Make sure this is a valid DATASET
zfs list ${DATASET} >/dev/null 2>/dev/null
if [ $? -ne 0 ] ; then
   exit_err "Invalid dataset specified ${DATASET}"
fi

# Create the snapshot now with the "auto-" tag
echo_log "Creating snapshot on ${DATASET}"
mkZFSSnap "${DATASET}" "auto-"
if [ $? -ne 0 ] ; then
  echo_log "ERROR: Failed creating snapshot on ${DATASET}"
  queue_msg "ERROR: Failed creating snapshot on ${DATASET} @ `date`\n\r`cat $CMDLOG`"
  snapStat=1
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

# Before we start pruning, check if any replication is running
skipPrune=0
export pidFile="${DBDIR}/.reptask-`echo ${DATASET} | sed 's|/|-|g'`"
if [ -e "${pidFile}" ] ; then
   pgrep -F ${pidFile} >/dev/null 2>/dev/null
   if [ $? -eq 0 ] ; then skipPrune=1; fi
fi

if [ $skipPrune -eq 1 ] ; then
  # No pruning since replication is currently running
  echo_log "WARNING: Skipped pruning snapshots on ${DATASET} while replication is running."
  queue_msg "WARNING: Skipped pruning snapshots on ${DATASET} while replication is running."

else
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
          queue_msg "ERROR: Failed pruning snapshot $snap on ${DATASET} @ `date`\n\r`cat $CMDLOG`"
          snapStat=1
        else
          queue_msg "Success pruning snapshot $snap on ${DATASET} @ `date`\n\r`cat $CMDLOG`"
        fi
      fi
  done
fi

# If we failed at any point, sent out a notice
if [ $snapStat -ne 0 ] ; then
   email_msg "FAILED - Automated Snapshot" "`echo_queue_msg`"
fi

# If we are successful and user wants all notifications, send out a message
if [ $snapStat -eq 0 -a "$EMAILMODE" = "ALL" ] ; then
   email_msg "Success - Automated Snapshot" "`echo_queue_msg`"
fi

# Check if we need to run a replication task for this dataset
${PROGDIR}/backend/runrep.sh ${DATASET} sync
