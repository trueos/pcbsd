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

POOL="${1}"
ACTION="${2}"

if [ -z "${POOL}" ]; then
  exit_err "No pool specified!"
fi

if [ "$ACTION" = "start" ] ; then

  TIME="${3}"

  # See if the user is running daily with a hour specified
  echo $TIME | grep -q "daily"
  if [ $? -eq 0 ] ; then
     hour="`echo $TIME | cut -d '@' -f 2`"
     if [ -z "$hour" ] ; then hour="22" ; fi
     TIME="daily"
  fi

  # See if the user is running weekly with a day and hour specified
  # 0 = Sun, Sat = 6
  echo $TIME | grep -q "weekly"
  if [ $? -eq 0 ] ; then
     day="`echo $TIME | cut -d '@' -f 2`"
     hour="`echo $TIME | cut -d '@' -f 3`"
     if [ -z "$day" ] ; then day="7" ; fi
     if [ -z "$hour" ] ; then hour="22" ; fi
     TIME="weekly"
  fi

  # See if the user is running monthly with a day and hour specified
  echo $TIME | grep -q "monthly"
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
      ;;
    weekly)
      if [ ! $(is_num "$hour") ] || [ "$hour" -lt 1 ] || [ "$hour" -gt 23  ] ; then
         echo "ERROR: Invalid hour specified!" ; exit 5
      fi
      if [ ! $(is_num "$day") ] || [ "$day" -lt 1 ] || [ "$day" -gt 7  ] ; then
         echo "ERROR: Invalid day specified!" ; exit 5
      fi
      ;;
    monthly)
      if [ ! $(is_num "$hour") ] || [ "$hour" -lt 1 ] || [ "$hour" -gt 23  ] ; then
         echo "ERROR: Invalid hour specified!" ; exit 5
      fi
      if [ ! $(is_num "$day") ] || [ "$day" -lt 1 ] || [ "$day" -gt 28  ] ; then
         echo "ERROR: Invalid day specified!" ; exit 5
      fi
      ;;
    *) echo "ERROR: Invalid frequency specified!" ; exit 5 ;;
  esac

  if [ -z "$day" ] ; then 
     enable_cron_scrub "$POOL" "$TIME" "$hour"
     echo "Scrub frequency set: $TIME @ hour $hour"
  fi
  if [ -n "$day" ] ; then 
     enable_cron_scrub "$POOL" "$TIME" "$day" "$hour"
     echo "Scrub frequency set: $TIME @ day $day @ hour $hour"
  fi
  exit 0
fi

if [ "$ACTION" = "stop" ] ; then
  enable_cron_scrub "$DATASET" "OFF"
  exit 0
fi

