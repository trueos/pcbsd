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

ACTION="${1}"
DATASET="${2}"

if [ "$ACTION" != "list" -a -z "${DATASET}" ]; then
  exit_err "No dataset specified!"
fi

if [ "$ACTION" = "start" ] ; then

  TIME="${3}"

  if [ "$TIME" = "auto" ] ; then
     enable_cron_snap "$DATASET" "auto" "auto" "auto"
     echo "Snapshot schedule started for dataset: $DATASET"
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
  echo "Snapshot schedule started for dataset: $DATASET"
  echo "Snapshot frequency set: $TIME @ $hour"
  echo "Snapshot # to keep set: $COUNT"
  exit 0
fi

if [ "$ACTION" = "exclude" ] ; then
  # Shift the arguments so we only have dataset excludes left in $@
  shift 2
  add_exclude "${DATASET}" "snap" "${@}"
fi

if [ "$ACTION" = "rmexclude" ] ; then
  # Shift the arguments so we only have dataset exclude removals left in $@
  shift 2
  remove_exclude "${DATASET}" "snap" "${@}"
fi


if [ "$ACTION" = "listexclude" ] ; then
  list_exclude "${DATASET}" "snap"
fi

if [ "$ACTION" = "stop" ] ; then
  enable_cron_snap "$DATASET" "OFF"
  echo "Snapshot schedule stopped for dataset: $DATASET"
  exit 0
fi

if [ "$ACTION" = "list" ] ; then
  list_cron_snap "${DATASET}"
  exit 0
fi
