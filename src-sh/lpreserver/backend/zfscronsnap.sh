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
  COUNT="${4}"
  case $TIME in
  daily|hourly|30min|10min|5min) ;;
	*) echo "ERROR: Invalid frequency specified!" ; exit 5 ;;
  esac
  if [ ! $(is_num "$COUNT") ] ; then
     echo "ERROR: Invalid count specified!" ; exit 5
  fi

  enable_cron "$DATASET" "$TIME" "$COUNT"
  echo "Snapshot frequency set: $TIME"
  echo "Snapshot # to keep set: $COUNT"
  exit 0
fi

if [ "$ACTION" = "stop" ] ; then
  enable_cron "$DATASET" "OFF"
  exit 0
fi

