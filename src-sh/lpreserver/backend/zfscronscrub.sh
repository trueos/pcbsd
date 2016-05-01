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
POOL="${2}"

if [ "$ACTION" != "list" -a -z "${POOL}" ]; then
  exit_err "No pool specified!"
fi

if [ "$ACTION" = "start" ] ; then

  TIME="${3}"

  # See if the user is running numbered anacron
  echo $TIME | grep -q '^[1-9][0-9]*$'
  if [ $? -eq 0 ] ; then
     nth_day="`echo $TIME`"
     TIME="anacron"
  fi

  # See if the user is running daily anacron
  echo $TIME | grep -q '^daily$'
  if [ $? -eq 0 ] ; then
     TIME="anacron"
     nth_day="1"
  fi

  # See if the user is running daily with a hour specified
  echo $TIME | grep -q '^daily@'
  if [ $? -eq 0 ] ; then
     hour="`echo $TIME | cut -d '@' -f 2`"
     if [ -z "$hour" ] ; then hour="22" ; fi
     TIME="daily"
  fi

  # See if the user is running weekly anacron
  echo $TIME | grep -q '^weekly$'
  if [ $? -eq 0 ] ; then
     TIME="anacron"
     nth_day="7"
  fi

  # See if the user is running weekly with a day and hour specified
  # 0 = Sun, Sat = 6
  echo $TIME | grep -q '^weekly@'
  if [ $? -eq 0 ] ; then
     day="`echo $TIME | cut -d '@' -f 2`"
     hour="`echo $TIME | cut -d '@' -f 3`"
     if [ -z "$day" ] ; then day="7" ; fi
     if [ -z "$hour" ] ; then hour="22" ; fi
     TIME="weekly"
  fi

  # See if the user is running monthly anacron
  echo $TIME | grep -q '^monthly$'
  if [ $? -eq 0 ] ; then
     TIME="anacron"
     nth_day="30"
  fi

  # See if the user is running monthly with a day and hour specified
  echo $TIME | grep -q '^monthly@'
  if [ $? -eq 0 ] ; then
     day="`echo $TIME | cut -d '@' -f 2`"
     hour="`echo $TIME | cut -d '@' -f 3`"
     if [ -z "$day" ] ; then day="1" ; fi
     if [ -z "$hour" ] ; then hour="22" ; fi
     TIME="monthly"
  fi

  case $TIME in
    daily)
      if [ ! $(is_num "$hour") ] || [ "$hour" -lt 1 ] || [ "$hour" -gt 23 ] ; then
         echo "ERROR: Invalid hour specified!" ; exit 5
      fi
      enable_cron_scrub "$POOL" "$TIME" "$hour"
      echo "Scrub schedule started for zpool: $POOL"
      echo "Scrub frequency set: $TIME @ hour $hour"
      ;;
    weekly)
      if [ ! $(is_num "$hour") ] || [ "$hour" -lt 1 ] || [ "$hour" -gt 23  ] ; then
         echo "ERROR: Invalid hour specified!" ; exit 5
      fi
      if [ ! $(is_num "$day") ] || [ "$day" -lt 1 ] || [ "$day" -gt 7  ] ; then
         echo "ERROR: Invalid day specified!" ; exit 5
      fi
      enable_cron_scrub "$POOL" "$TIME" "$day" "$hour"
      echo "Scrub schedule started for zpool: $POOL"
      echo "Scrub frequency set: $TIME @ day $day @ hour $hour"
      ;;
    monthly)
      if [ ! $(is_num "$hour") ] || [ "$hour" -lt 1 ] || [ "$hour" -gt 23  ] ; then
         echo "ERROR: Invalid hour specified!" ; exit 5
      fi
      if [ ! $(is_num "$day") ] || [ "$day" -lt 1 ] || [ "$day" -gt 28  ] ; then
         echo "ERROR: Invalid day specified!" ; exit 5
      fi
      enable_cron_scrub "$POOL" "$TIME" "$day" "$hour"
      echo "Scrub schedule started for zpool: $POOL"
      echo "Scrub frequency set: $TIME @ day $day @ hour $hour"
      ;;
    anacron)
      if [ ! $(is_num "$nth_day") ] || [ "$nth_day" -lt 1 ] ; then
         echo "ERROR: Invalid number of days specified!" ; exit 5
      fi
      enable_cron_scrub "$POOL" "$TIME" "$nth_day"
      echo "Scrub schedule started for zpool: $POOL"
      echo "Scrub frequency set: Every $nth_day days"
      ;;
    *) echo "ERROR: Invalid frequency specified!" ; exit 5 ;;
  esac

  exit 0
fi

if [ "$ACTION" = "stop" ] ; then
  enable_cron_scrub "$POOL" "OFF"
  echo "Scrub schedule stopped for zpool: $POOL"
  exit 0
fi

if [ "$ACTION" = "list" ] ; then
  list_cron_scrub "${POOL}"
  exit 0
fi

