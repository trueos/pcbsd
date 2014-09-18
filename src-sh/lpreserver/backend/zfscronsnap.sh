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
ACTION="${2}"

if [ -z "${DATASET}" ]; then
  exit_err "No dataset specified!"
fi

if [ "$ACTION" = "start" ] ; then

  TIME="${3}"

  if [ "$TIME" = "auto" ] ; then
     enable_cron_snap "$DATASET" "auto" "auto" "auto"
     echo "Snapshot frequency set: Automatic"
     exit 0
  fi

  # See if the user is running daily with a hour specified
  echo $TIME | grep -q "daily"
  if [ $? -eq 0 ] ; then
     hour="`echo $TIME | cut -d '@' -f 2`"
     if [ -z "$hour" ] ; then hour="22" ; fi
     TIME="daily"
  fi

  COUNT="${4}"
  case $TIME in
  daily|hourly|30min|10min|5min) ;;
	*) echo "ERROR: Invalid frequency specified!" ; exit 5 ;;
  esac
  if [ ! $(is_num "$COUNT") ] ; then
     echo "ERROR: Invalid count specified!" ; exit 5
  fi

  enable_cron_snap "$DATASET" "$TIME" "$COUNT" "$hour"
  echo "Snapshot frequency set: $TIME @ $hour"
  echo "Snapshot # to keep set: $COUNT"
  exit 0
fi

if [ "$ACTION" = "stop" ] ; then
  enable_cron_snap "$DATASET" "OFF"
  exit 0
fi

