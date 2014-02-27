#!/bin/sh
# Do the cron snapshot
######################################################################

# Set our vars
PROGDIR="/usr/local/share/lpreserver"

# Source our functions
. /usr/local/share/pcbsd/scripts/functions.sh
. ${PROGDIR}/backend/functions.sh

do_auto_prune() {
  echo_log "Pruning old snapshot: $2"
  rmZFSSnap "$1" "$2"
  if [ $? -ne 0 ] ; then
     echo_log "ERROR: Failed pruning snapshot $2 on ${1}"
     queue_msg "ERROR: Failed pruning snapshot $2 on ${1} @ `date`\n\r`cat $CMDLOG`"
     snapStat=1
  else
     queue_msg "Success pruning snapshot $2 on ${1} @ `date`\n\r`cat $CMDLOG`"
  fi
}

do_numeric_prune()
{

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
     if [ "$cur" != "auto" ] ; then continue; fi

     num=`expr $num + 1`
     if [ $num -gt $KEEP ] ; then
        do_auto_prune "$DATASET" "$snap"
     fi
  done
}

# Function to do automatic rotation / pruning
do_automatic_prune()
{
  curEpoc=`date +%s`
  lastYear=""; lastMon=""; lastDay=""; lastHour="" lastMin="" ; lastSec=""
  # Get our list of snaps
  snaps=$(snaplist "${DATASET}")

  # Reverse the list, sort from newest to oldest
  for tmp in $snaps
  do
     rSnaps="$tmp $rSnaps"
  done

  # Get the last replicated snapshot
  lastSEND=`zfs get -r backup:lpreserver ${LDATA} | grep LATEST | awk '{$1=$1}1' OFS=" " | tail -1 | cut -d '@' -f 2 | cut -d ' ' -f 1`

  num=0
  for snap in $rSnaps
  do
     # Skip snaps not created by life-preserver
     cur="`echo $snap | cut -d '-' -f 1`"
     if [ "$cur" != "auto" ] ; then continue; fi

     # If this snapshot is the last one replicated, lets skip pruning it for now
     if [ "$cur" = "$lastSEND" ]; then continue; fi

     sec="`echo $snap | cut -d '-' -f 7`"
     min="`echo $snap | cut -d '-' -f 6`"
     hour="`echo $snap | cut -d '-' -f 5`"
     day="`echo $snap | cut -d '-' -f 4`"
     mon="`echo $snap | cut -d '-' -f 3`"
     year="`echo $snap | cut -d '-' -f 2`"

     # Convert this snap to epoc time
     snapEpoc=`date -j -f "%Y %m %d %H %M %S" "$year $mon $day $hour $min $sec" "+%s"`

     # Get the epoch time elapsed
     check=`expr $curEpoc - $snapEpoc`
     pruned=0

     # Looking for snaps older than 12 months
     if [ $check -gt 31536000 ]; then
        do_auto_prune "$DATASET" "$snap"
        pruned=1
     fi

     # Looking for multiple snaps older than 30 days
     if [ $check -gt 2592000 -a $pruned -eq 0 ]; then
        # Did we already have a snapshot from this month?
        if [ "$year" = "$lastYear" -a "$mon" = "$lastMon" ] ; then
          do_auto_prune "$DATASET" "$snap"
          pruned=1
        fi
     fi

     # Looking for multiple snaps older than a day
     if [ $check -gt 86400 -a $pruned -eq 0 ]; then
        if [ "$year" = "$lastYear" -a "$mon" = "$lastMon" -a "$day" = "$lastDay" ] ; then
          do_auto_prune "$DATASET" "$snap"
          pruned=1
        fi
     fi

     # Looking for multiple snaps older than an hour
     if [ $check -gt 3600 -a $pruned -eq 0 ]; then
        if [ "$year" = "$lastYear" -a "$mon" = "$lastMon" -a "$day" = "$lastDay" -a "$hour" = "$lastHour" ] ; then
          do_auto_prune "$DATASET" "$snap"
          pruned=1
        fi
     fi

     # Save values of this snapshot for next pass
     lastYear="$year" ; lastMon="$mon" ; lastDay="$day" ; lastHour="$hour"
     lastMin="$min" ; lastSec="$sec"
  done

}

# Now run the main script
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
  # Safe to do the pruning, no replication is in progress
  if [ "$KEEP" = "auto" ] ; then
     do_automatic_prune
  else
     do_numeric_prune
  fi
fi

# If we failed at any point, sent out a notice
if [ $snapStat -ne 0 ] ; then
   email_msg "FAILED - Automated Snapshot" "`echo_queue_msg`"
fi

# If we are successful and user wants all notifications, send out a message
if [ $snapStat -eq 0 -a "$EMAILMODE" = "ALL" ] ; then
   email_msg "Success - Automated Snapshot" "`echo_queue_msg`"
else
   rm ${MSGQUEUE} 2>/dev/null
fi

# Check if we need to run a replication task for this dataset
${PROGDIR}/backend/runrep.sh ${DATASET} sync
